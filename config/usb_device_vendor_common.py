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
usbDeviceVendorInstnces = None 
usbDeviceVendorQueuDepth = None 


def handleMessage(messageID, args):	
	global usbDeviceVendorInstnces
	global usbDeviceVendorQueuDepth
	if (messageID == "UPDATE_VENDOR_INSTANCES"):
		usbDeviceVendorInstnces.setValue(args["vendorInstanceCount"])
	elif (messageID == "UPDATE_VENDOR_QUEUE_DEPTH_COMBINED"):
		usbDeviceVendorQueuDepth.setValue(args["vendorQueueDepth"])


def instantiateComponent(usbVendorComponentCommon):
	global usbDeviceVendorInstnces
	global usbDeviceVendorQueuDepth
	usbDeviceVendorInstnces = usbVendorComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_VENDOR_INSTANCES", None)
	usbDeviceVendorInstnces.setLabel("Number of Instances")
	usbDeviceVendorInstnces.setMin(1)
	usbDeviceVendorInstnces.setMax(10)
	usbDeviceVendorInstnces.setDefaultValue(1)
	usbDeviceVendorInstnces.setUseSingleDynamicValue(True)
	usbDeviceVendorInstnces.setVisible(False)
	
	usbDeviceVendorQueuDepth = usbVendorComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_VENDOR_QUEUE_DEPTH_COMBINED", None)
	usbDeviceVendorQueuDepth.setLabel("Combined Queue Depth")
	usbDeviceVendorQueuDepth.setMin(0)
	usbDeviceVendorQueuDepth.setMax(32767)
	usbDeviceVendorQueuDepth.setDefaultValue(0)
	usbDeviceVendorQueuDepth.setUseSingleDynamicValue(True)
	usbDeviceVendorQueuDepth.setVisible(False)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceVendorCommonSystemConfigFile = usbVendorComponentCommon.createFileSymbol(None, None)
	usbDeviceVendorCommonSystemConfigFile.setType("STRING")
	usbDeviceVendorCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceVendorCommonSystemConfigFile.setSourcePath("templates/device/vendor/system_config.h.device_vendor_common.ftl")
	usbDeviceVendorCommonSystemConfigFile.setMarkup(True)
	
	##############################################################
	# system_definitions.h file for USB Device Vendor Function driver   
	##############################################################
	usbDeviceVendorCommonSystemDefFile = usbVendorComponentCommon.createFileSymbol(None, None)
	usbDeviceVendorCommonSystemDefFile.setType("STRING")
	usbDeviceVendorCommonSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceVendorCommonSystemDefFile.setSourcePath("templates/device/vendor/system_definitions.h.device_vendor_includes.ftl")
	usbDeviceVendorCommonSystemDefFile.setMarkup(True)