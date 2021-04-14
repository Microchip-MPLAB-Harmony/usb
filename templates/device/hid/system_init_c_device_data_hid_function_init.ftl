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
/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
<#if __PROCESSOR?matches("PIC32MZ1025W.*") == true>
uint8_t USB_ALIGN hid_rpt${CONFIG_USB_DEVICE_FUNCTION_INDEX}[] =
<#else>
const uint8_t hid_rpt${CONFIG_USB_DEVICE_FUNCTION_INDEX}[] =
</#if>
{
<#if CONFIG_USB_DEVICE_HID_REPORT_DESCRIPTOR_TYPE == "Mouse">
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
<#elseif CONFIG_USB_DEVICE_HID_REPORT_DESCRIPTOR_TYPE == "Keyboard">
	0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x06, // USAGE (Keyboard)
    0xa1, 0x01, // COLLECTION (Application)
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x01, // LOGICAL_MAXIMUM (1)
    0x75, 0x01, // REPORT_SIZE (1)
    0x95, 0x08, // REPORT_COUNT (8)
    0x81, 0x02, // INPUT (Data,Var,Abs)
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x08, // REPORT_SIZE (8)
    0x81, 0x03, // INPUT (Cnst,Var,Abs)
    0x95, 0x05, // REPORT_COUNT (5)
    0x75, 0x01, // REPORT_SIZE (1)
    0x05, 0x08, // USAGE_PAGE (LEDs)
    0x19, 0x01, // USAGE_MINIMUM (Num Lock)
    0x29, 0x05, // USAGE_MAXIMUM (Kana)
    0x91, 0x02, // OUTPUT (Data,Var,Abs)
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x03, // REPORT_SIZE (3)
    0x91, 0x03, // OUTPUT (Cnst,Var,Abs)
    0x95, 0x06, // REPORT_COUNT (6)
    0x75, 0x08, // REPORT_SIZE (8)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x65, // LOGICAL_MAXIMUM (101)
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, // INPUT (Data,Ary,Abs)
    0xc0        // End Collection
<#elseif CONFIG_USB_DEVICE_HID_REPORT_DESCRIPTOR_TYPE == "Joystick">
    0x05,0x01,        // USAGE_PAGE (Generic Desktop)
    0x09,0x05,        // USAGE (Game Pad)
    0xA1,0x01,        // COLLECTION (Application)
    0x15,0x00,        // LOGICAL_MINIMUM(0)
    0x25,0x01,        // LOGICAL_MAXIMUM(1)
    0x35,0x00,        // PHYSICAL_MINIMUM(0)
    0x45,0x01,        // PHYSICAL_MAXIMUM(1)
    0x75,0x01,        // REPORT_SIZE(1)
    0x95,0x0D,        // REPORT_COUNT(13)
    0x05,0x09,        // USAGE_PAGE(Button)
    0x19,0x01,        // USAGE_MINIMUM(Button 1)
    0x29,0x0D,        // USAGE_MAXIMUM(Button 13)
    0x81,0x02,        // INPUT(Data,Var,Abs)
    0x95,0x03,        // REPORT_COUNT(3)
    0x81,0x01,        // INPUT(Cnst,Ary,Abs)
    0x05,0x01,        // USAGE_PAGE(Generic Desktop)
    0x25,0x07,        // LOGICAL_MAXIMUM(7)
    0x46,0x3B,0x01,   // PHYSICAL_MAXIMUM(315)
    0x75,0x04,        // REPORT_SIZE(4)
    0x95,0x01,        // REPORT_COUNT(1)
    0x65,0x14,        // UNIT(Eng Rot:Angular Pos)
    0x09,0x39,        // USAGE(Hat Switch)
    0x81,0x42,        // INPUT(Data,Var,Abs,Null)
    0x65,0x00,        // UNIT(None)
    0x95,0x01,        // REPORT_COUNT(1)
    0x81,0x01,        // INPUT(Cnst,Ary,Abs)
    0x26,0xFF,0x00,   // LOGICAL_MAXIMUM(255)
    0x46,0xFF,0x00,   // PHYSICAL_MAXIMUM(255)
    0x09,0x30,        // USAGE(X)
    0x09,0x31,        // USAGE(Y)
    0x09,0x32,        // USAGE(Z)
    0x09,0x35,        // USAGE(Rz)
    0x75,0x08,        // REPORT_SIZE(8)
    0x95,0x04,        // REPORT_COUNT(4)
    0x81,0x02,        // INPUT(Data,Var,Abs)
    0xC0
<#elseif CONFIG_USB_DEVICE_HID_REPORT_DESCRIPTOR_TYPE == "Custom">
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
</#if>
};

/**************************************************
 * USB Device HID Function Init Data
 **************************************************/
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