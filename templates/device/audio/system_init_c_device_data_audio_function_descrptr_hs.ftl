<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_device_data_audio_function_descrptr_hs.ftl

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
<#if CONFIG_USB_DEVICE_FUNCTION_AUDIO_VERSION == "Audio v2">
<#if CONFIG_USB_DEVICE_FUNCTION_USE_IAD == true>
	/* Descriptor for Function - Audio     */ 
    /* Interface Association Descriptor: Audio Function*/
    0x08,   // Size of this descriptor in bytes
    0x0B,   // Interface association descriptor type
    ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER},   // The first associated interface
    0x02,   // Number of contiguous associated interface
    0x02,   // bInterfaceClass of the first interface
    0x02,   // bInterfaceSubclass of the first interface
    0x01,   // bInterfaceProtocol of the first interface
    0x00,   // Interface string index
</#if>
	/* Interface Descriptor */

    0x09,                                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                       // Descriptor Type is Interface descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER},                                  // Interface Number
    0x00,                                           // Alternate Setting Number
    0x01,                                           // Number of endpoints in this interface
    USB_CDC_COMMUNICATIONS_INTERFACE_CLASS_CODE,    // Class code
    USB_CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL,        // Subclass code
    USB_CDC_PROTOCOL_AT_V250,                       // Protocol code
    0x00,                                           // Interface string index

    /* CDC Class-Specific Descriptors */

    sizeof(USB_CDC_HEADER_FUNCTIONAL_DESCRIPTOR),               // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_HEADER,                                  // Type of functional descriptor
    0x20,0x01,                                                  // CDC spec version

    sizeof(USB_CDC_ACM_FUNCTIONAL_DESCRIPTOR),                  // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_ABSTRACT_CONTROL_MANAGEMENT,             // Type of functional descriptor
    USB_CDC_ACM_SUPPORT_LINE_CODING_LINE_STATE_AND_NOTIFICATION,// bmCapabilities of ACM

    sizeof(USB_CDC_UNION_FUNCTIONAL_DESCRIPTOR_HEADER) + 1,     // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_UNION,                                   // Type of functional descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER},                                                       // com interface number
    ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER?number+1},

    sizeof(USB_CDC_CALL_MANAGEMENT_DESCRIPTOR),                 // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_CALL_MANAGEMENT,                         // Type of functional descriptor
    0x00,                                                       // bmCapabilities of CallManagement
    ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER?number+1},                                                       // Data interface number

    /* Interrupt Endpoint (IN) Descriptor */

    0x07,                           // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_INT_ENDPOINT_NUMBER} | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP${CONFIG_USB_DEVICE_FUNCTION_INT_ENDPOINT_NUMBER} IN INTERRUPT)
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes type of EP (INTERRUPT)
    0x10,0x00,                      // Max packet size of this EP
    0x02,                           // Interval (in ms)

    /* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // INTERFACE descriptor type
    ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER?number+1},      // Interface Number
    0x00,                               // Alternate Setting Number
    0x02,                               // Number of endpoints in this interface
    USB_CDC_DATA_INTERFACE_CLASS_CODE,  // Class code
    0x00,                               // Subclass code
    USB_CDC_PROTOCOL_NO_CLASS_SPECIFIC, // Protocol code
    0x00,                               // Interface string index

    /* Bulk Endpoint (OUT) Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_BULK_OUT_ENDPOINT_NUMBER} | USB_EP_DIRECTION_OUT,   // EndpointAddress ( EP${CONFIG_USB_DEVICE_FUNCTION_BULK_OUT_ENDPOINT_NUMBER} OUT )
    USB_TRANSFER_TYPE_BULK,     // Attributes type of EP (BULK)
    0x00, 0x02,                 // Max packet size of this EP
    0x00,                       // Interval (in ms)

     /* Bulk Endpoint (IN)Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_BULK_IN_ENDPOINT_NUMBER} | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP${CONFIG_USB_DEVICE_FUNCTION_BULK_IN_ENDPOINT_NUMBER} IN )
    0x02,                       // Attributes type of EP (BULK)
    0x00, 0x02,                 // Max packet size of this EP
    0x00,                       // Interval (in ms)
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->