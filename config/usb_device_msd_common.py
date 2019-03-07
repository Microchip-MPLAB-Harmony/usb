usbDeviceMsdMaxNumberofSectors = ["1", "2", "4", "8"]	

def instantiateComponent(usbMsdComponentCommon):
	print ("usb_device_msd_common.py")
	usbDeviceMsdInstnces = usbMsdComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_MSD_INSTANCES", None)
	usbDeviceMsdInstnces.setLabel("Number of Instances")
	usbDeviceMsdInstnces.setMin(1)
	usbDeviceMsdInstnces.setMax(10)
	usbDeviceMsdInstnces.setDefaultValue(1)
	usbDeviceMsdInstnces.setVisible(False)
	
	usbDeviceMsdLunNumber = usbMsdComponentCommon.createIntegerSymbol("USB_DEVICE_MSD_LUNS_NUMBER", None)
	usbDeviceMsdLunNumber.setLabel("Combined Queue Depth")
	usbDeviceMsdLunNumber.setMin(1)
	usbDeviceMsdLunNumber.setMax(32767)
	usbDeviceMsdLunNumber.setDefaultValue(3)
	usbDeviceMsdLunNumber.setVisible(False)
	
	usbDeviceMsdMaxSectorsToBufferCommon = usbMsdComponentCommon.createComboSymbol("CONFIG_USB_DEVICE_FUNCTION_MSD_MAX_SECTORS_COMMON", None, usbDeviceMsdMaxNumberofSectors)
	usbDeviceMsdMaxSectorsToBufferCommon.setLabel("Max Sectors To Buffer")
	usbDeviceMsdMaxSectorsToBufferCommon.setDefaultValue("1")
	usbDeviceMsdMaxSectorsToBufferCommon.setVisible(True)
	
	#########################################################
	# system_config.h file for USB Device MSD function driver 
	#########################################################
	usbDeviceMsdCommonSystemConfigFile = usbMsdComponentCommon.createFileSymbol(None, None)
	usbDeviceMsdCommonSystemConfigFile.setType("STRING")
	usbDeviceMsdCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceMsdCommonSystemConfigFile.setSourcePath("templates/device/msd/system_config.h.device_msd_common.ftl")
	usbDeviceMsdCommonSystemConfigFile.setMarkup(True)
	
	