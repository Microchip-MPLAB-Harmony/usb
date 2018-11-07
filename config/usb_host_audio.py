def onAttachmentConnected(source, target):
	ownerComponent = source["component"]
	print("USB Host Layer Connected")
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		Database.clearSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.setSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER", readValue + 1 , 2)
	
def onAttachmentDisconnected(source, target):
	ownerComponent = source["component"]
	print("USB Host Layer Disconnected")
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		Database.clearSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.setSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER", readValue - 1 , 2)
		
def destroyComponent(component):	
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		Database.clearSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.setSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER", readValue - 1 , 2)		
		
def instantiateComponent(usbHostAudioComponent):

	res = Database.activateComponents(["usb_host"])
	
	# USB Host Audio client driver instances 
	usbHostAudioClientDriverInstance = usbHostAudioComponent.createIntegerSymbol("CONFIG_USB_HOST_AUDIO_NUMBER_OF_INSTANCES", None)
	usbHostAudioClientDriverInstance.setLabel("Number of Audio Host Driver Instances")
	usbHostAudioClientDriverInstance.setDescription("Enter the number of Audio Class Driver instances required in the application.")
	usbHostAudioClientDriverInstance.setDefaultValue(1)
	usbHostAudioClientDriverInstance.setVisible(True)

	
	#USB Host Audio Attach Listeners Number 
	# usbHostAudioClientDriverAttachListnerNumber = usbHostAudioComponent.createIntegerSymbol("CONFIG_USB_HOST_AUDIO_ATTACH_LISTENERS_NUMBER", None)
	# usbHostAudioClientDriverAttachListnerNumber.setLabel("Number of Audio Host Attach Listeners")
	# usbHostAudioClientDriverAttachListnerNumber.setDescription("Enter the number of Audio Attach Listeners required in the application.")
	# usbHostAudioClientDriverAttachListnerNumber.setDefaultValue(1)
	# usbHostAudioClientDriverAttachListnerNumber.setVisible(True)

		
	##############################################################
	# system_definitions.h file for USB Host Audio Client driver   
	##############################################################
	usbHostAudioSystemDefFile = usbHostAudioComponent.createFileSymbol(None, None)
	usbHostAudioSystemDefFile.setType("STRING")
	usbHostAudioSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbHostAudioSystemDefFile.setSourcePath("templates/host/system_definitions.h.host_audio_includes.ftl")
	usbHostAudioSystemDefFile.setMarkup(True)
	
	##############################################################
	# system_config.h file for USB Host Audio Client driver   
	##############################################################
	usbHostAudioSystemConfigFile = usbHostAudioComponent.createFileSymbol(None, None)
	usbHostAudioSystemConfigFile.setType("STRING")
	usbHostAudioSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbHostAudioSystemConfigFile.setSourcePath("templates/host/system_config.h.host_audio.ftl")
	usbHostAudioSystemConfigFile.setMarkup(True)
	
	##############################################################
	# TPL Entry for Audio client driver 
	##############################################################
	usbHostAudioTplEntryFile = usbHostAudioComponent.createFileSymbol(None, None)
	usbHostAudioTplEntryFile.setType("STRING")
	usbHostAudioTplEntryFile.setOutputName("usb_host.LIST_USB_HOST_TPL_ENTRY")
	usbHostAudioTplEntryFile.setSourcePath("templates/host/system_init_c_audio_tpl.ftl")
	usbHostAudioTplEntryFile.setMarkup(True)
	
	################################################
	# USB Host Audio Client driver Files 
	################################################
	usbHostAudioHeaderFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_host_audio_v1_0.h', usbHostAudioComponent, usbHostAudioHeaderFile, "", "/usb/", True, None)
	
	usbAudioHeaderFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_audio_v1_0.h', usbHostAudioComponent, usbAudioHeaderFile, "", "/usb/", True, None)
	
	usbHostAudioSourceFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_host_audio_v1_0.c', usbHostAudioComponent, usbHostAudioSourceFile, "src/", "/usb/src/", True, None)
		
	usbHostAudioLocalHeaderFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_host_audio_local.h', usbHostAudioComponent, usbHostAudioLocalHeaderFile, "src/", "/usb/src", True, None)
	
	usbHostAudioLocalHeaderFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_host_audio_v1_mapping', usbHostAudioComponent, usbHostAudioLocalHeaderFile, "src/", "/usb/src", True, None)
	
	
	# all files go into src/
def addFileName(fileName, component, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
	#filename = component.createFileSymbol(None, None)
	symbol.setProjectPath("config/" + configName1 + destPath)
	symbol.setSourcePath(srcPath + fileName)
	symbol.setOutputName(fileName)
	symbol.setDestPath(destPath)
	if fileName[-2:] == '.h':
		symbol.setType("HEADER")
	else:
		symbol.setType("SOURCE")
	symbol.setEnabled(enabled)
	if callback != None:
		symbol.setDependencies(callback, ["USB_DEVICE_FUNCTION_1_DEVICE_CLASS"])