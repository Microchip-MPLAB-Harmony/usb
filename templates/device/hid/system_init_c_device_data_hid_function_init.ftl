<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_device_data_hid_function_init.ftl

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
/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${CONFIG_USB_DEVICE_FUNCTION_INDEX}[] =
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
const USB_DEVICE_HID_INIT hidInit${CONFIG_USB_DEVICE_FUNCTION_INDEX} =
{
	 .hidReportDescriptorSize = sizeof(hid_rpt${CONFIG_USB_DEVICE_FUNCTION_INDEX}),
	 .hidReportDescriptor = (void *)&hid_rpt${CONFIG_USB_DEVICE_FUNCTION_INDEX},
	 .queueSizeReportReceive = ${CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE},
	 .queueSizeReportSend = ${CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE}
};

<#--
/*******************************************************************************
 End of File
*/
-->