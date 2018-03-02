# USB Device MSD global definitions
usbDeviceMsdMaxNumberofSectors = ["1", "2", "4", "8"]	


def instantiateComponent(usbDeviceMsdComponent, index):
	# Config name: Configuration number
	configValue = usbDeviceMsdComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_CONFIG_VALUE", None)
	configValue.setLabel("Configuration Value")
	configValue.setVisible(True)
	configValue.setMin(1)
	configValue.setMax(16)
	configValue.setDefaultValue(1)
	
	# Adding Start Interface number 
	startInterfaceNumber = usbDeviceMsdComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER", None)
	startInterfaceNumber.setLabel("Start Interface Number")
	startInterfaceNumber.setVisible(True)
	startInterfaceNumber.setMin(0)
	startInterfaceNumber.setDefaultValue(0)
	
	# Adding Number of Interfaces
	numberOfInterfaces = usbDeviceMsdComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_NUMBER_OF_INTERFACES", None)
	numberOfInterfaces.setLabel("Number of Interfaces")
	numberOfInterfaces.setVisible(True)
	numberOfInterfaces.setMin(1)
	numberOfInterfaces.setMax(16)
	numberOfInterfaces.setDefaultValue(2)
	
	# MSD Function driver Max numbers sectors to buffer 
	usbDeviceMsdMaxSectorsToBuffer = usbDeviceMsdComponent.createComboSymbol("CONFIG_USB_DEVICE_FUNCTION_MSD_MAX_SECTORS", None, usbDeviceMsdMaxNumberofSectors)
	usbDeviceMsdMaxSectorsToBuffer.setLabel("Max number of sectors to buffer")
	usbDeviceMsdMaxSectorsToBuffer.setVisible(True)
	
	# MSD Function driver Number of Logical units  
	usbDeviceMsdNumberOfLogicalUnits = usbDeviceMsdComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_MSD_LUN", None)
	usbDeviceMsdNumberOfLogicalUnits.setLabel("Number of Logical Units")
	usbDeviceMsdNumberOfLogicalUnits.setVisible(True)
	usbDeviceMsdNumberOfLogicalUnits.setMin(1)
	
	# MSD Function driver Bulk Out Endpoint Number 
	usbDeviceMsdEPNumberBulkOut = usbDeviceMsdComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_MSD_BULK_OUT_ENDPOINT_NUMBER", None)		
	usbDeviceMsdEPNumberBulkOut.setLabel("Bulk OUT Endpoint Number")
	usbDeviceMsdEPNumberBulkOut.setVisible(True)
	usbDeviceMsdEPNumberBulkOut.setMin(1)

	# MSD Function driver Bulk IN Endpoint Number 
	usbDeviceMsdEPNumberBulkIn = usbDeviceMsdComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_MSD_BULK_IN_ENDPOINT_NUMBER", None)		
	usbDeviceMsdEPNumberBulkIn.setLabel("Bulk IN Endpoint Number")
	usbDeviceMsdEPNumberBulkIn.setVisible(True)
	usbDeviceMsdEPNumberBulkIn.setMin(1)
	