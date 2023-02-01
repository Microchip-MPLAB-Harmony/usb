"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""
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
usbHostHidClientDriverMouseSaveStatus = None 
usbHostHidClientDriverKeyboardSaveStatus = None 


def onAttachmentConnected(source, target):
	dependencyID = source["id"]
	ownerComponent = source["component"]
	if (dependencyID == "usb_host_dependency"):
		args = {"nTpl":1}
		res = Database.sendMessage("usb_host", "INCREMENT_TPL_ENTRY_NUMBER", args)

def onAttachmentDisconnected(source, target):
	dependencyID = source["id"]
	ownerComponent = source["component"]
	if (dependencyID == "usb_host_dependency"):
		args = {"nTpl":1}
		res = Database.sendMessage("usb_host", "DECREMENT_TPL_ENTRY_NUMBER", args)
		
def destroyComponent(component):	
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		args = {"nTpl": readValue - 1}
		res = Database.sendMessage("usb_host", "UPDATE_TPL_ENTRY_NUMBER", args)
		
def mouseEnable(symbol, event):
	global usbHostHidClientDriverTotalUsageInst
	if (event["value"] == True):
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)

def keyBoardEnable(symbol, event):
	global usbHostHidClientDriverTotalUsageInst
	if (event["value"] == True):
		symbol.setEnabled(True)
	else:
		symbol.setEnabled(False)

def setVisible (symbol, event):	
	if (event["value"] == True):
		symbol.setVisible(True)
	else:
		symbol.setVisible(False)

