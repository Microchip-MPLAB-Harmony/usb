def instantiateComponent(usbCdcComponentCommon):
	print ("usb_device_cdc_common.py")
	usbDeviceCdcInstnces = usbCdcComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_CDC_INSTANCES", None)
	usbDeviceCdcInstnces.setLabel("Number of Instances")
	usbDeviceCdcInstnces.setMin(1)
	usbDeviceCdcInstnces.setMax(10)
	usbDeviceCdcInstnces.setDefaultValue(1)
	usbDeviceCdcInstnces.setUseSingleDynamicValue(True)
	usbDeviceCdcInstnces.setVisible(False)
	
	usbDeviceCdcQueuDepth = usbCdcComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED", None)
	usbDeviceCdcQueuDepth.setLabel("Combined Queue Depth")
	usbDeviceCdcQueuDepth.setMin(1)
	usbDeviceCdcQueuDepth.setMax(32767)
	usbDeviceCdcQueuDepth.setDefaultValue(0)
	usbDeviceCdcQueuDepth.setUseSingleDynamicValue(True)
	usbDeviceCdcQueuDepth.setVisible(False)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceCdcCommonSystemConfigFile = usbCdcComponentCommon.createFileSymbol(None, None)
	usbDeviceCdcCommonSystemConfigFile.setType("STRING")
	usbDeviceCdcCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceCdcCommonSystemConfigFile.setSourcePath("templates/device/cdc/system_config.h.device_cdc_common.ftl")
	usbDeviceCdcCommonSystemConfigFile.setMarkup(True)
	
	##############################################################
	# system_definitions.h file for USB Device CDC Function driver   
	##############################################################
	usbDeviceCdcCommonSystemDefFile = usbCdcComponentCommon.createFileSymbol(None, None)
	usbDeviceCdcCommonSystemDefFile.setType("STRING")
	usbDeviceCdcCommonSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceCdcCommonSystemDefFile.setSourcePath("templates/device/cdc/system_definitions.h.device_cdc_includes.ftl")
	usbDeviceCdcCommonSystemDefFile.setMarkup(True)