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
	0x05, 0x01, /* Usage Page (Generic Desktop)        */
	0x09, 0x02, /* Usage (Mouse)                       */
	0xA1, 0x01, /* Collection (Application)            */
	0x09, 0x01, /* Usage (Pointer)                     */
	0xA1, 0x00, /* Collection (Physical)               */
	0x05, 0x09, /* Usage Page (Buttons)                */
	0x19, 0x01, /* Usage Minimum (01)                  */
	0x29, 0x03, /* Usage Maximum (03)                  */
	0x15, 0x00, /* Logical Minimum (0)                 */
	0x25, 0x01, /* Logical Maximum (1)                 */
	0x95, 0x03, /* Report Count (3)                    */
	0x75, 0x01, /* Report Size (1)                     */
	0x81, 0x02, /* Input (Data, Variable, Absolute)    */
	0x95, 0x01, /* Report Count (1)                    */
	0x75, 0x05, /* Report Size (5)                     */
	0x81, 0x01, /* Input (Constant)    ;5 bit padding  */
	0x05, 0x01, /* Usage Page (Generic Desktop)        */
	0x09, 0x30, /* Usage (X)                           */
	0x09, 0x31, /* Usage (Y)                           */
	0x15, 0x81, /* Logical Minimum (-127)              */
	0x25, 0x7F, /* Logical Maximum (127)               */
	0x75, 0x08, /* Report Size (8)                     */
	0x95, 0x02, /* Report Count (2)                    */
	0x81, 0x06, /* Input (Data, Variable, Relative)    */
	0xC0, 0xC0
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