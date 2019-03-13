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
def instantiateComponent(usbHidComponentCommon):
	print ("usb_device_hid_common.py")
	usbDeviceHidInstnces = usbHidComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_HID_INSTANCES", None)
	usbDeviceHidInstnces.setLabel("Number of Instances")
	usbDeviceHidInstnces.setMin(1)
	usbDeviceHidInstnces.setMax(10)
	usbDeviceHidInstnces.setDefaultValue(1)
	usbDeviceHidInstnces.setVisible(False)
	
	usbDeviceHidQueuDepth = usbHidComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_HID_QUEUE_DEPTH_COMBINED", None)
	usbDeviceHidQueuDepth.setLabel("Combined Queue Depth")
	usbDeviceHidQueuDepth.setMin(1)
	usbDeviceHidQueuDepth.setMax(32767)
	usbDeviceHidQueuDepth.setDefaultValue(3)
	usbDeviceHidQueuDepth.setUseSingleDynamicValue(True)
	usbDeviceHidQueuDepth.setVisible(False)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceHidCommonSystemConfigFile = usbHidComponentCommon.createFileSymbol(None, None)
	usbDeviceHidCommonSystemConfigFile.setType("STRING")
	usbDeviceHidCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceHidCommonSystemConfigFile.setSourcePath("templates/device/hid/system_config.h.device_hid_common.ftl")
	usbDeviceHidCommonSystemConfigFile.setMarkup(True)
	
	##############################################################
	# system_definitions.h file for USB Device HID Function driver   
	##############################################################
	usbDeviceHidSystemDefFile = usbHidComponentCommon.createFileSymbol(None, None)
	usbDeviceHidSystemDefFile.setType("STRING")
	usbDeviceHidSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceHidSystemDefFile.setSourcePath("templates/device/hid/system_definitions.h.device_hid_includes.ftl")
	usbDeviceHidSystemDefFile.setMarkup(True)