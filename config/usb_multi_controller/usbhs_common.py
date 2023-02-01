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
usbPeripheralHostSupport = None 
usbPeripheralDeviceSupport = None
usbDriverHostAttachDebounce = None
usbDriverHostResetDuration = None

usbDriverPath = "driver/"
usbDriverProjectPath = "/driver/usb/"
def handleMessage(messageID, args):	
	global usbPeripheralHostSupport
	global usbPeripheralDeviceSupport
	global usbDriverHostResetDuration
	global usbDriverHostAttachDebounce
	if (messageID == "UPDATE_OPERATION_MODE_COMMON"):
		opModeId0 = Database.getSymbolValue("drv_usbhs_index_0", "USB_OPERATION_MODE")
		opModeId1 = Database.getSymbolValue("drv_usbhs_index_1", "USB_OPERATION_MODE")

		if ((opModeId0 != None) and  ((opModeId0 == "Host") or (opModeId0 == "Dual Role"))) or ((opModeId1 != None) and  ((opModeId1 == "Host") or (opModeId1 == "Dual Role"))):
			usbPeripheralHostSupport.setValue(True)
			usbDriverHostAttachDebounce.setVisible(True)
			usbDriverHostResetDuration.setVisible(True)
		else:
			usbPeripheralHostSupport.setValue(False)
			usbDriverHostAttachDebounce.setVisible(False)
			usbDriverHostResetDuration.setVisible(False)
		if ((opModeId0 != None) and  (opModeId0 == "Device")) or ((opModeId1 != None) and  (opModeId1 == "Device")):
			usbPeripheralDeviceSupport.setValue(True)
		else:
			usbPeripheralDeviceSupport.setValue(False)



def instantiateComponent(usbPeripheralComponentCommon):
	global usbPeripheralInstnces
	global usbPeripheralHostSupport
	global usbPeripheralDeviceSupport
	global usbDriverHostResetDuration
	global usbDriverHostAttachDebounce

	# Enable "Generate Harmony Driver Common Files" option in MHC
	Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":True})

	# Enable "Generate Harmony System Service Common Files" option in MHC
	Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":True})
	
	configName = Variables.get("__CONFIGURATION_NAME")
	
	sourcePath = "templates/driver/usbhs_multi/"

	if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32CZ", "PIC32CK"]):
		modules = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals").getChildren()
		for module in range(len(modules)):
			if (modules[module].getAttribute("name")) == "USBHS":
				instances = modules[module].getChildren()
				usbControllersNumber = len(instances)
					
	usbControllersNumberSymbol = usbPeripheralComponentCommon.createIntegerSymbol("CONFIG_USB_CONTROLLERS_NUMBER", None)
	usbControllersNumberSymbol.setLabel("Number of Instances")
	usbControllersNumberSymbol.setMin(1)
	usbControllersNumberSymbol.setMax(2)
	usbControllersNumberSymbol.setDefaultValue(usbControllersNumber)
	usbControllersNumberSymbol.setVisible(False)

	usbPeripheralInstnces = usbPeripheralComponentCommon.createIntegerSymbol("CONFIG_USB_PERIPHERAL_INSTANCES", None)
	usbPeripheralInstnces.setLabel("Number of Instances")
	usbPeripheralInstnces.setMin(1)
	usbPeripheralInstnces.setMax(2)
	usbPeripheralInstnces.setDefaultValue(1)
	#usbDeviceCdcInstnces.setUseSingleDynamicValue(True)
	usbPeripheralInstnces.setVisible(False)
	
	# USB Driver Host mode Attach de-bounce duration
	usbDriverHostAttachDebounce = usbPeripheralComponentCommon.createIntegerSymbol("USB_DRV_HOST_ATTACH_DEBOUNCE_DURATION", None)
	usbDriverHostAttachDebounce.setLabel("USB Host Attach De-bounce Duration (mSec)")
	usbDriverHostAttachDebounce.setVisible(False)
	usbDriverHostAttachDebounce.setDescription("Set USB Host Attach De-bounce duration")
	usbDriverHostAttachDebounce.setDefaultValue(500)
	
	# USB Driver Host mode Reset Duration
	usbDriverHostResetDuration = usbPeripheralComponentCommon.createIntegerSymbol("USB_DRV_HOST_RESET_DUARTION", None)
	usbDriverHostResetDuration.setLabel("USB Host Reset Duration (mSec)")
	usbDriverHostResetDuration.setVisible(False)
	usbDriverHostResetDuration.setDescription("Set USB Host Attach De-bounce duration")
	usbDriverHostResetDuration.setDefaultValue(100)
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MK" , "PIC32MX", "PIC32MM"]):
		plib_usbhs_header_h = usbPeripheralComponentCommon.createFileSymbol("PLIB_USBHS_HEADER_H", None)
		plib_usbhs_header_h.setSourcePath( usbDriverPath + "usbhsv2/src/plib_usbhs_header.h.ftl")
		plib_usbhs_header_h.setOutputName("plib_usbhs_header.h")
		plib_usbhs_header_h.setDestPath("/driver/usb/usbhs/src")
		plib_usbhs_header_h.setProjectPath("config/" + configName + "/driver/usb/usbhs/src/")
		plib_usbhs_header_h.setType("SOURCE")
		plib_usbhs_header_h.setMarkup(True)
		plib_usbhs_header_h.setOverwrite(True)
	
	################################################
	# system_config.h file for USB Driver
	################################################
	usbDriverSystemConfigFile = usbPeripheralComponentCommon.createFileSymbol(None, None)
	usbDriverSystemConfigFile.setType("STRING")
	usbDriverSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDriverSystemConfigFile.setSourcePath(sourcePath + "system_config.h.driver.ftl")
	usbDriverSystemConfigFile.setMarkup(True)
	usbDriverSystemConfigFile.setOverwrite(True)
	
	usbPeripheralHostSupport = usbPeripheralComponentCommon.createBooleanSymbol("DRV_USBHS_MULTI_HOST_SUPPORT", None)
	usbPeripheralHostSupport.setLabel("HOST")
	usbPeripheralHostSupport.setDefaultValue(False)
	usbPeripheralHostSupport.setVisible(False)
	usbPeripheralHostSupport.setReadOnly(True)
	
	usbPeripheralDeviceSupport= usbPeripheralComponentCommon.createBooleanSymbol("DRV_USBHS_MULTI_DEVICE_SUPPORT", None)
	usbPeripheralDeviceSupport.setLabel("DEVICE")
	usbPeripheralDeviceSupport.setDefaultValue(True)
	usbPeripheralDeviceSupport.setVisible(False)
	usbPeripheralDeviceSupport.setReadOnly(True)
	
	drvUsbHsV1HostSourceFile = usbPeripheralComponentCommon.createFileSymbol("DRV_USBHS_COMMON_C_SOURCE", None)
	drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "usbhsv2/src/drv_usbhs.c.ftl")
	drvUsbHsV1HostSourceFile.setOutputName("drv_usbhs.c")
	drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "usbhs/src")
	drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbhs/src/")
	drvUsbHsV1HostSourceFile.setType("SOURCE")
	drvUsbHsV1HostSourceFile.setMarkup(True)
	drvUsbHsV1HostSourceFile.setOverwrite(True)
	drvUsbIsrEntry = usbPeripheralComponentCommon.createListSymbol("LIST_DRV_USB_ISR_ENTRY", None)
	
def destroyComponent(consoleComponent):
	Database.sendMessage("HarmonyCore", "ENABLE_DRV_COMMON", {"isEnabled":False})
	Database.sendMessage("HarmonyCore", "ENABLE_SYS_COMMON", {"isEnabled":False})	
