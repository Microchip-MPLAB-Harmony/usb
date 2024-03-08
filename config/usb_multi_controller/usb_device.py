"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
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
#constants 
USB_DEVICE_CONFIG_DESCRIPTOR_DEFAULT_VALUE = 9 
usbDeviceFunctionsNumberMax = 10
usbDeviceFunctionsNumberDefaultValue = 2
usbDeviceFunctionDriverList = [] 

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
"cdc_msd_sdcard_demo",
"cdc_serial_emulator_demo",
"cdc_serial_emulator_msd_demo",
"hid_basic_demo",
"usb_device_hid_bootloader",
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
"0x0057",
"0x000A",
"0x0057",
"0x003F",
"0x003C",
"0x005E",
"0x0055",
"0x0000",
"0x0054",
"0x0009",
"0x0009",
"0x0009",
"0x0009",
"0x00E7",
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
"CDC + MSD SD Card Demo",
"Simple CDC Device Demo",
"CDC + MSD Demo", 
"Simple HID Device Demo", 
"USB HID Bootloader",
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
usbDeviceSpeed = None
usbDeviceFunctionNumber = None
usbDeviceIadEnable = None
usbDeviceConfigDscrptrSize = None
usbDeviceInterfacesNumber = None
usbDeviceVendorReadQueueSize = None
usbDeviceVendorWriteQueueSize = None
usbDeviceEndpointsNumber = None
usbDeviceFunctionsNumberValue = usbDeviceFunctionsNumberDefaultValue
numEndpoints = 1
usbDeviceDriverInterface = None

#Duplicate variables for symbols updated by other modules 
usbDeviceFunctionsNumberLocal = 0
usbDeviceConfigDscrptrSizeLocal = USB_DEVICE_CONFIG_DESCRIPTOR_DEFAULT_VALUE
usbDeviceInterfaceNumberLocal = 0
usbDeviceEndpointsNumberLocal = 0
usbDeviceVendorReadQueueSizeLocal = 0
usbDeviceVendorWriteQueueSizeLocal = 0


usbDevicelayerInstance = None
usbDevicelayerIndex = None 

# A List of USB Device Function Menu Symbols
listUsbDeviceFunctionMenu = []

# List for holding Function Driver Specific details. 
listUsbDeviceFunctionType = []

listUsbDeviceStartInterfaceNumber = []
listUsbDeviceNumberOfInterfaces = []


usbDeviceMsdSupport = None
usbDeviceMsdDiskImageFile = None
usbDeviceMsdDiskImageFileAdd = None 
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
	
		
def onAttachmentConnected(source, target):
	global usbDeviceMsdSupport
	global usbDeviceMsdDiskImageFile
	global usbDeviceMsdDiskImageFileAdd
	global usbDeviceFunctionNumber
	global usbDeviceIadEnable
	global usbDeviceConfigDscrptrSize
	global usbDeviceInterfacesNumber
	global usbDeviceVendorReadQueueSize
	global usbDeviceVendorWriteQueueSize
	global usbDeviceSpeed
	global usbDeviceConfigDscrptrSizeLocal
	global usbDeviceVendorReadQueueSizeLocal
	global usbDeviceVendorWriteQueueSizeLocal
	global usbDeviceDriverInterface
	global usbDeviceFunctionDriverList
	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	remoteID_instance = remoteID[:-1]
	connectID = source["id"]
	targetID = target["id"]

	if (connectID == "usb_driver_dependency"):
		usbControllerInstance = ownerComponent.getSymbolByID("USB_DEVICE_INDEX")
		usbControllerInstance.clearValue()
		if any(x in Variables.get("__PROCESSOR") for x in ["PIC32CK"]):
			if remoteID.upper() == "DRV_USBFS":
				usbControllerInstance.setValue("DRV_USBFSV1_INDEX_0")
				usbDeviceDriverInterface.setValue("DRV_USBFSV1_DEVICE_INTERFACE")
			elif remoteID.upper() == "DRV_USBHS":
				usbControllerInstance.setValue("DRV_USBHS_INDEX_0")
				usbDeviceDriverInterface.setValue("DRV_USBHS_DEVICE_INTERFACE")
			else:
				print("USB Device Layer: There is an error occured")
		else:
			usbControllerInstance.setValue(remoteID.upper())

	if (remoteID == "usb_device_msd"):
		usbDeviceMsdSupport.setValue(True, 2)
	if (connectID == "usb_device"):
		usbDeviceFunctionDriverList.append(remoteID)

