"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""
# Global definitions  
listUsbSpeed = ["High Speed", "Full Speed"]
#listUsbSpeed = ["DRV_USBHSV1_DEVICE_SPEEDCONF_NORMAL", "DRV_USBHSV1_DEVICE_SPEEDCONF_LOW_POWER"]
listUsbOperationMode = ["Device", "Host", "Dual Role"]
usbDebugLogs = 1 
usbDriverPath = "driver/"
usbDriverProjectPath = "/driver/usb/"

def genRtosTask(symbol, event):
	if event["value"] != "BareMetal":
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)

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
	usbSpeed.setVisible(False)
        helpText = '''While operating in Host Mode, this option configures the types (speed) of USB devices to be supported. 
        For example, setting this option to High Speed will allow the Host to support High, Full and Low Speed devices. 
        Setting this option to Full Speed will allow the Host to support only Full and Low Speed devices. If a High Speed
        device is attached, it will operate at Full Speed.

        In Device mode, this configuration select the operational speed of the device. If configured for High Speed mode, the
        device will signal Chirp after receiving the bus reset from the host. If configured for Full Speed mode, the device 
        will not signal Chirp and will operate at Full Speed only.'''
	
	usbSpeed.setDescription(helpText)
	usbSpeed.setDefaultValue("High Speed")
	usbSpeed.setDependencies(blUSBDriverSpeedChanged, ["USB_SPEED"])	
	
	# USB Driver Host mode Attach de-bounce duration 
	usbDriverHostAttachDebounce = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_ATTACH_DEBOUNCE_DURATION", None)
	usbDriverHostAttachDebounce.setLabel("Attach De-bounce Duration (mSec)")
	usbDriverHostAttachDebounce.setVisible(True)
        helpText = '''Specify the time duration (in milliseconds) that the driver should wait after detecting the
        attach interrupt and before polling the attach interrupt again to check if the attach condition still exists.
        A longer duration slows down the Host Device Attach detection but allows for stable operation.'''
	usbDriverHostAttachDebounce.setDescription(helpText)
	usbDriverHostAttachDebounce.setMin(1)
	usbDriverHostAttachDebounce.setMax(2000)
	usbDriverHostAttachDebounce.setDefaultValue(500)
	usbDriverHostAttachDebounce.setDependencies(blUSBDriverOperationModeChanged, ["USB_OPERATION_MODE"])
	
	# USB Driver Host mode Reset Duration
	usbDriverHostResetDuration = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_RESET_DUARTION", None)
	usbDriverHostResetDuration.setLabel("Reset Duration (mSec)")
        helpText = '''Specify the duration of the USB Bus Reset Signal. A value of 100 millisecond works 
        well. There may be cases where some USB devices require a shorter or longer reset duration time.'''
	usbDriverHostResetDuration.setVisible(True)
	usbDriverHostResetDuration.setDescription(helpText)
	usbDriverHostResetDuration.setMin(1)
	usbDriverHostResetDuration.setMax(500)
	usbDriverHostResetDuration.setDefaultValue(100)
	usbDriverHostResetDuration.setDependencies(blUSBDriverOperationModeChanged, ["USB_OPERATION_MODE"])
	
	# USB Driver Host mode DRV_USB_UHP_NO_CACHE_BUFFER_LENGTH
	usbDriverHostBufferSize = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_BUFFER_SIZE", None)
	usbDriverHostBufferSize.setLabel("Transfer Buffer Cache Size (Bytes)")
	usbDriverHostBufferSize.setVisible(True)
        helpText = '''Specify the maximum transfer size here. The USB driver will 
		create a buffer for each transfer object with a size specified here. The 
		USB driver will decline the transfer request if the transfer size is greater
		than this value.'''
	usbDriverHostBufferSize.setDescription(helpText)
	usbDriverHostBufferSize.setMin(500)
	usbDriverHostBufferSize.setMax(4096)
	usbDriverHostBufferSize.setDefaultValue(4096)
	
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
	# Update USB General Interrupt Handler 
	Database.setSymbolValue("core", "UHPHS_INTERRUPT_ENABLE", True, 1)
	Database.setSymbolValue("core", "UHPHS_INTERRUPT_HANDLER_LOCK", True, 1)
	Database.setSymbolValue("core", "UHPHS_INTERRUPT_HANDLER", "UHPHS_Handler", 1)
	
	# Enable dependent Harmony core components
	if Database.getSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON") == False: 
		Database.setSymbolValue("HarmonyCore", "ENABLE_DRV_COMMON", True, 2)

	if Database.getSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON") == False: 
		Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_COMMON", True, 2)

	if Database.getSymbolValue("HarmonyCore", "ENABLE_SYS_INT") == False: 
		Database.setSymbolValue("HarmonyCore", "ENABLE_SYS_INT", True, 2)

	if Database.getSymbolValue("HarmonyCore", "ENABLE_OSAL") == False: 
		Database.setSymbolValue("HarmonyCore", "ENABLE_OSAL", True, 2)
	
	if Database.getSymbolValue("HarmonyCore", "ENABLE_APP_FILE") == False: 
		Database.setSymbolValue("HarmonyCore", "ENABLE_APP_FILE", True, 2)
	
	configName = Variables.get("__CONFIGURATION_NAME")
	
	
	sourcePath = "templates/driver/uhp/"
	
	
	################################################
	# system_definitions.h file for USB Driver   
	################################################
	usbDriverSystemDefIncludeFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemDefIncludeFile.setType("STRING")
	usbDriverSystemDefIncludeFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDriverSystemDefIncludeFile.setSourcePath( sourcePath + "system_definitions.h.driver_includes.ftl")
	usbDriverSystemDefIncludeFile.setMarkup(True)
	
	usbDriverSystemDefObjFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemDefObjFile.setType("STRING")
	usbDriverSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
	usbDriverSystemDefObjFile.setSourcePath(sourcePath + "system_definitions.h.driver_objects.ftl")
	usbDriverSystemDefObjFile.setMarkup(True)
	
	################################################
	# system_config.h file for USB Driver   
	################################################
	usbDriverSystemConfigFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemConfigFile.setType("STRING")
	usbDriverSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDriverSystemConfigFile.setSourcePath(sourcePath +"system_config.h.driver.ftl")
	usbDriverSystemConfigFile.setMarkup(True)
	
	################################################
	# system_init.c file for USB Driver   
	################################################
	usbDriverSystemInitDataFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemInitDataFile.setType("STRING")
	usbDriverSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA")
	usbDriverSystemInitDataFile.setSourcePath(sourcePath + "system_init_c_driver_data.ftl")
	usbDriverSystemInitDataFile.setMarkup(True)
	
	usbDriverSystemInitCallsFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemInitCallsFile.setType("STRING")
	usbDriverSystemInitCallsFile.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
	usbDriverSystemInitCallsFile.setSourcePath(sourcePath + "system_init_c_driver_calls.ftl")
	usbDriverSystemInitCallsFile.setMarkup(True)
	
	################################################
	# system_interrupt.c file for USB Driver   
	################################################
	# usbDriverSystemInterruptFile = usbDriverComponent.createFileSymbol(None, None)
	# usbDriverSystemInterruptFile.setType("STRING")
	# usbDriverSystemInterruptFile.setOutputName("core.LIST_SYSTEM_INTERRUPT_HANDLERS")
	# usbDriverSystemInterruptFile.setSourcePath(sourcePath + "system_interrupt_c_driver.ftl")
	# usbDriverSystemInterruptFile.setMarkup(True)
	
	
	################################################
	# system_tasks.c file for USB Driver   
	################################################
	usbDriverSystemTasksFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemTasksFile.setType("STRING")
	usbDriverSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
	usbDriverSystemTasksFile.setSourcePath( sourcePath + "system_tasks_c_driver.ftl")
	usbDriverSystemTasksFile.setMarkup(True)
	
	usbDriverSystemTasksFileRTOS = usbDriverComponent.createFileSymbol("USB_DRIVER_SYS_RTOS_TASK", None)
	usbDriverSystemTasksFileRTOS.setType("STRING")
	usbDriverSystemTasksFileRTOS.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	usbDriverSystemTasksFileRTOS.setSourcePath(sourcePath + "system_tasks_c_driver_rtos.ftl")
	usbDriverSystemTasksFileRTOS.setMarkup(True)
	usbDriverSystemTasksFileRTOS.setEnabled(enable_rtos_settings)
	usbDriverSystemTasksFileRTOS.setDependencies(genRtosTask, ["Harmony.SELECT_RTOS"])
	
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
	
	# Add drv_usb_uhp.h file
	drvUsbHsV1VarHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1VarHeaderFile.setSourcePath(usbDriverPath + "uhp/drv_usb_uhp.h.ftl")
	drvUsbHsV1VarHeaderFile.setOutputName("drv_usb_uhp.h")
	drvUsbHsV1VarHeaderFile.setDestPath(usbDriverProjectPath + "uhp")
	drvUsbHsV1VarHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp")
	drvUsbHsV1VarHeaderFile.setType("HEADER")
	drvUsbHsV1VarHeaderFile.setOverwrite(True)
	drvUsbHsV1VarHeaderFile.setMarkup(True)

	# EHCI Header file 
	drvUsbUhpEHCIHeaderFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_HEADER_FILE_EHCI", None)
	drvUsbUhpEHCIHeaderFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_uhp_ehci_host.h")
	drvUsbUhpEHCIHeaderFile.setOutputName("drv_usb_uhp_ehci_host.h")
	drvUsbUhpEHCIHeaderFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbUhpEHCIHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src/")
	drvUsbUhpEHCIHeaderFile.setType("HEADER")
	drvUsbUhpEHCIHeaderFile.setOverwrite(True)
	drvUsbUhpEHCIHeaderFile.setEnabled(True)
	
	# OHCI Header file 
	drvUsbUhpOHCIHeaderFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_HEADER_FILE_OHCI", None)
	drvUsbUhpOHCIHeaderFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_uhp_ohci_host.h")
	drvUsbUhpOHCIHeaderFile.setOutputName("drv_usb_uhp_ohci_host.h")
	drvUsbUhpOHCIHeaderFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbUhpOHCIHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src/")
	drvUsbUhpOHCIHeaderFile.setType("HEADER")
	drvUsbUhpOHCIHeaderFile.setOverwrite(True)
	drvUsbUhpOHCIHeaderFile.setEnabled(True)
	
	# OHCI SFR Header file 
	drvUsbUhpOHCISFRHeaderFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_HEADER_FILE_OHCI_SFR", None)
	drvUsbUhpOHCISFRHeaderFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_uhp_ohci_registers.h")
	drvUsbUhpOHCISFRHeaderFile.setOutputName("drv_usb_uhp_ohci_registers.h")
	drvUsbUhpOHCISFRHeaderFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbUhpOHCISFRHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src/")
	drvUsbUhpOHCISFRHeaderFile.setType("HEADER")
	drvUsbUhpOHCISFRHeaderFile.setOverwrite(True)
	drvUsbUhpOHCISFRHeaderFile.setEnabled(True)
	
	# Add drv_usb_uhp_variant_mapping.h file 
	drvUsbHsV1VarMapHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1VarMapHeaderFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_uhp_variant_mapping.h.ftl")
	drvUsbHsV1VarMapHeaderFile.setOutputName("drv_usb_uhp_variant_mapping.h")
	drvUsbHsV1VarMapHeaderFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbHsV1VarMapHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src")
	drvUsbHsV1VarMapHeaderFile.setType("HEADER")
	drvUsbHsV1VarMapHeaderFile.setOverwrite(True)
	drvUsbHsV1VarMapHeaderFile.setMarkup(True)

	# Add drv_usb_uhp_local.h file 
	drvUsbHsV1Var2HeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1Var2HeaderFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_uhp_local.h.ftl")
	drvUsbHsV1Var2HeaderFile.setOutputName("drv_usb_uhp_local.h")
	drvUsbHsV1Var2HeaderFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbHsV1Var2HeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src")
	drvUsbHsV1Var2HeaderFile.setType("HEADER")
	drvUsbHsV1Var2HeaderFile.setOverwrite(True)
	drvUsbHsV1Var2HeaderFile.setMarkup(True)

	usbHostControllerDriverHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_host_client_driver.h', usbDriverComponent, usbHostControllerDriverHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHostHubInterfaceHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_host_hub_interface.h', usbDriverComponent, usbHostHubInterfaceHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHostHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_host.h', usbDriverComponent, usbHostHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHubHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_hub.h', usbDriverComponent, usbHubHeaderFile, "middleware/", "/usb/", True, None)
	
	################################################
	# USB Driver Source files  
	################################################
	drvUsbHsV1SourceFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_SOURCE_FILE_COMMON", None)
	drvUsbHsV1SourceFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_uhp_ehci.c")
	drvUsbHsV1SourceFile.setOutputName("drv_usb_uhp.c")
	drvUsbHsV1SourceFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbHsV1SourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src/")
	drvUsbHsV1SourceFile.setType("SOURCE")
	drvUsbHsV1SourceFile.setOverwrite(True)
	
	drvUsbHsV1HostSourceFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_SOURCE_FILE_EHCI", None)
	drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_uhp_ehci_host.c")
	drvUsbHsV1HostSourceFile.setOutputName("drv_usb_uhp_ehci_host.c")
	drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src/")
	drvUsbHsV1HostSourceFile.setType("SOURCE")
	drvUsbHsV1HostSourceFile.setOverwrite(True)
	drvUsbHsV1HostSourceFile.setEnabled(True)
	
	drvUsbHsV1HostSourceFileOhci = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_SOURCE_FILE_OHCI", None)
	drvUsbHsV1HostSourceFileOhci.setSourcePath(usbDriverPath + "uhp/src/drv_usb_uhp_ohci_host.c")
	drvUsbHsV1HostSourceFileOhci.setOutputName("drv_usb_uhp_ohci_host.c")
	drvUsbHsV1HostSourceFileOhci.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbHsV1HostSourceFileOhci.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src/")
	drvUsbHsV1HostSourceFileOhci.setType("SOURCE")
	drvUsbHsV1HostSourceFileOhci.setOverwrite(True)
	drvUsbHsV1HostSourceFileOhci.setEnabled(True)

		
# all files go into src/
def addFileName(fileName, component, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
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
	
def blUsbVbusPinName(usbSymbolSource, event):
	if (event["value"] == True):
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
	
	
