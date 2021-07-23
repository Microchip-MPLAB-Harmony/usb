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
# Constants specific to USB Device Vendor 
VENDOR_INTERFACES_NUMBER = 1
VENDOR_DESCRIPTOR_SIZE = 23
VENDOR_ENDPOINTS_PIC32 = 1
VENDOR_ENDPOINTS_SAM = 2

# Variables 
currentQSizeRead  = 1
currentQSizeWrite = 1
indexFunction = None
configValue = None 
startInterfaceNumber = None 
numberOfInterfaces = None 
epNumberBulkOut = None
epNumberBulkIn = None
queueSizeRead = None
queueSizeWrite = None 
usbDeviceVendorDescriptorClassCodeFile = None
usbDeviceVendorDescriptorHsFile = None
usbDeviceVendorDescriptorFsFile = None
usbDeviceVendorFunRegTableFile = None
usbDeviceVendorFunInitFile = None

def onAttachmentConnected(source, target):
	global VENDOR_INTERFACES_NUMBER
	global VENDOR_DESCRIPTOR_SIZE
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global useIad
	global epNumberBulkOut
	global epNumberBulkIn
	global queueSizeRead
	global queueSizeWrite
	global VENDOR_ENDPOINTS_PIC32
	global VENDOR_ENDPOINTS_SAM
	global usbDeviceVendorDescriptorClassCodeFile
	global usbDeviceVendorDescriptorHsFile
	global usbDeviceVendorDescriptorFsFile
	global usbDeviceVendorFunRegTableFile
	global usbDeviceVendorFunInitFile
	
	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	
	# Read number of functions from USB Device Layer 
	nFunctions = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")

	if nFunctions != None: 
		#Log.writeDebugMessage ("USB Device Vendor Function Driver: Attachment connected")
		
		# Update Number of Functions in USB Device, Increment the value by One. 
		args = {"nFunction":nFunctions + 1}
		res = Database.sendMessage(remoteID, "UPDATE_FUNCTIONS_NUMBER", args)
		
		configDescriptorSize = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		descriptorSize =  VENDOR_DESCRIPTOR_SIZE
		if configDescriptorSize != None: 
			args = {"nFunction": configDescriptorSize + descriptorSize }
			res = Database.sendMessage(remoteID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)
	
		nInterfaces = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if nInterfaces != None: 
			args = {"nFunction": nInterfaces + VENDOR_INTERFACES_NUMBER }
			res = Database.sendMessage(remoteID, "UPDATE_INTERFACES_NUMBER", args)
			startInterfaceNumber.setValue(nInterfaces, 1)
			
		nEndpoints = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if nEndpoints != None:
			if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MM", "PIC32MK", "SAMD21", "SAMDA1", "SAMD51", "SAME51", "SAME53", "SAME54", "SAML21", "SAML22", "PIC32CM"]):
				args = {"nFunction":  nEndpoints + VENDOR_ENDPOINTS_PIC32 }
				res = Database.sendMessage(remoteID, "UPDATE_ENDPOINTS_NUMBER", args)
				epNumberBulkOut.setValue(nEndpoints + 1, 1)
				epNumberBulkIn.setValue(nEndpoints + 1, 1)
			else:
				args = {"nFunction":  nEndpoints + VENDOR_ENDPOINTS_SAM }
				res = Database.sendMessage(remoteID, "UPDATE_ENDPOINTS_NUMBER", args)
				epNumberBulkOut.setValue(nEndpoints + 1, 1)
				epNumberBulkIn.setValue(nEndpoints + 2, 1)
			
		readQSize = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINT_READ_QUEUE_SIZE")
		if readQSize!= None:
			args = {"nFunction":  readQSize + queueSizeRead.getValue()}
			res = Database.sendMessage(remoteID, "UPDATE_ENDPOINT_READ_QUEUE_SIZE", args)
		
		writeQSize = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINT_WRITE_QUEUE_SIZE")
		if writeQSize!= None:
			args = {"nFunction":  writeQSize + queueSizeWrite.getValue()}
			res = Database.sendMessage(remoteID, "UPDATE_ENDPOINT_WRITE_QUEUE_SIZE", args)
		
		usbDeviceVendorDescriptorClassCodeFile.setOutputName(remoteID + ".LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")
		usbDeviceVendorDescriptorHsFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
		usbDeviceVendorDescriptorFsFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
		usbDeviceVendorFunRegTableFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_ENTRY")
		usbDeviceVendorFunInitFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")


