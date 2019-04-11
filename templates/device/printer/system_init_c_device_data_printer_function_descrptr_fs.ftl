<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_device_data_printer_function_descrptr_fs.ftl

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
    /* Interface Descriptor */

    0x09,                                                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                               // Descriptor Type is Interface descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER},         // Interface Number
    0x00,                                                   // Alternate Setting Number
    0x01,                                                   // Number of endpoints in this interface
    USB_PRINTER_INTERFACE_CLASS_CODE,                       // Class code
    USB_PRINTER_INTERFACE_SUBCLASS_CODE,                    // Subclass code
    USB_PRINTER_INTERFACE_PROTOCOL,                         // Protocol code
    0x00,                                                   // Interface string index

    /* Bulk Endpoint (OUT) Descriptor */

    0x07,                                                   // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,                                // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_BULK_OUT_ENDPOINT_NUMBER} | USB_EP_DIRECTION_OUT,                               // EndpointAddress ( EP${CONFIG_USB_DEVICE_FUNCTION_BULK_OUT_ENDPOINT_NUMBER} OUT )
    USB_TRANSFER_TYPE_BULK,                                 // Attributes type of EP (BULK)
    0x40, 0x00,                                             // Max packet size of this EP
    0x00,                                                   // Interval (in ms)
<#--
/*******************************************************************************
 End of File
*/
-->