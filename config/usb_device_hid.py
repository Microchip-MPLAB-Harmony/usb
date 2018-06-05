currentQSizeRead  = 1
currentQSizeWrite = 1
hidInterfacesNumber = 1
hidDescriptorSize = 32

def onDependentComponentAdded(ownerComponent, dependencyID, dependentComponent):
	if (dependencyID == "usb_device_dependency"):
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER", readValue + 1 , 2)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE", readValue + hidDescriptorSize , 2)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER", readValue + hidInterfacesNumber , 2)
		
def destroyComponent(component):
	# global countFunctionDrivers
	functionsNumber = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
	if (functionsNumber != None):
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER", functionsNumber -  1 , 2)
	
	
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
	Database.clearSymbolValue("usb_device_hid", "CONFIG_USB_DEVICE_HID_QUEUE_DEPTH_COMBINED")
	Database.setSymbolValue("usb_device_hid", "CONFIG_USB_DEVICE_HID_QUEUE_DEPTH_COMBINED", queueDepthCombined, 2)



def instantiateComponent(usbDeviceHidComponent, index):
	# Index of this function 
	indexFunction = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INDEX", None)
	indexFunction.setVisible(False)
	indexFunction.setMin(0)
	indexFunction.setMax(16)
	indexFunction.setDefaultValue(index)
	
	
	# Config name: Configuration number
	configValue = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_CONFIG_VALUE", None)
	configValue.setLabel("Configuration Value")
	configValue.setVisible(True)
	configValue.setMin(1)
	configValue.setMax(16)
	configValue.setDefaultValue(1)
	
	# Adding Start Interface number 
	startInterfaceNumber = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER", None)
	startInterfaceNumber.setLabel("Start Interface Number")
	startInterfaceNumber.setVisible(True)
	startInterfaceNumber.setMin(0)
	startInterfaceNumber.setDefaultValue(0)
	
	# Adding Number of Interfaces
	numberOfInterfaces = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_NUMBER_OF_INTERFACES", None)
	numberOfInterfaces.setLabel("Number of Interfaces")
	numberOfInterfaces.setVisible(True)
	numberOfInterfaces.setMin(1)
	numberOfInterfaces.setMax(16)
	numberOfInterfaces.setDefaultValue(1)
	
	
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
	epNumberInterrupt = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INT_IN_ENDPOINT_NUMBER", None)
	epNumberInterrupt.setLabel("Interrupt IN Endpoint Number")
	epNumberInterrupt.setVisible(True)
	epNumberInterrupt.setMin(1)
	epNumberInterrupt.setMax(10)
	epNumberInterrupt.setDefaultValue(1)
	
	# HID Function driver Interrupt OUT Endpoint Number  
	epNumberInterrupt = usbDeviceHidComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INT_OUT_ENDPOINT_NUMBER", None)
	epNumberInterrupt.setLabel("Interrupt OUT Endpoint Number")
	epNumberInterrupt.setVisible(True)
	epNumberInterrupt.setMin(1)
	epNumberInterrupt.setMax(10)
	epNumberInterrupt.setDefaultValue(2)
	
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
	Database.clearSymbolValue("usb_device_hid", "CONFIG_USB_DEVICE_HID_INSTANCES")
	Database.setSymbolValue("usb_device_hid", "CONFIG_USB_DEVICE_HID_INSTANCES", (index+1), 2)
	Database.clearSymbolValue("usb_device_hid", "CONFIG_USB_DEVICE_HID_QUEUE_DEPTH_COMBINED")
	if (Database.setSymbolValue("usb_device_hid", "CONFIG_USB_DEVICE_HID_QUEUE_DEPTH_COMBINED", queueDepthCombined + currentQSizeRead + currentQSizeWrite, 1) == True):
		print("queueDepthCombined:", queueDepthCombined + currentQSizeRead + currentQSizeWrite)
	else:
		print("queueDepthCombined was not set")
		
	##############################################################
	# system_definitions.h file for USB Device HID Function driver   
	##############################################################
	usbDeviceHidSystemDefFile = usbDeviceHidComponent.createFileSymbol(None, None)
	usbDeviceHidSystemDefFile.setType("STRING")
	usbDeviceHidSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceHidSystemDefFile.setSourcePath("templates/device/hid/system_definitions.h.device_hid_includes.ftl")
	usbDeviceHidSystemDefFile.setMarkup(True)
	
	
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
	addFileName('usb_device_hid.h', usbDeviceHidComponent, usbDeviceHidHeaderFile, "", "/usb/", True, None)
	
	usbHidHeaderFile = usbDeviceHidComponent.createFileSymbol(None, None)
	addFileName('usb_hid.h', usbDeviceHidComponent, usbHidHeaderFile, "", "/usb/", True, None)
	
	usbDeviceHidSourceFile = usbDeviceHidComponent.createFileSymbol(None, None)
	addFileName('usb_device_hid.c', usbDeviceHidComponent, usbDeviceHidSourceFile, "src/", "/usb/src", True, None)
	
	usbDeviceHidLocalHeaderFile = usbDeviceHidComponent.createFileSymbol(None, None)
	addFileName('usb_device_hid_local.h', usbDeviceHidComponent, usbDeviceHidLocalHeaderFile, "src/", "/usb/src", True, None)
	
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