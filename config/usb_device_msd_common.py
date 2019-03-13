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
usbDeviceMsdMaxNumberofSectors = ["1", "2", "4", "8"]	

def instantiateComponent(usbMsdComponentCommon):
	print ("usb_device_msd_common.py")
	usbDeviceMsdInstnces = usbMsdComponentCommon.createIntegerSymbol("CONFIG_USB_DEVICE_MSD_INSTANCES", None)
	usbDeviceMsdInstnces.setLabel("Number of Instances")
	usbDeviceMsdInstnces.setMin(1)
	usbDeviceMsdInstnces.setMax(10)
	usbDeviceMsdInstnces.setDefaultValue(1)
	usbDeviceMsdInstnces.setVisible(False)
	
	usbDeviceMsdLunNumber = usbMsdComponentCommon.createIntegerSymbol("USB_DEVICE_MSD_LUNS_NUMBER", None)
	usbDeviceMsdLunNumber.setLabel("Combined Queue Depth")
	usbDeviceMsdLunNumber.setMin(1)
	usbDeviceMsdLunNumber.setMax(32767)
	usbDeviceMsdLunNumber.setDefaultValue(3)
	usbDeviceMsdLunNumber.setVisible(False)
	
	usbDeviceMsdMaxSectorsToBufferCommon = usbMsdComponentCommon.createComboSymbol("CONFIG_USB_DEVICE_FUNCTION_MSD_MAX_SECTORS_COMMON", None, usbDeviceMsdMaxNumberofSectors)
	usbDeviceMsdMaxSectorsToBufferCommon.setLabel("Max Sectors To Buffer")
	usbDeviceMsdMaxSectorsToBufferCommon.setDefaultValue("1")
	usbDeviceMsdMaxSectorsToBufferCommon.setVisible(True)
	
	#########################################################
	# system_config.h file for USB Device MSD function driver 
	#########################################################
	usbDeviceMsdCommonSystemConfigFile = usbMsdComponentCommon.createFileSymbol(None, None)
	usbDeviceMsdCommonSystemConfigFile.setType("STRING")
	usbDeviceMsdCommonSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceMsdCommonSystemConfigFile.setSourcePath("templates/device/msd/system_config.h.device_msd_common.ftl")
	usbDeviceMsdCommonSystemConfigFile.setMarkup(True)
	
	