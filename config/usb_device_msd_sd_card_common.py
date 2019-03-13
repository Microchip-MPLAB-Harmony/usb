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
def instantiateComponent(usbCdcComponentCommon):
	print ("usb_device_cdc_common.py")
	usbDeviceCdcInstnces = usbCdcComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_CDC_INSTANCES", None)
	usbDeviceCdcInstnces.setLabel("Number of Instances")
	usbDeviceCdcInstnces.setMin(1)
	usbDeviceCdcInstnces.setMax(10)
	usbDeviceCdcInstnces.setDefaultValue(1)
	usbDeviceCdcInstnces.setVisible(False)
	
	usbDeviceCdcQueuDepth = usbCdcComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED", None)
	usbDeviceCdcQueuDepth.setLabel("Combined Queue Depth")
	usbDeviceCdcQueuDepth.setMin(1)
	usbDeviceCdcQueuDepth.setMax(32767)
	usbDeviceCdcQueuDepth.setDefaultValue(3)
	usbDeviceCdcQueuDepth.setVisible(False)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceCdcCommonSystemConfigFile = usbCdcComponentCommon.createFileSymbol(None, None)
	usbDeviceCdcCommonSystemConfigFile.setType("STRING")
	usbDeviceCdcCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceCdcCommonSystemConfigFile.setSourcePath("templates/cdc/system_config.h.device_cdc_common.ftl")
	usbDeviceCdcCommonSystemConfigFile.setMarkup(True)