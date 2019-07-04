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
	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	if (dependencyID == "usb_host_dependency"):
		print("USB Host Layer Connected")
		readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		if readValue != None:
			args = {"nTpl":readValue + 1}
			res = Database.sendMessage("usb_host", "UPDATE_TPL_ENTRY_NUMBER", args)

def onAttachmentDisconnected(source, target):
	dependencyID = source["id"]
	ownerComponent = source["component"]
	remoteComponent = target["component"]
	remoteID = remoteComponent.getID()
	connectID = source["id"]
	targetID = target["id"]
	if (dependencyID == "usb_host_dependency"):
		readValue = Database.getSymbolValue("usb_host", "CONFIG_USB_HOST_TPL_ENTRY_NUMBER")
		if readValue != None:
			args = {"nTpl":readValue - 1}
			res = Database.sendMessage("usb_host", "UPDATE_TPL_ENTRY_NUMBER", args)

def destroyComponent(component):	
	print("USB HOST MSD Client Driver: Destroyed")
		
def instantiateComponent(usbHostMsdComponent):
	res = Database.activateComponents(["usb_host"])
	
	# USB Host MSD client driver instances 
	usbHostMsdClientDriverInstance = usbHostMsdComponent.createIntegerSymbol("CONFIG_USB_HOST_MSD_NUMBER_OF_INSTANCES", None)
	usbHostMsdClientDriverInstance.setLabel("Number of MSD Client Driver Instances")
	usbHostMsdClientDriverInstance.setDescription("Enter the number of MSD Class Driver instances required in the application.")
	usbHostMsdClientDriverInstance.setDefaultValue(1)
	usbHostMsdClientDriverInstance.setVisible(True)
	
	##############################################################
	# system_definitions.h file for USB Host MSD Client driver   
	##############################################################
	usbHostMsdSystemDefFile = usbHostMsdComponent.createFileSymbol(None, None)
	usbHostMsdSystemDefFile.setType("STRING")
	usbHostMsdSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	usbHostMsdSystemDefFile.setSourcePath("templates/host/system_definitions.h.host_msd_includes.ftl")
	usbHostMsdSystemDefFile.setMarkup(True)
	
	##############################################################
	# system_config.h file for USB Host MSD Client driver   
	##############################################################
	usbHostMsdSystemConfigFile = usbHostMsdComponent.createFileSymbol(None, None)
	usbHostMsdSystemConfigFile.setType("STRING")
	usbHostMsdSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbHostMsdSystemConfigFile.setSourcePath("templates/host/system_config.h.host_msd.ftl")
	usbHostMsdSystemConfigFile.setMarkup(True)
	
	##############################################################
	# TPL Entry for MSD client driver 
	##############################################################
	usbHostMsdTplEntryFile = usbHostMsdComponent.createFileSymbol(None, None)
	usbHostMsdTplEntryFile.setType("STRING")
	usbHostMsdTplEntryFile.setOutputName("usb_host.LIST_USB_HOST_TPL_ENTRY")
	usbHostMsdTplEntryFile.setSourcePath("templates/host/system_init_c_msd_tpl.ftl")
	usbHostMsdTplEntryFile.setMarkup(True)
	 
	################################################
	# USB Host MSD Client driver Files 
	################################################
	usbHostMsdHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_msd.h', usbHostMsdComponent, usbHostMsdHeaderFile, "middleware/", "/usb/", True, None)
	
	usbHostScsiHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_scsi.h', usbHostMsdComponent, usbHostScsiHeaderFile, "middleware/", "/usb/", True, None)
	
	usbMsdHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_msd.h', usbHostMsdComponent, usbMsdHeaderFile, "middleware/", "/usb/", True, None)
	
	usbScsiHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('scsi.h', usbHostMsdComponent, usbScsiHeaderFile, "middleware/", "/usb/", True, None)
	
	
	usbHostMsdSourceFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_msd.c', usbHostMsdComponent, usbHostMsdSourceFile, "middleware/src/", "/usb/src", True, None)
	
	usbHostScsiSourceFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_scsi.c', usbHostMsdComponent, usbHostScsiSourceFile, "middleware/src/", "/usb/src", True, None)

	usbHostMsdLocalHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_msd_local.h', usbHostMsdComponent, usbHostMsdLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	usbHostScsiLocalHeaderFile = usbHostMsdComponent.createFileSymbol(None, None)
	addFileName('usb_host_scsi_local.h', usbHostMsdComponent, usbHostScsiLocalHeaderFile, "middleware/src/", "/usb/src", True, None)
	
	
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