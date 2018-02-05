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
/*** USB Driver Configuration ***/


<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true && CONFIG_PIC32MZ == true >
/* Enables Device Support */
#define DRV_USBHS_DEVICE_SUPPORT      true
<#elseif CONFIG_DRV_USB_DEVICE_SUPPORT == true && ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
/* Enables Device Support */
#define DRV_USBFS_DEVICE_SUPPORT      true
<#elseif CONFIG_DRV_USB_DEVICE_SUPPORT == false && CONFIG_PIC32MZ == true >
/* Disable Device Support */
#define DRV_USBHS_DEVICE_SUPPORT      false
<#elseif CONFIG_DRV_USB_DEVICE_SUPPORT == false && ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
/* Disable Device Support */
#define DRV_USBFS_DEVICE_SUPPORT      false
</#if>

<#if CONFIG_DRV_USB_HOST_SUPPORT == true && CONFIG_PIC32MZ == true >
/* Enables Host Support */
#define DRV_USBHS_HOST_SUPPORT      true
<#elseif CONFIG_DRV_USB_HOST_SUPPORT == true && ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
/* Enables Host Support */
#define DRV_USBFS_HOST_SUPPORT      true
<#elseif CONFIG_DRV_USB_HOST_SUPPORT == false && CONFIG_PIC32MZ == true >
/* Disable Host Support */
#define DRV_USBHS_HOST_SUPPORT      false
<#elseif CONFIG_DRV_USB_HOST_SUPPORT == false && ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
/* Disable Host Support */
#define DRV_USBFS_HOST_SUPPORT      false
</#if>

<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true || CONFIG_DRV_USB_HOST_SUPPORT == true>
<#if CONFIG_PIC32MZ == true >
/* Maximum USB driver instances */
#define DRV_USBHS_INSTANCES_NUMBER    1
<#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) ||(CONFIG_PIC32WK == true)) && CONFIG_DRV_USB_DRIVER_MODE == "DYNAMIC">
/* Maximum USB driver instances */
#define DRV_USBFS_INSTANCES_NUMBER    1
</#if>
</#if>

<#if CONFIG_DRV_USB_INTERRUPT_MODE == true>
<#if CONFIG_PIC32MZ == true>
/* Interrupt mode enabled */
#define DRV_USBHS_INTERRUPT_MODE      true
<#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
/* Interrupt mode enabled */
#define DRV_USBFS_INTERRUPT_MODE      true
</#if>
<#elseif CONFIG_DRV_USB_INTERRUPT_MODE == false>
<#if CONFIG_PIC32MZ == true>
/* Interrupt mode Disabled */
#define DRV_USBHS_INTERRUPT_MODE      false
<#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
/* Interrupt mode Disabled */
#define DRV_USBFS_INTERRUPT_MODE      false
</#if>
</#if>


/* Number of Endpoints used */
<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
<#if CONFIG_PIC32MZ == true>
#define DRV_USBHS_ENDPOINTS_NUMBER    ${CONFIG_DRV_USB_ENDPOINTS_NUMBER}
<#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
#define DRV_USBFS_ENDPOINTS_NUMBER    ${CONFIG_DRV_USB_ENDPOINTS_NUMBER}
</#if>
<#elseif CONFIG_DRV_USB_HOST_SUPPORT == true>
<#if CONFIG_PIC32MZ == true>
#define DRV_USBHS_ENDPOINTS_NUMBER    1
<#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
#define DRV_USBFS_ENDPOINTS_NUMBER    1
</#if>
</#if>




<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
<#include "/framework/usb/templates/system_config.h.device.ftl">
</#if>

<#if CONFIG_DRV_USB_HOST_SUPPORT == true>
<#include "/framework/usb/templates/system_config.h.host.ftl">
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