def onAttachmentDisconnected(source, target):
	global usbDeviceMsdSupport
	global usbDeviceCDCFunctionCount
	global usbDeviceConfigDscrptrSizeLocal
	global usbDeviceVendorReadQueueSizeLocal
	global usbDeviceVendorWriteQueueSizeLocal
	global usbDeviceFunctionDriverList
	global usbDeviceIadEnable

	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	remoteID_instance = remoteID[:-1]
	if (remoteID == "usb_device_msd"):
		usbDeviceMsdSupport.setValue(False, 2)
	if (connectID == "usb_device"):
		usbDeviceFunctionDriverList.remove(remoteID)
	isIadEnabled = False
	for function in usbDeviceFunctionDriverList:
		isIadEnabled = Database.getSymbolValue(function, "CONFIG_USB_DEVICE_FUNCTION_USE_IAD")
		if isIadEnabled == True: 
			break
	if isIadEnabled == False:
		usbDeviceIadEnable.setValue(False)
	
def handleMessage(messageID, args):	
	global usbDeviceFunctionNumber
	global usbDeviceIadEnable
	global usbDeviceConfigDscrptrSize
	global usbDeviceInterfacesNumber
	global usbDeviceVendorReadQueueSize
	global usbDeviceVendorWriteQueueSize
	global numEndpoints
	global usbDeviceEndpointsNumber
	global usbDeviceSpeed
	global usbDeviceFunctionsNumberLocal
	global usbDeviceConfigDscrptrSizeLocal
	global usbDeviceInterfaceNumberLocal
	global usbDeviceEndpointsNumberLocal
	global usbDeviceVendorReadQueueSizeLocal
	global usbDeviceVendorWriteQueueSizeLocal
	global usbDeviceFunctionDriverList
	if (messageID == "UPDATE_ENDPOINTS_NUMBER"):
		usbDeviceEndpointsNumberLocal += args["nFunction"]
		usbDeviceEndpointsNumber.setValue( usbDeviceEndpointsNumberLocal)
	elif (messageID == "UPDATE_FUNCTIONS_NUMBER"):
		usbDeviceFunctionsNumberLocal += args["nFunction"]
		usbDeviceFunctionNumber.setValue(usbDeviceFunctionsNumberLocal)
	elif (messageID == "UPDATE_IAD_ENABLE"):
		if(args["nFunction"]==True):
			usbDeviceIadEnable.setValue(True)
		elif(args["nFunction"]==False): 
			isIadEnabled = False
			for function in usbDeviceFunctionDriverList:
				isIadEnabled = Database.getSymbolValue(function, "CONFIG_USB_DEVICE_FUNCTION_USE_IAD")
				if isIadEnabled == True: 
					break
			if isIadEnabled == False:
				usbDeviceIadEnable.setValue(False)

	elif (messageID == "UPDATE_CONFIG_DESCRPTR_SIZE"): 
		usbDeviceConfigDscrptrSizeLocal += args["nFunction"]
		usbDeviceConfigDscrptrSize.setValue(usbDeviceConfigDscrptrSizeLocal)
	elif (messageID == "UPDATE_INTERFACES_NUMBER"): 
		usbDeviceInterfaceNumberLocal += args["nFunction"]
		usbDeviceInterfacesNumber.setValue(usbDeviceInterfaceNumberLocal)
	elif (messageID == "UPDATE_ENDPOINT_READ_QUEUE_SIZE"): 
		usbDeviceVendorReadQueueSizeLocal += (args["nFunction"])
		usbDeviceVendorReadQueueSize.setValue(usbDeviceVendorReadQueueSizeLocal)
	elif (messageID == "UPDATE_ENDPOINT_WRITE_QUEUE_SIZE"):
		usbDeviceVendorWriteQueueSizeLocal += (args["nFunction"])    
		usbDeviceVendorWriteQueueSize.setValue(usbDeviceVendorWriteQueueSizeLocal)
	elif (messageID == "USB_DEVICE_UPDATE_SPEED"):
		usbDeviceSpeed.setValue(args["usbDriverSpeed"])

