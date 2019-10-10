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
usbDriverPath = "driver/"
usbDriverProjectPath = "/driver/usb/"
def handleMessage(messageID, args):	
	global usbPeripheralHostSupport
	global usbPeripheralDeviceSupport
	if (messageID == "UPDATE_OPERATION_MODE_COMMON"):
		opModeId0 = Database.getSymbolValue("drv_usbfs_index_0", "USB_OPERATION_MODE")
		opModeId1 = Database.getSymbolValue("drv_usbfs_index_1", "USB_OPERATION_MODE")
		
		if ((opModeId0 != None) and  (opModeId0 == "Host")) or ((opModeId1 != None) and  (opModeId1 == "Host")):
			usbPeripheralHostSupport.setValue(True)
		else:	
			usbPeripheralHostSupport.setValue(False)
		if ((opModeId0 != None) and  (opModeId0 == "Device")) or ((opModeId1 != None) and  (opModeId1 == "Device")):
			usbPeripheralDeviceSupport.setValue(True)
		else:	
			usbPeripheralDeviceSupport.setValue(False)
			


def instantiateComponent(usbPeripheralComponentCommon):
	global usbPeripheralInstnces
	global usbPeripheralHostSupport
	global usbPeripheralDeviceSupport
	
	configName = Variables.get("__CONFIGURATION_NAME")
	
	sourcePath = "templates/driver/usbfs_multi/"
		
	availablePeripherals = []
	if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK" , "PIC32MX"]):
		modules = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals").getChildren()
		for module in range(len(modules)):
			instances = modules[module].getChildren()
			for instance in range(len(instances)):
				if str(instances[instance].getAttribute("name")) == "USB":
					usbRegGroup = ATDF.getNode('/avr-tools-device-file/modules/module@[name="USB"]/register-group@[name="USB"]').getChildren()
					usbIndex = 1
					usbmaxcontrollers = 0
					for register in usbRegGroup:
						regName = str(register.getAttribute("name"))
						usbInstance = "U" + str(usbIndex) + "CON"
						if regName == usbInstance:
							availablePeripherals.append("USB" + str(usbIndex))
							usbIndex += 1
							usbmaxcontrollers += 1
				else:
					availablePeripherals.append(str(instances[instance].getAttribute("name")))
					
	usbControllersNumber = usbPeripheralComponentCommon.createIntegerSymbol("CONFIG_USB_CONTROLLERS_NUMBER", None)
	usbControllersNumber.setLabel("Number of Instances")
	usbControllersNumber.setMin(1)
	usbControllersNumber.setMax(2)
	usbControllersNumber.setDefaultValue(usbmaxcontrollers)
	usbControllersNumber.setVisible(False)

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
	usbDriverHostAttachDebounce.setVisible(True)
	usbDriverHostAttachDebounce.setDescription("Set USB Host Attach De-bounce duration")
	usbDriverHostAttachDebounce.setDefaultValue(500)
	
	# USB Driver Host mode Reset Duration
	usbDriverHostResetDuration = usbPeripheralComponentCommon.createIntegerSymbol("USB_DRV_HOST_RESET_DUARTION", None)
	usbDriverHostResetDuration.setLabel("USB Host Reset Duration (mSec)")
	usbDriverHostResetDuration.setVisible(True)
	usbDriverHostResetDuration.setDescription("Set USB Host Attach De-bounce duration")
	usbDriverHostResetDuration.setDefaultValue(100)
	
	if any(x in Variables.get("__PROCESSOR") for x in ["PIC32MK" , "PIC32MX" ]):
		plib_usbfs_header_h = usbPeripheralComponentCommon.createFileSymbol("PLIB_USBFS_HEADER_H", None)
		plib_usbfs_header_h.setSourcePath( usbDriverPath + "usbfs/src/plib_usbfs_header.h.ftl")
		plib_usbfs_header_h.setOutputName("plib_usbfs_header.h")
		plib_usbfs_header_h.setDestPath("/driver/usb/usbfs/src")
		plib_usbfs_header_h.setProjectPath("config/" + configName + "/driver/usb/usbfs/src/")
		plib_usbfs_header_h.setType("SOURCE")
		plib_usbfs_header_h.setMarkup(True)
		plib_usbfs_header_h.setOverwrite(True)
	
	################################################
	# system_config.h file for USB Driver
	################################################
	usbDriverSystemConfigFile = usbPeripheralComponentCommon.createFileSymbol(None, None)
	usbDriverSystemConfigFile.setType("STRING")
	usbDriverSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_MIDDLEWARE_CONFIGURATION")
	usbDriverSystemConfigFile.setSourcePath(sourcePath + "system_config.h.driver.ftl")
	usbDriverSystemConfigFile.setMarkup(True)
	usbDriverSystemConfigFile.setOverwrite(True)
	
	usbPeripheralHostSupport = usbPeripheralComponentCommon.createBooleanSymbol("DRV_USBFS_MULTI_HOST_SUPPORT", None)
	usbPeripheralHostSupport.setLabel("HOST")
	usbPeripheralHostSupport.setDefaultValue(False)
	usbPeripheralHostSupport.setVisible(False)
	usbPeripheralHostSupport.setReadOnly(True)
	
	usbPeripheralDeviceSupport= usbPeripheralComponentCommon.createBooleanSymbol("DRV_USBFS_MULTI_DEVICE_SUPPORT", None)
	usbPeripheralDeviceSupport.setLabel("DEVICE")
	usbPeripheralDeviceSupport.setDefaultValue(True)
	usbPeripheralDeviceSupport.setVisible(False)
	usbPeripheralDeviceSupport.setReadOnly(True)
	
	drvUsbHsV1HostSourceFile = usbPeripheralComponentCommon.createFileSymbol("DRV_USBFS_COMMON_C_SOURCE", None)
	drvUsbHsV1HostSourceFile.setSourcePath(usbDriverPath + "usbfs/src/drv_usbfs.c.ftl")
	drvUsbHsV1HostSourceFile.setOutputName("drv_usbfs.c")
	drvUsbHsV1HostSourceFile.setDestPath(usbDriverProjectPath + "usbfs/src")
	drvUsbHsV1HostSourceFile.setProjectPath("config/" + configName + usbDriverProjectPath + "usbfs/src/")
	drvUsbHsV1HostSourceFile.setType("SOURCE")
	drvUsbHsV1HostSourceFile.setMarkup(True)
	drvUsbHsV1HostSourceFile.setOverwrite(True)
	drvUsbIsrEntry = usbPeripheralComponentCommon.createListSymbol("LIST_DRV_USB_ISR_ENTRY", None)
	
	
