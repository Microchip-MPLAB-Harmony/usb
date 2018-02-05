<#--
/*******************************************************************************
  USB  Host Initialization File

  File Name:
    usb_host_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
 *******************************************************************************/

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
// <editor-fold defaultstate="collapsed" desc="USB Host Initialization Data"> 
<#if CONFIG_DRV_USB_HOST_SUPPORT == true>

<#if CONFIG_USB_HOST_USE_MOUSE == true>

USB_HOST_HID_USAGE_DRIVER_INTERFACE usageDriverInterface =
{
  .initialize = NULL,
  .deinitialize = NULL,
  .usageDriverEventHandler = _USB_HOST_HID_MOUSE_EventHandler,
  .usageDriverTask = _USB_HOST_HID_MOUSE_Task
};

USB_HOST_HID_USAGE_DRIVER_TABLE_ENTRY usageDriverTableEntry[1] =
{
    {
        .usage = (USB_HID_USAGE_PAGE_GENERIC_DESKTOP_CONTROLS << 16) | USB_HID_USAGE_MOUSE,
        .initializeData = NULL,
        .interface = &usageDriverInterface
    }
};

</#if>

<#if CONFIG_USB_HOST_USE_KEYBOARD == true>

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

</#if>

<#if CONFIG_USB_HOST_USE_HID == true>
USB_HOST_HID_INIT hidInitData =
{
    .nUsageDriver = 1,
    .usageDriverTable = usageDriverTableEntry
};
</#if>
const USB_HOST_TPL_ENTRY USBTPList[ ${CONFIG_USB_HOST_TPL_ENTRY_NUMBER} ] =
{
	
<#if CONFIG_USB_HOST_USE_MSD == true>
    TPL_INTERFACE_CLASS_SUBCLASS_PROTOCOL(0x08, 0x06, 0x50, NULL,  USB_HOST_MSD_INTERFACE) ,
</#if>

<#if CONFIG_USB_HOST_USE_MOUSE == true>
    TPL_INTERFACE_CLASS_SUBCLASS_PROTOCOL(0x03, 0x01, 0x02, &hidInitData,  USB_HOST_HID_INTERFACE) ,
</#if>

<#if CONFIG_USB_HOST_USE_KEYBOARD == true>
    TPL_INTERFACE_CLASS_SUBCLASS_PROTOCOL(0x03, 0x01, 0x01, &hidInitData,  USB_HOST_HID_INTERFACE) ,
</#if>

<#if CONFIG_USB_HOST_USE_HUB == true>
    TPL_INTERFACE_CLASS_SUBCLASS(0x09, 0x00, NULL,  USB_HOST_HUB_INTERFACE),
</#if>

<#if CONFIG_USB_HOST_USE_CDC == true>
    TPL_INTERFACE_CLASS(0x02, NULL,  USB_HOST_CDC_INTERFACE),
</#if>

<#if CONFIG_USB_HOST_USE_AUDIO == true>
    TPL_INTERFACE_CLASS(USB_AUDIO_CLASS_CODE, NULL, (void*)USB_HOST_AUDIO_V1_INTERFACE),
</#if>

};

<#if (CONFIG_PIC32MX || CONFIG_PIC32MK || CONFIG_PIC32WK)>
const USB_HOST_HCD hcdTable = 
{
    .drvIndex = DRV_USBFS_INDEX_0,
    .hcdInterface = DRV_USBFS_HOST_INTERFACE
};
</#if>

<#if CONFIG_PIC32MZ>
const USB_HOST_HCD hcdTable = 
{
    .drvIndex = DRV_USBHS_INDEX_0,
    .hcdInterface = DRV_USBHS_HOST_INTERFACE
};
</#if>

const USB_HOST_INIT usbHostInitData = 
{
    .nTPLEntries = ${CONFIG_USB_HOST_TPL_ENTRY_NUMBER} ,
    .tplList = (USB_HOST_TPL_ENTRY *)USBTPList,
    .hostControllerDrivers = (USB_HOST_HCD *)&hcdTable
    
};
</#if>
// </editor-fold>
<#--
/*******************************************************************************
 End of File
*/
-->
