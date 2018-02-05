/*******************************************************************************
  USB Host Layer Configuration constants

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_config_template.h

  Summary:
    USB host configuration template header file.

  Description:
    This file contains USB host layer compile time options (macros) that are to
    be configured by the user. This file is a template file and must be used as
    an example only. This file must not be directly included in the project.

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

#ifndef _USB_HOST_CONFIG_TEMPLATE_H_
#define _USB_HOST_CONFIG_TEMPLATE_H_

#error "This is a configuration template file.  Do not include it directly."

// *****************************************************************************
// *****************************************************************************
// Section: USB Host configuration Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Host Layer Pipes Number 
  
  Summary:
    Defines the maximum number of pipes that the application will need.

  Description:
    This configuration constant defines the maximum number of device
    communication pipes that Host Layer would need in the application. Every
    attached device requires atleast one pipe. This pipe is the control transfer
    pipe. Additional pipes are needed based on the type of device. For example,
    a standard Mass Storage Class device will need 2 pipes, a Communication
    Class Device will need 3 pipes, a HID device will need atleast 1 pipe.
    Vendor device will need communication pipe based on the device
    implementation. The number of pipes must also take into account the number
    of devices to support.

    Example - If a USB Host application must support 2 devices, either 2 USB pen
    driver or 2 CDC devices or a mix of both, then the pipes number should be set to
    8. This is because the 2 CDC devices connected to the host will pose larger pipe
    requirement. Two such devices will requires 2 control pipes, 2 interrupt pipes,
    2 Bulk IN and 2 Bulk OUT pipes.

  Remarks:
    This number should match the number of pipes configured in the HCD that this
    application will use, that is DRV_USBHS_HOST_PIPES_NUMBER or
    DRV_USBFS_HOST_PIPES_NUMBER.
*/


#define USB_HOST_PIPES_NUMBER /*DOM-IGNORE-BEGIN*/10/*DOM-IGNORE-END*/    

// *****************************************************************************
/* USB Host Layer Devices Number 
 
  Summary: 
    Defines the maximum number of devices to support.

  Description:
    This configuration constant defines the maximum number of devices that this
    USB Host application must support. The value of this constant should be
    atleast 1. Multiple devices can be supported if Hub support is enabled. See
    USB_HOST_HUB_SUPPORT_ENABLE. The Hub itself will be treated as a device.  

    Example - If the USB Host application must support one USB Pen Drive and one USB
    Serial COM port (CDC Device), then this constant should be set to 3 (one
    additional device will be the Hub).

  Remarks:
    Supporting multiple devices requires more data memory and processing time. 
*/

#define USB_HOST_DEVICES_NUMBER /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Layer Controller Numbers
 
  Summary: 
    Defines the number of USB Host Controllers that this Host Layer must manage.

  Description:
    This constant defines the number of USB Host Controllers that this Host
    Layer must manage. The value of this constant should be atleast 1. Typical
    embedded applicatons contains only 1 USB host controller and hence only 1
    USB. A microcontroller that features multiple USB modules can support
    multiple USB Host controllers and multipel USBs. USB contollers can also be
    interfaced to the microcontroller through common communication peripherals
    such as SPI.

    This constant also defines the number of entries in the Host Controller
    Driver interface table, a pointer to which is passed in the
    hostContollerDrivers member of the USB_HOST_INIT data structure.

  Remarks:
    None.
*/

#define USB_HOST_CONTROLLERS_NUMBER  /*DOM-IGNORE-BEGIN*/1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Layer Hub Support
 
  Summary: 
    Defines if this USB Host application must support a Hub.

  Description:
    Specifying this macro will enable Hub Support. The HUB tier level to be
    supported is then specified by USB_HOST_HUB_TIER_LEVEL constant. If this
    macro is specified, the file usb_host_hub.c must be included in the
    application.

  Remarks:
    Not specifying this macro will disable Hub support.
*/

#define USB_HOST_HUB_SUPPORT_ENABLE

// *****************************************************************************
/* USB Host Layer Transfers Number 
 
  Summary: 
    Defines the maximum number of transfers that host layer should handle.

  Description:
    This constant defines the maximum number of transfers that the host layer
    should handle. The choice of this constant depends on the nature of devices
    that the USB Host application must support. Atleast two transfers are needed
    per pipe in the system. If the number of transfers provisioned in the system
    are insufficient, the USB Host will decline transfer request citing a busy
    status. This will affect the speed performance of the system. 

    Example - A USB Host application will support an MSD device and a CDC
    Device. The total number of pipes needed in the system are 7. The
    USB_HOST_TRANSFERS_NUMBER constant should be atleast 14 (2 per pipe).
    Specifying a larger number will enable more transfers to be queued but will
    also require more data memory.

  Remarks:
    None.

*/

#define USB_HOST_TRANSFERS_NUMBER  /*DOM-IGNORE-BEGIN*/10 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Device Interface Numbers 
 
  Summary: 
    Defines the maximum number of interface that the attached device can contain
    in order for the USB Host Layer to process the device.

  Description:
    This constant defines the maximum number of interface that the attached device can contain
    in order for the USB Host Layer to process the device. The device will be
    processed if it only contains less interfaces than the value of this
    constant.

    Example - An attached device contains a configuration that contains 10
    interfaces, but the USB_HOST_DEVICE_INTERFACES_NUMBER is set to 5. The
    device will not be processed by the Host Layer. A dual CDC device needs to
    be supported. This device will have 4 interfaces. The
    USB_HOST_DEVICE_INTERFACES_NUMBER constant should be atleast 4.

  Remarks:
    Supporting more interface per device required more processing time and data
    memory.
*/

#define USB_HOST_DEVICE_INTERFACES_NUMBER  /*DOM-IGNORE-BEGIN*/5 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Hub Tier Level 
 
  Summary: 
    Defines the maximum tier of connected hubs to be supported.

  Description:
    This constant defines the maximum hub tiers to be supported by the USB Host
    application. This constant is considered only if the
    USB_HOST_HUB_SUPPORT_ENABLE option in specified. If specified, the value
    should be atleast 1. This means that one hub should be supported. In a case
    where another hub will be be connected to the hub which is connected to the
    USB Host, the value should be 2. As per the USB specification the maximum
    number of non-root hub tiers can be 5. Hence the value of this configuration
    constant should not exceed 5.

  Remarks:
    None.

*/

#define USB_HOST_HUB_TIER_LEVEL  /*DOM-IGNORE-BEGIN*/1 /*DOM-IGNORE-END*/

#endif // #ifndef __USB_HOST_CONFIG_TEMPLATE_H_

/*******************************************************************************
 End of File
*/
