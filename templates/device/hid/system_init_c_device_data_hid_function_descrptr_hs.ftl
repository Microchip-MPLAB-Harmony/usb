<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_device_data_hid_function_descrptr_hs.ftl

  Summary:
    USB Device Freemarker Template File

  Description:
    This file contains configurations necessary to run the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
*******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
-->

	/* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // Descriptor Type is Interface descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER},                                  // Interface Number
    0x00,                                  // Alternate Setting Number
    0x02,                                  // Number of endpoints in this interface
    USB_HID_CLASS_CODE,                 // Class code
    USB_HID_SUBCLASS_CODE_NO_SUBCLASS , // Subclass code
    USB_HID_PROTOCOL_CODE_NONE,         // No Protocol
    0x00,                                  // Interface string index

    /* HID Class-Specific Descriptor */

    0x09,                           // Size of this descriptor in bytes
    USB_HID_DESCRIPTOR_TYPES_HID,   // HID descriptor type
    0x11,0x01,                      // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    1,                              // Number of class descriptors
    USB_HID_DESCRIPTOR_TYPES_REPORT,// Report descriptor type
    USB_DEVICE_16bitTo8bitArrange(sizeof(hid_rpt${CONFIG_USB_DEVICE_FUNCTION_INDEX})),   // Size of the report descriptor

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_INT_IN_ENDPOINT_NUMBER} | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP${CONFIG_USB_DEVICE_FUNCTION_INT_IN_ENDPOINT_NUMBER} IN )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // Size
    0x01,                           // Interval

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_INT_OUT_ENDPOINT_NUMBER} | USB_EP_DIRECTION_OUT,   // EndpointAddress ( EP${CONFIG_USB_DEVICE_FUNCTION_INT_OUT_ENDPOINT_NUMBER} OUT )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // size
    0x01,                           // Interval
    
    

<#--
/*******************************************************************************
 End of File
*/
-->