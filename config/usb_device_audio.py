currentQSizeRead  = 1
currentQSizeWrite = 1
audioInterfacesNumber = 2
audioDescriptorSize = 58

	
audioVersion =["Audio v1", "Audio v2"]
audioDeviceTypes = [
"Audio v1.0 USB Speaker",
"Audio v1.0 USB Microphone",
"Audio v1.0 USB Headset",
"Audio v1.0 USB Headset Multiple Sampling rate",
"Audio v2.0 USB Speaker" 
]
# countFunctionDrivers = 0; 

def onDependentComponentAdded(ownerComponent, dependencyID, dependentComponent):
	print("Debug: Connected to USB Device Layer")
	print(ownerComponent)
	if (dependencyID == "usb_device_dependency"):
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		print("Debug: CONFIG_USB_DEVICE_FUNCTIONS_NUMBER", readValue)
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER", readValue - 1 + 1 , 2)
		
		# If we have Audio function driver plus any function driver (no matter what class), we enable IAD. 
		if readValue > 0:
			Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_DESCRIPTOR_IAD_ENABLE")
			Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_DESCRIPTOR_IAD_ENABLE", True, 2)
			iadEnableSymbol = ownerComponent.getSymbolByID("CONFIG_USB_DEVICE_FUNCTION_USE_IAD")
			iadEnableSymbol.clearValue()
			iadEnableSymbol.setValue(True, 1)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE", readValue + audioDescriptorSize , 2)
		
		readValue = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER")
		Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_INTERFACES_NUMBER", readValue + audioInterfacesNumber , 2)

		
def destroyComponent(component):
	# global countFunctionDrivers
	functionsNumber = Database.getSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
	Database.clearSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER")
	Database.setSymbolValue("usb_device", "CONFIG_USB_DEVICE_FUNCTIONS_NUMBER", functionsNumber -  1 , 2)
	

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
	Database.setSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED", queueDepthCombined, 2)