def instantiateComponent(usbDeviceComponent,index):	
	global usbDeviceMsdSupport
	global usbDeviceMsdDiskImageFile
	global usbDeviceMsdDiskImageFileAdd
	global usbDeviceFunctionNumber
	global usbDeviceIadEnable
	global usbDeviceConfigDscrptrSize
	global usbDeviceInterfacesNumber
	global usbDeviceEndpointsNumber
	global usbDeviceVendorReadQueueSize
	global usbDeviceVendorWriteQueueSize
	global usbDevicelayerInstance
	global usbDevicelayerIndex
	global usbDeviceSpeed
	global usbDeviceFunctionsNumberLocal
	global usbDeviceDriverInterface

	usbDeviceFunctionsNumberLocal = 0 
	res = Database.activateComponents(["HarmonyCore"])
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32CZ", "PIC32CK"]):
		usbDevicelayerIndex = usbDeviceComponent.createIntegerSymbol("INDEX", None)
		usbDevicelayerIndex.setVisible(False)
		usbDevicelayerIndex.setDefaultValue(index)
		usbDevicelayerInstance = usbDeviceComponent.createStringSymbol("USB_DEVICE_INDEX", None)
		usbDevicelayerInstance.setLabel("USB Controller instance")
		usbDevicelayerInstance.setReadOnly(True)
		usbDevicelayerInstance.setHelp("mcc_configuration_device_layer")
		res = Database.activateComponents(["drv_usbhs_index"])
		driverIndex = "DRV_USBHS_INDEX_0"
		driverInterface = "DRV_USBHS_DEVICE_INTERFACE"
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MK"]):
		usbDevicelayerIndex = usbDeviceComponent.createIntegerSymbol("INDEX", None)
		usbDevicelayerIndex.setVisible(False)
		usbDevicelayerIndex.setDefaultValue(index)
		usbDevicelayerInstance = usbDeviceComponent.createStringSymbol("USB_DEVICE_INDEX", None)
		usbDevicelayerInstance.setLabel("USB Controller instance")
		usbDevicelayerInstance.setReadOnly(True)
		usbDevicelayerInstance.setHelp("mcc_configuration_device_layer")
		res = Database.activateComponents(["drv_usbfs_index"])
		speed = Database.getSymbolValue("drv_usbfs_index", "USB_SPEED")
		driverIndex = "DRV_USBFS_INDEX_0"
		driverInterface = "DRV_USBFS_DEVICE_INTERFACE"


		
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
		res = Database.activateComponents(["drv_usbhs_v1"])
		speed = Database.getSymbolValue("drv_usbhs_v1", "USB_SPEED")
		driverIndex = "DRV_USBHS_INDEX_0"
		driverInterface = "DRV_USBHS_DEVICE_INTERFACE"
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MX" , "PIC32MM"]):
		res = Database.activateComponents(["drv_usbfs_index"])
		speed = Database.getSymbolValue("drv_usbfs", "USB_SPEED")
		driverIndex = "DRV_USBFS_INDEX_0"
		driverInterface = "DRV_USBFS_DEVICE_INTERFACE"
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMD21", "SAMDA1", "SAMD5", "SAME5", "LAN9255", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "PIC32CM", "PIC32CX"]):
		res = Database.activateComponents(["drv_usbfs_v1"])
		speed = Database.getSymbolValue("drv_usbfs_v1", "USB_SPEED")
		driverIndex = "DRV_USBFSV1_INDEX_0"
		driverInterface = "DRV_USBFSV1_DEVICE_INTERFACE"
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2", "SAM9X60", "SAM9X7", "SAMA7"]):
		res = Database.activateComponents(["drv_usb_udphs"])
		speed = Database.getSymbolValue("drv_usb_udphs", "USB_SPEED")
		driverIndex = "DRV_USB_UDPHS_INDEX_0"
		driverInterface = "DRV_USB_UDPHS_DEVICE_INTERFACE"
		
	# USB Device Speed 
	usbDeviceSpeed = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_SPEED", None)
	usbDeviceSpeed.setVisible(False)
	usbDeviceSpeed.setDefaultValue("Full Speed")
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
	usbDeviceConfigDscrptrSize.setLabel("Configuration Descriptor Size")
	usbDeviceConfigDscrptrSize.setVisible(True)
	usbDeviceConfigDscrptrSize.setMin(0)
	usbDeviceConfigDscrptrSize.setDefaultValue(9)
	usbDeviceConfigDscrptrSize.setUseSingleDynamicValue(True)
	usbDeviceConfigDscrptrSize.setHelp("mcc_configuration_device_layer")

	
	#Number of interfaces
	usbDeviceInterfacesNumber = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_INTERFACES_NUMBER", None)	
	usbDeviceInterfacesNumber.setVisible(False)
	usbDeviceInterfacesNumber.setMin(0)
	usbDeviceInterfacesNumber.setDefaultValue(0)
	usbDeviceInterfacesNumber.setUseSingleDynamicValue(True)
	
	# Number of function numbers registered to this Device Layer Instance 
	usbDeviceFunctionNumber = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTIONS_NUMBER", None)
	usbDeviceFunctionNumber.setLabel("Number of Functions")	
        helpText = '''This indicates the number of Function Driver instances in the current application. This is provided
        for indication purposes only and is updated automatically based on the number of Function Drivers (and their 
        instances) in the current application.'''
	usbDeviceFunctionNumber.setVisible(True)
	usbDeviceFunctionNumber.setDescription(helpText)
	usbDeviceFunctionNumber.setMin(0)
	usbDeviceFunctionNumber.setValue(0)
	usbDeviceFunctionNumber.setUseSingleDynamicValue(True)
	usbDeviceFunctionNumber.setHelp("Function_Driver_Registration_Table")

	#usbDeviceFunctionNumber.setReadOnly(True)
	
	# USB Device Endpoint Number 
	usbDeviceEndpointsNumber = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_ENDPOINTS_NUMBER", None)
	usbDeviceEndpointsNumber.setLabel("Number of Endpoints")
	usbDeviceEndpointsNumber.setVisible(False)
	usbDeviceEndpointsNumber.setMin(0)
	usbDeviceEndpointsNumber.setValue(0)
	usbDeviceEndpointsNumber.setUseSingleDynamicValue(True)
	usbDeviceEndpointsNumber.setReadOnly(True)
	
	# USB Device Vendor ID 
	usbDeviceVendorId = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_VENDOR_ID_IDX0", None)
        helpText = '''Specify the Vendor ID (VID) to be specified in the Device
        Descriptor. This should be a value assigned by the USB IF. The value
        0x04D8 is assigned to Microchip Technology Inc'''
	usbDeviceVendorId.setLabel("Vendor ID")
	usbDeviceVendorId.setDescription(helpText)
	usbDeviceVendorId.setVisible(True)
	usbDeviceVendorId.setDefaultValue("0x04D8")
	usbDeviceVendorId.setHelp("mcc_configuration_device_layer")

	
	# USB Device Product ID Selection
	usbDeviceProductId = usbDeviceComponent.createComboSymbol("CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0", None, usbDeviceDemoList)
        helpText = '''Choose from a list of available demo applications with
        pre-assigned Product IDs or choose the 'Enter Product ID' option to
        enter a custom Product ID. This value is then assigned as the PID in
        the Device Descriptor'''
	usbDeviceProductId.setLabel("Product ID Selection")
        usbDeviceProductId.setDescription(helpText)
	usbDeviceProductId.setVisible(True)
	usbDeviceProductId.setDefaultValue("Enter Product ID")
	usbDeviceProductId.setHelp("mcc_configuration_device_layer")

	
	# USB Device Product ID 
	usbDeviceProductId = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_PRODUCT_ID_IDX0", None)
	usbDeviceProductId.setLabel("Product ID")
        helpText = '''If the Product ID selection is set to a available demo
        application, then this field is automatically populated with the
        Product ID of that demo application. If the Product ID selection is set
        to 'Enter Product ID' option, then enter the 16-bit Product ID in this
        field'''
        usbDeviceProductId.setDescription(helpText)
	usbDeviceProductId.setVisible(True)
	usbDeviceProductId.setDefaultValue("0x0000")
	usbDeviceProductId.setUseSingleDynamicValue(True)
	usbDeviceProductId.setDependencies(blUSBDeviceProductIDSelection,["CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0"])
	usbDeviceProductId.setHelp("mcc_configuration_device_layer")

	
	# USB Device Manufacturer String 
	usbDeviceManufacturerString = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_MANUFACTURER_STRING", None)
	usbDeviceManufacturerString.setLabel("Manufacturer String")
        helpText = '''Specify the Manufacturer String in this field.'''
        usbDeviceManufacturerString.setDescription(helpText)
	usbDeviceManufacturerString.setVisible(True)
	usbDeviceManufacturerString.setDefaultValue("Microchip Technology Inc.")
	usbDeviceManufacturerString.setHelp("mcc_configuration_device_layer")

	
	# USB Device Product String 
	usbDeviceProductString = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR", None)
	usbDeviceProductString.setLabel("Product String Selection")
	usbDeviceProductString.setVisible(True)
        helpText = '''If the Product ID selection is set to a available demo
        application, then this field is automatically populated with the
        Product String that describes the demo application. If the Product ID
        selection is set to 'Enter Product ID' option, then enter the Product
        String in this field'''
	usbDeviceProductString.setDefaultValue("Enter Product string here")
        usbDeviceProductString.setDescription(helpText)
	usbDeviceProductString.setUseSingleDynamicValue(True)
	usbDeviceProductString.setDependencies(blUSBDeviceProductStringSelection,["CONFIG_USB_DEVICE_PRODUCT_ID_SELECTION_IDX0"])
	usbDeviceProductString.setHelp("mcc_configuration_device_layer")

	
	
	# Serial Number String enable 
	usbDeviceFeatureSerialNumberStringEnable = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_SERIAL_NUMBER_STRING_ENABLE", None)
	usbDeviceFeatureSerialNumberStringEnable.setLabel("Add Device Serial Number")
        helpText = '''Enable this option to add Device Serial Number.'''
        usbDeviceFeatureSerialNumberStringEnable.setDescription(helpText)
	usbDeviceFeatureSerialNumberStringEnable.setVisible(True)
	usbDeviceFeatureSerialNumberStringEnable.setHelp("mcc_configuration_device_layer")

	
	# USB Device Serial Number String 
	usbDeviceSerialNumberString = usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_SERIAL_NUMBER_STRING_DESCRIPTOR", usbDeviceFeatureSerialNumberStringEnable)
	usbDeviceSerialNumberString.setLabel("Serial Number")
	usbDeviceSerialNumberString.setVisible(False)
        helpText = '''Enter Device Serial Number here. The Serial Number will be added as a String descriptor table.'''
	usbDeviceProductString.setDescription(helpText)
	usbDeviceSerialNumberString.setDefaultValue("123456789012")
	usbDeviceSerialNumberString.setDependencies(blUSBDeviceEnableSerialNumberString,["CONFIG_USB_DEVICE_SERIAL_NUMBER_STRING_ENABLE"])
		

	# USB Device IAD Enable 
	usbDeviceIadEnable = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_DESCRIPTOR_IAD_ENABLE", None)
	usbDeviceIadEnable.setLabel("Enable Interface Association Descriptor(IAD)")
	usbDeviceIadEnable.setVisible(True)
	usbDeviceIadEnable.setDefaultValue(False)
	usbDeviceIadEnable.setUseSingleDynamicValue(True)
	usbDeviceIadEnable.setHelp("mcc_configuration_device_layer")
	
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
	
	# USB Device Configurations
	usbDeviceConfigurations = usbDeviceComponent.createMenuSymbol("USB_DEVICE_CONFIGURATION", None)
	usbDeviceConfigurations.setLabel("USB Device Configuration")	
	usbDeviceConfigurations.setVisible(True)
	
	# USB Device Attributes Self Powered. 
	usbDeviceConfigDescritorAttributeSelfPowered = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_ATTRIBUTE_SELF_POWERED", usbDeviceConfigurations)
	usbDeviceConfigDescritorAttributeSelfPowered.setLabel("Self Powered")
	usbDeviceConfigDescritorAttributeSelfPowered.setVisible(True)
	helpText = '''Enable this option if the USB Device is Self Powered. If not it implies that the Device is Bus Powered.'''
	usbDeviceConfigDescritorAttributeSelfPowered.setDescription(helpText)
	usbDeviceConfigDescritorAttributeSelfPowered.setDefaultValue(True)
	usbDeviceConfigDescritorAttributeSelfPowered.setHelp("mcc_configuration_device_layer")

	
	
	# USB Device Attributes Remote Wakeup. 
	usbDeviceConfigDescritorAttributeRemoteWakeup = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_ATTRIBUTE_REMOTE_WAKEUP", usbDeviceConfigurations)
	usbDeviceConfigDescritorAttributeRemoteWakeup.setLabel("Remote Wakeup Feature")
	usbDeviceConfigDescritorAttributeRemoteWakeup.setVisible(True)
	helpText = '''Enable this option if the USB Device supports remote wakeup 
				  feature. This informs the Host that the device is capable of 
				  generating wakeup signal when the bus is suspended '''
	usbDeviceConfigDescritorAttributeRemoteWakeup.setDescription(helpText)
	usbDeviceConfigDescritorAttributeRemoteWakeup.setDefaultValue(False)
	usbDeviceConfigDescritorAttributeRemoteWakeup.setHelp("mcc_configuration_device_layer")

	
	
	# USB Device max power 
	usbDeviceConfigDescritorMaxPower = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_MAX_POWER", usbDeviceConfigurations)
	usbDeviceConfigDescritorMaxPower.setLabel("Maximum Power (mA)")
	usbDeviceConfigDescritorMaxPower.setVisible(True)
	helpText = '''Maximum power consumption of the USB device from the bus in this specific
					configuration when the device is fully operational.'''
	usbDeviceConfigDescritorMaxPower.setDescription(helpText)
	usbDeviceConfigDescritorMaxPower.setMin(1)
	usbDeviceConfigDescritorMaxPower.setMax(250)
	usbDeviceConfigDescritorMaxPower.setDefaultValue(100)
	usbDeviceConfigDescritorMaxPower.setHelp("mcc_configuration_device_layer")

	
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
	usbDeviceRTOSStackSize.setMin(0)
	usbDeviceRTOSStackSize.setDefaultValue(1024)
	usbDeviceRTOSStackSize.setReadOnly(True)

	usbDeviceRTOSTaskPriority = usbDeviceComponent.createIntegerSymbol("USB_DEVICE_RTOS_TASK_PRIORITY", usbDeviceRTOSMenu)
	usbDeviceRTOSTaskPriority.setLabel("Task Priority")
	usbDeviceRTOSTaskPriority.setMin(0)
	usbDeviceRTOSTaskPriority.setDefaultValue(1)

	usbDeviceRTOSTaskDelay = usbDeviceComponent.createBooleanSymbol("USB_DEVICE_RTOS_USE_DELAY", usbDeviceRTOSMenu)
	usbDeviceRTOSTaskDelay.setLabel("Use Task Delay?")
	usbDeviceRTOSTaskDelay.setDefaultValue(True)

	usbDeviceRTOSTaskDelayVal = usbDeviceComponent.createIntegerSymbol("USB_DEVICE_RTOS_DELAY", usbDeviceRTOSMenu)
	usbDeviceRTOSTaskDelayVal.setLabel("Task Delay")
	usbDeviceRTOSTaskDelayVal.setMin(0)
	usbDeviceRTOSTaskDelayVal.setDefaultValue(10) 
	usbDeviceRTOSTaskDelayVal.setVisible((usbDeviceRTOSTaskDelay.getValue() == True))
	usbDeviceRTOSTaskDelayVal.setDependencies(setVisible, ["USB_DEVICE_RTOS_USE_DELAY"])
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MK", "PIC32CZ", "PIC32CK"]):
		sourcePath = "templates/device/usbdevice_multi/"
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MX" , "PIC32MM"]):
		sourcePath = "templates/device/"
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255", "SAMD20", "SAMD21", "SAMDA1","SAML21","SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "PIC32CM", "PIC32CX"]):
		sourcePath = "templates/device/"
	
	################################################
	# system_definitions.h file for USB Device Layer    
	################################################
	usbDeviceSystemDefFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemDefFile.setType("STRING")
	usbDeviceSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceSystemDefFile.setSourcePath(sourcePath +"system_definitions.h.device_includes.ftl")
	usbDeviceSystemDefFile.setMarkup(True)
	
	usbDeviceSystemDefObjFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemDefObjFile.setType("STRING")
	usbDeviceSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
	usbDeviceSystemDefObjFile.setSourcePath(sourcePath +"system_definitions.h.device_objects.ftl")
	usbDeviceSystemDefObjFile.setMarkup(True)
	
	usbDeviceSystemDefExtFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemDefExtFile.setType("STRING")
	usbDeviceSystemDefExtFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_EXTERNS")
	usbDeviceSystemDefExtFile.setSourcePath(sourcePath +"system_definitions.h.device_externs.ftl")
	usbDeviceSystemDefExtFile.setMarkup(True)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceSystemConfigFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemConfigFile.setType("STRING")
	usbDeviceSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceSystemConfigFile.setSourcePath(sourcePath +"system_config.h.device.ftl")
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
	usbDeviceSystemInitDataFile.setOutputName("usb_device_init_data_" + str(index) + ".c")
	usbDeviceSystemInitDataFile.setDestPath("")
	usbDeviceSystemInitDataFile.setProjectPath("config/" + configName + "/")
	usbDeviceSystemInitDataFile.setSourcePath(sourcePath +"system_init_c_device_data.ftl")
	usbDeviceSystemInitDataFile.setMarkup(True)
	
	usbDeviceFunctionInitEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_FUNCTION_INIT_ENTRY", None)
	usbDeviceFunctionEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_FUNCTION_ENTRY", None)
	usbDeviceFunctionDscrptrHSEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY", None)
	usbDeviceFunctionDscrptrFSEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY", None)
	usbDeviceDescriptorClassCodeEntry = usbDeviceComponent.createListSymbol("LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY", None)
	
	
	usbDeviceSystemInitCallsFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemInitCallsFile.setType("STRING")
	usbDeviceSystemInitCallsFile.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
	usbDeviceSystemInitCallsFile.setSourcePath(sourcePath +"system_init_c_device_calls.ftl")
	usbDeviceSystemInitCallsFile.setMarkup(True)
	
	################################################
	# system_tasks.c file for USB Device stack  
	################################################
	usbDeviceSystemTasksFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemTasksFile.setType("STRING")
	usbDeviceSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
	usbDeviceSystemTasksFile.setSourcePath(sourcePath +"system_tasks_c_device.ftl")
	usbDeviceSystemTasksFile.setMarkup(True)
	
	usbDeviceSystemTasksFileRTOS = usbDeviceComponent.createFileSymbol("USB_DEVICE_SYS_RTOS_TASK", None)
	usbDeviceSystemTasksFileRTOS.setType("STRING")
	usbDeviceSystemTasksFileRTOS.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
	usbDeviceSystemTasksFileRTOS.setSourcePath(sourcePath +"system_tasks_c_device_rtos.ftl")
	usbDeviceSystemTasksFileRTOS.setMarkup(True)
	usbDeviceSystemTasksFileRTOS.setEnabled(enable_rtos_settings)
	usbDeviceSystemTasksFileRTOS.setDependencies(genRtosTask, ["HarmonyCore.SELECT_RTOS"])
	
	
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
	configName1 = Variables.get("__CONFIGURATION_NAME")
	usbDeviceMsdDiskImageFile.setProjectPath("config/" + configName1 + "")
	usbDeviceMsdDiskImageFile.setSourcePath("templates/device/msd/" + 'diskImage.c')
	usbDeviceMsdDiskImageFile.setOutputName('diskImage.c')
	usbDeviceMsdDiskImageFile.setDestPath("")
	usbDeviceMsdDiskImageFile.setType("SOURCE")
	usbDeviceMsdDiskImageFile.setEnabled(usbDeviceMsdDiskImageFileAdd.getValue())
	#addFileName('diskImage.c', usbDeviceComponent, usbDeviceMsdDiskImageFile, "templates/device/msd/", "", usbDeviceMsdDiskImageFileAdd.getValue(), None)

		
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
		symbol.setDependencies(callback, ["USB_DEVICE_FUNCTION_1_DEVICE_CLASS"])
		
