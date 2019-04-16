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
usbDebugLogs = 1 
 
listUsbSpeed = ["High Speed", "Full Speed"]

# USB Device Global definitions 
usbDeviceClasses =["CDC", "MSD", "HID", "Vendor", "Audio V1", "Audio V2", "Printer"]
usbDeviceEp0BufferSizes = ["64", "32", "16", "8"]
usbDeviceDemoList = [
"Enter Product ID", 
"usb_speaker_demo",
"usb_microphone_demo",
"usb_headset_demo",
"usb_headset_multiple_sampling_demo",
"mac_audio_hi_res_demo",
"cdc_com_port_dual_demo",
"cdc_com_port_single_demo",
"cdc_msd_basic_demo",
"cdc_serial_emulator_demo",
"cdc_serial_emulator_msd_demo",
"hid_basic_demo",
"hid_joystick_demo",
"hid_keyboard_demo",
"hid_mouse_demo",
"hid_msd_demo",
"msd_basic_demo",
"msd_sdcard_demo",
"msd_spiflash_demo",
"msd_multiple_luns_demo",
"printer_basic_demo",
"vendor_demo",
]

usbDeviceDemoProductList = [
"0x0000",
"0x0064",
"0x0065",
"0x00FF",
"0x00FF",
"0xABCD",
"0x0208",
"0x000A",
"0x0057",
"0x000A",
"0x0057",
"0x003F",
"0x005E",
"0x0055",
"0x0000",
"0x0054",
"0x0009",
"0x0009",
"0x0009",
"0x0009",
"0x0070",
"0x0053",
]

usbDeviceProductStringList = [
"Enter Product String Here",
"Harmony USB Speaker Example",
"Harmony USB Microphone Example",
"Harmony USB Headset Example",
"Harmony USB Headset Multiple Sampling Rate Example",
"Harmony USB Speaker 2.0",
"CDC Dual COM Port Demo",
"Simple CDC Device Demo",
"CDC + MSD Demo",
"Simple CDC Device Demo",
"CDC + MSD Demo", 
"Simple HID Device Demo", 
"HID Joystick Demo", 
"HID Keyboard Demo",
"HID Mouse Demo", 
"HID + MSD Demo", 
"Simple MSD Device Demo", 
"Simple MSD Device Demo", 
"MSD SPI Flash Device Demo", 
"Multiple LUN MSD Demo",
"Generic Text Printer Demo",
"Simple WinUSB Device Demo" 
]


usbDeviceFunctionsNumberMax = 10
usbDeviceFunctionsNumberDefaultValue = 2 
usbDeviceFunctionsNumberValue = usbDeviceFunctionsNumberDefaultValue
usbDeviceFunctionIndex = 20

# A List of USB Device Function Menu Symbols
listUsbDeviceFunctionMenu = []

# List for holding Function Driver Specific details. 
listUsbDeviceFunctionType = []

listUsbDeviceStartInterfaceNumber = []
listUsbDeviceNumberOfInterfaces = []

# USB Device CDC global definitions 
usbDeviceCdcQueueSizeRead = []
usbDeviceCdcQueueSizeWrite = []
usbDeviceCdcQueueSizeSerialStateNotification = []
usbDeviceCdcEPNumberInt = []
usbDeviceCdcEPNumberBulkOut = []
usbDeviceCdcEPNumberBulkIn = []

# USB Device HID global definitions
usbDeviceHidQueueSizeReportSend = []
usbDeviceHidQueueSizeReportReceive = []
usbDeviceHidDeviceType = []	
usbDeviceHidEPNumberIntOut = []
usbDeviceHidEPNumberIntIn = []

#USB Device Vendor 
usbDeviceEndpointReadQueueSize = []
usbDeviceEndpointWriteQueueSize = []

usbDeviceMsdSupport = None
usbDeviceMsdDiskImageFile = None
usbDeviceMsdDiskImageFileAdd = None 

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


