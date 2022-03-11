 
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
usbHostControllerList = None
usbHostControllerEntryFile = None

def genRtosTask(symbol, event):
	if event["value"] != "BareMetal":
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)

def onAttachmentConnected(source, target):
	global usbHostControllerList
	global usbHostControllerEntryFile
	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	if (remoteID == "usb_host"):
		usbHostControllerEntryFile.setEnabled(True)
	if connectID == "usb_peripheral_dependency":
		usbPLIB = localComponent.getSymbolByID("DRV_USB_PLIB")
		usbPLIB.clearValue()
		usbPLIB.setValue(remoteID.upper())
		
		if usbPLIB.getValue() == "PERIPHERAL_USB_1":
			print("Peripheral 1 is enabled")
			
			Database.clearSymbolValue("core", "USB_1_INTERRUPT_ENABLE")
			Database.clearSymbolValue("core", "USB_1_INTERRUPT_HANDLER_LOCK")
			Database.clearSymbolValue("core", "USB_1_INTERRUPT_HANDLER")
			Database.setSymbolValue("core", "USB_1_INTERRUPT_ENABLE", True, 2)
			Database.setSymbolValue("core", "USB_1_INTERRUPT_HANDLER_LOCK", True, 2)
			Database.setSymbolValue("core", "USB_1_INTERRUPT_HANDLER", "DRV_USBFS_USB1_Handler", 2)
			Database.setSymbolValue("core", "USB1_CLOCK_ENABLE", True, 1)
		elif usbPLIB.getValue() == "PERIPHERAL_USB_2":
			# Update USB General Interrupt Handler
			print("Peripheral 2 is enabled")
			Database.clearSymbolValue("core", "USB_2_INTERRUPT_ENABLE")
			Database.clearSymbolValue("core", "USB_2_INTERRUPT_HANDLER_LOCK")
			Database.clearSymbolValue("core", "USB_2_INTERRUPT_HANDLER")
			Database.setSymbolValue("core", "USB_2_INTERRUPT_ENABLE", True, 2)
			Database.setSymbolValue("core", "USB_2_INTERRUPT_HANDLER_LOCK", True, 2)
			Database.setSymbolValue("core", "USB_2_INTERRUPT_HANDLER", "DRV_USBFS_USB2_Handler", 2)
			Database.setSymbolValue("core", "USB2_CLOCK_ENABLE", True, 1)

def onAttachmentDisconnected(source, target):
	localComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	if connectID == "usb_peripheral_dependency":
		usbPLIB = localComponent.getSymbolByID("DRV_USB_PLIB")
		if usbPLIB.getValue() == "PERIPHERAL_USB_1":
			Database.clearSymbolValue("core", "USB_1_INTERRUPT_ENABLE")
			Database.clearSymbolValue("core", "USB_1_INTERRUPT_HANDLER_LOCK")
			Database.clearSymbolValue("core", "USB_1_INTERRUPT_HANDLER")
		elif usbPLIB.getValue() == "PERIPHERAL_USB_2":
			# Update USB General Interrupt Handler
			Database.clearSymbolValue("core", "USB_2_INTERRUPT_ENABLE")
			Database.clearSymbolValue("core", "USB_2_INTERRUPT_HANDLER_LOCK")
			Database.clearSymbolValue("core", "USB_2_INTERRUPT_HANDLER")
			
def blUsbOperatioMode(symbol, event):
	args = {"operationMode":event["value"]}
	res = Database.sendMessage("drv_usbfs_index", "UPDATE_OPERATION_MODE_COMMON", args)
	
	
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
	# no RTOS Task required for USBFS driver if operating in Interrupt mode. MHC only support interrupt mode. 
	show_rtos_menu = False

