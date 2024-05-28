 
"""*****************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
usbOpMode = None 
drvUsbHsV1DeviceSourceFile = None 
drvUsbHsV1HostSourceFile = None 
addDrvUsbDeviceFile = None 
addDrvUsbHostFile = None 
usbHostControllerList = None
usbHostControllerEntryFile = None
usbSpeed = None 
usbDriverHsConnectedListUpperLayer = []

def blusbHostVbusEnable(usbSymbolSource, event):
	if (event["value"] == "Device"):
		usbSymbolSource.setVisible(False)
	else:
		usbSymbolSource.setVisible(True)

def blUsbHostVbusEnablePinName(usbSymbolSource, event):
	if (event["value"] == True):
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)


def blUSBDriverOpModeChanged(symbol, event):
	global addDrvUsbDeviceFile
	global addDrvUsbHostFile
	global usbOpMode
	if (event["value"] == "Device"):
		addDrvUsbDeviceFile.setValue(True)
		addDrvUsbHostFile.setValue(False)
	elif (event["value"] == "Host"):
		addDrvUsbDeviceFile.setValue(False)
		addDrvUsbHostFile.setValue(True)
	elif (event["value"] == "Dual Role"):
		addDrvUsbDeviceFile.setValue(True)
		addDrvUsbHostFile.setValue(True)
	usbOpMode.setValue(event["value"])
	args = {"operationMode":event["value"]}
	res = Database.sendMessage("drv_usbhs_index", "UPDATE_OPERATION_MODE_COMMON", args)

def genRtosTask(symbol, event):
	if event["value"] != "BareMetal":
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)

# This function is called when the Driver component is connected 
def onAttachmentConnected(source, target):
	global usbHostControllerList
	global usbHostControllerEntryFile
	global usbDriverHsConnectedListUpperLayer
	global usbSpeed
	global usbOpMode
	localComponent = source["component"]

	# This is the Capability of the local Component 
	connectID = source["id"]

	# Dependency ID of the remote component 
	targetID = target["id"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()

	if (connectID == "DRV_USB" and targetID == "usb_driver_dependency"):
		# This indicates an upper layer component (USB Host or Device) is connected 
		# Save the remoteID component. 
		usbDriverHsConnectedListUpperLayer.append(remoteID)
        # Notify the USB Device Layer about the speed 
		args = {"usbDriverSpeed":usbSpeed.getValue()}
		res = Database.sendMessage(remoteID, "USB_DEVICE_UPDATE_SPEED", args)

	if (remoteID == "usb_host"):
		usbHostControllerEntryFile.setEnabled(True)
		usbOpMode.setValue("Host")
	if remoteID.startswith("usb_device"):
		usbOpMode.setValue("Device")
	if connectID == "usb_peripheral_dependency":
		usbPLIB = localComponent.getSymbolByID("DRV_USB_PLIB")
		usbPLIB.clearValue()
		usbPLIB.setValue(remoteID.upper())
		
		if usbPLIB.getValue() == "PERIPHERAL_USB_0":
			# Update USBHS0 General Interrupt Handler
			Database.clearSymbolValue("core", "USBHS0_INTERRUPT_ENABLE")
			Database.clearSymbolValue("core", "USBHS0_INTERRUPT_HANDLER_LOCK")
			Database.clearSymbolValue("core", "USBHS0_INTERRUPT_HANDLER")
			Database.setSymbolValue("core", "USBHS0_INTERRUPT_ENABLE", True, 2)
			Database.setSymbolValue("core", "USBHS0_INTERRUPT_HANDLER_LOCK", True, 2)
			Database.setSymbolValue("core", "USBHS0_INTERRUPT_HANDLER", "DRV_USBHS0_Handler", 2)
			Database.setSymbolValue("core", "USBHS0_CLOCK_ENABLE", True, 1)
		elif usbPLIB.getValue() == "PERIPHERAL_USB_1":
			# Update USBHS1 General Interrupt Handler
			Database.clearSymbolValue("core", "USBHS1_INTERRUPT_ENABLE")
			Database.clearSymbolValue("core", "USBHS1_INTERRUPT_HANDLER_LOCK")
			Database.clearSymbolValue("core", "USBHS1_INTERRUPT_HANDLER")
			Database.setSymbolValue("core", "USBHS1_INTERRUPT_ENABLE", True, 2)
			Database.setSymbolValue("core", "USBHS1_INTERRUPT_HANDLER_LOCK", True, 2)
			Database.setSymbolValue("core", "USBHS1_INTERRUPT_HANDLER", "DRV_USBHS1_Handler", 2)
			Database.setSymbolValue("core", "USBHS1_CLOCK_ENABLE", True, 1)


def onAttachmentDisconnected(source, target):
	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	if (connectID == "DRV_USB" and targetID == "usb_driver_dependency"):
		# This indicates an upper layer component (USB Host or Device) is connected 
		# Save the remoteID component. 
		usbDriverHsConnectedListUpperLayer.remove(remoteID)

	if connectID == "usb_peripheral_dependency":
		usbPLIB = localComponent.getSymbolByID("DRV_USB_PLIB")
		if usbPLIB.getValue() == "PERIPHERAL_USB_0":
			Database.clearSymbolValue("core", "USBHS0_INTERRUPT_ENABLE")
			Database.clearSymbolValue("core", "USBHS0_INTERRUPT_HANDLER_LOCK")
			Database.clearSymbolValue("core", "USBHS0_INTERRUPT_HANDLER")
		elif usbPLIB.getValue() == "PERIPHERAL_USB_1":
			# Update USB General Interrupt Handler
			Database.clearSymbolValue("core", "USBHS1_INTERRUPT_ENABLE")
			Database.clearSymbolValue("core", "USBHS1_INTERRUPT_HANDLER_LOCK")
			Database.clearSymbolValue("core", "USBHS1_INTERRUPT_HANDLER")
			
def blUsbOperatioMode(symbol, event):
	args = {"operationMode":event["value"]}
	res = Database.sendMessage("drv_usbhs_index", "UPDATE_OPERATION_MODE_COMMON", args)
	
	
def dependencyStatus(symbol, event):
	if (event["value"] == False):
		symbol.setVisible(True)
	else :
		symbol.setVisible(False)

def blUSBDriverSpeedChanged(symbol, event):
	# This callback function is involked when the user chages the USB Driver 
	# Speed from the Configuration settings. The new speed setting must be 
	# notified to the connected USB Device Layer instance. 

	# Brodcast the speed change to all the Components connected to this driver.  
	for component in usbDriverHsConnectedListUpperLayer: 
		args = {"usbDriverSpeed":usbSpeed.getValue()}
		res = Database.sendMessage(component, "USB_DEVICE_UPDATE_SPEED", args)

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

def instantiateComponent(usbDriverComponent, index):
	global usbOpMode
	global drvUsbHsV1DeviceSourceFile
	global usbHostControllerEntryFile
	global usbSpeed
	global addDrvUsbDeviceFile
	global addDrvUsbHostFile
	usbDriverSourcePath = "ubshs"
	
	usbDriverIndex = usbDriverComponent.createIntegerSymbol("INDEX", None)
	usbDriverIndex.setVisible(False)
	usbDriverIndex.setDefaultValue(index)
	
	res = Database.activateComponents(["HarmonyCore"])
	
	usbPLIB = usbDriverComponent.createStringSymbol("DRV_USB_PLIB", None)
	usbPLIB.setLabel("USB Peripheral used")
	usbPLIB.setReadOnly(True)
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CZ", "PIC32CK"]):
		res = Database.activateComponents(["sys_time"])

	# USB Driver Speed selection
	usbSpeed = usbDriverComponent.createComboSymbol("USB_SPEED", None, listUsbSpeed)
	usbSpeed.setLabel("USB Speed Selection")
	usbSpeed.setVisible(True)
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

	# USB Driver Operation mode selection 
	usbOpMode = usbDriverComponent.createComboSymbol("USB_OPERATION_MODE", None, listUsbOperationMode)
	usbOpMode.setLabel("USB Mode Selection")
	usbOpMode.setVisible(True)
        helpText = '''This configuration controls the operational mode of the USB controller. If set to Host, the controller
        operates in USB Host mode. If configured as Device, the controller operates as USB Device. If configured for 
        Dual Role, the application can switch the role of controller between Host and Device during the operations of the 
        controller. When configured for Dual Role, application must explicitly set the startup operation mode (Device or Host)
        of the controller using available USB Device and Host Stack API.'''
	usbOpMode.setDescription(helpText)
	usbOpMode.setDefaultValue("Device")
	usbOpMode.setReadOnly(False)
	usbOpMode.setUseSingleDynamicValue(True)
	usbOpMode.setDependencies(blUSBDriverOpModeChanged, ["USB_OPERATION_MODE"])
	
	addDrvUsbDeviceFile = usbDriverComponent.createBooleanSymbol("ENABLE_DRV_USB_DEVICE_SOURCE", None)
	addDrvUsbDeviceFile.setLabel("Add USB Driver Device Mode File")
	addDrvUsbDeviceFile.setVisible(False)
	addDrvUsbDeviceFile.setDefaultValue(True)
	
	addDrvUsbHostFile = usbDriverComponent.createBooleanSymbol("ENABLE_DRV_USB_HOST_SOURCE", None)
	addDrvUsbHostFile.setLabel("Add USB Driver Host Mode File")
	addDrvUsbHostFile.setVisible(False)
	addDrvUsbHostFile.setDefaultValue(False)
	
	usbHostVbusEnable = usbDriverComponent.createBooleanSymbol("USB_HOST_VBUS_ENABLE", usbOpMode)
	usbHostVbusEnable.setLabel("Generate VBUS Enable Function")
        helpText = '''Enable this option if a VBUS control (enable/disable) function must be generated. This should be enabled 
        if the board contains a VBUS switch that controls VBUS supply to the device and the switch can be controlled by a 
        GPIO pin. The GPIO pin configuration must be peformned in the MHC pin mappping table. The driver code will configured
        to call the generated function when port power is to be controlled. '''
	usbHostVbusEnable.setDescription(helpText)
	usbHostVbusEnable.setVisible(False)
	usbHostVbusEnable.setDefaultValue(True)
	usbHostVbusEnable.setUseSingleDynamicValue(True)
	usbHostVbusEnable.setDependencies(blusbHostVbusEnable, ["USB_OPERATION_MODE"])
	
	
	usbHostVbusEnableFunctionName = usbDriverComponent.createStringSymbol("USB_HOST_VBUS_ENABLE_PIN_NAME", usbHostVbusEnable)
	usbHostVbusEnableFunctionName.setLabel("VBUS Enable Pin Name")
	usbHostVbusEnableFunctionName.setDefaultValue("VBUS_AH")
        helpText = '''Specify the name of the GPIO Pin that controls the VBUS switch. The name is specified in the MHC
        Pin Mapping table'''
        usbHostVbusEnableFunctionName.setDescription(helpText)
	usbHostVbusEnableFunctionName.setVisible(True)
	usbHostVbusEnableFunctionName.setDependencies(blUsbHostVbusEnablePinName, ["USB_HOST_VBUS_ENABLE"])

	# USB Driver Index - This symbol actually should get set from a Driver dependency connected callback. 
	# This is temporary work around to initialize using hard coded values. 
	usbDeviceDriverIndex = usbDriverComponent.createStringSymbol("CONFIG_USB_DRIVER_INDEX", None)
	usbDeviceDriverIndex.setVisible(False)
	usbDeviceDriverIndex.setDefaultValue("DRV_USBHS_INDEX_" + str(index))
	
	# USB Driver Interface -  This symbol actually should get set from a Driver dependency connected callback. 
	# This is temporary work around to initialize using hard coded values. 
	usbDeviceDriverInterface = usbDriverComponent.createStringSymbol("CONFIG_USB_DRIVER_INTERFACE", None)
	usbDeviceDriverInterface.setVisible(False)
	usbDeviceDriverInterface.setDefaultValue("DRV_USBHS_HOST_INTERFACE")

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
	usbDriverRTOSStackSize.setMin(0)
	usbDriverRTOSStackSize.setReadOnly(True)

	usbDriverRTOSTaskPriority = usbDriverComponent.createIntegerSymbol("USB_DRIVER_RTOS_TASK_PRIORITY", usbDriverRTOSMenu)
	usbDriverRTOSTaskPriority.setLabel("Task Priority")
	usbDriverRTOSTaskPriority.setDefaultValue(1)
	usbDriverRTOSTaskPriority.setMin(0)

	usbDriverRTOSTaskDelay = usbDriverComponent.createBooleanSymbol("USB_DRIVER_RTOS_USE_DELAY", usbDriverRTOSMenu)
	usbDriverRTOSTaskDelay.setLabel("Use Task Delay?")
	usbDriverRTOSTaskDelay.setDefaultValue(True)

	usbDriverRTOSTaskDelayVal = usbDriverComponent.createIntegerSymbol("USB_DRIVER_RTOS_DELAY", usbDriverRTOSMenu)
	usbDriverRTOSTaskDelayVal.setLabel("Task Delay")
	usbDriverRTOSTaskDelayVal.setDefaultValue(10) 
	usbDriverRTOSTaskDelayVal.setMin(0)
	usbDriverRTOSTaskDelayVal.setVisible((usbDriverRTOSTaskDelay.getValue() == True))
	usbDriverRTOSTaskDelayVal.setDependencies(setVisible, ["USB_DRIVER_RTOS_USE_DELAY"])
	
	configName = Variables.get("__CONFIGURATION_NAME")

	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32CZ", "PIC32CK"]):
		sourcePath = "templates/driver/usbhs_multi/"

	################################################
	# system_definitions.h file for USB Driver
	################################################
	usbDriverSystemDefIncludeFile = usbDriverComponent.createFileSymbol("DRV_USBHS_SYS_DEF_INCLUDES_USBHS_MULTI", None)
	usbDriverSystemDefIncludeFile.setType("STRING")
	usbDriverSystemDefIncludeFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDriverSystemDefIncludeFile.setSourcePath(sourcePath + "system_definitions.h.driver_includes.ftl")
	usbDriverSystemDefIncludeFile.setMarkup(True)

	usbDriverSystemDefObjFile = usbDriverComponent.createFileSymbol("DRV_USBHS_SYS_DEF_OBJ_USBHS_MULTI", None)
	usbDriverSystemDefObjFile.setType("STRING")
	usbDriverSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
	usbDriverSystemDefObjFile.setSourcePath(sourcePath + "system_definitions.h.driver_objects.ftl")
	usbDriverSystemDefObjFile.setMarkup(True)

	################################################
	# system_init.c file for USB Driver
	################################################
	usbDriverSystemInitDataFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemInitDataFile.setType("STRING")
	usbDriverSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA")
	usbDriverSystemInitDataFile.setSourcePath( sourcePath +"system_init_c_driver_data.ftl")
	usbDriverSystemInitDataFile.setMarkup(True)
	
	##############################################################
	# Controller entry
	##############################################################
	usbHostControllerEntryFile = usbDriverComponent.createFileSymbol("USB_HOST_CONTROLLER_ENTRY_FILE", None)
	usbHostControllerEntryFile.setType("STRING")
	usbHostControllerEntryFile.setOutputName("usb_host.LIST_USB_HOST_CONTROLLER_ENTRY")
	usbHostControllerEntryFile.setSourcePath(sourcePath +"system_init_c_host_controller_function.ftl")
	usbHostControllerEntryFile.setMarkup(True)
	usbHostControllerEntryFile.setEnabled(False)
	
	
	usbDriverSystemInitCallsFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemInitCallsFile.setType("STRING")
	usbDriverSystemInitCallsFile.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
	usbDriverSystemInitCallsFile.setSourcePath(sourcePath +"system_init_c_driver_calls.ftl")
	usbDriverSystemInitCallsFile.setMarkup(True)

	################################################
	# system_interrupt.c file for USB Driver
	################################################
	usbDriverSystemInterruptFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemInterruptFile.setType("STRING")
	usbDriverSystemInterruptFile.setOutputName("core.LIST_SYSTEM_INTERRUPT_HANDLERS")
	usbDriverSystemInterruptFile.setSourcePath(sourcePath +"system_interrupt_c_driver.ftl")
	usbDriverSystemInterruptFile.setMarkup(True)


	################################################
	# system_tasks.c file for USB Driver
	################################################
	usbDriverSystemTasksFile = usbDriverComponent.createFileSymbol(None, None)
	usbDriverSystemTasksFile.setType("STRING")
	usbDriverSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
	usbDriverSystemTasksFile.setSourcePath(sourcePath +"system_tasks_c_driver.ftl")
	usbDriverSystemTasksFile.setMarkup(True)

	################################################
	# ISR Generation file for USB Driver
	################################################
	usbDriverIsrFile = usbDriverComponent.createFileSymbol("DRV_USB_ISR", None)
	usbDriverIsrFile.setType("STRING")
	usbDriverIsrFile.setOutputName("drv_usbhs_index.LIST_DRV_USB_ISR_ENTRY")
	usbDriverIsrFile.setSourcePath( sourcePath + "drv_usb_isr.ftl")
	usbDriverIsrFile.setMarkup(True)

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
	drvUsbHeaderFile.setSourcePath(usbDriverPath + "drv_usb.h.ftl")
	drvUsbHeaderFile.setMarkup(True)
	drvUsbHeaderFile.setOutputName("drv_usb.h")
	drvUsbHeaderFile.setDestPath(usbDriverProjectPath)
	drvUsbHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath)
	drvUsbHeaderFile.setType("HEADER")
	drvUsbHeaderFile.setOverwrite(True)

	drvUsbExternalDependenciesFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbExternalDependenciesFile.setSourcePath(usbDriverPath + "drv_usb_external_dependencies.h.ftl")
	drvUsbExternalDependenciesFile.setMarkup(True)
	drvUsbExternalDependenciesFile.setOutputName("drv_usb_external_dependencies.h")
	drvUsbExternalDependenciesFile.setDestPath(usbDriverProjectPath)
	drvUsbExternalDependenciesFile.setProjectPath("config/" + configName + usbDriverProjectPath)
	drvUsbExternalDependenciesFile.setType("HEADER")
	drvUsbExternalDependenciesFile.setOverwrite(True)

	drvUsbHsV1HeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1HeaderFile.setSourcePath(usbDriverPath + "usbhsv2/drv_usbhs.h.ftl")
	drvUsbHsV1HeaderFile.setMarkup(True)
	drvUsbHsV1HeaderFile.setOutputName("drv_usbhs.h")
	drvUsbHsV1HeaderFile.setDestPath(usbDriverProjectPath+ "usbhs")
	drvUsbHsV1HeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath+ "usbhs")
	drvUsbHsV1HeaderFile.setType("HEADER")
	drvUsbHsV1HeaderFile.setOverwrite(True)

	drvUsbHsV1VarMapHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1VarMapHeaderFile.setSourcePath(usbDriverPath + "usbhsv2/src/drv_usbhs_variant_mapping.h.ftl")
	drvUsbHsV1VarMapHeaderFile.setMarkup(True)
	drvUsbHsV1VarMapHeaderFile.setOutputName("drv_usbhs_variant_mapping.h")
	drvUsbHsV1VarMapHeaderFile.setDestPath(usbDriverProjectPath + "usbhs/src")
	drvUsbHsV1VarMapHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhs/src")
	drvUsbHsV1VarMapHeaderFile.setType("HEADER")
	drvUsbHsV1VarMapHeaderFile.setOverwrite(True)
	drvUsbHsV1VarMapHeaderFile.setMarkup(True)


	drvUsbHsV1LocalHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1LocalHeaderFile.setSourcePath(usbDriverPath + "usbhsv2/src/drv_usbhs_local.h.ftl")
	drvUsbHsV1LocalHeaderFile.setMarkup(True)
	drvUsbHsV1LocalHeaderFile.setOutputName("drv_usbhs_local.h")
	drvUsbHsV1LocalHeaderFile.setDestPath(usbDriverProjectPath + "usbhs/src")
	drvUsbHsV1LocalHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhs/src")
	drvUsbHsV1LocalHeaderFile.setType("HEADER")
	drvUsbHsV1LocalHeaderFile.setOverwrite(True)


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


	drvUsbHsV1DeviceSourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1DeviceSourceFile.setSourcePath(usbDriverPath + "usbhsv2/src/drv_usbhs_device.c.ftl")
	drvUsbHsV1DeviceSourceFile.setMarkup(True)
	drvUsbHsV1DeviceSourceFile.setOutputName("drv_usbhs_device.c")
	drvUsbHsV1DeviceSourceFile.setDestPath(usbDriverProjectPath + "usbhs/src")
	drvUsbHsV1DeviceSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhs/src/")
	drvUsbHsV1DeviceSourceFile.setType("SOURCE")
	drvUsbHsV1DeviceSourceFile.setOverwrite(True)
	drvUsbHsV1DeviceSourceFile.setDependencies(blDrvUsbHsV1DeviceSourceFile, ["USB_OPERATION_MODE"])
		
	drvUsbHsV1HostSourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "usbhsv2/src/drv_usbhs_host.c.ftl")
	drvUsbHsV1HostSourceFile.setMarkup(True)
	drvUsbHsV1HostSourceFile.setOutputName("drv_usbhs_host.c")
	drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "usbhs/src")
	drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhs/src/")
	drvUsbHsV1HostSourceFile.setType("SOURCE")
	drvUsbHsV1HostSourceFile.setOverwrite(True)
	drvUsbHsV1HostSourceFile.setEnabled(False)
	drvUsbHsV1HostSourceFile.setDependencies(blDrvUsbHsV1HostSourceFile, ["USB_OPERATION_MODE"])
	
	# Add USBHS PLIB files for PIC32MZ 
	plib_usbhs_h = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('plib_usbhs.h', usbDriverComponent, plib_usbhs_h, usbDriverPath + "usbhsv2/src/", usbDriverProjectPath + "usbhs/src", True, None)
		
	plib_usbhs_header_h = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('plib_usbhs_header.h', usbDriverComponent, plib_usbhs_header_h, usbDriverPath + "usbhsv2/src/", usbDriverProjectPath + "usbhs/src", True, None)
	
	usbhs_registers_h = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_registers.h', usbDriverComponent, usbhs_registers_h, usbDriverPath + "usbhsv2/src/", usbDriverProjectPath + "usbhs/src", True, None)
		
	usbhs_registers_h_templates = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_registers.h', usbDriverComponent, usbhs_registers_h_templates, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
		
	usbhs_ClockResetControl_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_ClockResetControl_Default.h', usbDriverComponent, usbhs_ClockResetControl_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_ClockResetControl_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_ClockResetControl_Unsupported.h', usbDriverComponent, usbhs_ClockResetControl_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_EndpointFIFO_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_EndpointFIFO_Default.h', usbDriverComponent, usbhs_EndpointFIFO_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_EndpointFIFO_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_EndpointFIFO_Unsupported.h', usbDriverComponent, usbhs_EndpointFIFO_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_EndpointOperations_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_EndpointOperations_Default.h', usbDriverComponent, usbhs_EndpointOperations_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_EndpointOperations_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_EndpointOperations_Unsupported.h', usbDriverComponent, usbhs_EndpointOperations_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_EP0Status_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_EP0Status_Default.h', usbDriverComponent, usbhs_EP0Status_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_EP0Status_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_EP0Status_Unsupported.h', usbDriverComponent, usbhs_EP0Status_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_HighSpeedSupport_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_HighSpeedSupport_Default.h', usbDriverComponent, usbhs_HighSpeedSupport_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_HighSpeedSupport_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_HighSpeedSupport_Unsupported.h', usbDriverComponent, usbhs_HighSpeedSupport_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_Interrupts_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_Interrupts_Default.h', usbDriverComponent, usbhs_Interrupts_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_Interrupts_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_Interrupts_Unsupported.h', usbDriverComponent, usbhs_Interrupts_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_ModuleControl_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_ModuleControl_Default.h', usbDriverComponent, usbhs_ModuleControl_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_ModuleControl_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_ModuleControl_Unsupported.h', usbDriverComponent, usbhs_ModuleControl_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_RxEPStatus_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_RxEPStatus_Default.h', usbDriverComponent, usbhs_RxEPStatus_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_RxEPStatus_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_RxEPStatus_Unsupported.h', usbDriverComponent, usbhs_RxEPStatus_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_SoftReset_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_SoftReset_Default.h', usbDriverComponent, usbhs_SoftReset_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_SoftReset_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_SoftReset_Unsupported.h', usbDriverComponent, usbhs_SoftReset_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_TxEPStatus_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_TxEPStatus_Default.h', usbDriverComponent, usbhs_TxEPStatus_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_TxEPStatus_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_TxEPStatus_Unsupported.h', usbDriverComponent, usbhs_TxEPStatus_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_USBIDControl_Default = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_USBIDControl_Default.h', usbDriverComponent, usbhs_USBIDControl_Default, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
	usbhs_USBIDControl_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
	addFileName('usbhs_USBIDControl_Unsupported.h', usbDriverComponent, usbhs_USBIDControl_Unsupported, usbDriverPath + "usbhsv2/src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)

	
# all files go into src/
def addFileName(fileName, component, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
	symbol.setProjectPath("config/" + configName1 + destPath)
	symbol.setSourcePath(srcPath + fileName + ".ftl")
	symbol.setMarkup(True)
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
	if (event["value"] == "Device") or (event["value"] == "Dual Role"):
		usbSymbolSource.setEnabled(True)
	elif (event["value"] == "Host"):
		usbSymbolSource.setEnabled(False)

def blDrvUsbHsV1HostSourceFile (usbSymbolSource, event):
	if (event["value"] == "Device"):
		usbSymbolSource.setEnabled(False)
	elif (event["value"] == "Host") or (event["value"] == "Dual Role"):
		usbSymbolSource.setEnabled(True)

def blUSBDriverOperationModeDevice(usbSymbolSource, event):
	if (event["value"] == "Host"):
		usbSymbolSource.setVisible(False)
	else:
		usbSymbolSource.setVisible(False)

def blUSBDriverOperationModeChanged(usbSymbolSource, event):
	if (event["value"] == "Device"):
		usbSymbolSource.setVisible(False)
	else:
		usbSymbolSource.setVisible(True)

def blUsbVbusPinName(usbSymbolSource, event):
	if (event["value"] == True):
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)

