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
currentQSizeRead  = 1
currentQSizeWrite = 1
audioInterfacesNumber = 2
audioDescriptorSize = 101
usbConfigSizeUpdated = False
usbNumberOfInterfacesUpdated = False


indexFunction = None
configValue = None
startInterfaceNumber = None
numberOfInterfaces = None
queueSizeRead = None
queueSizeWrite = None
epNumberIn = None
epNumberOut = None
usbDeviceAudioFunInitFile = None
usbDeviceAudioFunRegTableFile = None
usbDeviceAudioDescriptorHsFile = None
usbDeviceAudioDescriptorFsFile = None
usbDeviceAudioDescriptorClassCodeFile = None
	
audioVersion =["Audio v1", "Audio v2"]
audioDeviceTypes = [
"Audio v1.0 USB Speaker",
"Audio v1.0 USB Microphone",
"Audio v1.0 USB Headset",
"Audio v1.0 USB Headset Multi Sampling rates",
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
	global usbDeviceAudioFunInitFile
	global usbDeviceAudioFunRegTableFile
	global usbDeviceAudioDescriptorHsFile
	global usbDeviceAudioDescriptorFsFile
	global usbDeviceAudioDescriptorClassCodeFile
	
	ownerComponent = source["component"]
	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	
	if (dependencyID == "usb_device_dependency"):
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		if readValue != None: 
			args = {"nFunction":readValue + 1}
			res = Database.sendMessage(remoteID, "UPDATE_FUNCTIONS_NUMBER", args)
	
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if readValue != None: 
			args = {"nFunction": readValue + audioDescriptorSize}
			res = Database.sendMessage(remoteID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)
	
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if readValue != None: 
			args = {"nFunction":  readValue + audioInterfacesNumber}
			res = Database.sendMessage(remoteID, "UPDATE_INTERFACES_NUMBER", args)
			startInterfaceNumber.setValue(readValue, 1)
	
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if readValue != None:
			args = {"nFunction":   readValue + 2}
			res = Database.sendMessage(remoteID, "UPDATE_ENDPOINTS_NUMBER", args)
			epNumberIn.setValue(readValue , 1)
			epNumberOut.setValue(readValue + 1, 1)
		usbDeviceAudioFunInitFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
		usbDeviceAudioFunRegTableFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_ENTRY")
		usbDeviceAudioDescriptorHsFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
		usbDeviceAudioDescriptorFsFile.setOutputName(remoteID + ".LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
		usbDeviceAudioDescriptorClassCodeFile.setOutputName(remoteID + ".LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")


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
	global usbDeviceAudioFunInitFile
	global usbDeviceAudioFunRegTableFile
	global usbDeviceAudioDescriptorHsFile
	global usbDeviceAudioDescriptorFsFile
	global usbDeviceAudioDescriptorClassCodeFile

	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	
	ownerComponent = source["component"]
	dependencyID = source["id"]
	if (dependencyID == "usb_device_dependency"):
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		if readValue != None: 
			args = {"nFunction":readValue - 1}
			res = Database.sendMessage(remoteID, "UPDATE_FUNCTIONS_NUMBER", args)
		
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if readValue != None: 
			args = {"nFunction": readValue - audioDescriptorSize}
			res = Database.sendMessage(remoteID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)
		
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if readValue != None: 
			args = {"nFunction":  readValue - audioInterfacesNumber}
			res = Database.sendMessage(remoteID, "UPDATE_INTERFACES_NUMBER", args)
		
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_ENDPOINTS_NUMBER")
		if readValue != None:
			args = {"nFunction":readValue - 2 }
			res = Database.sendMessage(remoteID, "UPDATE_ENDPOINTS_NUMBER", args)
			
			
def destroyComponent(component):
	print ("Audio Function Driver: Destroyed")
	

def updateConfigurationDescriptorSize(descriptorSizeOld):
	global usbConfigSizeUpdated
	if (usbConfigSizeUpdated == False):
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if (readValue != None):
			args = {"nFunction":readValue + audioDescriptorSize}
			res = Database.sendMessage(remoteID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)
	else:
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		if (readValue != None):
			args = {"nFunction": readValue + audioDescriptorSize - descriptorSizeOld}
			res = Database.sendMessage(remoteID, "UPDATE_CONFIG_DESCRPTR_SIZE", args)
	usbConfigSizeUpdated = True

def updateNumberOfInterfaces(numberOfInterfacesOld, nInterfacesNew):
	global usbNumberOfInterfacesUpdated
	if (usbNumberOfInterfacesUpdated == False):
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if (readValue != None):
			args = {"nFunction":  readValue + audioInterfacesNumber}
			res = Database.sendMessage(remoteID, "UPDATE_INTERFACES_NUMBER", args)
	else:
		readValue = Database.getSymbolValue(remoteID, "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		if (readValue != None):
			args = {"nFunction":   readValue + nInterfacesNew - numberOfInterfacesOld}
			res = Database.sendMessage(remoteID, "UPDATE_INTERFACES_NUMBER", args)
	usbNumberOfInterfacesUpdated = True 

def usbDeviceAudioBufferQueueSize(usbSymbolSource, event):
	global currentQSizeRead
	global currentQSizeWrite
	queueDepthCombined = Database.getSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED")
	if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE"):
		queueDepthCombined = queueDepthCombined - currentQSizeRead + event["value"]
		currentQSizeRead = event["value"]
	if (event["id"] == "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"):
		queueDepthCombined = queueDepthCombined - currentQSizeWrite  + event["value"]
		currentQSizeWrite = event["value"]
	Database.clearSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED")
	Database.setSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED", queueDepthCombined)

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
	nInterfaces = usbSymbolSource.getValue()
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
	elif (event["value"] == "Audio v1.0 USB Headset Multiple Sampling rate"):	
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
	elif (event["value"] == "Audio v1.0 USB Headset Multiple Sampling rate"):	
		usbSymbolSource.setValue(2,2)
	else :
		usbSymbolSource.setValue(2,2)
	
def usbDeviceAudioDeviceTypeUpdate(usbSymbolSource, event):
	global audioDescriptorSize
	descriptorSize = audioDescriptorSize
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		audioDescriptorSize = 100
	elif (event["value"] == "Audio v1.0 USB Speaker"):	
		audioDescriptorSize = 101
	elif (event["value"] == "Audio v1.0 USB Microphone"):	
		audioDescriptorSize = 102
	elif (event["value"] == "Audio v1.0 USB Headset"):	
		audioDescriptorSize = 225
	elif (event["value"] == "Audio v1.0 USB Headset Multiple Sampling rate"):	
		audioDescriptorSize = 100
	else :
		audioDescriptorSize = 100
	updateConfigurationDescriptorSize(audioDescriptorSize)	
		
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
	global usbDeviceAudioFunInitFile
	global usbDeviceAudioFunRegTableFile
	global usbDeviceAudioDescriptorHsFile
	global usbDeviceAudioDescriptorFsFile
	global usbDeviceAudioDescriptorClassCodeFile
	
	
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
	
	# Audio Device Type 
	audioDeviceType = usbDeviceAudioComponent.createComboSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE", None, audioDeviceTypes)
	audioDeviceType.setLabel("Audio Device Type")
	audioDeviceType.setVisible(True)
	audioDeviceType.setUseSingleDynamicValue(True)
	audioDeviceType.setDependencies(usbDeviceAudioDeviceTypeUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	
	# Audio Spec version
	audioSpecVersion = usbDeviceAudioComponent.createComboSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION", None, audioVersion)
	audioSpecVersion.setLabel("Version")
	audioSpecVersion.setReadOnly(True)
	audioSpecVersion.setVisible(True)
	audioSpecVersion.setDefaultValue("Audio v1")
	audioSpecVersion.setDependencies(usbDeviceAudioSpecVersionUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	
	# Use IAD
	useIad = usbDeviceAudioComponent.createBooleanSymbol("CONFIG_USB_DEVICE_FUNCTION_USE_IAD", None)
	useIad.setLabel("Use Interface Association Descriptor")
	useIad.setVisible(True)
	useIad.setReadOnly(True)
	useIad.setDefaultValue(False)
	useIad.setUseSingleDynamicValue(True)
	useIad.setDependencies(usbDeviceAudioIadUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	
	# Adding Number of Interfaces
	numberOfInterfaces = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_NUMBER_OF_INTERFACES", None)
	numberOfInterfaces.setLabel("Number of Interfaces")
	numberOfInterfaces.setVisible(True)
	numberOfInterfaces.setMin(1)
	numberOfInterfaces.setMax(16)
	numberOfInterfaces.setDefaultValue(index+2)
	numberOfInterfaces.setReadOnly(True)
	numberOfInterfaces.setDependencies(usbDeviceAudioNumberOfInterfacesUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	
	# Audio Number of Streaming Interfaces 
	noStreamingInterface = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_STREAMING_INTERFACES_NUMBER", None)
	noStreamingInterface.setLabel("Number of Audio Streaming Interfaces")
	noStreamingInterface.setVisible(True)
	noStreamingInterface.setMin(1)
	noStreamingInterface.setMax(32767)
	noStreamingInterface.setDefaultValue(1)
	noStreamingInterface.setReadOnly(True)
	noStreamingInterface.setDependencies(usbDeviceAudioNumberOfStreamingInterfacesUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	
	# Audio Maximum Number of Interface Alternate Settings
	noMaxAlternateSettings = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_MAX_ALTERNATE_SETTING", None)
	noMaxAlternateSettings.setLabel("Maximum Number of Interface Alternate Settings")
	noMaxAlternateSettings.setVisible(True)
	noMaxAlternateSettings.setMin(1)
	noMaxAlternateSettings.setMax(32767)
	noMaxAlternateSettings.setDefaultValue(2)
	noMaxAlternateSettings.setReadOnly(True)
	noMaxAlternateSettings.setDependencies(usbDeviceAudioNumberOfAlternateSettingsUpdate, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE"])
	
	usbDeviceAudioBufPool = usbDeviceAudioComponent.createBooleanSymbol("CONFIG_USB_DEVICE_AUDIO_BUFFER_POOL", None)
	usbDeviceAudioBufPool.setLabel("**** Buffer Pool Update ****")
	usbDeviceAudioBufPool.setDependencies(usbDeviceAudioBufferQueueSize, ["CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", "CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE"])
	usbDeviceAudioBufPool.setVisible(False)
	
	# Audio Function driver Read Queue Size 
	queueSizeRead = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE", None)
	queueSizeRead.setLabel("Audio Read Queue Size")
	queueSizeRead.setVisible(True)
	queueSizeRead.setMin(1)
	queueSizeRead.setMax(32767)
	queueSizeRead.setDefaultValue(1)
	currentQSizeRead = queueSizeRead.getValue()

	# Audio Function driver Write Queue Size 
	queueSizeWrite = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE", None)
	queueSizeWrite.setLabel("Audio Write Queue Size")
	queueSizeWrite.setVisible(True)
	queueSizeWrite.setMin(1)
	queueSizeWrite.setMax(32767)
	queueSizeWrite.setDefaultValue(1)	
	currentQSizeWrite = queueSizeWrite.getValue()
	
	# Audio Function driver Data IN Endpoint Number   
	epNumberIn = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_IN_ENDPOINT_NUMBER", None)
	epNumberIn.setLabel("IN Endpoint Number")
	epNumberIn.setVisible(True)
	epNumberIn.setMin(1)
	epNumberIn.setMax(10)
	epNumberIn.setDefaultValue(3)
	
	# Audio Function driver Data OUT Endpoint Number   
	epNumberOut = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_OUT_ENDPOINT_NUMBER", None)
	epNumberOut.setLabel("OUT Endpoint Number")
	epNumberOut.setVisible(False)
	epNumberOut.setMin(1)
	epNumberOut.setMax(10)
	epNumberOut.setDefaultValue(2)
	


	############################################################################
	#### Dependency ####
	############################################################################
	# USB DEVICE Audio v1 Common Dependency
	
	numInstances  = Database.getSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_INSTANCES")
	if (numInstances == None):
		numInstances = 0
		
	queueDepthCombined = Database.getSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED")
	if (queueDepthCombined == None):
		queueDepthCombined = 0
		
	maxStreamingIntfc = Database.getSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_STREAMING_INTERFACES_NUMBER")
	if (maxStreamingIntfc == None):
		maxStreamingIntfc = 0
		
	maxIntfcAltSettings = Database.getSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_MAX_ALTERNATE_SETTING")
	if (maxIntfcAltSettings == None):
		maxIntfcAltSettings = 0
	
	#if numInstances < (index+1):
	args = {"audioInstanceCount": index+1}
	res = Database.sendMessage("usb_device_audio", "UPDATE_AUDIO_INSTANCES", args)
	
	args = {"audioQueueDepth": queueDepthCombined + currentQSizeRead + currentQSizeWrite }
	res = Database.sendMessage("usb_device_audio", "UPDATE_AUDIO_QUEUE_DEPTH_COMBINED", args)
	
	Database.clearSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_STREAMING_INTERFACES_NUMBER_COMBINED")
	Database.setSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_STREAMING_INTERFACES_NUMBER_COMBINED", 1)
	
	
	Database.clearSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_MAX_ALTERNATE_SETTING_COMBINED")
	Database.setSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_MAX_ALTERNATE_SETTING_COMBINED", 2)
	
	##############################################################
	# system_definitions.h file for USB Device Audio v1 Function driver   
	##############################################################
	usbDeviceAudioSystemDefFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_SYSTEM_DEF_FILE", None)
	usbDeviceAudioSystemDefFile.setType("STRING")
	usbDeviceAudioSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceAudioSystemDefFile.setSourcePath("templates/device/audio/system_definitions.h.device_audio_includes.ftl")
	usbDeviceAudioSystemDefFile.setMarkup(True)
	
	
	#############################################################
	# Function Init Entry for Audio 
	#############################################################
	usbDeviceAudioFunInitFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_FUNCTION_INIT_FILE", None)
	usbDeviceAudioFunInitFile.setType("STRING")
	usbDeviceAudioFunInitFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_init.ftl")
	usbDeviceAudioFunInitFile.setMarkup(True)
	
	
	#############################################################
	# Function Registration table for Audio 
	#############################################################
	usbDeviceAudioFunRegTableFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_FUN_REG_TABLE_FILE", None)
	usbDeviceAudioFunRegTableFile.setType("STRING")
	usbDeviceAudioFunRegTableFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function.ftl")
	usbDeviceAudioFunRegTableFile.setMarkup(True)
	
	#############################################################
	# HS Descriptors for Audio Function 
	#############################################################
	usbDeviceAudioDescriptorHsFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_DESCRIPTOR_HS_FILE", None)
	usbDeviceAudioDescriptorHsFile.setType("STRING")
	usbDeviceAudioDescriptorHsFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_descrptr_hs.ftl")
	usbDeviceAudioDescriptorHsFile.setMarkup(True)
	
	#############################################################
	# FS Descriptors for Audio Function 
	#############################################################
	usbDeviceAudioDescriptorFsFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_DESCRIPTOR_FS_FILE", None)
	usbDeviceAudioDescriptorFsFile.setType("STRING")
	usbDeviceAudioDescriptorFsFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_descrptr_fs.ftl")
	usbDeviceAudioDescriptorFsFile.setMarkup(True)
	
	
	#############################################################
	# Class code Entry for Audio Function 
	#############################################################
	usbDeviceAudioDescriptorClassCodeFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_DESCRIPTOR_CLAS_CODE_FILE", None)
	usbDeviceAudioDescriptorClassCodeFile.setType("STRING")
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
	symbol.setSourcePath(srcPath + fileName)
	symbol.setOutputName(fileName)
	symbol.setDestPath(destPath)
	if fileName[-2:] == '.h':
		symbol.setType("HEADER")
	else:
		symbol.setType("SOURCE")
	symbol.setEnabled(enabled)

