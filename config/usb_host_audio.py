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
def onAttachmentConnected(source, target):
	ownerComponent = source["component"]
	print("USB HOST Audio Client Driver: USB Host Layer Connected")
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		args = {"nTpl":readValue + 1 }
		res = Database.sendMessage("usb_host", "UPDATE_TPL_ENTRY_NUMBER", args)
	
def onAttachmentDisconnected(source, target):
	ownerComponent = source["component"]
	print("USB HOST Audio Client Driver: USB Host Layer Disconnected")
	readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
	if readValue != None:
		args = {"nTpl":readValue - 1 }
		res = Database.sendMessage("usb_host", "UPDATE_TPL_ENTRY_NUMBER", args)
		
def destroyComponent(component):	
	print("USB HOST Audio Client Driver: Destroyed")
	
def instantiateComponent(usbHostAudioComponent):

	res = Database.activateComponents(["usb_host"])
	
	# USB Host Audio client driver instances 
	usbHostAudioClientDriverInstance = usbHostAudioComponent.createIntegerSymbol("CONFIG_USB_HOST_AUDIO_NUMBER_OF_INSTANCES", None)
	usbHostAudioClientDriverInstance.setLabel("Number of Audio Host Driver Instances")
	usbHostAudioClientDriverInstance.setDescription("Enter the number of Audio Class Driver instances required in the application.")
	usbHostAudioClientDriverInstance.setDefaultValue(1)
	usbHostAudioClientDriverInstance.setMin(1)
	usbHostAudioClientDriverInstance.setVisible(True)

	
	#USB Host Audio Attach Listeners Number 
	# usbHostAudioClientDriverAttachListnerNumber = usbHostAudioComponent.createIntegerSymbol("CONFIG_USB_HOST_AUDIO_ATTACH_LISTENERS_NUMBER", None)
	# usbHostAudioClientDriverAttachListnerNumber.setLabel("Number of Audio Host Attach Listeners")
	# usbHostAudioClientDriverAttachListnerNumber.setDescription("Enter the number of Audio Attach Listeners required in the application.")
	# usbHostAudioClientDriverAttachListnerNumber.setDefaultValue(1)
	# usbHostAudioClientDriverAttachListnerNumber.setVisible(True)

	# Number of Audio streaming interfaces in the attached USB Audio device 
	usbHostAudioStreamingInterfaceNumbers =  usbHostAudioComponent.createIntegerSymbol("USB_HOST_AUDIO_NUMBER_OF_STREAMING_INTERFACES", None)
	usbHostAudioStreamingInterfaceNumbers.setLabel("Number of Audio Streaming Interfaces")
	usbHostAudioStreamingInterfaceNumbers.setDefaultValue(2)
	helpText = '''Enter the maximum number of Audio streaming interfaces that an 
				  attached USB Audio device would have. E.g., A USB Headset will 
				  usually have Two Audio streaming interfaces. So this value 
				  should be set to Two for Audio Headset.'''
	usbHostAudioStreamingInterfaceNumbers.setDescription(helpText)
	usbHostAudioStreamingInterfaceNumbers.setMin(1)
	usbHostAudioStreamingInterfaceNumbers.setVisible(True)
    
	# Number of the Audio Streaming interface alternate settings 
	usbHostAudioStreamingInterfaceSettingNumbers =  usbHostAudioComponent.createIntegerSymbol("USB_HOST_AUDIO_NUMBER_OF_STREAMING_INTERFACE_SETTINGS", None)
	usbHostAudioStreamingInterfaceSettingNumbers.setLabel("Number of Audio Streaming Interface Settings")
	usbHostAudioStreamingInterfaceSettingNumbers.setDefaultValue(2)
	helpText = '''Enter the number of Audio streaming interface settings. This
				  value should include the default Interface setting as well. 
				  Most standard USB Audio devices will have two interface 
				  settings. However, USB Audio devices are available with 
				  multiple interface settings.'''
	usbHostAudioStreamingInterfaceSettingNumbers.setDescription(helpText)
	usbHostAudioStreamingInterfaceSettingNumbers.setMin(2)
	usbHostAudioStreamingInterfaceSettingNumbers.setVisible(True)
	
	# Number of sampling frequencies 
	usbHostAudioSamplingFrequenciesNumber =  usbHostAudioComponent.createIntegerSymbol("USB_HOST_AUDIO_NUMBER_OF_SAMPLING_FREQUENCIES", None)
	usbHostAudioSamplingFrequenciesNumber.setLabel("Number of Sampling Frequencies supported")
	usbHostAudioSamplingFrequenciesNumber.setDefaultValue(3)
	helpText = '''Enter the number of discrete Sampling frequencies supported by 
				  the attached device.'''
	usbHostAudioSamplingFrequenciesNumber.setDescription(helpText)
	usbHostAudioSamplingFrequenciesNumber.setMin(1)
	usbHostAudioSamplingFrequenciesNumber.setVisible(True)

	
	##############################################################
	# system_definitions.h file for USB Host Audio Client driver   
	##############################################################
	usbHostAudioSystemDefFile = usbHostAudioComponent.createFileSymbol(None, None)
	usbHostAudioSystemDefFile.setType("STRING")
	usbHostAudioSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbHostAudioSystemDefFile.setSourcePath("templates/host/system_definitions.h.host_audio_includes.ftl")
	usbHostAudioSystemDefFile.setMarkup(True)
	
	##############################################################
	# system_config.h file for USB Host Audio Client driver   
	##############################################################
	usbHostAudioSystemConfigFile = usbHostAudioComponent.createFileSymbol(None, None)
	usbHostAudioSystemConfigFile.setType("STRING")
	usbHostAudioSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbHostAudioSystemConfigFile.setSourcePath("templates/host/system_config.h.host_audio.ftl")
	usbHostAudioSystemConfigFile.setMarkup(True)
	
	##############################################################
	# TPL Entry for Audio client driver 
	##############################################################
	usbHostAudioTplEntryFile = usbHostAudioComponent.createFileSymbol(None, None)
	usbHostAudioTplEntryFile.setType("STRING")
	usbHostAudioTplEntryFile.setOutputName("usb_host.LIST_USB_HOST_TPL_ENTRY")
	usbHostAudioTplEntryFile.setSourcePath("templates/host/system_init_c_audio_tpl.ftl")
	usbHostAudioTplEntryFile.setMarkup(True)
	
	################################################
	# USB Host Audio Client driver Files 
	################################################
	usbHostAudioHeaderFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_host_audio_v1_0.h', usbHostAudioComponent, usbHostAudioHeaderFile, "middleware/", "/usb/", True, None)
	
	usbAudioHeaderFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_audio_v1_0.h', usbHostAudioComponent, usbAudioHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHostAudioSourceFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_host_audio_v1_0.c', usbHostAudioComponent, usbHostAudioSourceFile, "middleware/src/", "/usb/src/", True, None)
		
	usbHostAudioLocalHeaderFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_host_audio_local.h', usbHostAudioComponent, usbHostAudioLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	usbHostAudioLocalHeaderFile = usbHostAudioComponent.createFileSymbol(None, None)
	addFileName('usb_host_audio_v1_mapping.h', usbHostAudioComponent, usbHostAudioLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	
	# all files go into src/
def addFileName(fileName, component, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
	#filename = component.createFileSymbol(None, None)
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
		symbol.setDependencies(callback, ["USB_DEVICE_FUNCTION_1_DEVICE_CLASS"])