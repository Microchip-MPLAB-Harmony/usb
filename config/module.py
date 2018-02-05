def loadModule():
	print("Load Module: Harmony USB Middle ware")
	
	if (Peripheral.moduleExists("USBHS")):
		print("create component: USB Middleware")
		usbComponent = Module.CreateComponent("usb", "USB", "/USB/", "config/usb.py")
		usbComponent.addCapability("LIB_USB")
		#usbComponent.addDependency("USB_Dependency", "USBHS")
	else:
		print("No USB peripheral")