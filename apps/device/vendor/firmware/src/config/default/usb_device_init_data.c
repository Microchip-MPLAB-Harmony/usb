/*******************************************************************************
  System Initialization File

  File Name:
    usb_device_init_data.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
/**************************************************
 * USB Device Function Driver Init Data
 **************************************************/
	/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt0[] =
{
	0x06, 0x00, 0xFF,   // Usage Page = 0xFF00 (Vendor Defined Page 1)
    0x09, 0x01,             // Usage (Vendor Usage 1)
    0xA1, 0x01,             // Colsslection (Application)
    0x19, 0x01,             // Usage Minimum
    0x29, 0x40,             // Usage Maximum 	//64 input usages total (0x01 to 0x40)
    0x15, 0x01,             // Logical Minimum (data bytes in the report may have minimum value = 0x00)
    0x25, 0x40,      	    // Logical Maximum (data bytes in the report may have maximum value = 0x00FF = unsigned 255)
    0x75, 0x08,             // Report Size: 8-bit field size
    0x95, 0x40,             // Report Count: Make sixty-four 8-bit fields (the next time the parser hits an "Input", "Output", or "Feature" item)
    0x81, 0x00,             // Input (Data, Array, Abs): Instantiates input packet fields based on the above report size, count, logical min/max, and usage.
    0x19, 0x01,             // Usage Minimum
    0x29, 0x40,             // Usage Maximum 	//64 output usages total (0x01 to 0x40)
    0x91, 0x00,             // Output (Data, Array, Abs): Instantiates output packet fields.  Uses same report size and count as "Input" fields, since nothing new/different was specified to the parser since the "Input" item.
    0xC0
};
const USB_DEVICE_HID_INIT hidInit0 =
{
	 .hidReportDescriptorSize = sizeof(hid_rpt0),
	 .hidReportDescriptor = (void *)&hid_rpt0,
	 .queueSizeReportReceive = 1,
	 .queueSizeReportSend = 1
};



/**************************************************
 * USB Device Layer Function Driver Registration
 * Table
 **************************************************/
const USB_DEVICE_FUNCTION_REGISTRATION_TABLE funcRegistrationTable[1] =
{
    /* Function 1 */
    {
        .configurationValue = 1,                        /* Configuration value */
        .interfaceNumber = 0,                           /* First interfaceNumber of this function */
        .speed = USB_SPEED_HIGH,                        /* Function Speed */
        .numberOfInterfaces = 1,                        /* Number of interfaces */
        .funcDriverIndex = 0,                           /* Index of HID Function Driver */
        .driver = NULL,                                 /* No Function Driver data */
        .funcDriverInit = NULL                          /* No Function Driver Init data */
    },



};

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/
/*******************************************
 *  USB Device Descriptor
 *******************************************/
const USB_DEVICE_DESCRIPTOR deviceDescriptor =
{
    0x12,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,                              // DEVICE descriptor type
    0x0200,                                             // USB Spec Release Number in BCD format
	0x00,         // Class Code
    0x00,         // Subclass code
    0x00,         // Protocol code
    USB_DEVICE_EP0_BUFFER_SIZE,                         // Max packet size for EP0, see system_config.h
    0x04D8,                                             // Vendor ID
    0x0053,                                             // Product ID
    0x0100,                                             // Device release number in BCD format
    0x01,                                               // Manufacturer string index
    0x02,                                               // Product string index
    0x00,                                               // Device serial number string index
    0x01                                                // Number of possible configurations
};

/*******************************************
 *  USB Device Qualifier Descriptor for this
 *  demo.
 *******************************************/
const USB_DEVICE_QUALIFIER deviceQualifierDescriptor1 =
{
    0x0A,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE_QUALIFIER,                    // Device Qualifier Type
    0x0200,                                             // USB Specification Release number
    0x00,                                               // Class Code
    0x00,                                               // Subclass code
    0x00,                                               // Protocol code
    USB_DEVICE_EP0_BUFFER_SIZE,                         // Maximum packet size for endpoint 0
    0x01,                                               // Number of possible configurations
    0x00                                                // Reserved for future use.
};

/*******************************************
 *  USB High Speed Configuration Descriptor
 *******************************************/

const uint8_t highSpeedConfigurationDescriptor[]=
{
	/* Configuration Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    USB_DEVICE_16bitTo8bitArrange(32),                  //(32 Bytes)Size of the Config descriptor
    0x01,                                               // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,                                                 // Max power consumption (2X mA)

    /* Descriptor for Function 1 - Vendor     */

    /* Interface Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                           // INTERFACE descriptor type
    0x00,                                               // Interface Number
    0x00,                                               // Alternate Setting Number
    0x02,                                               // Number of endpoints in this interface
    0xFF,                                               // Class code
    0xFF,                                               // Subclass code
    0xFF,                                               // No Protocol
    0x00,                                               // Interface string index

    /* Endpoint (OUT) Descriptor */

    0x07,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,                            // Endpoint Descriptor
    0x01 | USB_EP_DIRECTION_OUT,                        // EndpointAddress ( EP1 OUT )
    USB_TRANSFER_TYPE_BULK,                             // Attributes
    0x00, 0x02,                                         // Max packet size of this EP
    0x01,                                               // Interval

    /* Endpoint Descriptor */

    0x07,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,                            // Endpoint Descriptor
    0x02 | USB_EP_DIRECTION_IN,                         // EndpointAddress ( EP2 OUT )
    USB_TRANSFER_TYPE_BULK,                             // Attributes
    0x00, 0x02,                                         // size
    0x01,                                               // Interval


};