def instantiateComponent(usbDriverComponent, index):
	global usbHostControllerEntryFile
	usbDriverSourcePath = "usbfs"
	
	usbDriverIndex = usbDriverComponent.createIntegerSymbol("INDEX", None)
	usbDriverIndex.setVisible(False)
	usbDriverIndex.setDefaultValue(index)
	
	res = Database.activateComponents(["HarmonyCore"])
	
	usbPLIB = usbDriverComponent.createStringSymbol("DRV_USB_PLIB", None)
	usbPLIB.setLabel("USB Peripheral used")
	usbPLIB.setReadOnly(True)
	

	# USB Driver Speed selection
	usbSpeed = usbDriverComponent.createComboSymbol("USB_SPEED", None, listUsbSpeed)
	usbSpeed.setLabel("USB Speed Selection")
	usbSpeed.setVisible(False)
	usbSpeed.setDescription("Select USB Operation Speed")
	usbSpeed.setDefaultValue("Full Speed")
	usbSpeed.setDependencies(blUSBDriverSpeedChanged, ["USB_SPEED"])

	# USB Driver Operation mode selection
	usbOpMode = usbDriverComponent.createComboSymbol("USB_OPERATION_MODE", None, listUsbOperationMode)
	usbOpMode.setLabel("USB Mode Selection")
	usbOpMode.setVisible(True)
	usbOpMode.setDescription("Select USB Operation Mode")
	usbOpMode.setDefaultValue("Device")
	usbOpMode.setUseSingleDynamicValue(True)
	usbOpMode.setDependencies(blUsbOperatioMode, ["USB_OPERATION_MODE"])
	
	if any(x in Variables.get("__PROCESSOR") for x in ["SAML22"]):
		usbOpMode.setReadOnly(True)
		
	# USB Driver Index - This symbol actually should get set from a Driver dependency connected callback. 
	# This is temporary work around to initialize using hard coded values. 
	usbDeviceDriverIndex = usbDriverComponent.createStringSymbol("CONFIG_USB_DRIVER_INDEX", None)
	usbDeviceDriverIndex.setVisible(False)
	usbDeviceDriverIndex.setDefaultValue("DRV_USBFS_INDEX_" + str(index))
	
	# USB Driver Interface -  This symbol actually should get set from a Driver dependency connected callback. 
	# This is temporary work around to initialize using hard coded values. 
	usbDeviceDriverInterface = usbDriverComponent.createStringSymbol("CONFIG_USB_DRIVER_INTERFACE", None)
	usbDeviceDriverInterface.setVisible(False)
	usbDeviceDriverInterface.setDefaultValue("DRV_USBFS_HOST_INTERFACE")

	enable_rtos_settings = False

	if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
		enable_rtos_settings = True

	if any(x in Variables.get("__PROCESSOR") for x in ["SAMD20", "SAMD21","SAMDA1", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "PIC32CM"]):
		# Update USB General Interrupt Handler
		Database.setSymbolValue("core", "USB_INTERRUPT_ENABLE", True, 1)
		Database.setSymbolValue("core", "USB_INTERRUPT_HANDLER_LOCK", True, 1)
		Database.setSymbolValue("core", "USB_INTERRUPT_HANDLER", "DRV_USBFSV1_USB_Handler", 1)
		
		Database.setSymbolValue("core", "USB_CLOCK_ENABLE", True, 1)
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MX", "PIC32MM"]):
		Database.clearSymbolValue("core", "USB_1_INTERRUPT_ENABLE")
		Database.clearSymbolValue("core", "USB_1_INTERRUPT_HANDLER_LOCK")
		Database.clearSymbolValue("core", "USB_1_INTERRUPT_HANDLER")
	
		# Update USB General Interrupt Handler
		Database.setSymbolValue("core", "USB_1_INTERRUPT_ENABLE", True, 1)
		Database.setSymbolValue("core", "USB_1_INTERRUPT_HANDLER_LOCK", True, 1)
		Database.setSymbolValue("core", "USB_1_INTERRUPT_HANDLER", "DRV_USBFS_USB_Handler", )
		Database.setSymbolValue("core", "USB_CLOCK_ENABLE", True, 1)
	
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMD51", "SAME51", "SAME53", "SAME54", "PIC32CX"]):

		# Update USB General Interrupt Handler
		Database.setSymbolValue("core", "USB_OTHER_INTERRUPT_ENABLE", True, 1)
		Database.setSymbolValue("core", "USB_OTHER_INTERRUPT_HANDLER_LOCK", True, 1)
		Database.setSymbolValue("core", "USB_OTHER_INTERRUPT_HANDLER", "DRV_USBFSV1_OTHER_Handler", 1)

		# Update USB General Interrupt Handler
		Database.setSymbolValue("core", "USB_SOF_HSOF_INTERRUPT_ENABLE", True, 1)
		Database.setSymbolValue("core", "USB_SOF_HSOF_INTERRUPT_HANDLER_LOCK", True, 1)
		Database.setSymbolValue("core", "USB_SOF_HSOF_INTERRUPT_HANDLER", "DRV_USBFSV1_SOF_HSOF_Handler", 1)

		# Update USB General Interrupt Handler
		Database.setSymbolValue("core", "USB_TRCPT0_INTERRUPT_ENABLE", True, 1)
		Database.setSymbolValue("core", "USB_TRCPT0_INTERRUPT_HANDLER_LOCK", True, 1)
		Database.setSymbolValue("core", "USB_TRCPT0_INTERRUPT_HANDLER", "DRV_USBFSV1_TRCPT0_Handler", 1)

		# Update USB General Interrupt Handler
		Database.setSymbolValue("core", "USB_TRCPT1_INTERRUPT_ENABLE", True, 1)
		Database.setSymbolValue("core", "USB_TRCPT1_INTERRUPT_HANDLER_LOCK", True, 1)
		Database.setSymbolValue("core", "USB_TRCPT1_INTERRUPT_HANDLER", "DRV_USBFSV1_TRCPT1_Handler", 1)
		
		Database.setSymbolValue("core", "USB_CLOCK_ENABLE", True, 1)

	configName = Variables.get("__CONFIGURATION_NAME")
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MK"]):
		sourcePath = "templates/driver/usbfs_multi/"
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MX", "PIC32MM"]):
		sourcePath = "templates/driver/usbfs/"
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMD51", "SAME51", "SAME53", "SAME54" ,"SAMD20", "SAMD21", "SAMDA1","SAML21","SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "PIC32CM", "PIC32CX"]):
		sourcePath = "templates/driver/usbfsv1/"

	################################################
	# system_definitions.h file for USB Driver
	################################################
	usbDriverSystemDefIncludeFile = usbDriverComponent.createFileSymbol("DRV_USBFS_SYS_DEF_INCLUDES_PIC32MK", None)
	usbDriverSystemDefIncludeFile.setType("STRING")
	usbDriverSystemDefIncludeFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDriverSystemDefIncludeFile.setSourcePath(sourcePath + "system_definitions.h.driver_includes.ftl")
	usbDriverSystemDefIncludeFile.setMarkup(True)

	usbDriverSystemDefObjFile = usbDriverComponent.createFileSymbol("DRV_USBFS_SYS_DEF_OBJ_PIC32MK", None)
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
	usbDriverIsrFile.setOutputName("drv_usbfs_index.LIST_DRV_USB_ISR_ENTRY")
	usbDriverIsrFile.setSourcePath( sourcePath + "drv_usb_isr.ftl")
	usbDriverIsrFile.setMarkup(True)
	
	################################################
	# USB Driver Header files
	################################################
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMD51", "SAME51", "SAME53", "SAME54" ,"SAMD20", "SAMD21", "SAMDA1","SAML21","SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "PIC32CM", "PIC32CX"]):
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
		drvUsbHsV1HeaderFile.setSourcePath(usbDriverPath + usbDriverSourcePath + "/drv_usbfsv1.h")
		drvUsbHsV1HeaderFile.setOutputName("drv_usbfsv1.h")
		drvUsbHsV1HeaderFile.setDestPath(usbDriverProjectPath+ "usbfsv1")
		drvUsbHsV1HeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath+ "usbfsv1")
		drvUsbHsV1HeaderFile.setType("HEADER")
		drvUsbHsV1HeaderFile.setOverwrite(True)

		drvUsbHsV1VarMapHeaderFile = usbDriverComponent.createFileSymbol(None, None)
		drvUsbHsV1VarMapHeaderFile.setSourcePath(usbDriverPath + "usbfsv1/src/drv_usbfsv1_variant_mapping.h.ftl")
		drvUsbHsV1VarMapHeaderFile.setOutputName("drv_usbfsv1_variant_mapping.h")
		drvUsbHsV1VarMapHeaderFile.setDestPath(usbDriverProjectPath + "usbfsv1/src")
		drvUsbHsV1VarMapHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfsv1/src")
		drvUsbHsV1VarMapHeaderFile.setType("HEADER")
		drvUsbHsV1VarMapHeaderFile.setOverwrite(True)
		drvUsbHsV1VarMapHeaderFile.setMarkup(True)


		drvUsbHsV1LocalHeaderFile = usbDriverComponent.createFileSymbol(None, None)
		drvUsbHsV1LocalHeaderFile.setSourcePath(usbDriverPath + "usbfsv1/src/drv_usbfsv1_local.h")
		drvUsbHsV1LocalHeaderFile.setOutputName("drv_usbfsv1_local.h")
		drvUsbHsV1LocalHeaderFile.setDestPath(usbDriverProjectPath + "usbfsv1/src")
		drvUsbHsV1LocalHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfsv1/src")
		drvUsbHsV1LocalHeaderFile.setType("HEADER")
		drvUsbHsV1LocalHeaderFile.setOverwrite(True)
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MK", "PIC32MX", "PIC32MM"]):
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
		drvUsbHsV1HeaderFile.setSourcePath(usbDriverPath + "usbfs/drv_usbfs.h")
		drvUsbHsV1HeaderFile.setOutputName("drv_usbfs.h")
		drvUsbHsV1HeaderFile.setDestPath(usbDriverProjectPath+ "usbfs")
		drvUsbHsV1HeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath+ "usbfs")
		drvUsbHsV1HeaderFile.setType("HEADER")
		drvUsbHsV1HeaderFile.setOverwrite(True)

		drvUsbHsV1VarMapHeaderFile = usbDriverComponent.createFileSymbol(None, None)
		drvUsbHsV1VarMapHeaderFile.setSourcePath(usbDriverPath + "usbfs/src/drv_usbfs_variant_mapping.h")
		drvUsbHsV1VarMapHeaderFile.setOutputName("drv_usbfs_variant_mapping.h")
		drvUsbHsV1VarMapHeaderFile.setDestPath(usbDriverProjectPath + "usbfs/src")
		drvUsbHsV1VarMapHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfs/src")
		drvUsbHsV1VarMapHeaderFile.setType("HEADER")
		drvUsbHsV1VarMapHeaderFile.setOverwrite(True)
		drvUsbHsV1VarMapHeaderFile.setMarkup(True)


		drvUsbHsV1LocalHeaderFile = usbDriverComponent.createFileSymbol(None, None)
		drvUsbHsV1LocalHeaderFile.setSourcePath(usbDriverPath + "usbfs/src/drv_usbfs_local.h")
		drvUsbHsV1LocalHeaderFile.setOutputName("drv_usbfs_local.h")
		drvUsbHsV1LocalHeaderFile.setDestPath(usbDriverProjectPath + "usbfs/src")
		drvUsbHsV1LocalHeaderFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfs/src")
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
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMD51", "SAME51", "SAME53", "SAME54" ,"SAMD20", "SAMD21", "SAMDA1","SAML21","SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "PIC32CM", "PIC32CX"]):
		drvUsbHsV1SourceFile = usbDriverComponent.createFileSymbol(None, None)
		drvUsbHsV1SourceFile.setSourcePath(usbDriverPath + usbDriverSourcePath + "/src/dynamic/drv_usbfsv1.c")
		drvUsbHsV1SourceFile.setOutputName("drv_usbfsv1.c")
		drvUsbHsV1SourceFile.setDestPath(usbDriverProjectPath + "usbfsv1/src")
		drvUsbHsV1SourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfsv1/src/")
		drvUsbHsV1SourceFile.setType("SOURCE")
		drvUsbHsV1SourceFile.setOverwrite(True)

		drvUsbHsV1DeviceSourceFile = usbDriverComponent.createFileSymbol(None, None)
		drvUsbHsV1DeviceSourceFile.setSourcePath(usbDriverPath + usbDriverSourcePath + "/src/dynamic/drv_usbfsv1_device.c")
		drvUsbHsV1DeviceSourceFile.setOutputName("drv_usbfsv1_device.c")
		drvUsbHsV1DeviceSourceFile.setDestPath(usbDriverProjectPath + "usbfsv1/src")
		drvUsbHsV1DeviceSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfsv1/src/")
		drvUsbHsV1DeviceSourceFile.setType("SOURCE")
		drvUsbHsV1DeviceSourceFile.setOverwrite(True)
		drvUsbHsV1DeviceSourceFile.setDependencies(blDrvUsbHsV1DeviceSourceFile, ["USB_OPERATION_MODE"])

		drvUsbHsV1HostSourceFile = usbDriverComponent.createFileSymbol(None, None)
		drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "usbfsv1/src/dynamic/drv_usbfsv1_host.c")
		drvUsbHsV1HostSourceFile.setOutputName("drv_usbfsv1_host.c")
		drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "usbfsv1/src")
		drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfsv1/src/")
		drvUsbHsV1HostSourceFile.setType("SOURCE")
		drvUsbHsV1HostSourceFile.setOverwrite(True)
		drvUsbHsV1HostSourceFile.setEnabled(False)
		drvUsbHsV1HostSourceFile.setDependencies(blDrvUsbHsV1HostSourceFile, ["USB_OPERATION_MODE"])
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MK" , "PIC32MX", "PIC32MM"]):
		#drvUsbHsV1HostSourceFile = usbDriverComponent.createFileSymbol("DRV_USBFS_COMMON_C_SOURCE", None)
		#drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "usbfs/src/drv_usbfs.c.ftl")
		#drvUsbHsV1HostSourceFile.setOutputName("drv_usbfs.c")
		#drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "usbfs/src")
		#drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfs/src/")
		#drvUsbHsV1HostSourceFile.setType("SOURCE")
		#drvUsbHsV1HostSourceFile.setMarkup(True)
		#drvUsbHsV1HostSourceFile.setOverwrite(True)
		
				
		drvUsbHsV1DeviceSourceFile = usbDriverComponent.createFileSymbol(None, None)
		drvUsbHsV1DeviceSourceFile.setSourcePath(usbDriverPath + "usbfs/src/drv_usbfs_device.c")
		drvUsbHsV1DeviceSourceFile.setOutputName("drv_usbfs_device.c")
		drvUsbHsV1DeviceSourceFile.setDestPath(usbDriverProjectPath + "usbfs/src")
		drvUsbHsV1DeviceSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfs/src/")
		drvUsbHsV1DeviceSourceFile.setType("SOURCE")
		drvUsbHsV1DeviceSourceFile.setOverwrite(True)
		drvUsbHsV1DeviceSourceFile.setDependencies(blDrvUsbHsV1DeviceSourceFile, ["USB_OPERATION_MODE"])
		
		drvUsbHsV1HostSourceFile = usbDriverComponent.createFileSymbol(None, None)
		drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "usbfs/src/drv_usbfs_host.c")
		drvUsbHsV1HostSourceFile.setOutputName("drv_usbfs_host.c")
		drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "usbfs/src")
		drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfs/src/")
		drvUsbHsV1HostSourceFile.setType("SOURCE")
		drvUsbHsV1HostSourceFile.setOverwrite(True)
		drvUsbHsV1HostSourceFile.setEnabled(False)
		drvUsbHsV1HostSourceFile.setDependencies(blDrvUsbHsV1HostSourceFile, ["USB_OPERATION_MODE"])
	# Add USBHS PLIB files for PIC32MK
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MK" , "PIC32MX", "PIC32MM"]):
		plib_usbfs_h = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('plib_usbfs.h', usbDriverComponent, plib_usbfs_h, usbDriverPath + "usbfs/src/", usbDriverProjectPath + "usbfs/src", True, None)
		
		#plib_usbfs_header_h = usbDriverComponent.createFileSymbol(None, None)	
		#addFileName('plib_usbfs_header.h', usbDriverComponent, plib_usbfs_header_h, usbDriverPath + "usbfs/src/", usbDriverProjectPath + "usbfs/src", True, None)
	
		usbfs_registers_h = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbfs_registers.h', usbDriverComponent, usbfs_registers_h, usbDriverPath + "usbfs/src/", usbDriverProjectPath + "usbfs/src", True, None)
		
		usbfs_registers_h_templates = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbfs_registers.h', usbDriverComponent, usbfs_registers_h_templates, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ActivityPending_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ActivityPending_Default.h', usbDriverComponent, usb_ActivityPending_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
	
		usb_ActivityPending_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ActivityPending_Unsupported.h', usbDriverComponent, usb_ActivityPending_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
	
		usb_ALL_Interrupt_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ALL_Interrupt_Default.h', usbDriverComponent, usb_ALL_Interrupt_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ALL_Interrupt_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ALL_Interrupt_Unsupported.h', usbDriverComponent, usb_ALL_Interrupt_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_AutomaticSuspend_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_AutomaticSuspend_Default.h', usbDriverComponent, usb_AutomaticSuspend_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_AutomaticSuspend_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_AutomaticSuspend_Unsupported.h', usbDriverComponent, usb_AutomaticSuspend_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_BDTBaseAddress_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_BDTBaseAddress_Default.h', usbDriverComponent, usb_BDTBaseAddress_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_BDTBaseAddress_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_BDTBaseAddress_Unsupported.h', usbDriverComponent, usb_BDTBaseAddress_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_BDTFunctions_PIC32 = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_BDTFunctions_PIC32.h', usbDriverComponent, usb_BDTFunctions_PIC32, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_BDTFunctions_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_BDTFunctions_Unsupported.h', usbDriverComponent, usb_BDTFunctions_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_BufferFreeze_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_BufferFreeze_Default.h', usbDriverComponent, usb_BufferFreeze_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_BufferFreeze_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_BufferFreeze_Unsupported.h', usbDriverComponent, usb_BufferFreeze_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_DeviceAddress_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_DeviceAddress_Default.h', usbDriverComponent, usb_DeviceAddress_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_DeviceAddress_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_DeviceAddress_Unsupported.h', usbDriverComponent, usb_DeviceAddress_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EP0LowSpeedConnect_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EP0LowSpeedConnect_Default.h', usbDriverComponent, usb_EP0LowSpeedConnect_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EP0LowSpeedConnect_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EP0LowSpeedConnect_Unsupported.h', usbDriverComponent, usb_EP0LowSpeedConnect_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EP0NAKRetry_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EP0NAKRetry_Default.h', usbDriverComponent, usb_EP0NAKRetry_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EP0NAKRetry_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EP0NAKRetry_Unsupported.h', usbDriverComponent, usb_EP0NAKRetry_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnControlTransfer_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnControlTransfer_Default.h', usbDriverComponent, usb_EPnControlTransfer_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnControlTransfer_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnControlTransfer_Unsupported.h', usbDriverComponent, usb_EPnControlTransfer_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnHandshake_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnHandshake_Default.h', usbDriverComponent, usb_EPnHandshake_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnHandshake_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnHandshake_Unsupported.h', usbDriverComponent, usb_EPnHandshake_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnRxEnableEnhanced_PIC32 = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnRxEnableEnhanced_PIC32.h', usbDriverComponent, usb_EPnRxEnableEnhanced_PIC32, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnRxEnableEnhanced_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnRxEnableEnhanced_Unsupported.h', usbDriverComponent, usb_EPnRxEnableEnhanced_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnStall_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnStall_Default.h', usbDriverComponent, usb_EPnStall_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnStall_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnStall_Unsupported.h', usbDriverComponent, usb_EPnStall_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnTxRx_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnTxRx_Default.h', usbDriverComponent, usb_EPnTxRx_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EPnTxRx_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EPnTxRx_Unsupported.h', usbDriverComponent, usb_EPnTxRx_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ERR_Interrupt_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ERR_Interrupt_Default.h', usbDriverComponent, usb_ERR_Interrupt_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ERR_Interrupt_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ERR_Interrupt_Unsupported.h', usbDriverComponent, usb_ERR_Interrupt_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ERR_InterruptStatus_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ERR_InterruptStatus_Default.h', usbDriverComponent, usb_ERR_InterruptStatus_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)

		usb_ERR_InterruptStatus_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ERR_InterruptStatus_Unsupported.h', usbDriverComponent, usb_ERR_InterruptStatus_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EyePattern_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EyePattern_Default.h', usbDriverComponent, usb_EyePattern_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_EyePattern_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_EyePattern_Unsupported.h', usbDriverComponent, usb_EyePattern_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_FrameNumber_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_FrameNumber_Default.h', usbDriverComponent, usb_FrameNumber_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_FrameNumber_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_FrameNumber_Unsupported.h', usbDriverComponent, usb_FrameNumber_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_GEN_Interrupt_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_GEN_Interrupt_Default.h', usbDriverComponent, usb_GEN_Interrupt_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_GEN_Interrupt_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_GEN_Interrupt_Unsupported.h', usbDriverComponent, usb_GEN_Interrupt_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_GEN_InterruptStatus_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_GEN_InterruptStatus_Default.h', usbDriverComponent, usb_GEN_InterruptStatus_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_GEN_InterruptStatus_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_GEN_InterruptStatus_Unsupported.h', usbDriverComponent, usb_GEN_InterruptStatus_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_HostBusyWithToken_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_HostBusyWithToken_Default.h', usbDriverComponent, usb_HostBusyWithToken_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_HostBusyWithToken_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_HostBusyWithToken_Unsupported.h', usbDriverComponent, usb_HostBusyWithToken_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_HostGeneratesReset_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_HostGeneratesReset_Default.h', usbDriverComponent, usb_HostGeneratesReset_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_HostGeneratesReset_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_HostGeneratesReset_Unsupported.h', usbDriverComponent, usb_HostGeneratesReset_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LastDirection_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LastDirection_Default.h', usbDriverComponent, usb_LastDirection_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LastDirection_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LastDirection_Unsupported.h', usbDriverComponent, usb_LastDirection_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LastEndpoint_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LastEndpoint_Default.h', usbDriverComponent, usb_LastEndpoint_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LastEndpoint_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LastEndpoint_Unsupported.h', usbDriverComponent, usb_LastEndpoint_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LastPingPong_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LastPingPong_Default.h', usbDriverComponent, usb_LastPingPong_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LastPingPong_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LastPingPong_Unsupported.h', usbDriverComponent, usb_LastPingPong_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LastTransactionDetails_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LastTransactionDetails_Default.h', usbDriverComponent, usb_LastTransactionDetails_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LastTransactionDetails_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LastTransactionDetails_Unsupported.h', usbDriverComponent, usb_LastTransactionDetails_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LiveJState_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LiveJState_Default.h', usbDriverComponent, usb_LiveJState_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LiveJState_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LiveJState_Unsupported.h', usbDriverComponent, usb_LiveJState_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LiveSingleEndedZero_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LiveSingleEndedZero_Default.h', usbDriverComponent, usb_LiveSingleEndedZero_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_LiveSingleEndedZero_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_LiveSingleEndedZero_Unsupported.h', usbDriverComponent, usb_LiveSingleEndedZero_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ModuleBusy_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ModuleBusy_Default.h', usbDriverComponent, usb_ModuleBusy_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ModuleBusy_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ModuleBusy_Unsupported.h', usbDriverComponent, usb_ModuleBusy_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ModulePower_32Bit16Bit = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ModulePower_32Bit16Bit.h', usbDriverComponent, usb_ModulePower_32Bit16Bit, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ModulePower_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ModulePower_Unsupported.h', usbDriverComponent, usb_ModulePower_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_NextTokenSpeed_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_NextTokenSpeed_Default.h', usbDriverComponent, usb_NextTokenSpeed_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_NextTokenSpeed_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_NextTokenSpeed_Unsupported.h', usbDriverComponent, usb_NextTokenSpeed_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OnChipPullup_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OnChipPullup_Unsupported.h', usbDriverComponent, usb_OnChipPullup_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OnChipTransceiver_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OnChipTransceiver_Unsupported.h', usbDriverComponent, usb_OnChipTransceiver_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OpModeSelect_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OpModeSelect_Default.h', usbDriverComponent, usb_OpModeSelect_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OpModeSelect_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OpModeSelect_Unsupported.h', usbDriverComponent, usb_OpModeSelect_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_ASessionValid_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_ASessionValid_Default.h', usbDriverComponent, usb_OTG_ASessionValid_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_ASessionValid_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_ASessionValid_Unsupported.h', usbDriverComponent, usb_OTG_ASessionValid_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_BSessionEnd_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_BSessionEnd_Default.h', usbDriverComponent, usb_OTG_BSessionEnd_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_BSessionEnd_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_BSessionEnd_Unsupported.h', usbDriverComponent, usb_OTG_BSessionEnd_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_IDPinState_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_IDPinState_Default.h', usbDriverComponent, usb_OTG_IDPinState_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_IDPinState_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_IDPinState_Unsupported.h', usbDriverComponent, usb_OTG_IDPinState_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_Interrupt_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_Interrupt_Default.h', usbDriverComponent, usb_OTG_Interrupt_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_Interrupt_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_Interrupt_Unsupported.h', usbDriverComponent, usb_OTG_Interrupt_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_InterruptStatus_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_InterruptStatus_Default.h', usbDriverComponent, usb_OTG_InterruptStatus_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_InterruptStatus_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_InterruptStatus_Unsupported.h', usbDriverComponent, usb_OTG_InterruptStatus_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_LineState_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_LineState_Default.h', usbDriverComponent, usb_OTG_LineState_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_LineState_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_LineState_Unsupported.h', usbDriverComponent, usb_OTG_LineState_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_PullUpPullDown_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_PullUpPullDown_Default.h', usbDriverComponent, usb_OTG_PullUpPullDown_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_PullUpPullDown_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_PullUpPullDown_Unsupported.h', usbDriverComponent, usb_OTG_PullUpPullDown_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_SessionValid_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_SessionValid_Default.h', usbDriverComponent, usb_OTG_SessionValid_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_SessionValid_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_SessionValid_Unsupported.h', usbDriverComponent, usb_OTG_SessionValid_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_VbusCharge_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_VbusCharge_Default.h', usbDriverComponent, usb_OTG_VbusCharge_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_VbusCharge_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_VbusCharge_Unsupported.h', usbDriverComponent, usb_OTG_VbusCharge_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_VbusDischarge_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_VbusDischarge_Default.h', usbDriverComponent, usb_OTG_VbusDischarge_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_VbusDischarge_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_VbusDischarge_Unsupported.h', usbDriverComponent, usb_OTG_VbusDischarge_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_VbusPowerOnOff_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_VbusPowerOnOff_Default.h', usbDriverComponent, usb_OTG_VbusPowerOnOff_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_OTG_VbusPowerOnOff_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_OTG_VbusPowerOnOff_Unsupported.h', usbDriverComponent, usb_OTG_VbusPowerOnOff_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_PacketTransfer_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_PacketTransfer_Default.h', usbDriverComponent, usb_PacketTransfer_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_PacketTransfer_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_PacketTransfer_Unsupported.h', usbDriverComponent, usb_PacketTransfer_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_PingPongMode_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_PingPongMode_Unsupported.h', usbDriverComponent, usb_PingPongMode_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usbfs_registers = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usbfs_registers.h', usbDriverComponent, usbfs_registers, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ResumeSignaling_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ResumeSignaling_Default.h', usbDriverComponent, usb_ResumeSignaling_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_ResumeSignaling_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_ResumeSignaling_Unsupported.h', usbDriverComponent, usb_ResumeSignaling_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_SleepEntryGuard_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_SleepEntryGuard_Default.h', usbDriverComponent, usb_SleepEntryGuard_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_SleepEntryGuard_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_SleepEntryGuard_Unsupported.h', usbDriverComponent, usb_SleepEntryGuard_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_SOFThreshold_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_SOFThreshold_Default.h', usbDriverComponent, usb_SOFThreshold_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_SOFThreshold_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_SOFThreshold_Unsupported.h', usbDriverComponent, usb_SOFThreshold_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_SpeedControl_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_SpeedControl_Unsupported.h', usbDriverComponent, usb_SpeedControl_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_StartOfFrames_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_StartOfFrames_Default.h', usbDriverComponent, usb_StartOfFrames_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_StartOfFrames_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_StartOfFrames_Unsupported.h', usbDriverComponent, usb_StartOfFrames_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_StopInIdle_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_StopInIdle_Default.h', usbDriverComponent, usb_StopInIdle_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_StopInIdle_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_StopInIdle_Unsupported.h', usbDriverComponent, usb_StopInIdle_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_Suspend_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_Suspend_Default.h', usbDriverComponent, usb_Suspend_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_Suspend_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_Suspend_Unsupported.h', usbDriverComponent, usb_Suspend_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_TokenEP_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_TokenEP_Default.h', usbDriverComponent, usb_TokenEP_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_TokenEP_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_TokenEP_Unsupported.h', usbDriverComponent, usb_TokenEP_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_TokenPID_Default = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_TokenPID_Default.h', usbDriverComponent, usb_TokenPID_Default, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_TokenPID_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_TokenPID_Unsupported.h', usbDriverComponent, usb_TokenPID_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
		
		usb_UOEMonitor_Unsupported = usbDriverComponent.createFileSymbol(None, None)	
		addFileName('usb_UOEMonitor_Unsupported.h', usbDriverComponent, usb_UOEMonitor_Unsupported, usbDriverPath + "usbfs/src/templates/", usbDriverProjectPath + "usbfs/src/templates", True, None)
	
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

