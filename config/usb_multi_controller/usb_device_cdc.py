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
cdcFunctionsNumber = 1
currentQSizeRead  = 1
currentQSizeWrite = 1
currentQSizeSerialStateNotification = 1
cdcInterfacesNumber = 2
cdcDescriptorSize = 58
cdcEndpointsPic32 = 2
cdcEndpointsSAM = 3
indexFunction = None
configValue = None 
startInterfaceNumber = None 
numberOfInterfaces = None 
useIad = None
useIadPrev = None
epNumberInterrupt = None
epNumberBulkOut = None
epNumberBulkIn = None
cdcEndpointNumber = None
usbDeviceCdcDescriptorFsFile = None
usbDeviceCdcFunRegTableFile = None
usbDeviceCdcFunInitFile = None
usbDeviceCdcDescriptorHsFile = None
usbDeviceCdcDescriptorClassCodeFile = None
isConnectedToDeviceLayer = False
connectedDeviceLayerID = None 


def handleMessage(messageID, args):
	global useIad
	global connectedDeviceLayerID
	if (messageID == "UPDATE_CDC_IAD_ENABLE"):
		useIad.setValue(args["iadEnable"])
	return args

def blIadEnable(source, event):
	global isConnectedToDeviceLayer
	global connectedDeviceLayerID
	global useIadPrev

	if isConnectedToDeviceLayer == True: 
		if event["value"] == True and useIadPrev.getValue() == False:
			useIadPrev.setValue(True)
			args = {"nFunction":True}
			res = Database.sendMessage(connectedDeviceLayerID, "UPDATE_IAD_ENABLE", args)
			args = {"nFunction": 8}
			res = Database.sendMessage(connectedDeviceLayerID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		elif event["value"] == False and useIadPrev.getValue() == True:
			useIadPrev.setValue(False)
			args = {"nFunction":False}
			res = Database.sendMessage(connectedDeviceLayerID, "UPDATE_IAD_ENABLE", args)
			args = {"nFunction": 0 - 8}
			res = Database.sendMessage(connectedDeviceLayerID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)



def onAttachmentConnected(source, target):
	global cdcInterfacesNumber
	global cdcDescriptorSize
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global useIad
	global epNumberInterrupt
	global epNumberBulkOut
	global epNumberBulkIn
	global cdcEndpointsPic32
	global cdcEndpointsSAM
	global currentQSizeRead
	global currentQSizeWrite
	global currentQSizeSerialStateNotification
	global usbDeviceCdcDescriptorFsFile
	global usbDeviceCdcFunRegTableFile
	global usbDeviceCdcFunInitFile
	global usbDeviceCdcDescriptorHsFile
	global usbDeviceCdcDescriptorClassCodeFile
	global isConnectedToDeviceLayer
	global connectedDeviceLayerID
	global useIadPrev

	print ("CDC Function Driver: Attached")
	
	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	
	# Trim off the Instance index 
	remoteIDtrimmed = remoteID[:-1]
	
	if (remoteIDtrimmed == "usb_device_"):
		isConnectedToDeviceLayer = True
		useIadPrev.setValue(useIad.getValue())
		connectedDeviceLayerID = remoteID
		# Read number of functions from USB Device Layer 
		nFunctions = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		#usbDeviceCdcFunInitFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
		usbDeviceCdcFunRegTableFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_ENTRY")
		usbDeviceCdcFunInitFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
		usbDeviceCdcDescriptorHsFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
		usbDeviceCdcDescriptorFsFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
		usbDeviceCdcDescriptorClassCodeFile.setOutputName(remoteID + ".LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")

		if nFunctions != None: 
			# Update Number of Functions in USB Device, Increment the value by One. 
			args = {"nFunction":cdcFunctionsNumber}
			res = Database.sendMessage(remoteID, "UPDATE_FUNCTIONS_NUMBER", args)

			configDescriptorSize = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
			if configDescriptorSize != None: 
				iadEnableSymbol = ownerComponent.getSymbolByID("CONFIG_USB_DEVICE_FUNCTION_USE_IAD")
				iadSymbolValue = useIad.getValue()
				if (iadSymbolValue == True):
					descriptorSize =  cdcDescriptorSize + 8
				else:
					descriptorSize =  cdcDescriptorSize
				args = {"nFunction": descriptorSize}
				res = Database.sendMessage(remoteID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)
				args = {"nFunction": iadSymbolValue}
				res = Database.sendMessage(remoteID, "UPDATE_IAD_ENABLE", args)

			nInterfaces = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
			if nInterfaces != None: 
				args = {"nFunction":  cdcInterfacesNumber}
				res = Database.sendMessage(remoteID, "UPDATE_INTERFACES_NUMBER", args)
				nInterfaces = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
				startInterfaceNumber.setValue(nInterfaces - cdcInterfacesNumber, 1)
				
			nEndpoints = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
			if nEndpoints != None:
				args = {"nFunction":  cdcEndpointsPic32}
				res = Database.sendMessage(remoteID, "UPDATE_ENDPOINTS_NUMBER", args)
				nEndpoints = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
				epNumberInterrupt.setValue(nEndpoints - 1, 1)
				epNumberBulkOut.setValue(nEndpoints, 1)
				epNumberBulkIn.setValue(nEndpoints, 1)

		queueDepthCombined = Database.getSymbolValue("usb_device_cdc", "CONFIG_USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED")
		if (queueDepthCombined == None):
			queueDepthCombined = 0
		args = {"cdcQueueDepth": queueDepthCombined + currentQSizeRead + currentQSizeWrite + currentQSizeSerialStateNotification}
		res = Database.sendMessage("usb_device_cdc", "UPDATE_CDC_QUEUE_DEPTH_COMBINED", args)
	
	
def onAttachmentDisconnected(source, target):

	print ("CDC Function Driver: Detached")
	global cdcInterfacesNumber
	global cdcDescriptorSize
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global useIad
	global epNumberInterrupt
	global epNumberBulkOut
	global epNumberBulkIn
	global cdcEndpointsPic32
	global cdcEndpointsSAM
	global cdcInstancesCount
	global currentQSizeRead
	global currentQSizeWrite
	global currentQSizeSerialStateNotification
	global isConnectedToDeviceLayer
	global connectedDeviceLayerID
	
	dependencyID = source["id"]
	ownerComponent = source["component"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	remoteIDtrimmed = remoteID[:-1]
	
	# Trim off the Instance index 
	if (remoteIDtrimmed == "usb_device_"):

		isConnectedToDeviceLayer = False
		connectedDeviceLayerID = None
		nFunctions = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		if nFunctions != None: 
			args = {"nFunction": 0 - cdcFunctionsNumber}
			res = Database.sendMessage(remoteID, "UPDATE_FUNCTIONS_NUMBER", args)
		
		endpointNumber = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if endpointNumber != None:
			args = {"nFunction":0 -  cdcEndpointsPic32 }
			res = Database.sendMessage(remoteID, "UPDATE_ENDPOINTS_NUMBER", args)

		interfaceNumber = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if interfaceNumber != None: 
			args = {"nFunction":   0 - cdcInterfacesNumber}
			res = Database.sendMessage(remoteID, "UPDATE_INTERFACES_NUMBER", args)
			
		nCDCInstances = Database.getSymbolValue("usb_device_cdc", "CONFIG_USB_DEVICE_CDC_INSTANCES")
		if nCDCInstances != None:
			nCDCInstances = nCDCInstances - 1
			args = {"cdcInstanceCount": nCDCInstances}
			res = Database.sendMessage("usb_device_cdc", "UPDATE_CDC_INSTANCES", args)
			
			# As the component is destroyed update the Combined Queue Length 
			queueDepthCombined = Database.getSymbolValue("usb_device_cdc", "CONFIG_USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED")
			if (queueDepthCombined == None):
				queueDepthCombined = 0
			args = {"cdcQueueDepth": queueDepthCombined - (currentQSizeRead + currentQSizeWrite + currentQSizeSerialStateNotification)}
			res = Database.sendMessage("usb_device_cdc", "UPDATE_CDC_QUEUE_DEPTH_COMBINED", args)

		configDescriptorSize = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if configDescriptorSize != None: 
			if useIad.getValue() == True:
				descriptorSize =  cdcDescriptorSize + 8
				args = {"nFunction": False}
				res = Database.sendMessage(remoteID, "UPDATE_IAD_ENABLE", args)
			else:
				descriptorSize =  cdcDescriptorSize
			args = {"nFunction": 0 - descriptorSize}
			res = Database.sendMessage(remoteID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)
	
def destroyComponent(component):
	print ("CDC Function Driver: Destroyed")
			
		
# This function is called when user modifies the CDC Queue Size. 
def usbDeviceCdcBufferQueueSize(usbSymbolSource, event):
	global currentQSizeRead
	global currentQSizeWrite
	global currentQSizeSerialStateNotification
	queueDepthCombined = Database.getSymbolValue("usb_device_cdc", "CONFIG_USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED")
	if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE"):
		queueDepthCombined = queueDepthCombined - currentQSizeRead + event["value"]
		currentQSizeRead = event["value"]
	if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"):
		queueDepthCombined = queueDepthCombined - currentQSizeWrite  + event["value"]
		currentQSizeWrite = event["value"]
	if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE"):
		queueDepthCombined = queueDepthCombined - currentQSizeSerialStateNotification + event["value"]
		currentQSizeSerialStateNotification = event["value"]
	# We have updated queueDepthCombined variable with current combined queue length. 
	# Now send a message to USB_DEVICE_CDC_COMMON.PY to modify the Combined queue length. 
	args = {"cdcQueueDepth": queueDepthCombined}
	res = Database.sendMessage("usb_device_cdc", "UPDATE_CDC_QUEUE_DEPTH_COMBINED", args)
	
def instantiateComponent(usbDeviceCdcComponent, index):
	global cdcDescriptorSize
	global cdcInterfacesNumber
	global cdcDescriptorSize
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global useIad
	global useIadPrev
	global currentQSizeRead
	global currentQSizeWrite
	global currentQSizeSerialStateNotification
	global epNumberInterrupt
	global epNumberBulkOut
	global epNumberBulkIn
	global usbDeviceCdcFunRegTableFile
	global devInstance
	global usbDeviceCdcFunInitFile
	global usbDeviceCdcDescriptorHsFile
	global usbDeviceCdcDescriptorFsFile
	global usbDeviceCdcDescriptorClassCodeFile
	global isConnectedToDeviceLayer
	global connectedDeviceLayerID

	isConnectedToDeviceLayer = False
	connectedDeviceLayerID = None 

	value = Database.getComponentByID("usb_device")
	if (value == None):
		res = Database.activateComponents(["usb_device"])
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MX", "PIC32MK", "PIC32MM"]):
		MaxEpNumber = 15
		BulkInDefaultEpNumber = 2
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CZ", "PIC32CK", "SAMD21", "SAMDA1", "SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "SAMD11", "PIC32CM", "PIC32CX"]):
		MaxEpNumber = 7
		BulkInDefaultEpNumber = 2
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAM9X60", "SAM9X7"]):
		MaxEpNumber = 6
		BulkInDefaultEpNumber = 3	
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2", "SAMA7"]):
		MaxEpNumber = 15
		BulkInDefaultEpNumber = 3	
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAME70", "SAMS70", "SAMV70", "SAMV71"]):
		MaxEpNumber = 9
		BulkInDefaultEpNumber = 3	
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMG55"]):
		MaxEpNumber = 5
		BulkInDefaultEpNumber = 3
	
	# Index of this function 
	indexFunction = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INDEX", None)
	indexFunction.setVisible(False)
	indexFunction.setMin(0)
	indexFunction.setMax(16)
	indexFunction.setDefaultValue(index)
	
	# Config name: Configuration number
	configValue = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_CONFIG_VALUE", None)
	configValue.setLabel("Configuration Value")
	configValue.setVisible(False)
	configValue.setMin(1)
	configValue.setMax(16)
	configValue.setDefaultValue(1)
	configValue.setReadOnly(True)
	
	# Adding Start Interface number 
	startInterfaceNumber = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER", None)
	startInterfaceNumber.setLabel("Start Interface Number")
	startInterfaceNumber.setVisible(True)
	startInterfaceNumber.setMin(0)
	startInterfaceNumber.setDefaultValue(0)
	startInterfaceNumber.setReadOnly(False)
	startInterfaceNumber.setHelp("cdc_device_Library_Initialization")
	
	# Adding Number of Interfaces
	numberOfInterfaces = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_NUMBER_OF_INTERFACES", None)
	numberOfInterfaces.setLabel("Number of Interfaces")
	numberOfInterfaces.setVisible(True)
	numberOfInterfaces.setMin(1)
	numberOfInterfaces.setMax(16)
	numberOfInterfaces.setDefaultValue(2)
	numberOfInterfaces.setReadOnly(True)
	numberOfInterfaces.setHelp("cdc_device_Library_Initialization")
	
	
	# Use IAD
	useIad = usbDeviceCdcComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FUNCTION_USE_IAD", None)
	useIad.setLabel("Use Interface Association Descriptor")
	useIad.setVisible(True)
	useIad.setDefaultValue(False)
	useIad.setUseSingleDynamicValue(True)
	useIad.setDependencies(blIadEnable, ["CONFIG_USB_DEVICE_FUNCTION_USE_IAD"])
	useIad.setHelp("mcc_configuration_device_cdc")
    
	#To store the previous state of IAD Checkbox
	useIadPrev = usbDeviceCdcComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FUNCTION_USE_IAD_PREVIOUS_STATE", None)
	useIadPrev.setVisible(False)
	useIadPrev.setDefaultValue(False)

	# CDC Function driver Read Queue Size 
	queueSizeRead = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", None)
	queueSizeRead.setLabel("CDC Read Queue Size")
	queueSizeRead.setVisible(True)
	queueSizeRead.setMin(1)
	queueSizeRead.setMax(32767)
	queueSizeRead.setDefaultValue(1)
	currentQSizeRead = queueSizeRead.getValue()
	queueSizeRead.setHelp("USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED")

	
	# CDC Function driver Write Queue Size 
	queueSizeWrite = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE", None)
	queueSizeWrite.setLabel("CDC Write Queue Size")
	queueSizeWrite.setVisible(True)
	queueSizeWrite.setMin(1)
	queueSizeWrite.setMax(32767)
	queueSizeWrite.setDefaultValue(1)
	currentQSizeWrite = queueSizeWrite.getValue()
	queueSizeWrite.setHelp("USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED")
	
	
	# CDC Function driver Serial state notification Queue Size  
	queueSizeSerialStateNotification = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE", None)
	queueSizeSerialStateNotification.setLabel("CDC Serial Notification Queue Size")
	queueSizeSerialStateNotification.setVisible(True)
	queueSizeSerialStateNotification.setMin(1)
	queueSizeSerialStateNotification.setMax(32767)
	queueSizeSerialStateNotification.setDefaultValue(1)	
	currentQSizeSerialStateNotification = queueSizeSerialStateNotification.getValue()
	queueSizeSerialStateNotification.setHelp("USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED")
	
	# CDC Function driver Notification Endpoint Number  
	epNumberInterrupt = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INT_ENDPOINT_NUMBER", None)
	epNumberInterrupt.setLabel("Interrupt Endpoint Number")
	epNumberInterrupt.setVisible(True)
	epNumberInterrupt.setMin(1)
	epNumberInterrupt.setDefaultValue(1)
	epNumberInterrupt.setMax(MaxEpNumber)
	epNumberInterrupt.setHelp("mcc_configuration_device_cdc")
	

	# CDC Function driver Data OUT Endpoint Number   
	epNumberBulkOut = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_BULK_OUT_ENDPOINT_NUMBER", None)
	epNumberBulkOut.setLabel("Bulk OUT Endpoint Number")
	epNumberBulkOut.setVisible(True)
	epNumberBulkOut.setMin(1)
	epNumberBulkOut.setDefaultValue(2)
	epNumberBulkOut.setMax(MaxEpNumber)
	epNumberBulkOut.setHelp("mcc_configuration_device_cdc")
	
	# CDC Function driver Data IN Endpoint Number   
	epNumberBulkIn = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_BULK_IN_ENDPOINT_NUMBER", None)
	epNumberBulkIn.setLabel("Bulk IN Endpoint Number")
	epNumberBulkIn.setVisible(True)
	epNumberBulkIn.setMin(1)
	epNumberBulkIn.setMax(MaxEpNumber)
	epNumberBulkIn.setDefaultValue(BulkInDefaultEpNumber)
	epNumberBulkIn.setHelp("mcc_configuration_device_cdc")
	
	usbDeviceCdcBufPool = usbDeviceCdcComponent.createBooleanSymbol("CONFIG_USB_DEVICE_CDC_BUFFER_POOL", None)
	usbDeviceCdcBufPool.setLabel("**** Buffer Pool Update ****")
	usbDeviceCdcBufPool.setDependencies(usbDeviceCdcBufferQueueSize, ["CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE", "CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE"])
	usbDeviceCdcBufPool.setVisible(False)
	
	
	

	############################################################################
	#### Dependency ####
	############################################################################
	# USB DEVICE CDC Common Dependency
	
	Log.writeDebugMessage ("Dependency Started")
	
	numInstances  = Database.getSymbolValue("usb_device_cdc", "CONFIG_USB_DEVICE_CDC_INSTANCES")
	if (numInstances == None):
		numInstances = 0

	args = {"cdcInstanceCount": index+1}
	res = Database.sendMessage("usb_device_cdc", "UPDATE_CDC_INSTANCES", args)
	
	
	#############################################################
	# Function Init Entry for CDC
	#############################################################
	usbDeviceCdcFunInitFile = usbDeviceCdcComponent.createFileSymbol("USB_DEVICE_CDC_FUN_INIT_TABLE_FILE", None)
	usbDeviceCdcFunInitFile.setType("STRING")
	#usbDeviceCdcFunInitFile.setOutputName("usb_device_1.LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
	usbDeviceCdcFunInitFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function_init.ftl")
	usbDeviceCdcFunInitFile.setMarkup(True)
	
	
	#############################################################
	# Function Registration table for CDC
	#############################################################
	usbDeviceCdcFunRegTableFile = usbDeviceCdcComponent.createFileSymbol("USB_DEVICE_CDC_FUN_REG_TABLE_FILE", None)
	usbDeviceCdcFunRegTableFile.setType("STRING")
	#usbDeviceCdcFunRegTableFile.setOutputName("usb_device_1.LIST_USB_DEVICE_FUNCTION_ENTRY")
	usbDeviceCdcFunRegTableFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function.ftl")
	usbDeviceCdcFunRegTableFile.setMarkup(True)
	
	#############################################################
	# HS Descriptors for CDC Function 
	#############################################################
	usbDeviceCdcDescriptorHsFile = usbDeviceCdcComponent.createFileSymbol("USB_DEVICE_CDC_DESCRIPTOR_HS_FILE", None)
	usbDeviceCdcDescriptorHsFile.setType("STRING")
	#usbDeviceCdcDescriptorHsFile.setOutputName("usb_device_1.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
	usbDeviceCdcDescriptorHsFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function_descrptr_hs.ftl")
	usbDeviceCdcDescriptorHsFile.setMarkup(True)
	
	#############################################################
	# FS Descriptors for CDC Function 
	#############################################################
	usbDeviceCdcDescriptorFsFile = usbDeviceCdcComponent.createFileSymbol("USB_DEVICE_CDC_DESCRIPTOR_FS_FILE", None)
	usbDeviceCdcDescriptorFsFile.setType("STRING")
	#usbDeviceCdcDescriptorFsFile.setOutputName("usb_device_1.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
	usbDeviceCdcDescriptorFsFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function_descrptr_fs.ftl")
	usbDeviceCdcDescriptorFsFile.setMarkup(True)
	
	
	#############################################################
	# Class code Entry for CDC Function 
	#############################################################
	usbDeviceCdcDescriptorClassCodeFile = usbDeviceCdcComponent.createFileSymbol("USB_DEVICE_CDC_DESCRIPTOR_CLASS_CODE_FILE", None)
	usbDeviceCdcDescriptorClassCodeFile.setType("STRING")
	#usbDeviceCdcDescriptorClassCodeFile.setOutputName("usb_device_1.LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")
	usbDeviceCdcDescriptorClassCodeFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function_class_codes.ftl")
	usbDeviceCdcDescriptorClassCodeFile.setMarkup(True)
	
	################################################
	# USB CDC Function driver Files 
	################################################
	usbDeviceCdcHeaderFile = usbDeviceCdcComponent.createFileSymbol("USB_DEVICE_CDC_HEADER_FILE", None)
	addFileName('usb_device_cdc.h', usbDeviceCdcComponent, usbDeviceCdcHeaderFile, "middleware/", "/usb/", True, None)
	
	usbCdcHeaderFile = usbDeviceCdcComponent.createFileSymbol("USB_CDC_HEADER_FILE", None)
	addFileName('usb_cdc.h', usbDeviceCdcComponent, usbCdcHeaderFile, "middleware/", "/usb/", True, None)
	
	usbDeviceCdcSourceFile = usbDeviceCdcComponent.createFileSymbol("USB_DEVICE_CDC_SOURCE_FILE", None)
	addFileName('usb_device_cdc.c', usbDeviceCdcComponent, usbDeviceCdcSourceFile, "middleware/src/", "/usb/src", True, None)
	
	usbDeviceCdcAcmSourceFile = usbDeviceCdcComponent.createFileSymbol("USB_DEVICE_CDC_ACM_SOURCE_FILE", None)
	addFileName('usb_device_cdc_acm.c', usbDeviceCdcComponent, usbDeviceCdcAcmSourceFile, "middleware/src/", "/usb/src", True, None)
	
	usbDeviceCdcLocalHeaderFile = usbDeviceCdcComponent.createFileSymbol("USB_DEVICE_CDC_LOCAL_HEADER_FILE", None)
	addFileName('usb_device_cdc_local.h', usbDeviceCdcComponent, usbDeviceCdcLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	
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