<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.device_msd_common.ftl

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
/* Maximum instances of MSD function driver */
#define USB_DEVICE_MSD_INSTANCES_NUMBER     ${__INSTANCE_COUNT} 

#define USB_DEVICE_MSD_NUM_SECTOR_BUFFERS ${CONFIG_USB_DEVICE_FUNCTION_MSD_MAX_SECTORS_COMMON}

<#-- Find out max LUN -->
<#assign maxLUN = 0>
<#if (usb_device_msd_0.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN)?has_content == true>
    <#assign maxLUN = usb_device_msd_0.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN >
</#if>
<#if (usb_device_msd_1.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN)?has_content == true>
    <#if usb_device_msd_1.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN gt maxLUN >
        <#assign maxLUN = usb_device_msd_1.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN>
    </#if>
</#if>
<#if (usb_device_msd_2.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN)?has_content == true>
    <#if usb_device_msd_2.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN gt maxLUN >
        <#assign maxLUN = usb_device_msd_2.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN>
    </#if>
</#if>
<#if (usb_device_msd_3.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN)?has_content == true>
    <#if usb_device_msd_3.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN gt maxLUN >
        <#assign maxLUN = usb_device_msd_3.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN>
    </#if>
</#if>
<#if (usb_device_msd_4.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN)?has_content == true>
    <#if usb_device_msd_4.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN gt maxLUN >
        <#assign maxLUN = usb_device_msd_4.CONFIG_USB_DEVICE_FUNCTION_MSD_LUN>
    </#if>
</#if>

/* Number of Logical Units */
#define USB_DEVICE_MSD_LUNS_NUMBER      ${maxLUN}


<#--
/*******************************************************************************
 End of File
*/
-->

