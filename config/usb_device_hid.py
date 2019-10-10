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
currentQSizeRead  = 1
currentQSizeWrite = 1
hidInterfacesNumber = 1
hidDescriptorSize = 32
hidEndpointsPic32 = 1
hidEndpointsSAM = 2

usbDeviceHidReportList = ["Mouse", "Keyboard", "Joystick", "Custom"]

indexFunction = None
configValue = None
startInterfaceNumber = None
numberOfInterfaces = None
usbDeviceHidReportType = None
queueSizeRead = None
queueSizeWrite = None
usbDeviceHidBufPool = None
epNumberInterruptIn = None
epNumberInterruptOut = None

def onAttachmentConnected(source, target):
	print ("HID Function Driver: Attached")
	global hidInterfacesNumber
	global hidDescriptorSize 
	
	global indexFunction
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global usbDeviceHidReportType
	global queueSizeRead
	global queueSizeWrite
	global usbDeviceHidBufPool
	global epNumberInterruptIn
	global epNumberInterruptOut
	global hidEndpointsPic32
	global hidEndpointsSAM
	
	ownerComponent = source["component"]
	dependencyID = source["id"]
	if (dependencyID == "usb_device_dependency"):
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		if readValue != None: 
			args = {"nFunction":readValue + 1}
			res = Database.sendMessage("usb_device", "UPDATE_FUNCTIONS_NUMBER", args)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if readValue != None: 
			args = {"nFunction": readValue + hidDescriptorSize}
			res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if readValue != None: 
			args = {"nFunction": readValue + hidInterfacesNumber}
			res = Database.sendMessage("usb_device", "UPDATE_INTERFACES_NUMBER", args)
			startInterfaceNumber.setValue(readValue, 1)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if readValue != None:
			if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MK", "SAMD21", "SAMD51", "SAME51", "SAME53", "SAME54", "SAML21", "SAML22", "SAMD11"]):
				args = {"nFunction":  readValue + hidEndpointsPic32}
				res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
				epNumberInterruptIn.setValue(readValue + 1, 1)
				epNumberInterruptOut.setValue(readValue + 1, 1)
			else:
				args = {"nFunction":  readValue + hidEndpointsSAM}
				res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
				epNumberInterruptIn.setValue(readValue + 1, 1)
				epNumberInterruptOut.setValue(readValue + 2, 1)

def onAttachmentDisconnected(source, target):
	global hidEndpointsPic32
	global hidEndpointsSAM
	
	print ("HID Function Driver: Detached")
	ownerComponent = source["component"]
	dependencyID = source["id"]
	if (dependencyID == "usb_device_dependency"):
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		if readValue != None: 
			args = {"nFunction":readValue - 1}
			res = Database.sendMessage("usb_device", "UPDATE_FUNCTIONS_NUMBER", args)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if readValue != None: 
			args = {"nFunction": readValue - hidDescriptorSize }
			res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if readValue != None: 
			rgs = {"nFunction":   readValue - hidInterfacesNumber}
			res = Database.sendMessage("usb_device", "UPDATE_INTERFACES_NUMBER", args)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if readValue != None:
			if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MK", "SAMD21", "SAMD51", "SAME51", "SAME53", "SAME54", "SAML21", "SAML22", "SAMD11"]):
				args = {"nFunction": readValue - hidEndpointsPic32 }
				res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
			else:
				args = {"nFunction":  readValue - hidEndpointsSAM }
				res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
				
			
def destroyComponent(component):
	print ("HID Function Driver: Destroyed")
	
	
def usbDeviceHidBufferQueueSize(usbSymbolSource, event):
	global currentQSizeRead
	global currentQSizeWrite
	queueDepthCombined = Database.getSymbolValue("usb_device_hid", "CONFIG_USB_DEVICE_HID_QUEUE_DEPTH_COMBINED")
	if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE"):
		queueDepthCombined = queueDepthCombined - currentQSizeRead + event["value"]
		currentQSizeRead = event["value"]
	if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"):
		queueDepthCombined = queueDepthCombined - currentQSizeWrite  + event["value"]
		currentQSizeWrite = event["value"]
	args = {"hidQueueDepth": queueDepthCombined}
	res = Database.sendMessage("usb_device_hid", "UPDATE_HID_QUEUE_DEPTH_COMBINED", args)



