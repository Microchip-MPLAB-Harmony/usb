def instantiateComponent(usbHostHidComponent):

	# USB Host HID Client driver Number of Instances
	usbHostHidClientDriverInstancesNumber = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_HID_NUMBER_OF_INSTANCES", None)
	usbHostHidClientDriverInstancesNumber.setLabel("Number of HID Host Driver Instances")
	usbHostHidClientDriverInstancesNumber.setDescription("Enter the number of HID Client Driver instances required in the application.")
	usbHostHidClientDriverInstancesNumber.setVisible(False)
	usbHostHidClientDriverInstancesNumber.setDefaultValue(False)
	
	# USB Host HID Client driver Interrupt In Endpoints Number 
	usbHostHidClientDriverIntEPsNumber = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_HID_INTERRUPT_IN_ENDPOINTS_NUMBER", None)
	usbHostHidClientDriverIntEPsNumber.setLabel("Number of INTERRUPT IN endpoints supported per HID interface")
	usbHostHidClientDriverIntEPsNumber.setDescription("Enter the number of INTERRUPT IN endpoints supported per HID interface.")
	usbHostHidClientDriverIntEPsNumber.setVisible(False)
	usbHostHidClientDriverIntEPsNumber.setDefaultValue(False)
	
	# USB Host HID Client driver Total Usage Driver instances 
	usbHostHidClientDriverTotalUsageInst = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HID_TOTAL_USAGE_DRIVER_INSTANCES", None)
	usbHostHidClientDriverTotalUsageInst.setLabel("Number of total usage driver instances registered with HID client driver")
	usbHostHidClientDriverTotalUsageInst.setDescription("Enter the number of total usage driver instances registered with HID client driver.")
	usbHostHidClientDriverTotalUsageInst.setVisible(False)
	usbHostHidClientDriverTotalUsageInst.setDefaultValue(False)
	
	# USB Host HID Client driver Global Push Pop Stack size
	usbHostHidClientDriverPushPopStackSize = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HID_GLOBAL_PUSH_POP_STACK_SIZE", None)
	usbHostHidClientDriverPushPopStackSize.setLabel("Number of PUSH items that can be saved in the Global item queue per field per HID interface")
	usbHostHidClientDriverPushPopStackSize.setDescription("Enter the number of PUSH items that can be saved in the Global item queue per field per HID interface.")
	usbHostHidClientDriverPushPopStackSize.setVisible(False)
	usbHostHidClientDriverPushPopStackSize.setDefaultValue(False)
	
	# USB Host HID Client driver Mouse 
	usbHostHidClientDriverMouse = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_USE_MOUSE", None)
	usbHostHidClientDriverMouse.setLabel("Use HID Host Mouse Driver")
	usbHostHidClientDriverMouse.setDescription("Selecting this will include HID Host Mouse Usage Driver into the project.")
	usbHostHidClientDriverMouse.setVisible(False)
	usbHostHidClientDriverMouse.setDefaultValue(False)
	
	# USB Host HID Client driver Mouse Buttons Number 
	usbHostHidClientDriverMouseButtonsNumber = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_HID_MOUSE_BUTTONS_NUMBER", None)
	usbHostHidClientDriverMouseButtonsNumber.setLabel("Number of Mouse buttons whose value will be captured per HID Mouse device")
	usbHostHidClientDriverMouseButtonsNumber.setDescription("Enter the Number of Mouse buttons whose value will be captured per HID Mouse device")
	usbHostHidClientDriverMouseButtonsNumber.setVisible(False)
	usbHostHidClientDriverMouseButtonsNumber.setDefaultValue(False)
	
	# USB Host HID Client driver Keyboard 
	usbHostHidClientDriverKeyboard = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_USE_KEYBOARD", None)
	usbHostHidClientDriverKeyboard.setLabel("Use HID Host Keyboard Driver")
	usbHostHidClientDriverKeyboard.setDescription("Selecting this will include HID Host Keyboard Usage Driver into the project.")
	usbHostHidClientDriverKeyboard.setVisible(False)
	usbHostHidClientDriverKeyboard.setDefaultValue(False)
		
	##############################################################
	# system_definitions.h file for USB Host HID Client driver   
	##############################################################
	usbHostHidSystemDefFile = usbHostHidComponent.createFileSymbol(None, None)
	usbHostHidSystemDefFile.setType("STRING")
	usbHostHidSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbHostHidSystemDefFile.setSourcePath("templates/host/system_definitions.h.host_hid_includes.ftl")
	usbHostHidSystemDefFile.setMarkup(True)
	
	##############################################################
	# system_config.h file for USB Host HID Client driver   
	##############################################################
	usbHostHidSystemConfigFile = usbHostHidComponent.createFileSymbol(None, None)
	usbHostHidSystemConfigFile.setType("STRING")
	usbHostHidSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbHostHidSystemConfigFile.setSourcePath("templates/host/system_config.h.host_hid.ftl")
	usbHostHidSystemConfigFile.setMarkup(True)
	
	
	################################################
	# USB Host HID Client driver Files 
	################################################
	usbHostHidHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid.h', usbHostHidComponent, usbHostHidHeaderFile, "", "/usb/", True, None)
	
	usbHidHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_hid.h', usbHostHidComponent, usbHidHeaderFile, "", "/usb/", True, None)
	
	usbHostHidSourceFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid.c', usbHostHidComponent, usbHostHidSourceFile, "src/dynamic/", "/usb/src/dynamic", True, None)
	
	usbHostHidLocalHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid_local.h', usbHostHidComponent, usbHostHidLocalHeaderFile, "src/", "/usb/src", True, None)
	
	
	# all files go into src/
def addFileName(fileName, component, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
	#filename = component.createFileSymbol(None, None)
	symbol.setProjectPath("config/" + configName1 + destPath)
	symbol.setSourcePath(srcPath + fileName)
	symbol.setOutputName(fileName)
	symbol.setDestPath(destPath)
	if fileName[-2:] == '.h':
		symbol.setType("HEADER")
	else:
		symbol.setType("SOURCE")
	symbol.setEnabled(enabled)
	if callback != None:
		symbol.setDependencies(callback, ["USB_DEVICE_FUNCTION_1_DEVICE_CLASS"])