def usbDeviceAudioSpecVersionChanged(usbSymbolSource, event):
	if event["value"] == "Audio v1":
		if usbSymbolSource.getID() == "USB_DEVICE_AUDIO_HEADER_FILE":
			addFileName('usb_device_audio_v1_0.h', usbSymbolSource, "", "/usb/", True, None)
		elif usbSymbolSource.getID() == "USB_AUDIO_HEADER_FILE":
			addFileName('usb_audio_v1_0.h', usbSymbolSource, "", "/usb/", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_SOURCE_FILE":
			addFileName('usb_device_audio_v1_0.c', usbSymbolSource, "src/", "/usb/src", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_TRANSFER_SOURCE_FILE":
			addFileName('usb_device_audio_read_write.c', usbSymbolSource, "src/", "/usb/src", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_LOCAL_HEADER_FILE":
			addFileName('usb_device_audio_local.h', usbSymbolSource, "src/", "/usb/src", True, None)
	elif event["value"] == "Audio v2":	
		if usbSymbolSource.getID() == "USB_DEVICE_AUDIO_HEADER_FILE":
			addFileName('usb_device_audio_v2_0.h', usbSymbolSource, "", "/usb/", True, None)
		elif usbSymbolSource.getID() == "USB_AUDIO_HEADER_FILE":
			addFileName('usb_audio_v2_0.h', usbSymbolSource, "", "/usb/", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_SOURCE_FILE":
			addFileName('usb_device_audio_v2_0.c', usbSymbolSource, "src/", "/usb/src", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_TRANSFER_SOURCE_FILE":
			addFileName('usb_device_audio_v2_read_write.c', usbSymbolSource, "src/", "/usb/src", True, None)
		elif usbSymbolSource.getID() == "USB_DEVICE_AUDIO_LOCAL_HEADER_FILE":
			addFileName('usb_device_audio_v2_local.h', usbSymbolSource, "src/", "/usb/src", True, None)

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
	if (event["value"] == "Audio v2.0 USB Speaker"):	
		usbSymbolSource.setValue(2,2)
	elif (event["value"] == "Audio v1.0 USB Speaker"):	
		usbSymbolSource.setValue(2,2)
	elif (event["value"] == "Audio v1.0 USB Microphone"):	
		usbSymbolSource.setValue(2,2)
	elif (event["value"] == "Audio v1.0 USB Headset"):	
		usbSymbolSource.setValue(3,2)
	elif (event["value"] == "Audio v1.0 USB Headset Multiple Sampling rate"):	
		usbSymbolSource.setValue(3,2)
	else :
		usbSymbolSource.setValue(2,2)	
		
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
		
def instantiateComponent(usbDeviceAudioComponent, index):
	# Index of this function 
	indexFunction = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_INDEX", None)
	indexFunction.setVisible(False)
	indexFunction.setMin(0)
	indexFunction.setMax(16)
	indexFunction.setDefaultValue(index)
	
	# Config name: Configuration number
	configValue = usbDeviceAudioComponent.createIntegerSymbol("CONFIG_USB_DEVICE_FUNCTION_CONFIG_VALUE", None)
	configValue.setLabel("Configuration Value")
	configValue.setVisible(True)
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

	
	# Audio Device Type 
	audioDeviceType = usbDeviceAudioComponent.createComboSymbol("CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE", None, audioDeviceTypes)
	audioDeviceType.setLabel("Audio Device Type")
	audioDeviceType.setVisible(True)
	audioDeviceType.setUseSingleDynamicValue(True)
	
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
	Database.clearSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_INSTANCES")
	Database.setSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_INSTANCES", (index+1), 2)
	Database.clearSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED")
	Database.setSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED", queueDepthCombined + currentQSizeRead + currentQSizeWrite, 2)
	print(maxStreamingIntfc, maxIntfcAltSettings, currentQSizeRead,currentQSizeWrite)
	
	
	Database.clearSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_STREAMING_INTERFACES_NUMBER_COMBINED")
	Database.setSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_STREAMING_INTERFACES_NUMBER_COMBINED", 1, 2)
	
	
	Database.clearSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_MAX_ALTERNATE_SETTING_COMBINED")
	Database.setSymbolValue("usb_device_audio", "CONFIG_USB_DEVICE_FUNCTION_AUDIO_MAX_ALTERNATE_SETTING_COMBINED", 2, 2)
		
		
	##############################################################
	# system_definitions.h file for USB Device Audio v1 Function driver   
	##############################################################
	usbDeviceAudioSystemDefFile = usbDeviceAudioComponent.createFileSymbol(None, None)
	usbDeviceAudioSystemDefFile.setType("STRING")
	usbDeviceAudioSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbDeviceAudioSystemDefFile.setSourcePath("templates/device/audio/system_definitions.h.device_audio_includes.ftl")
	usbDeviceAudioSystemDefFile.setMarkup(True)
	
	
	#############################################################
	# Function Init Entry for Audio 
	#############################################################
	usbDeviceAudioFunInitFile = usbDeviceAudioComponent.createFileSymbol(None, None)
	usbDeviceAudioFunInitFile.setType("STRING")
	usbDeviceAudioFunInitFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_INIT_ENTRY")
	usbDeviceAudioFunInitFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_init.ftl")
	usbDeviceAudioFunInitFile.setMarkup(True)
	
	
	#############################################################
	# Function Registration table for Audio 
	#############################################################
	usbDeviceAudioFunRegTableFile = usbDeviceAudioComponent.createFileSymbol(None, None)
	usbDeviceAudioFunRegTableFile.setType("STRING")
	usbDeviceAudioFunRegTableFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_ENTRY")
	usbDeviceAudioFunRegTableFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function.ftl")
	usbDeviceAudioFunRegTableFile.setMarkup(True)
	
	#############################################################
	# HS Descriptors for Audio Function 
	#############################################################
	usbDeviceAudioDescriptorHsFile = usbDeviceAudioComponent.createFileSymbol(None, None)
	usbDeviceAudioDescriptorHsFile.setType("STRING")
	usbDeviceAudioDescriptorHsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY")
	usbDeviceAudioDescriptorHsFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_descrptr_hs.ftl")
	usbDeviceAudioDescriptorHsFile.setMarkup(True)
	
	#############################################################
	# FS Descriptors for Audio Function 
	#############################################################
	usbDeviceAudioDescriptorFsFile = usbDeviceAudioComponent.createFileSymbol(None, None)
	usbDeviceAudioDescriptorFsFile.setType("STRING")
	usbDeviceAudioDescriptorFsFile.setOutputName("usb_device.LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY")
	usbDeviceAudioDescriptorFsFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_descrptr_fs.ftl")
	usbDeviceAudioDescriptorFsFile.setMarkup(True)
	
	
	#############################################################
	# Class code Entry for Audio Function 
	#############################################################
	usbDeviceAudioDescriptorClassCodeFile = usbDeviceAudioComponent.createFileSymbol(None, None)
	usbDeviceAudioDescriptorClassCodeFile.setType("STRING")
	usbDeviceAudioDescriptorClassCodeFile.setOutputName("usb_device.LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY")
	usbDeviceAudioDescriptorClassCodeFile.setSourcePath("templates/device/audio/system_init_c_device_data_audio_function_class_codes.ftl")
	usbDeviceAudioDescriptorClassCodeFile.setMarkup(True)
	
	################################################
	# USB Audio Function driver Files 
	################################################
	usbDeviceAudioHeaderFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_HEADER_FILE", None)
	addFileName('usb_device_audio_v1_0.h', usbDeviceAudioHeaderFile, "", "/usb/", True, None)
	usbDeviceAudioHeaderFile.setDependencies(usbDeviceAudioSpecVersionChanged, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION"])
	
	usbAudioHeaderFile = usbDeviceAudioComponent.createFileSymbol("USB_AUDIO_HEADER_FILE", None)
	addFileName('usb_audio_v1_0.h', usbAudioHeaderFile, "", "/usb/", True, None)
	usbAudioHeaderFile.setDependencies(usbDeviceAudioSpecVersionChanged, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION"])
	
	usbDeviceAudioSourceFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_SOURCE_FILE", None)
	addFileName('usb_device_audio_v1_0.c', usbDeviceAudioSourceFile, "src/", "/usb/src", True, None)
	usbDeviceAudioSourceFile.setDependencies(usbDeviceAudioSpecVersionChanged, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION"])
	
	usbDeviceAudioTransferSourceFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_TRANSFER_SOURCE_FILE", None)
	addFileName('usb_device_audio_read_write.c', usbDeviceAudioTransferSourceFile, "src/", "/usb/src", True, None)
	usbDeviceAudioTransferSourceFile.setDependencies(usbDeviceAudioSpecVersionChanged, ["CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION"])
	
	usbDeviceAudioLocalHeaderFile = usbDeviceAudioComponent.createFileSymbol("USB_DEVICE_AUDIO_LOCAL_HEADER_FILE", None)
	addFileName('usb_device_audio_local.h', usbDeviceAudioLocalHeaderFile, "src/", "/usb/src", True, None)
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
	if callback != None:
		symbol.setDependencies(callback, ["USB_DEVICE_FUNCTION_1_DEVICE_CLASS"])