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

usbHostTplEntryNumber = None
usbHostHubSaveValue = None 

def genRtosTask(symbol, event):
	if event["value"] != "BareMetal":
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)


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
	

def handleMessage(messageID, args):	
	global usbHostTplEntryNumber
	if (messageID == "UPDATE_TPL_ENTRY_NUMBER"):
		usbHostTplEntryNumber.setValue(args["nTpl"])


def instantiateComponent(usbHostComponent):
	global usbHostTplEntryNumber
	global usbHostHubSaveValue
	res = Database.activateComponents(["HarmonyCore"])
	res = Database.activateComponents(["sys_time"])
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		res = Database.activateComponents(["drv_usbhs_v1"])
		speed = Database.getSymbolValue("drv_usbhs_v1", "USB_SPEED")
		driverIndex = "DRV_USBHSV1_INDEX_0"
		driverInterface = "DRV_USBHSV1_HOST_INTERFACE"
		args = {"operationMode":"Host"}
		Database.sendMessage("drv_usbhs_v1", "UPDATE_OPERATION_MODE", args)
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MK" , "PIC32MX" ,"PIC32MZ1025W"]):
		res = Database.activateComponents(["drv_usbfs_v1"])
		speed = Database.getSymbolValue("drv_usbfs_v1", "USB_SPEED")
		driverIndex = "DRV_USBFS_INDEX_0"
		driverInterface = "DRV_USBFS_HOST_INTERFACE"
		args = {"operationMode":"Host"}
		Database.sendMessage("drv_usbfs_v1", "UPDATE_OPERATION_MODE", args)
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
		res = Database.activateComponents(["drv_usbhs_v1"])
		speed = Database.getSymbolValue("drv_usbhs_v1", "USB_SPEED")
		driverIndex = "DRV_USBHS_INDEX_0"
		driverInterface = "DRV_USBHS_HOST_INTERFACE"
		args = {"operationMode":"Host"}
		Database.sendMessage("drv_usbhs_v1", "UPDATE_OPERATION_MODE", args)
	
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMD21", "SAMDA1", "SAMD5", "SAME5", "SAML21"]):
		res = Database.activateComponents(["drv_usbfs_v1"])
		speed = Database.getSymbolValue("drv_usbfs_v1", "USB_SPEED")
		driverIndex = "DRV_USBFSV1_INDEX_0"
		driverInterface = "DRV_USBFSV1_HOST_INTERFACE"
		args = {"operationMode":"Host"}
		Database.sendMessage("drv_usbfs_v1", "UPDATE_OPERATION_MODE", args)
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2", "SAM9X60", "SAMG55"]):
		res = Database.activateComponents(["drv_uhphs"])
		speed = Database.getSymbolValue("drv_uhphs", "USB_SPEED")
		driverIndex = "DRV_USB_UHP_INDEX_0"
		driverInterface = "DRV_USB_UHP_HOST_INTERFACE"
		args = {"operationMode":"Host"}
		Database.sendMessage("drv_uhphs", "UPDATE_OPERATION_MODE", args)
