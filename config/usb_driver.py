# Global definitions  
listUsbSpeed = ["High Speed", "Full Speed"]
#listUsbSpeed = ["DRV_USBHSV1_DEVICE_SPEEDCONF_NORMAL", "DRV_USBHSV1_DEVICE_SPEEDCONF_LOW_POWER"]
listUsbOperationMode = ["Device", "Host", "Dual Role"]
usbDebugLogs = 1 
usbDriverPath = "driver/"
usbDriverProjectPath = "/driver/usb/"


def speedChanged(symbol, event):
	Database.clearSymbolValue("core", "PMC_SCER_USBCLK")
	Database.setSymbolValue("core", "PMC_SCER_USBCLK", True, 2)

def dependencyStatus(symbol, event):
	if (event["value"] == False):
		symbol.setVisible(True)
	else :
		symbol.setVisible(False)
		
def blUSBDriverSpeedChanged(symbol, event):
	Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_SPEED")
	Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_SPEED", event["value"], 2)

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
	
def instantiateComponent(usbDriverComponent):	

	res = Database.activateComponents(["HarmonyCore"])
	
	# USB Driver Speed selection 	
	usbSpeed = usbDriverComponent.createComboSymbol("USB_SPEED", None, listUsbSpeed)
	usbSpeed.setLabel("USB Speed Selection")
	usbSpeed.setVisible(True)
	usbSpeed.setDescription("Select USB Operation Speed")
	usbSpeed.setDefaultValue("High Speed")
	usbSpeed.setDependencies(blUSBDriverSpeedChanged, ["USB_SPEED"])
	
	# USB Driver Operation mode selection 
	usbOpMode = usbDriverComponent.createComboSymbol("USB_OPERATION_MODE", None, listUsbOperationMode)
	usbOpMode.setLabel("USB Mode Selection")
	usbOpMode.setVisible(True)
	usbOpMode.setDescription("Select USB Operation Mode")
	usbOpMode.setDefaultValue("Device")
	usbOpMode.setReadOnly(True)
	usbOpMode.setUseSingleDynamicValue(True)
	
	usbVbusSense = usbDriverComponent.createBooleanSymbol("USB_DEVICE_VBUS_SENSE", usbOpMode)
	usbVbusSense.setLabel("Enable VBUS Sense")
	usbVbusSense.setDescription("A Self Powered USB Device firmware must have some means to detect VBUS from Host. A GPIO pin can be configured as an Input and connected to VBUS (through a resistor), and can be used to detect when VBUS is high (host actively powering). This configuration instructs MHC to generate a VBUS SENSE function. The GPIO pin name must be configured as in the below ")
	usbVbusSense.setVisible(True)
	usbVbusSense.setDescription("Select USB Operation Mode")
	usbVbusSense.setDefaultValue(True)
	usbVbusSense.setUseSingleDynamicValue(True)
	usbVbusSense.setDependencies(blUSBDriverOperationModeDevice, ["USB_OPERATION_MODE"])
	
	usbVbusSenseFunctionName = usbDriverComponent.createStringSymbol("USB_DEVICE_VBUS_SENSE_PIN_NAME", usbOpMode)
	usbVbusSenseFunctionName.setLabel("VBUS SENSE Pin Name")
	usbVbusSenseFunctionName.setDefaultValue("USB_VBUS_INState")
	usbVbusSenseFunctionName.setVisible(True)
	usbVbusSenseFunctionName.setDependencies(blUSBDriverOperationModeDevice, ["USB_OPERATION_MODE"])
	
	# USB Driver Host mode Attach de-bounce duration 
	usbDriverHostAttachDebounce = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_ATTACH_DEBOUNCE_DURATION", usbOpMode)
	usbDriverHostAttachDebounce.setLabel("Attach De-bounce Duration (mSec)")
	usbDriverHostAttachDebounce.setVisible(False)
	usbDriverHostAttachDebounce.setDescription("Set USB Host Attach De-bounce duration")
	usbDriverHostAttachDebounce.setDefaultValue(500)
	usbDriverHostAttachDebounce.setDependencies(blUSBDriverOperationModeChanged, ["USB_OPERATION_MODE"])
	
	# USB Driver Host mode Reset Duration
	usbDriverHostResetDuration = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_RESET_DUARTION", usbOpMode)
	usbDriverHostResetDuration.setLabel("Reset Duration (mSec)")
	usbDriverHostResetDuration.setVisible(False)
	usbDriverHostResetDuration.setDescription("Set USB Host Attach De-bounce duration")
	usbDriverHostResetDuration.setDefaultValue(100)
	usbDriverHostResetDuration.setDependencies(blUSBDriverOperationModeChanged, ["USB_OPERATION_MODE"])
	
	enable_rtos_settings = False

	if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
		enable_rtos_settings = True

	# RTOS Settings 
	usbDriverRTOSMenu = usbDriverComponent.createMenuSymbol(None, None)
	usbDriverRTOSMenu.setLabel("RTOS settings")
	usbDriverRTOSMenu.setDescription("RTOS settings")
	usbDriverRTOSMenu.setVisible(enable_rtos_settings)
	usbDriverRTOSMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])

	usbDriverRTOSTask = usbDriverComponent.createComboSymbol("USB_DRIVER_RTOS", usbDriverRTOSMenu, ["Standalone"])
	usbDriverRTOSTask.setLabel("Run Library Tasks As")
	usbDriverRTOSTask.setDefaultValue("Standalone")
	usbDriverRTOSTask.setVisible(False)

	usbDriverRTOSStackSize = usbDriverComponent.createIntegerSymbol("USB_DRIVER_RTOS_STACK_SIZE", usbDriverRTOSMenu)
	usbDriverRTOSStackSize.setLabel("Stack Size")
	usbDriverRTOSStackSize.setDefaultValue(1024)
	usbDriverRTOSStackSize.setReadOnly(True)

	usbDriverRTOSTaskPriority = usbDriverComponent.createIntegerSymbol("USB_DRIVER_RTOS_TASK_PRIORITY", usbDriverRTOSMenu)
	usbDriverRTOSTaskPriority.setLabel("Task Priority")
	usbDriverRTOSTaskPriority.setDefaultValue(1)

	usbDriverRTOSTaskDelay = usbDriverComponent.createBooleanSymbol("USB_DRIVER_RTOS_USE_DELAY", usbDriverRTOSMenu)
	usbDriverRTOSTaskDelay.setLabel("Use Task Delay?")
	usbDriverRTOSTaskDelay.setDefaultValue(True)

	usbDriverRTOSTaskDelayVal = usbDriverComponent.createIntegerSymbol("USB_DRIVER_RTOS_DELAY", usbDriverRTOSMenu)
	usbDriverRTOSTaskDelayVal.setLabel("Task Delay")
	usbDriverRTOSTaskDelayVal.setDefaultValue(10) 
	usbDriverRTOSTaskDelayVal.setVisible((usbDriverRTOSTaskDelay.getValue() == True))
	usbDriverRTOSTaskDelayVal.setDependencies(setVisible, ["USB_DRIVER_RTOS_USE_DELAY"])
	
	############################################################################
    #### Dependency ####
    ############################################################################
	NVICVector = "NVIC_USBHS_ENABLE"
	NVICHandler = "NVIC_USBHS_HANDLER"
	NVICHandlerLock = "NVIC_USBHS_HANDLER_LOCK"

    # Initial settings for CLK and NVIC
	Database.clearSymbolValue("core", "PMC_CKGR_MOR_MOSCXTEN")
	Database.setSymbolValue("core", "PMC_CKGR_MOR_MOSCXTEN", True, 2)
	Database.clearSymbolValue("core", "PMC_CKGR_MOR_MOSCSEL")
	Database.setSymbolValue("core", "PMC_CKGR_MOR_MOSCSEL", True, 2)
	Database.clearSymbolValue("core", "PMC_CKGR_UCKR_UPLLEN")
	Database.setSymbolValue("core", "PMC_CKGR_UCKR_UPLLEN", True, 2)
	Database.clearSymbolValue("core", "USBHS_CLOCK_ENABLE")
	Database.setSymbolValue("core", "USBHS_CLOCK_ENABLE", True, 2)
	Database.clearSymbolValue("core", "PMC_SCER_USBCLK")
	Database.setSymbolValue("core", "PMC_SCER_USBCLK", True, 2)
	Database.clearSymbolValue("core", NVICVector)
	Database.setSymbolValue("core", NVICVector, True, 2)
	Database.clearSymbolValue("core", NVICHandler)
	Database.setSymbolValue("core", NVICHandler, "USBHS_InterruptHandler", 2)
	Database.clearSymbolValue("core", NVICHandlerLock)
	Database.setSymbolValue("core", NVICHandlerLock, True, 2)

	
    # NVIC Dynamic settings
	# usbhsNVICControl = usbDriverComponent.createBooleanSymbol("NVIC_USBHS_ENABLE", None)
	# usbhsNVICControl.setDependencies(NVICControl, ["INTERRUPT_MODE"])
	# usbhsNVICControl.setVisible(False)

    # Dependency Status
	usbhsSymClkEnComment = usbDriverComponent.createCommentSymbol("USBHS_CLK_ENABLE_COMMENT", None)
	usbhsSymClkEnComment.setVisible(False)
	usbhsSymClkEnComment.setLabel("Warning!!! USBHS Peripheral Clock is Disabled in Clock Manager")
	usbhsSymClkEnComment.setDependencies(dependencyStatus, ["core.PMC_ID_USBHS"])

	usbhsSymIntEnComment = usbDriverComponent.createCommentSymbol("USBHS_NVIC_ENABLE_COMMENT", None)
	usbhsSymIntEnComment.setVisible(False)
	usbhsSymIntEnComment.setLabel("Warning!!! USBHS Interrupt is Disabled in Interrupt Manager")
	usbhsSymIntEnComment.setDependencies(dependencyStatus, ["core." + NVICVector])
	
	# Enable dependent Harmony core components
	Database.clearSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON")
	Database.setSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON", True, 2)
	
	Database.clearSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON")
	Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON", True, 2)

	Database.clearSymbolValue("HarmonyCore", "ENABLE_SYS_INT")
	Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_INT", True, 2)

    # Database.clearSymbolValue("Harmony", "ENABLE_SYS_DMA")
    # Database.setSymbolValue("Harmony", "ENABLE_SYS_DMA", True, 2)

	Database.clearSymbolValue("HarmonyCore", "ENABLE_OSAL")
	Database.setSymbolValue("HarmonyCore", "ENABLE_OSAL", True, 2)

	Database.clearSymbolValue("HarmonyCore", "ENABLE_APP_FILE")
	Database.setSymbolValue("HarmonyCore", "ENABLE_APP_FILE", True, 2)
	
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
	
	usbDriverSystemTasksFileRTOS = usbDriverComponent.createFileSymbol("USB_DRIVER_SYS_RTOS_TASK", None)
	usbDriverSystemTasksFileRTOS.setType("STRING")
	usbDriverSystemTasksFileRTOS.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	usbDriverSystemTasksFileRTOS.setSourcePath("templates/system_tasks_c_driver_rtos.ftl")
	usbDriverSystemTasksFileRTOS.setMarkup(True)
	usbDriverSystemTasksFileRTOS.setEnabled(enable_rtos_settings)
	
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
	
	drvUsbExternalDependenciesFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbExternalDependenciesFile.setSourcePath(usbDriverPath + "drv_usb_external_dependencies.h")
	drvUsbExternalDependenciesFile.setOutputName("drv_usb_external_dependencies.h")
	drvUsbExternalDependenciesFile.setDestPath(usbDriverProjectPath)
	drvUsbExternalDependenciesFile.setProjectPath("config/" + configName + usbDriverProjectPath)
	drvUsbExternalDependenciesFile.setType("HEADER")
	drvUsbExternalDependenciesFile.setOverwrite(True)
	
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
	#drvUsbHsV1SourceFile.setDependencies(blDrvUsbHsV1SourceFile, ["USB_OPERATION_MODE"])
	
	drvUsbHsV1DeviceSourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1DeviceSourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1_device.c")
	drvUsbHsV1DeviceSourceFile.setOutputName("drv_usbhsv1_device.c")
	drvUsbHsV1DeviceSourceFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
	drvUsbHsV1DeviceSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src/")
	drvUsbHsV1DeviceSourceFile.setType("SOURCE")
	drvUsbHsV1DeviceSourceFile.setOverwrite(True)
	drvUsbHsV1DeviceSourceFile.setDependencies(blDrvUsbHsV1DeviceSourceFile, ["USB_OPERATION_MODE"])
	
	drvUsbHsV1HostSourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1_host.c")
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

def blUSBDriverOperationModeDevice(usbSymbolSource, event):
	if (event["value"] == "Host"):
		usbSymbolSource.setVisible(False)
	else:
		usbSymbolSource.setVisible(True)
	
def blUSBDriverOperationModeChanged(usbSymbolSource, event):
	if (event["value"] == "Device"):
		usbSymbolSource.setVisible(False)
	else:
		usbSymbolSource.setVisible(True)
		
def onDependentComponentAdded(ownerComponent, dependencyID, dependentComponent):
	print(ownerComponent, dependencyID, dependentComponent)
	