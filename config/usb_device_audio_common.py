def instantiateComponent(usbAudioComponentCommon):
	print ("usb_device_audio_common.py")
	usbDeviceAudioInstnces = usbAudioComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_AUDIO_INSTANCES", None)
	usbDeviceAudioInstnces.setLabel("Number of Instances")
	usbDeviceAudioInstnces.setMin(1)
	usbDeviceAudioInstnces.setMax(10)
	usbDeviceAudioInstnces.setDefaultValue(1)
	usbDeviceAudioInstnces.setUseSingleDynamicValue(True)
	usbDeviceAudioInstnces.setVisible(False)
	
	usbDeviceAudioQueuDepth = usbAudioComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED", None)
	usbDeviceAudioQueuDepth.setLabel("Combined Queue Depth")
	usbDeviceAudioQueuDepth.setMin(1)
	usbDeviceAudioQueuDepth.setMax(32767)
	usbDeviceAudioQueuDepth.setDefaultValue(0)
	usbDeviceAudioQueuDepth.setUseSingleDynamicValue(True)
	usbDeviceAudioQueuDepth.setVisible(False)
	
	
	usbDeviceAudioStrmInterfaceMax = usbAudioComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_STREAMING_INTERFACES_NUMBER_COMBINED", None)
	usbDeviceAudioStrmInterfaceMax.setMin(1)
	usbDeviceAudioStrmInterfaceMax.setMax(32767)
	usbDeviceAudioStrmInterfaceMax.setDefaultValue(0)
	usbDeviceAudioStrmInterfaceMax.setUseSingleDynamicValue(True)
	usbDeviceAudioStrmInterfaceMax.setVisible(False)
	
	
	usbDeviceAudioAltInterfaceMax = usbAudioComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_MAX_ALTERNATE_SETTING_COMBINED", None)
	usbDeviceAudioAltInterfaceMax.setMin(1)
	usbDeviceAudioAltInterfaceMax.setMax(32767)
	usbDeviceAudioAltInterfaceMax.setDefaultValue(0)
	usbDeviceAudioAltInterfaceMax.setUseSingleDynamicValue(True)
	usbDeviceAudioAltInterfaceMax.setVisible(False)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceAudioCommonSystemConfigFile = usbAudioComponentCommon.createFileSymbol(None, None)
	usbDeviceAudioCommonSystemConfigFile.setType("STRING")
	usbDeviceAudioCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceAudioCommonSystemConfigFile.setSourcePath("templates/device/audio/system_config.h.device_audio_common.ftl")
	usbDeviceAudioCommonSystemConfigFile.setMarkup(True)