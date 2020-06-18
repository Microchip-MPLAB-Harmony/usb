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

def blUSBDeviceFeatureEnableMicrosoftOsDescriptor(usbSymbolSource, event):
	if (event["value"] == True):		
		usbSymbolSource.setVisible(True)
	else:
		usbSymbolSource.setVisible(False)
			
def instantiateComponent(usbDeviceComponent):
	global usbDeviceInstnces
	global usbDeviceEp0BufferSize
	global usbDeviceEndpointsNumber
		
	configName = Variables.get("__CONFIGURATION_NAME")
	
	sourcePath = "templates/device/usbdevice_multi/"
	
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
	
		# USB Device events enable 
	usbDeviceEventsEnable = usbDeviceComponent.createMenuSymbol("USB_DEVICE_EVENTS", None)
	usbDeviceEventsEnable.setLabel("Special Events")	
	usbDeviceEventsEnable.setVisible(True)
	
	#SOF Event Enable 
	usbDeviceEventEnableSOF = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_EVENT_ENABLE_SOF", usbDeviceEventsEnable)
	usbDeviceEventEnableSOF.setLabel("Enable SOF Events")	
        helpText = '''Enable this option if Start of Frame event needs to be generated. If enabled the Device Layer will generate
        a Start of Frame event. In case of Full Speed USB, this event would be generated every 1 millisecond. In case of High
        Speed USB, this event would be generated every 125 microseconds.'''
        usbDeviceEventEnableSOF.setDescription(helpText)
	usbDeviceEventEnableSOF.setVisible(True)	
		
	#Set Descriptor Event Enable 
	usbDeviceEventEnableSetDescriptor = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_EVENT_ENABLE_SET_DESCRIPTOR", usbDeviceEventsEnable)
	usbDeviceEventEnableSetDescriptor.setLabel("Enable Set Descriptor Events")	
        helpText = '''Enable this option to support Set Descriptor requests. If enabled, the Device Layer will generate a
        Set Descriptor Event to the application when such a request is received from the host. If disabled, the Device Layer will Stall the
        Set Descritpor request.'''
        usbDeviceEventEnableSetDescriptor.setDescription(helpText)
	usbDeviceEventEnableSetDescriptor.setVisible(True)	
	
	#Synch Frame Event Enable 
	usbDeviceEventEnableSynchFrame = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_EVENT_ENABLE_SYNCH_FRAME", usbDeviceEventsEnable)
	usbDeviceEventEnableSynchFrame.setLabel("Enable Synch Frame Events")	
        helpText = '''Enable this option to support Sync Frame requests. If
        enabled, the Device Layer will generate a Sync Frame Event to the
        application when such a request is received from the host. If disabled,
        the Device Layer will Stall the Sync Frame request.'''
        usbDeviceEventEnableSynchFrame.setDescription(helpText)
	usbDeviceEventEnableSynchFrame.setVisible(True)	
	
	# USB Device Features enable 
	usbDeviceFeatureEnable = usbDeviceComponent.createMenuSymbol("USB_DEVICE_FEATURES", None)
	usbDeviceFeatureEnable.setLabel("Special Features")	
	usbDeviceFeatureEnable.setVisible(True)
	
	# Advanced string descriptor table enable 
	usbDeviceFeatureEnableAdvancedStringDescriptorTable = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FEATURE_ENABLE_ADVANCED_STRING_DESCRIPTOR_TABLE", usbDeviceFeatureEnable)
	usbDeviceFeatureEnableAdvancedStringDescriptorTable.setLabel("Enable advanced String Descriptor Table")
        helpText = '''Enable this option to use an Advanced String Descriptor
        Table. This table allows the string index of the string to be specified
        against the actual string. This option should be used when the Device
        Layer is expected to support String Descriptor requests which are not
        consecutive. Supporting these requests using a traditional string array
        would cause gaps in memory.'''
        usbDeviceFeatureEnableAdvancedStringDescriptorTable.setDescription(helpText)
	usbDeviceFeatureEnableAdvancedStringDescriptorTable.setVisible(True)
	
	#Microsoft OS descriptor support Enable 
	usbDeviceFeatureEnableMicrosoftOsDescriptor = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FEATURE_ENABLE_MICROSOFT_OS_DESCRIPTOR", usbDeviceFeatureEnableAdvancedStringDescriptorTable)
	usbDeviceFeatureEnableMicrosoftOsDescriptor.setLabel("Enable Microsoft OS Descriptor Support")	
        helpText = '''Enable this option to allow the Device Layer to
        automatically handle Microsoft OS Descriptor requests. Enabling this
        option will generate the OS String Descriptor with index 0xEE and
        update the Advanced String Descriptor table with this information. This
        option is typically used when creating Vendor Devices.'''
        usbDeviceFeatureEnableMicrosoftOsDescriptor.setDescription(helpText)
	usbDeviceFeatureEnableMicrosoftOsDescriptor.setVisible(False)	
	usbDeviceFeatureEnableMicrosoftOsDescriptor.setDependencies(blUSBDeviceFeatureEnableMicrosoftOsDescriptor, ["CONFIG_USB_DEVICE_FEATURE_ENABLE_ADVANCED_STRING_DESCRIPTOR_TABLE"])

	usbDeviceFeatureEnableMicrosoftOsDescriptorVendorCode =  usbDeviceComponent.createStringSymbol("CONFIG_USB_DEVICE_FEATURE_ENABLE_MICROSOFT_OS_DESCRIPTOR_VENDOR_CODE", usbDeviceFeatureEnableMicrosoftOsDescriptor)
	usbDeviceFeatureEnableMicrosoftOsDescriptorVendorCode.setLabel("Vendor Code")
	helpText = '''Vendor code is used to retrieve the associated feature 
				   descriptors. The vendor defines the format of this field.'''
	usbDeviceFeatureEnableMicrosoftOsDescriptorVendorCode.setDescription(helpText)
	usbDeviceFeatureEnableMicrosoftOsDescriptorVendorCode.setVisible(False)
	usbDeviceFeatureEnableMicrosoftOsDescriptorVendorCode.setDependencies(blUSBDeviceFeatureEnableMicrosoftOsDescriptor, ["CONFIG_USB_DEVICE_FEATURE_ENABLE_MICROSOFT_OS_DESCRIPTOR"])
	usbDeviceFeatureEnableMicrosoftOsDescriptorVendorCode.setDefaultValue("0xFF")
	#BOS descriptor support Enable 
	usbDeviceFeatureEnableBosDescriptor = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FEATURE_ENABLE_BOS_DESCRIPTOR", usbDeviceFeatureEnable)
	usbDeviceFeatureEnableBosDescriptor.setLabel("Enable BOS Descriptor Support")	
        helpText = '''Enable this option to allow the Device Layer to support
        BOS descriptor requests. If enabled, the Device Layer will forward the
        BOS Descriptor Request to the application. If not enabled, the Device
        Layer will Stall the request. The BOS Descriptor Request support is
        required when implementing Billboard Device Class support.'''
        usbDeviceFeatureEnableBosDescriptor.setDescription(helpText)
	usbDeviceFeatureEnableBosDescriptor.setVisible(True)	
	
	# Enable Auto timed remote wakeup functions  
	usbDeviceFeatureEnableAutioTimeRemoteWakeup = usbDeviceComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FEATURE_ENABLE_AUTO_TIMED_REMOTE_WAKEUP_FUNCTIONS", usbDeviceFeatureEnable)
	usbDeviceFeatureEnableAutioTimeRemoteWakeup.setLabel("Use Auto Timed Remote Wake up Functions")	
	usbDeviceFeatureEnableAutioTimeRemoteWakeup.setVisible(False)
	
	################################################
	# system_config.h file for USB Driver
	################################################
	usbDeviceSystemConfigFile = usbDeviceComponent.createFileSymbol("USB_DEVICE_SYSTEM_CONFIG_FILE", None)
	usbDeviceSystemConfigFile.setType("STRING")
	usbDeviceSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceSystemConfigFile.setSourcePath( sourcePath + "system_config.h.device_common.ftl")
	usbDeviceSystemConfigFile.setMarkup(True)
	
	

	
	
	