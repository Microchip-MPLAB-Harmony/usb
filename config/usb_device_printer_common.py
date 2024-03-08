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

usbDevicePrinterInstnces = None 
usbDevicePrinterQueuDepth = None 

def handleMessage(messageID, args):	
	global usbDevicePrinterInstnces
	global usbDevicePrinterQueuDepth
	if (messageID == "UPDATE_PRINTER_INSTANCES"):
		usbDevicePrinterInstnces.setValue(args["printerInstanceCount"])
	elif (messageID == "UPDATE_PRINTER_QUEUE_DEPTH_COMBINED"):
		usbDevicePrinterQueuDepth.setValue(args["printerQueueDepth"])


def instantiateComponent(usbPrinterComponentCommon):

	global usbDevicePrinterInstnces
	global usbDevicePrinterQueuDepth 
	usbDevicePrinterInstnces = usbPrinterComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_PRINTER_INSTANCES", None)
	usbDevicePrinterInstnces.setLabel("Number of Instances")
	usbDevicePrinterInstnces.setMin(1)
	usbDevicePrinterInstnces.setDefaultValue(1)
	usbDevicePrinterInstnces.setUseSingleDynamicValue(True)
	usbDevicePrinterInstnces.setVisible(False)
	
	usbDevicePrinterQueuDepth = usbPrinterComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED", None)
	usbDevicePrinterQueuDepth.setLabel("Combined Queue Depth")
	usbDevicePrinterQueuDepth.setMin(1)
	usbDevicePrinterQueuDepth.setMax(32767)
	usbDevicePrinterQueuDepth.setDefaultValue(2)
	usbDevicePrinterQueuDepth.setUseSingleDynamicValue(True)
	usbDevicePrinterQueuDepth.setVisible(False)

	# Printer Function device id string 
	deviceIdString = usbPrinterComponentCommon.createStringSymbol("CONFIG_USB_PRINTER_DEVICE_ID_STRING", None)
	deviceIdString.setLabel("Device ID String compatible with IEEE 1284")
	deviceIdString.setVisible(True)
	deviceIdString.setDefaultValue("MFG:Microchip;MDL:Generic;CMD:EPSON;CLS:PRINTER;DES:GenericTextOnlyPrinterDemo;")
	deviceIdString.setHelp("mcc_configuration_device_printer")

	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDevicePrinterCommonSystemConfigFile = usbPrinterComponentCommon.createFileSymbol(None, None)
	usbDevicePrinterCommonSystemConfigFile.setType("STRING")
	usbDevicePrinterCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDevicePrinterCommonSystemConfigFile.setSourcePath("templates/device/printer/system_config.h.device_printer_common.ftl")
	usbDevicePrinterCommonSystemConfigFile.setMarkup(True)
	
	###################################################################
	# system_definitions.h file for USB Device Printer Function driver   
	###################################################################
	usbDevicePrinterCommonSystemDefFile = usbPrinterComponentCommon.createFileSymbol(None, None)
	usbDevicePrinterCommonSystemDefFile.setType("STRING")
	usbDevicePrinterCommonSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDevicePrinterCommonSystemDefFile.setSourcePath("templates/device/printer/system_definitions.h.device_printer_includes.ftl")
	usbDevicePrinterCommonSystemDefFile.setMarkup(True)