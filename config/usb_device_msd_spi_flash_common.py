def instantiateComponent(usbMsdComponentCommon):
	print ("usb_device_msd_common.py")
	usbDeviceMsdInstnces = usbMsdComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_MSD_INSTANCES", None)
	usbDeviceMsdInstnces.setLabel("Number of Instances")
	usbDeviceMsdInstnces.setMin(1)
	usbDeviceMsdInstnces.setMax(10)
	usbDeviceMsdInstnces.setDefaultValue(1)
	usbDeviceMsdInstnces.setVisible(False)
	
	usbDeviceMsdQueuDepth = usbMsdComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_MSD_QUEUE_DEPTH_COMBINED", None)
	usbDeviceMsdQueuDepth.setLabel("Combined Queue Depth")
	usbDeviceMsdQueuDepth.setMin(1)
	usbDeviceMsdQueuDepth.setMax(32767)
	usbDeviceMsdQueuDepth.setDefaultValue(3)
	usbDeviceMsdQueuDepth.setVisible(False)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceMsdCommonSystemConfigFile = usbMsdComponentCommon.createFileSymbol(None, None)
	usbDeviceMsdCommonSystemConfigFile.setType("STRING")
	usbDeviceMsdCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceMsdCommonSystemConfigFile.setSourcePath("templates/device/msd/system_config.h.device_msd_common.ftl")
	usbDeviceMsdCommonSystemConfigFile.setMarkup(True)