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
epNumberInterrupt = None
epNumberBulkOut = None
epNumberBulkIn = None
cdcEndpointNumber = None

def handleMessage(messageID, args):
	global useIad
	if (messageID == "UPDATE_CDC_IAD_ENABLE"):
		useIad.setValue(args["iadEnable"])
	return args
	
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
	
	print ("CDC Function Driver: Attached")
	
	remoteComponent = target["component"]
	remoteComponentId = remoteComponent.getID()
	
	if (remoteComponentId == "usb_device"):
		dependencyID = source["id"]
		ownerComponent = source["component"]
		
		# Read number of functions from USB Device Layer 
		nFunctions = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")

		if nFunctions != None: 
			#Log.writeDebugMessage ("USB Device CDC Function Driver: Attachment connected")
			
			# Update Number of Functions in USB Device, Increment the value by One. 
			args = {"nFunction":nFunctions + 1}
			res = Database.sendMessage("usb_device", "UPDATE_FUNCTIONS_NUMBER", args)
		
			# If we have CDC function driver plus any function driver (no matter what Class), we enable IAD. 
			if nFunctions > 0:
				args = {"nFunction":True}
				res = Database.sendMessage("usb_device", "UPDATE_IAD_ENABLE", args)
				iadEnableSymbol = ownerComponent.getSymbolByID("CONFIG_USB_DEVICE_FUNCTION_USE_IAD")
				iadEnableSymbol.clearValue()
				iadEnableSymbol.setValue(True, 1)
			
				isIadEnabled = Database.getSymbolValue("usb_device_cdc_0", "CONFIG_USB_DEVICE_FUNCTION_USE_IAD")
				if isIadEnabled == False:
					args = {"iadEnable":True}
					res = Database.sendMessage("usb_device_cdc_0", "UPDATE_CDC_IAD_ENABLE", args)
				
				nCDCInstances = Database.getSymbolValue("usb_device_cdc", "CONFIG_USB_DEVICE_CDC_INSTANCES")
				if nCDCInstances == 2:
					configDescriptorSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
					if configDescriptorSize != None:
						args = {"nFunction": configDescriptorSize + 8}
						res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
			
			configDescriptorSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
			if configDescriptorSize != None: 
				iadEnableSymbol = ownerComponent.getSymbolByID("CONFIG_USB_DEVICE_FUNCTION_USE_IAD")
				if iadEnableSymbol.getValue() == True:
					descriptorSize =  cdcDescriptorSize + 8
				else:
					descriptorSize =  cdcDescriptorSize
				args = {"nFunction": configDescriptorSize + descriptorSize}
				res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		
			nInterfaces = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
			if nInterfaces != None: 
				args = {"nFunction":  nInterfaces + cdcInterfacesNumber}
				res = Database.sendMessage("usb_device", "UPDATE_INTERFACES_NUMBER", args)
				startInterfaceNumber.setValue(nInterfaces, 1)
				
			nEndpoints = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
			if nEndpoints != None:
				epNumberInterrupt.setValue(nEndpoints + 1, 1)
				epNumberBulkOut.setValue(nEndpoints + 2, 1)
				if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MM", "PIC32MK", "SAMD21", "SAMDA1","SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "SAMD11", "PIC32CM", "PIC32MZ1025W", "PIC32CX2051BZ6", "PIC32WM_BZ6204", "WBZ653", "PIC32MZ2051W", "WFI32E01", "WFI32E02", "WFI32E03", "PIC32CX", "PIC32CK"]):
					epNumberBulkIn.setValue(nEndpoints + 2, 1)
					args = {"nFunction":  nEndpoints + cdcEndpointsPic32}
					res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
				else:
					epNumberBulkIn.setValue(nEndpoints + 3, 1)
					args = {"nFunction":  nEndpoints + cdcEndpointsSAM}
					res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
	
	
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
	
	dependencyID = source["id"]
	ownerComponent = source["component"]
	
	remoteComponent = target["component"]
	remoteComponentId = remoteComponent.getID()
	
	if (remoteComponentId == "usb_device"):
	
		nFunctions = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		if nFunctions != None: 
			nFunctions = nFunctions - 1
			args = {"nFunction":nFunctions}
			res = Database.sendMessage("usb_device", "UPDATE_FUNCTIONS_NUMBER", args)
		
		endpointNumber = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if endpointNumber != None:
			if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MM", "PIC32MK", "SAMD21", "SAMDA1","SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "SAMD11", "PIC32CM", "PIC32MZ1025W", "PIC32CX2051BZ6", "PIC32WM_BZ6204", "WBZ653", "PIC32MZ2051W", "WFI32E01", "WFI32E02", "WFI32E03", "PIC32CX", "PIC32CK"]):
				args = {"nFunction":endpointNumber -  cdcEndpointsPic32 }
				res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
			else:
				args = {"nFunction":endpointNumber -  cdcEndpointsSAM }
				res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
		
		interfaceNumber = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if interfaceNumber != None: 
			args = {"nFunction":   interfaceNumber - 2}
			res = Database.sendMessage("usb_device", "UPDATE_INTERFACES_NUMBER", args)
			
		nCDCInstances = Database.getSymbolValue("usb_device_cdc", "CONFIG_USB_DEVICE_CDC_INSTANCES")
		if nCDCInstances != None:
			nCDCInstances = nCDCInstances - 1
			args = {"cdcInstanceCount": nCDCInstances}
			res = Database.sendMessage("usb_device_cdc", "UPDATE_CDC_INSTANCES", args)
			
			if nCDCInstances == 1 and nFunctions != None and nFunctions == 1:
				args = {"iadEnable":False}
				res = Database.sendMessage("usb_device_cdc_0", "UPDATE_CDC_IAD_ENABLE", args)
				args = {"nFunction":False}
				res = Database.sendMessage("usb_device", "UPDATE_IAD_ENABLE", args)
				configDescriptorSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
				if configDescriptorSize != None:
					args = {"nFunction": configDescriptorSize - 8}
					res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		
		configDescriptorSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if configDescriptorSize != None: 
			if useIad.getValue() == True:
				descriptorSize =  cdcDescriptorSize + 8
			else:
				descriptorSize =  cdcDescriptorSize
			args = {"nFunction": configDescriptorSize - descriptorSize}
			res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
	
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
	global currentQSizeRead
	global currentQSizeWrite
	global currentQSizeSerialStateNotification
	global epNumberInterrupt
	global epNumberBulkOut
	global epNumberBulkIn
	
	res = Database.activateComponents(["usb_device"])
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MX", "PIC32MK", "PIC32MM", "PIC32MZ1025W", "PIC32CX2051BZ6", "PIC32WM_BZ6204", "WBZ653", "PIC32MZ2051W", "WFI32E01", "WFI32E02", "WFI32E03"]):
		MaxEpNumber = 15
		BulkInDefaultEpNumber = 2
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		MaxEpNumber = 7
		BulkInDefaultEpNumber = 2
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMD21", "SAMDA1", "SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "SAMD11", "PIC32CM", "PIC32CX"]):
		MaxEpNumber = 7
		BulkInDefaultEpNumber = 2
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAM9X60", "SAM9X7"]):
		MaxEpNumber = 6
		BulkInDefaultEpNumber = 3	
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2", "SAMA7"]):
		MaxEpNumber = 15
		BulkInDefaultEpNumber = 3	
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAME70", "SAMS70", "SAMV70", "SAMV71", "PIC32CZ"]):
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
        helpText = '''Indicates the Interface Number of the first interfaces in
        the Communication Device Interface Group.  This is provided here for
        indication purposes only and is automatically updated based on the
        function driver selection.'''
        startInterfaceNumber.setDescription(helpText)
	startInterfaceNumber.setVisible(True)
	startInterfaceNumber.setMin(0)
	startInterfaceNumber.setDefaultValue(0)
	startInterfaceNumber.setReadOnly(True)
	startInterfaceNumber.setHelp("cdc_device_Library_Initialization")
	
	# Adding Number of Interfaces
	numberOfInterfaces = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_NUMBER_OF_INTERFACES", None)
	numberOfInterfaces.setLabel("Number of Interfaces")
        helpText = '''Indicates the interfaces in the Communication Device
        Interface Group.  This is provided here for indication purposes
        only.'''
        numberOfInterfaces.setDescription(helpText)
	numberOfInterfaces.setVisible(True)
	numberOfInterfaces.setMin(1)
	numberOfInterfaces.setMax(16)
	numberOfInterfaces.setDefaultValue(2)
	numberOfInterfaces.setReadOnly(True)
	numberOfInterfaces.setHelp("cdc_device_Library_Initialization")
	
	# Use IAD
	useIad = usbDeviceCdcComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FUNCTION_USE_IAD", None)
	useIad.setLabel("Use Interface Association Descriptor")
        helpText = '''Enable this option to generate a Interface Association
        Descriptor (IAD). This option should be enabled in case multiple CDC
        interfaces are included in the Device. Enabling the option will update
        the Class, Subclass fields in the Device Descriptor to indicate that
        that device uses IAD.'''
        useIad.setDescription(helpText)
	useIad.setVisible(True)
	useIad.setDefaultValue(False)
	useIad.setUseSingleDynamicValue(True)
	useIad.setHelp("mcc_configuration_device_cdc")
	
	# CDC Function driver Read Queue Size 
	queueSizeRead = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", None)
	queueSizeRead.setLabel("CDC Read Queue Size")
        helpText = '''Configure the size of the Read Queue. This configures the
        maximum number of Read Requests that can be queued before the Function
        Driver returns a queue full response. Using a queue increases memory
        consumption but also increases throughput. The driver will queue
        requests if the transfer request is currently being processed.'''
	queueSizeRead.setDescription(helpText)
        queueSizeRead.setVisible(True)
	queueSizeRead.setMin(1)
	queueSizeRead.setMax(32767)
	queueSizeRead.setDefaultValue(1)
	currentQSizeRead = queueSizeRead.getValue()
	queueSizeRead.setHelp("USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED")

	
	# CDC Function driver Write Queue Size 
	queueSizeWrite = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE", None)
        helpText = '''Configure the size of the Write Queue. This configures
        the maximum number of Write Requests that can be queued before the
        Function Driver returns a queue full response. Using a queue increases
        memory consumption but also increases throughput. The driver will queue
        requests if the transfer request is currently being processed.'''
        queueSizeWrite.setDescription(helpText)
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
        helpText = '''Configure the size of the Serial State Notification
        Queue. This configures the maximum number of Serial State Notification
        Requests that can be queued before the Function Driver returns a queue
        full response. Using a queue increases memory consumption but also
        increases throughput. The driver will queue requests if the transfer
        request is currently being processed.'''
        queueSizeSerialStateNotification.setDescription(helpText)
	queueSizeSerialStateNotification.setVisible(True)
	queueSizeSerialStateNotification.setMin(1)
	queueSizeSerialStateNotification.setMax(32767)
	queueSizeSerialStateNotification.setDefaultValue(1)	
	currentQSizeSerialStateNotification = queueSizeSerialStateNotification.getValue()
	queueSizeSerialStateNotification.setHelp("USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED")
	
	# CDC Function driver Notification Endpoint Number  
	epNumberInterrupt = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INT_ENDPOINT_NUMBER", None)
        helpText = '''Specify the endpoint number of Interrupt IN Endpoint to
        be used for this instance of the CDC Interface. Refer to Device
        Datasheet for details on available endpoints and limitations.'''
        epNumberInterrupt.setDescription(helpText)
        epNumberInterrupt.setLabel("Interrupt Endpoint Number")
	epNumberInterrupt.setVisible(True)
	epNumberInterrupt.setMin(1)
	epNumberInterrupt.setDefaultValue(1)
	epNumberInterrupt.setMax(MaxEpNumber)
	epNumberInterrupt.setHelp("mcc_configuration_device_cdc")
	

	# CDC Function driver Data OUT Endpoint Number   
	epNumberBulkOut = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_BULK_OUT_ENDPOINT_NUMBER", None)
        helpText = '''Specify the endpoint number of Bulk Out Endpoint to
        be used for this instance of the CDC Interface. Refer to Device
        Datasheet for details on available endpoints and limitations.'''
        epNumberBulkOut.setDescription(helpText)
	epNumberBulkOut.setLabel("Bulk OUT Endpoint Number")
	epNumberBulkOut.setVisible(True)
	epNumberBulkOut.setMin(1)
	epNumberBulkOut.setDefaultValue(2)
	epNumberBulkOut.setMax(MaxEpNumber)
	epNumberBulkOut.setHelp("mcc_configuration_device_cdc")
	
	# CDC Function driver Data IN Endpoint Number   
	epNumberBulkIn = usbDeviceCdcComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_BULK_IN_ENDPOINT_NUMBER", None)
        helpText = '''Specify the endpoint number of Bulk IN Endpoint to
        be used for this instance of the CDC Interface. Refer to Device
        Datasheet for details on available endpoints and limitations.'''
        epNumberBulkIn.setDescription(helpText)
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
	usbDeviceCdcFunInitFile = usbDeviceCdcComponent.createFileSymbol(None, None)
	usbDeviceCdcFunInitFile.setType("STRING")
	usbDeviceCdcFunInitFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
	usbDeviceCdcFunInitFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function_init.ftl")
	usbDeviceCdcFunInitFile.setMarkup(True)
	
	
	#############################################################
	# Function Registration table for CDC
	#############################################################
	usbDeviceCdcFunRegTableFile = usbDeviceCdcComponent.createFileSymbol(None, None)
	usbDeviceCdcFunRegTableFile.setType("STRING")
	usbDeviceCdcFunRegTableFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_ENTRY")
	usbDeviceCdcFunRegTableFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function.ftl")
	usbDeviceCdcFunRegTableFile.setMarkup(True)
	
	#############################################################
	# HS Descriptors for CDC Function 
	#############################################################
	usbDeviceCdcDescriptorHsFile = usbDeviceCdcComponent.createFileSymbol(None, None)
	usbDeviceCdcDescriptorHsFile.setType("STRING")
	usbDeviceCdcDescriptorHsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
	usbDeviceCdcDescriptorHsFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function_descrptr_hs.ftl")
	usbDeviceCdcDescriptorHsFile.setMarkup(True)
	
	#############################################################
	# FS Descriptors for CDC Function 
	#############################################################
	usbDeviceCdcDescriptorFsFile = usbDeviceCdcComponent.createFileSymbol(None, None)
	usbDeviceCdcDescriptorFsFile.setType("STRING")
	usbDeviceCdcDescriptorFsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
	usbDeviceCdcDescriptorFsFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function_descrptr_fs.ftl")
	usbDeviceCdcDescriptorFsFile.setMarkup(True)
	
	
	#############################################################
	# Class code Entry for CDC Function 
	#############################################################
	usbDeviceCdcDescriptorClassCodeFile = usbDeviceCdcComponent.createFileSymbol(None, None)
	usbDeviceCdcDescriptorClassCodeFile.setType("STRING")
	usbDeviceCdcDescriptorClassCodeFile.setOutputName("usb_device.LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")
	usbDeviceCdcDescriptorClassCodeFile.setSourcePath("templates/device/cdc/system_init_c_device_data_cdc_function_class_codes.ftl")
	usbDeviceCdcDescriptorClassCodeFile.setMarkup(True)
	
	################################################
	# USB CDC Function driver Files 
	################################################
	usbDeviceCdcHeaderFile = usbDeviceCdcComponent.createFileSymbol(None, None)
	addFileName('usb_device_cdc.h', usbDeviceCdcComponent, usbDeviceCdcHeaderFile, "middleware/", "/usb/", True, None)
	
	usbCdcHeaderFile = usbDeviceCdcComponent.createFileSymbol(None, None)
	addFileName('usb_cdc.h', usbDeviceCdcComponent, usbCdcHeaderFile, "middleware/", "/usb/", True, None)
	
	usbDeviceCdcSourceFile = usbDeviceCdcComponent.createFileSymbol(None, None)
	addFileName('usb_device_cdc.c', usbDeviceCdcComponent, usbDeviceCdcSourceFile, "middleware/src/", "/usb/src", True, None)
	
	usbDeviceCdcAcmSourceFile = usbDeviceCdcComponent.createFileSymbol(None, None)
	addFileName('usb_device_cdc_acm.c', usbDeviceCdcComponent, usbDeviceCdcAcmSourceFile, "middleware/src/", "/usb/src", True, None)
	
	usbDeviceCdcLocalHeaderFile = usbDeviceCdcComponent.createFileSymbol(None, None)
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
