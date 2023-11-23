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
def destroyComponent(component):
	print("USB HOST Generic Client Driver: Destroyed")
	
def instantiateComponent(usbHostGenericComponent):

	res = Database.activateComponents(["usb_host"])

	# Vendor ID
	usbHostGenericClientDriverVendorId = usbHostGenericComponent.createStringSymbol("CONFIG_USB_HOST_GENERIC_VENDOR_ID", None)
	usbHostGenericClientDriverVendorId.setLabel("Vendor ID")
        helpText = '''Specify the Vendor ID here with 0x Prefix. e.g - 0x04D8'''
	usbHostGenericClientDriverVendorId.setDescription(helpText)
	usbHostGenericClientDriverVendorId.setDefaultValue("0x04D8")
	usbHostGenericClientDriverVendorId.setVisible(True)
	
	# Product ID 
	usbHostGenericClientDriverProductId = usbHostGenericComponent.createStringSymbol("CONFIG_USB_HOST_GENERIC_PRODUCT_ID", None)
	usbHostGenericClientDriverProductId.setLabel("Product ID")
        helpText = '''Specify the Product ID here with 0x Prefix. e.g - 0x0053'''
	usbHostGenericClientDriverProductId.setDescription(helpText)
	usbHostGenericClientDriverProductId.setDefaultValue("0x0053")
	usbHostGenericClientDriverProductId.setVisible(True)

	##############################################################
	# system_definitions.h file for USB Host Generic Client driver   
	##############################################################
	usbHostGenericSystemDefFile = usbHostGenericComponent.createFileSymbol(None, None)
	usbHostGenericSystemDefFile.setType("STRING")
	usbHostGenericSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbHostGenericSystemDefFile.setSourcePath("templates/host/system_definitions.h.host_generic_includes.ftl")
	usbHostGenericSystemDefFile.setMarkup(True)

	
	##############################################################
	# TPL Entry for Generic client driver 
	##############################################################
	usbHostGenericTplEntryFile = usbHostGenericComponent.createFileSymbol(None, None)
	usbHostGenericTplEntryFile.setType("STRING")
	usbHostGenericTplEntryFile.setOutputName("usb_host.LIST_USB_HOST_TPL_ENTRY")
	usbHostGenericTplEntryFile.setSourcePath("templates/host/system_init_c_generic_tpl.ftl")
	usbHostGenericTplEntryFile.setMarkup(True)
	
	################################################
	# USB Host Generic Client driver Files 
	################################################
	usbHostGenericHeaderFile = usbHostGenericComponent.createFileSymbol(None, None)
	addFileName('usb_host_generic.h', usbHostGenericComponent, usbHostGenericHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHostGenericSourceFile = usbHostGenericComponent.createFileSymbol(None, None)
	addFileName('usb_host_generic.c', usbHostGenericComponent, usbHostGenericSourceFile, "middleware/src/", "/usb/src/", True, None)
	
	usbHostGenericLocalHeaderFile = usbHostGenericComponent.createFileSymbol(None, None)
	addFileName('usb_host_generic_local.h', usbHostGenericComponent, usbHostGenericLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	
	# all files go into src/
def addFileName(fileName, component, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
	#filename = component.createFileSymbol(None, None)
	symbol.setProjectPath("config/" + configName1 + destPath)
	symbol.setSourcePath(srcPath + fileName + ".ftl")
	symbol.setMarkup(True)
	symbol.setOutputName(fileName)
	symbol.setDestPath(destPath)
	if fileName[-2:] == '.h':
		symbol.setType("HEADER")
	else:
		symbol.setType("SOURCE")
	symbol.setEnabled(enabled)
	if callback != None:
		symbol.setDependencies(callback, ["USB_DEVICE_FUNCTION_1_DEVICE_CLASS"])
