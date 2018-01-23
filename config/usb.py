def instantiateComponent(usbComponent, index):

	usbMenu = usbComponent.createMenuSymbol(None, None)
	usbMenu.setLabel("USB Settings")

	usbEnable = usbComponent.createBooleanSymbol("USE_usb", usbMenu)
	usbEnable.setLabel("Use USB Stack?")
	usbEnable.setDescription("Enables usb driver instance " + str(index))

	usbBL = usbComponent.createBooleanSymbol("BL", usbMenu)
	usbBL.setVisible(False)
	usbBL.setDependencies(usbBusinessLogic, ["USE_usb"])

	usbIndex = usbComponent.createIntegerSymbol("INDEX", usbMenu)
	usbIndex.setVisible(False)
	usbIndex.setDefaultValue(index)

	usbSource1File = usbComponent.createFileSymbol(None, None)
	usbSource1File.setSourcePath("driver/usb/templates/drv_usb.c.ftl")
	usbSource1File.setOutputName("drv_usb" + str(index) + ".c")
	usbSource1File.setDestPath("driver/usb/")
	usbSource1File.setProjectPath("driver/usb/")
	usbSource1File.setType("SOURCE")


def usbBusinessLogic(usbBL, usbEnable):
	if (usbEnable.getValue() == True):
		print("usb Driver is enabled. setting plib...")
#		usbBL.getComponent().getDependencyComponent("DRV_usb_Dependency").setSymbolValue("Config1", "Leonard", True, 1)
	else:
		print("usb Driver is disabled. clearing plib...")
#		usbBL.getComponent().getDependencyComponent("DRV_usb_Dependency").setSymbolValue("Config1", "Leonard", False, 1)


