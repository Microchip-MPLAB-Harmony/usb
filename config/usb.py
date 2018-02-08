# Global definitions  
listUsbSpeed = ["USB_SPEED_NORMAL", "USB_SPEED_LOW_POWER"]
listUsbOperationMode = ["Device", "Host", "Dual Role"]
listUsbDeviceClass =["CDC", "MSD", "HID", "VENDOR", "Audio V1", "Audio V2" ]
listUsbDeviceEp0BufferSize = ["64", "32", "16", "8"]
usbDeviceFunctionsNumberMax = 10
usbDeviceFunctionsNumberDefaultValue = 2 
usbDeviceFunctionsNumberValue = usbDeviceFunctionsNumberDefaultValue
usbDeviceFunctionArray = []
usbDeviceClassArray = []
usbDeviceConfigValueArray = []
usbDeviceStartInterfaceNumberArray = []
usbDeviceNumberOfInterfacesArray = []
usbDebugLogs = 1 
usbDriverPath = "framework/driver/"
usbMiddlewarePath = "framework/usb/" 
	
	
def instantiateComponent(usbComponent):	
	
	# USB Driver Speed selection 	
	usbSpeed = usbComponent.createComboSymbol("USB_SPEED", None, listUsbSpeed)
	usbSpeed.setLabel("USB Speed Selection")
	usbSpeed.setVisible(True)
	usbSpeed.setDescription("Select USB Operation Speed")
	usbSpeed.setDefaultValue("USB_SPEED_NORMAL")
	
	# USB Driver Operation mode selection 
	usbOpMode = usbComponent.createComboSymbol("USB_OPERATION_MODE", None, listUsbOperationMode)
	usbOpMode.setLabel("USB Mode Selection")
	usbOpMode.setVisible(True)
	usbOpMode.setDescription("Select USB Operation Mode")
	usbOpMode.setDefaultValue("Device")
	
	# USB Device EP0 Buffer Size  
	usbDeviceEp0BufferSize = usbComponent.createComboSymbol("USB_DEVICE_EP0_BUFFER_SIZE", usbOpMode, listUsbDeviceEp0BufferSize)
	usbDeviceEp0BufferSize.setLabel("Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setVisible(True)
	usbDeviceEp0BufferSize.setDescription("Select Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setDefaultValue("64")
	usbDeviceEp0BufferSize.setDependencies(blUsbEp0BufferSize, ["USB_OPERATION_MODE"])
	
	# USB Device number of functions 
	usbDeviceFunctionsNumber = usbComponent.createIntegerSymbol("USB_DEVICE_FUNCTIONS_NUMBER", usbOpMode)
	usbDeviceFunctionsNumber.setLabel("Number of Functions")
	usbDeviceFunctionsNumber.setMax(usbDeviceFunctionsNumberMax)
	usbDeviceFunctionsNumber.setMin(1)
	usbDeviceFunctionsNumber.setDefaultValue(usbDeviceFunctionsNumberDefaultValue)
	usbDeviceFunctionsNumber.setVisible(True)
	usbDeviceFunctionsNumber.setDescription("Number of Function Drivers Registered to this instance")
	usbDeviceFunctionsNumber.setDependencies(blUsbFunctionsNumber, ["USB_OPERATION_MODE"])
	
	# USB Device Functions  
	for i in range (0, usbDeviceFunctionsNumberMax):
		# Adding Function driver config
		usbDeviceFunctionArray.append(usbComponent.createMenuSymbol("USB_DEVICE_FUNCTION_" + str(i), usbOpMode))
		if (i < usbDeviceFunctionsNumberDefaultValue):
			usbDeviceFunctionArray[i].setVisible(True)
		else:
			usbDeviceFunctionArray[i].setVisible(False)
		usbDeviceFunctionArray[i].setLabel("Function " + str(i))
		usbDeviceFunctionArray[i].setDependencies(blusbDeviceFunctionArray, ["USB_OPERATION_MODE", "USB_DEVICE_FUNCTIONS_NUMBER"])
		
		# Adding Device Class config, Parent: Function driver 
		usbDeviceClassArray.append(usbComponent.createComboSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_DEVICE_CLASS", usbDeviceFunctionArray[i], listUsbDeviceClass))
		usbDeviceClassArray[i].setLabel("Device Class")
		usbDeviceClassArray[i].setDefaultValue("CDC")
		usbDeviceClassArray[i].setVisible(True)
		
		# Config name: Configuration number Parent: Function driver 
		usbDeviceConfigValueArray.append(usbComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_CONFIGURATION", usbDeviceFunctionArray[i]))
		usbDeviceConfigValueArray[i].setLabel("Configuration Value")
		usbDeviceConfigValueArray[i].setVisible(True)
		usbDeviceConfigValueArray[i].setMin(1)
		usbDeviceConfigValueArray[i].setMax(16)
		usbDeviceConfigValueArray[i].setDefaultValue(1)
		
		#Adding Start Interface number Parent: Function driver 
		usbDeviceStartInterfaceNumberArray.append(usbComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_INTERFACE_NUMBER", usbDeviceFunctionArray[i]))
		usbDeviceStartInterfaceNumberArray[i].setLabel("Start Interface Number")
		usbDeviceStartInterfaceNumberArray[i].setVisible(True)
		usbDeviceStartInterfaceNumberArray[i].setMin(0)
		usbDeviceStartInterfaceNumberArray[i].setDefaultValue(1)
		
		
		#Adding Number of Interfaces, Parent: Function driver 
		usbDeviceNumberOfInterfacesArray.append(usbComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_NUMBER_OF_INTERFACES", usbDeviceFunctionArray[i]))
		usbDeviceNumberOfInterfacesArray[i].setLabel("Number of Interfaces")
		usbDeviceNumberOfInterfacesArray[i].setVisible(False)
		usbDeviceNumberOfInterfacesArray[i].setMin(1)
		
	configName = Variables.get("__CONFIGURATION_NAME")

	usbSystemDefFile = usbComponent.createFileSymbol(None, None)
	usbSystemDefFile.setType("STRING")
	usbSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbSystemDefFile.setSourcePath("templates/system_definitions.h.includes.ftl")
	usbSystemDefFile.setMarkup(True)
	
	
	################################################
	# USB Driver Header file  
	################################################
	drvUsbHeaderFile = usbComponent.createFileSymbol(None, None)
	drvUsbHeaderFile.setSourcePath(usbDriverPath + "drv_usb.h")
	drvUsbHeaderFile.setOutputName("drv_usb.h")
	drvUsbHeaderFile.setDestPath("driver/usb/")
	drvUsbHeaderFile.setProjectPath("config/" + configName + "/driver/usb/")
	drvUsbHeaderFile.setType("HEADER")
	drvUsbHeaderFile.setOverwrite(True)
	
	drvUsbHsV1HeaderFile = usbComponent.createFileSymbol(None, None)
	drvUsbHsV1HeaderFile.setSourcePath(usbDriverPath + "usbhsv1/drv_usbhsv1.h")
	drvUsbHsV1HeaderFile.setOutputName("drv_usbv1.h")
	drvUsbHsV1HeaderFile.setDestPath("driver/usb/usbhsv1")
	drvUsbHsV1HeaderFile.setProjectPath("config/" + configName + "/driver/usb/usbhsv1")
	drvUsbHsV1HeaderFile.setType("HEADER")
	drvUsbHsV1HeaderFile.setOverwrite(True)
	
	drvUsbHsV1VarMapHeaderFile = usbComponent.createFileSymbol(None, None)
	drvUsbHsV1VarMapHeaderFile.setSourcePath(usbDriverPath + "usbhsv1/src/drv_usbhsv1_variant_mapping.h")
	drvUsbHsV1VarMapHeaderFile.setOutputName("drv_usbhsv1_variant_mapping.h")
	drvUsbHsV1VarMapHeaderFile.setDestPath("driver/usb/usbhsv1/src")
	drvUsbHsV1VarMapHeaderFile.setProjectPath("config/" + configName + "/driver/usb/usbhsv1/src")
	drvUsbHsV1VarMapHeaderFile.setType("HEADER")
	drvUsbHsV1VarMapHeaderFile.setOverwrite(True)

	
	drvUsbHsV1LocalHeaderFile = usbComponent.createFileSymbol(None, None)
	drvUsbHsV1LocalHeaderFile.setSourcePath(usbDriverPath + "usbhsv1/src/drv_usbhsv1_local.h")
	drvUsbHsV1LocalHeaderFile.setOutputName("drv_usbhsv1_local.h")
	drvUsbHsV1LocalHeaderFile.setDestPath("driver/usb/usbhsv1/src")
	drvUsbHsV1LocalHeaderFile.setProjectPath("config/" + configName + "/driver/usb/usbhsv1/src")
	drvUsbHsV1LocalHeaderFile.setType("HEADER")
	drvUsbHsV1LocalHeaderFile.setOverwrite(True)
	
	
	drvUsbHsV1SourceFile = usbComponent.createFileSymbol(None, None)
	drvUsbHsV1SourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1.c")
	drvUsbHsV1SourceFile.setOutputName("drv_usbhsv1.c")
	drvUsbHsV1SourceFile.setDestPath("driver/usb/usbhsv1/src/")
	drvUsbHsV1SourceFile.setProjectPath("config/" + configName + "/driver/usb/usbhsv1/src/")
	drvUsbHsV1SourceFile.setType("SOURCE")
	drvUsbHsV1SourceFile.setOverwrite(True)
	
	drvUsbHsV1DeviceSourceFile = usbComponent.createFileSymbol(None, None)
	drvUsbHsV1DeviceSourceFile.setSourcePath(usbDriverPath + "usbhsv1/src/dynamic/drv_usbhsv1_device.c")
	drvUsbHsV1DeviceSourceFile.setOutputName("drv_usbhsv1_device.c")
	drvUsbHsV1DeviceSourceFile.setDestPath("driver/usb/usbhsv1/src/")
	drvUsbHsV1DeviceSourceFile.setProjectPath("config/" + configName + "/driver/usb/usbhsv1/src/")
	drvUsbHsV1DeviceSourceFile.setType("SOURCE")
	drvUsbHsV1DeviceSourceFile.setOverwrite(True)
	
	################################################
	# USB Device Layer Files 
	################################################
	usbDeviceHeaderFile = usbComponent.createFileSymbol(None, None)
	usbDeviceHeaderFile.setSourcePath(usbMiddlewarePath + "usb_device.h")
	usbDeviceHeaderFile.setOutputName("usb_device.h")
	usbDeviceHeaderFile.setDestPath("usb/")
	usbDeviceHeaderFile.setProjectPath("config/" + configName + "/usb/")
	usbDeviceHeaderFile.setType("HEADER")
	usbDeviceHeaderFile.setOverwrite(True)
	
	usbDeviceCommonHeaderFile = usbComponent.createFileSymbol(None, None)
	usbDeviceCommonHeaderFile.setSourcePath(usbMiddlewarePath + "usb_common.h")
	usbDeviceCommonHeaderFile.setOutputName("usb_common.h")
	usbDeviceCommonHeaderFile.setDestPath("usb/")
	usbDeviceCommonHeaderFile.setProjectPath("config/" + configName + "/usb/")
	usbDeviceCommonHeaderFile.setType("HEADER")
	usbDeviceCommonHeaderFile.setOverwrite(True)
	
	usbDeviceChapter9HeaderFile = usbComponent.createFileSymbol(None, None)
	usbDeviceChapter9HeaderFile.setSourcePath(usbMiddlewarePath + "usb_chapter_9.h")
	usbDeviceChapter9HeaderFile.setOutputName("usb_chapter_9.h")
	usbDeviceChapter9HeaderFile.setDestPath("usb/")
	usbDeviceChapter9HeaderFile.setProjectPath("config/" + configName + "/usb/")
	usbDeviceChapter9HeaderFile.setType("HEADER")
	usbDeviceChapter9HeaderFile.setOverwrite(True)
	
	usbDeviceLocalHeaderFile = usbComponent.createFileSymbol(None, None)
	usbDeviceLocalHeaderFile.setSourcePath(usbMiddlewarePath + "src/usb_device_local.h")
	usbDeviceLocalHeaderFile.setOutputName("usb_device_local.h")
	usbDeviceLocalHeaderFile.setDestPath("usb/src")
	usbDeviceLocalHeaderFile.setProjectPath("config/" + configName + "/usb/src")
	usbDeviceLocalHeaderFile.setType("HEADER")
	usbDeviceLocalHeaderFile.setOverwrite(True)
	
	usbDeviceMappingHeaderFile = usbComponent.createFileSymbol(None, None)
	usbDeviceMappingHeaderFile.setSourcePath(usbMiddlewarePath + "src/usb_device_mapping.h")
	usbDeviceMappingHeaderFile.setOutputName("usb_device_mapping.h")
	usbDeviceMappingHeaderFile.setDestPath("usb/src")
	usbDeviceMappingHeaderFile.setProjectPath("config/" + configName + "/usb/src")
	usbDeviceMappingHeaderFile.setType("HEADER")
	usbDeviceMappingHeaderFile.setOverwrite(True)
	
	usbDeviceFunctionDriverHeaderFile = usbComponent.createFileSymbol(None, None)
	usbDeviceFunctionDriverHeaderFile.setSourcePath(usbMiddlewarePath + "src/usb_device_function_driver.h")
	usbDeviceFunctionDriverHeaderFile.setOutputName("usb_device_function_driver.h")
	usbDeviceFunctionDriverHeaderFile.setDestPath("usb/src")
	usbDeviceFunctionDriverHeaderFile.setProjectPath("config/" + configName + "/usb/src")
	usbDeviceFunctionDriverHeaderFile.setType("HEADER")
	usbDeviceFunctionDriverHeaderFile.setOverwrite(True)

	usbDeviceSourceFile = usbComponent.createFileSymbol(None, None)
	usbDeviceSourceFile.setSourcePath(usbMiddlewarePath + "src/dynamic/usb_device.c")
	usbDeviceSourceFile.setOutputName("usb_device.c")
	usbDeviceSourceFile.setDestPath("usb/src/dynamic")
	usbDeviceSourceFile.setProjectPath("config/" + configName + "usb/src/dynamic")
	usbDeviceSourceFile.setType("SOURCE")
	usbDeviceSourceFile.setOverwrite(True)
	
	################################################
	# CDC Function driver Files 
	################################################


def blUsbEp0BufferSize(usbSymbolSource, event):
	blUsbLog(usbSymbolSource, event)
	if (event["value"] == "Device"):		
		blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
		
def blUsbFunctionsNumber(usbSymbolSource, event):
	blUsbLog(usbSymbolSource, event)
	if (event["value"] == "Device"):
		usbSymbolSource.setVisible(True)
	else: 
		usbSymbolSource.setVisible(False)
	
		
def blusbDeviceFunctionArray(usbSymbolSource, event):
	global usbDeviceFunctionsNumberValue
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
				
def blUsbLog (usbSymbolSource, event):
	if (usbDebugLogs == 1):
			print("Source: " + usbSymbolSource.getID().encode('ascii', 'ignore'), "Trigger: " + event["id"].encode('ascii', 'ignore') , "Value: ", event["value"])
			
def blUsbPrint(string):
	if (usbDebugLogs == 1):
		print(string)