def updateUsageDriverInstanceNumber (symbol, event):
	global usbHostHidClientDriverMouseSaveStatus
	global usbHostHidClientDriverKeyboardSaveStatus
	if (event["id"] == "CONFIG_USB_HOST_USE_MOUSE"):
		if (event["value"] == True) and usbHostHidClientDriverMouseSaveStatus.getValue() == False:
			usageDriverInstances = symbol.getValue()
			symbol.setValue(usageDriverInstances + 1, 2)
		elif (event["value"] == False) and usbHostHidClientDriverMouseSaveStatus.getValue() == True: 
			usageDriverInstances = symbol.getValue()
			symbol.setValue(usageDriverInstances - 1, 2)
		usbHostHidClientDriverMouseSaveStatus.setValue(event["value"])
	elif (event["id"] == "CONFIG_USB_HOST_USE_KEYBOARD"):
		if (event["value"] == True) and usbHostHidClientDriverKeyboardSaveStatus.getValue() == False:
			usageDriverInstances = symbol.getValue()
			symbol.setValue(usageDriverInstances + 1, 2)
		elif (event["value"] == False) and usbHostHidClientDriverKeyboardSaveStatus.getValue() == True: 
			usageDriverInstances = symbol.getValue()
			symbol.setValue(usageDriverInstances - 1, 2)
		usbHostHidClientDriverKeyboardSaveStatus.setValue(event["value"])
		
		
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
	global usbHostHidClientDriverMouseSaveStatus
	global usbHostHidClientDriverKeyboardSaveStatus

	res = Database.activateComponents(["usb_host"])

	# USB Host HID Client driver Number of Instances
	usbHostHidClientDriverInstancesNumber = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HOST_HID_NUMBER_OF_INSTANCES", None)
	usbHostHidClientDriverInstancesNumber.setLabel("Number of Instances")
        helpText = '''Configure the number of HID devices or interfaces to be supported. For example, setting this 
        value to 2 will allow the application to support two different HID devices, say a mouse and a keyboard or a 
        composite USB device that has two HID interfaces.'''
	usbHostHidClientDriverInstancesNumber.setDescription(helpText)
	usbHostHidClientDriverInstancesNumber.setVisible(True)
	usbHostHidClientDriverInstancesNumber.setDefaultValue(1)
	
	# USB Host HID Client driver Interrupt In Endpoints Number 
	usbHostHidClientDriverIntEPsNumber = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HOST_HID_INTERRUPT_IN_ENDPOINTS_NUMBER", None)
	usbHostHidClientDriverIntEPsNumber.setLabel("Number of INTERRUPT IN endpoints per Interface")
        helpText = '''Configure the maximum number of Interrupt endpoints that a attached HID device or interface
        will contain. For example, if the application supports two HID interfaces, one of them has 1 interrupt 
        endpoint and then other has 2 interrupt endpoints, then set this number to 2.'''
	usbHostHidClientDriverIntEPsNumber.setDescription(helpText)
	usbHostHidClientDriverIntEPsNumber.setVisible(True)
	usbHostHidClientDriverIntEPsNumber.setDefaultValue(1)
	
	# USB Host HID Client driver Global Push Pop Stack size
	usbHostHidClientDriverPushPopStackSize = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HID_GLOBAL_PUSH_POP_STACK_SIZE", None)
	usbHostHidClientDriverPushPopStackSize.setLabel("Number of PUSH items")
        helpText = '''Configure the size of the Global Item Stack. Each nested PUSH and POP item encountered 
        in the Report Descriptor requires space in the Global Item Stack. To configure this item, review the
        device HID report descriptor and identify the Push/Pop item nesting level and set this configuration
        item to match the item level. If the application support multiple HID devices, this number should be
        set to the maximum expected nesting level'''
	usbHostHidClientDriverPushPopStackSize.setDescription(helpText)
	usbHostHidClientDriverPushPopStackSize.setVisible(True)
	usbHostHidClientDriverPushPopStackSize.setDefaultValue(1)
	
	# USB Host HID Client driver Mouse 
	usbHostHidClientDriverMouse = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_USE_MOUSE", None)
	usbHostHidClientDriverMouse.setLabel("Use Mouse Driver")
        helpText = '''Enable this option to add the Mouse Usage Driver to the application. This allows the
        application to interact with a USB Mouse'''
	usbHostHidClientDriverMouse.setDescription(helpText)
	usbHostHidClientDriverMouse.setVisible(True)
	usbHostHidClientDriverMouse.setDefaultValue(False)
	
	# USB Host HID Client driver Mouse Buttons Number 
	usbHostHidClientDriverMouseButtonsNumber = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HOST_HID_MOUSE_BUTTONS_NUMBER", usbHostHidClientDriverMouse)
	usbHostHidClientDriverMouseButtonsNumber.setLabel("Number of Mouse buttons")
        helpText = '''Configure the maximum number of mouse button that the Mouse Usage Driver should 
        support. Additional mouse buttons will be ignored.'''
	usbHostHidClientDriverMouseButtonsNumber.setDescription(helpText)
	usbHostHidClientDriverMouseButtonsNumber.setVisible(False)
	usbHostHidClientDriverMouseButtonsNumber.setDefaultValue(5)
	usbHostHidClientDriverMouseButtonsNumber.setDependencies(setVisible, ["CONFIG_USB_HOST_USE_MOUSE"])
	
	# USB Host HID Client driver Mouse save status 
	usbHostHidClientDriverMouseSaveStatus = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_USE_MOUSE_SAVE_STATUS", usbHostHidClientDriverMouse)
	usbHostHidClientDriverMouseSaveStatus.setLabel("Number of Mouse Save Status")
	usbHostHidClientDriverMouseSaveStatus.setVisible(False)
	usbHostHidClientDriverMouseSaveStatus.setDefaultValue(False)
	
	# USB Host HID Client driver Keyboard 
	usbHostHidClientDriverKeyboard = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_USE_KEYBOARD", None)
	usbHostHidClientDriverKeyboard.setLabel("Use Keyboard Driver")
        helpText = '''Enable this option to add the Keyboard Usage Driver to the application. This allows the
        application to interact with a USB Keyboard'''
	usbHostHidClientDriverKeyboard.setDescription(helpText)
	usbHostHidClientDriverKeyboard.setVisible(True)
	usbHostHidClientDriverKeyboard.setDefaultValue(False)
	
	# USB Host HID Client driver Keyboard save status 
	usbHostHidClientDriverKeyboardSaveStatus = usbHostHidComponent.createBooleanSymbol("CONFIG_USB_HOST_USE_KEYBOARD_SAVE_STATUS", usbHostHidClientDriverKeyboard)
	usbHostHidClientDriverKeyboardSaveStatus.setLabel("Number of Keyboard Save Status")
	usbHostHidClientDriverKeyboardSaveStatus.setVisible(False)
	usbHostHidClientDriverKeyboardSaveStatus.setDefaultValue(False)
	
	# USB Host HID Client driver Total Usage Driver instances 
	usbHostHidClientDriverTotalUsageInst = usbHostHidComponent.createIntegerSymbol("CONFIG_USB_HID_TOTAL_USAGE_DRIVER_INSTANCES", None)
	usbHostHidClientDriverTotalUsageInst.setLabel("Number of Usage Driver instances")
	usbHostHidClientDriverTotalUsageInst.setDescription("Enter the number of total usage driver instances registered with HID client driver.")
	usbHostHidClientDriverTotalUsageInst.setVisible(True)
	usbHostHidClientDriverTotalUsageInst.setDefaultValue(0)
	usbHostHidClientDriverTotalUsageInst.setUseSingleDynamicValue(True)
	usbHostHidClientDriverTotalUsageInst.setDependencies(updateUsageDriverInstanceNumber, ["CONFIG_USB_HOST_USE_MOUSE","CONFIG_USB_HOST_USE_KEYBOARD"])
		
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
	usbHostHidHeaderFile = usbHostHidComponent.createFileSymbol("USB_HOST_HID_FILE_HEADER", None)
	addFileName('usb_host_hid.h', usbHostHidComponent, usbHostHidHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHidHeaderFile = usbHostHidComponent.createFileSymbol("USB_HID_FILE_HEADER", None)
	addFileName('usb_hid.h', usbHostHidComponent, usbHidHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHostHidSourceFile = usbHostHidComponent.createFileSymbol("USB_HOST_HID_FILE_SOURCE", None)
	addFileName('usb_host_hid.c', usbHostHidComponent, usbHostHidSourceFile, "middleware/src/", "/usb/src/", True, None)
	
	usbHostHidLocalHeaderFile = usbHostHidComponent.createFileSymbol("USB_HOST_HID_FILE_HEADER_LOCAL", None)
	addFileName('usb_host_hid_local.h', usbHostHidComponent, usbHostHidLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	usbHostHidKeyboardSourceFile = usbHostHidComponent.createFileSymbol("USB_HOST_HID_KEYBOARD_SOURCE", None)
	addFileName('usb_host_hid_keyboard.c', usbHostHidComponent, usbHostHidKeyboardSourceFile, "middleware/src/", "/usb/src/", usbHostHidClientDriverKeyboard.getValue(), keyBoardEnable)

	
	usbHostHidKeyboardHeaderFile = usbHostHidComponent.createFileSymbol("USB_HOST_HID_KEYBOARD_HEADER", None)
	addFileName('usb_host_hid_keyboard.h', usbHostHidComponent, usbHostHidKeyboardHeaderFile, "middleware/", "/usb/", usbHostHidClientDriverKeyboard.getValue(), keyBoardEnable)
	
	
	usbHostHidKeyboardLocalHeaderFile = usbHostHidComponent.createFileSymbol("USB_HOST_HID_KEYBOARD_HEADER_LOCAL", None)
	addFileName('usb_host_hid_keyboard_local.h', usbHostHidComponent, usbHostHidKeyboardLocalHeaderFile, "middleware/src/", "/usb/src", usbHostHidClientDriverKeyboard.getValue(), keyBoardEnable)
	
	usbHostHidMouseSourceFile = usbHostHidComponent.createFileSymbol("USB_HOST_HID_MOUSE_SOURCE", None)
	addFileName('usb_host_hid_mouse.c', usbHostHidComponent, usbHostHidMouseSourceFile, "middleware/src/", "/usb/src/", usbHostHidClientDriverMouse.getValue(), mouseEnable)
	
	usbHostHidMouseHeaderFile = usbHostHidComponent.createFileSymbol("USB_HOST_HID_MOUSE_HEADER", None)
	addFileName('usb_host_hid_mouse.h', usbHostHidComponent, usbHostHidMouseHeaderFile, "middleware/", "/usb/", usbHostHidClientDriverMouse.getValue(), mouseEnable)
	
	usbHostHidMouseLocalHeaderFile = usbHostHidComponent.createFileSymbol("USB_HOST_HID_MOUSE_HEADER_LOCAL", None)
	addFileName('usb_host_hid_mouse_local.h', usbHostHidComponent, usbHostHidMouseLocalHeaderFile, "middleware/src/", "/usb/src", usbHostHidClientDriverMouse.getValue(), mouseEnable)

	
	# all files go into src/
def addFileName(fileName, component, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
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
