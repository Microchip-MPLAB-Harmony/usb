<#--
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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
 -->
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
	${LIST_USB_DEVICE_FUNCTION_INIT_ENTRY}
/**************************************************
 * USB Device Layer Function Driver Registration 
 * Table
 **************************************************/
const USB_DEVICE_FUNCTION_REGISTRATION_TABLE funcRegistrationTable[${CONFIG_USB_DEVICE_FUNCTIONS_NUMBER}] =
{
    ${LIST_USB_DEVICE_FUNCTION_ENTRY}
};

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/
/*******************************************
 *  USB Device Descriptor 
 *******************************************/
const USB_DEVICE_DESCRIPTOR deviceDescriptor =
{
    0x12,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,          // DEVICE descriptor type
    0x0200,                         // USB Spec Release Number in BCD format
<#if CONFIG_USB_DEVICE_DESCRIPTOR_IAD_ENABLE == true>
    0xEF,                           // Class Code
    0x02,                           // Subclass code
    0x01,                           // Protocol code
<#elseif CONFIG_USB_DEVICE_FUNCTIONS_NUMBER == 1>
    ${LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY}
<#else>
    0x00,                           // Class Code
    0x00,                           // Subclass code
    0x00,                           // Protocol code
</#if>
    USB_DEVICE_EP0_BUFFER_SIZE,     // Max packet size for EP0, see configuration.h
    ${CONFIG_USB_DEVICE_VENDOR_ID_IDX0},                         // Vendor ID
    ${CONFIG_USB_DEVICE_PRODUCT_ID_IDX0},                         // Product ID				
    0x0100,                         // Device release number in BCD format
    0x01,                           // Manufacturer string index
    0x02,                           // Product string index
<#if CONFIG_USB_DEVICE_USE_MSD == true>
    0x03,                           // Device serial number string index
<#else>
	0x00,
</#if>
    0x01                            // Number of possible configurations
};

<#if CONFIG_USB_DEVICE_SPEED == "High Speed">
/*******************************************
 *  USB Device Qualifier Descriptor for this
 *  demo.
 *******************************************/
const USB_DEVICE_QUALIFIER deviceQualifierDescriptor1 =
{
    0x0A,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE_QUALIFIER,    // Device Qualifier Type
    0x0200,                             // USB Specification Release number
<#if CONFIG_USB_DEVICE_DESCRIPTOR_IAD_ENABLE == true>
	0xEF,                           // Class Code
    0x02,                           // Subclass code
    0x01,                           // Protocol code
<#elseif CONFIG_USB_DEVICE_FUNCTIONS_NUMBER == 1>
	${LIST_USB_DEVICE_DESCRIPTOR_CLASS_CODE_ENTRY}
<#else>
    0x00,                           // Class Code
    0x00,                           // Subclass code
    0x00,                           // Protocol code
</#if>
    USB_DEVICE_EP0_BUFFER_SIZE,         // Maximum packet size for endpoint 0
    0x01,                               // Number of possible configurations
    0x00                                // Reserved for future use.
};


/*******************************************
 *  USB High Speed Configuration Descriptor
 *******************************************/
 
