def instantiateComponent(usbHostComponent):	
	# USB Host Max Number of Devices   
	usbHostDeviceNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_DEVICE_NUMNBER", None)
	usbHostDeviceNumber.setLabel("Number of Devices")
	usbHostDeviceNumber.setVisible(False)
	usbHostDeviceNumber.setDescription("Maximum Number of Devices that will be attached to this Host")
	usbHostDeviceNumber.setDefaultValue(1)
	usbHostDeviceNumber.setDependencies(blUsbHostDeviceNumber, ["USB_OPERATION_MODE"])

	# USB Host Number of TPL Entries 
	usbHostTplEntryNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_TPL_ENTRY_NUMNBER", None)
	usbHostTplEntryNumber.setLabel("Number of TPL Entries")
	usbHostTplEntryNumber.setVisible(False)
	usbHostTplEntryNumber.setDescription("Number of TPL entries")
	usbHostTplEntryNumber.setDefaultValue(1)
	usbHostTplEntryNumber.setDependencies(blUsbHostDeviceNumber, ["USB_OPERATION_MODE"])

	# USB Host Max Interfaces  
	usbHostMaxInterfaceNumber = usbHostComponent.createIntegerSymbol("CONFIG_USB_HOST_MAX_INTERFACES", None)
	usbHostMaxInterfaceNumber.setLabel("Host Max Interface per Device")
	usbHostMaxInterfaceNumber.setVisible(False)
	usbHostMaxInterfaceNumber.setDescription("Maximum Number of Interfaces per Device")
	usbHostMaxInterfaceNumber.setDefaultValue(1)
	usbHostMaxInterfaceNumber.setDependencies(blUsbHostMaxInterfaceNumber, ["USB_OPERATION_MODE"])

def blUsbHostDeviceNumber(usbSymbolSource, event):
	blUsbLog(usbSymbolSource, event)
	if (event["value"] == "Host"):		
		blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
		
def usbHostTplEntryNumber(usbSymbolSource, event):
	blUsbLog(usbSymbolSource, event)
	if (event["value"] == "Host"):		
		blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
		
def blUsbHostMaxInterfaceNumber(usbSymbolSource, event):
	blUsbLog(usbSymbolSource, event)
	if (event["value"] == "Host"):		
		blUsbPrint("Set Visible " + usbSymbolSource.getID().encode('ascii', 'ignore'))
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)	
		
