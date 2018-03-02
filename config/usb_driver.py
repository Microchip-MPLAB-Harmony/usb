# Global definitions  
listUsbSpeed = ["DRV_USBHSV1_DEVICE_SPEEDCONF_NORMAL", "DRV_USBHSV1_DEVICE_SPEEDCONF_LOW_POWER"]
listUsbOperationMode = ["Device", "Host", "Dual Role"]
usbDebugLogs = 1 
usbDriverPath = "driver/"
usbDriverProjectPath = "/usb/driver/"

def instantiateComponent(usbDriverComponent):	
	#print(usbDriverComponent)
	# USB Driver Speed selection 	
	usbSpeed = usbDriverComponent.createComboSymbol("USB_SPEED", None, listUsbSpeed)
	usbSpeed.setLabel("USB Speed Selection")
	usbSpeed.setVisible(True)
	usbSpeed.setDescription("Select USB Operation Speed")
	usbSpeed.setDefaultValue("DRV_USBHSV1_DEVICE_SPEEDCONF_NORMAL")
	
	# USB Driver Operation mode selection 
	usbOpMode = usbDriverComponent.createComboSymbol("USB_OPERATION_MODE", None, listUsbOperationMode)
	usbOpMode.setLabel("USB Mode Selection")
	usbOpMode.setVisible(True)
	usbOpMode.setDescription("Select USB Operation Mode")
	usbOpMode.setDefaultValue("Device")
	
	
	configName = Variables.get("__CONFIGURATION_NAME")
	
	################################################
	# system_definitions.h file for USB Driver   
	################################################
	usbDriverSystemDefIncludeFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemDefIncludeFile.setType("STRING")
	usbDriverSystemDefIncludeFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDriverSystemDefIncludeFile.setSourcePath("templates/system_definitions.h.driver_includes.ftl")
	usbDriverSystemDefIncludeFile.setMarkup(True)
	
	usbDriverSystemDefObjFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemDefObjFile.setType("STRING")
	usbDriverSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
	usbDriverSystemDefObjFile.setSourcePath("templates/system_definitions.h.driver_objects.ftl")
	usbDriverSystemDefObjFile.setMarkup(True)
	
	################################################
	# system_config.h file for USB Driver   
	################################################
	usbDriverSystemConfigFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemConfigFile.setType("STRING")
	usbDriverSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDriverSystemConfigFile.setSourcePath("templates/system_config.h.driver.ftl")
	usbDriverSystemConfigFile.setMarkup(True)
	
	################################################
	# system_init.c file for USB Driver   
	################################################
	usbDriverSystemInitDataFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemInitDataFile.setType("STRING")
	usbDriverSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA")
	usbDriverSystemInitDataFile.setSourcePath("templates/system_init_c_driver_data.ftl")
	usbDriverSystemInitDataFile.setMarkup(True)
	
	usbDriverSystemInitCallsFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemInitCallsFile.setType("STRING")
	usbDriverSystemInitCallsFile.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
	usbDriverSystemInitCallsFile.setSourcePath("templates/system_init_c_driver_calls.ftl")
	usbDriverSystemInitCallsFile.setMarkup(True)
	
	################################################
	# system_interrupt.c file for USB Driver   
	################################################
	usbDriverSystemInterruptFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemInterruptFile.setType("STRING")
	usbDriverSystemInterruptFile.setOutputName("core.LIST_SYSTEM_INTERRUPT_HANDLERS")
	usbDriverSystemInterruptFile.setSourcePath("templates/system_interrupt_c_driver.ftl")
	usbDriverSystemInterruptFile.setMarkup(True)
	
	
	################################################
	# system_tasks.c file for USB Driver   
	################################################
	usbDriverSystemTasksFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemTasksFile.setType("STRING")
	usbDriverSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
	usbDriverSystemTasksFile.setSourcePath("templates/system_tasks_c_driver.ftl")
	usbDriverSystemTasksFile.setMarkup(True)
	
	################################################
	# USB Driver Header files  
	################################################
	drvUsbHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHeaderFile.setSourcePath(usbDriverPath + "drv_usb.h")
	drvUsbHeaderFile.setOutputName("drv_usb.h")
	drvUsbHeaderFile.setDestPath(usbDriverProjectPath)
	drvUsbHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath)
	drvUsbHeaderFile.setType("HEADER")
	drvUsbHeaderFile.setOverwrite(True)
	
	drvUsbHsV1HeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1HeaderFile.setSourcePath(usbDriverPath + "usbhsv1/drv_usbhsv1.h")
	drvUsbHsV1HeaderFile.setOutputName("drv_usbhsv1.h")
	drvUsbHsV1HeaderFile.setDestPath(usbDriverProjectPath+ "usbhsv1")
	drvUsbHsV1HeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath+ "usbhsv1")
	drvUsbHsV1HeaderFile.setType("HEADER")
	drvUsbHsV1HeaderFile.setOverwrite(True)
	
	drvUsbHsV1VarMapHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1VarMapHeaderFile.setSourcePath(usbDriverPath + "usbhsv1/src/drv_usbhsv1_variant_mapping.h")
	drvUsbHsV1VarMapHeaderFile.setOutputName("drv_usbhsv1_variant_mapping.h")
	drvUsbHsV1VarMapHeaderFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
	drvUsbHsV1VarMapHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src")
	drvUsbHsV1VarMapHeaderFile.setType("HEADER")
	drvUsbHsV1VarMapHeaderFile.setOverwrite(True)

	
	drvUsbHsV1LocalHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1LocalHeaderFile.setSourcePath(usbDriverPath + "usbhsv1/src/drv_usbhsv1_local.h")
	drvUsbHsV1LocalHeaderFile.setOutputName("drv_usbhsv1_local.h")
	drvUsbHsV1LocalHeaderFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
	drvUsbHsV1LocalHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src")
	drvUsbHsV1LocalHeaderFile.setType("HEADER")
	drvUsbHsV1LocalHeaderFile.setOverwrite(True)
	
	
	usbHostControllerDriverHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_host_client_driver.h', usbDriverComponent, usbHostControllerDriverHeaderFile, "", "/usb/", True, None)
	
	usbHostHubInterfaceHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_host_hub_interface.h', usbDriverComponent, usbHostHubInterfaceHeaderFile, "", "/usb/", True, None)
	
	usbHostHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_host.h', usbDriverComponent, usbHostHeaderFile, "", "/usb/", True, None)
	
	usbHubHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_hub.h', usbDriverComponent, usbHubHeaderFile, "", "/usb/", True, None)
	
	################################################
	# USB Driver Source files  
	################################################
	drvUsbHsV1SourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1SourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1.c")
	drvUsbHsV1SourceFile.setOutputName("drv_usbhsv1.c")
	drvUsbHsV1SourceFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
	drvUsbHsV1SourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src/")
	drvUsbHsV1SourceFile.setType("SOURCE")
	drvUsbHsV1SourceFile.setOverwrite(True)
	
	drvUsbHsV1DeviceSourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1DeviceSourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1_device.c")
	drvUsbHsV1DeviceSourceFile.setOutputName("drv_usbhsv1_device.c")
	drvUsbHsV1DeviceSourceFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
	drvUsbHsV1DeviceSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src/")
	drvUsbHsV1DeviceSourceFile.setType("SOURCE")
	drvUsbHsV1DeviceSourceFile.setOverwrite(True)
	drvUsbHsV1DeviceSourceFile.setDependencies(blDrvUsbHsV1DeviceSourceFile, ["USB_OPERATION_MODE"])
	
	drvUsbHsV1HostSourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1_device.c")
	drvUsbHsV1HostSourceFile.setOutputName("drv_usbhsv1_host.c")
	drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
	drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src/")
	drvUsbHsV1HostSourceFile.setType("SOURCE")
	drvUsbHsV1HostSourceFile.setOverwrite(True)
	drvUsbHsV1HostSourceFile.setEnabled(False)
	drvUsbHsV1HostSourceFile.setDependencies(blDrvUsbHsV1HostSourceFile, ["USB_OPERATION_MODE"])

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
		symbol.setDependencies(callback, ["USB_OPERATION_MODE"])
		
def blDrvUsbHsV1DeviceSourceFile (usbSymbolSource, event):
	if (event["value"] == "Device"):
		usbSymbolSource.setEnabled(True)
	elif (event["value"] == "Host"):
		usbSymbolSource.setEnabled(False)
		
def blDrvUsbHsV1HostSourceFile (usbSymbolSource, event):
	if (event["value"] == "Device"):
		usbSymbolSource.setEnabled(False)
	elif (event["value"] == "Host"):
		usbSymbolSource.setEnabled(True)