#	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMG55"]):
#		res = Database.activateComponents(["drv_uhp"])
#		speed = Database.getSymbolValue("drv_uhp", "USB_SPEED")
#		driverIndex = "DRV_USB_UHP_INDEX_0"
#		driverInterface = "DRV_USB_UHP_HOST_INTERFACE"
	
	
	# USB Driver Index - This symbol actually should get set from a Driver dependency connected callback. 
	# This is temporary work around to initialize using hard coded values. 
	usbDeviceDriverIndex = usbHostComponent.createStringSymbol("CONFIG_USB_DRIVER_INDEX", None)
	usbDeviceDriverIndex.setVisible(False)
	usbDeviceDriverIndex.setDefaultValue(driverIndex)
	
	# USB Driver Interface -  This symbol actually should get set from a Driver dependency connected callback. 
	# This is temporary work around to initialize using hard coded values. 
	usbDeviceDriverInterface = usbHostComponent.createStringSymbol("CONFIG_USB_DRIVER_INTERFACE", None)
	usbDeviceDriverInterface.setVisible(False)
	usbDeviceDriverInterface.setDefaultValue(driverInterface)

	# USB Host Number of TPL Entries 
	usbHostTplEntryNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_TPL_ENTRY_NUMBER", None)
	usbHostTplEntryNumber.setVisible(True)
	usbHostTplEntryNumber.setLabel("Number of TPL Entries")
        helpText = '''This configuration option indicates the current number of
        entries in the Host TPL table. This is shown for indication purposes
        only. It is automatically updated based on the number of device types
        to be supported by the Host'''
	usbHostTplEntryNumber.setDescription(helpText)
	usbHostTplEntryNumber.setDefaultValue(0)
	usbHostTplEntryNumber.setUseSingleDynamicValue(True)
	#usbHostTplEntryNumber.setDependencies(blUsbHostDeviceNumber, ["USB_OPERATION_MODE"])
	usbHostTplEntryNumber.setReadOnly(True)

	# USB Host Max Interfaces  
	usbHostMaxInterfaceNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_MAX_INTERFACES", None)
	usbHostMaxInterfaceNumber.setLabel("Maximum Interfaces per Device")
        helpText = '''This value should be set to the maximum number of
        interfaces that a device attached to this Host will contain. For
        example, if two devices can be connected to the Host, one device has 3
        interfaces and the other device has 5 interfaces, then this
        configuration should be set to 5. Setting this value incorrectly could
        result in the attached device operating incorrectly. Increasing the
        number increases the attached device operational memory.'''
	usbHostMaxInterfaceNumber.setVisible(True)
	usbHostMaxInterfaceNumber.setDescription(helpText)
	usbHostMaxInterfaceNumber.setDefaultValue(5)
	usbHostMaxInterfaceNumber.setDependencies(blUsbHostMaxInterfaceNumber, ["USB_OPERATION_MODE"])	
	
	# USB Host Transfers Number 
	usbHostTransfersNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_TRANSFERS_NUMBER", None)
	usbHostTransfersNumber.setLabel("Maximum Number of Transfers")
	usbHostTransfersNumber.setVisible(True)
        helpText = '''Configures the maximum number of USB transfers requests that the Host Layer can process. 
        Client driver (MSD, CDC, HID etc.) place data transfer requests with the Host Layer. If a transfer is
        currently in progress, then the transfer request is queued. If the number of currently queued transfer 
        requests has reached this number, the Host Layer directs the Client Driver to try again later. Increasing
        the number increases the ability of the Host Layer to queue additional requests but also increases the
        memory requirements.'''
	usbHostTransfersNumber.setDescription(helpText)
	usbHostTransfersNumber.setDefaultValue(10)
	usbHostTransfersNumber.setDependencies(blUsbHostMaxInterfaceNumber, ["USB_OPERATION_MODE"])	
	
	# USB Host Hub Support
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ" , "PIC32MX" , "SAMA5D2", "SAM9X60" ]):
		usbHostHubsupport = usbHostComponent.createBooleanSymbol("CONFIG_USB_HOST_HUB_SUPPORT", None)
		usbHostHubsupport.setLabel( "Hub support" )
		usbHostHubsupport.setVisible( True)
                helpText = '''Enable this option to add Hub Support to the current application. Checking this box 
                adds the Hub Driver to the USB Host application and allows the Host Layer to communicate with several
                USB devices.'''
                usbHostHubsupport.setDescription(helpText)
		usbHostHubsupport.setDefaultValue(False)
		usbHostHubsupport.setDependencies(hubUpdateTPL, ["CONFIG_USB_HOST_HUB_SUPPORT"])
		usbHostHubSaveValue = usbHostComponent.createBooleanSymbol("CONFIG_USB_HOST_HUB_SUPPORT_SAVE_STATUS", None)
		usbHostHubSaveValue.setLabel( "Hub support" )
		usbHostHubSaveValue.setVisible( False)
		usbHostHubSaveValue.setDefaultValue(False)
		
		
		# USB Host Hub Client Driver instances
		usbHostHubDriverInstance = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_HUB_NUMBER_OF_INSTANCES", usbHostHubsupport)
		usbHostHubDriverInstance.setLabel( "Number of Hub Client Driver Instances" )
		usbHostHubDriverInstance.setDescription("Enter the number of HUB Class Driver instances required in the application.")
		usbHostHubDriverInstance.setVisible(False)
		usbHostHubDriverInstance.setDefaultValue(1)
		usbHostHubDriverInstance.setDependencies(hubSupportSetVisible, ["CONFIG_USB_HOST_HUB_SUPPORT"])	
		
		# USB Host Max Number of Devices  
		usbHostDeviceNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_DEVICE_NUMNBER", usbHostHubsupport)
		usbHostDeviceNumber.setLabel("Maximum Number of Devices")
		usbHostDeviceNumber.setVisible(False)
		usbHostDeviceNumber.setDescription("Maximum Number of Devices that will be attached to this Host")
		usbHostDeviceNumber.setDefaultValue(1)
		usbHostDeviceNumber.setUseSingleDynamicValue(True)
		usbHostDeviceNumber.setDependencies(hubSupportSetVisible, ["CONFIG_USB_HOST_HUB_SUPPORT"])
			
			
	##############################################################
	# TPL Entry for HUB client driver 
	##############################################################
		usbHostHubTplEntryFile = usbHostComponent.createFileSymbol("FILE_USB_HOST_HUB_TPL", None)
		usbHostHubTplEntryFile.setType("STRING")
		usbHostHubTplEntryFile.setOutputName("usb_host.LIST_USB_HOST_TPL_ENTRY")
		usbHostHubTplEntryFile.setSourcePath("templates/host/system_init_c_hub_tpl.ftl")
		usbHostHubTplEntryFile.setMarkup(True)
		usbHostHubTplEntryFile.setEnabled(False)
		usbHostHubTplEntryFile.setDependencies(hubSupportSetEnable, ["CONFIG_USB_HOST_HUB_SUPPORT"])
		
	
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
	
	# system_config.h file for USB Host Layer    
	################################################
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ" , "PIC32MX", "SAMA5D2", "SAM9X60"]):
		usbHostHubConfigFile = usbHostComponent.createFileSymbol("FILE_USB_HOST_HUB_CONFIG", None)
		usbHostHubConfigFile.setType("STRING")
		usbHostHubConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
		usbHostHubConfigFile.setSourcePath("templates/host/system_config.h.host_hub.ftl")
		usbHostHubConfigFile.setMarkup(True)
		usbHostHubConfigFile.setEnabled(False)
		usbHostHubConfigFile.setDependencies(hubSupportSetEnable, ["CONFIG_USB_HOST_HUB_SUPPORT"])
		
	
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
	usbHostSystemTasksFileRTOS.setDependencies(genRtosTask, ["HarmonyCore.SELECT_RTOS"])
	
	################################################
	# USB Host Layer Files 
	################################################
	usbHostHeaderFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_host.h', usbHostComponent, usbHostHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHostSourceFile = usbHostComponent.createFileSymbol(None, None)
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ1025W"]):
		addFileName('usb_host.c', usbHostComponent, usbHostSourceFile, "templates/host/", "/usb/src", True, None)
	else :
		addFileName('usb_host.c', usbHostComponent, usbHostSourceFile, "middleware/src/", "/usb/src", True, None)
	
	usbHostLocalHeaderFile = usbHostComponent.createFileSymbol(None, None)
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ1025W"]):
		addFileName('usb_host_local.h', usbHostComponent, usbHostLocalHeaderFile, "templates/host/", "/usb/src", True, None)
	else :
		addFileName('usb_host_local.h', usbHostComponent, usbHostLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	usbCommonFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_common.h', usbHostComponent, usbCommonFile, "middleware/", "/usb/", True, None)
	
	usbChapter9File = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_chapter_9.h', usbHostComponent, usbChapter9File, "middleware/", "/usb/", True, None)
	
	usbHostHubMappingFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_host_hub_mapping.h', usbHostComponent, usbHostHubMappingFile, "middleware/src/", "/usb/src", True, None)
	
	usbExternalDependenciesFile = usbHostComponent.createFileSymbol(None, None)
	addFileName('usb_external_dependencies.h', usbHostComponent, usbExternalDependenciesFile, "middleware/src/", "/usb/src", True, None)
		
	################################################
	# USB Host HUB Client driver Files 
	################################################
	
	usbHostHubLocalHeaderFile = usbHostComponent.createFileSymbol("FILE_USB_HOST_HUB_HEADER_LOCAL", None)
	addFileName('usb_host_hub_local.h', usbHostComponent, usbHostHubLocalHeaderFile, "middleware/src/", "/usb/src", False, hubSupportSetEnable)
	
	usbHostHubSourceFile = usbHostComponent.createFileSymbol("FILE_USB_HOST_HUB_SOURCE", None)
	addFileName('usb_host_hub.c', usbHostComponent, usbHostHubSourceFile, "middleware/src/", "/usb/src", False, hubSupportSetEnable)
			
	usbHostHubHeaderFile = usbHostComponent.createFileSymbol("FILE_USB_HOST_HUB_HEADER", None)
	addFileName('usb_host_hub.h', usbHostComponent, usbHostHubHeaderFile, "middleware/", "/usb/", False, hubSupportSetEnable)
	
def blUsbHostDeviceNumber(usbSymbolSource, event):
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
	if callback == hubSupportSetEnable:
		symbol.setDependencies(callback, ["CONFIG_USB_HOST_HUB_SUPPORT"])
	
def hubSupportSetVisible(usbSymbolSource, event):
	if (event["value"] == True):		
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)

def hubSupportSetEnable(usbSymbolSource, event):
	if (event["value"] == True):		
		usbSymbolSource.setEnabled(True)
	else:
		usbSymbolSource.setEnabled(False)
		
			
def hubUpdateTPL(usbSymbolSource, event):	
	global usbHostTplEntryNumber
	global usbHostHubSaveValue
	if (event["value"] == True) and usbHostHubSaveValue.getValue() == False:		
		readValue = usbHostTplEntryNumber.getValue()
		if readValue != None:
			usbHostTplEntryNumber.setValue(readValue + 1)
	elif (event["value"] == False) and usbHostHubSaveValue.getValue() == True:	
		readValue = usbHostTplEntryNumber.getValue()
		if readValue != None:
			usbHostTplEntryNumber.setValue(readValue - 1)
	usbHostHubSaveValue.setValue(event["value"])		
	
