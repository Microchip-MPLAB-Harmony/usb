#include "configuration.h"
#include "definitions.h" 
 
const USB_HOST_TPL_ENTRY USBTPList[2] = 
{
	TPL_INTERFACE_CLASS_SUBCLASS_PROTOCOL(0x08, 0x06, 0x50, NULL,  USB_HOST_MSD_INTERFACE) ,

	TPL_INTERFACE_CLASS(0x02, NULL,  USB_HOST_CDC_INTERFACE),


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
    .nTPLEntries = 2 ,
    .tplList = (USB_HOST_TPL_ENTRY *)USBTPList,
    .hostControllerDrivers = (USB_HOST_HCD *)&hcdTable    
};
// </editor-fold>
