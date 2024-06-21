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
        helpText = '''While operating in Host Mode, this option configures the types (speed) of USB devices to be supported. 
        For example, setting this option to High Speed will allow the Host to support High, Full and Low Speed devices. 
        Setting this option to Full Speed will allow the Host to support only Full and Low Speed devices. If a High Speed
        device is attached, it will operate at Full Speed.

        In Device mode, this configuration select the operational speed of the device. If configured for High Speed mode, the
        device will signal Chirp after receiving the bus reset from the host. If configured for Full Speed mode, the device 
        will not signal Chirp and will operate at Full Speed only.'''

	usbSpeed.setVisible(True)
	usbSpeed.setDescription(helpText)
	usbSpeed.setDefaultValue("High Speed")
	usbSpeed.setDependencies(blUSBDriverSpeedChanged, ["USB_SPEED"])

	# USB Driver: DMA Maximum Transfer Size
	usbDMATransferSize = usbDriverComponent.createIntegerSymbol("USB_DMA_MAXIMUM_TRANSFER", None)
	usbDMATransferSize.setLabel("USB DMA Maximum Transfer Size")
	usbDMATransferSize.setVisible(True)
	usbDMATransferSize.setMin(1)
	helpText = '''Set maximum size for a DMA transfer, multiple of 64KB'''
	usbDMATransferSize.setDescription(helpText)
	usbDMATransferSize.setDefaultValue(2)

	# UDPHS DMA CONTROL REGISTER OFFSET
	usbDMAOffset = usbDriverComponent.createIntegerSymbol("CONFIG_USB_UDPHS_DMA_OFFSET", None)
	usbDMAOffset.setLabel("USB DMA Register Address Offset")
	usbDMAOffset.setVisible(False)
	modules = ATDF.getNode("/avr-tools-device-file/modules").getChildren()
	for module in range(len(modules)):
		if (modules[module].getAttribute("name")) == "UDPHS":
			register_groups = modules[module].getChildren()
			print(modules[module].getAttribute("name"), " Test")
			for register_group in range(len(register_groups)):
				if (register_groups[register_group].getAttribute("name") == "UDPHS"):
					sub_register_groups = register_groups[register_group].getChildren()
					for sub_register_group in range(len(sub_register_groups)):
						if (sub_register_groups[sub_register_group].getAttribute("name") == "UDPHS_DMA"):
							udphsDmaAdrresOffset = sub_register_groups[sub_register_group].getAttribute("offset")
	if udphsDmaAdrresOffset == "0x310":
		usbDMAOffset.setDefaultValue(0)
	elif udphsDmaAdrresOffset == "0x300":
		usbDMAOffset.setDefaultValue(1)
	else:
		print("USB DMA Register Address Offset is incorrect in the ATDF")



	# USB Driver: USB VBUS Sense
	usbVbusSense = usbDriverComponent.createBooleanSymbol("USB_DEVICE_VBUS_SENSE", None)
	usbVbusSense.setLabel("Enable VBUS Sense")
        helpText = '''A Self Powered USB Device firmware must have some means to detect VBUS from Host. A GPIO pin can be 
        configured as an Input and connected to VBUS (through a resistor), and can be used to detect when VBUS is high 
        (host actively powering). This configuration instructs MHC to generate a VBUS SENSE function. The GPIO pin name 
        must be configured as in the below '''
	usbVbusSense.setDescription(helpText)
	usbVbusSense.setVisible(True)
	usbVbusSense.setDefaultValue(True)
	
	usbVbusSenseFunctionName = usbDriverComponent.createStringSymbol("USB_DEVICE_VBUS_SENSE_PIN_NAME", usbVbusSense)
	usbVbusSenseFunctionName.setLabel("VBUS SENSE Pin Name")
	usbVbusSenseFunctionName.setDefaultValue("USB_VBUS_SENSE")
        helpText = '''Enter the name of the GPIO pin to be monitored for VBUS level. The pin must be named and configured
        in the MHC Pin Mapping tool. The name entered here must match the pin name given in the MHC Pin Mapping Tool'''
        usbVbusSenseFunctionName.setDescription(helpText)
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
	if any(x in Variables.get("__PROCESSOR") for x in [ "SAMA7"]):
		Database.setSymbolValue("core", "UDPHSA_INTERRUPT_ENABLE", True, 1)
		Database.setSymbolValue("core", "UDPHSA_INTERRUPT_HANDLER_LOCK", True, 1)
		Database.setSymbolValue("core", "UDPHSA_INTERRUPT_HANDLER", "DRV_USB_UDPHSA_Handler", 1)
	else :
		Database.setSymbolValue("core", "UDPHS_INTERRUPT_ENABLE", True, 1)
		Database.setSymbolValue("core", "UDPHS_INTERRUPT_HANDLER_LOCK", True, 1)
		Database.setSymbolValue("core", "UDPHS_INTERRUPT_HANDLER", "DRV_USB_UDPHS_Handler", 1)

	if Database.getSymbolValue("core", "UDPHS_CLOCK_ENABLE") == False:
		Database.setSymbolValue("core", "UDPHS_CLOCK_ENABLE", True, 2)	
				
	# Enable Driver common files 
	Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

	# Enable System common files. 
	Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})
	
	configName = Variables.get("__CONFIGURATION_NAME")
	
	sourcePath = "templates/driver/udphs/"
	
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
	
	drvUdphsHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUdphsHeaderFile.setSourcePath(usbDriverPath + "udphs/drv_usb_udphs.h.ftl")
	drvUdphsHeaderFile.setMarkup(True)
	drvUdphsHeaderFile.setOutputName("drv_usb_udphs.h")
	drvUdphsHeaderFile.setDestPath(usbDriverProjectPath+ "udphs")
	drvUdphsHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath+ "udphs")
	drvUdphsHeaderFile.setType("HEADER")
	drvUdphsHeaderFile.setOverwrite(True)
	
	drvUsbHsV1VarMapHeaderFile = usbDriverComponent.createFileSymbol(None, None)	
	drvUsbHsV1VarMapHeaderFile.setSourcePath(usbDriverPath + "udphs/src/drv_usb_udphs_variant_mapping.h.ftl")
	drvUsbHsV1VarMapHeaderFile.setMarkup(True)
	drvUsbHsV1VarMapHeaderFile.setOutputName("drv_usb_udphs_variant_mapping.h")
	drvUsbHsV1VarMapHeaderFile.setDestPath(usbDriverProjectPath + "udphs/src")
	drvUsbHsV1VarMapHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "udphs/src")
	drvUsbHsV1VarMapHeaderFile.setType("HEADER")
	drvUsbHsV1VarMapHeaderFile.setOverwrite(True)
	
	drvUsbHsV1LocalHeaderFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1LocalHeaderFile.setSourcePath(usbDriverPath + "udphs/src/drv_usb_udphs_local.h.ftl")
	drvUsbHsV1LocalHeaderFile.setMarkup(True)
	drvUsbHsV1LocalHeaderFile.setOutputName("drv_usb_udphs_local.h")
	drvUsbHsV1LocalHeaderFile.setDestPath(usbDriverProjectPath + "udphs/src")
	drvUsbHsV1LocalHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "udphs/src")
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
	drvUsbHsV1SourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1SourceFile.setSourcePath(usbDriverPath + "udphs/src/dynamic/drv_usb_udphs.c.ftl")
	drvUsbHsV1SourceFile.setMarkup(True)
	drvUsbHsV1SourceFile.setOutputName("drv_usb_udphs.c")
	drvUsbHsV1SourceFile.setDestPath(usbDriverProjectPath + "udphs/src")
	drvUsbHsV1SourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "udphs/src/")
	drvUsbHsV1SourceFile.setType("SOURCE")
	drvUsbHsV1SourceFile.setOverwrite(True)
	
	drvUsbHsV1DeviceSourceFile = usbDriverComponent.createFileSymbol(None, None)
	drvUsbHsV1DeviceSourceFile.setSourcePath(usbDriverPath + "udphs/src/dynamic/drv_usb_udphs_device.c.ftl")
	drvUsbHsV1DeviceSourceFile.setMarkup(True)
	drvUsbHsV1DeviceSourceFile.setOutputName("drv_usb_udphs_device.c")
	drvUsbHsV1DeviceSourceFile.setDestPath(usbDriverProjectPath + "udphs/src")
	drvUsbHsV1DeviceSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "udphs/src/")
	drvUsbHsV1DeviceSourceFile.setType("SOURCE")
	drvUsbHsV1DeviceSourceFile.setOverwrite(True)
			
			
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
			
def blUsbVbusPinName(usbSymbolSource, event):
	if (event["value"] == True):
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
	
	
