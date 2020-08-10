![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB Harmony 3 Release Notes
## USB Release v3.6.1
### NEW FEATURES
- **New part support** - N/A

- **New Features and Enhancements**
   * Dual Channel USB to UART application for ATSAMD21 Xplained Pro Evaluation kit.   
   * USB Host Hub demo for SAM9X60 and SAMA5D25 Microcprocessors.
  

- **Development kit and demo application support** - The following table provides number of demo applications available for different development kits newly added in this release.

    | Development Kits                                                                                                                               | Number of applications |
    | ---                                                                                                                                            | --- |
    | [SAM D21 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMD21-XPRO)                     | 1 |
    | [ATSAM9X60-EK](https://www.microchip.com/design-centers/32-bit-mpus/microprocessors/sam9)                                                      | 2 |
     [SAMA5D2 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsama5d2c-xult)                             | 2 |
    
### Bug fixes
- Added High Speed Hub support for USB Host EHCI Driver.   

### Known Issues

The current known issues are as follows:

- Isochronous transfers are not supported with USB Host Port Driver(EHCI/OHCI) for SAMA5D2/SAM9X60 MCUs. The issue will be fixed in an upcoming release.

- USB Host Port High-Speed Driver for SAMA5D2/SAM9X60 MCUs supports only High Speed USB Hubs. Full speed Hubs are not supported. Only High Speed USB Devices could be attached to the Host. The issue will be fixed in an upcoming release.

- USB keyboard Host demonstration application on the SAMG55x microcontroller does not toggle the keyboard LED. This issue will be fixed in an upcoming release.

- Detaching a USB device from a SAMG55x Host when the enumeration is in progress causes the affects Plug N'Play operation. This issue will be fixed in an upcoming release.

- Isochronous tranfers are not tested with USB Device Port High Speed Driver(UDPHS) Driver.  The issue will be fixed in an upcoming release.

- Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the &quot;Configuring the Library&quot; section in the help documentation in the doc folder for this module instead.  Help is available in both CHM and PDF formats.

- In a case where an MPLAB Harmony USB Device Project requires multiple Function Driver instances in the project, the MPLAB Harmony Configurator(MHC) may generate incorrect USB descriptors if a USB Device function driver component is added and then removed from the MHC Project Graph.  Removing all the USB components from the project graph and starting over fixes this issue. The issue will be fixed in an upcoming release.

- IAR projects for SAM MCUs and MPUs build with warning messages. The issue will be fixed in an upcoming release.

- The following USB Host Stack functions are not implemented:
    - USB_HOST_BusResume 
    - USB_HOST_DeviceSuspend 
    - USB_HOST_DeviceResume 

- The USB Host Layer does not check for the Hub Tier Level. This feature will be available in a future release of MPLAB Harmony.
   
- The USB Host Layer will only enable the first configuration when there are multiple configurations. If there are no interface matches in the first configuration, this causes the device to become inoperative. Multiple configuration enabling will be activated in a future release.

- The MSD Host Client Driver has not been tested with Multi-LUN Mass Storage Device and USB Card Readers.

- The USB Host SCSI Block Driver, the CDC Client Driver, and the Audio Host Client Driver only support single-client operation. Multi-client operation will be enabled in a future release of MPLAB Harmony.

- USB HID Host Client driver has not been tested with multiple usage devices. Sending of output or feature report has not been tested.

- The USB Audio Host Client driver does not provide an implementation for the following functions:
    - USB_HOST_AUDIO_V1_DeviceObjHandleGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeRangeGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSubRangeNumbersGet 
    - USB_HOST_AUDIO_V1_StreamSamplingFrequencyGet 
    - USB_HOST_AUDIO_V1_TerminalIDGet 

- USB Host Stack has been tested only limited numbers of USB Flash drives.  
### Development Tools

* [MPLAB® X IDE v5.40](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.41](https://www.microchip.com/mplab/compilers)
* [IAR Embedded Workbench® for ARM® v8.5] (https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm)
* MPLAB® X IDE plug-ins:
* MPLAB® Harmony Configurator (MHC) v3.5.0 and above.
## USB Release v3.6.0
### NEW FEATURES
- **New part support** - N/A

- **New Features and Enhancements**
   * USB Device stack support for SAMG55 Microcontrollers.
   * USB Device applications for SAMG55 Microcontrollers.
   * USB Host transfer scheduler for SAMD2x/E5x/D5x/L21 Microcontroller families. 
   * USB Device MSD application with SD Card as media.
   * USB Device Composite demo with CDC and MSD SD Card. 
   
- All FreeRTOS based applications are tested with the CMSIS-FreeRTOS v10.3.0.

- **Development kit and demo application support** - The following table provides number of demo applications available for different development kits newly added in this release.

    | Development Kits                                                                                                                               | Number of applications |
    | ---                                                                                                                                            | --- |
    | [SAM D21 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMD21-XPRO)                     | 2 |
    | [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAME54-XPRO)                                 | 1 |
    | [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAME70-XULT)                   | 1 |
    | [SAM G55 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamg55-xpro)                                 | 3 |
    | [ATSAM9X60-EK](https://www.microchip.com/design-centers/32-bit-mpus/microprocessors/sam9)                                                      | 2 |
    
### Bug fixes
- Resolved Async transfer scheduling issues with USB Host EHCI Driver.   

### Known Issues

The current known issues are as follows:

- Isochronous transfers are not supported with USB Host Port Driver(EHCI/OHCI) for SAMA5D2/SAM9X60 MCUs. The issue will be fixed in an upcoming release.

- USB Host Port High-Speed Driver for SAMA5D2/SAM9X60 MCUs does not support USB Hub. The issue will be fixed in an upcoming release.

- USB keyboard Host demonstration application on the SAMG55x microcontroller does not toggle the keyboard LED. This issue will be fixed in an upcoming release.

- Detaching a USB device from a SAMG55x Host when the enumeration is in progress causes the affects Plug N'Play operation. This issue will be fixed in an upcoming release.

- Isochronous tranfers are not tested with USB Device Port High Speed Driver(UDPHS) Driver.  The issue will be fixed in an upcoming release.

- Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the &quot;Configuring the Library&quot; section in the help documentation in the doc folder for this module instead.  Help is available in both CHM and PDF formats.

- In a case where an MPLAB Harmony USB Device Project requires multiple Function Driver instances in the project, the MPLAB Harmony Configurator(MHC) may generate incorrect USB descriptors if a USB Device function driver component is added and then removed from the MHC Project Graph.  Removing all the USB components from the project graph and starting over fixes this issue. The issue will be fixed in an upcoming release.

- IAR projects for SAM MCUs and MPUs build with warning messages. The issue will be fixed in an upcoming release.

- The following USB Host Stack functions are not implemented:
    - USB_HOST_BusResume 
    - USB_HOST_DeviceSuspend 
    - USB_HOST_DeviceResume 

- The USB Host Layer does not check for the Hub Tier Level. This feature will be available in a future release of MPLAB Harmony.
   
- The USB Host Layer will only enable the first configuration when there are multiple configurations. If there are no interface matches in the first configuration, this causes the device to become inoperative. Multiple configuration enabling will be activated in a future release.

- The MSD Host Client Driver has not been tested with Multi-LUN Mass Storage Device and USB Card Readers.

- The USB Host SCSI Block Driver, the CDC Client Driver, and the Audio Host Client Driver only support single-client operation. Multi-client operation will be enabled in a future release of MPLAB Harmony.

- USB HID Host Client driver has not been tested with multiple usage devices. Sending of output or feature report has not been tested.

- The USB Audio Host Client driver does not provide an implementation for the following functions:
    - USB_HOST_AUDIO_V1_DeviceObjHandleGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeRangeGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSubRangeNumbersGet 
    - USB_HOST_AUDIO_V1_StreamSamplingFrequencyGet 
    - USB_HOST_AUDIO_V1_TerminalIDGet 

- USB Host Stack has been tested only limited numbers of USB Flash drives.  
### Development Tools

* [MPLAB® X IDE v5.40](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.41](https://www.microchip.com/mplab/compilers)
* [IAR Embedded Workbench® for ARM® v8.5] (https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm)
* MPLAB® X IDE plug-ins:
* MPLAB® Harmony Configurator (MHC) v3.5.0 and above.

## USB Release v3.5.0
### NEW FEATURES
- **New part support** - This release introduces initial support for [SAMG55](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-g-mcus), [SAM DA1](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-l-mcus), PIC32MK MCM and PIC32MZ W1 families of 32-bit microcontrollers.

- **New Features and Enhancements**
   * USB Device demo for SAMDA1 Microcontrollers.
   * USB Host stack support for SAMG55 Microcontrollers.
   * USB Dual Controller demo for PIC32MK MCM Microcontrollers.
   * Dual Role support for PIC32MZ Microcontrollers.
   * IAR Projects for SAM Microcontrollers.
   * USB Device and Host stack support for PIC32MZ W1 Microcontrollers.
   
- All FreeRTOS based applications are tested with the CMSIS-FreeRTOS v10.2.0.

- **Development kit and demo application support** - The following table provides number of demo application available for different development kits newly added in this release.

    | Development Kits                                                                                                                               | Number of applications |
    | ---                                                                                                                                            | --- |
    | [SAM D21 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMD21-XPRO)                     | 1 |
    | [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAME54-XPRO)                                 | 1 |
    | [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAME70-XULT)                   | 1 |
    | [SAM DA1 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/ATSAMDA1-XPRO)                          | 1 |
    | [SAM G55 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamg55-xpro)                                 | 7 |
    | PIC32MK MCM Curiosity Pro                                                    | 3 |
    | [PIC32MZ Embedded Connectivity with FPU (EF) Starter Kit](https://www.microchip.com/Developmenttools/ProductDetails/Dm320007)                  | 1 |
    | [ATSAM9X60-EK](https://www.microchip.com/design-centers/32-bit-mpus/microprocessors/sam9)                                                      | 1 |
    | PIC32MZ W1 Curiosity Board                                                                                                                     | 2 |
    


### Known Issues

The current known issues are as follows:

- Isochronous are not supported with USB Host Port Driver(EHCI/OHCI) on SAMA5D2. The issue will be fixed in an upcoming release.

- USB Host Port High Speed Driver on SAMA5D2 does not support USB Hub. The issue will be fixed in an upcoming release.

- Isochronous tranfers are not tested with USB Device Port High Speed Driver(UDPHS) Driver.  The issue will be fixed in an upcoming release.

- Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the &quot;Configuring the Library&quot; section in the help documentation in the doc folder for this module instead.  Help is available in both CHM and PDF formats.

- In a case where a MPLAB Harmony USB Device Project requires multiple Function Driver instances in the project, the MPLAB Harmony Configurator(MHC) may generate incorrect USB descriptors if a USB Device function driver component is added and then removed from the MHC Project Graph.  Removing all the USB components from project graph and starting over fixes this issue. The issue will be fixed in an upcoming release.

- IAR projects for SAM MCUs and MPUs builds with warning messages. The issue will be fixed in an upcoming release.

- The following USB Host Stack functions are not implemented:
    - USB_HOST_BusResume 
    - USB_HOST_DeviceSuspend 
    - USB_HOST_DeviceResume 

- The USB Host Layer does not check for the Hub Tier Level. This feature will be available in a future release of MPLAB Harmony.
   
- The USB Host Layer will only enable the first configuration when there are multiple configurations. If there are no interface matches in the first configuration, this causes the device to be become inoperative. Multiple configuration enabling will be activated in a future release of the of MPLAB Harmony.

- The MSD Host Client Driver has not been tested with Multi-LUN Mass Storage Device and USB Card Readers.

- The USB Host SCSI Block Driver, the CDC Client Driver, and the Audio Host Client Driver only support single-client operation. Multi-client operation will be enabled in a future release of MPLAB Harmony.

- USB HID Host Client driver has not been tested with multiple usage devices. Sending of output or feature report has not been tested.

- The USB Audio Host Client driver does not provide implementation for the following functions:
    - USB_HOST_AUDIO_V1_DeviceObjHandleGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeRangeGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSubRangeNumbersGet 
    - USB_HOST_AUDIO_V1_StreamSamplingFrequencyGet 
    - USB_HOST_AUDIO_V1_TerminalIDGet 

- USB Host Stack has been tested only limited numbers of USB Flash drives.  
### Development Tools

* [MPLAB® X IDE v5.35](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.40](https://www.microchip.com/mplab/compilers)
* [IAR Embedded Workbench® for ARM® v8.4] (https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm)
* MPLAB® X IDE plug-ins:
  * MPLAB® Harmony Configurator (MHC) v3.4.2 and above.

## USB Release v3.4.0
### NEW FEATURES
- **New part support** - This release introduces initial support for [SAMD11](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-g-mcus) family of 32-bit microcontrollers and [SAM9X60](https://www.microchip.com/design-centers/32-bit-mpus/microprocessors/sam9) family of 32-bit microprocessors.

- All FreeRTOS based applications are tested with the CMSIS-FreeRTOS v10.2.0.

- **Development kit and demo application support** - The following table provides number of demo application available for different development kits newly added in this release.

    | Development Kits                                                                                                                               | Number of applications |
    | ---                                                                                                                                            | --- |
    | [SAM D11 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsamd11-xpro)                                 | 4 |
    | [Curiosity PIC32MZ EF 2.0 Development Board](https://www.microchip.com/Developmenttools/ProductDetails/DM320209)                                 | 2 |
    | [PIC32MK GP Development Kit](https://www.microchip.com/developmenttools/ProductDetails/dm320106)                                               | 3 |
    | [SAMA5D2 Xplained Ultra Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsama5d2c-xult)                             | 2 |
    | [ATSAM9X60-EK](https://www.microchip.com/design-centers/32-bit-mpus/microprocessors/sam9)                                                      | 8 |
    


### Known Issues

The current known issues are as follows:

- Isochronous are not supported with USB Host Port Driver(EHCI/OHCI) on SAMA5D2. The issue will be fixed in an upcoming release.

- USB Host Port High SPeed Driver on SAMA5D2 does not support USB Hub. The issue will be fixed in an upcoming release.

- Isochronous tranfers are not tested with USB Device Port High Speed Driver(UDPHS) Driver.  The issue will be fixed in an upcoming release.

- Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the &quot;Configuring the Library&quot; section in the help documentation in the doc folder for this module instead.  Help is available in both CHM and PDF formats.

- In a case where a MPLAB Harmony USB Device Project requires multiple Function Driver instances in the project, the MPLAB Harmony Configurator(MHC) may generate incorrect USB descriptors if a USB Device function driver component is added and then removed from the MHC Project Graph.  Removing all the USB components from project graph and starting over fixes this issue. The issue will be fixed in an upcoming release.

- ATSAMA5D2C and SAM9X60 demo applications build with warning messages. The issue will be fixed in an upcoming release.

- The following USB Host Stack functions are not implemented:
    - USB_HOST_BusResume 
    - USB_HOST_DeviceSuspend 
    - USB_HOST_DeviceResume 

- The USB Host Layer does not check for the Hub Tier Level. This feature will be available in a future release of MPLAB Harmony.
   
- The USB Host Layer will only enable the first configuration when there are multiple configurations. If there are no interface matches in the first configuration, this causes the device to be become inoperative. Multiple configuration enabling will be activated in a future release of the of MPLAB Harmony.

- The MSD Host Client Driver has not been tested with Multi-LUN Mass Storage Device and USB Card Readers.

- The USB Host SCSI Block Driver, the CDC Client Driver, and the Audio Host Client Driver only support single-client operation. Multi-client operation will be enabled in a future release of MPLAB Harmony.

- USB HID Host Client driver has not been tested with multiple usage devices. Sending of output or feature report has not been tested.

- The USB Audio Host Client driver does not provide implementation for the following functions:
    - USB_HOST_AUDIO_V1_DeviceObjHandleGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeRangeGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSubRangeNumbersGet 
    - USB_HOST_AUDIO_V1_StreamSamplingFrequencyGet 
    - USB_HOST_AUDIO_V1_TerminalIDGet 

- USB Host Stack has been tested only limited numbers of USB Flash drives.  
### Development Tools

* [MPLAB® X IDE v5.25](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.30](https://www.microchip.com/mplab/compilers)
* [IAR Embedded Workbench® for ARM® v8.4] (https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm)
* MPLAB® X IDE plug-ins:
  * MPLAB® Harmony Configurator (MHC) v3.3.0.1 and above.


## USB Release v3.3.0
### NEW FEATURES
- **New part support** - This release introduces initial support for [SAML21](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-l-mcus),
[SAML22](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-l-mcus),
[PIC32MX 2/5](https://www.microchip.com/design-centers/32-bit/pic-32-bit-mcus/pic32mx-family),
[PIC32MX 3/4](https://www.microchip.com/design-centers/32-bit/pic-32-bit-mcus/pic32mx-family) and [PIC32MK](https://www.microchip.com/design-centers/32-bit/pic-32-bit-mcus/pic32mk-family) families of 32-bit microcontrollers.

- Added support for the USB Device Port High Speed Driver(UDPHS) on SAMA5D2. 

- Added support for the USB Full Speed driver(USBFS) on PIC32MX2/3/4/5. 

- Added support for the OHCI driver on SAMA5D2.

- Added USB Host Hub driver support on PIC32MZ Microcontroller. 

- All FreeRTOS based applications are tested with the CMSIS-FreeRTOS v10.2.0.

- **Development kit and demo application support** - The following table provides number of demo application available for different development kits newly added in this release.

    | Development kits | Bare-metal applications | RTOS applications |
    | --- | --- | --- |
    | [PIC32MX Curiosity Development Board](https://www.microchip.com/Developmenttools/ProductDetails/dm320103) | 2 | 2 |
    | [PIC32MK GP Development Kit](https://www.microchip.com/developmenttools/ProductDetails/dm320106) | 2 | 2 |
    | [SAM L21 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAML21-XPRO-B) | 2 | 0 |
    | [SAM L22 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAML22-XPRO-B) | 1 | 0 | 
    | [ATSAMA5D2C-XULT](https://www.microchip.com/Developmenttools/ProductDetails/ATSAMA5D2C-XULT) | 3 | 0 |
    | [PIC32MZ Embedded Graphics with Stacked DRAM (DA) Starter Kit (Crypto)](https://www.microchip.com/DevelopmentTools/ProductDetails/DM320010-C) | 2 | 0 |
    


### Known Issues

The current known issues are as follows:

- Periodic transfers (Isochronous/Interrupt) are not supported with USB Host Port Driver(EHCI/OHCI) on SAMA5D2. The issue will be fixed in an upcoming release.

- USB Host Port High SPeed Driver on SAMA5D2 does not support USB Hub. The issue will be fixed in an upcoming release.

- Isochronous tranfers are not tested with USB Device Port High Speed Driver(UDPHS) Driver.  The issue will be fixed in an upcoming release.

- Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the &quot;Configuring the Library&quot; section in the help documentation in the doc folder for this module instead.  Help is available in both CHM and PDF formats.

- USB Host Layer does not reset data toggle after sending an ENDPOINT_HALT Clear Feature. Certain USB Devices may not work due to this issue. The issue will be fixed in an upcoming release.

- In a case where a MPLAB Harmony USB Device Project requires multiple Function Driver instances in the project, the MPLAB Harmony Configurator(MHC) may generate incorrect USB descriptors if a USB Device function driver component is added and then removed from the MHC Project Graph.  Removing all the USB components from project graph and starting over fixes this issue. The issue will be fixed in an upcoming release.

- ATSAMA5D2C demo applications build with warning messages. The issue will be fixed in an upcoming release.

- MPLAB Harmony Configurator can be used to configure only USB1 peripheral on the PIC32 microcontroller. USB2 Peripheral is not supported by MHC. The issue will be fixed in an upcoming release.

- The following USB Host Stack functions are not implemented:
    - USB_HOST_BusResume 
    - USB_HOST_DeviceSuspend 
    - USB_HOST_DeviceResume 

- The USB Host Layer does not check for the Hub Tier Level. This feature will be available in a future release of MPLAB Harmony.
   
- The USB Host Layer will only enable the first configuration when there are multiple configurations. If there are no interface matches in the first configuration, this causes the device to be become inoperative. Multiple configuration enabling will be activated in a future release of the of MPLAB Harmony.

- The MSD Host Client Driver has not been tested with Multi-LUN Mass Storage Device and USB Card Readers.

- The USB Host SCSI Block Driver, the CDC Client Driver, and the Audio Host Client Driver only support single-client operation. Multi-client operation will be enabled in a future release of MPLAB Harmony.

- USB HID Host Client driver has not been tested with multiple usage devices. Sending of output or feature report has not been tested.

- The USB Audio Host Client driver does not provide implementation for the following functions:
    - USB_HOST_AUDIO_V1_DeviceObjHandleGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeRangeGet 
    - USB_HOST_AUDIO_V1_FeatureUnitChannelVolumeSubRangeNumbersGet 
    - USB_HOST_AUDIO_V1_StreamSamplingFrequencyGet 
    - USB_HOST_AUDIO_V1_TerminalIDGet 

- USB Host Stack has been tested only limited numbers of USB Flash drives.  
### Development Tools

* [MPLAB® X IDE v5.20](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.20](https://www.microchip.com/mplab/compilers)
* [IAR Embedded Workbench® for ARM® (v8.32 or above)](https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm)
* MPLAB® X IDE plug-ins:
  * MPLAB® Harmony Configurator (MHC) v3.3.0.1 and above.

## USB Release v3.2.2
### NEW FEATURES
- **USB Device Printer Function Driver** - This release adds USB Device printer function driver library. This release also adds Genric Text Only USB printer demo for [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAME54-XPRO).

### Known Issues

The current known issues are as follows:

- Programming and debugging through PKOB is not yet supported for PIC32MZ DA, need to use external debugger (ICD4)

- PIC32MZ DA(S) device family will be supported by next coming XC32 release.

- The ICD4 loads the reset line of the SAM V71 Xplained Ultra board. Do not press reset button on the Xplained Ultra board while ICD4 is connected to the board.

- Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the &quot;Configuring the Library&quot; section in the help documentation in the doc folder for this module instead.  Help is available in both CHM and PDF formats.

- USB HS Host Port Driver for SAMA5D2 will freeze its operation if the attached USB device responds with a STALL. Many USB Devices will not work due to this issue. This will be fixed in future releases. 

- USB Host Layer does not reset data toggle after sending an ENDPOINT_HALT Clear Feature. Certain USB Devices may not work due to this issue. 

- USB Host Mass Storage Client Driver for SAME70/V71/S70 will freeze its operation if the attached USB Mass Storage device responds with a STALL during a SCSI MODE_SENSE command. Few USB Devices will not work due to this issue. This will be fixed in future releases. 

- In a case where a MPLAB Harmony USB Device Project requires multiple Function Driver instances in the project, the MPLAB Harmony Configurator(MHC) may generate incorrect USB descriptors if a USB Device function driver component is added and then removed from the MHC Project Graph.  Removing all the USB components from project graph and starting over fixes this issue. The issue will be fixed in an upcoming release.


### Development Tools

* [MPLAB X IDE v5.15](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB XC32 C/C++ Compiler v2.15](https://www.microchip.com/mplab/compilers)
* MPLAB X IDE plug-ins:
  * MPLAB Harmony Configurator (MHC) v3.3.0.1.


## USB Release v3.2.1
### NEW FEATURES
- **New part support** - This release introduces initial support for [SAMA5D2](https://www.microchip.com/design-centers/32-bit-mpus/microprocessors/sama5/sama5d2-series) family of 32-bit microprocessors. 

| Development kits | Applications |
| --- | --- | --- |
| [ATSAMA5D2C-XULT](https://www.microchip.com/Developmenttools/ProductDetails/ATSAMA5D2C-XULT) | 4 |4|

### Known Issues

The current known issues are as follows:

- Programming and debugging through PKOB is not yet supported for PIC32MZ DA, need to use external debugger (ICD4)

- PIC32MZ DA(S) device family will be supported by next coming XC32 release.

- The ICD4 loads the reset line of the SAM V71 Xplained Ultra board. Do not press reset button on the Xplained Ultra board while ICD4 is connected to the board.

- Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the &quot;Configuring the Library&quot; section in the help documentation in the doc folder for this module instead.  Help is available in both CHM and PDF formats.

- USB HS Host Port Driver for SAMA5D2 will freeze its operation if the attached USB device responds with a STALL. Many USB Devices will not work due to this issue. This will be fixed in future releases. 

- USB Host Layer does not reset data toggle after sending an ENDPOINT_HALT Clear Feature. Certain USB Devices may not work due to this issue. 


### Development Tools

* [MPLAB X IDE v5.15](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB XC32 C/C++ Compiler v2.15](https://www.microchip.com/mplab/compilers)
* MPLAB X IDE plug-ins:
  * MPLAB Harmony Configurator (MHC) v3.2.0.0.


## USB Release v3.2.0
### NEW FEATURES
- **New part support** - This release introduces initial support for [PIC32MZ DA](https://www.microchip.com/design-centers/32-bit/pic-32-bit-mcus/pic32mz-da-family), [PIC32MZ EF](https://www.microchip.com/design-centers/32-bit/pic-32-bit-mcus/pic32mz-ef-family), [SAM E54](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-e-mcus), families of 32-bit microcontrollers. 

- **Development kit and demo application support** - The following table provides number of demo application available for different development kits newly added in this release.

| Development kits | Applications |
| --- | --- | --- |
| [PIC32MZ Embedded Graphics with Stacked DRAM (DA) Starter Kit (Crypto)](https://www.microchip.com/DevelopmentTools/ProductDetails/DM320010-C) | 2 |
| [PIC32MZ Embedded Connectivity with FPU(EF) Starter Kit](https://www.microchip.com/Developmenttools/ProductDetails/Dm320007) | 11 |
| [SAM E54 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/ATSAME54-XPRO) | 7 |

### Known Issues

The current known issues are as follows:

- Programming and debugging through PKOB is not yet supported for PIC32MZ DA, need to use external debugger (ICD4)

- PIC32MZ DA(S) device family will be supported by next coming XC32 release.

- The ICD4 loads the reset line of the SAM V71 Xplained Ultra board. Do not press reset button on the Xplained Ultra board while ICD4 is connected to the board.

- Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the &quot;Configuring the Library&quot; section in the help documentation in the doc folder for this module instead.  Help is available in both CHM and PDF formats.

- USB Host MSD (apps\host\msd_basic) demo has been tested only with limited number of pen drives. The list of tested pen drives are provided in the USB Help doc section  "USB MSD Host USB Pen Drive Tests" 


### Development Tools

* [MPLAB X IDE v5.15](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB XC32 C/C++ Compiler v2.15](https://www.microchip.com/mplab/compilers)
* MPLAB X IDE plug-ins:
  * MPLAB Harmony Configurator (MHC) v3.2.0.0.

## USB Release v3.1.0
### NEW FEATURES
- **New part support** - This release introduces initial support for [SAM D20/D21](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-d-mcus), [SAM S70](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-s-mcus), [SAM E70](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-e-mcus), [SAM V70/V71](https://www.microchip.com/design-centers/32-bit/sam-32-bit-mcus/sam-v-mcus) families of 32-bit microcontrollers.

- **USB Library** - The following table provides the list of USB Library components and Applications. 

| Type | Module Name |  Module Caption |
| --- | --- | --- |
| Driver | DRV\_USBHSV1 | USB High Speed Driver [Host and Device] for [SAMV70/V71/S70/E70] families of 32-bit microcontrollers  |
| Driver | DRV\_USBFSV1 | USB Full Speed Driver [Host and Device] for SAM D20/D21 families of 32-bit microcontrollers |
| Middleware | USB Device Layer | USB Device Layer Library  |
| Middleware | USB Audio v1.0 Function Driver | USB Audio 1.0 Device Library|
| Middleware | USB CDC Function Driver | USB CDC Device Library  |
| Middleware | USB HID Function Driver | USB HID Device Library  |
| Middleware | USB MSD Function Driver | USB MSD Device Library  |
| Middleware | USB Host Layer  | USB Host Layer Library  |
| Middleware | USB Audio v1.0 Client Driver | USB Audio 1.0 Host Library|
| Middleware | USB CDC Client Driver | USB CDC Host Library  |
| Middleware | USB HID Client Driver | USB HID Host Library  |
| Middleware | USB MSD Client Driver | USB MSD Host Library  |
| USB Device App | cdc_com_port_dual  | CDC Dual Serial COM Ports Emulation Demonstration  |
| USB Device App | cdc_com_port_single   | CDC Single Serial COM Port Emulation Demonstration   |
| USB Device App | cdc_serial_emulator  | CDC Serial Emulation Demonstration   |
| USB Device App | hid_basic   | Basic USB Human Interface Device (HID) Demonstration  |
| USB Device App | hid_keyboard    | USB HID Class Keyboard Device Demonstration   |
| USB Device App | hid_mouse    | USB HID Class Mouse Device Demonstration   |
| USB Device App | msd_basic    | USB Device Mass Storage Device (MSD) Demonstration |
| USB Host App | cdc_basic    | USB Host CDC Basic Demonstration    |
| USB Host App | cdc_msd    | USB Host CDC + MSD Basic Demonstration   |
| USB Host App | hid_basic_keyboard     | USB HID Host Keyboard Demonstration    |
| USB Host App | msd_basic     | USB MSD Host Simple Thumb Drive Demonstration   |

- **Development kit and demo application support** - The following table provides number of bare-metal and RTOS demo application available for different development kits.

| Development kits | Bare-metal applications | RTOS applications |
| --- | --- | --- |
| [SAM E70 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAME70-XULT) | 11 | 9 |
| [SAM V71 Xplained Ultra Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMV71-XULT) | 1 | 3 |
| [SAM D21 Xplained Pro Evaluation Kit](https://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=ATSAMD21-XPRO) | 3 | 0 |

### KNOWN ISSUES

The current known issues are as follows:

- The device does not run after programming the device through EDBG. The user needs to reset the device manually using the reset button on the Xplained boards to run the firmware.

- **Programming and debugging through EDBG is not supported.** The ICD4 needs to be used for programming and debugging.

- The ICD4 loads the reset line of the SAM V71 Xplained Ultra board. Do not press reset button on the Xplained Ultra board while ICD4 is connected to the board.

- Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the &quot;Configuring the Library&quot; section in the help documentation in the doc folder for this module instead.  Help is available in both CHM and PDF formats.

### DEVELOPMENT TOOLS

- [MPLAB X IDE v5.10](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB XC32 C/C++ Compiler v2.15](https://www.microchip.com/mplab/compilers)
- MPLAB X IDE plug-ins:
 - MPLAB Harmony Configurator (MHC) v3.1
