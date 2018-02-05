/*******************************************************************************
  USB Device Layer Compile Time Options

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_config_template.h

  Summary:
    USB device configuration template header file.

  Description:
    This file contains USB device layer compile time options (macros) that are
    to be configured by the user. This file is a template file and must be used
    as an example only. This file must not be directly included in the project.
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

#ifndef _USB_DEVICE_CONFIG_TEMPLATE_H_
#define _USB_DEVICE_CONFIG_TEMPLATE_H_

#error (" This is a template file and must not be included directly in the project" );

// *****************************************************************************
// *****************************************************************************
// Section: USB Device configuration Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Number of Device Layer Instances.
 
  Summary:
    Number of Device Layer instances to provisioned in the application.

  Description:
    This configuration macro defines the number of Device Layer instances in the
    application. In cases of microcontrollers that feature multiple USB
    peripherals, this allows the application to run multiple instances of the
    Device Layer. As an example, for a microcontroller containing two USB
    peripherals, setting USB_DEVICE_INSTANCES_NUMBER to 2 will cause 2 instances
    of the Device Layer to execute.
    
  Remarks:
    Setting this value to more than the number of USB peripheral will result if
    unused RAM consumption.
*/

#define USB_DEVICE_INSTANCES_NUMBER 1

// *****************************************************************************
/* Endpoint 0 Buffer Size
 
  Summary:
    Buffer Size in Bytes for Endpoint 0. 

  Description:
    This number defines the size (in bytes) of Endpoint 0. For High Speed USB Devices, 
    this number should be 64. For Full Speed USB Devices, this number can be 8, 16, 32 
    or 64 bytes. This number will be applicable to all USB Device Stack instances.
    
  Remarks:
    None.
*/

#define USB_DEVICE_EP0_BUFFER_SIZE 

// *****************************************************************************
/* USB Device Layer SOF Event Enable
 
  Summary:
    Enables the Device Layer SOF event.

  Description:
    Specifying this configuration macro will cause the USB Device Layer to
    generate the USB_DEVICE_EVENT_SOF event. On Full Speed USB Devices, this
    event will be generated every 1 millisecond. On High Speed USB devices, this
    event will be generated every 125 microsecond.

  Remarks:
    None.
*/

#define USB_DEVICE_SOF_EVENT_ENABLE

// *****************************************************************************
/* USB Device Layer Set Descriptor Event Enable
 
  Summary:
    Enables the Device Layer Set Descriptor Event.

  Description:
    Specifying this configuration macro will cause the USB Device Layer to
    generate the USB_DEVICE_EVENT_SET_DESCRIPTOR event when a Set Descriptor
    request is received. If this macro is not defined, the USB Device Layer will
    stall the Set Descriptor control transfer request.

  Remarks:
    None.
*/

#define USB_DEVICE_SET_DESCRIPTOR_EVENT_ENABLE

// *****************************************************************************
/* USB Device Layer Synch Frame Event Enable
 
  Summary:
    Enables the Device Layer Synch Frame Event.

  Description:
    Specifying this configuration macro will cause the USB Device Layer to
    generate the USB_DEVICE_EVENT_SYNCH_FRAME event when a Synch Frame control
    transfer request is received. If this macro is not defined, the USB Device
    Layer will stall the control transfer request associated with this event.

  Remarks:
    None.
*/

#define USB_DEVICE_SYNCH_FRAME_EVENT_ENABLE