/*******************************************
 * Array of High speed config descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE highSpeedConfigDescSet[1] =
{
    highSpeedConfigurationDescriptor
};


/*******************************************
 *  USB Full Speed Configuration Descriptor
 *******************************************/
const uint8_t fullSpeedConfigurationDescriptor[]=
{
    /* Configuration Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    USB_DEVICE_16bitTo8bitArrange(32),                  //(32 Bytes)Size of the Config descriptor
    0x01,                                               // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,                                                 // Max power consumption (2X mA)

    /* Descriptor for Function 1 - Vendor     */

	/* Interface Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                           // Descriptor Type is Interface descriptor
    0,                                                  // Interface Number
    0x00,                                               // Alternate Setting Number
    0x02,                                               // Number of endpoints in this interface
    0xFF,                                               // Class code
    0xFF ,                                              // Subclass code
    0xFF,                                               // No Protocol
    0x00,                                               // Interface string index

    /* Endpoint (OUT) Descriptor */

    0x07,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,                            // Endpoint Descriptor
    0x01 | USB_EP_DIRECTION_OUT,                        // EndpointAddress ( EP1 OUT )
    USB_TRANSFER_TYPE_BULK,                             // Attributes
    0x40,0x00,                                          // Max packet size of this EP
    0x01,                                               // Interval

    /* Endpoint (IN) Descriptor */

    0x07,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,                            // Endpoint Descriptor
    0x02 | USB_EP_DIRECTION_IN,                         // EndpointAddress ( EP2 IN )
    USB_TRANSFER_TYPE_BULK,                             // Attributes
    0x40,0x00,                                          // Max packet size of this EP
    0x01,                                               // Interval



};

/*******************************************
 * Array of Full speed config descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE fullSpeedConfigDescSet[1] =
{
    fullSpeedConfigurationDescriptor
};


/**************************************
 *  String descriptors.
 *************************************/

 /*******************************************
 *  Language code string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;
        uint8_t bDscType;
        uint16_t string[1];
    }
    sd000 =
    {
        sizeof(sd000),          // Size of this descriptor in bytes
        USB_DESCRIPTOR_STRING,  // STRING descriptor type
        {0x0409}                // Language ID
    };
/*******************************************
 *  Manufacturer string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[25];    // String
    }
    sd001 =
    {
        sizeof(sd001),
        USB_DESCRIPTOR_STRING,
        {'M','i','c','r','o','c','h','i','p',' ','T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'}

    };

/*******************************************
 *  Product string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[25];    // String
    }
    sd002 =
    {
        sizeof(sd002),
        USB_DESCRIPTOR_STRING,
        {'S','i','m','p','l','e',' ','W','i','n','U','S','B',' ','D','e','v','i','c','e',' ','D','e','m','o'}
    };

/***************************************
 * Array of string descriptors
 ***************************************/
USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[3]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002
};


/*******************************************
 * USB Device Layer Master Descriptor Table
 *******************************************/
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &deviceDescriptor,          /* Full speed descriptor */
    1,                          /* Total number of full speed configurations available */
    fullSpeedConfigDescSet,     /* Pointer to array of full speed configurations descriptors*/
    &deviceDescriptor,          /* High speed device descriptor*/
    1,                          /* Total number of high speed configurations available */
    highSpeedConfigDescSet,     /* Pointer to array of high speed configurations descriptors. */
    3,                          // Total number of string descriptors available.
    stringDescriptors,          // Pointer to array of string descriptors.
    &deviceQualifierDescriptor1,// Pointer to full speed dev qualifier.
    &deviceQualifierDescriptor1 // Pointer to high speed dev qualifier.
};


/****************************************************
 * USB Device Layer Initialization Data
 ****************************************************/
const USB_DEVICE_INIT usbDevInitData =
{
    /* Number of function drivers registered to this instance of the
       USB device layer */
    .registeredFuncCount = 1,

    /* Function driver table registered to this instance of the USB device layer*/
    .registeredFunctions = (USB_DEVICE_FUNCTION_REGISTRATION_TABLE*)funcRegistrationTable,

    /* Pointer to USB Descriptor structure */
    .usbMasterDescriptor = (USB_DEVICE_MASTER_DESCRIPTOR*)&usbMasterDescriptor,

    /* USB Device Speed */
    .deviceSpeed =  USB_SPEED_HIGH,

	/* Index of the USB Driver to be used by this Device Layer Instance */
    .driverIndex = DRV_USBHSV1_INDEX_0,

    /* Pointer to the USB Driver Functions. */
    .usbDriverInterface = DRV_USBHSV1_DEVICE_INTERFACE,

    /* Specify queue size for vendor endpoint read */
    .queueSizeEndpointRead = 1,

    /* Specify queue size for vendor endpoint write */
    .queueSizeEndpointWrite= 1,
};
// </editor-fold>
