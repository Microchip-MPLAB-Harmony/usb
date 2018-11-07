def instantiateComponent(usbHidComponentCommon):
	print ("usb_device_hid_common.py")
	usbDeviceHidInstnces = usbHidComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_HID_INSTANCES", None)
	usbDeviceHidInstnces.setLabel("Number of Instances")
	usbDeviceHidInstnces.setMin(1)
	usbDeviceHidInstnces.setMax(10)
	usbDeviceHidInstnces.setDefaultValue(1)
	usbDeviceHidInstnces.setVisible(False)
	
	usbDeviceHidQueuDepth = usbHidComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_HID_QUEUE_DEPTH_COMBINED", None)
	usbDeviceHidQueuDepth.setLabel("Combined Queue Depth")
	usbDeviceHidQueuDepth.setMin(1)
	usbDeviceHidQueuDepth.setMax(32767)
	usbDeviceHidQueuDepth.setDefaultValue(3)
	usbDeviceHidQueuDepth.setUseSingleDynamicValue(True)
	usbDeviceHidQueuDepth.setVisible(False)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceHidCommonSystemConfigFile = usbHidComponentCommon.createFileSymbol(None, None)
	usbDeviceHidCommonSystemConfigFile.setType("STRING")
	usbDeviceHidCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceHidCommonSystemConfigFile.setSourcePath("templates/device/hid/system_config.h.device_hid_common.ftl")
	usbDeviceHidCommonSystemConfigFile.setMarkup(True)
	
	##############################################################
	# system_definitions.h file for USB Device HID Function driver   
	##############################################################
	usbDeviceHidSystemDefFile = usbHidComponentCommon.createFileSymbol(None, None)
	usbDeviceHidSystemDefFile.setType("STRING")
	usbDeviceHidSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceHidSystemDefFile.setSourcePath("templates/device/hid/system_definitions.h.device_hid_includes.ftl")
	usbDeviceHidSystemDefFile.setMarkup(True)