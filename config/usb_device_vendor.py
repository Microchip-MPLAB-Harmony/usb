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
	
	dependencyID = source["id"]
	ownerComponent = source["component"]
	
	# Read number of functions from USB Device Layer 
	nFunctions = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")

	if nFunctions != None: 
		#Log.writeDebugMessage ("USB Device Vendor Function Driver: Attachment connected")
		
		# Update Number of Functions in USB Device, Increment the value by One. 
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER", nFunctions + 1 , 2)
		
		configDescriptorSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if configDescriptorSize != None: 
			Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
			descriptorSize =  VENDOR_DESCRIPTOR_SIZE
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE", configDescriptorSize + descriptorSize , 2)
	
		nInterfaces = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if nInterfaces != None: 
			Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER", nInterfaces + VENDOR_INTERFACES_NUMBER , 2)
			startInterfaceNumber.setValue(nInterfaces, 1)
			
		nEndpoints = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if nEndpoints != None:
			if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
				Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
				Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER", nEndpoints + VENDOR_ENDPOINTS_PIC32 , 2)
				epNumberBulkOut.setValue(nEndpoints + 1, 1)
				epNumberBulkIn.setValue(nEndpoints + 1, 1)
			else:
				Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
				Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER", nEndpoints + VENDOR_ENDPOINTS_SAM , 2)
				epNumberBulkOut.setValue(nEndpoints + 1, 1)
				epNumberBulkIn.setValue(nEndpoints + 2, 1)
			
		readQSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_READ_QUEUE_SIZE")
		if readQSize!= None:
			Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_READ_QUEUE_SIZE")
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_READ_QUEUE_SIZE", readQSize + queueSizeRead.getValue(), 2)
		
		writeQSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_WRITE_QUEUE_SIZE")
		if writeQSize!= None:
			Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_WRITE_QUEUE_SIZE")
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_WRITE_QUEUE_SIZE", writeQSize + queueSizeWrite.getValue(), 2)

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
	
	dependencyID = source["id"]
	ownerComponent = source["component"]
	
	nFunctions = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
	if nFunctions != None: 
		nFunctions = nFunctions - 1
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER", nFunctions , 2)
	
	endpointNumber = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
	if endpointNumber != None:
		if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ"]):
			Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER", endpointNumber -  VENDOR_ENDPOINTS_PIC32 , 2)
		else:
			Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER", endpointNumber -  VENDOR_ENDPOINTS_SAM , 2)
	
	interfaceNumber = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
	if interfaceNumber != None: 
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER", interfaceNumber - 1, 2)
		
	nVendorInstances = Database.getSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_INSTANCES")
	if nVendorInstances != None:
		nVendorInstances = nVendorInstances - 1
		Database.clearSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_INSTANCES")
		Database.setSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_INSTANCES", nVendorInstances, 2)
	
	configDescriptorSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
	if configDescriptorSize != None: 
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		descriptorSize =  VENDOR_DESCRIPTOR_SIZE
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE", configDescriptorSize - descriptorSize , 2)
		
	
def destroyComponent(component):
	print ("Vendor Function Driver: Destroyed")
			
		
	
