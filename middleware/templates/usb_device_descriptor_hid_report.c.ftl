<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX0 == true >

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt${usbDeviceHIDIndex}[] =
{
	    <#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_mouse_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_keyboard_demo">
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
    <#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "hid_joystick_demo">
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
    <#else>
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
    0xC0                    // End Collection
    </#if>
};
<#assign usbDeviceHIDIndex = usbDeviceHIDIndex + 1>
</#if>