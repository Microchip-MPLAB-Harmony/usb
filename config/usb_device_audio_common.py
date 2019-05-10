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
usbDeviceAudioInstnces = None 
usbDeviceAudioQueuDepth = None 

def handleMessage(messageID, args):	
	global usbDeviceAudioInstnces
	global usbDeviceAudioQueuDepth
	if (messageID == "UPDATE_AUDIO_INSTANCES"):
		usbDeviceAudioInstnces.setValue(args["audioInstanceCount"])
	elif (messageID == "UPDATE_AUDIO_QUEUE_DEPTH_COMBINED"):
		usbDeviceAudioQueuDepth.setValue(args["audioQueueDepth"])
		

def instantiateComponent(usbAudioComponentCommon):
	global usbDeviceAudioInstnces 
	global usbDeviceAudioQueuDepth 
	usbDeviceAudioInstnces = usbAudioComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_AUDIO_INSTANCES", None)
	usbDeviceAudioInstnces.setLabel("Number of Instances")
	usbDeviceAudioInstnces.setMin(1)
	usbDeviceAudioInstnces.setMax(10)
	usbDeviceAudioInstnces.setDefaultValue(1)
	usbDeviceAudioInstnces.setUseSingleDynamicValue(True)
	usbDeviceAudioInstnces.setVisible(False)
	
	usbDeviceAudioQueuDepth = usbAudioComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED", None)
	usbDeviceAudioQueuDepth.setLabel("Combined Queue Depth")
	usbDeviceAudioQueuDepth.setMin(0)
	usbDeviceAudioQueuDepth.setMax(32767)
	usbDeviceAudioQueuDepth.setDefaultValue(3)
	usbDeviceAudioQueuDepth.setUseSingleDynamicValue(True)
	usbDeviceAudioQueuDepth.setVisible(False)
	
	
	usbDeviceAudioStrmInterfaceMax = usbAudioComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_STREAMING_INTERFACES_NUMBER_COMBINED", None)
	usbDeviceAudioStrmInterfaceMax.setMin(0)
	usbDeviceAudioStrmInterfaceMax.setMax(32767)
	usbDeviceAudioStrmInterfaceMax.setDefaultValue(1)
	usbDeviceAudioStrmInterfaceMax.setUseSingleDynamicValue(True)
	usbDeviceAudioStrmInterfaceMax.setVisible(False)
	
	
	usbDeviceAudioAltInterfaceMax = usbAudioComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_MAX_ALTERNATE_SETTING_COMBINED", None)
	usbDeviceAudioAltInterfaceMax.setMin(0)
	usbDeviceAudioAltInterfaceMax.setMax(32767)
	usbDeviceAudioAltInterfaceMax.setDefaultValue(2)
	usbDeviceAudioAltInterfaceMax.setUseSingleDynamicValue(True)
	usbDeviceAudioAltInterfaceMax.setVisible(False)
	
	################################################
	# system_config.h file for USB Device stack    
	################################################
	usbDeviceAudioCommonSystemConfigFile = usbAudioComponentCommon.createFileSymbol(None, None)
	usbDeviceAudioCommonSystemConfigFile.setType("STRING")
	usbDeviceAudioCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceAudioCommonSystemConfigFile.setSourcePath("templates/device/audio/system_config.h.device_audio_common.ftl")
	usbDeviceAudioCommonSystemConfigFile.setMarkup(True)