def onAttachmentConnected(source, target):
	global usbDeviceMsdSupport
	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	if (remoteID == "usb_device_msd"):
		usbDeviceMsdSupport.setValue(True, 2)
		

	
	
def onAttachmentDisconnected(source, target):
	global usbDeviceMsdSupport
	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	if (remoteID == "usb_device_msd"):
		usbDeviceMsdSupport.setValue(False, 2)
	
		

def instantiateComponent(usbDeviceComponent):	
	global usbDeviceMsdSupport
	global usbDeviceMsdDiskImageFile
	global usbDeviceMsdDiskImageFileAdd
	res = Database.activateComponents(["HarmonyCore"])
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		res = Database.activateComponents(["drv_usbhs_v1"])
		speed = Database.getSymbolValue("drv_usbhs_v1", "USB_SPEED")
		driverIndex = "DRV_USBHSV1_INDEX_0"
		driverInterface = "DRV_USBHSV1_DEVICE_INTERFACE"
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
		res = Database.activateComponents(["drv_usbhs_v1"])
		speed = Database.getSymbolValue("drv_usbhs_v1", "USB_SPEED")
		driverIndex = "DRV_USBHS_INDEX_0"
		driverInterface = "DRV_USBHS_DEVICE_INTERFACE"
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMD20", "SAMD21", "SAMD51", "SAME51", "SAME53", "SAME54"]):
		res = Database.activateComponents(["drv_usbfs_v1"])
		speed = Database.getSymbolValue("drv_usbfs_v1", "USB_SPEED")
		driverIndex = "DRV_USBFSV1_INDEX_0"
		driverInterface = "DRV_USBFSV1_DEVICE_INTERFACE"
		
	# USB Device Speed 
	usbDeviceSpeed = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_SPEED", None)
	usbDeviceSpeed.setVisible(False)
	usbDeviceSpeed.setDefaultValue(speed)
	usbDeviceSpeed.setUseSingleDynamicValue(True)
	
	# USB Driver Index - This symbol actually should get set from a Driver dependency connected callback. 
	# This is temporary work around to initialize using hard coded values. 
	usbDeviceDriverIndex = usbDeviceComponent.createStringSymbol("CONFIG_USB_DRIVER_INDEX", None)
	usbDeviceDriverIndex.setVisible(False)
	usbDeviceDriverIndex.setDefaultValue(driverIndex)
	
	# USB Driver Interface -  This symbol actually should get set from a Driver dependency connected callback. 
	# This is temporary work around to initialize using hard coded values. 
	usbDeviceDriverInterface = usbDeviceComponent.createStringSymbol("CONFIG_USB_DRIVER_INTERFACE", None)
	usbDeviceDriverInterface.setVisible(False)
	usbDeviceDriverInterface.setDefaultValue(driverInterface)
	
	
	#Configuration descriptor size
	usbDeviceConfigDscrptrSize = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE", None)
	usbDeviceConfigDscrptrSize.setVisible(False)
	usbDeviceConfigDscrptrSize.setMin(0)
	usbDeviceConfigDscrptrSize.setDefaultValue(9)
	usbDeviceConfigDscrptrSize.setUseSingleDynamicValue(True)
	
	#Number of interfaces
	usbDeviceInterfacesNumber = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_INTERFACES_NUMBER", None)	
	usbDeviceInterfacesNumber.setVisible(False)
	usbDeviceInterfacesNumber.setMin(0)
	usbDeviceInterfacesNumber.setDefaultValue(0)
	usbDeviceInterfacesNumber.setUseSingleDynamicValue(True)
	
	# Number of function numbers registered to this Device Layer Instance 
	usbDeviceFunctionNumber = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTIONS_NUMBER", None)
	usbDeviceFunctionNumber.setLabel("Number of Functions")	
	usbDeviceFunctionNumber.setVisible(True)
	usbDeviceFunctionNumber.setMin(0)
	usbDeviceFunctionNumber.setDefaultValue(0)
	usbDeviceFunctionNumber.setUseSingleDynamicValue(True)
	usbDeviceFunctionNumber.setReadOnly(True)
	
	# USB Device Endpoint Number 
	usbDeviceEndpointsNumber = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_ENDPOINTS_NUMBER", None)
	usbDeviceEndpointsNumber.setLabel("Number of Endpoints")	
	usbDeviceEndpointsNumber.setVisible(False)
	usbDeviceEndpointsNumber.setMin(0)
	usbDeviceEndpointsNumber.setDefaultValue(0)
	usbDeviceEndpointsNumber.setUseSingleDynamicValue(True)
	usbDeviceEndpointsNumber.setReadOnly(True)
	
	# USB Device events enable 
	usbDeviceEventsEnable = usbDeviceComponent.createMenuSymbol("USB_DEVICE_EVENTS", None)
	usbDeviceEventsEnable.setLabel("Special Events")	
	usbDeviceEventsEnable.setVisible(True)
	
	#SOF Event Enable 
	usbDeviceEventEnableSOF = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_EVENT_ENABLE_SOF", usbDeviceEventsEnable)
	usbDeviceEventEnableSOF.setLabel("Enable SOF Events")	
	usbDeviceEventEnableSOF.setVisible(True)	
		
	#Set Descriptor Event Enable 
	usbDeviceEventEnableSetDescriptor = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_EVENT_ENABLE_SET_DESCRIPTOR", usbDeviceEventsEnable)
	usbDeviceEventEnableSetDescriptor.setLabel("Enable Set Descriptor Events")	
	usbDeviceEventEnableSetDescriptor.setVisible(True)	
	
	#Synch Frame Event Enable 
	usbDeviceEventEnableSynchFrame = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_EVENT_ENABLE_SYNCH_FRAME", usbDeviceEventsEnable)
	usbDeviceEventEnableSynchFrame.setLabel("Enable Synch Frame Events")	
	usbDeviceEventEnableSynchFrame.setVisible(True)	
	
	# USB Device Features enable 
	usbDeviceFeatureEnable = usbDeviceComponent.createMenuSymbol("USB_DEVICE_FEATURES", None)
	usbDeviceFeatureEnable.setLabel("Special Features")	
	usbDeviceFeatureEnable.setVisible(True)
	
	# Advanced string descriptor table enable 
	usbDeviceFeatureEnableAdvancedStringDescriptorTable = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FEATURE_ENABLE_ADVANCED_STRING_DESCRIPTOR_TABLE", usbDeviceFeatureEnable)
	usbDeviceFeatureEnableAdvancedStringDescriptorTable.setLabel("Enable advanced String Descriptor Table")
	usbDeviceFeatureEnableAdvancedStringDescriptorTable.setVisible(True)
	
	#Microsoft OS descriptor support Enable 
	usbDeviceFeatureEnableMicrosoftOsDescriptor = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FEATURE_ENABLE_MICROSOFT_OS_DESCRIPTOR", usbDeviceFeatureEnableAdvancedStringDescriptorTable)
	usbDeviceFeatureEnableMicrosoftOsDescriptor.setLabel("Enable Microsoft OS Descriptor Support")	
	usbDeviceFeatureEnableMicrosoftOsDescriptor.setVisible(False)	
	usbDeviceFeatureEnableMicrosoftOsDescriptor.setDependencies(blUSBDeviceFeatureEnableMicrosoftOsDescriptor, ["CONFIG_USB_DEVICE_FEATURE_ENABLE_ADVANCED_STRING_DESCRIPTOR_TABLE"])

	#BOS descriptor support Enable 
	usbDeviceFeatureEnableBosDescriptor = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FEATURE_ENABLE_BOS_DESCRIPTOR", usbDeviceFeatureEnable)
	usbDeviceFeatureEnableBosDescriptor.setLabel("Enable BOS Descriptor Support")	
	usbDeviceFeatureEnableBosDescriptor.setVisible(True)	
	
	# Enable Auto timed remote wakeup functions  
	usbDeviceFeatureEnableAutioTimeRemoteWakeup = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FEATURE_ENABLE_AUTO_TIMED_REMOTE_WAKEUP_FUNCTIONS", usbDeviceFeatureEnable)
	usbDeviceFeatureEnableAutioTimeRemoteWakeup.setLabel("Use Auto Timed Remote Wake up Functions")	
	usbDeviceFeatureEnableAutioTimeRemoteWakeup.setVisible(False)	
	
	# USB Device EP0 Buffer Size  
	usbDeviceEp0BufferSize = usbDeviceComponent.createComboSymbol("CONFIG_USB_DEVICE_EP0_BUFFER_SIZE", None, usbDeviceEp0BufferSizes)
	usbDeviceEp0BufferSize.setLabel("Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setVisible(True)
	usbDeviceEp0BufferSize.setDescription("Select Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setDefaultValue("64")
	
	# USB Device Vendor ID 
	usbDeviceVendorId = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_VENDOR_ID_IDX0", None)
	usbDeviceVendorId.setLabel("Vendor ID")
	usbDeviceVendorId.setVisible(True)
	usbDeviceVendorId.setDefaultValue("0x04D8")
	
	# USB Device Product ID Selection
	usbDeviceProductId = usbDeviceComponent.createComboSymbol("CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0", None, usbDeviceDemoList)
	usbDeviceProductId.setLabel("Product ID Selection")
	usbDeviceProductId.setVisible(True)
	usbDeviceProductId.setDefaultValue("Enter Product ID")
	
	# USB Device Product ID 
	usbDeviceProductId = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_PRODUCT_ID_IDX0", None)
	usbDeviceProductId.setLabel("Product ID")
	usbDeviceProductId.setVisible(True)
	usbDeviceProductId.setDefaultValue("0x0000")
	usbDeviceProductId.setUseSingleDynamicValue(True)
	usbDeviceProductId.setDependencies(blUSBDeviceProductIDSelection,["CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0"])
	
	# USB Device Manufacturer String 
	usbDeviceManufacturerString = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_MANUFACTURER_STRING", None)
	usbDeviceManufacturerString.setLabel("Manufacturer String")
	usbDeviceManufacturerString.setVisible(True)
	usbDeviceManufacturerString.setDefaultValue("Microchip Technology Inc.")
	
	# USB Device Product String 
	usbDeviceProductString = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR", None)
	usbDeviceProductString.setLabel("Product String Selection")
	usbDeviceProductString.setVisible(True)
	usbDeviceProductString.setDefaultValue("Enter Product string here")
	usbDeviceProductString.setUseSingleDynamicValue(True)
	usbDeviceProductString.setDependencies(blUSBDeviceProductStringSelection,["CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0"])

	# USB Device IAD Enable 
	usbDeviceIadEnable = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_DESCRIPTOR_IAD_ENABLE", None)
	usbDeviceIadEnable.setVisible(False)
	usbDeviceIadEnable.setDefaultValue(False)
	usbDeviceIadEnable.setUseSingleDynamicValue(True)
	
	# USB Device MSD Support 
	usbDeviceMsdSupport = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_USE_MSD", None)
	usbDeviceMsdSupport.setVisible(False)
	usbDeviceMsdSupport.setDefaultValue(False)
	usbDeviceMsdSupport.setUseSingleDynamicValue(True)
	
	
	# USB Device MSD Disk Image file  
	usbDeviceMsdDiskImageFileAdd = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_MSD_DISK_IMAGE", None)
	usbDeviceMsdDiskImageFileAdd.setVisible(False)
	usbDeviceMsdDiskImageFileAdd.setDefaultValue(False)
	usbDeviceMsdDiskImageFileAdd.setUseSingleDynamicValue(True)
	usbDeviceMsdDiskImageFileAdd.setDependencies(checkIfDiskImagefileNeeded,["CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0"])

	# Read Queue Size for Vendor 
	usbDeviceVendorReadQueueSize = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_ENDPOINT_READ_QUEUE_SIZE", None)
	usbDeviceVendorReadQueueSize.setLabel("Combined Queue Depth")
	usbDeviceVendorReadQueueSize.setMin(0)
	usbDeviceVendorReadQueueSize.setMax(32767)
	usbDeviceVendorReadQueueSize.setDefaultValue(0)
	usbDeviceVendorReadQueueSize.setUseSingleDynamicValue(True)
	usbDeviceVendorReadQueueSize.setVisible(False)
	
	# Write Queue Size for Vendor
	usbDeviceVendorWriteQueueSize = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_ENDPOINT_WRITE_QUEUE_SIZE", None)
	usbDeviceVendorWriteQueueSize.setLabel("Combined Queue Depth")
	usbDeviceVendorWriteQueueSize.setMin(0)
	usbDeviceVendorWriteQueueSize.setMax(32767)
	usbDeviceVendorWriteQueueSize.setDefaultValue(0)
	usbDeviceVendorWriteQueueSize.setUseSingleDynamicValue(True)
	usbDeviceVendorWriteQueueSize.setVisible(False)
	
	configName = Variables.get("__CONFIGURATION_NAME")
	
	enable_rtos_settings = False

	if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
		enable_rtos_settings = True

	# RTOS Settings 
	usbDeviceRTOSMenu = usbDeviceComponent.createMenuSymbol(None, None)
	usbDeviceRTOSMenu.setLabel("RTOS settings")
	usbDeviceRTOSMenu.setDescription("RTOS settings")
	usbDeviceRTOSMenu.setVisible(enable_rtos_settings)
	usbDeviceRTOSMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])

	usbDeviceRTOSTask = usbDeviceComponent.createComboSymbol("USB_DEVICE_RTOS", usbDeviceRTOSMenu, ["Standalone"])
	usbDeviceRTOSTask.setLabel("Run Library Tasks As")
	usbDeviceRTOSTask.setDefaultValue("Standalone")
	usbDeviceRTOSTask.setVisible(False)

	usbDeviceRTOSStackSize = usbDeviceComponent.createIntegerSymbol("USB_DEVICE_RTOS_STACK_SIZE", usbDeviceRTOSMenu)
	usbDeviceRTOSStackSize.setLabel("Stack Size")
	usbDeviceRTOSStackSize.setDefaultValue(1024)
	usbDeviceRTOSStackSize.setReadOnly(True)

	usbDeviceRTOSTaskPriority = usbDeviceComponent.createIntegerSymbol("USB_DEVICE_RTOS_TASK_PRIORITY", usbDeviceRTOSMenu)
	usbDeviceRTOSTaskPriority.setLabel("Task Priority")
	usbDeviceRTOSTaskPriority.setDefaultValue(1)

	usbDeviceRTOSTaskDelay = usbDeviceComponent.createBooleanSymbol("USB_DEVICE_RTOS_USE_DELAY", usbDeviceRTOSMenu)
	usbDeviceRTOSTaskDelay.setLabel("Use Task Delay?")
	usbDeviceRTOSTaskDelay.setDefaultValue(True)

	usbDeviceRTOSTaskDelayVal = usbDeviceComponent.createIntegerSymbol("USB_DEVICE_RTOS_DELAY", usbDeviceRTOSMenu)
	usbDeviceRTOSTaskDelayVal.setLabel("Task Delay")
	usbDeviceRTOSTaskDelayVal.setDefaultValue(10) 
	usbDeviceRTOSTaskDelayVal.setVisible((usbDeviceRTOSTaskDelay.getValue() == True))
	usbDeviceRTOSTaskDelayVal.setDependencies(setVisible, ["USB_DEVICE_RTOS_USE_DELAY"])
	
	################################################
	# system_definitions.h file for USB Device Layer    
	################################################
	usbDeviceSystemDefFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemDefFile.setType("STRING")
	usbDeviceSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceSystemDefFile.setSourcePath("templates/device/system_definitions.h.device_includes.ftl")
	usbDeviceSystemDefFile.setMarkup(True)
	
	usbDeviceSystemDefObjFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemDefObjFile.setType("STRING")
	usbDeviceSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
	usbDeviceSystemDefObjFile.setSourcePath("templates/device/system_definitions.h.device_objects.ftl")
	usbDeviceSystemDefObjFile.setMarkup(True)
	
	usbDeviceSystemDefExtFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemDefExtFile.setType("STRING")
	usbDeviceSystemDefExtFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_EXTERNS")
	usbDeviceSystemDefExtFile.setSourcePath("templates/device/system_definitions.h.device_externs.ftl")
	usbDeviceSystemDefExtFile.setMarkup(True)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceSystemConfigFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemConfigFile.setType("STRING")
	usbDeviceSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceSystemConfigFile.setSourcePath("templates/device/system_config.h.device.ftl")
	usbDeviceSystemConfigFile.setMarkup(True)
	
	# Function driver configurations are added to the following list
	usbDeviceFunctionDriverConfigist = usbDeviceComponent.createListSymbol("LIST_SYSTEM_CONFIG_H_USB_DEVICE_FUNCTION_DRIVER", None)
	
	################################################
	# system_init.c file for USB Device stack    
	################################################
	usbDeviceSystemInitDataFile = usbDeviceComponent.createFileSymbol(None, None)
	#usbDeviceSystemInitDataFile.setType("STRING")
	#usbDeviceSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA")
	usbDeviceSystemInitDataFile.setType("SOURCE")
	usbDeviceSystemInitDataFile.setOutputName("usb_device_init_data.c")
	usbDeviceSystemInitDataFile.setDestPath("")
	usbDeviceSystemInitDataFile.setProjectPath("config/" + configName + "/")
	usbDeviceSystemInitDataFile.setSourcePath("templates/device/system_init_c_device_data.ftl")
	usbDeviceSystemInitDataFile.setMarkup(True)
	usbDeviceFunctionInitEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_FUNCTION_INIT_ENTRY", None)
	usbDeviceFunctionEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_FUNCTION_ENTRY", None)
	usbDeviceFunctionDscrptrHSEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY", None)
	usbDeviceFunctionDscrptrFSEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY", None)
	usbDeviceDescriptorClassCodeEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY", None)
	
	
	usbDeviceSystemInitCallsFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemInitCallsFile.setType("STRING")
	usbDeviceSystemInitCallsFile.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
	usbDeviceSystemInitCallsFile.setSourcePath("templates/device/system_init_c_device_calls.ftl")
	usbDeviceSystemInitCallsFile.setMarkup(True)
	
	################################################
	# system_tasks.c file for USB Device stack  
	################################################
	usbDeviceSystemTasksFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemTasksFile.setType("STRING")
	usbDeviceSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
	usbDeviceSystemTasksFile.setSourcePath("templates/device/system_tasks_c_device.ftl")
	usbDeviceSystemTasksFile.setMarkup(True)
	
	usbDeviceSystemTasksFileRTOS = usbDeviceComponent.createFileSymbol("USB_DEVICE_SYS_RTOS_TASK", None)
	usbDeviceSystemTasksFileRTOS.setType("STRING")
	usbDeviceSystemTasksFileRTOS.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	usbDeviceSystemTasksFileRTOS.setSourcePath("templates/device/system_tasks_c_device_rtos.ftl")
	usbDeviceSystemTasksFileRTOS.setMarkup(True)
	usbDeviceSystemTasksFileRTOS.setEnabled(enable_rtos_settings)
	
	
	################################################
	# USB Device Layer Files 
	################################################
	usbDeviceHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device.h', usbDeviceComponent, usbDeviceHeaderFile, "middleware/", "/usb/", True, None)
	
	usbDeviceCommonHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_common.h', usbDeviceComponent, usbDeviceCommonHeaderFile, "middleware/", "/usb/", True, None)
	
	usbDeviceChapter9HeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_chapter_9.h', usbDeviceComponent, usbDeviceChapter9HeaderFile, "middleware/", "/usb/", True, None)
	
	usbDeviceLocalHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device_local.h', usbDeviceComponent, usbDeviceLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	usbDeviceMappingHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device_mapping.h', usbDeviceComponent, usbDeviceMappingHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	usbDeviceFunctionDriverHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device_function_driver.h', usbDeviceComponent, usbDeviceFunctionDriverHeaderFile, "middleware/src/", "/usb/src", True, None)

	usbDeviceSourceFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device.c', usbDeviceComponent, usbDeviceSourceFile, "middleware/src/", "/usb/src", True, None)
	
	usbExternalDependenciesFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_external_dependencies.h', usbDeviceComponent, usbExternalDependenciesFile, "middleware/src/", "/usb/src", True, None)
	
	################################################
	# USB Device Application files  
	################################################
	usbDeviceMsdDiskImageFile = usbDeviceComponent.createFileSymbol("USB_DEVICE_DISK_IMAGE_FILE", None)
	addFileName('diskImage.c', usbDeviceComponent, usbDeviceMsdDiskImageFile, "templates/device/msd/", "", usbDeviceMsdDiskImageFileAdd.getValue(), None)
	#usbDeviceMsdDiskImageFile.setDependencies(checkIfDiskImagefileNeeded, ["CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0"])
	# if (usbDeviceMsdDiskImageFileAdd.getValue() == True):
		# print("Disk Image File Enabled")
		# usbDeviceMsdDiskImageFile.setEnabled(True)
	# else:	
		# print("Disk Image File Disabled")
		# usbDeviceMsdDiskImageFile.setEnabled(False)
		
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
		symbol.setDependencies(callback, ["USB_DEVICE_FUNCTION_1_DEVICE_CLASS"])
		
