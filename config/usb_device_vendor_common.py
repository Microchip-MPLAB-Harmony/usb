def instantiateComponent(usbVendorComponentCommon):
	usbDeviceVendorInstnces = usbVendorComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_VENDOR_INSTANCES", None)
	usbDeviceVendorInstnces.setLabel("Number of Instances")
	usbDeviceVendorInstnces.setMin(1)
	usbDeviceVendorInstnces.setMax(10)
	usbDeviceVendorInstnces.setDefaultValue(1)
	usbDeviceVendorInstnces.setUseSingleDynamicValue(True)
	usbDeviceVendorInstnces.setVisible(False)
	
	usbDeviceVendorQueuDepth = usbVendorComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_VENDOR_QUEUE_DEPTH_COMBINED", None)
	usbDeviceVendorQueuDepth.setLabel("Combined Queue Depth")
	usbDeviceVendorQueuDepth.setMin(0)
	usbDeviceVendorQueuDepth.setMax(32767)
	usbDeviceVendorQueuDepth.setDefaultValue(0)
	usbDeviceVendorQueuDepth.setUseSingleDynamicValue(True)
	usbDeviceVendorQueuDepth.setVisible(False)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceVendorCommonSystemConfigFile = usbVendorComponentCommon.createFileSymbol(None, None)
	usbDeviceVendorCommonSystemConfigFile.setType("STRING")
	usbDeviceVendorCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceVendorCommonSystemConfigFile.setSourcePath("templates/device/vendor/system_config.h.device_vendor_common.ftl")
	usbDeviceVendorCommonSystemConfigFile.setMarkup(True)
	
	##############################################################
	# system_definitions.h file for USB Device Vendor Function driver   
	##############################################################
	usbDeviceVendorCommonSystemDefFile = usbVendorComponentCommon.createFileSymbol(None, None)
	usbDeviceVendorCommonSystemDefFile.setType("STRING")
	usbDeviceVendorCommonSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceVendorCommonSystemDefFile.setSourcePath("templates/device/vendor/system_definitions.h.device_vendor_includes.ftl")
	usbDeviceVendorCommonSystemDefFile.setMarkup(True)