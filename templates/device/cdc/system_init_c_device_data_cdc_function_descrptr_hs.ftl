<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_device_data_cdc_function_descrptr_hs.ftl

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
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
 *******************************************************************************/
-->
<#if CONFIG_USB_DEVICE_FUNCTION_USE_IAD == true>
    /* Descriptor for Function - CDC     */
    /* Interface Association Descriptor: CDC Function*/
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

<#--
/*******************************************************************************
 End of File
*/
-->