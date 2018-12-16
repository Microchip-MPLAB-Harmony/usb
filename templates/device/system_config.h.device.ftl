<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.device.ftl

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
<#-- Instance 0 -->
/* Number of Endpoints used */
<#if CONFIG_USB_DRIVER_INTERFACE == "DRV_USBHSV1_DEVICE_INTERFACE">
#define DRV_USBHSV1_ENDPOINTS_NUMBER                        ${CONFIG_USB_DEVICE_ENDPOINTS_NUMBER}
<#elseif CONFIG_USB_DRIVER_INTERFACE == "DRV_USBFSV1_DEVICE_INTERFACE">
#define DRV_USBFSV1_ENDPOINTS_NUMBER                        ${CONFIG_USB_DEVICE_ENDPOINTS_NUMBER}
</#if>

/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT 

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER                         1 

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE                          ${CONFIG_USB_DEVICE_EP0_BUFFER_SIZE}

<#if CONFIG_USB_DEVICE_EVENT_ENABLE_SOF == true>
/* Enable SOF Events */ 
#define USB_DEVICE_SOF_EVENT_ENABLE     
</#if>

<#if CONFIG_USB_DEVICE_EVENT_ENABLE_SET_DESCRIPTOR == true>
/* Enable Set descriptor events */
#define USB_DEVICE_SET_DESCRIPTOR_EVENT_ENABLE
</#if>

<#if CONFIG_USB_DEVICE_EVENT_ENABLE_SYNCH_FRAME == true>
/* Enable Synch Frame Event */ 
#define USB_DEVICE_SYNCH_FRAME_EVENT_ENABLE
</#if>

<#if CONFIG_USB_DEVICE_FEATURE_ENABLE_BOS_DESCRIPTOR == true>
/* Enable BOS Descriptor */
#define USB_DEVICE_BOS_DESCRIPTOR_SUPPORT_ENABLE
</#if>

<#if CONFIG_USB_DEVICE_FEATURE_ENABLE_ADVANCED_STRING_DESCRIPTOR_TABLE == true>
/* Enable Advanced String Descriptor table. This feature lets the user specify 
   String Index along with the String descriptor Structure  */
#define USB_DEVICE_STRING_DESCRIPTOR_TABLE_ADVANCED_ENABLE
<#if CONFIG_USB_DEVICE_FEATURE_ENABLE_MICROSOFT_OS_DESCRIPTOR == true>

/* Enable Microsoft OS Descriptor support.  */
#define USB_DEVICE_MICROSOFT_OS_DESCRIPTOR_SUPPORT_ENABLE
</#if>
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->