// *****************************************************************************
/* USB Device Layer Combined Endpoint Queue Depth
 
  Summary:
    Specifies the combined endpoint queue depth in case of a vendor USB device
    implementation.

  Description:
    This configuration constant specifies the combined endpoint queue depth in a
    case where the endpoint read and endpoint write functions are used to
    implement a vendor USB device. This constant should be used in conjunction
    with the usb_device_endpoint_functions.c file.

    This macro defines the number of entries in all IN and OUT endpoint queues
    in all instances of the USB Device Layer. This value can be obtained by
    adding up the endpoint read and write queue sizes of each USB Device Layer
    instance . In a simple single instance USB Device Layer, that requires only
    one read and one write endpoint with one buffer each, the
    USB_DEVICE_ENDPOINT_QUEUE_DEPTH_COMBINED macro can be set to 2. Consider a
    case with one Device Layer instance using 2 IN and 2 OUT endpoints, each
    requiring 2 buffers, this macro should be set to 8 (2 + 2 + 2 + 2). 

  Remarks:
    This constant needs to be specified only if a Vendor USB Device is to be
    implemented and the usb_device_endpoint_functions.c file is included in the
    project. This constant does not have any effect on queues of other standard
    function drivers that are included in the USB device implementation.
*/

#define USB_DEVICE_ENDPOINT_QUEUE_DEPTH_COMBINED  2

// *****************************************************************************
/* USB Device Layer BOS Descriptor Support Enable 
 
  Summary: 
    Specifies if the Device Layer should process a Host request for a BOS
    descriptor.

  Description:
    Specifying this configuration macro will enable support for BOS request.
    When the request is received, the device layer will transfer the data
    pointed to by the bosDescriptor member of the USB_DEVICE_INIT data
    structure. If this configuration macro is not specified, request for a BOS
    descriptor is stalled.

  Remarks:
    The USB Host will request for a BOS descriptor when the bcdVersion field in
    the Device Descriptor is greater than 0x0200.

*/

#define USB_DEVICE_BOS_DESCRIPTOR_SUPPORT_ENABLE

// *****************************************************************************
/* USB Device Layer Advanced String Descriptor Table Entry Format Enable. 
 
  Summary: 
    Specifying this macro enables the Advanced String Descriptor Table Entry
    Format.

  Description:
    Specifyin this macro enables the Advanced String Descriptor Table Entry
    Format. The advanced format allows the application to specify the String
    Index and the language in the entry itself. In the basic format, this
    information is obtained by the virtue of the entry index of the String
    Descriptor in the String Descriptor Table. Using the Advanced format allows
    the application to specify strings with arbitrary strings indexes. In basic
    format, the string indexes are forced to be contiguous.

  Remarks:
    The basic string descriptor entry format is selected by default. The
    advanced format must be enabled explicitly by specifying this macro.
*/

#define USB_DEVICE_STRING_DESCRIPTOR_TABLE_ADVANCED_ENABLE

// *****************************************************************************
/* USB Device Layer USB Controller Driver Explicit Initialize 
 
  Summary: 
    Specifies if the USB Controller Driver must be initialized explicitly as
    opposed to being initialized by the Device Layer.

  Description:
    Specifying this macro indicates that the USB Controller Driver will be
    initialized explicitly in the SYS_Initialize() function. The Device Layer
    will not initialize the USB Controller Driver. All releases of the USB
    Device Layer starting from v1.04 of MPALB Harmony will specify this macro
    i.e the controller driver will be initialized explicitly.

    If this macro is not specified, then the USB Device Layer will initialize
    the controller driver and run its tasks routines.

  Remarks:
    None.
*/

#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

// *****************************************************************************
/* USB Device Layer Microsoft OS Descriptor Support Enable  
 
  Summary: 
    Specifies if the USB Device stack should support Microsoft OS Descriptor. 

  Description:
    This macro needs to be defined to enable Microsoft OS descriptor support. 
	If this macro is defined all Vendor Interface requests are forwarded to 
	client unconditionally and Device layer does not validate the recipient 
	interface field in a Control transfer. 
	
	Device and Class Control Requests are not affected by this configuration. 
	The Device Layer will validate the recipient interface in Device and Class 
	Control Requests, irrespective of this configuration constant, and will 
	stall these requests if the interface is provisioned in the Function Driver
	Registration Table.
	
	If this macro is not defined, then USB Device Layer will validate the 
	interface number in a Vendor Interface request and stall the request if 
	Interface number is not available in the Function registration table. 

  Remarks:
    None.
*/

#define USB_DEVICE_MICROSOFT_OS_DESCRIPTOR_SUPPORT_ENABLE
#endif