def blUsbLog (usbSymbolSource, event):
	if (usbDebugLogs == 1):
			print("Source: " + usbSymbolSource.getID().encode('ascii', 'ignore'), "id: " + event["id"].encode('ascii', 'ignore') , "Value: ", event["value"])
			
def blUsbPrint(string):
	if (usbDebugLogs == 1):
		print(string)
		
		
def checkIfDiskImagefileNeeded (usbSymbolSource, event):
	global usbDeviceMsdDiskImageFile
	index = usbDeviceDemoList.index(event["value"])
	if any(x in usbDeviceDemoList[index] for x in ["msd_basic_demo", "hid_msd_demo" , "cdc_msd_basic_demo" , "cdc_serial_emulator_msd_demo"]):
		usbSymbolSource.setValue(True, 2)
		usbDeviceMsdDiskImageFile.setEnabled(True)
	else:
		usbSymbolSource.setValue(False, 2)
		usbDeviceMsdDiskImageFile.setEnabled(False)
	
def blUSBDeviceProductIDSelection(usbSymbolSource, event):
	index = usbDeviceDemoList.index(event["value"])
	usbSymbolSource.setValue(usbDeviceDemoProductList[index],2)
	
def blUSBDeviceProductStringSelection(usbSymbolSource, event):
	index = usbDeviceDemoList.index(event["value"])
	usbSymbolSource.setValue(usbDeviceProductStringList[index],2)
	
	
def blUSBDeviceEnableSerialNumberString(usbSymbolSource, event):
	if (event["value"] == True):		
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)	
