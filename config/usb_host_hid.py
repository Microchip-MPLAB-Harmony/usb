usbHostHidClientDriverInstancesNumber = None 
usbHostHidClientDriverIntEPsNumber = None 
usbHostHidClientDriverTotalUsageInst = None 
usbHostHidClientDriverPushPopStackSize = None 
usbHostHidClientDriverMouse = None 
usbHostHidClientDriverMouseButtonsNumber = None 
usbHostHidClientDriverKeyboard = None
usbHostHidSystemDefFile = None
usbHostHidSystemConfigFile = None
usbHostHidInitFile = None
usbHostHidTplEntryFile = None
usbHostHidHeaderFile  = None
usbHidHeaderFile  = None
usbHostHidSourceFile  = None
usbHostHidLocalHeaderFile  = None
usbHostHidKeyboardSourceFile  = None
usbHostHidKeyboardHeaderFile  = None
usbHostHidKeyboardLocalHeaderFile  = None
usbHostHidMouseSourceFile  = None
usbHostHidMouseHeaderFile  = None
usbHostHidMouseLocalHeaderFile  = None


def onAttachmentConnected(source, target):
	ownerComponent = source["component"]
	print("USB Host Layer Connected")
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		Database.clearSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.setSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER", readValue + 1 , 2)
	
def onAttachmentDisconnected(source, target):
	ownerComponent = source["component"]
	print("USB Host Layer Disconnected")
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		Database.clearSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.setSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER", readValue - 1 , 2)
		
def destroyComponent(component):	
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		Database.clearSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		Database.setSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER", readValue - 1 , 2)
		
def mouseEnable(symbol, event):
	if (event["value"] == True):
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)

def keyBoardEnable(symbol, event):
	
	if (event["value"] == True):
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)

def setVisible (symbol, event):	
	if (event["value"] == True):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)
		
