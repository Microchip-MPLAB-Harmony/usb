def onDependentComponentAdded(ownerComponent, dependencyID, dependentComponent):
	print(ownerComponent)
	if (dependencyID == "usb_host_dependency"):
		readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.clearSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.setSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER", readValue + 1 , 2)
		
def instantiateComponent(usbHostCdcComponent):

	# USB Host CDC client driver instances 
	usbHostCdcClientDriverInstance = usbHostCdcComponent.createIntegerSymbol("CONFIG_USB_HOST_CDC_NUMBER_OF_INSTANCES", None)
	usbHostCdcClientDriverInstance.setLabel("Number of CDC Host Driver Instances")
	usbHostCdcClientDriverInstance.setDescription("Enter the number of CDC Class Driver instances required in the application.")
	usbHostCdcClientDriverInstance.setDefaultValue(1)
	usbHostCdcClientDriverInstance.setVisible(True)
	
	
	#USB Host CDC Attach Listeners Number 
	usbHostCdcClientDriverAttachListnerNumber = usbHostCdcComponent.createIntegerSymbol("CONFIG_USB_HOST_CDC_ATTACH_LISTENERS_NUMBER", None)
	usbHostCdcClientDriverAttachListnerNumber.setLabel("Number of CDC Host Attach Listeners")
	usbHostCdcClientDriverAttachListnerNumber.setDescription("Enter the number of CDC Attach Listeners required in the application.")
	usbHostCdcClientDriverAttachListnerNumber.setDefaultValue(1)
	usbHostCdcClientDriverAttachListnerNumber.setVisible(True)

		
	##############################################################
	# system_definitions.h file for USB Host CDC Client driver   
	##############################################################
	usbHostCdcSystemDefFile = usbHostCdcComponent.createFileSymbol(None, None)
	usbHostCdcSystemDefFile.setType("STRING")
	usbHostCdcSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbHostCdcSystemDefFile.setSourcePath("templates/host/system_definitions.h.host_cdc_includes.ftl")
	usbHostCdcSystemDefFile.setMarkup(True)
	
	##############################################################
	# system_config.h file for USB Host CDC Client driver   
	##############################################################
	usbHostCdcSystemConfigFile = usbHostCdcComponent.createFileSymbol(None, None)
	usbHostCdcSystemConfigFile.setType("STRING")
	usbHostCdcSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbHostCdcSystemConfigFile.setSourcePath("templates/host/system_config.h.host_cdc.ftl")
	usbHostCdcSystemConfigFile.setMarkup(True)
	
	##############################################################
	# TPL Entry for CDC client driver 
	##############################################################
	usbHostCdcTplEntryFile = usbHostCdcComponent.createFileSymbol(None, None)
	usbHostCdcTplEntryFile.setType("STRING")
	usbHostCdcTplEntryFile.setOutputName("usb_host.LIST_USB_HOST_TPL_ENTRY")
	usbHostCdcTplEntryFile.setSourcePath("templates/host/system_init_c_cdc_tpl.ftl")
	usbHostCdcTplEntryFile.setMarkup(True)
	
	################################################
	# USB Host CDC Client driver Files 
	################################################
	usbHostCdcHeaderFile = usbHostCdcComponent.createFileSymbol(None, None)
	addFileName('usb_host_cdc.h', usbHostCdcComponent, usbHostCdcHeaderFile, "", "/usb/", True, None)
	
	usbCdcHeaderFile = usbHostCdcComponent.createFileSymbol(None, None)
	addFileName('usb_cdc.h', usbHostCdcComponent, usbCdcHeaderFile, "", "/usb/", True, None)
	
	usbHostCdcSourceFile = usbHostCdcComponent.createFileSymbol(None, None)
	addFileName('usb_host_cdc.c', usbHostCdcComponent, usbHostCdcSourceFile, "src/", "/usb/src/", True, None)
	
	usbHostCdcAcmSourceFile = usbHostCdcComponent.createFileSymbol(None, None)
	addFileName('usb_host_cdc_acm.c', usbHostCdcComponent, usbHostCdcAcmSourceFile, "src/", "/usb/src/", True, None)
	
	usbHostCdcLocalHeaderFile = usbHostCdcComponent.createFileSymbol(None, None)
	addFileName('usb_host_cdc_local.h', usbHostCdcComponent, usbHostCdcLocalHeaderFile, "src/", "/usb/src", True, None)
	
	
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