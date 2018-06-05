def loadModule():
	print("Load Module: Harmony USB Middle ware")
	
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70"]):
		print("create component: USB Driver")
		usbDriverComponent =  Module.CreateComponent("drv_usbhs_v1", "USB High Speed Driver", "/Harmony/Drivers", "config/usb_driver.py")
		usbDriverComponent.addCapability("DRV_USB", "DRV_USB")
	
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMD20", "SAMD21", "SAMD51", "SAME51", "SAME53", "SAME54"]):
		print("create component: USB Driver")
		usbDriverComponent =  Module.CreateComponent("drv_usbfs_v1", "USB Full Speed Driver", "/Harmony/Drivers", "config/usbfs_v1_driver.py")
		usbDriverComponent.addCapability("DRV_USB", "DRV_USB")
		
	print("create component: USB Device Layer")
	usbDeviceComponent = Module.CreateSharedComponent("usb_device", "USB Device Layer", "/Libraries/USB/Device Stack", "config/usb_device.py")
	usbDeviceComponent.addDependency("usb_driver_dependency", "DRV_USB")
	usbDeviceComponent.addCapability("USB Device", "USB_DEVICE")
	
	print("create component: USB Device CDC")
	usbDeviceCdcComponent = Module.CreateGeneratorComponent("usb_device_cdc", "CDC", "/Libraries/USB/Device Stack", "config/usb_device_cdc_common.py", "config/usb_device_cdc.py")
	usbDeviceCdcComponent.addDependency("usb_device_dependency", "USB_DEVICE")
	usbDeviceCdcComponent.addCapability("USB Device", "USB_DEVICE_CDC")
	
	print("create component: USB Device Vendor")
	usbDeviceVendorComponent = Module.CreateGeneratorComponent("usb_device_vendor", "Vendor", "/Libraries/USB/Device Stack", "config/usb_device_vendor_common.py", "config/usb_device_vendor.py")
	usbDeviceVendorComponent.addDependency("usb_device_dependency", "USB_DEVICE")
	usbDeviceVendorComponent.addCapability("USB Device", "USB_DEVICE_VENDOR")
	
	print("create component: USB Device Audio")
	usbDeviceAudioComponent = Module.CreateGeneratorComponent("usb_device_audio", "Audio", "/Libraries/USB/Device Stack", "config/usb_device_audio_common.py", "config/usb_device_audio.py")
	usbDeviceAudioComponent.addDependency("usb_device_dependency", "USB_DEVICE")
	usbDeviceAudioComponent.addCapability("USB Device", "USB_DEVICE_AUDIO")
	
	print("create component: USB Device HID")
	usbDeviceHidComponent = Module.CreateGeneratorComponent("usb_device_hid", "HID", "/Libraries/USB/Device Stack", "config/usb_device_hid_common.py", "config/usb_device_hid.py")
	usbDeviceHidComponent.addDependency("usb_device_dependency", "USB_DEVICE")
	usbDeviceHidComponent.addCapability("USB Device", "USB_DEVICE_HID")
	
	print("create component: USB Device MSD")
	usbDeviceMsdComponent = Module.CreateGeneratorComponent("usb_device_msd", "MSD", "/Libraries/USB/Device Stack", "config/usb_device_msd_common.py", "config/usb_device_msd.py")
	usbDeviceMsdComponent.addDependency("usb_device_dependency", "USB_DEVICE")
	usbDeviceMsdComponent.addCapability("USB Device", "USB_DEVICE_MSD")
	
	print("create component: USB Device MSD LUN Media NVM")
	usbDeviceMsdLunNVMComponent = Module.CreateGeneratorComponent("usb_device_msd_lun_media_nvm", "LUN Media NVM", "/Libraries/USB/Device Stack/MSD Media Type", "config/usb_device_msd_nvm_common.py", "config/usb_device_msd_nvm.py")
	usbDeviceMsdLunNVMComponent.addDependency("usb_device_msd_dependency", "USB_DEVICE_MSD")
	usbDeviceMsdLunNVMComponent.addDependency("drv_memory_dependency", "DRV_MEMORY")	
	#usbDeviceMsdLunNVMComponent.addCapability("USB Device", "USB_DEVICE_MSD")
	
	print("create component: USB Device MSD LUN Media SD Card")
	usbDeviceMsdLunSdCardComponent = Module.CreateGeneratorComponent("usb_device_msd_lun_media_sd_card", "LUN Media SD Card", "/Libraries/USB/Device Stack/MSD Media Type", "config/usb_device_msd_sd_card_common.py", "config/usb_device_msd_sd_card.py")
	usbDeviceMsdLunSdCardComponent.addDependency("usb_device_msd_dependency", "USB_DEVICE_MSD")
	usbDeviceMsdLunSdCardComponent.addDependency("drv_memory_dependency", "DRV_MEMORY")
	#usbDeviceMsdLunSdCardComponent.addCapability("USB Device", "USB_DEVICE_MSD")
	
	print("create component: USB Device MSD LUN Media SPI Flash")
	usbDeviceMsdLunSpiFlashComponent = Module.CreateGeneratorComponent("usb_device_msd_lun_media_spi_flash", "LUN Media SPI Flash", "/Libraries/USB/Device Stack/MSD Media Type", "config/usb_device_msd_sd_card_common.py", "config/usb_device_msd_spi_flash.py")
	usbDeviceMsdLunSpiFlashComponent.addDependency("usb_device_msd_dependency", "USB_DEVICE_MSD")
	usbDeviceMsdLunSpiFlashComponent.addDependency("drv_memory_dependency", "DRV_MEMORY")
	#usbDeviceMsdLunSpiFlashComponent.addCapability("USB Device", "USB_DEVICE_MSD")
	
	print("create component: USB Host")
	usbHostComponent = Module.CreateSharedComponent("usb_host", "Host Layer", "/Libraries/USB/Host Stack", "config/usb_host.py")
	usbHostComponent.addDependency("usb_driver_dependency", "DRV_USB")
	usbHostComponent.addDependency("usb_host_tmr_dependency", "SYS_TIME")
	usbHostComponent.addCapability("usb_host", "USB_HOST")
	
	print("create component: USB Host MSD")
	usbHostMsdComponent = Module.CreateComponent("usb_host_msd", "MSD Client Driver", "/Libraries/USB/Host Stack","config/usb_host_msd.py")
	usbHostMsdComponent.addDependency("usb_host_dependency", "USB_HOST")
	
	print("create component: USB Host CDC")
	usbHostCdcComponent = Module.CreateComponent("usb_host_cdc", "CDC Client Driver", "/Libraries/USB/Host Stack", "config/usb_host_cdc.py")
	usbHostCdcComponent.addDependency("usb_host_dependency", "USB_HOST")
	
	print("create component: USB Host HID")
	usbHostCdcComponent = Module.CreateComponent("usb_host_hid", "HID Client Driver", "/Libraries/USB/Host Stack", "config/usb_host_hid.py")
	usbHostCdcComponent.addDependency("usb_host_dependency", "USB_HOST")

		