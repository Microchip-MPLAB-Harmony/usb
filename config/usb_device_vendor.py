def instantiateComponent(usbCdcComponent, index):

	res = Database.activateComponents(["usb_device"])
	
	# Vendor Endpoint Read Queue Size 
	usbDeviceEndpointReadQueueSize.append(usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_" + str(i) + "_ENDPOINT_QUEUE_SIZE_READ_IDX0", listUsbDeviceFunctionMenu[i]))
	usbDeviceEndpointReadQueueSize[i].setLabel("Endpoint Read Queue Size")	
	usbDeviceEndpointReadQueueSize[i].setVisible(False)	
	usbDeviceEndpointReadQueueSize[i].setDependencies(blUsbDeviceVendorFunctionChanged, ["CONFIG_USB_DEVICE_FUNCTION_" + str(i) + "_DEVICE_CLASS"])

	# VendorEndpoint Write Queue Size
	usbDeviceEndpointWriteQueueSize.append(usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_" + str(i) + "_ENDPOINT_QUEUE_SIZE_WRITE", listUsbDeviceFunctionMenu[i]))
	usbDeviceEndpointWriteQueueSize[i].setLabel("Endpoint Write Queue Size")	
	usbDeviceEndpointWriteQueueSize[i].setVisible(False)	
	usbDeviceEndpointWriteQueueSize[i].setDependencies(blUsbDeviceVendorFunctionChanged, ["CONFIG_USB_DEVICE_FUNCTION_" + str(i) + "_DEVICE_CLASS"])
