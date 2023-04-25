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
	args = {"usbSpeed" : event["value"]}
	Database.sendMessage("usb_device", "UPDATE_DEVICE_SPEED", args)

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
	
	# EHCI Driver Menu 
	usbDriverEHCI = usbDriverComponent.createBooleanSymbol("USB_DRV_EHCI_MENU", None)
	usbDriverEHCI.setLabel("Enable EHCI")
	helpText = '''Check this box to enable EHCI Driver'''
	usbDriverEHCI.setDescription(helpText)
	usbDriverEHCI.setVisible(True)
	usbDriverEHCI.setReadOnly(True)
	usbDriverEHCI.setDefaultValue(True)
	
	# USB Driver Host mode Control Transfer Buffer Size 
	usbDriverHostEhciBufferSizeControl = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_EHCI_BUFFER_SIZE_CONTROL", usbDriverEHCI)
	usbDriverHostEhciBufferSizeControl.setLabel("Control Transfer Cache Buffer Size (Bytes)")
	usbDriverHostEhciBufferSizeControl.setVisible(True)
        helpText = '''Specify the maximum transfer size here. The USB driver will 
		create a buffer for each transfer object with a size specified here. The 
		USB driver will decline the transfer request if the transfer size is greater
		than this value.'''
	usbDriverHostEhciBufferSizeControl.setDescription(helpText)
	usbDriverHostEhciBufferSizeControl.setMin(64)
	usbDriverHostEhciBufferSizeControl.setMax(4096)
	usbDriverHostEhciBufferSizeControl.setDefaultValue(512)
	usbDriverHostEhciBufferSizeControl.setDependencies(blUSBDriverEHCI, ["USB_DRV_EHCI_MENU"])
	
	# USB Driver Host EHCI CACHE_BUFFER_LENGTH
	usbDriverHostEhciBufferSize = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_EHCI_BUFFER_SIZE", usbDriverEHCI)
	usbDriverHostEhciBufferSize.setLabel("Non Control Transfer Cache Buffer Size (Bytes)")
	usbDriverHostEhciBufferSize.setVisible(True)
        helpText = '''Specify the maximum transfer size here. The USB driver will 
		create a buffer for each transfer object with a size specified here. The 
		USB driver will decline the transfer request if the transfer size is greater
		than this value.'''
	usbDriverHostEhciBufferSize.setDescription(helpText)
	usbDriverHostEhciBufferSize.setMin(500)
	usbDriverHostEhciBufferSize.setMax(4096)
	usbDriverHostEhciBufferSize.setDefaultValue(512)
	usbDriverHostEhciBufferSize.setDependencies(blUSBDriverEHCI, ["USB_DRV_EHCI_MENU"]) 	
	
	# USB EHCI Driver Ports selection
	usbDriverEHCIPortSelection = usbDriverComponent.createStringSymbol("USB_DRV_HOST_EHCI_PORTS_SELECTION", usbDriverEHCI)
	usbDriverEHCIPortSelection.setLabel("Ports Selection")
	usbDriverEHCIPortSelection.setVisible(True)
        helpText = '''The EHCI controller supports multiple ports. Specify the bit 
		map value of the ports should be scanned by the host controller. e.g. a value
		of 0x02 will cause the EHCI driver to scan the Port B.'''
	usbDriverEHCIPortSelection.setDescription(helpText)
	usbDriverEHCIPortSelection.setDefaultValue("0x02")
	
	
	# OHCI Menu starts here 
	usbDriverOHCI = usbDriverComponent.createBooleanSymbol("USB_DRV_OHCI_MENU", None)
	usbDriverOHCI.setLabel("Enable OHCI")
	helpText = '''Check this box to enable OHCI Driver'''
	usbDriverOHCI.setDescription(helpText)
	usbDriverOHCI.setVisible(True)
	usbDriverOHCI.setReadOnly(False)
	usbDriverOHCI.setReadOnly(True)
	usbDriverOHCI.setDefaultValue(True)
	
	
	# USB Driver OHCI Host mode Control Transfer Buffer Size 
	usbDriverOhciHostBufferSizeControl = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_OHCI_BUFFER_SIZE_CONTROL", usbDriverOHCI)
	usbDriverOhciHostBufferSizeControl.setLabel("Control Transfer Cache Buffer Size (Bytes)")
	usbDriverOhciHostBufferSizeControl.setVisible(True)
        helpText = '''Specify the maximum transfer size here. The USB driver will 
		create a buffer for each transfer object with a size specified here. The 
		USB driver will decline the transfer request if the transfer size is greater
		than this value.'''
	usbDriverOhciHostBufferSizeControl.setDescription(helpText)
	usbDriverOhciHostBufferSizeControl.setMin(64)
	usbDriverOhciHostBufferSizeControl.setMax(4096)
	usbDriverOhciHostBufferSizeControl.setDefaultValue(512)
	usbDriverOhciHostBufferSizeControl.setDependencies(blUSBDriverOHCI, ["USB_DRV_OHCI_MENU"])
	
	# USB Driver Host OHCI  NO CACHE BUFFER LENGTH
	usbDriverHostOhciBufferSize = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_OHCI_BUFFER_SIZE", usbDriverOHCI)
	usbDriverHostOhciBufferSize.setLabel("Non Control Transfer Cache Buffer Size (Bytes)")
	usbDriverHostOhciBufferSize.setVisible(True)
        helpText = '''Specify the maximum transfer size here. The USB driver will 
		create a buffer for each transfer object with a size specified here. The 
		USB driver will decline the transfer request if the transfer size is greater
		than this value.'''
	usbDriverHostOhciBufferSize.setDescription(helpText)
	usbDriverHostOhciBufferSize.setMin(500)
	usbDriverHostOhciBufferSize.setMax(4096)
	usbDriverHostOhciBufferSize.setDefaultValue(512)
	usbDriverHostOhciBufferSize.setDependencies(blUSBDriverOHCI, ["USB_DRV_OHCI_MENU"]) 
	
	# USB OHCI Driver Ports selection
	usbDriverOHCIPortSelection = usbDriverComponent.createStringSymbol("USB_DRV_HOST_OHCI_PORTS_SELECTION", usbDriverOHCI)
	usbDriverOHCIPortSelection.setLabel("Ports Selection")
	usbDriverOHCIPortSelection.setVisible(True)
        helpText = '''The EHCI controller supports multiple ports. Specify the bit 
		map value of the ports should be scanned by the host controller. e.g. a value
		of 0x02 will cause the EHCI driver to scan the Port B.'''
	usbDriverOHCIPortSelection.setDescription(helpText)
	usbDriverOHCIPortSelection.setDefaultValue("0x02")
	
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
	
	# Enable Driver common files 
	Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

	# Enable System common files. 
	Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})
	
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
	usbDriverSystemTasksFileRTOS.setDependencies(genRtosTask, ["HarmonyCore.SELECT_RTOS"])
	
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
	
	# EHCI Header file 
	drvUsbEHCIHeaderFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_HEADER_FILE_EHCI", None)
	drvUsbEHCIHeaderFile.setSourcePath(usbDriverPath + "uhp/drv_usb_ehci.h")
	drvUsbEHCIHeaderFile.setOutputName("drv_usb_ehci.h")
	drvUsbEHCIHeaderFile.setDestPath(usbDriverProjectPath + "uhp/")
	drvUsbEHCIHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/")
	drvUsbEHCIHeaderFile.setType("HEADER")
	drvUsbEHCIHeaderFile.setOverwrite(True)
	drvUsbEHCIHeaderFile.setEnabled(True)
	
	# Add EHCI Local Header file 
	drvUsbEHCILocalHeaderFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_HEADER_FILE_EHCI_LOCAL", None)
	drvUsbEHCILocalHeaderFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_ehci_local.h")
	drvUsbEHCILocalHeaderFile.setOutputName("drv_usb_ehci_local.h")
	drvUsbEHCILocalHeaderFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbEHCILocalHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src")
	drvUsbEHCILocalHeaderFile.setType("HEADER")
	drvUsbEHCILocalHeaderFile.setOverwrite(True)

	# OHCI Header file 
	drvUsbOHCIHeaderFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_HEADER_FILE_OHCI", None)
	drvUsbOHCIHeaderFile.setSourcePath(usbDriverPath + "uhp/drv_usb_ohci.h")
	drvUsbOHCIHeaderFile.setOutputName("drv_usb_ohci.h")
	drvUsbOHCIHeaderFile.setDestPath(usbDriverProjectPath + "uhp/")
	drvUsbOHCIHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/")
	drvUsbOHCIHeaderFile.setType("HEADER")
	drvUsbOHCIHeaderFile.setOverwrite(True)
	drvUsbOHCIHeaderFile.setEnabled(True)
	
	# Add OHCI Local Header file 
	drvUsbOHCILocalHeaderFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_HEADER_FILE_OHCI_LOCAL", None)
	drvUsbOHCILocalHeaderFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_ohci_local.h")
	drvUsbOHCILocalHeaderFile.setOutputName("drv_usb_ohci_local.h")
	drvUsbOHCILocalHeaderFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbOHCILocalHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src")
	drvUsbOHCILocalHeaderFile.setType("HEADER")
	drvUsbOHCILocalHeaderFile.setOverwrite(True)

	# OHCI SFR Header file 
	drvUsbUhpOHCISFRHeaderFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_HEADER_FILE_OHCI_SFR", None)
	drvUsbUhpOHCISFRHeaderFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_uhp_ohci_registers.h")
	drvUsbUhpOHCISFRHeaderFile.setOutputName("drv_usb_uhp_ohci_registers.h")
	drvUsbUhpOHCISFRHeaderFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbUhpOHCISFRHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src/")
	drvUsbUhpOHCISFRHeaderFile.setType("HEADER")
	drvUsbUhpOHCISFRHeaderFile.setOverwrite(True)
	drvUsbUhpOHCISFRHeaderFile.setEnabled(True)
	

	usbHostControllerDriverHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_host_client_driver.h', usbDriverComponent, usbHostControllerDriverHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHostHubInterfaceHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_host_hub_interface.h', usbDriverComponent, usbHostHubInterfaceHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHostHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_host.h', usbDriverComponent, usbHostHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHubHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_hub.h', usbDriverComponent, usbHubHeaderFile, "middleware/", "/usb/", True, None)
	
	usbEHCIHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_ehci.h', usbDriverComponent, usbEHCIHeaderFile, "middleware/", "/usb/", True, None)
	
	usbOHCIHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	addFileName('usb_ohci.h', usbDriverComponent, usbOHCIHeaderFile, "middleware/", "/usb/", True, None)
	
	# EHCI Source file 
	drvUsbHsV1HostSourceFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_SOURCE_FILE_EHCI", None)
	drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_ehci.c")
	drvUsbHsV1HostSourceFile.setOutputName("drv_usb_ehci.c")
	drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src/")
	drvUsbHsV1HostSourceFile.setType("SOURCE")
	drvUsbHsV1HostSourceFile.setOverwrite(True)
	drvUsbHsV1HostSourceFile.setEnabled(True)
	
	# OHCI Source file 
	drvUsbHostOhciSourceFile = usbDriverComponent.createFileSymbol("DRV_USB_UHPHS_SOURCE_FILE_OHCI", None)
	drvUsbHostOhciSourceFile.setSourcePath(usbDriverPath + "uhp/src/drv_usb_ohci.c")
	drvUsbHostOhciSourceFile.setOutputName("drv_usb_ohci.c")
	drvUsbHostOhciSourceFile.setDestPath(usbDriverProjectPath + "uhp/src")
	drvUsbHostOhciSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "uhp/src/")
	drvUsbHostOhciSourceFile.setType("SOURCE")
	drvUsbHostOhciSourceFile.setOverwrite(True)
	drvUsbHostOhciSourceFile.setEnabled(True)

		
def destroyComponent(usbDriverComponent):
	Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
	Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})
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
	
def blUSBDriverEHCI(usbSymbolSource, event):
	usbSymbolSource.setVisible(event["value"])
	
def blUSBDriverOHCI(usbSymbolSource, event):
	usbSymbolSource.setVisible(event["value"])
		
def onDependentComponentAdded(ownerComponent, dependencyID, dependentComponent):
	pass
	
def blUsbVbusPinName(usbSymbolSource, event):
	if (event["value"] == True):
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
	
