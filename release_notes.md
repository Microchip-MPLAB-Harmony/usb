# Microchip MPLAB Harmony 3 Release Notes
## USB Release v3.1.1
### NEW FEATURES
- Added USB Host demos for SAMD2x Devices. 
- Added MHC support for Generic USB Devices. 
- Minor documentation update. 

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
