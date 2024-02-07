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
usbOpMode = None 
drvUsbHsV1DeviceSourceFile = None 
drvUsbHsV1HostSourceFile = None 
addDrvUsbDeviceFile = None 
addDrvUsbHostFile = None
usbSpeed = None 
usbHostControllerEntryFile = None


def genRtosTask(symbol, event):
	if event["value"] != "BareMetal":
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)


def handleMessage(messageID, args):	
	global usbOpMode
	if (messageID == "UPDATE_OPERATION_MODE"):
		usbOpMode.setValue(args["operationMode"])

def speedChanged(symbol, event):
	Database.clearSymbolValue("core", "PMC_SCER_USBCLK")
	Database.setSymbolValue("core", "PMC_SCER_USBCLK", True)

def blUSBDriverOpModeChanged(symbol, event):
	global addDrvUsbDeviceFile
	global addDrvUsbHostFile
	if (event["value"] == "Device"):
		addDrvUsbDeviceFile.setValue(True)
		addDrvUsbHostFile.setValue(False)
	elif (event["value"] == "Host"):
		addDrvUsbDeviceFile.setValue(False)
		addDrvUsbHostFile.setValue(True)
	elif (event["value"] == "Dual Role"):
		addDrvUsbDeviceFile.setValue(True)
		addDrvUsbHostFile.setValue(True)
		
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

