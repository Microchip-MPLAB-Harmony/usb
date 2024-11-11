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
printerInterfacesNumber = 1
printerDescriptorSize = 16
printerEndpointsPic32 = 1
printerEndpointsSAM = 2
indexFunction = None
configValue = None 
startInterfaceNumber = None 
numberOfInterfaces = None 
epNumberBulkOut = None
printerEndpointNumber = None


def onAttachmentConnected(source, target):
	global printerInterfacesNumber
	global printerDescriptorSize
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global epNumberBulkOut
	global printerEndpointsPic32
	global printerEndpointsSAM
	
	dependencyID = source["id"]
	ownerComponent = source["component"]
	
	# Read number of functions from USB Device Layer 
	nFunctions = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")

	if nFunctions != None: 
		#Log.writeDebugMessage ("USB Device Printer Function Driver: Attachment connected")
		
		# Update Number of Functions in USB Device, Increment the value by One. 
		args = {"nFunction":nFunctions + 1}
		res = Database.sendMessage("usb_device", "UPDATE_FUNCTIONS_NUMBER", args)
	
		configDescriptorSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if configDescriptorSize != None: 
			args = {"nFunction":  configDescriptorSize + printerDescriptorSize}
			res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
	
		# Update Total Interfaces number 
		nInterfaces = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if nInterfaces != None: 
			args = {"nFunction":  nInterfaces + printerInterfacesNumber}
			res = Database.sendMessage("usb_device", "UPDATE_INTERFACES_NUMBER", args)
			startInterfaceNumber.setValue(nInterfaces, 1)
			
	# Update Total Endpoints used 
		nEndpoints = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if nEndpoints != None:

			epNumberBulkOut.setValue(nEndpoints + 1, 1)
			if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MM", "PIC32MK", "SAMD21", "SAMDA1", "SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "SAMD11", "PIC32CM", "PIC32MZ1025W", "PIC32MZ2051W", "PIC32CX2051BZ6", "WBZ653", "WFI32E01", "WFI32E02", "WFI32E03", "PIC32CX", "PIC32CK"]):
				args = {"nFunction": nEndpoints + printerEndpointsPic32}
				res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
			else:
				args = {"nFunction": nEndpoints + printerEndpointsSAM}
				res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
			

def onAttachmentDisconnected(source, target):

	print ("Printer Function Driver: Detached")
	global printerInterfacesNumber
	global printerDescriptorSize
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global epNumberBulkOut
	global printerEndpointsPic32
	global printerEndpointsSAM
	dependencyID = source["id"]
	ownerComponent = source["component"]
	
	nFunctions = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
	if nFunctions != None: 
		nFunctions = nFunctions - 1
		args = {"nFunction": nFunctions}
		res = Database.sendMessage("usb_device", "UPDATE_FUNCTIONS_NUMBER", args)
	
	endpointNumber = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
	if endpointNumber != None:
		if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MM", "PIC32MK", "SAMD21", "SAMDA1", "SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "SAMD11", "PIC32CM", "PIC32MZ1025W", "PIC32MZ2051W", "PIC32CX2051BZ6", "WBZ653", "WFI32E01", "WFI32E02", "WFI32E03", "PIC32CX", "PIC32CK"]):
			args = {"nFunction": endpointNumber -  printerEndpointsPic32 }
			res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
		else:
			args = {"nFunction": endpointNumber -  printerEndpointsSAM }
			res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
	
	# Update Total Interfaces number
	interfaceNumber = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
	if interfaceNumber != None: 
		args = {"nFunction":   interfaceNumber - 2}
		res = Database.sendMessage("usb_device", "UPDATE_INTERFACES_NUMBER", args)

	# Update Total configuration descriptor size 	
	configDescriptorSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
	if configDescriptorSize != None: 
		args = {"nFunction": configDescriptorSize - printerDescriptorSize }
		res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		
	
def destroyComponent(component):
	print ("Printer Function Driver: Destroyed")
			
		
	
def usbDevicePrinterBufferQueueSize(usbSymbolSource, event):
	global currentQSizeRead
	global currentQSizeWrite
	queueDepthCombined = Database.getSymbolValue("usb_device_printer", "CONFIG_USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED")
	if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE"):
		queueDepthCombined = queueDepthCombined - currentQSizeRead + event["value"]
		currentQSizeRead = event["value"]
	if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"):
		queueDepthCombined = queueDepthCombined - currentQSizeWrite  + event["value"]
		currentQSizeWrite = event["value"]
	args = {"printerQueueDepth": queueDepthCombined}
	res = Database.sendMessage("usb_device_cdc", "UPDATE_PRINTER_QUEUE_DEPTH_COMBINED", args)
	