const uint8_t highSpeedConfigurationDescriptor[]=
{
	/* Configuration Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    USB_DEVICE_16bitTo8bitArrange(${CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE}),              //(${CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE} Bytes)Size of the Config descriptor
    ${CONFIG_USB_DEVICE_INTERFACES_NUMBER},                                        // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,
	
${LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_HS_ENTRY}
};

/*******************************************
 * Array of High speed config descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE highSpeedConfigDescSet[1] =
{
    highSpeedConfigurationDescriptor
};
</#if>

/*******************************************
 *  USB Full Speed Configuration Descriptor
 *******************************************/
const uint8_t fullSpeedConfigurationDescriptor[]=
{

	/* Configuration Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    USB_DEVICE_16bitTo8bitArrange(${CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE}),                  //(${CONFIG_USB_DEVICE_CONFIG_DESCRPTR_SIZE} Bytes)Size of the Config descriptor
    ${CONFIG_USB_DEVICE_INTERFACES_NUMBER},                                                  // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,
	
${LIST_USB_DEVICE_FUNCTION_DESCRIPTOR_FS_ENTRY}
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
 <#if CONFIG_USB_DEVICE_FEATURE_ENABLE_ADVANCED_STRING_DESCRIPTOR_TABLE == true >
    const struct __attribute__ ((packed))
    {
        uint8_t stringIndex;    //Index of the string descriptor
        uint16_t languageID ;   // Language ID of this string.
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type 
        uint16_t string[1];     // String
    }
    sd000 =
    {
        0, // Index of this string is 0
        0, // This field is always blank for String Index 0
        sizeof(sd000)-sizeof(sd000.stringIndex)-sizeof(sd000.languageID),
        USB_DESCRIPTOR_STRING,
        {0x0409}                // Language ID
    };  
<#else>
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
</#if>
/*******************************************
 *  Manufacturer string descriptor
 *******************************************/
 <#if CONFIG_USB_DEVICE_FEATURE_ENABLE_ADVANCED_STRING_DESCRIPTOR_TABLE == true >
    const struct __attribute__ ((packed))
    {
        uint8_t stringIndex;    //Index of the string descriptor
        uint16_t languageID ;    // Language ID of this string.
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[${CONFIG_USB_DEVICE_MANUFACTURER_STRING?length}];    // String
    }
    sd001 =
    {
        1,      // Index of this string descriptor is 1. 
        0x0409, // Language ID of this string descriptor is 0x0409 (English)
        sizeof(sd001)-sizeof(sd001.stringIndex)-sizeof(sd001.languageID),
        USB_DESCRIPTOR_STRING,
		<#if CONFIG_USB_DEVICE_MANUFACTURER_STRING?length gt 0 >
        {<#list 1..CONFIG_USB_DEVICE_MANUFACTURER_STRING?length as index>'${CONFIG_USB_DEVICE_MANUFACTURER_STRING?substring(index-1,index)}'<#if index_has_next>,</#if></#list>}
		</#if>
    };
<#else>
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[${CONFIG_USB_DEVICE_MANUFACTURER_STRING?length}];    // String
    }
    sd001 =
    {
        sizeof(sd001),
        USB_DESCRIPTOR_STRING,
		<#if CONFIG_USB_DEVICE_MANUFACTURER_STRING?length gt 0 >
        {<#list 1..CONFIG_USB_DEVICE_MANUFACTURER_STRING?length as index>'${CONFIG_USB_DEVICE_MANUFACTURER_STRING?substring(index-1,index)}'<#if index_has_next>,</#if></#list>}
		</#if>
		
    };
</#if>

/*******************************************
 *  Product string descriptor
 *******************************************/
<#if CONFIG_USB_DEVICE_FEATURE_ENABLE_ADVANCED_STRING_DESCRIPTOR_TABLE == true >
    const struct __attribute__ ((packed))
    {
        uint8_t stringIndex;    //Index of the string descriptor
        uint16_t languageID ;   // Language ID of this string.
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type 
        uint16_t string[${CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR?length}];    // String
    }
    sd002 =
    {
        2,       // Index of this string descriptor is 2. 
        0x0409,  // Language ID of this string descriptor is 0x0409 (English)
        sizeof(sd002)-sizeof(sd002.stringIndex)-sizeof(sd002.languageID),
        USB_DESCRIPTOR_STRING,
		<#if CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR?length gt 0 >
        {<#list 1..CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR?length as index>'${CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR?substring(index-1,index)}'<#if index_has_next>,</#if></#list>} 
		</#if>
    }; 
<#else>
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[${CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR?length}];    // String
    }
    sd002 =
    {
        sizeof(sd002),
        USB_DESCRIPTOR_STRING,
		<#if CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR?length gt 0 >
		{<#list 1..CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR?length as index>'${CONFIG_USB_DEVICE_PRODUCT_STRING_DESCRIPTOR?substring(index-1,index)}'<#if index_has_next>,</#if></#list>}
		</#if>
    }; 
</#if>
<#if CONFIG_USB_DEVICE_USE_MSD == true>
/******************************************************************************
 * Serial number string descriptor.  Note: This should be unique for each unit
 * built on the assembly line.  Plugging in two units simultaneously with the
 * same serial number into a single machine can cause problems.  Additionally,
 * not all hosts support all character values in the serial number string.  The
 * MSD Bulk Only Transport (BOT) specs v1.0 restrict the serial number to
 * consist only of ASCII characters "0" through "9" and capital letters "A"
 * through "F".
 ******************************************************************************/
 <#if CONFIG_USB_DEVICE_FEATURE_ENABLE_ADVANCED_STRING_DESCRIPTOR_TABLE == true >
const struct __attribute__ ((packed))
    {
        uint8_t stringIndex;    //Index of the string descriptor
        uint16_t languageID ;   // Language ID of this string.
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type 
        uint16_t string[12];    // String
    }
    sd003 =
    {
        3,       // Index of this string descriptor is 3. 
        0x0409,  // Language ID of this string descriptor is 0x0409 (English)
        sizeof(sd003)-sizeof(sd003.stringIndex)-sizeof(sd003.languageID),
        USB_DESCRIPTOR_STRING,
		{'1','2','3','4','5','6','7','8','9','9','9','9'}
    };
<#else>
const struct
{
    uint8_t bLength;
    uint8_t bDscType;
    uint16_t string[12];
}
sd003 =
{
    sizeof(sd003),
    USB_DESCRIPTOR_STRING,
    {'1','2','3','4','5','6','7','8','9','9','9','9'}
};
</#if>
</#if>
/***************************************
 * Array of string descriptors
 ***************************************/
 <#if CONFIG_USB_DEVICE_USE_MSD == true>
 USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[4]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002,
	(const uint8_t *const)&sd003
};
 <#else>
USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[3]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002
};
</#if>

/*******************************************
 * USB Device Layer Master Descriptor Table 
 *******************************************/
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &deviceDescriptor,          /* Full speed descriptor */
    1,                          /* Total number of full speed configurations available */
    fullSpeedConfigDescSet,     /* Pointer to array of full speed configurations descriptors*/
<#if CONFIG_USB_DEVICE_SPEED == "High Speed">
    &deviceDescriptor,          /* High speed device descriptor*/
    1,                          /* Total number of high speed configurations available */
    highSpeedConfigDescSet,     /* Pointer to array of high speed configurations descriptors. */
<#else>
	NULL, 
	0,
	NULL,
</#if>
    3,                          // Total number of string descriptors available.
    stringDescriptors,          // Pointer to array of string descriptors.
<#if CONFIG_USB_DEVICE_SPEED == "High Speed">
    &deviceQualifierDescriptor1,// Pointer to full speed dev qualifier.
    &deviceQualifierDescriptor1 // Pointer to high speed dev qualifier.
<#else>
	NULL, 
	NULL
</#if>
};


/****************************************************
 * USB Device Layer Initialization Data
 ****************************************************/
<#-- Instance 0 -->
const USB_DEVICE_INIT usbDevInitData =
{
    /* Number of function drivers registered to this instance of the
       USB device layer */
    .registeredFuncCount = ${CONFIG_USB_DEVICE_FUNCTIONS_NUMBER},
	
    /* Function driver table registered to this instance of the USB device layer*/
    .registeredFunctions = (USB_DEVICE_FUNCTION_REGISTRATION_TABLE*)funcRegistrationTable,

    /* Pointer to USB Descriptor structure */
    .usbMasterDescriptor = (USB_DEVICE_MASTER_DESCRIPTOR*)&usbMasterDescriptor,

    /* USB Device Speed */
	<#if CONFIG_USB_DEVICE_SPEED == "High Speed">
    .deviceSpeed =  USB_SPEED_HIGH,   
	<#else>
	.deviceSpeed =  USB_SPEED_FULL,
    </#if>
	
	/* Index of the USB Driver to be used by this Device Layer Instance */
    .driverIndex = ${CONFIG_USB_DRIVER_INDEX},

    /* Pointer to the USB Driver Functions. */
    .usbDriverInterface = ${CONFIG_USB_DRIVER_INTERFACE},
};
// </editor-fold>
<#--
/*******************************************************************************
 End of File
*/
-->