def instantiateComponent(usbHostHidComponent):
	
	global usbHostHidClientDriverInstancesNumber
	global usbHostHidClientDriverIntEPsNumber 
	global usbHostHidClientDriverTotalUsageInst 
	global usbHostHidClientDriverPushPopStackSize  
	global usbHostHidClientDriverMouse  
	global usbHostHidClientDriverMouseButtonsNumber  
	global usbHostHidClientDriverKeyboard 
	global usbHostHidSystemDefFile 
	global usbHostHidSystemConfigFile 
	global usbHostHidInitFile 
	global usbHostHidTplEntryFile 
	global usbHostHidHeaderFile  
	global usbHidHeaderFile  
	global usbHostHidSourceFile 
	global usbHostHidLocalHeaderFile  
	global usbHostHidKeyboardSourceFile 
	global usbHostHidKeyboardHeaderFile  
	global usbHostHidKeyboardLocalHeaderFile  
	global usbHostHidMouseSourceFile 
	global usbHostHidMouseHeaderFile 
	global usbHostHidMouseLocalHeaderFile  

	res = Database.activateComponents(["usb_host"])

	# USB Host HID Client driver Number of Instances
	usbHostHidClientDriverInstancesNumber = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HOST_HID_NUMBER_OF_INSTANCES", None)
	usbHostHidClientDriverInstancesNumber.setLabel("Number of Instances")
	usbHostHidClientDriverInstancesNumber.setDescription("Enter the number of HID Client Driver instances required in the application.")
	usbHostHidClientDriverInstancesNumber.setVisible(True)
	usbHostHidClientDriverInstancesNumber.setDefaultValue(1)
	
	# USB Host HID Client driver Interrupt In Endpoints Number 
	usbHostHidClientDriverIntEPsNumber = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HOST_HID_INTERRUPT_IN_ENDPOINTS_NUMBER", None)
	usbHostHidClientDriverIntEPsNumber.setLabel("Number of INTERRUPT IN endpoints per Interface")
	usbHostHidClientDriverIntEPsNumber.setDescription("Enter the number of INTERRUPT IN endpoints supported per HID interface.")
	usbHostHidClientDriverIntEPsNumber.setVisible(True)
	usbHostHidClientDriverIntEPsNumber.setDefaultValue(1)
	
	# USB Host HID Client driver Total Usage Driver instances 
	usbHostHidClientDriverTotalUsageInst = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HID_TOTAL_USAGE_DRIVER_INSTANCES", None)
	usbHostHidClientDriverTotalUsageInst.setLabel("Number of Usage Driver instances")
	usbHostHidClientDriverTotalUsageInst.setDescription("Enter the number of total usage driver instances registered with HID client driver.")
	usbHostHidClientDriverTotalUsageInst.setVisible(True)
	usbHostHidClientDriverTotalUsageInst.setDefaultValue(1)
	
	# USB Host HID Client driver Global Push Pop Stack size
	usbHostHidClientDriverPushPopStackSize = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HID_GLOBAL_PUSH_POP_STACK_SIZE", None)
	usbHostHidClientDriverPushPopStackSize.setLabel("Number of PUSH items")
	usbHostHidClientDriverPushPopStackSize.setDescription("Enter the number of PUSH items that can be saved in the Global item queue per field per HID interface.")
	usbHostHidClientDriverPushPopStackSize.setVisible(True)
	usbHostHidClientDriverPushPopStackSize.setDefaultValue(1)
	
	# USB Host HID Client driver Mouse 
	usbHostHidClientDriverMouse = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_USE_MOUSE", None)
	usbHostHidClientDriverMouse.setLabel("Use Mouse Driver")
	usbHostHidClientDriverMouse.setDescription("Selecting this will include HID Host Mouse Usage Driver into the project.")
	usbHostHidClientDriverMouse.setVisible(True)
	usbHostHidClientDriverMouse.setDefaultValue(False)
	
	# USB Host HID Client driver Mouse Buttons Number 
	usbHostHidClientDriverMouseButtonsNumber = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HOST_HID_MOUSE_BUTTONS_NUMBER", usbHostHidClientDriverMouse)
	usbHostHidClientDriverMouseButtonsNumber.setLabel("Number of Mouse buttons")
	usbHostHidClientDriverMouseButtonsNumber.setDescription("Enter the Number of Mouse buttons whose value will be captured per HID Mouse device")
	usbHostHidClientDriverMouseButtonsNumber.setVisible(False)
	usbHostHidClientDriverMouseButtonsNumber.setDefaultValue(False)
	usbHostHidClientDriverMouseButtonsNumber.setDependencies(setVisible, ["CONFIG_USB_HOST_USE_MOUSE"])
	
	# USB Host HID Client driver Keyboard 
	usbHostHidClientDriverKeyboard = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_USE_KEYBOARD", None)
	usbHostHidClientDriverKeyboard.setLabel("Use Keyboard Driver")
	usbHostHidClientDriverKeyboard.setDescription("Selecting this will include HID Host Keyboard Usage Driver into the project.")
	usbHostHidClientDriverKeyboard.setVisible(True)
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
	
	############################################################
	# Client driver Init Data Entry for HID
	#############################################################
	usbHostHidInitFile = usbHostHidComponent.createFileSymbol(None, None)
	usbHostHidInitFile.setType("STRING")
	usbHostHidInitFile.setOutputName("usb_host.LIST_USB_HOST_CLIENT_INIT_DATA")
	usbHostHidInitFile.setSourcePath("templates/host/system_init_c_hid_data.ftl")
	usbHostHidInitFile.setMarkup(True)
	
	
	##############################################################
	# TPL Entry for HID client driver 
	##############################################################
	usbHostHidTplEntryFile = usbHostHidComponent.createFileSymbol(None, None)
	usbHostHidTplEntryFile.setType("STRING")
	usbHostHidTplEntryFile.setOutputName("usb_host.LIST_USB_HOST_TPL_ENTRY")
	usbHostHidTplEntryFile.setSourcePath("templates/host/system_init_c_hid_tpl.ftl")
	usbHostHidTplEntryFile.setMarkup(True)
	
	################################################
	# USB Host HID Client driver Files 
	################################################
	usbHostHidHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid.h', usbHostHidComponent, usbHostHidHeaderFile, "", "/usb/", True, None)
	
	usbHidHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_hid.h', usbHostHidComponent, usbHidHeaderFile, "", "/usb/", True, None)
	
	usbHostHidSourceFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid.c', usbHostHidComponent, usbHostHidSourceFile, "src/", "/usb/src/", True, None)
	
	usbHostHidLocalHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid_local.h', usbHostHidComponent, usbHostHidLocalHeaderFile, "src/", "/usb/src", True, None)
	
	usbHostHidKeyboardSourceFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid_keyboard.c', usbHostHidComponent, usbHostHidKeyboardSourceFile, "src/", "/usb/src/", False, keyBoardEnable)
	
	usbHostHidKeyboardHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid_keyboard.h', usbHostHidComponent, usbHostHidKeyboardHeaderFile, "", "/usb/", False, keyBoardEnable)
	
	usbHostHidKeyboardLocalHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid_keyboard_local.h', usbHostHidComponent, usbHostHidKeyboardLocalHeaderFile, "src/", "/usb/src", False, keyBoardEnable)
	
	usbHostHidMouseSourceFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid_mouse.c', usbHostHidComponent, usbHostHidMouseSourceFile, "src/", "/usb/src/", False, mouseEnable)
	
	usbHostHidMouseHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid_mouse.h', usbHostHidComponent, usbHostHidMouseHeaderFile, "", "/usb/", False, mouseEnable)
	
	usbHostHidMouseLocalHeaderFile = usbHostHidComponent.createFileSymbol(None, None)
	addFileName('usb_host_hid_mouse_local.h', usbHostHidComponent, usbHostHidMouseLocalHeaderFile, "src/", "/usb/src", False, mouseEnable)

	
	
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
		if callback == mouseEnable:
			symbol.setDependencies(callback, ["CONFIG_USB_HOST_USE_MOUSE"])
		elif callback == keyBoardEnable:
			symbol.setDependencies(callback, ["CONFIG_USB_HOST_USE_KEYBOARD"])