# Global definitions  
usbDebugLogs = 1 
 

# USB Device Global definitions 
listUsbDeviceClass =["CDC", "MSD", "HID", "VENDOR", "Audio V1", "Audio V2" ]
listUsbDeviceEp0BufferSize = ["64", "32", "16", "8"]
usbDeviceFunctionsNumberMax = 10
usbDeviceFunctionsNumberDefaultValue = 2 
usbDeviceFunctionsNumberValue = usbDeviceFunctionsNumberDefaultValue
usbDeviceFunctionIndex = 20
usbDeviceFunctionArray = []
usbDeviceClassArray = []
usbDeviceConfigValueArray = []
usbDeviceStartInterfaceNumberArray = []
usbDeviceNumberOfInterfacesArray = []

# USB Device CDC global definitions 
usbDeviceCdcQueueSizeRead = []
usbDeviceCdcQueueSizeWrite = []
usbDeviceCdcQueueSizeSerialStateNotification = []

# USB Device HID global definitions
usbDeviceHidQueueSizeReportSend = []
usbDeviceHidQueueSizeReportReceive = []
usbDeviceHidDeviceType = []	
	
def instantiateComponent(usbDeviceComponent):	
	
	# USB Device EP0 Buffer Size  
	usbDeviceEp0BufferSize = usbDeviceComponent.createComboSymbol("CONFIG_USB_DEVICE_EP0_BUFFER_SIZE", None, listUsbDeviceEp0BufferSize)
	usbDeviceEp0BufferSize.setLabel("Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setVisible(True)
	usbDeviceEp0BufferSize.setDescription("Select Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setDefaultValue("64")
	
	# USB Device number of functions 
	usbDeviceFunctionsNumber = usbDeviceComponent.createIntegerSymbol("USB_DEVICE_FUNCTIONS_NUMBER", None)
	usbDeviceFunctionsNumber.setLabel("Number of Functions")
	usbDeviceFunctionsNumber.setMax(usbDeviceFunctionsNumberMax)
	usbDeviceFunctionsNumber.setMin(1)
	usbDeviceFunctionsNumber.setDefaultValue(usbDeviceFunctionsNumberDefaultValue)
	usbDeviceFunctionsNumber.setVisible(True)
	usbDeviceFunctionsNumber.setDescription("Number of Function Drivers Registered to this instance")
	
	# USB Device Functions  
	for i in range (0, usbDeviceFunctionsNumberMax):
		# Adding Function driver config
		usbDeviceFunctionArray.append(usbDeviceComponent.createMenuSymbol("USB_DEVICE_FUNCTION_" + str(i), None))
		if (i < usbDeviceFunctionsNumberDefaultValue):
			usbDeviceFunctionArray[i].setVisible(True)
		else:
			usbDeviceFunctionArray[i].setVisible(False)
		usbDeviceFunctionArray[i].setLabel("Function " + str(i))
		usbDeviceFunctionArray[i].setDependencies(blusbDeviceFunctionArray, ["USB_OPERATION_MODE", "USB_DEVICE_FUNCTIONS_NUMBER"])
	
		# Adding Device Class config, Parent: Function driver 
		usbDeviceClassArray.append(usbDeviceComponent.createComboSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_DEVICE_CLASS", usbDeviceFunctionArray[i], listUsbDeviceClass))
		usbDeviceClassArray[i].setLabel("Device Class")
		usbDeviceClassArray[i].setDefaultValue("CDC")
		usbDeviceClassArray[i].setVisible(True)
		
		# Config name: Configuration number, Parent: Function driver 
		usbDeviceConfigValueArray.append(usbDeviceComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_CONFIGURATION", usbDeviceFunctionArray[i]))
		usbDeviceConfigValueArray[i].setLabel("Configuration Value")
		usbDeviceConfigValueArray[i].setVisible(True)
		usbDeviceConfigValueArray[i].setMin(1)
		usbDeviceConfigValueArray[i].setMax(16)
		usbDeviceConfigValueArray[i].setDefaultValue(1)
		
		#Adding Start Interface number, Parent: Function driver 
		usbDeviceStartInterfaceNumberArray.append(usbDeviceComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_INTERFACE_NUMBER", usbDeviceFunctionArray[i]))
		usbDeviceStartInterfaceNumberArray[i].setLabel("Start Interface Number")
		usbDeviceStartInterfaceNumberArray[i].setVisible(True)
		usbDeviceStartInterfaceNumberArray[i].setMin(0)
		usbDeviceStartInterfaceNumberArray[i].setDefaultValue(1)
			
		#Adding Number of Interfaces, Parent: Function driver 
		usbDeviceNumberOfInterfacesArray.append(usbDeviceComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_NUMBER_OF_INTERFACES", usbDeviceFunctionArray[i]))
		usbDeviceNumberOfInterfacesArray[i].setLabel("Number of Interfaces")
		usbDeviceNumberOfInterfacesArray[i].setVisible(False)
		usbDeviceNumberOfInterfacesArray[i].setMin(1)
		
		#CDC Function driver Read Queue Size  
		usbDeviceCdcQueueSizeRead.append(usbDeviceComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_CDC_READ_Q_SIZE", usbDeviceFunctionArray[i]))
		usbDeviceCdcQueueSizeRead[i].setLabel("CDC Read Queue Size")
		usbDeviceCdcQueueSizeRead[i].setVisible(True)
		usbDeviceCdcQueueSizeRead[i].setMin(1)
		usbDeviceCdcQueueSizeRead[i].setDependencies(blusbDeviceClassChanged, ["USB_DEVICE_FUNCTION_" + str(i) + "_DEVICE_CLASS"])
		
		#CDC Function driver Write Queue Size  
		usbDeviceCdcQueueSizeWrite.append(usbDeviceComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_CDC_WRITE_Q_SIZE", usbDeviceFunctionArray[i]))
		usbDeviceCdcQueueSizeWrite[i].setLabel("CDC Write Queue Size")
		usbDeviceCdcQueueSizeWrite[i].setVisible(True)
		usbDeviceCdcQueueSizeWrite[i].setMin(1)
		usbDeviceCdcQueueSizeWrite[i].setDependencies(blusbDeviceClassChanged, ["USB_DEVICE_FUNCTION_" + str(i) + "_DEVICE_CLASS"])
		
		#CDC Function driver Serial state notification Queue Size  
		usbDeviceCdcQueueSizeSerialStateNotification.append(usbDeviceComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_CDC_SERIAL_NOTIFIACATION_Q_SIZE", usbDeviceFunctionArray[i]))
		usbDeviceCdcQueueSizeSerialStateNotification[i].setLabel("CDC Serial Notification Queue Size")
		usbDeviceCdcQueueSizeSerialStateNotification[i].setVisible(True)
		usbDeviceCdcQueueSizeSerialStateNotification[i].setMin(1)
		usbDeviceCdcQueueSizeSerialStateNotification[i].setDependencies(blusbDeviceClassChanged, ["USB_DEVICE_FUNCTION_" + str(i) + "_DEVICE_CLASS"])

	#Endpoint Read Queue Size 
	usbDeviceEndpointReadQueueSize = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_ENDPOINT_QUEUE_SIZE_READ", None)
	usbDeviceEndpointReadQueueSize.setLabel("Endpoint Read Queue Size")	
	usbDeviceEndpointReadQueueSize.setVisible(False)	
	usbDeviceEndpointReadQueueSize.setDependencies(blusbDeviceClassChanged, [ ",".join(map(str,usbDeviceClassArray))])


	#Endpoint Write Queue Size
	usbDeviceEndpointWriteQueueSize = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_ENDPOINT_QUEUE_SIZE_WRITE", None)
	usbDeviceEndpointWriteQueueSize.setLabel("Endpoint Write Queue Size")	
	usbDeviceEndpointWriteQueueSize.setVisible(False)	
	usbDeviceEndpointWriteQueueSize.setDependencies(blusbDeviceClassChanged, [ ",".join(map(str,usbDeviceClassArray))])
	
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
	usbDeviceFeatureEnableAutioTimeRemoteWakeup.setVisible(True)	
	
	
	# USB Device CDC Instances Number 
	usbDeviceCDCInstancesNumber = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_CDC_INSTANCES_NUMBER", None)
	usbDeviceCDCInstancesNumber.setVisible(False)
	usbDeviceCDCInstancesNumber.setDefaultValue(0)
	
	configName = Variables.get("__CONFIGURATION_NAME")
	
	################################################
	# system_definitions.h file for USB Device stack    
	################################################
	usbDeviceSystemDefFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemDefFile.setType("STRING")
	usbDeviceSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceSystemDefFile.setSourcePath("templates/system_definitions.h.device_includes.ftl")
	usbDeviceSystemDefFile.setMarkup(True)
	
	usbDeviceSystemDefObjFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemDefObjFile.setType("STRING")
	usbDeviceSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
	usbDeviceSystemDefObjFile.setSourcePath("templates/system_definitions.h.device_objects.ftl")
	usbDeviceSystemDefObjFile.setMarkup(True)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceSystemConfigFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemConfigFile.setType("STRING")
	usbDeviceSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceSystemConfigFile.setSourcePath("templates/system_config.h.device.ftl")
	usbDeviceSystemConfigFile.setMarkup(True)
	
	################################################
	# system_init.c file for USB Device stack    
	################################################
	usbDeviceSystemInitDataFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemInitDataFile.setType("STRING")
	usbDeviceSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA")
	usbDeviceSystemInitDataFile.setSourcePath("templates/system_init_c_device_data.ftl")
	usbDeviceSystemInitDataFile.setMarkup(True)
	
	usbDeviceSystemInitCallsFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemInitCallsFile.setType("STRING")
	usbDeviceSystemInitCallsFile.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
	usbDeviceSystemInitCallsFile.setSourcePath("templates/system_init_c_device_calls.ftl")
	usbDeviceSystemInitCallsFile.setMarkup(True)
	
	
	################################################
	# system_tasks.c file for USB Device stack  
	################################################
	usbDeviceSystemTasksFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemTasksFile.setType("STRING")
	usbDeviceSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS")
	usbDeviceSystemTasksFile.setSourcePath("templates/system_tasks_c_device.ftl")
	usbDeviceSystemTasksFile.setMarkup(True)
	
	################################################
	# USB Device Layer Files 
	################################################
	usbDeviceHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device.h', usbDeviceComponent, usbDeviceHeaderFile, "", "/usb/", True, None)
	
	usbDeviceCommonHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_common.h', usbDeviceComponent, usbDeviceCommonHeaderFile, "", "/usb/", True, None)
	
	usbDeviceChapter9HeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_chapter_9.h', usbDeviceComponent, usbDeviceChapter9HeaderFile, "", "/usb/", True, None)
	
	usbDeviceLocalHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device_local.h', usbDeviceComponent, usbDeviceLocalHeaderFile, "src/", "/usb/src", True, None)
	
	usbDeviceMappingHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device_mapping.h', usbDeviceComponent, usbDeviceMappingHeaderFile, "src/", "/usb/src", True, None)
	
	usbDeviceFunctionDriverHeaderFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device_function_driver.h', usbDeviceComponent, usbDeviceFunctionDriverHeaderFile, "src/", "/usb/src", True, None)

	usbDeviceSourceFile = usbDeviceComponent.createFileSymbol(None, None)
	addFileName('usb_device.c', usbDeviceComponent, usbDeviceSourceFile, "src/dynamic/", "/usb/src/dynamic", True, None)
		
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
				
def blusbDeviceFunctionArray(usbSymbolSource, event):
	global usbDeviceFunctionsNumberValue
	global usbDeviceFunctionsNumberMax
	blUsbLog(usbSymbolSource, event)
	if (event["id"] == "USB_OPERATION_MODE"):
		if (event["value"] == "Device"):
			for i in range (0, usbDeviceFunctionsNumberMax):
				if (i < usbDeviceFunctionsNumberValue):
					blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
					usbDeviceFunctionArray[i].setVisible(True)
				else:
					blUsbPrint("Set Hide " + usbSymbolSource.getID().encode('ascii', 'ignore'))
					usbDeviceFunctionArray[i].setVisible(False)
		elif (event["value"] == "Host"):
			usbSymbolSource.setVisible(False)
	elif (event["id"] == "USB_DEVICE_FUNCTIONS_NUMBER"):
		usbDeviceFunctionsNumberValue = event["value"]
		for i in range (0, usbDeviceFunctionsNumberMax):
			if (i < usbDeviceFunctionsNumberValue):
				usbDeviceFunctionArray[i].setVisible(True)
			else:
				usbDeviceFunctionArray[i].setVisible(False)
	
def blusbDeviceClassChanged (usbSymbolSource, event):
	global usbDeviceFunctionIndex
	global usbComponent
	blUsbLog(usbSymbolSource, event)
	#print ("Entering blusbDeviceClassChanged")
	functionIndex = int(event["id"][usbDeviceFunctionIndex])
	print (functionIndex)
	if (event["value"] == "CDC"):
		#usbSymbolSource.setVisible(True)
		usbDeviceCdcQueueSizeRead[functionIndex].setVisible(True)
		usbDeviceCdcQueueSizeWrite[functionIndex].setVisible(True)
		usbDeviceCdcQueueSizeSerialStateNotification[functionIndex].setVisible(True)
	else: 
		#usbSymbolSource.setVisible(False)
		usbDeviceCdcQueueSizeRead[functionIndex].setVisible(False)
		usbDeviceCdcQueueSizeWrite[functionIndex].setVisible(False)
		usbDeviceCdcQueueSizeSerialStateNotification[functionIndex].setVisible(False)
	