def onAttachmentDisconnected(source, target):

	print ("Vendor Function Driver: Detached")
	global VENDOR_INTERFACES_NUMBER
	global VENDOR_DESCRIPTOR_SIZE
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global useIad
	global epNumberInterrupt
	global epNumberBulkOut
	global epNumberBulkIn
	global queueSizeRead
	global queueSizeWrite
	global VENDOR_ENDPOINTS_PIC32
	global VENDOR_ENDPOINTS_SAM
	global usbDeviceVendorDescriptorClassCodeFile
	global usbDeviceVendorDescriptorHsFile
	global usbDeviceVendorDescriptorFsFile
	global usbDeviceVendorFunRegTableFile
	global usbDeviceVendorFunInitFile
	
	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	
	nFunctions = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
	if nFunctions != None: 
		nFunctions = nFunctions - 1
		args = {"nFunction":nFunctions}
		res = Database.sendMessage(remoteID, "UPDATE_FUNCTIONS_NUMBER", args)
	
	endpointNumber = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
	if endpointNumber != None:
		if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MM", "PIC32MK", "SAMD21", "SAMDA1", "SAMD51", "SAME51", "SAME53", "SAME54", "SAML21", "SAML22" , "PIC32CM"]):
			args = {"nFunction":endpointNumber -  VENDOR_ENDPOINTS_PIC32 }
			res = Database.sendMessage(remoteID, "UPDATE_ENDPOINTS_NUMBER", args)
		else:
			args = {"nFunction":endpointNumber -  VENDOR_ENDPOINTS_SAM }
			res = Database.sendMessage(remoteID, "UPDATE_ENDPOINTS_NUMBER", args)
	
	
	interfaceNumber = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
	if interfaceNumber != None:
		args = {"nFunction":  interfaceNumber - 1}
		res = Database.sendMessage(remoteID, "UPDATE_INTERFACES_NUMBER", args)
			
	nVendorInstances = Database.getSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_INSTANCES")
	if nVendorInstances != None:
		nVendorInstances = nVendorInstances - 1
		args = {"vendorInstanceCount": nVendorInstances}
		res = Database.sendMessage("usb_device_vendor", "UPDATE_VENDOR_INSTANCES", args)
	
	configDescriptorSize = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
	if configDescriptorSize != None: 
		descriptorSize =  VENDOR_DESCRIPTOR_SIZE
		args = {"nFunction": configDescriptorSize - descriptorSize}
		res = Database.sendMessage(remoteID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		
		
	
def destroyComponent(component):
	print ("Vendor Function: Destroyed")
			
		
	
def usbDeviceVendorBufferQueueSize(usbSymbolSource, event):
	global currentQSizeRead
	global currentQSizeWrite
	readQSize = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINT_READ_QUEUE_SIZE")
	if readQSize != None:
		if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE"):
			readQSize = readQSize - currentQSizeRead + event["value"]
			currentQSizeRead = event["value"]
			args = {"nFunction": readQSize}
			res = Database.sendMessage(remoteID, "UPDATE_ENDPOINT_READ_QUEUE_SIZE", args)
	writeQSize = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINT_WRITE_QUEUE_SIZE")
	if writeQSize != None:
		if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"):
			writeQSize = writeQSize - currentQSizeWrite  + event["value"]
			args = {"nFunction": writeQSize}
			res = Database.sendMessage(remoteID, "UPDATE_ENDPOINT_WRITE_QUEUE_SIZE", args)
	args = {"vendorQueueDepth": readQSize + writeQSize}
	res = Database.sendMessage("usb_device_vendor", "UPDATE_VENDOR_QUEUE_DEPTH_COMBINED", args)
	
def instantiateComponent(usbDeviceVendorComponent, index):
	global VENDOR_DESCRIPTOR_SIZE
	global VENDOR_INTERFACES_NUMBER
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global useIad
	global currentQSizeRead
	global currentQSizeWrite
	global epNumberBulkOut
	global epNumberBulkIn
	global queueSizeRead
	global queueSizeWrite
	global usbDeviceVendorDescriptorClassCodeFile
	global usbDeviceVendorDescriptorHsFile
	global usbDeviceVendorDescriptorFsFile
	global usbDeviceVendorFunRegTableFile
	global usbDeviceVendorFunInitFile
	
	res = Database.activateComponents(["usb_device"])
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
		MaxEpNumber = 7
		BulkInDefaultEpNumber = 1
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MX", "PIC32MK", "PIC32MM"]):
		MaxEpNumber = 15
		BulkInDefaultEpNumber = 1
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMD21", "SAMDA1","SAMD51", "SAME51", "SAME53", "SAME54", "SAML21", "SAML22", "PIC32CM"]):
		MaxEpNumber = 7
		BulkInDefaultEpNumber = 1
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2", "SAME70", "SAMS70", "SAMV70", "SAMV71", "SAMA7"]):
		MaxEpNumber = 9
		BulkInDefaultEpNumber = 2
	
	# Index of this function 
	indexFunction = usbDeviceVendorComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INDEX", None)
	indexFunction.setVisible(False)
	indexFunction.setMin(0)
	indexFunction.setMax(16)
	indexFunction.setDefaultValue(index)
	
	# Config name: Configuration number
	configValue = usbDeviceVendorComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_CONFIG_VALUE", None)
	configValue.setLabel("Configuration Value")
	configValue.setVisible(False)
	configValue.setMin(1)
	configValue.setMax(16)
	configValue.setDefaultValue(1)
	configValue.setReadOnly(True)
	
	# Adding Start Interface number 
	startInterfaceNumber = usbDeviceVendorComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER", None)
	startInterfaceNumber.setLabel("Start Interface Number")
	startInterfaceNumber.setVisible(True)
	startInterfaceNumber.setMin(0)
	startInterfaceNumber.setDefaultValue(0)
	startInterfaceNumber.setReadOnly(True)
	
	
	# Adding Number of Interfaces
	numberOfInterfaces = usbDeviceVendorComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_NUMBER_OF_INTERFACES", None)
	numberOfInterfaces.setLabel("Number of Interfaces")
	numberOfInterfaces.setVisible(True)
	numberOfInterfaces.setMin(1)
	numberOfInterfaces.setMax(16)
	numberOfInterfaces.setDefaultValue(1)
	numberOfInterfaces.setReadOnly(True)
	
	# Vendor Function driver Read Queue Size 
	queueSizeRead = usbDeviceVendorComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", None)
	queueSizeRead.setLabel("Vendor Read Queue Size")
	queueSizeRead.setVisible(True)
	queueSizeRead.setMin(1)
	queueSizeRead.setMax(32767)
	queueSizeRead.setDefaultValue(1)
	currentQSizeRead = queueSizeRead.getValue()

	
	# Vendor Function driver Write Queue Size 
	queueSizeWrite = usbDeviceVendorComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE", None)
	queueSizeWrite.setLabel("Vendor Write Queue Size")
	queueSizeWrite.setVisible(True)
	queueSizeWrite.setMin(1)
	queueSizeWrite.setMax(32767)
	queueSizeWrite.setDefaultValue(1)	
	currentQSizeWrite = queueSizeWrite.getValue()
	

	# Vendor Function driver Data OUT Endpoint Number   
	epNumberBulkOut = usbDeviceVendorComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_BULK_OUT_ENDPOINT_NUMBER", None)
	epNumberBulkOut.setLabel("Bulk OUT Endpoint Number")
	epNumberBulkOut.setVisible(True)
	epNumberBulkOut.setMin(1)
	epNumberBulkOut.setMax(MaxEpNumber)
	epNumberBulkOut.setDefaultValue(1)
	
	# Vendor Function driver Data IN Endpoint Number   
	epNumberBulkIn = usbDeviceVendorComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_BULK_IN_ENDPOINT_NUMBER", None)
	epNumberBulkIn.setLabel("Bulk IN Endpoint Number")
	epNumberBulkIn.setVisible(True)
	epNumberBulkIn.setMin(1)
	epNumberBulkIn.setMax(MaxEpNumber)
	epNumberBulkIn.setDefaultValue(BulkInDefaultEpNumber)
	
	usbDeviceVendorBufPool = usbDeviceVendorComponent.createBooleanSymbol("CONFIG_USB_DEVICE_VENDOR_BUFFER_POOL", None)
	usbDeviceVendorBufPool.setLabel("**** Buffer Pool Update ****")
	usbDeviceVendorBufPool.setDependencies(usbDeviceVendorBufferQueueSize, ["CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"])
	usbDeviceVendorBufPool.setVisible(False)
	
	
	

	############################################################################
	#### Dependency ####
	############################################################################
	# USB DEVICE Vendor Common Dependency
	
	Log.writeDebugMessage ("Dependency Started")
	
	numInstances  = Database.getSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_INSTANCES")
	if (numInstances == None):
		numInstances = 0
		
	queueDepthCombined = Database.getSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_QUEUE_DEPTH_COMBINED")
	if (queueDepthCombined == None):
		queueDepthCombined = 0

	args = {"vendorInstanceCount": index+1}
	res = Database.sendMessage("usb_device_vendor", "UPDATE_VENDOR_INSTANCES", args)
	
	args = {"vendorQueueDepth": queueDepthCombined + currentQSizeRead + currentQSizeWrite }
	res = Database.sendMessage("usb_device_vendor", "UPDATE_VENDOR_QUEUE_DEPTH_COMBINED", args)
	
	
	#############################################################
	# Function Init Entry for Vendor
	#############################################################
	usbDeviceVendorFunInitFile = usbDeviceVendorComponent.createFileSymbol("USB_DEVICE_VENDOR_FUN_INIT_FILE", None)
	usbDeviceVendorFunInitFile.setType("STRING")
	usbDeviceVendorFunInitFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function_init.ftl")
	usbDeviceVendorFunInitFile.setMarkup(True)
	
	
	#############################################################
	# Function Registration table for Vendor
	#############################################################
	usbDeviceVendorFunRegTableFile = usbDeviceVendorComponent.createFileSymbol("USB_DEVICE_VENDOR_FUN_REG_TABLE_FILE", None)
	usbDeviceVendorFunRegTableFile.setType("STRING")
	usbDeviceVendorFunRegTableFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function.ftl")
	usbDeviceVendorFunRegTableFile.setMarkup(True)
	
	#############################################################
	# HS Descriptors for Vendor Function 
	#############################################################
	usbDeviceVendorDescriptorHsFile = usbDeviceVendorComponent.createFileSymbol("USB_DEVICE_VENDOR_DESCRIPTOR_HS_FILE", None)
	usbDeviceVendorDescriptorHsFile.setType("STRING")
	usbDeviceVendorDescriptorHsFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function_descrptr_hs.ftl")
	usbDeviceVendorDescriptorHsFile.setMarkup(True)
	
	#############################################################
	# FS Descriptors for Vendor Function 
	#############################################################
	usbDeviceVendorDescriptorFsFile = usbDeviceVendorComponent.createFileSymbol("USB_DEVICE_VENDOR_DESCRIPTOR_FS_FILE", None)
	usbDeviceVendorDescriptorFsFile.setType("STRING")
	usbDeviceVendorDescriptorFsFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function_descrptr_fs.ftl")
	usbDeviceVendorDescriptorFsFile.setMarkup(True)
	
	
	#############################################################
	# Class code Entry for Vendor Function 
	#############################################################
	usbDeviceVendorDescriptorClassCodeFile = usbDeviceVendorComponent.createFileSymbol("USB_DEVICE_VENDOR_DESCRIPTOR_CLASS_CODE_FILE", None)
	usbDeviceVendorDescriptorClassCodeFile.setType("STRING")
	usbDeviceVendorDescriptorClassCodeFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function_class_codes.ftl")
	usbDeviceVendorDescriptorClassCodeFile.setMarkup(True)
	
	################################################
	# USB Vendor Function driver Files 
	################################################
	usbDeviceVendorSourceFile = usbDeviceVendorComponent.createFileSymbol("USB_DEVICE_VENDOR_SOURCE_FILE", None)
	addFileName('usb_device_endpoint_functions.c', usbDeviceVendorComponent, usbDeviceVendorSourceFile, "middleware/src/", "/usb/src", True, None)
	

	
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