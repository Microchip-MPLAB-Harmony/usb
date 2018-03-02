def loadModule():
	print("Load Module: Harmony USB Middle ware")
	
	if (Peripheral.moduleExists("USBHS")):
		print("create component: USB Driver")
		usbDriverComponent =  Module.CreateComponent("usb", "USB High Speed Driver", "/Libraries/USB", "config/usb_driver.py")
		usbDriverComponent.addCapability("DRV_USBHSV1", "DRV_USBHSV1")
		usbDriverComponent.addDependency("drv_dependency", "drv_core")
		usbDriverComponent.addDependency("SYS_Dependency", "sys_core")
		usbDriverComponent.addDependency("SYS_int_dependency", "sys_int")
		#usbDriverComponent.addCapability("DRV_USBHSV1_HOST", "DRV_USBHSV1_HOST")
		
		print("create component: USB Device")
		usbDeviceComponent = Module.CreateComponent("usbDevice", "USB Device", "/Libraries/USB", "config/usb_device.py")
		usbDeviceComponent.addDependency("USB_Dependency", "DRV_USBHSV1")
		
		print("create component: USB Host")
		usbHostComponent = Module.CreateComponent("usbHost", "USB Host", "/Libraries/USB", "config/usb_host.py")
		usbHostComponent.addDependency("USB_Dependency", "DRV_USBHSV1")
	else:
		print("No USB peripheral")
		