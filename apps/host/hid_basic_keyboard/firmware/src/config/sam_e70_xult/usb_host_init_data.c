#include "configuration.h"
#include "definitions.h" 
#include "usb/usb_host_hid_keyboard.h"

USB_HOST_HID_USAGE_DRIVER_INTERFACE usageDriverInterface =
{
  .initialize = NULL,
  .deinitialize = NULL,
  .usageDriverEventHandler = _USB_HOST_HID_KEYBOARD_EventHandler,
  .usageDriverTask = _USB_HOST_HID_KEYBOARD_Task
};

USB_HOST_HID_USAGE_DRIVER_TABLE_ENTRY usageDriverTableEntry[1] =
{
    {
        .usage = (USB_HID_USAGE_PAGE_GENERIC_DESKTOP_CONTROLS << 16) | USB_HID_GENERIC_DESKTOP_KEYBOARD,
        .initializeData = NULL,
        .interface = &usageDriverInterface
    }
};


USB_HOST_HID_INIT hidInitData =
{
    .nUsageDriver = 1,
    .usageDriverTable = usageDriverTableEntry
};
const USB_HOST_TPL_ENTRY USBTPList[1] = 
{
	TPL_INTERFACE_CLASS_SUBCLASS_PROTOCOL(0x03, 0x01, 0x01, &hidInitData,  USB_HOST_HID_INTERFACE),


};

const USB_HOST_HCD hcdTable = 
{
	/* Index of the USB Driver used by the Host Layer */
    .drvIndex = DRV_USBHSV1_INDEX_0,

    /* Pointer to the USB Driver Functions. */
    .hcdInterface = DRV_USBHSV1_HOST_INTERFACE,
};

const USB_HOST_INIT usbHostInitData = 
{
    .nTPLEntries = 1 ,
    .tplList = (USB_HOST_TPL_ENTRY *)USBTPList,
    .hostControllerDrivers = (USB_HOST_HCD *)&hcdTable    
};
// </editor-fold>
