# Global definitions  
listUsbSpeed = ["USB_SPEED_NORMAL", "USB_SPEED_LOW_POWER"]
listUsbOperationMode = ["Device", "Host", "Dual Role"]
listUsbDeviceClass =["CDC", "MSD", "HID", "VENDOR", "Audio V1", "Audio V2" ]
listUsbDeviceEp0BufferSize = ["64", "32", "16", "8"]
usbDeviceFunctionsNumberMax = 10
usbDeviceFunctionsNumberDefaultValue = 1 
usbDeviceFunctionsNumberValue = usbDeviceFunctionsNumberDefaultValue
usbDeviceFunctionArray = []
usbDeviceClassArray = []
usbDeviceConfigValueArray = []
usbDeviceStartInterfaceNumberArray = []
usbDeviceNumberOfInterfacesArray = []
usbDebugLogs = 0 
def instantiateComponent(usbComponent):	
	# USB Stack Enable 
	usbEnable = usbComponent.createBooleanSymbol("USE_USB", None)
	usbEnable.setLabel("Use USB Stack?")
	usbEnable.setDescription("Enables USB stack")
	
	# USB Driver Speed selection 	
	usbSpeed = usbComponent.createComboSymbol("USB_SPEED", usbEnable, listUsbSpeed)
	usbSpeed.setLabel("USB Speed Selection")
	usbSpeed.setVisible(False)
	usbSpeed.setDescription("Select USB Operation Speed")
	usbSpeed.setDependencies(blUsbSpeedSelection, ["USE_USB"])
	
	# USB Driver Operation mode selection 
	usbOpMode = usbComponent.createComboSymbol("USB_OPERATION_MODE", usbEnable, listUsbOperationMode)
	usbOpMode.setLabel("USB Mode Selection")
	usbOpMode.setVisible(False)
	usbOpMode.setDescription("Select USB Operation Mode")
	usbOpMode.setDependencies(blUsbOperationMode, ["USE_USB"])
	
	# USB Device EP0 Buffer Size  
	usbDeviceEp0BufferSize = usbComponent.createComboSymbol("USB_DEVICE_EP0_BUFFER_SIZE", usbOpMode, listUsbDeviceEp0BufferSize)
	usbDeviceEp0BufferSize.setLabel("Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setVisible(True)
	usbDeviceEp0BufferSize.setDescription("Select Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setDependencies(blUsbEp0BufferSize, ["USB_OPERATION_MODE"])
	
	# USB Device number of functions 
	usbDeviceFunctionsNumber = usbComponent.createIntegerSymbol("USB_DEVICE_FUNCTIONS_NUMBER", usbOpMode)
	usbDeviceFunctionsNumber.setLabel("Number of Function Drivers Registered to this instance")
	usbDeviceFunctionsNumber.setMax(usbDeviceFunctionsNumberMax)
	usbDeviceFunctionsNumber.setMin(1)
	usbDeviceFunctionsNumber.setValue("USB_DEVICE_FUNCTIONS_NUMBER", usbDeviceFunctionsNumberDefaultValue, 1)
	usbDeviceFunctionsNumber.setVisible(True)
	usbDeviceFunctionsNumber.setDescription("Number of Function Drivers Registered to this instance")
	usbDeviceFunctionsNumber.setDependencies(blUsbFunctionsNumber, ["USB_OPERATION_MODE"])
	
	# USB Device Functions  
	for i in range (0, usbDeviceFunctionsNumberMax):
		# Adding Function driver config
		usbDeviceFunctionArray.append(usbComponent.createBooleanSymbol("USB_DEVICE_FUNCTION" + str(i), usbOpMode))
		usbDeviceFunctionArray[i].setValue("USB_DEVICE_FUNCTION" + str(i), True, 0)
		if (i < usbDeviceFunctionsNumberDefaultValue):
			usbDeviceFunctionArray[i].setVisible(True)
		else:
			usbDeviceFunctionArray[i].setVisible(False)
		usbDeviceFunctionArray[i].setLabel("Function " + str(i))
		usbDeviceFunctionArray[i].setDependencies(blusbDeviceFunctionArray, ["USB_OPERATION_MODE", "USB_DEVICE_FUNCTIONS_NUMBER"])
		
		# Adding Device Class config, Parent: Function driver 
		usbDeviceClassArray.append(usbComponent.createComboSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_DEVICE_CLASS", usbDeviceFunctionArray[i], listUsbDeviceClass))
		usbDeviceClassArray[i].setLabel("Device Class")
		usbDeviceClassArray[i].setVisible(True)
		
		# Config name: Configuration number Parent: Function driver 
		usbDeviceConfigValueArray.append(usbComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_CONFIGURATION", usbDeviceFunctionArray[i]))
		usbDeviceConfigValueArray[i].setLabel("Configuration Value")
		usbDeviceConfigValueArray[i].setVisible(True)
		usbDeviceConfigValueArray[i].setMin(1)
		
		#Adding Start Interface number Parent: Function driver 
		usbDeviceStartInterfaceNumberArray.append(usbComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_INTERFACE_NUMBER", usbDeviceFunctionArray[i]))
		usbDeviceStartInterfaceNumberArray[i].setLabel("Start Interface Number")
		usbDeviceStartInterfaceNumberArray[i].setVisible(True)
		usbDeviceStartInterfaceNumberArray[i].setMin(0)
		
		
		#Adding Number of Interfaces, Parent: Function driver 
		usbDeviceNumberOfInterfacesArray.append(usbComponent.createIntegerSymbol("USB_DEVICE_FUNCTION_" + str(i) + "_NUMBER_OF_INTERFACES", usbDeviceFunctionArray[i]))
		usbDeviceNumberOfInterfacesArray[i].setLabel("Number of Interfaces")
		usbDeviceNumberOfInterfacesArray[i].setVisible(False)
		usbDeviceNumberOfInterfacesArray[i].setMin(1)
		
	
	driverUsbDeviceSource1File = usbComponent.createFileSymbol(None, None)
	driverUsbDeviceSource1File.setSourcePath("usb/templates/drv_usb.c.ftl")
	driverUsbDeviceSource1File.setOutputName("drv_usb" + ".c")
	driverUsbDeviceSource1File.setDestPath("driver/usb/")
	driverUsbDeviceSource1File.setProjectPath("driver/usb/")
	driverUsbDeviceSource1File.setType("SOURCE")

	usbSource1File = usbComponent.createFileSymbol(None, None)
	usbSource1File.setSourcePath("usb/templates/drv_usb.c.ftl")
	usbSource1File.setOutputName("drv_usb" + ".c")
	usbSource1File.setDestPath("driver/usb/")
	usbSource1File.setProjectPath("driver/usb/")
	usbSource1File.setType("SOURCE")
	
def blUsbSpeedSelection(usbSymbolSource, usbSymbolTrigger):
	blUsbLog(usbSymbolSource, usbSymbolTrigger)
	if (usbSymbolTrigger.getValue() == True):
		print("USB Stack is Enabled.")		
		usbSymbolSource.setVisible(True)
	else:
		print("USB Stack is Disabled.")
		usbSymbolSource.setVisible(False)

def blUsbOperationMode(usbSymbolSource, usbSymbolTrigger):
	blUsbLog(usbSymbolSource, usbSymbolTrigger)
	if (usbSymbolTrigger.getValue() == True):
		usbSymbolSource.setVisible(True)
		
	else:
		usbSymbolSource.setVisible(False)

def blUsbEp0BufferSize(usbSymbolSource, usbSymbolTrigger):
	blUsbLog(usbSymbolSource, usbSymbolTrigger)
	if (usbSymbolTrigger.getValue() == "Device"):		
		blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
		
def blUsbFunctionsNumber(usbSymbolSource, usbSymbolTrigger):
	blUsbLog(usbSymbolSource, usbSymbolTrigger)
	if (usbSymbolTrigger.getValue() == "Device"):
		usbSymbolSource.setVisible(True)
	else: 
		usbSymbolSource.setVisible(False)
	
		
def blusbDeviceFunctionArray(usbSymbolSource, usbSymbolTrigger):
	global usbDeviceFunctionsNumberValue
	blUsbLog(usbSymbolSource, usbSymbolTrigger)
	if (usbSymbolTrigger.getID() == "USB_OPERATION_MODE"):
		if (usbSymbolTrigger.getValue() == "Device"):
			for i in range (0, usbDeviceFunctionsNumberMax):
				if (i < usbDeviceFunctionsNumberValue):
					blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
					usbDeviceFunctionArray[i].setVisible(True)
				else:
					blUsbPrint("Set Hide " + usbSymbolSource.getID().encode('ascii', 'ignore'))
					usbDeviceFunctionArray[i].setVisible(False)
		elif (usbSymbolTrigger.getValue() == "Host"):
			usbSymbolSource.setVisible(False)
	elif (usbSymbolTrigger.getID() == "USB_DEVICE_FUNCTIONS_NUMBER"):
		usbDeviceFunctionsNumberValue = usbSymbolTrigger.getValue()
		for i in range (0, usbDeviceFunctionsNumberMax):
			if (i < usbDeviceFunctionsNumberValue):
				usbDeviceFunctionArray[i].setVisible(True)
			else:
				usbDeviceFunctionArray[i].setVisible(False)
				
def blUsbLog (usbSymbolSource, usbSymbolTrigger):
	if (usbDebugLogs == 1):
			print("Source: " + usbSymbolSource.getID().encode('ascii', 'ignore'), "Trigger: " + usbSymbolTrigger.getID().encode('ascii', 'ignore') , "Value: ", usbSymbolTrigger.getValue())
			
def blUsbPrint(string):
	if (usbDebugLogs == 1):
		print(string)