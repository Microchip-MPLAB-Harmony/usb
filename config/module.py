def loadModule():
	print("Load Module: Harmony USB Middle ware")

	if (Peripheral.moduleExists("USBHS")):
		print("create component: USB Middleware")
		usbComponent = Module.CreateGeneratorComponent("usb", "USB", "/USB/", "config/usb.py")
		usbComponent.addCapability("USB")
		usbComponent.addDependency("USB_Dependency", "USB")
	else:
		print("No USB peripheral")