def instantiateComponent(usbDeviceHidComponent, index):
	global indexFunction
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global usbDeviceHidReportType
	global queueSizeRead
	global queueSizeWrite
	global usbDeviceHidBufPool
	global epNumberInterruptIn
	global epNumberInterruptOut
	
	res = Database.activateComponents(["usb_device"])
		
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
		MaxIntEpNumber = 7
		IntOutDefaultEpNumber = 1
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MX", "PIC32MK"]):
		MaxIntEpNumber = 15
		IntOutDefaultEpNumber = 1
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMD21", "SAMD51", "SAME51", "SAME53", "SAME54", "SAML21", "SAML22", "SAMD11"]):
		MaxIntEpNumber = 7
		IntOutDefaultEpNumber = 1
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2", "SAM9X60"]):
		MaxIntEpNumber = 15
		IntOutDefaultEpNumber = 2
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAME70", "SAMS70", "SAMV70", "SAMV71"]):
		MaxIntEpNumber = 9
		IntOutDefaultEpNumber = 2
	
	# Index of this function 
	indexFunction = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INDEX", None)
	indexFunction.setVisible(False)
	indexFunction.setMin(0)
	indexFunction.setMax(16)
	indexFunction.setDefaultValue(index)
	
	
	# Config name: Configuration number
	configValue = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_CONFIG_VALUE", None)
	configValue.setLabel("Configuration Value")
	configValue.setVisible(False)
	configValue.setMin(1)
	configValue.setMax(16)
	configValue.setDefaultValue(1)
	
	# Adding Start Interface number 
	startInterfaceNumber = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER", None)
	startInterfaceNumber.setLabel("Start Interface Number")
	startInterfaceNumber.setVisible(True)
	startInterfaceNumber.setMin(0)
	startInterfaceNumber.setDefaultValue(0)
	startInterfaceNumber.setReadOnly(True)
	
	# Adding Number of Interfaces
	numberOfInterfaces = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_NUMBER_OF_INTERFACES", None)
	numberOfInterfaces.setLabel("Number of Interfaces")
	numberOfInterfaces.setVisible(True)
	numberOfInterfaces.setMin(1)
	numberOfInterfaces.setMax(16)
	numberOfInterfaces.setDefaultValue(1)
	
	# USB HID Report Descriptor 
	usbDeviceHidReportType = usbDeviceHidComponent.createComboSymbol("CONFIG_USB_DEVICE_HID_REPORT_DESCRIPTOR_TYPE", None, usbDeviceHidReportList)
	usbDeviceHidReportType.setLabel("Select Report Type")
	usbDeviceHidReportType.setVisible(True)
	usbDeviceHidReportType.setDefaultValue("Mouse")
	
	# HID Function driver Report Receive Queue Size
	queueSizeRead = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", None)
	queueSizeRead.setLabel("HID Report Receive Queue Size")
	queueSizeRead.setVisible(True)
	queueSizeRead.setMin(1)
	queueSizeRead.setMax(32767)
	queueSizeRead.setDefaultValue(1)
	currentQSizeRead = queueSizeRead.getValue()

		
	# HID Function driver Report Send Queue Size 	
	queueSizeWrite = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE", None)
	queueSizeWrite.setLabel("HID Report Send Queue Size")
	queueSizeWrite.setVisible(True)
	queueSizeWrite.setMin(1)
	queueSizeWrite.setMax(32767)
	queueSizeWrite.setDefaultValue(1)
	currentQSizeWrite = queueSizeWrite.getValue()

	usbDeviceHidBufPool = usbDeviceHidComponent.createBooleanSymbol("CONFIG_USB_DEVICE_HID_BUFFER_POOL", None)
	usbDeviceHidBufPool.setLabel("**** Buffer Pool Update ****")
	usbDeviceHidBufPool.setDependencies(usbDeviceHidBufferQueueSize, ["CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"])
	usbDeviceHidBufPool.setVisible(False)
	
	
	# HID Function driver Interrupt IN Endpoint Number  
	epNumberInterruptIn = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INT_IN_ENDPOINT_NUMBER", None)
	epNumberInterruptIn.setLabel("Interrupt IN Endpoint Number")
	epNumberInterruptIn.setVisible(True)
	epNumberInterruptIn.setMin(1)
	epNumberInterruptIn.setMax(MaxIntEpNumber)
	
	# HID Function driver Interrupt OUT Endpoint Number  
	epNumberInterruptOut = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INT_OUT_ENDPOINT_NUMBER", None)
	epNumberInterruptOut.setLabel("Interrupt OUT Endpoint Number")
	epNumberInterruptOut.setVisible(True)
	epNumberInterruptOut.setMin(1)
	epNumberInterruptOut.setMax(MaxIntEpNumber)
	epNumberInterruptOut.setDefaultValue(IntOutDefaultEpNumber)
	
	############################################################################
	#### Dependency ####
	############################################################################
	# USB DEVICE HID Common Dependency
	
	numInstances  = Database.getSymbolValue("usb_device_hid", "CONFIG_USB_DEVICE_HID_INSTANCES")
	if (numInstances == None):
		numInstances = 0
		
	queueDepthCombined = Database.getSymbolValue("usb_device_hid", "CONFIG_USB_DEVICE_HID_QUEUE_DEPTH_COMBINED")
	if (queueDepthCombined == None):
		queueDepthCombined = 0
	#if numInstances < (index+1):
	args = {"hidInstanceCount": index+1}
	res = Database.sendMessage("usb_device_cdc", "UPDATE_HID_INSTANCES", args)
	
	args = {"hidQueueDepth": queueDepthCombined + currentQSizeRead + currentQSizeWrite}
	res = Database.sendMessage("usb_device_hid", "UPDATE_HID_QUEUE_DEPTH_COMBINED", args)
	
	#############################################################
	# Function Init Entry for HID
	#############################################################
	usbDeviceHidFunInitFile = usbDeviceHidComponent.createFileSymbol(None, None)
	usbDeviceHidFunInitFile.setType("STRING")
	usbDeviceHidFunInitFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
	usbDeviceHidFunInitFile.setSourcePath("templates/device/hid/system_init_c_device_data_hid_function_init.ftl")
	usbDeviceHidFunInitFile.setMarkup(True)
	
	#############################################################
	# Function Registration table for HID
	#############################################################
	usbDeviceHidFunRegTableFile = usbDeviceHidComponent.createFileSymbol(None, None)
	usbDeviceHidFunRegTableFile.setType("STRING")
	usbDeviceHidFunRegTableFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_ENTRY")
	usbDeviceHidFunRegTableFile.setSourcePath("templates/device/hid/system_init_c_device_data_hid_function.ftl")
	usbDeviceHidFunRegTableFile.setMarkup(True)
	
	#############################################################
	# HS Descriptors for HID Function 
	#############################################################
	usbDeviceHidDescriptorHsFile = usbDeviceHidComponent.createFileSymbol(None, None)
	usbDeviceHidDescriptorHsFile.setType("STRING")
	usbDeviceHidDescriptorHsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
	usbDeviceHidDescriptorHsFile.setSourcePath("templates/device/hid/system_init_c_device_data_hid_function_descrptr_hs.ftl")
	usbDeviceHidDescriptorHsFile.setMarkup(True)
	
	#############################################################
	# FS Descriptors for HID Function 
	#############################################################
	usbDeviceHidDescriptorFsFile = usbDeviceHidComponent.createFileSymbol(None, None)
	usbDeviceHidDescriptorFsFile.setType("STRING")
	usbDeviceHidDescriptorFsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
	usbDeviceHidDescriptorFsFile.setSourcePath("templates/device/hid/system_init_c_device_data_hid_function_descrptr_fs.ftl")
	usbDeviceHidDescriptorFsFile.setMarkup(True)
	
	#############################################################
	# Class code Entry for HID Function 
	#############################################################
	usbDeviceHidDescriptorClassCodeFile = usbDeviceHidComponent.createFileSymbol(None, None)
	usbDeviceHidDescriptorClassCodeFile.setType("STRING")
	usbDeviceHidDescriptorClassCodeFile.setOutputName("usb_device.LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")
	usbDeviceHidDescriptorClassCodeFile.setSourcePath("templates/device/hid/system_init_c_device_data_hid_function_class_codes.ftl")
	usbDeviceHidDescriptorClassCodeFile.setMarkup(True)
	
	################################################
	# USB HID Function driver Files 
	################################################
	usbDeviceHidHeaderFile = usbDeviceHidComponent.createFileSymbol(None, None)
	addFileName('usb_device_hid.h', usbDeviceHidComponent, usbDeviceHidHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHidHeaderFile = usbDeviceHidComponent.createFileSymbol(None, None)
	addFileName('usb_hid.h', usbDeviceHidComponent, usbHidHeaderFile, "middleware/", "/usb/", True, None)
	
	usbDeviceHidSourceFile = usbDeviceHidComponent.createFileSymbol(None, None)
	addFileName('usb_device_hid.c', usbDeviceHidComponent, usbDeviceHidSourceFile, "middleware/src/", "/usb/src", True, None)
	
	usbDeviceHidLocalHeaderFile = usbDeviceHidComponent.createFileSymbol(None, None)
	addFileName('usb_device_hid_local.h', usbDeviceHidComponent, usbDeviceHidLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
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