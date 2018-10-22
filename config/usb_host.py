def setVisible(symbol, event):
	if (event["value"] == True):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)
		
def showRTOSMenu(symbol, event):
	show_rtos_menu = False

	if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
		show_rtos_menu = True
	symbol.setVisible(show_rtos_menu)


def instantiateComponent(usbHostComponent):

	res = Database.activateComponents(["HarmonyCore"])
	res = Database.activateComponents(["drv_usbhs_v1"])
	
	# USB Host Max Number of Devices   
	usbHostDeviceNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_DEVICE_NUMNBER", None)
	usbHostDeviceNumber.setLabel("Maximum Number of Devices")
	usbHostDeviceNumber.setVisible(False)
	usbHostDeviceNumber.setDescription("Maximum Number of Devices that will be attached to this Host")
	usbHostDeviceNumber.setDefaultValue(1)
	usbHostDeviceNumber.setDependencies(blUsbHostDeviceNumber, ["USB_OPERATION_MODE"])

	# USB Host Number of TPL Entries 
	usbHostTplEntryNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_TPL_ENTRY_NUMBER", None)
	usbHostTplEntryNumber.setVisible(True)
	usbHostTplEntryNumber.setLabel("Number of TPL Entries")
	usbHostTplEntryNumber.setDescription("Number of TPL entries")
	usbHostTplEntryNumber.setDefaultValue(0)
	usbHostTplEntryNumber.setUseSingleDynamicValue(True)
	usbHostTplEntryNumber.setDependencies(blUsbHostDeviceNumber, ["USB_OPERATION_MODE"])
	usbHostTplEntryNumber.setReadOnly(True)

	# USB Host Max Interfaces  
	usbHostMaxInterfaceNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_MAX_INTERFACES", None)
	usbHostMaxInterfaceNumber.setLabel("Maximum Interfaces per Device")
	usbHostMaxInterfaceNumber.setVisible(True)
	usbHostMaxInterfaceNumber.setDescription("Maximum Number of Interfaces per Device")
	usbHostMaxInterfaceNumber.setDefaultValue(5)
	usbHostMaxInterfaceNumber.setDependencies(blUsbHostMaxInterfaceNumber, ["USB_OPERATION_MODE"])	
	
	# USB Host Transfers Number 
	usbHostTransfersNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_TRANSFERS_NUMBER", None)
	usbHostTransfersNumber.setLabel("Maximum Number of Transfers")
	usbHostTransfersNumber.setVisible(True)
	usbHostTransfersNumber.setDescription("Maximum number of transfers that host layer should handle")
	usbHostTransfersNumber.setDefaultValue(10)
	usbHostTransfersNumber.setDependencies(blUsbHostMaxInterfaceNumber, ["USB_OPERATION_MODE"])	
	
	enable_rtos_settings = False

	if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
		enable_rtos_settings = True

	# RTOS Settings 
	usbHostRTOSMenu = usbHostComponent.createMenuSymbol(None, None)
	usbHostRTOSMenu.setLabel("RTOS settings")
	usbHostRTOSMenu.setDescription("RTOS settings")
	usbHostRTOSMenu.setVisible(enable_rtos_settings)
	usbHostRTOSMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])

	usbHostRTOSTask = usbHostComponent.createComboSymbol("USB_HOST_RTOS", usbHostRTOSMenu, ["Standalone"])
	usbHostRTOSTask.setLabel("Run Library Tasks As")
	usbHostRTOSTask.setDefaultValue("Standalone")
	usbHostRTOSTask.setVisible(False)

	usbHostRTOSStackSize = usbHostComponent.createIntegerSymbol("USB_HOST_RTOS_STACK_SIZE", usbHostRTOSMenu)
	usbHostRTOSStackSize.setLabel("Stack Size")
	usbHostRTOSStackSize.setDefaultValue(1024)
	usbHostRTOSStackSize.setReadOnly(True)

	usbHostRTOSTaskPriority = usbHostComponent.createIntegerSymbol("USB_HOST_RTOS_TASK_PRIORITY", usbHostRTOSMenu)
	usbHostRTOSTaskPriority.setLabel("Task Priority")
	usbHostRTOSTaskPriority.setDefaultValue(1)

	usbHostRTOSTaskDelay = usbHostComponent.createBooleanSymbol("USB_HOST_RTOS_USE_DELAY", usbHostRTOSMenu)
	usbHostRTOSTaskDelay.setLabel("Use Task Delay?")
	usbHostRTOSTaskDelay.setDefaultValue(True)

	usbHostRTOSTaskDelayVal = usbHostComponent.createIntegerSymbol("USB_HOST_RTOS_DELAY", usbHostRTOSMenu)
	usbHostRTOSTaskDelayVal.setLabel("Task Delay")
	usbHostRTOSTaskDelayVal.setDefaultValue(10) 
	usbHostRTOSTaskDelayVal.setVisible((usbHostRTOSTaskDelay.getValue() == True))
	usbHostRTOSTaskDelayVal.setDependencies(setVisible, ["USB_HOST_RTOS_USE_DELAY"])
	
	configName = Variables.get("__CONFIGURATION_NAME")
	
	################################################
	# system_definitions.h file for USB Host Layer    
	################################################
	usbHostSystemDefFile = usbHostComponent.createFileSymbol(None, None)
	usbHostSystemDefFile.setType("STRING")
	usbHostSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbHostSystemDefFile.setSourcePath("templates/host/system_definitions.h.host_includes.ftl")
	usbHostSystemDefFile.setMarkup(True)
	
	usbHostSystemDefObjFile = usbHostComponent.createFileSymbol(None, None)
	usbHostSystemDefObjFile.setType("STRING")
	usbHostSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
	usbHostSystemDefObjFile.setSourcePath("templates/host/system_definitions.h.host_objects.ftl")
	usbHostSystemDefObjFile.setMarkup(True)
	
	usbHostSystemDefExtFile = usbHostComponent.createFileSymbol(None, None)
	usbHostSystemDefExtFile.setType("STRING")
	usbHostSystemDefExtFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_EXTERNS")
	usbHostSystemDefExtFile.setSourcePath("templates/host/system_definitions.h.host_externs.ftl")
	usbHostSystemDefExtFile.setMarkup(True)
	
	################################################
	# system_config.h file for USB Host Layer    
	################################################
	usbHostSystemConfigFile = usbHostComponent.createFileSymbol(None, None)
	usbHostSystemConfigFile.setType("STRING")
	usbHostSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbHostSystemConfigFile.setSourcePath("templates/host/system_config.h.host.ftl")
	usbHostSystemConfigFile.setMarkup(True)
	
	################################################
	# system_init.c file for USB Host Layer    
	################################################
	#usbHostSystemInitDataFile1 = usbHostComponent.createFileSymbol(None, None)
	#usbHostSystemInitDataFile1.setType("SOURCE")
	#usbHostSystemInitDataFile1.setOutputName("core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA")
	#usbHostSystemInitDataFile1.setOutputName("usb_host_tpl_init.c")
	
	usbHostSystemInitDataFile = usbHostComponent.createFileSymbol(None, None)
	usbHostSystemInitDataFile.setType("SOURCE")
	#usbHostSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA")
	usbHostSystemInitDataFile.setOutputName("usb_host_init_data.c")
	usbHostSystemInitDataFile.setSourcePath("templates/host/system_init_c_host_data.ftl")
	usbHostSystemInitDataFile.setMarkup(True)
	usbHostSystemInitDataFile.setOverwrite(True)
	usbHostSystemInitDataFile.setDestPath("")
	usbHostSystemInitDataFile.setProjectPath("config/" + configName + "/")
	usbHostClientInitEntry = usbHostComponent.createListSymbol("LIST_USB_HOST_CLIENT_INIT_DATA", None)
	usbHostTplList = usbHostComponent.createListSymbol("LIST_USB_HOST_TPL_ENTRY", None)

	usbHostSystemInitCallsFile = usbHostComponent.createFileSymbol(None, None)
	usbHostSystemInitCallsFile.setType("STRING")
	usbHostSystemInitCallsFile.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
	usbHostSystemInitCallsFile.setSourcePath("templates/host/system_init_c_host_calls.ftl")
	usbHostSystemInitCallsFile.setMarkup(True)
	
	################################################
	# system_tasks.c file for USB Device stack  
	################################################
	usbHostSystemTasksFile = usbHostComponent.createFileSymbol(None, None)
	usbHostSystemTasksFile.setType("STRING")
	usbHostSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
	usbHostSystemTasksFile.setSourcePath("templates/host/system_tasks_c_host.ftl")
	usbHostSystemTasksFile.setMarkup(True)
	
	usbHostSystemTasksFileRTOS = usbHostComponent.createFileSymbol("USB_HOST_SYS_RTOS_TASK", None)
	usbHostSystemTasksFileRTOS.setType("STRING")
	usbHostSystemTasksFileRTOS.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	usbHostSystemTasksFileRTOS.setSourcePath("templates/host/system_tasks_c_host_rtos.ftl")
	usbHostSystemTasksFileRTOS.setMarkup(True)
	usbHostSystemTasksFileRTOS.setEnabled(enable_rtos_settings)
	
	################################################
	# USB Host Layer Files 
	################################################
	usbHostHeaderFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_host.h', usbHostComponent, usbHostHeaderFile, "", "/usb/", True, None)
	
	usbHostSourceFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_host.c', usbHostComponent, usbHostSourceFile, "src/", "/usb/src", True, None)

	usbHostLocalHeaderFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_host_local.h', usbHostComponent, usbHostLocalHeaderFile, "src/", "/usb/src", True, None)
	
	usbCommonFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_common.h', usbHostComponent, usbCommonFile, "", "/usb/", True, None)
	
	usbChapter9File = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_chapter_9.h', usbHostComponent, usbChapter9File, "", "/usb/", True, None)
	
	usbHostHubMappingFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_host_hub_mapping.h', usbHostComponent, usbHostHubMappingFile, "src/", "/usb/src", True, None)
	
	usbExternalDependenciesFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_external_dependencies.h', usbHostComponent, usbExternalDependenciesFile, "src/", "/usb/src", True, None)
		
	
	
def blUsbHostDeviceNumber(usbSymbolSource, event):
	blUsbLog(usbSymbolSource, event)
	if (event["value"] == "Host"):		
		blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
		
def usbHostTplEntryNumber(usbSymbolSource, event):
	blUsbLog(usbSymbolSource, event)
	if (event["value"] == "Host"):		
		blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
		
def blUsbHostMaxInterfaceNumber(usbSymbolSource, event):
	blUsbLog(usbSymbolSource, event)
	if (event["value"] == "Host"):		
		blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)	
		
def blUSBHostEnableConfig(usbSymbolSource, event):
	if (event["value"] == True):		
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
		
		
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
		
def onDependentComponentAdded(component, id, usart):
	if id == "usb_driver_dependency" :
		Database.clearSymbolValue("drv_usbhs_v1", "USB_OPERATION_MODE")
		Database.setSymbolValue("drv_usbhs_v1", "USB_OPERATION_MODE", "Host" , 2)
