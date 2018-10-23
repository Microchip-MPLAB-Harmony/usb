def onDependentComponentAdded(ownerComponent, dependencyID, dependentComponent):
	print(ownerComponent)
	if (dependencyID == "usb_host_dependency"):
		readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.clearSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.setSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER", readValue + 1 , 2)

def instantiateComponent(usbHostMsdComponent):
	res = Database.activateComponents(["usb_host"])
	
	# USB Host MSD client driver instances 
	usbHostMsdClientDriverInstance = usbHostMsdComponent.createIntegerSymbol("CONFIG_USB_HOST_MSD_NUMBER_OF_INSTANCES", None)
	usbHostMsdClientDriverInstance.setLabel("Number of MSD Client Driver Instances")
	usbHostMsdClientDriverInstance.setDescription("Enter the number of MSD Class Driver instances required in the application.")
	usbHostMsdClientDriverInstance.setDefaultValue(1)
	usbHostMsdClientDriverInstance.setVisible(True)
	
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		Database.clearSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.setSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER", readValue + 1 , 2)
	
	##############################################################
	# system_definitions.h file for USB Host MSD Client driver   
	##############################################################
	usbHostMsdSystemDefFile = usbHostMsdComponent.createFileSymbol(None, None)
	usbHostMsdSystemDefFile.setType("STRING")
	usbHostMsdSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbHostMsdSystemDefFile.setSourcePath("templates/host/system_definitions.h.host_msd_includes.ftl")
	usbHostMsdSystemDefFile.setMarkup(True)
	
	##############################################################
	# system_config.h file for USB Host MSD Client driver   
	##############################################################
	usbHostMsdSystemConfigFile = usbHostMsdComponent.createFileSymbol(None, None)
	usbHostMsdSystemConfigFile.setType("STRING")
	usbHostMsdSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbHostMsdSystemConfigFile.setSourcePath("templates/host/system_config.h.host_msd.ftl")
	usbHostMsdSystemConfigFile.setMarkup(True)
	
	##############################################################
	# TPL Entry for MSD client driver 
	##############################################################
	usbHostMsdTplEntryFile = usbHostMsdComponent.createFileSymbol(None, None)
	usbHostMsdTplEntryFile.setType("STRING")
	usbHostMsdTplEntryFile.setOutputName("usb_host.LIST_USB_HOST_TPL_ENTRY")
	usbHostMsdTplEntryFile.setSourcePath("templates/host/system_init_c_msd_tpl.ftl")
	usbHostMsdTplEntryFile.setMarkup(True)
	 
	################################################
	# USB Host MSD Client driver Files 
	################################################
	usbHostMsdHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_msd.h', usbHostMsdComponent, usbHostMsdHeaderFile, "", "/usb/", True, None)
	
	usbHostScsiHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_scsi.h', usbHostMsdComponent, usbHostScsiHeaderFile, "", "/usb/", True, None)
	
	usbMsdHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_msd.h', usbHostMsdComponent, usbMsdHeaderFile, "", "/usb/", True, None)
	
	usbScsiHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('scsi.h', usbHostMsdComponent, usbScsiHeaderFile, "", "/usb/", True, None)
	
	
	usbHostMsdSourceFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_msd.c', usbHostMsdComponent, usbHostMsdSourceFile, "src/", "/usb/src", True, None)
	
	usbHostScsiSourceFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_scsi.c', usbHostMsdComponent, usbHostScsiSourceFile, "src/", "/usb/src", True, None)

	usbHostMsdLocalHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_msd_local.h', usbHostMsdComponent, usbHostMsdLocalHeaderFile, "src/", "/usb/src", True, None)
	
	usbHostScsiLocalHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_scsi_local.h', usbHostMsdComponent, usbHostScsiLocalHeaderFile, "src/", "/usb/src", True, None)
	
	
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