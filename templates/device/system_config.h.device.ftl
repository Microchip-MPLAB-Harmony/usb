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
<#-- Instance 0 -->
/* Number of Endpoints used */
<#if CONFIG_USB_DRIVER_INTERFACE == "DRV_USBHSV1_DEVICE_INTERFACE">
#define DRV_USBHSV1_ENDPOINTS_NUMBER                        ${CONFIG_USB_DEVICE_ENDPOINTS_NUMBER + 1}U
<#elseif  CONFIG_USB_DRIVER_INTERFACE == "DRV_USBHS_DEVICE_INTERFACE">
#define DRV_USBHS_ENDPOINTS_NUMBER                        ${CONFIG_USB_DEVICE_ENDPOINTS_NUMBER + 2}U
<#elseif CONFIG_USB_DRIVER_INTERFACE == "DRV_USBFSV1_DEVICE_INTERFACE">
#define DRV_USBFSV1_ENDPOINTS_NUMBER                        ${CONFIG_USB_DEVICE_ENDPOINTS_NUMBER + 1}U
<#elseif  CONFIG_USB_DRIVER_INTERFACE == "DRV_USBFS_DEVICE_INTERFACE">
#define DRV_USBFS_ENDPOINTS_NUMBER                        ${CONFIG_USB_DEVICE_ENDPOINTS_NUMBER + 2}U
<#elseif  CONFIG_USB_DRIVER_INTERFACE == "DRV_USB_UDPHS_DEVICE_INTERFACE">
#define DRV_USB_UDPHS_ENDPOINTS_NUMBER                    ${CONFIG_USB_DEVICE_ENDPOINTS_NUMBER + 1}U
<#elseif  CONFIG_USB_DRIVER_INTERFACE == "DRV_USBDP_INTERFACE">
#define DRV_USBDP_ENDPOINTS_NUMBER                        ${CONFIG_USB_DEVICE_ENDPOINTS_NUMBER + 1}U
</#if>

/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER                         1U

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE                          ${CONFIG_USB_DEVICE_EP0_BUFFER_SIZE}U

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

/* Enable Microsoft OS Descriptor Vendor code  */
#define USB_DEVICE_MICROSOFT_OS_DESCRIPTOR_VENDOR_CODE ${CONFIG_USB_DEVICE_FEATURE_ENABLE_MICROSOFT_OS_DESCRIPTOR_VENDOR_CODE}

</#if>
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->