def usbDeviceVendorBufferQueueSize(usbSymbolSource, event):
	print("usbDeviceVendorBufferQueueSize" )
	global currentQSizeRead
	global currentQSizeWrite
	readQSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_READ_QUEUE_SIZE")
	if readQSize != None:
		if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE"):
			readQSize = readQSize - currentQSizeRead + event["value"]
			currentQSizeRead = event["value"]
			Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_READ_QUEUE_SIZE")
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_READ_QUEUE_SIZE", readQSize, 2)
	writeQSize = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_WRITE_QUEUE_SIZE")
	if writeQSize != None:
		if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"):
			writeQSize = writeQSize - currentQSizeWrite  + event["value"]
			currentQSizeWrite = event["value"]
			Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_WRITE_QUEUE_SIZE")
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINT_WRITE_QUEUE_SIZE", writeQSize, 2)
	Database.clearSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_QUEUE_DEPTH_COMBINED")
	Database.setSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_QUEUE_DEPTH_COMBINED", readQSize + writeQSize, 2)
	
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
	
	res = Database.activateComponents(["usb_device"])
	
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
	epNumberBulkOut.setMax(10)
	epNumberBulkOut.setDefaultValue(2)
	
	# Vendor Function driver Data IN Endpoint Number   
	epNumberBulkIn = usbDeviceVendorComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_BULK_IN_ENDPOINT_NUMBER", None)
	epNumberBulkIn.setLabel("Bulk IN Endpoint Number")
	epNumberBulkIn.setVisible(True)
	epNumberBulkIn.setMin(1)
	epNumberBulkIn.setMax(10)
	epNumberBulkIn.setDefaultValue(3)
	
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

	Database.clearSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_INSTANCES")
	Database.setSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_INSTANCES", (index+1), 2)
	Database.clearSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_QUEUE_DEPTH_COMBINED")
	Database.setSymbolValue("usb_device_vendor", "CONFIG_USB_DEVICE_VENDOR_QUEUE_DEPTH_COMBINED", queueDepthCombined + currentQSizeRead + currentQSizeWrite, 2)
	
	
	#############################################################
	# Function Init Entry for Vendor
	#############################################################
	usbDeviceVendorFunInitFile = usbDeviceVendorComponent.createFileSymbol(None, None)
	usbDeviceVendorFunInitFile.setType("STRING")
	usbDeviceVendorFunInitFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
	usbDeviceVendorFunInitFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function_init.ftl")
	usbDeviceVendorFunInitFile.setMarkup(True)
	
	
	#############################################################
	# Function Registration table for Vendor
	#############################################################
	usbDeviceVendorFunRegTableFile = usbDeviceVendorComponent.createFileSymbol(None, None)
	usbDeviceVendorFunRegTableFile.setType("STRING")
	usbDeviceVendorFunRegTableFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_ENTRY")
	usbDeviceVendorFunRegTableFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function.ftl")
	usbDeviceVendorFunRegTableFile.setMarkup(True)
	
	#############################################################
	# HS Descriptors for Vendor Function 
	#############################################################
	usbDeviceVendorDescriptorHsFile = usbDeviceVendorComponent.createFileSymbol(None, None)
	usbDeviceVendorDescriptorHsFile.setType("STRING")
	usbDeviceVendorDescriptorHsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
	usbDeviceVendorDescriptorHsFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function_descrptr_hs.ftl")
	usbDeviceVendorDescriptorHsFile.setMarkup(True)
	
	#############################################################
	# FS Descriptors for Vendor Function 
	#############################################################
	usbDeviceVendorDescriptorFsFile = usbDeviceVendorComponent.createFileSymbol(None, None)
	usbDeviceVendorDescriptorFsFile.setType("STRING")
	usbDeviceVendorDescriptorFsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
	usbDeviceVendorDescriptorFsFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function_descrptr_fs.ftl")
	usbDeviceVendorDescriptorFsFile.setMarkup(True)
	
	
	#############################################################
	# Class code Entry for Vendor Function 
	#############################################################
	usbDeviceVendorDescriptorClassCodeFile = usbDeviceVendorComponent.createFileSymbol(None, None)
	usbDeviceVendorDescriptorClassCodeFile.setType("STRING")
	usbDeviceVendorDescriptorClassCodeFile.setOutputName("usb_device.LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")
	usbDeviceVendorDescriptorClassCodeFile.setSourcePath("templates/device/vendor/system_init_c_device_data_vendor_function_class_codes.ftl")
	usbDeviceVendorDescriptorClassCodeFile.setMarkup(True)
	
	################################################
	# USB Vendor Function driver Files 
	################################################
	usbDeviceVendorSourceFile = usbDeviceVendorComponent.createFileSymbol(None, None)
	addFileName('usb_device_endpoint_functions.c', usbDeviceVendorComponent, usbDeviceVendorSourceFile, "src/", "/usb/src", True, None)
	

	
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
	if callback != None:
		symbol.setDependencies(callback, ["USB_DEVICE_FUNCTION_1_DEVICE_CLASS"])