"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
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
# Global definitions  
usbDebugLogs = 1 

usbDeviceFunctionInitEntry = None
usbDeviceEp0BufferSize = None
usbDeviceInstnces = None
usbDeviceEndpointsNumber = None
numEndpoints = None
usbDeviceEp0BufferSizes = ["64", "32", "16", "8"]


def handleMessage(messageID, args):	
	global usbPeripheralHostSupport
	global usbPeripheralDeviceSupport
	global numEndpoints
	if (messageID == "UPDATE_OPERATION_MODE_COMMON"):
		opModeId0 = Database.getSymbolValue("drv_usbfs_0", "USB_OPERATION_MODE")
		opModeId1 = Database.getSymbolValue("drv_usbfs_1", "USB_OPERATION_MODE")
		
		if ((opModeId0 != None) and  (opModeId0 == "Host")) or ((opModeId1 != None) and  (opModeId1 == "Host")):
			usbPeripheralHostSupport.setValue(True)
		else:	
			usbPeripheralHostSupport.setValue(False)
		if ((opModeId0 != None) and  (opModeId0 == "Device")) or ((opModeId1 != None) and  (opModeId1 == "Device")):
			usbPeripheralDeviceSupport.setValue(True)
		else:	
			usbPeripheralDeviceSupport.setValue(False)
			
		if (messageID == "UPDATE_ENDPOINTS_NUMBER"):
			numEndpoints =  usbDeviceEndpointsNumber.getValue()	
			usbDeviceEndpointsNumber.setValue( numEndpoints + args["nFunction"])
			numEndpoints =  usbDeviceEndpointsNumber.getValue()
			
		
			


def instantiateComponent(usbDeviceComponent):
	global usbDeviceInstnces
	global usbDeviceEp0BufferSize
	global usbDeviceEndpointsNumber
		
	configName = Variables.get("__CONFIGURATION_NAME")
	
	sourcePath = "templates/device/usbdevice_multi/"
	
	usbDeviceInstnces = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_LAYER_INSTANCES", None)
	usbDeviceInstnces.setLabel("Number of Instances")
	usbDeviceInstnces.setMin(1)
	usbDeviceInstnces.setMax(2)
	usbDeviceInstnces.setDefaultValue(1)
	usbDeviceInstnces.setUseSingleDynamicValue(True)
	usbDeviceInstnces.setVisible(True)
	
	# USB Device Endpoint Number 
	usbDeviceEndpointsNumber = usbDeviceComponent.createIntegerSymbol("CONFIG_USB_DEVICE_ENDPOINTS_NUMBER", None)
	usbDeviceEndpointsNumber.setLabel("Number of Endpoints")	
	usbDeviceEndpointsNumber.setVisible(False)
	usbDeviceEndpointsNumber.setMin(0)
	usbDeviceEndpointsNumber.setDefaultValue(0)
	usbDeviceEndpointsNumber.setUseSingleDynamicValue(True)
	usbDeviceEndpointsNumber.setReadOnly(True)
	
	# USB Device EP0 Buffer Size  
	usbDeviceEp0BufferSize = usbDeviceComponent.createComboSymbol("CONFIG_USB_DEVICE_EP0_BUFFER_SIZE", None, usbDeviceEp0BufferSizes)
	usbDeviceEp0BufferSize.setLabel("Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setVisible(True)
	usbDeviceEp0BufferSize.setDescription("Select Endpoint 0 Buffer Size")
	usbDeviceEp0BufferSize.setDefaultValue("64")
	
	
	################################################
	# system_definitions.h file for USB Device Layer    
	################################################
	#usbDeviceSystemDefFile = usbDeviceComponent.createFileSymbol(None, None)
	#usbDeviceSystemDefFile.setType("STRING")
	#usbDeviceSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	#usbDeviceSystemDefFile.setSourcePath( sourcePath + "system_definitions.h.device_includes.ftl")
	#usbDeviceSystemDefFile.setMarkup(True)
	
	################################################
	# system_config.h file for USB Driver
	################################################
	usbDeviceSystemConfigFile = usbDeviceComponent.createFileSymbol(None, None)
	usbDeviceSystemConfigFile.setType("STRING")
	usbDeviceSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceSystemConfigFile.setSourcePath( sourcePath + "system_config.h.device_common.ftl")
	usbDeviceSystemConfigFile.setMarkup(True)
	

	
	
	