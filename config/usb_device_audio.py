"""*****************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
indexFunction = None
configValue = None
startInterfaceNumber = None
numberOfInterfaces = None
queueSizeRead = None
queueSizeWrite = None
epNumberIn = None
epNumberOut = None
currentAudioDescriptorSize = None 
audioDeviceType = None 


	
audioVersion =["Audio v1", "Audio v2"]
audioDeviceTypes = [
"Audio v1.0 USB Speaker",
"Audio v1.0 USB Microphone",
"Audio v1.0 USB Headset",
"Audio v2.0 USB Speaker" 
]


def onAttachmentConnected(source, target):
	print ("Audio Function Driver: Attached")
	
	global indexFunction
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global queueSizeRead
	global queueSizeWrite
	global epNumberIn
	global epNumberOut
	global currentAudioDescriptorSize
	global currentAudioNumberOfInterfaces
	global audioDeviceType
	
	ownerComponent = source["component"]
	dependencyID = source["id"]
	if (dependencyID == "usb_device_dependency"):
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		if readValue != None: 
			args = {"nFunction":readValue + 1}
			res = Database.sendMessage("usb_device", "UPDATE_FUNCTIONS_NUMBER", args)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if readValue != None: 
			args = {"nFunction": readValue + currentAudioDescriptorSize.getValue()}
			res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if readValue != None: 
			args = {"nFunction":  readValue + numberOfInterfaces.getValue()}
			res = Database.sendMessage("usb_device", "UPDATE_INTERFACES_NUMBER", args)
			startInterfaceNumber.setValue(readValue, 1)
		
		nEndpoints = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if nEndpoints != None:
			if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MM", "PIC32MK", "PIC32CM", "SAMD21", "SAMDA1","SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255" ,"SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "SAMD11", "WFI32E01", "PIC32CX"]):
				epNumberIn.setValue(nEndpoints + 1)
				epNumberOut.setValue(nEndpoints + 1)
				args = {"nFunction":  nEndpoints + 1}
			elif any(x in Variables.get("__PROCESSOR") for x in ["SAME70", "SAMV70", "SAMV71", "SAMS70", "PIC32CZ"]):
				if (audioDeviceType.getValue() == "Audio v2.0 USB Speaker") \
				or (audioDeviceType.getValue() == "Audio v1.0 USB Speaker") \
				or (audioDeviceType.getValue() == "Audio v1.0 USB Headset") \
				or (audioDeviceType.getValue() == "Audio v1.0 USB Headset Multi Sampling rates") :	
					epNumberOut.setValue(nEndpoints + 1)
					epNumberIn.setValue(nEndpoints + 2)
				elif (audioDeviceType.getValue() == "Audio v1.0 USB Microphone"):	
					epNumberIn.setValue(nEndpoints + 1)	
					epNumberOut.setValue(nEndpoints + 2)
				args = {"nFunction":  nEndpoints + 2}
			res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)	
	

def onAttachmentDisconnected(source, target):

	print ("Audio Function Driver: Detached")
	
	global indexFunction
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global queueSizeRead
	global queueSizeWrite
	global epNumberIn
	global epNumberOut
	global currentAudioDescriptorSize
	global currentAudioNumberOfInterfaces
	global audioDeviceType
	
	ownerComponent = source["component"]
	dependencyID = source["id"]
	if (dependencyID == "usb_device_dependency"):
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		if readValue != None: 
			args = {"nFunction":readValue - 1}
			res = Database.sendMessage("usb_device", "UPDATE_FUNCTIONS_NUMBER", args)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if readValue != None: 
			args = {"nFunction": readValue - currentAudioDescriptorSize.getValue()}
			res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if readValue != None: 
			args = {"nFunction":  readValue - numberOfInterfaces.getValue()}
			res = Database.sendMessage("usb_device", "UPDATE_INTERFACES_NUMBER", args)
		
		nEndpoints = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if nEndpoints != None:
			if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MZ", "PIC32MX", "PIC32MM", "PIC32MK", "PIC32CM", "SAMD21", "SAMDA1","SAMD51", "SAME51", "SAME53", "SAME54", "LAN9255", "SAML21", "SAML22", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "SAMD11", "WFI32E01", "PIC32CX"]):
				epNumberIn.setValue(1)
				epNumberOut.setValue(1)
				args = {"nFunction":  nEndpoints - 1}
			elif any(x in Variables.get("__PROCESSOR") for x in ["SAME70", "SAMV70", "SAMV71", "SAMS70", "PIC32CZ"]):
				if (audioDeviceType.getValue() == "Audio v2.0 USB Speaker") \
				or (audioDeviceType.getValue() == "Audio v1.0 USB Speaker") \
				or (audioDeviceType.getValue() == "Audio v1.0 USB Headset") \
				or (audioDeviceType.getValue() == "Audio v1.0 USB Headset Multi Sampling rates") :		
					epNumberOut.setValue(1)
					epNumberIn.setValue(2)
				elif (audioDeviceType.getValue() == "Audio v1.0 USB Microphone"):	
					epNumberIn.setValue(1)	
					epNumberOut.setValue(2)
				args = {"nFunction":  nEndpoints - 2}
			res = Database.sendMessage("usb_device", "UPDATE_ENDPOINTS_NUMBER", args)
			
			
def destroyComponent(component):
	print ("Audio Function Driver: Destroyed")
	
def updateConfigurationDescriptorSize(descriptorSizeOld):
	global currentAudioDescriptorSize
	readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
	if (readValue != None):
		args = {"nFunction": readValue + currentAudioDescriptorSize.getValue() - descriptorSizeOld}
		res = Database.sendMessage("usb_device", "UPDATE_CONFIG_DESCRPTR_SIZE", args)

def updateNumberOfInterfaces(numberOfInterfacesOld, nInterfacesNew):
	readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
	if (readValue != None):
		args = {"nFunction":   readValue + nInterfacesNew - numberOfInterfacesOld}
		res = Database.sendMessage("usb_device", "UPDATE_INTERFACES_NUMBER", args)
	
def usbDeviceAudioSpecVersionChanged(usbSymbolSource, event):
	if event["value"] == "Audio v1":
		if usbSymbolSource.getID() == "USB_DEVICE_AUDIO_HEADER_FILE":
			addFileName('usb_device_audio_v1_0.h', usbSymbolSource, "middleware/", "/usb/", True, None)
		elif usbSymbolSource.getID() == "USB_AUDIO_HEADER_FILE":
			addFileName('usb_audio_v1_0.h', usbSymbolSource, "middleware/", "/usb/", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_SOURCE_FILE":
			addFileName('usb_device_audio_v1_0.c', usbSymbolSource, "middleware/src/", "/usb/src", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_TRANSFER_SOURCE_FILE":
			addFileName('usb_device_audio_read_write.c', usbSymbolSource, "middleware/src/", "/usb/src", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_LOCAL_HEADER_FILE":
			addFileName('usb_device_audio_local.h', usbSymbolSource, "middleware/src/", "/usb/src", True, None)
	elif event["value"] == "Audio v2":	
		if usbSymbolSource.getID() == "USB_DEVICE_AUDIO_HEADER_FILE":
			addFileName('usb_device_audio_v2_0.h', usbSymbolSource, "middleware/", "/usb/", True, None)
		elif usbSymbolSource.getID() == "USB_AUDIO_HEADER_FILE":
			addFileName('usb_audio_v2_0.h', usbSymbolSource, "middleware/", "/usb/", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_SOURCE_FILE":
			addFileName('usb_device_audio_v2_0.c', usbSymbolSource, "middleware/src/", "/usb/src", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_TRANSFER_SOURCE_FILE":
			addFileName('usb_device_audio_v2_read_write.c', usbSymbolSource, "middleware/src/", "/usb/src", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_LOCAL_HEADER_FILE":
			addFileName('usb_device_audio_v2_local.h', usbSymbolSource, "middleware/src/", "/usb/src", True, None)

def usbDeviceAudioIadUpdate(usbSymbolSource, event):
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		usbSymbolSource.setValue(True, 2)
	else:
		usbSymbolSource.setValue(False, 2)

def usbDeviceAudioSpecVersionUpdate(usbSymbolSource, event):
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		usbSymbolSource.setValue("Audio v2",2)
	else :
		usbSymbolSource.setValue("Audio v1",2)		

def usbDeviceAudioNumberOfInterfacesUpdate(usbSymbolSource, event):
	nInterfacesOld = usbSymbolSource.getValue()
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		usbSymbolSource.setValue(2,2)
		nInterfacesNew = 2
	elif (event["value"] == "Audio v1.0 USB Speaker"):	
		usbSymbolSource.setValue(2,2)
		nInterfacesNew = 2
	elif (event["value"] == "Audio v1.0 USB Microphone"):	
		usbSymbolSource.setValue(2,2)
		nInterfacesNew = 2
	elif (event["value"] == "Audio v1.0 USB Headset"):	
		usbSymbolSource.setValue(3,2)
		nInterfacesNew = 3
	elif (event["value"] == "Audio v1.0 USB Headset Multi Sampling rates"):	
		usbSymbolSource.setValue(3,2)
		nInterfacesNew = 3
	else :
		usbSymbolSource.setValue(2,2)
	updateNumberOfInterfaces(nInterfacesOld, nInterfacesNew)
		
def usbDeviceAudioNumberOfStreamingInterfacesUpdate(usbSymbolSource, event):
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		usbSymbolSource.setValue(1,2)
	elif (event["value"] == "Audio v1.0 USB Speaker"):	
		usbSymbolSource.setValue(1,2)
	elif (event["value"] == "Audio v1.0 USB Microphone"):	
		usbSymbolSource.setValue(1,2)
	elif (event["value"] == "Audio v1.0 USB Headset"):	
		usbSymbolSource.setValue(2,2)
	elif (event["value"] == "Audio v1.0 USB Headset Multiple Sampling rate"):	
		usbSymbolSource.setValue(2,2)
	else :
		usbSymbolSource.setValue(2,2)	
		
def usbDeviceAudioNumberOfAlternateSettingsUpdate(usbSymbolSource, event):
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		usbSymbolSource.setValue(2,2)
	elif (event["value"] == "Audio v1.0 USB Speaker"):	
		usbSymbolSource.setValue(2,2)
	elif (event["value"] == "Audio v1.0 USB Microphone"):	
		usbSymbolSource.setValue(2,2)
	elif (event["value"] == "Audio v1.0 USB Headset"):	
		usbSymbolSource.setValue(2,2)
	elif (event["value"] == "Audio v1.0 USB Headset Multi Sampling rates"):	
		usbSymbolSource.setValue(2,2)
	else :
		usbSymbolSource.setValue(2,2)
	
def usbDeviceAudioDeviceTypeUpdate(usbSymbolSource, event):
	global audioDescriptorSize
	descriptorSize = currentAudioDescriptorSize.getValue()
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		currentAudioDescriptorSize.setValue(100)
	elif (event["value"] == "Audio v1.0 USB Speaker"):	
		currentAudioDescriptorSize.setValue(101)
	elif (event["value"] == "Audio v1.0 USB Microphone"):	
		currentAudioDescriptorSize.setValue(102)
	elif (event["value"] == "Audio v1.0 USB Headset"):	
		currentAudioDescriptorSize.setValue(225)
	elif (event["value"] == "Audio v1.0 USB Headset Multi Sampling rates"):	
		currentAudioDescriptorSize.setValue(100)
	else :
		currentAudioDescriptorSize.setValue(100)
	updateConfigurationDescriptorSize(descriptorSize)	
	
def usbDeviceAudioEpInUpdate(usbSymbolSource, event):
	global epNumberIn
	global epNumberOut
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		usbSymbolSource.setVisible(False)
	elif (event["value"] == "Audio v1.0 USB Speaker"):	
		usbSymbolSource.setVisible(False)
	elif (event["value"] == "Audio v1.0 USB Microphone"):	
		usbSymbolSource.setVisible(True)
	elif (event["value"] == "Audio v1.0 USB Headset"):	
		usbSymbolSource.setVisible(True)
	elif (event["value"] == "Audio v1.0 USB Headset Multi Sampling rates"):	
		usbSymbolSource.setVisible(True)

	if any(x in Variables.get("__PROCESSOR") for x in ["SAME70", "SAMV70", "SAMV71", "SAMS70", "PIC32CZ"]):
		if (audioDeviceType.getValue() == "Audio v1.0 USB Microphone"):	
			if (epNumberIn.getValue() >  epNumberOut.getValue()):
				temp = epNumberIn.getValue()
				epNumberIn.setValue(epNumberOut.getValue())
				epNumberOut.setValue(temp)	
		else:
			if (epNumberOut.getValue() >  epNumberIn.getValue()):
				temp = epNumberIn.getValue()
				epNumberIn.setValue(epNumberOut.getValue())
				epNumberOut.setValue(temp)	
			

def usbDeviceAudioEpOutUpdate(usbSymbolSource, event):
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		usbSymbolSource.setVisible(True)
	elif (event["value"] == "Audio v1.0 USB Speaker"):	
		usbSymbolSource.setVisible(True)
	elif (event["value"] == "Audio v1.0 USB Microphone"):	
		usbSymbolSource.setVisible(False)
	elif (event["value"] == "Audio v1.0 USB Headset"):	
		usbSymbolSource.setVisible(True)
	elif (event["value"] == "Audio v1.0 USB Headset Multi Sampling rates"):	
		usbSymbolSource.setVisible(True)
		
def instantiateComponent(usbDeviceAudioComponent, index):

	res = Database.activateComponents(["usb_device"])
	
	global indexFunction
	global configValue
	global startInterfaceNumber
	global numberOfInterfaces
	global queueSizeRead
	global queueSizeWrite
	global epNumberIn
	global epNumberOut
	global currentQSizeRead
	global currentQSizeWrite
	global currentAudioDescriptorSize
	global audioDeviceType
	
	
	# Index of this function 
	indexFunction = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INDEX", None)
	indexFunction.setVisible(False)
	indexFunction.setMin(0)
	indexFunction.setMax(16)
	indexFunction.setDefaultValue(index)
	
	# Config name: Configuration number
	configValue = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_CONFIG_VALUE", None)
	configValue.setLabel("Configuration Value")
	configValue.setVisible(False)
	configValue.setMin(1)
	configValue.setMax(16)
	configValue.setDefaultValue(1)
	configValue.setReadOnly(True)
	
	# Adding Start Interface number 
	startInterfaceNumber = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER", None)
	startInterfaceNumber.setLabel("Start Interface Number")
	startInterfaceNumber.setVisible(True)
	startInterfaceNumber.setMin(0)
	startInterfaceNumber.setDefaultValue(index)
	startInterfaceNumber.setReadOnly(True)
	startInterfaceNumber.setHelp("Audio_v_1_0_Initializing_the_Library")

	# Audio Device Type 
	audioDeviceType = usbDeviceAudioComponent.createComboSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE", None, audioDeviceTypes)
	audioDeviceType.setLabel("Audio Device Type")
	audioDeviceType.setVisible(True)
	audioDeviceType.setDefaultValue("Audio v1.0 USB Speaker")
	audioDeviceType.setDependencies(usbDeviceAudioDeviceTypeUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	audioDeviceType.setHelp("mcc_configuration_device_audio")
	
	# Audio Spec version
	audioSpecVersion = usbDeviceAudioComponent.createComboSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION", None, audioVersion)
	audioSpecVersion.setLabel("Version")
	audioSpecVersion.setReadOnly(True)
	audioSpecVersion.setVisible(True)
	audioSpecVersion.setDefaultValue("Audio v1")
	audioSpecVersion.setDependencies(usbDeviceAudioSpecVersionUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	audioSpecVersion.setHelp("mcc_configuration_device_audio")
	
	# Use IAD
	useIad = usbDeviceAudioComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FUNCTION_USE_IAD", None)
	useIad.setLabel("Use Interface Association Descriptor")
	useIad.setVisible(True)
	useIad.setReadOnly(True)
	useIad.setDefaultValue(False)
	useIad.setUseSingleDynamicValue(True)
	useIad.setDependencies(usbDeviceAudioIadUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	useIad.setHelp("mcc_configuration_device_audio")
	
	# Adding Number of Interfaces
	numberOfInterfaces = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_NUMBER_OF_INTERFACES", None)
	numberOfInterfaces.setLabel("Number of Interfaces")
	numberOfInterfaces.setVisible(True)
	numberOfInterfaces.setMin(1)
	numberOfInterfaces.setMax(16)
	numberOfInterfaces.setDefaultValue(index+2)
	numberOfInterfaces.setReadOnly(True)
	numberOfInterfaces.setDependencies(usbDeviceAudioNumberOfInterfacesUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	numberOfInterfaces.setHelp("Audio_v_1_0_Initializing_the_Library")
	
	# Audio Number of Streaming Interfaces 
	noStreamingInterface = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_STREAMING_INTERFACES_NUMBER", None)
	noStreamingInterface.setLabel("Number of Audio Streaming Interfaces")
	noStreamingInterface.setVisible(True)
	noStreamingInterface.setMin(1)
	noStreamingInterface.setMax(32767)
	noStreamingInterface.setDefaultValue(1)
	noStreamingInterface.setReadOnly(True)
	noStreamingInterface.setDependencies(usbDeviceAudioNumberOfStreamingInterfacesUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	noStreamingInterface.setHelp("USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES")

	
	# Audio Maximum Number of Interface Alternate Settings
	noMaxAlternateSettings = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_MAX_ALTERNATE_SETTING", None)
	noMaxAlternateSettings.setLabel("Maximum Number of Interface Alternate Settings")
	noMaxAlternateSettings.setVisible(True)
	noMaxAlternateSettings.setMin(1)
	noMaxAlternateSettings.setMax(32767)
	noMaxAlternateSettings.setDefaultValue(2)
	noMaxAlternateSettings.setReadOnly(True)
	noMaxAlternateSettings.setDependencies(usbDeviceAudioNumberOfAlternateSettingsUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	noMaxAlternateSettings.setHelp("USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING")

	
	# Audio Function driver Read Queue Size 
	queueSizeRead = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", None)
	queueSizeRead.setLabel("Audio Read Queue Size")
	queueSizeRead.setVisible(True)
	queueSizeRead.setMin(0)
	queueSizeRead.setMax(32767)
	queueSizeRead.setDefaultValue(1)
	queueSizeRead.setHelp("USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED")

	# Audio Function driver Write Queue Size 
	queueSizeWrite = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE", None)
	queueSizeWrite.setLabel("Audio Write Queue Size")
	queueSizeWrite.setVisible(True)
	queueSizeWrite.setMin(0)
	queueSizeWrite.setMax(32767)
	queueSizeWrite.setDefaultValue(1)	
	queueSizeWrite.setHelp("USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED")	

	
	# Audio Function driver Data IN Endpoint Number   
	epNumberIn = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_IN_ENDPOINT_NUMBER", None)
	epNumberIn.setLabel("IN Endpoint Number")
	epNumberIn.setVisible(False)
	epNumberIn.setMin(1)
	epNumberIn.setMax(10)
	epNumberIn.setDefaultValue(1)
	epNumberIn.setDependencies(usbDeviceAudioEpInUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	
	# Audio Function driver Data OUT Endpoint Number   
	epNumberOut = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_OUT_ENDPOINT_NUMBER", None)
	epNumberOut.setLabel("OUT Endpoint Number")
	epNumberOut.setVisible(True)
	epNumberOut.setMin(1)
	epNumberOut.setMax(10)
	epNumberOut.setDefaultValue(1)
	epNumberOut.setDependencies(usbDeviceAudioEpOutUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	epNumberOut.setHelp("mcc_configuration_device_audio")

	
	# Audio Function save descriptor size
	currentAudioDescriptorSize = usbDeviceAudioComponent.createIntegerSymbol("USB_DEVICE_AUDIO_FUNCTION_DESCRIPTOR_SIZE_CURRENT", None)
	currentAudioDescriptorSize.setVisible(False)
	currentAudioDescriptorSize.setDefaultValue(101)
	
	##############################################################
	# system_definitions.h file for USB Device Audio v1 Function driver   
	##############################################################
	usbDeviceAudioSystemDefFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_SYSTEM_DEF_FILE", None)
	usbDeviceAudioSystemDefFile.setType("STRING")
	usbDeviceAudioSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceAudioSystemDefFile.setSourcePath("templates/device/audio/system_definitions.h.device_audio_includes.ftl")
	usbDeviceAudioSystemDefFile.setMarkup(True)
	
	##############################################################
	# system_config.h file for USB Device Audio Function driver   
	##############################################################
	usbDeviceAudioSystemConfigFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_SYSTEM_CONFIG_FILE", None)
	usbDeviceAudioSystemConfigFile.setType("STRING")
	usbDeviceAudioSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDeviceAudioSystemConfigFile.setSourcePath("templates/device/audio/system_config.h.device_audio.ftl")
	usbDeviceAudioSystemConfigFile.setMarkup(True)
	
	
	#############################################################
	# Function Init Entry for Audio 
	#############################################################
	usbDeviceAudioFunInitFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_FUN_INIT_FILE", None)
	usbDeviceAudioFunInitFile.setType("STRING")
	usbDeviceAudioFunInitFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
	usbDeviceAudioFunInitFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_init.ftl")
	usbDeviceAudioFunInitFile.setMarkup(True)
	
	
	#############################################################
	# Function Registration table for Audio 
	#############################################################
	usbDeviceAudioFunRegTableFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_FUN_REG_TABLE_FILE", None)
	usbDeviceAudioFunRegTableFile.setType("STRING")
	usbDeviceAudioFunRegTableFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_ENTRY")
	usbDeviceAudioFunRegTableFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function.ftl")
	usbDeviceAudioFunRegTableFile.setMarkup(True)
	
	#############################################################
	# HS Descriptors for Audio Function 
	#############################################################
	usbDeviceAudioDescriptorHsFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_DESCRIPTOR_HS_FILE", None)
	usbDeviceAudioDescriptorHsFile.setType("STRING")
	usbDeviceAudioDescriptorHsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
	usbDeviceAudioDescriptorHsFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_descrptr_hs.ftl")
	usbDeviceAudioDescriptorHsFile.setMarkup(True)
	
	#############################################################
	# FS Descriptors for Audio Function 
	#############################################################
	usbDeviceAudioDescriptorFsFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_DESCRIPTOR_FS_FILE", None)
	usbDeviceAudioDescriptorFsFile.setType("STRING")
	usbDeviceAudioDescriptorFsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
	usbDeviceAudioDescriptorFsFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_descrptr_fs.ftl")
	usbDeviceAudioDescriptorFsFile.setMarkup(True)
	
	
	#############################################################
	# Class code Entry for Audio Function 
	#############################################################
	usbDeviceAudioDescriptorClassCodeFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_DESCRIPTOR_CLASS_CODE_FILE", None)
	usbDeviceAudioDescriptorClassCodeFile.setType("STRING")
	usbDeviceAudioDescriptorClassCodeFile.setOutputName("usb_device.LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")
	usbDeviceAudioDescriptorClassCodeFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_class_codes.ftl")
	usbDeviceAudioDescriptorClassCodeFile.setMarkup(True)
	
	################################################
	# USB Audio Function driver Files 
	################################################
	usbDeviceAudioHeaderFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_HEADER_FILE", None)
	addFileName('usb_device_audio_v1_0.h', usbDeviceAudioHeaderFile, "middleware/", "/usb/", True, None)
	usbDeviceAudioHeaderFile.setDependencies(usbDeviceAudioSpecVersionChanged, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION"])
	
	usbAudioHeaderFile = usbDeviceAudioComponent.createFileSymbol("USB_AUDIO_HEADER_FILE", None)
	addFileName('usb_audio_v1_0.h', usbAudioHeaderFile, "middleware/", "/usb/", True, None)
	usbAudioHeaderFile.setDependencies(usbDeviceAudioSpecVersionChanged, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION"])
	
	usbDeviceAudioSourceFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_SOURCE_FILE", None)
	addFileName('usb_device_audio_v1_0.c', usbDeviceAudioSourceFile, "middleware/src/", "/usb/src", True, None)
	usbDeviceAudioSourceFile.setDependencies(usbDeviceAudioSpecVersionChanged, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION"])
	
	usbDeviceAudioTransferSourceFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_TRANSFER_SOURCE_FILE", None)
	addFileName('usb_device_audio_read_write.c', usbDeviceAudioTransferSourceFile, "middleware/src/", "/usb/src", True, None)
	usbDeviceAudioTransferSourceFile.setDependencies(usbDeviceAudioSpecVersionChanged, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION"])
	
	usbDeviceAudioLocalHeaderFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_LOCAL_HEADER_FILE", None)
	addFileName('usb_device_audio_local.h', usbDeviceAudioLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	usbDeviceAudioLocalHeaderFile.setDependencies(usbDeviceAudioSpecVersionChanged, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION"])

	
	# all files go into src/
def addFileName(fileName, symbol, srcPath, destPath, enabled, callback):
	configName1 = Variables.get("__CONFIGURATION_NAME")
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