# This function is called when the Driver component is connected 
def onAttachmentConnected(source, target):
	global usbSpeed
	global usbHostControllerEntryFile

	# This is the Capability of the local Component 
	connectID = source["id"]

	# Dependency ID of the remote component 
	targetID = target["id"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	if (connectID == "DRV_USB" and targetID == "usb_driver_dependency"):
		# This indicates an upper layer component (USB Host or Device) is connected 
		# Save the remoteID component. 
        # Notify the USB Device Layer about the speed 
		args = {"usbDriverSpeed":usbSpeed.getValue()}
		res = Database.sendMessage(remoteID, "USB_DEVICE_UPDATE_SPEED", args)
		if( remoteID == "usb_host"):
				if any(x in Variables.get("__PROCESSOR") for x in ["PIC32CK"]):
					usbHostControllerEntryFile.setEnabled(True)
            
def onAttachmentDisconnected(source, target):
	global usbHostControllerEntryFile
	connectID = source["id"]
	targetID = target["id"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	if (connectID == "DRV_USB" and targetID == "usb_driver_dependency" and remoteID == "usb_host"):
		if any(x in Variables.get("__PROCESSOR") for x in ["PIC32CK"]):
			usbHostControllerEntryFile.setEnabled(False)

def instantiateComponent(usbDriverComponent):	
	global usbOpMode
	global drvUsbHsV1DeviceSourceFile
	global drvUsbHsV1HostSourceFile
	global addDrvUsbDeviceFile
	global addDrvUsbHostFile
	global usbSpeed
	global usbHostControllerEntryFile
	
	res = Database.activateComponents(["HarmonyCore"])
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
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
	
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		usbVbusSense = usbDriverComponent.createBooleanSymbol("USB_DEVICE_VBUS_SENSE", usbOpMode)
		usbVbusSense.setLabel("Enable VBUS Sense")
                helpText = '''A Self Powered USB Device firmware must have some means to detect VBUS from Host. 
                A GPIO pin can be configured as an Input and connected to VBUS (through a resistor), and can be 
                used to detect when VBUS is high (host actively powering). This configuration instructs MHC to 
                generate a VBUS SENSE function. The GPIO pin name must be configured as in the below and should
                match the pin name in the MHC Pin Mapping Table'''
		usbVbusSense.setDescription(helpText)
		usbVbusSense.setVisible(True)
		usbVbusSense.setDescription("Select USB Operation Mode")
		usbVbusSense.setDefaultValue(True)
		usbVbusSense.setUseSingleDynamicValue(True)
		usbVbusSense.setDependencies(blUSBDriverOperationModeDevice, ["USB_OPERATION_MODE"])
		
		usbVbusSenseFunctionName = usbDriverComponent.createStringSymbol("USB_DEVICE_VBUS_SENSE_PIN_NAME", usbVbusSense)
		usbVbusSenseFunctionName.setLabel("VBUS SENSE Pin Name")
                helpText = '''Enter the name of the GPIO pin to be monitored for VBUS level. The pin must be named and configured
                in the MHC Pin Mapping tool. The name entered here must match the pin name given in the MHC Pin Mapping Tool'''
                usbVbusSenseFunctionName.setDescription(helpText)
		usbVbusSenseFunctionName.setDefaultValue("USB_VBUS_SENSE")
		usbVbusSenseFunctionName.setVisible(True)
		usbVbusSenseFunctionName.setDependencies(blUsbVbusPinName, ["USB_DEVICE_VBUS_SENSE"])
	
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
	
	# USB Driver Host mode Attach de-bounce duration 
	usbDriverHostAttachDebounce = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_ATTACH_DEBOUNCE_DURATION", usbOpMode)
	usbDriverHostAttachDebounce.setLabel("Attach De-bounce Duration (mSec)")
	usbDriverHostAttachDebounce.setVisible(False)
        helpText = '''Specify the time duration (in milliseconds) that the driver should wait after detecting the
        attach interrupt and before polling the attach interrupt again to check if the attach condition still exists.
        A longer duration slows down the Host Device Attach detection but allows for stable operation.'''
	usbDriverHostAttachDebounce.setDescription(helpText)
	usbDriverHostAttachDebounce.setDefaultValue(500)
	usbDriverHostAttachDebounce.setMin(0)
	usbDriverHostAttachDebounce.setDependencies(blUSBDriverOperationModeChanged, ["USB_OPERATION_MODE"])
	
	# USB Driver Host mode Reset Duration
	usbDriverHostResetDuration = usbDriverComponent.createIntegerSymbol("USB_DRV_HOST_RESET_DUARTION", usbOpMode)
	usbDriverHostResetDuration.setLabel("Reset Duration (mSec)")
	usbDriverHostResetDuration.setVisible(False)
        helpText = '''Specify the duration of the USB Bus Reset Signal. A value of 100 millisecond works 
        well. There may be cases where some USB devices require a shorter or longer reset duration time.'''
	usbDriverHostResetDuration.setDescription(helpText)
	usbDriverHostResetDuration.setDefaultValue(100)
	usbDriverHostResetDuration.setMin(0)
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
	
	############################################################################
    #### Dependency ####
    ############################################################################
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		# Update USB General Interrupt Handler 
		Database.setSymbolValue("core", "USBHS_INTERRUPT_ENABLE", True)
		Database.setSymbolValue("core", "USBHS_INTERRUPT_HANDLER_LOCK", True)
		Database.setSymbolValue("core", "USBHS_INTERRUPT_HANDLER", "DRV_USBHSV1_USBHS_Handler")

		# Initial settings for CLK and NVIC
		if Database.getSymbolValue("core", "PMC_CKGR_UCKR_UPLLEN") == False:
			Database.setSymbolValue("core", "PMC_CKGR_UCKR_UPLLEN", True)
		if Database.getSymbolValue("core", "USBHS_CLOCK_ENABLE") == False:
			Database.setSymbolValue("core", "USBHS_CLOCK_ENABLE", True)
		if Database.getSymbolValue("core", "PMC_SCER_USBCLK") == False:
			Database.setSymbolValue("core", "PMC_SCER_USBCLK", True)
	
		
		# Dependency Status
		usbhsSymClkEnComment = usbDriverComponent.createCommentSymbol("USBHS_CLK_ENABLE_COMMENT", None)
		usbhsSymClkEnComment.setVisible(False)
		usbhsSymClkEnComment.setLabel("Warning!!! USBHS Peripheral Clock is Disabled in Clock Manager")
		usbhsSymClkEnComment.setDependencies(dependencyStatus, ["core.PMC_ID_USBHS"])

		usbhsSymIntEnComment = usbDriverComponent.createCommentSymbol("USBHS_NVIC_ENABLE_COMMENT", None)
		usbhsSymIntEnComment.setVisible(False)
		usbhsSymIntEnComment.setLabel("Warning!!! USBHS Interrupt is Disabled in Interrupt Manager")
		usbhsSymIntEnComment.setDependencies(dependencyStatus, ["core." + "USBHS_INTERRUPT_ENABLE"])
		
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
		# Update USB General Interrupt Handler 
		Database.setSymbolValue("core", "USB_INTERRUPT_ENABLE", True)
		Database.setSymbolValue("core", "USB_INTERRUPT_HANDLER_LOCK", True)
		Database.setSymbolValue("core", "USB_INTERRUPT_HANDLER", "DRV_USBHS_InterruptHandler")
		
		# Update USB DMA Interrupt Handler 
		Database.setSymbolValue("core", "USB_DMA_INTERRUPT_ENABLE", True)
		Database.setSymbolValue("core", "USB_DMA_INTERRUPT_HANDLER_LOCK", True)
		Database.setSymbolValue("core", "USB_DMA_INTERRUPT_HANDLER", "DRV_USBHS_DMAInterruptHandler")

	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32CK"]):
		# Update USBHS0 General Interrupt Handler
		Database.clearSymbolValue("core", "USBHS_INTERRUPT_ENABLE")
		Database.clearSymbolValue("core", "USBHS_INTERRUPT_HANDLER_LOCK")
		Database.clearSymbolValue("core", "USBHS_INTERRUPT_HANDLER")
		Database.setSymbolValue("core", "USBHS_INTERRUPT_ENABLE", True, 2)
		Database.setSymbolValue("core", "USBHS_INTERRUPT_HANDLER_LOCK", True, 2)
		Database.setSymbolValue("core", "USBHS_INTERRUPT_HANDLER", "DRV_USBHS_Handler", 2)
		Database.setSymbolValue("core", "USBHS_CLOCK_ENABLE", True, 1)
	
	# Enable Driver common files 
	Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

	# Enable System common files. 
	Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})
	
	configName = Variables.get("__CONFIGURATION_NAME")
	
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		sourcePath = "templates/driver/usbhsv1/"
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		sourcePath = "templates/driver/usbhs/"
	
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
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32CK"]):
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
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
		sourcePathDriver = usbDriverPath + "usbhs/"
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32CK"]):
		sourcePathDriver = usbDriverPath + "usbhsv2/"
  
	drvUsbHsV1HeaderFile = usbDriverComponent.createFileSymbol(None, None)
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		drvUsbHsV1HeaderFile.setSourcePath(usbDriverPath + "usbhsv1/drv_usbhsv1.h.ftl")
		drvUsbHsV1HeaderFile.setMarkup(True)
		drvUsbHsV1HeaderFile.setOutputName("drv_usbhsv1.h")
		drvUsbHsV1HeaderFile.setDestPath(usbDriverProjectPath+ "usbhsv1")
		drvUsbHsV1HeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath+ "usbhsv1")
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		drvUsbHsV1HeaderFile.setSourcePath(sourcePathDriver + "drv_usbhs.h.ftl")
		drvUsbHsV1HeaderFile.setMarkup(True)
		drvUsbHsV1HeaderFile.setOutputName("drv_usbhs.h")
		drvUsbHsV1HeaderFile.setDestPath(usbDriverProjectPath+ "usbhs")
		drvUsbHsV1HeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath+ "usbhs")
	drvUsbHsV1HeaderFile.setType("HEADER")
	drvUsbHsV1HeaderFile.setOverwrite(True)
	
	drvUsbHsV1VarMapHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		drvUsbHsV1VarMapHeaderFile.setSourcePath(usbDriverPath + "usbhsv1/src/drv_usbhsv1_variant_mapping.h.ftl")
		drvUsbHsV1VarMapHeaderFile.setMarkup(True)
		drvUsbHsV1VarMapHeaderFile.setOutputName("drv_usbhsv1_variant_mapping.h")
		drvUsbHsV1VarMapHeaderFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
		drvUsbHsV1VarMapHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src")
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		drvUsbHsV1VarMapHeaderFile.setSourcePath(sourcePathDriver + "src/drv_usbhs_variant_mapping.h.ftl")
		drvUsbHsV1VarMapHeaderFile.setMarkup(True)
		drvUsbHsV1VarMapHeaderFile.setOutputName("drv_usbhs_variant_mapping.h")
		drvUsbHsV1VarMapHeaderFile.setDestPath(usbDriverProjectPath + "usbhs/src")
		drvUsbHsV1VarMapHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhs/src")
	drvUsbHsV1VarMapHeaderFile.setType("HEADER")
	drvUsbHsV1VarMapHeaderFile.setOverwrite(True)

	
	drvUsbHsV1LocalHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		drvUsbHsV1LocalHeaderFile.setSourcePath(usbDriverPath + "usbhsv1/src/drv_usbhsv1_local.h.ftl")
		drvUsbHsV1LocalHeaderFile.setMarkup(True)
		drvUsbHsV1LocalHeaderFile.setOutputName("drv_usbhsv1_local.h")
		drvUsbHsV1LocalHeaderFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
		drvUsbHsV1LocalHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src")
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		drvUsbHsV1LocalHeaderFile.setSourcePath(sourcePathDriver + "src/drv_usbhs_local.h.ftl")
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
	drvUsbHsV1SourceFile = usbDriverComponent.createFileSymbol("DRV_USB_SOURCE_FILE_COMMON", None)
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		drvUsbHsV1SourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1.c.ftl")
		drvUsbHsV1SourceFile.setMarkup(True)
		drvUsbHsV1SourceFile.setOutputName("drv_usbhsv1.c")
		drvUsbHsV1SourceFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
		drvUsbHsV1SourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src/")
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		drvUsbHsV1SourceFile.setSourcePath(sourcePathDriver + "src/drv_usbhs.c.ftl")
		drvUsbHsV1SourceFile.setMarkup(True)
		drvUsbHsV1SourceFile.setOutputName("drv_usbhs.c")
		drvUsbHsV1SourceFile.setDestPath(usbDriverProjectPath + "usbhs/src")
		drvUsbHsV1SourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhs/src/")
	drvUsbHsV1SourceFile.setType("SOURCE")
	drvUsbHsV1SourceFile.setOverwrite(True)
	
	drvUsbHsV1DeviceSourceFile = usbDriverComponent.createFileSymbol("DRV_USB_SOURCE_FILE_DEVICE", None)
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		drvUsbHsV1DeviceSourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1_device.c.ftl")
		drvUsbHsV1DeviceSourceFile.setMarkup(True)
		drvUsbHsV1DeviceSourceFile.setOutputName("drv_usbhsv1_device.c")
		drvUsbHsV1DeviceSourceFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
		drvUsbHsV1DeviceSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src/")
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		drvUsbHsV1DeviceSourceFile.setSourcePath(sourcePathDriver + "src/drv_usbhs_device.c.ftl")
		drvUsbHsV1DeviceSourceFile.setMarkup(True)
		drvUsbHsV1DeviceSourceFile.setOutputName("drv_usbhs_device.c")
		drvUsbHsV1DeviceSourceFile.setDestPath(usbDriverProjectPath + "usbhs/src")
		drvUsbHsV1DeviceSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhs/src/")
	drvUsbHsV1DeviceSourceFile.setType("SOURCE")
	drvUsbHsV1DeviceSourceFile.setOverwrite(True)
	drvUsbHsV1DeviceSourceFile.setEnabled(True)
	drvUsbHsV1DeviceSourceFile.setDependencies(blDrvUsbHsV1DeviceSourceFile, ["ENABLE_DRV_USB_DEVICE_SOURCE"])
	
	drvUsbHsV1HostSourceFile = usbDriverComponent.createFileSymbol("DRV_USB_SOURCE_FILE_HOST", None)
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1_host.c.ftl")
		drvUsbHsV1HostSourceFile.setMarkup(True)
		drvUsbHsV1HostSourceFile.setOutputName("drv_usbhsv1_host.c")
		drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "usbhsv1/src")
		drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhsv1/src/")
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		drvUsbHsV1HostSourceFile.setSourcePath(sourcePathDriver + "src/drv_usbhs_host.c.ftl")
		drvUsbHsV1HostSourceFile.setMarkup(True)
		drvUsbHsV1HostSourceFile.setOutputName("drv_usbhs_host.c")
		drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "usbhs/src")
		drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhs/src/")
	drvUsbHsV1HostSourceFile.setType("SOURCE")
	drvUsbHsV1HostSourceFile.setOverwrite(True)
	drvUsbHsV1HostSourceFile.setEnabled(False)
	drvUsbHsV1HostSourceFile.setDependencies(blDrvUsbHsV1HostSourceFile, ["ENABLE_DRV_USB_HOST_SOURCE"])

	# Add USBHS PLIB files for PIC32MZ 
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		plib_usbhs_h = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('plib_usbhs.h', usbDriverComponent, plib_usbhs_h, sourcePathDriver + "src/", usbDriverProjectPath + "usbhs/src", True, None)
		
		plib_usbhs_header_h = usbDriverComponent.createFileSymbol(None, None)	
		if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
			addFileName('plib_usbhs_header.h', usbDriverComponent, plib_usbhs_header_h, sourcePathDriver + "src/", usbDriverProjectPath + "usbhs/src", True, None)
		if any(x in Variables.get("__PROCESSOR") for x in ["PIC32CK"]):
			#addFileName('plib_usbhs_header.h', usbDriverComponent, plib_usbhs_header_h, usbDriverPath + "usbhs/" + "src/", usbDriverProjectPath + "usbhs/src", True, None)
			configNameTemp = Variables.get("__CONFIGURATION_NAME")
			plib_usbhs_header_h.setProjectPath("config/" + configNameTemp + usbDriverProjectPath + "usbhs/src")
			plib_usbhs_header_h.setSourcePath(sourcePathDriver + "src/" + 'plib_usbhs_header_pic32ck.h.ftl')
			plib_usbhs_header_h.setMarkup(True)
			plib_usbhs_header_h.setOutputName('plib_usbhs_header.h')
			plib_usbhs_header_h.setDestPath(usbDriverProjectPath + "usbhs/src")
			plib_usbhs_header_h.setType("HEADER")
			plib_usbhs_header_h.setEnabled(True)


		usbhs_registers_h = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_registers.h', usbDriverComponent, usbhs_registers_h, sourcePathDriver + "src/", usbDriverProjectPath + "usbhs/src", True, None)
		
		usbhs_registers_h_templates = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_registers.h', usbDriverComponent, usbhs_registers_h_templates, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
		
		usbhs_ClockResetControl_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_ClockResetControl_Default.h', usbDriverComponent, usbhs_ClockResetControl_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_ClockResetControl_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_ClockResetControl_Unsupported.h', usbDriverComponent, usbhs_ClockResetControl_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_EndpointFIFO_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_EndpointFIFO_Default.h', usbDriverComponent, usbhs_EndpointFIFO_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_EndpointFIFO_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_EndpointFIFO_Unsupported.h', usbDriverComponent, usbhs_EndpointFIFO_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_EndpointOperations_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_EndpointOperations_Default.h', usbDriverComponent, usbhs_EndpointOperations_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_EndpointOperations_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_EndpointOperations_Unsupported.h', usbDriverComponent, usbhs_EndpointOperations_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_EP0Status_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_EP0Status_Default.h', usbDriverComponent, usbhs_EP0Status_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_EP0Status_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_EP0Status_Unsupported.h', usbDriverComponent, usbhs_EP0Status_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_HighSpeedSupport_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_HighSpeedSupport_Default.h', usbDriverComponent, usbhs_HighSpeedSupport_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_HighSpeedSupport_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_HighSpeedSupport_Unsupported.h', usbDriverComponent, usbhs_HighSpeedSupport_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_Interrupts_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_Interrupts_Default.h', usbDriverComponent, usbhs_Interrupts_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_Interrupts_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_Interrupts_Unsupported.h', usbDriverComponent, usbhs_Interrupts_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_ModuleControl_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_ModuleControl_Default.h', usbDriverComponent, usbhs_ModuleControl_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_ModuleControl_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_ModuleControl_Unsupported.h', usbDriverComponent, usbhs_ModuleControl_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_RxEPStatus_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_RxEPStatus_Default.h', usbDriverComponent, usbhs_RxEPStatus_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_RxEPStatus_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_RxEPStatus_Unsupported.h', usbDriverComponent, usbhs_RxEPStatus_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_SoftReset_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_SoftReset_Default.h', usbDriverComponent, usbhs_SoftReset_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_SoftReset_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_SoftReset_Unsupported.h', usbDriverComponent, usbhs_SoftReset_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_TxEPStatus_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_TxEPStatus_Default.h', usbDriverComponent, usbhs_TxEPStatus_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_TxEPStatus_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_TxEPStatus_Unsupported.h', usbDriverComponent, usbhs_TxEPStatus_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_USBIDControl_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_USBIDControl_Default.h', usbDriverComponent, usbhs_USBIDControl_Default, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
		usbhs_USBIDControl_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbhs_USBIDControl_Unsupported.h', usbDriverComponent, usbhs_USBIDControl_Unsupported, sourcePathDriver + "src/templates/", usbDriverProjectPath + "usbhs/src/templates", True, None)
	
def destroyComponent(usbDriverComponent):
	Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
	Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})
	
# all files go into src/
def addFileName(fileName, component, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
	#filename = component.createFileSymbol(None, None)
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
		
def blDrvUsbHsV1DeviceSourceFile (symbol, event):
	symbol.setEnabled(event["value"])
		
def blDrvUsbHsV1HostSourceFile (symbol, event):
	symbol.setEnabled(event["value"])

def blUSBDriverOperationModeDevice(usbSymbolSource, event):
	if (event["value"] == "Host"):
		usbSymbolSource.setVisible(False)
	else:
		usbSymbolSource.setVisible(True)
		

def blusbHostVbusEnable(usbSymbolSource, event):
	if (event["value"] == "Device"):
		usbSymbolSource.setVisible(False)
	else:
		usbSymbolSource.setVisible(True)
	
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
		
def blUsbHostVbusEnablePinName(usbSymbolSource, event):
	if (event["value"] == True):
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
	