def instantiateComponent(usbDevicePrinterComponent, index):
	global printerDescriptorSize
	global printerInterfacesNumber
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global currentQSizeRead
	global currentQSizeWrite
	global epNumberBulkOut
	
	res = Database.activateComponents(["usb_device"])
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MX", "PIC32MK", "PIC32MM", "PIC32MZ1025W", "PIC32MZ2051W", "PIC32CX2051BZ6", "WBZ653", "WFI32E01", "WFI32E02", "WFI32E03"]):
		BulkOutMaxEpNumber = 15
	elif any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32CK"]):
		BulkOutMaxEpNumber = 7
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMD21", "SAMDA1","SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "SAMD11", "PIC32CM", "PIC32CX"]):
		BulkOutMaxEpNumber = 7
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAM9X60", "SAM9X7"]):
		BulkOutMaxEpNumber = 6
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2", "SAMA7"]):
		BulkOutMaxEpNumber = 9
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAME70", "SAMS70", "SAMV70", "SAMV71", "PIC32CZ"]):
		BulkOutMaxEpNumber = 9
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMG55"]):
		BulkOutMaxEpNumber = 5
	
	# Index of this function 
	indexFunction = usbDevicePrinterComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INDEX", None)
	indexFunction.setVisible(False)
	indexFunction.setMin(0)
	indexFunction.setMax(16)
	indexFunction.setDefaultValue(index)
	indexFunction.setReadOnly(True)

	# Config name: Configuration number
	configValue = usbDevicePrinterComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_CONFIG_VALUE", None)
	configValue.setLabel("Configuration Value")
	configValue.setVisible(False)
	configValue.setMin(1)
	configValue.setMax(16)
	configValue.setDefaultValue(1)
	configValue.setReadOnly(True)

	# Adding Start Interface number 
	startInterfaceNumber = usbDevicePrinterComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER", None)
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
	startInterfaceNumber.setHelp("printer_device_Library_Initialization")


	# Adding Number of Interfaces
	numberOfInterfaces = usbDevicePrinterComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_NUMBER_OF_INTERFACES", None)
	numberOfInterfaces.setLabel("Number of Interfaces")
        helpText = '''Indicates the interfaces in the Printer Device
        Interface Group.  This is provided here for indication purposes
        only.'''
        numberOfInterfaces.setDescription(helpText)
	numberOfInterfaces.setVisible(True)
	numberOfInterfaces.setMin(1)
	numberOfInterfaces.setMax(16)
	numberOfInterfaces.setDefaultValue(printerInterfacesNumber)
	numberOfInterfaces.setHelp("printer_device_Library_Initialization")

	
	# Printer Function driver Read Queue Size 
	queueSizeRead = usbDevicePrinterComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", None)
	queueSizeRead.setLabel("Printer Read Queue Size")
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
	queueSizeRead.setHelp("USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED")

	# Printer Function driver Write Queue Size 
	queueSizeWrite = usbDevicePrinterComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE", None)
	queueSizeWrite.setLabel("Printer Write Queue Size")
        helpText = '''Configure the size of the Write Queue. This configures
        the maximum number of Write Requests that can be queued before the
        Function Driver returns a queue full response. Using a queue increases
        memory consumption but also increases throughput. The driver will queue
        requests if the transfer request is currently being processed.'''
        queueSizeWrite.setDescription(helpText)
	queueSizeWrite.setVisible(True)
	queueSizeWrite.setMin(1)
	queueSizeWrite.setMax(32767)
	queueSizeWrite.setDefaultValue(1)	
	currentQSizeWrite = queueSizeWrite.getValue()
	queueSizeWrite.setHelp("USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED")

	# Printer Function driver Bulk OUT Endpoint Number   
	epNumberBulkOut = usbDevicePrinterComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_BULK_OUT_ENDPOINT_NUMBER", None)
	epNumberBulkOut.setLabel("Bulk OUT Endpoint Number")
        helpText = '''Specify the endpoint number of Bulk Out Endpoint to
        be used for this instance of the Printer Interface. Refer to Device
        Datasheet for details on available endpoints and limitations.'''
        epNumberBulkOut.setDescription(helpText)
	epNumberBulkOut.setVisible(True)
	epNumberBulkOut.setMin(1)
	epNumberBulkOut.setDefaultValue(1)
	epNumberBulkOut.setMax(BulkOutMaxEpNumber)	
	epNumberBulkOut.setHelp("mcc_configuration_device_printer")	

	usbDevicePrinterBufPool = usbDevicePrinterComponent.createBooleanSymbol("CONFIG_USB_DEVICE_PRINTER_BUFFER_POOL", None)
	usbDevicePrinterBufPool.setLabel("**** Buffer Pool Update ****")
	usbDevicePrinterBufPool.setDependencies(usbDevicePrinterBufferQueueSize, ["CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"])
	usbDevicePrinterBufPool.setVisible(False)
	
	
	############################################################################
	#### Dependency ####
	############################################################################
	# USB DEVICE Printer Common Dependency
	
	Log.writeDebugMessage ("Dependency Started")
	
	numInstances  = Database.getSymbolValue("usb_device_printer", "CONFIG_USB_DEVICE_PRINTER_INSTANCES")
	if (numInstances == None):
		numInstances = 0
		
	queueDepthCombined = Database.getSymbolValue("usb_device_printer", "CONFIG_USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED")
	if (queueDepthCombined == None):
		queueDepthCombined = 0

	args = {"printerInstanceCount": index+1}
	res = Database.sendMessage("usb_device_cdc", "UPDATE_PRINTER_INSTANCES", args)
	
	args = {"printerQueueDepth": queueDepthCombined + currentQSizeRead + currentQSizeWrite}
	res = Database.sendMessage("usb_device_printer", "UPDATE_PRINTER_QUEUE_DEPTH_COMBINED", args)
	
	#############################################################
	# Function Init Entry for Printer
	#############################################################
	usbDevicePrinterFunInitFile = usbDevicePrinterComponent.createFileSymbol(None, None)
	usbDevicePrinterFunInitFile.setType("STRING")
	usbDevicePrinterFunInitFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
	usbDevicePrinterFunInitFile.setSourcePath("templates/device/printer/system_init_c_device_data_printer_function_init.ftl")
	usbDevicePrinterFunInitFile.setMarkup(True)
	
	
	#############################################################
	# Function Registration table for Printer
	#############################################################
	usbDevicePrinterFunRegTableFile = usbDevicePrinterComponent.createFileSymbol(None, None)
	usbDevicePrinterFunRegTableFile.setType("STRING")
	usbDevicePrinterFunRegTableFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_ENTRY")
	usbDevicePrinterFunRegTableFile.setSourcePath("templates/device/printer/system_init_c_device_data_printer_function.ftl")
	usbDevicePrinterFunRegTableFile.setMarkup(True)
	
	#############################################################
	# HS Descriptors for Printer Function 
	#############################################################
	usbDevicePrinterDescriptorHsFile = usbDevicePrinterComponent.createFileSymbol(None, None)
	usbDevicePrinterDescriptorHsFile.setType("STRING")
	usbDevicePrinterDescriptorHsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
	usbDevicePrinterDescriptorHsFile.setSourcePath("templates/device/printer/system_init_c_device_data_printer_function_descrptr_hs.ftl")
	usbDevicePrinterDescriptorHsFile.setMarkup(True)
	
	#############################################################
	# FS Descriptors for Printer Function 
	#############################################################
	usbDevicePrinterDescriptorFsFile = usbDevicePrinterComponent.createFileSymbol(None, None)
	usbDevicePrinterDescriptorFsFile.setType("STRING")
	usbDevicePrinterDescriptorFsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
	usbDevicePrinterDescriptorFsFile.setSourcePath("templates/device/printer/system_init_c_device_data_printer_function_descrptr_fs.ftl")
	usbDevicePrinterDescriptorFsFile.setMarkup(True)
	
	
	#############################################################
	# Class code Entry for Printer Function 
	#############################################################
	usbDevicePrinterDescriptorClassCodeFile = usbDevicePrinterComponent.createFileSymbol(None, None)
	usbDevicePrinterDescriptorClassCodeFile.setType("STRING")
	usbDevicePrinterDescriptorClassCodeFile.setOutputName("usb_device.LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")
	usbDevicePrinterDescriptorClassCodeFile.setSourcePath("templates/device/printer/system_init_c_device_data_printer_function_class_codes.ftl")
	usbDevicePrinterDescriptorClassCodeFile.setMarkup(True)
	
	################################################
	# USB Printer Function driver Files 
	################################################
	usbDevicePrinterHeaderFile = usbDevicePrinterComponent.createFileSymbol(None, None)
	addFileName('usb_device_printer.h', usbDevicePrinterComponent, usbDevicePrinterHeaderFile, "middleware/", "/usb/", True, None)
	
	usbPrinterHeaderFile = usbDevicePrinterComponent.createFileSymbol(None, None)
	addFileName('usb_printer.h', usbDevicePrinterComponent, usbPrinterHeaderFile, "middleware/", "/usb/", True, None)
	
	usbDevicePrinterSourceFile = usbDevicePrinterComponent.createFileSymbol(None, None)
	addFileName('usb_device_printer.c', usbDevicePrinterComponent, usbDevicePrinterSourceFile, "middleware/src/", "/usb/src", True, None)
	
	usbDevicePrinterLocalHeaderFile = usbDevicePrinterComponent.createFileSymbol(None, None)
	addFileName('usb_device_printer_local.h', usbDevicePrinterComponent, usbDevicePrinterLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	
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
