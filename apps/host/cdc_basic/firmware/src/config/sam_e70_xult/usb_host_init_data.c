#include "configuration.h"
#include "definitions.h" 
 
const USB_HOST_TPL_ENTRY USBTPList[1] = 
{
	TPL_INTERFACE_CLASS(0x02, NULL,  USB_HOST_CDC_INTERFACE),


};

const USB_HOST_HCD hcdTable = 
{
    .drvIndex = DRV_USBHSV1_INDEX_0,
    .hcdInterface = DRV_USBHSV1_HOST_INTERFACE
};

const USB_HOST_INIT usbHostInitData = 
{
    .nTPLEntries = 1 ,
    .tplList = (USB_HOST_TPL_ENTRY *)USBTPList,
    .hostControllerDrivers = (USB_HOST_HCD *)&hcdTable    
};
// </editor-fold>