def blUsbLog (usbSymbolSource, event):
	if (usbDebugLogs == 1):
			print("Source: " + usbSymbolSource.getID().encode('ascii', 'ignore'), "id: " + event["id"].encode('ascii', 'ignore') , "Value: ", event["value"])
			
def blUsbPrint(string):
	if (usbDebugLogs == 1):
		print(string)
		
def blUSBDeviceFeatureEnableMicrosoftOsDescriptor(usbSymbolSource, event):
	blUsbLog(usbSymbolSource, event)
	if (event["value"] == True):		
		blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
		
def checkIfDiskImagefileNeeded (usbSymbolSource, event):
	global usbDeviceMsdDiskImageFile
	index = usbDeviceDemoList.index(event["value"])
	if any(x in usbDeviceDemoList[index] for x in ["msd_basic_demo", "hid_msd_demo" , "cdc_msd_basic_demo" , "cdc_serial_emulator_msd_demo"]):
		usbSymbolSource.setValue(True, 2)
		usbDeviceMsdDiskImageFile.setEnabled(True)
		print ("Disk Image Enabled")
	else:
		usbSymbolSource.setValue(False, 2)
		usbDeviceMsdDiskImageFile.setEnabled(False)
	
def blUSBDeviceProductIDSelection(usbSymbolSource, event):
	print("blUSBDeviceProductIDSelection is called")
	index = usbDeviceDemoList.index(event["value"])
	usbSymbolSource.setValue(usbDeviceDemoProductList[index],2)
	
def blUSBDeviceProductStringSelection(usbSymbolSource, event):
	index = usbDeviceDemoList.index(event["value"])
	usbSymbolSource.setValue(usbDeviceProductStringList[index],2)
	
	
		
