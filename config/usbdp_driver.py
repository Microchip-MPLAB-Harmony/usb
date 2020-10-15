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
usbDebugLogs = 1
usbDriverPath = "driver/"
usbDriverProjectPath = "/driver/usb/"


# def speedChanged(symbol, event):
	# Database.clearSymbolValue("core", "PMC_SCER_USBCLK")
	# Database.setSymbolValue("core", "PMC_SCER_USBCLK", True, 2)

# def dependencyStatus(symbol, event):
	# if (event["value"] == False):
		# symbol.setVisible(True)
	# else :
		# symbol.setVisible(False)

# def blUSBDriverSpeedChanged(symbol, event):
	# Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_SPEED")
	# Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_SPEED", event["value"], 2)

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
	usbSpeed.setDescription("Select USB Operation Speed")
	usbSpeed.setDefaultValue("Full Speed")
	# usbSpeed.setDependencies(blUSBDriverSpeedChanged, ["USB_SPEED"])

	usbVbusSense = usbDriverComponent.createBooleanSymbol("USB_DEVICE_VBUS_SENSE", None)
	usbVbusSense.setLabel("Enable VBUS Sense")
	usbVbusSense.setDescription("A Self Powered USB Device firmware must have some means to detect VBUS from Host. A GPIO pin can be configured as an Input and connected to VBUS (through a resistor), and can be used to detect when VBUS is high (host actively powering). This configuration instructs MHC to generate a VBUS SENSE function. The GPIO pin name must be configured as in the below ")
	usbVbusSense.setVisible(True)
	usbVbusSense.setDefaultValue(True)

	usbVbusSenseFunctionName = usbDriverComponent.createStringSymbol("USB_DEVICE_VBUS_SENSE_PIN_NAME", usbVbusSense)
	usbVbusSenseFunctionName.setLabel("VBUS SENSE Pin Name")
	usbVbusSenseFunctionName.setDefaultValue("USB_VBUS_SENSE")
	usbVbusSenseFunctionName.setVisible(True)
	usbVbusSenseFunctionName.setDependencies(blUsbVbusPinName, ["USB_DEVICE_VBUS_SENSE"])

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
	usbDriverRTOSStackSize.setMin(0)

	usbDriverRTOSTaskPriority = usbDriverComponent.createIntegerSymbol("USB_DRIVER_RTOS_TASK_PRIORITY", usbDriverRTOSMenu)
	usbDriverRTOSTaskPriority.setLabel("Task Priority")
	usbDriverRTOSTaskPriority.setDefaultValue(1)
	usbDriverRTOSTaskPriority.setMin(1)

	usbDriverRTOSTaskDelay = usbDriverComponent.createBooleanSymbol("USB_DRIVER_RTOS_USE_DELAY", usbDriverRTOSMenu)
	usbDriverRTOSTaskDelay.setLabel("Use Task Delay?")
	usbDriverRTOSTaskDelay.setDefaultValue(True)

	usbDriverRTOSTaskDelayVal = usbDriverComponent.createIntegerSymbol("USB_DRIVER_RTOS_DELAY", usbDriverRTOSMenu)
	usbDriverRTOSTaskDelayVal.setLabel("Task Delay")
	usbDriverRTOSTaskDelayVal.setMin(0)
	usbDriverRTOSTaskDelayVal.setDefaultValue(10)
	usbDriverRTOSTaskDelayVal.setVisible((usbDriverRTOSTaskDelay.getValue() == True))
	usbDriverRTOSTaskDelayVal.setDependencies(setVisible, ["USB_DRIVER_RTOS_USE_DELAY"])

	############################################################################
	#### Dependency ####
	############################################################################
	# Update USB General Interrupt Handler
	Database.setSymbolValue("core", "UDP_INTERRUPT_ENABLE", True, 1)
	Database.setSymbolValue("core", "UDP_INTERRUPT_HANDLER_LOCK", True, 1)
	Database.setSymbolValue("core", "UDP_INTERRUPT_HANDLER", "DRV_USBDP_Handler", 1)

	if Database.getSymbolValue("core", "UDP_CLOCK_ENABLE") == False:
		Database.setSymbolValue("core", "UDP_CLOCK_ENABLE", True, 2)

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

	sourcePath = "templates/driver/usbdp/"

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
	usbDriverSystemInterruptFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemInterruptFile.setType("STRING")
	usbDriverSystemInterruptFile.setOutputName("core.LIST_SYSTEM_INTERRUPT_HANDLERS")
	usbDriverSystemInterruptFile.setSourcePath(sourcePath + "system_interrupt_c_driver.ftl")
	usbDriverSystemInterruptFile.setMarkup(True)


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

	drvUdpHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUdpHeaderFile.setSourcePath(usbDriverPath + "usbdp/drv_usbdp.h")
	drvUdpHeaderFile.setOutputName("drv_usbdp.h")
	drvUdpHeaderFile.setDestPath(usbDriverProjectPath+ "usbdp")
	drvUdpHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath+ "usbdp")
	drvUdpHeaderFile.setType("HEADER")
	drvUdpHeaderFile.setOverwrite(True)

	drvUdpVarMapHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUdpVarMapHeaderFile.setSourcePath(usbDriverPath + "usbdp/src/drv_usbdp_variant_mapping.h")
	drvUdpVarMapHeaderFile.setOutputName("drv_usbdp_variant_mapping.h")
	drvUdpVarMapHeaderFile.setDestPath(usbDriverProjectPath + "usbdp/src")
	drvUdpVarMapHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbdp/src")
	drvUdpVarMapHeaderFile.setType("HEADER")
	drvUdpVarMapHeaderFile.setOverwrite(True)

	drvUdpLocalHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUdpLocalHeaderFile.setSourcePath(usbDriverPath + "usbdp/src/drv_usbdp_local.h")
	drvUdpLocalHeaderFile.setOutputName("drv_usbdp_local.h")
	drvUdpLocalHeaderFile.setDestPath(usbDriverProjectPath + "usbdp/src")
	drvUdpLocalHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbdp/src")
	drvUdpLocalHeaderFile.setType("HEADER")
	drvUdpLocalHeaderFile.setOverwrite(True)

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
	drvUdpDeviceSourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUdpDeviceSourceFile.setSourcePath(usbDriverPath + "usbdp/src/dynamic/drv_usbdp_device.c")
	drvUdpDeviceSourceFile.setOutputName("drv_usbdp_device.c")
	drvUdpDeviceSourceFile.setDestPath(usbDriverProjectPath + "usbdp/src")
	drvUdpDeviceSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbdp/src/")
	drvUdpDeviceSourceFile.setType("SOURCE")
	drvUdpDeviceSourceFile.setOverwrite(True)

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

def blUsbVbusPinName(usbSymbolSource, event):
	if (event["value"] == True):
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)

