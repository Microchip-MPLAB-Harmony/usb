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
def loadModule():
    # Initially Set all USB Library modules to False. We will enable each
    # Library Module depending on the MCU/MPU selected.  
    loadUSBHostLayer = False
    loadUSBHostCDC = False
    loadUSBHostMSD = False
    loadUSBHostHID = False
    loadUSBHostAudio = False
    loadUSBHostGeneric = False	
    
    loadUSBDeviceLayer = False
    loadUSBDeviceCDC = False
    loadUSBDeviceHID = False
    loadUSBDeviceAudio = False
    loadUSBDeviceMSD = False
    loadUSBDeviceVendor = False 
    loadUSBDevicePrinter = False  
    #Below variable indicates if the part is PIC32CZ CA70 or not
    isPIC32CZ_CA70 = False
    # Below variables used to help using different capability names for SAMA5D2 
    USBDeviceDriverCapabilityName = "DRV_USB" 
    USBHostDriverCapabilityName = "DRV_USB" 
    availablePeripherals = []
    usbControllersNumber = 0
    if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK" , "PIC32MX" , "PIC32MM"]):
        modules = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals").getChildren()
        for module in range(len(modules)):
            instances = modules[module].getChildren()
            for instance in range(len(instances)):
                if str(instances[instance].getAttribute("name")) == "USB":
                    usbRegGroup = ATDF.getNode('/avr-tools-device-file/modules/module@[name="USB"]/register-group@[name="USB"]').getChildren()
                    usbIndex = 1
                    for register in usbRegGroup:
                        regName = str(register.getAttribute("name"))
                        usbInstance = "U" + str(usbIndex) + "CON"
                        if regName == usbInstance:
                            availablePeripherals.append("USB" + str(usbIndex))
                            usbIndex += 1
                            usbControllersNumber += 1
                else:
                    availablePeripherals.append(str(instances[instance].getAttribute("name")))

    if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32CK"]):
        modules = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals").getChildren()
        for module in range(len(modules)):
            if (modules[module].getAttribute("name")) == "USB":
                instancesFS = modules[module].getChildren()
                usbControllersNumberFS = len(instancesFS)
            if (modules[module].getAttribute("name")) == "USBHS":
                instancesHS = modules[module].getChildren()
                usbControllersNumberHS = len(instancesHS)

        if usbControllersNumberFS != None and usbControllersNumberFS > 0:
            # Create USB Full Speed Driver Component 
            usbDriverComponentFS =  Module.CreateComponent("drv_usbfs", "USB Full Speed Driver", "/USB/Drivers", "config/usbfs_v1_driver.py")
            usbDriverComponentFS.setHelpKeyword("USB_Common_Driver_Interface")
            usbDriverComponentFS.addCapability("DRV_USB", "DRV_USB")
            usbDriverComponentFS.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)

        if usbControllersNumberHS != None and usbControllersNumberHS > 0:
            # Create USB High Speed Driver Component
            usbDriverComponentHS =  Module.CreateComponent("drv_usbhs", "USB High Speed Driver", "/USB/Drivers", "config/usbhs_driver.py")
            usbDriverComponentHS.setHelpKeyword("USB_Common_Driver_Interface")
            usbDriverComponentHS.addMultiCapability("DRV_USB", "DRV_USB", "DRV_USB")
            usbDriverComponentHS.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)

        # Enable USB Library modules 
        loadUSBHostLayer = True
        loadUSBHostCDC = True
        loadUSBHostMSD = True
        loadUSBHostHID = True
        loadUSBHostAudio = True
        loadUSBHostGeneric = True

        loadUSBDeviceLayer = True
        loadUSBDeviceCDC = True
        loadUSBDeviceHID = True
        loadUSBDeviceAudio = True
        loadUSBDeviceMSD = True
        loadUSBDeviceVendor = True
        loadUSBDevicePrinter = True

    if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32CZ"]):
        modules = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals").getChildren()
        for module in range(len(modules)):
            if (modules[module].getAttribute("name")) == "USBHS":
                instances = modules[module].getChildren()
                usbControllersNumber = len(instances)
                # Module id "11292" indicates usb peripheal used in PIC32CZCA70
                isPIC32CZ_CA70  = True if modules[module].getAttribute("id") == "11292" else False
        if usbControllersNumber != None and usbControllersNumber > 0 and isPIC32CZ_CA70  == False:
            # Create USB Peripheral 1 Component 
            usbPeripheralComponent1 =  Module.CreateComponent("peripheral_usb_0", "USBHS0", "/USB/Peripherals/USB", "config/usb_multi_controller/usb_peripheral.py")
            usbPeripheralComponent1.setHelpKeyword("USB_Common_Driver_Interface")
            usbPeripheralComponent1.addCapability("USBHS0", "USBHS")

            if usbControllersNumber != None and usbControllersNumber > 1:
                # Create USB Peripheral 2 Component
                usbPeripheralComponent2 =  Module.CreateComponent("peripheral_usb_1", "USBHS1", "/USB/Peripherals/USB", "config/usb_multi_controller/usb_peripheral.py")
                usbPeripheralComponent2.setHelpKeyword("USB_Common_Driver_Interface")
                usbPeripheralComponent2.addCapability("USBHS1", "USBHS")

            # Create USB High Speed Driver Component
            usbDriverComponent =  Module.CreateGeneratorComponent("drv_usbhs_index", "USB High Speed Driver", "/USB/Drivers", "config/usb_multi_controller/usbhs_common.py", "config/usb_multi_controller/usbhs_driver.py")
            usbDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
            usbDriverComponent.addMultiCapability("DRV_USB", "DRV_USB", "DRV_USB")
            usbDriverComponent.addDependency("usb_peripheral_dependency", "USBHS", False, True)
        elif isPIC32CZ_CA70  == True:
            usbDriverComponent =  Module.CreateComponent("drv_usbhs_v1", "USB High Speed Driver", "/USB/Drivers", "config/usbhs_driver.py")
            usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
            usbDriverComponent.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
            
        # Enable USB Library modules 
        loadUSBHostLayer = True
        loadUSBHostCDC = True
        loadUSBHostMSD = True
        loadUSBHostHID = True
        loadUSBHostAudio = True
        loadUSBHostGeneric = True

        loadUSBDeviceLayer = True
        loadUSBDeviceCDC = True
        loadUSBDeviceHID = True
        loadUSBDeviceAudio = True
        loadUSBDeviceMSD = True
        loadUSBDeviceVendor = True
        loadUSBDevicePrinter = True

    if any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2", "SAM9X60", "SAM9X7", "SAMA7"]):
        # Create USB High Speed Host Port Driver Component for SAMA5D2 & SAM9X60
        usbUhpHsDriverComponent =  Module.CreateComponent("drv_usbhs_v1", "USB Host Port HS Driver", "/USB/Drivers", "config/usb_uhphs_driver.py")
        usbUhpHsDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
        usbUhpHsDriverComponent.addCapability("DRV_UHPHS", "DRV_UHPHS", True)
        usbUhpHsDriverComponent.addDependency("usb_uhphs_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        
        # Use different name for UHPHS Driver 
        USBHostDriverCapabilityName = "DRV_UHPHS"
        
        # Load USB Host Layer Components  
        loadUSBHostLayer = True 
        loadUSBHostCDC = True
        loadUSBHostMSD = True
        loadUSBHostHID = True 
        loadUSBHostGeneric = True
        
        # Create USB Device Port High Speed (UDPHS) Driver Component for SAMA5D2 & SAM9X60
        usbDevicePortHighSpeedDriverComponent =  Module.CreateComponent("drv_usb_udphs", "USB UDPHS Device Driver", "/USB/Drivers", "config/usb_udphs_driver.py")
        usbDevicePortHighSpeedDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
        usbDevicePortHighSpeedDriverComponent.addCapability("DRV_UDPHS", "DRV_UDPHS",True)
        
        # Add Generic Dependency on Core Service
        usbDevicePortHighSpeedDriverComponent.addDependency("usb_udphs_HarmonyCoreDependency", "Core Service", "Core Service", True, True)

        USBDeviceDriverCapabilityName = "DRV_UDPHS"
            
        # Load USB Device Layer Components 
        loadUSBDeviceLayer = True
        loadUSBDeviceCDC = True
        loadUSBDeviceHID = True
        loadUSBDeviceMSD = True
        loadUSBDeviceVendor = True 
        loadUSBDevicePrinter = True 
    elif any(x in Variables.get("__PROCESSOR") for x in ["SAMD21","SAMDA1", "SAMD5", "SAME5", "LAN9255" ,"SAML21", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "PIC32MZ1025W", "PIC32CX2051BZ6", "PIC32WM_BZ6204", "WBZ653", "PIC32MZ2051W", "WFI32E01", "WFI32E02", "WFI32E03", "PIC32CM", "PIC32CX"]):
        # Create USB Full Speed Driver Component
        usbDriverComponent =  Module.CreateComponent("drv_usbfs_v1", "USB Full Speed Driver", "/USB/Drivers", "config/usbfs_v1_driver.py")
        usbDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
        usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
        usbDriverComponent.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        
        # Enable USB Library modules 
        loadUSBHostLayer = True
        loadUSBHostCDC = True
        loadUSBHostMSD = True
        loadUSBHostHID = True
        loadUSBHostAudio = True
        loadUSBHostGeneric = True
    
        loadUSBDeviceLayer = True
        loadUSBDeviceCDC = True
        loadUSBDeviceHID = True
        loadUSBDeviceAudio = True
        loadUSBDeviceMSD = True
        loadUSBDeviceVendor = True
        loadUSBDevicePrinter = True 

    elif any(x in Variables.get("__PROCESSOR") for x in ["SAMG55"]):
        # Create USB High Speed Host Port Driver Component for SAMG55
        usbUhpHsDriverComponent =  Module.CreateComponent("drv_usbhs_v1", "USB Host Port Driver", "/USB/Drivers", "config/usb_uhp_driver.py")
        usbUhpHsDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
        usbUhpHsDriverComponent.addCapability("DRV_UHPHS", "DRV_UHP", True)
        usbUhpHsDriverComponent.addDependency("usb_uhphs_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        
        # Use different name for UHPHS Driver 
        USBHostDriverCapabilityName = "DRV_UHP"
        
        
        # Enable USB Library modules 
        loadUSBHostLayer = True
        loadUSBHostCDC = True
        loadUSBHostMSD = True
        loadUSBHostHID = True
        loadUSBHostAudio = True 
        loadUSBHostGeneric = True

        # Create USB Device Port (UDP) Full Speed Driver Component for SAMG55
        usbUdpDriverComponent =  Module.CreateComponent("drv_usbdp", "USB UDP Device Driver", "/USB/Drivers", "config/usbdp_driver.py")
        usbUdpDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
        usbUdpDriverComponent.addCapability("DRV_USBDP", "DRV_USBDP",True)

        # Add Generic Dependency on Core Service
        usbUdpDriverComponent.addDependency("usbdp_HarmonyCoreDependency", "Core Service", "Core Service", True, True)

        USBDeviceDriverCapabilityName = "DRV_USBDP"

        # Load USB Device Layer Components
        loadUSBDeviceLayer = True
        loadUSBDeviceCDC = True
        loadUSBDeviceHID = True
        loadUSBDeviceMSD = True
        loadUSBDeviceVendor = True
        loadUSBDevicePrinter = True
        loadUSBDeviceAudio = True

    elif any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70", "PIC32MZ"]):
        # Create USB High Speed Driver Component
        usbDriverComponent =  Module.CreateComponent("drv_usbhs_v1", "USB High Speed Driver", "/USB/Drivers", "config/usbhs_driver.py")
        usbDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
        usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
        usbDriverComponent.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        
        # Enable USB Library modules 
        loadUSBHostLayer = True
        loadUSBHostCDC = True
        loadUSBHostMSD = True
        loadUSBHostHID = True
        loadUSBHostAudio = True
        loadUSBHostGeneric = True
    
        loadUSBDeviceLayer = True
        loadUSBDeviceCDC = True
        loadUSBDeviceHID = True
        loadUSBDeviceAudio = True
        loadUSBDeviceMSD = True
        loadUSBDeviceVendor = True
        loadUSBDevicePrinter = True 

    elif any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MX2", "PIC32MX3", "PIC32MX4", "PIC32MX5", "PIC32MX6", "PIC32MX7", "PIC32MM"]):
        if usbControllersNumber != None and usbControllersNumber > 0:
            # Create USB Full Speed Driver Component
            usbDriverComponent =  Module.CreateComponent("drv_usbfs_v1", "USB Full Speed Driver", "/USB/Drivers", "config/usbfs_v1_driver.py")
            usbDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
            usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
            usbDriverComponent.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        
            # Enable USB Library modules 
            loadUSBHostLayer = True
            loadUSBHostCDC = True
            loadUSBHostMSD = True
            loadUSBHostHID = True
            loadUSBHostAudio = True 
            loadUSBHostGeneric = True
        
            loadUSBDeviceLayer = True
            loadUSBDeviceCDC = True
            loadUSBDeviceHID = True
            loadUSBDeviceAudio = True
            loadUSBDeviceMSD = True
            loadUSBDeviceVendor = True
            loadUSBDevicePrinter = True

    elif any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK"]):
        if usbControllersNumber != None and usbControllersNumber > 0:
            # Create USB Peripheral 1 Component 
            usbPeripheralComponent1 =  Module.CreateComponent("peripheral_usb_1", "USB1", "/Peripherals/USB", "config/usb_multi_controller/usb_peripheral.py")
            usbPeripheralComponent1.setHelpKeyword("USB_Common_Driver_Interface")
            usbPeripheralComponent1.addCapability("USB_1", "USB")
            
            if usbControllersNumber != None and usbControllersNumber > 1:
                # Create USB Peripheral 2 Component
                usbPeripheralComponent2 =  Module.CreateComponent("peripheral_usb_2", "USB2", "/Peripherals/USB", "config/usb_multi_controller/usb_peripheral.py")
                usbPeripheralComponent2.setHelpKeyword("USB_Common_Driver_Interface")
                usbPeripheralComponent2.addCapability("USB_2", "USB")
        
            # Create USB Full Speed Driver Component
            usbDriverComponent =  Module.CreateGeneratorComponent("drv_usbfs_index", "USB Full Speed Driver", "/USB/Drivers", "config/usb_multi_controller/usbfs_common.py", "config/usb_multi_controller/usbfs_driver.py")
            usbDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
            usbDriverComponent.addCapability("DRV_USB", "DRV_USB")
            usbDriverComponent.addDependency("usb_peripheral_dependency", "USB", False, True)
        
            # Enable USB Library modules 
            loadUSBHostLayer = True
            loadUSBHostCDC = True
            loadUSBHostMSD = True
            loadUSBHostHID = True
            loadUSBHostAudio = True 
            loadUSBHostGeneric = True
        
            loadUSBDeviceLayer = True
            loadUSBDeviceCDC = True
            loadUSBDeviceHID = True
            loadUSBDeviceAudio = True
            loadUSBDeviceMSD = True
            loadUSBDeviceVendor = True
            loadUSBDevicePrinter = True
            
    elif any(x in Variables.get("__PROCESSOR") for x in ["SAML22", "SAMD11"]):
        # Create USB Full Speed Driver Component
        usbDriverComponent =  Module.CreateComponent("drv_usbfs_v1", "USB Full Speed Driver", "/USB/Drivers", "config/usbfs_v1_driver.py")
        usbDriverComponent.setHelpKeyword("USB_Common_Driver_Interface")
        usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
        usbDriverComponent.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        
        
        loadUSBDeviceLayer = True
        loadUSBDeviceCDC = True
        loadUSBDeviceHID = True
        loadUSBDeviceAudio = True
        loadUSBDeviceMSD = True
        loadUSBDeviceVendor = True
        loadUSBDevicePrinter = True  
        
    # Create USB Device Layer Component
    if  loadUSBDeviceLayer == True:
        if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK", "PIC32CZ", "PIC32CK"]) and isPIC32CZ_CA70  == False:
            usbDeviceComponent =  Module.CreateGeneratorComponent("usb_device", "USB Device Layer", "/USB/Device Stack", "config/usb_multi_controller/usb_device_common.py", "config/usb_multi_controller/usb_device.py")
            usbDeviceComponent.setHelpKeyword("USB_DEVICE_LAYER_LIBRARY")
            usbDeviceComponent.addDependency("usb_driver_dependency", "DRV_USB", False, True)
            usbDeviceComponent.addMultiCapability("usb_device", "USB_DEVICE", "USB_DEVICE")
        else: 
            usbDeviceComponent = Module.CreateSharedComponent("usb_device", "USB Device Layer", "/USB/Device Stack", "config/usb_device.py")
            usbDeviceComponent.setHelpKeyword("USB_DEVICE_LAYER_LIBRARY")
            usbDeviceComponent.addDependency("usb_driver_dependency", USBDeviceDriverCapabilityName, True, True)
            usbDeviceComponent.addCapability("USB Device", "USB_DEVICE", True)
    
    # Create USB Device CDC Function driver Component 
    if loadUSBDeviceCDC == True:
        if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK", "PIC32CZ", "PIC32CK"])and isPIC32CZ_CA70  == False:
            usbDeviceCdcComponent = Module.CreateGeneratorComponent("usb_device_cdc", "CDC Function Driver", "/USB/Device Stack", "config/usb_device_cdc_common.py", "config/usb_multi_controller/usb_device_cdc.py")
            usbDeviceCdcComponent.setHelpKeyword("USB_CDC_DEVICE_LIBRARY")
            usbDeviceCdcComponent.addDependency("usb_device_dependency", "USB_DEVICE", False, True)
            usbDeviceCdcComponent.addCapability("USB Device", "USB_DEVICE_CDC")
        else:   
            usbDeviceCdcComponent = Module.CreateGeneratorComponent("usb_device_cdc", "CDC Function Driver", "/USB/Device Stack", "config/usb_device_cdc_common.py", "config/usb_device_cdc.py")
            usbDeviceCdcComponent.setHelpKeyword("USB_CDC_DEVICE_LIBRARY")
            usbDeviceCdcComponent.addDependency("usb_device_dependency", "USB_DEVICE", True, True)
            usbDeviceCdcComponent.addCapability("USB Device", "USB_DEVICE_CDC")
    
    # Create USB Device Vendor Component 
    if loadUSBDeviceVendor == True:
        if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK", "PIC32CZ", "PIC32CK"])and isPIC32CZ_CA70  == False:
            usbDeviceVendorComponent = Module.CreateGeneratorComponent("usb_device_vendor", "Vendor Function", "/USB/Device Stack", "config/usb_device_vendor_common.py", "config/usb_multi_controller/usb_device_vendor.py")
            usbDeviceVendorComponent.setHelpKeyword("USB_GENERIC_USB_LIBRARY")
            usbDeviceVendorComponent.addDependency("usb_device_dependency", "USB_DEVICE" , False, True)
            usbDeviceVendorComponent.addCapability("USB Device", "USB_DEVICE_VENDOR")
        else:
            usbDeviceVendorComponent = Module.CreateGeneratorComponent("usb_device_vendor", "Vendor Function", "/USB/Device Stack", "config/usb_device_vendor_common.py", "config/usb_device_vendor.py")
            usbDeviceVendorComponent.setHelpKeyword("USB_GENERIC_USB_LIBRARY")
            usbDeviceVendorComponent.addDependency("usb_device_dependency", "USB_DEVICE" , True, True)
            usbDeviceVendorComponent.addCapability("USB Device", "USB_DEVICE_VENDOR")
    
    # Create USB Device Audio Component 
    if loadUSBDeviceAudio == True:
        if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK", "PIC32CZ", "PIC32CK"])and isPIC32CZ_CA70  == False:
            usbDeviceAudioComponent = Module.CreateGeneratorComponent("usb_device_audio", "Audio Function Driver", "/USB/Device Stack", "config/usb_device_audio_common.py", "config/usb_multi_controller/usb_device_audio.py")
            usbDeviceAudioComponent.setHelpKeyword("USB_AUDIO_V_1_0_DEVICE_LIBRARY")
            usbDeviceAudioComponent.addDependency("usb_device_dependency", "USB_DEVICE" , False, True)
            usbDeviceAudioComponent.addCapability("USB Device", "USB_DEVICE_AUDIO")
        else:
            usbDeviceAudioComponent = Module.CreateGeneratorComponent("usb_device_audio", "Audio Function Driver", "/USB/Device Stack", "config/usb_device_audio_common.py", "config/usb_device_audio.py")
            usbDeviceAudioComponent.setHelpKeyword("USB_AUDIO_V_1_0_DEVICE_LIBRARY")
            usbDeviceAudioComponent.addDependency("usb_device_dependency", "USB_DEVICE" , True, True)
            usbDeviceAudioComponent.addCapability("USB Device", "USB_DEVICE_AUDIO")
        
    # Create USB Device HID Component 
    if loadUSBDeviceHID == True:
        if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK", "PIC32CZ", "PIC32CK"])and isPIC32CZ_CA70  == False:
            usbDeviceHidComponent = Module.CreateGeneratorComponent("usb_device_hid", "HID Function Driver", "/USB/Device Stack", "config/usb_device_hid_common.py", "config/usb_multi_controller/usb_device_hid.py")
            usbDeviceHidComponent.setHelpKeyword("USB_HID_DEVICE_LIBRARY")
            usbDeviceHidComponent.addDependency("usb_device_dependency", "USB_DEVICE" , False, True)
            usbDeviceHidComponent.addCapability("USB Device", "USB_DEVICE_HID")
        else:
            usbDeviceHidComponent = Module.CreateGeneratorComponent("usb_device_hid", "HID Function Driver", "/USB/Device Stack", "config/usb_device_hid_common.py", "config/usb_device_hid.py")
            usbDeviceHidComponent.setHelpKeyword("USB_HID_DEVICE_LIBRARY")
            usbDeviceHidComponent.addDependency("usb_device_dependency", "USB_DEVICE" , True, True)
            usbDeviceHidComponent.addCapability("USB Device", "USB_DEVICE_HID")
    
    # Create USB Device MSD Component 
    if loadUSBDeviceMSD == True:
        if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK", "PIC32CZ", "PIC32CK"])and isPIC32CZ_CA70  == False:
            usbDeviceMsdComponent = Module.CreateGeneratorComponent("usb_device_msd", "MSD Function Driver", "/USB/Device Stack", "config/usb_device_msd_common.py", "config/usb_multi_controller/usb_device_msd.py")
            usbDeviceMsdComponent.setHelpKeyword("USB_MSD_DEVICE_LIBRARY")
            usbDeviceMsdComponent.addDependency("usb_device_dependency", "USB_DEVICE", False, True)
            usbDeviceMsdComponent.addMultiDependency("usb_device_msd_media_dependency", "DRV_MEDIA", None, True)
            usbDeviceMsdComponent.addCapability("USB Device", "USB_DEVICE_MSD")
        else:
            usbDeviceMsdComponent = Module.CreateGeneratorComponent("usb_device_msd", "MSD Function Driver", "/USB/Device Stack", "config/usb_device_msd_common.py", "config/usb_device_msd.py")
            usbDeviceMsdComponent.setHelpKeyword("USB_MSD_DEVICE_LIBRARY")
            usbDeviceMsdComponent.addDependency("usb_device_dependency", "USB_DEVICE", True, True)
            usbDeviceMsdComponent.addMultiDependency("usb_device_msd_media_dependency", "DRV_MEDIA", None, True)
            usbDeviceMsdComponent.addCapability("USB Device", "USB_DEVICE_MSD")

    # Create USB Device Printer Component 
    if loadUSBDevicePrinter == True:
        if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK", "PIC32CZ", "PIC32CK"])and isPIC32CZ_CA70  == False:
            usbDevicePrinterComponent = Module.CreateGeneratorComponent("usb_device_printer", "Printer Function Driver", "/USB/Device Stack", "config/usb_device_printer_common.py", "config/usb_multi_controller/usb_device_printer.py")
            usbDevicePrinterComponent.setHelpKeyword("USB_DEVICE_PRINTER_LIBRARY")
            usbDevicePrinterComponent.addDependency("usb_device_dependency", "USB_DEVICE", False, True)
            usbDevicePrinterComponent.addCapability("USB Device", "USB_DEVICE_PRINTER") 
        else:
            usbDevicePrinterComponent = Module.CreateGeneratorComponent("usb_device_printer", "Printer Function Driver", "/USB/Device Stack", "config/usb_device_printer_common.py", "config/usb_device_printer.py")
            usbDevicePrinterComponent.setHelpKeyword("USB_DEVICE_PRINTER_LIBRARY")
            usbDevicePrinterComponent.addDependency("usb_device_dependency", "USB_DEVICE", True, True)
            usbDevicePrinterComponent.addCapability("USB Device", "USB_DEVICE_PRINTER")     
    
    # Create USB Host Layer Component   
    if loadUSBHostLayer == True:
        if any(x in Variables.get("__PROCESSOR") for x in [ "PIC32MK", "PIC32CZ", "PIC32CK"])and isPIC32CZ_CA70  == False:
            usbHostComponent = Module.CreateSharedComponent("usb_host", "Host Layer", "/USB/Host Stack", "config/usb_multi_controller/usb_host.py")
            usbHostComponent.setHelpKeyword("USB_HOST_LAYER_LIBRARY")
            usbHostComponent.addMultiDependency("usb_driver_dependency", "DRV_USB", None, True)
            usbHostComponent.addDependency("usb_host_tmr_dependency", "SYS_TIME", True, True)
            usbHostComponent.addCapability("usb_host", "USB_HOST", True)
        else:
            usbHostComponent = Module.CreateSharedComponent("usb_host", "Host Layer", "/USB/Host Stack", "config/usb_host.py")
            usbHostComponent.setHelpKeyword("USB_HOST_LAYER_LIBRARY")
            usbHostComponent.addDependency("usb_driver_dependency", USBHostDriverCapabilityName, True, True)
            usbHostComponent.addDependency("usb_host_tmr_dependency", "SYS_TIME", True, True)
            usbHostComponent.addCapability("usb_host", "USB_HOST", True)
    
    # Create USB Host Stack MSD Component 
    if loadUSBHostMSD == True:
        usbHostMsdComponent = Module.CreateComponent("usb_host_msd", "MSD Client Driver", "/USB/Host Stack","config/usb_host_msd.py")
        usbHostMsdComponent.setHelpKeyword("USB_MSD_HOST_CLIENT_DRIVER_LIBRARY")
        usbHostMsdComponent.addDependency("usb_host_dependency", "USB_HOST", True, True)
        usbHostMsdComponent.addCapability("USB Host MSD", "DRV_MEDIA")
    
    # Create USB Host Stack CDC Component 
    if loadUSBHostCDC == True:  
        usbHostCdcComponent = Module.CreateComponent("usb_host_cdc", "CDC Client Driver", "/USB/Host Stack", "config/usb_host_cdc.py")
        usbHostCdcComponent.setHelpKeyword("USB_CDC_HOST_LIBRARY")
        usbHostCdcComponent.addDependency("usb_host_dependency", "USB_HOST", True, True)
    
    # Create USB Host Stack HID Component   
    if loadUSBHostHID == True:
        usbHostHidComponent = Module.CreateComponent("usb_host_hid", "HID Client Driver", "/USB/Host Stack", "config/usb_host_hid.py")
        usbHostHidComponent.setHelpKeyword("USB_HID_HOST_MOUSE_DRIVER_LIBRARY")
        usbHostHidComponent.addDependency("usb_host_dependency", "USB_HOST", True, True)
    
    # Create USB Host Stack Audio Component 
    if loadUSBHostAudio == True:
        usbHostAudioComponent = Module.CreateComponent("usb_host_audio", "Audio Client Driver", "/USB/Host Stack", "config/usb_host_audio.py")
        usbHostAudioComponent.setHelpKeyword("USB_AUDIO_V_1_0_HOST_CLIENT_DRIVER_LIBRARY")
        usbHostAudioComponent.addDependency("usb_host_dependency", "USB_HOST", True, True)

    # Create USB Host Stack Generic Component 
    if loadUSBHostGeneric == True:  
        usbHostGenericComponent = Module.CreateComponent("usb_host_generic", "Generic Client Driver", "/USB/Host Stack", "config/usb_host_generic.py")
        usbHostGenericComponent.setHelpKeyword("USB_GENERIC_HOST_CLIENT_DRIVER_LIBRARY")
        usbHostGenericComponent.addDependency("usb_host_dependency", "USB_HOST", True, True)
        
