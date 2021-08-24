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
/*** USB Driver Configuration ***/

/* Maximum USB driver instances */
#define DRV_USBFS_INSTANCES_NUMBER                        1

/* Interrupt mode enabled */
#define DRV_USBFS_INTERRUPT_MODE                          true

<#if (USB_OPERATION_MODE?has_content)
	  && (USB_OPERATION_MODE == "Device")>

/* Enables Device Support */
#define DRV_USBFS_DEVICE_SUPPORT                          true
	
/* Disable Host Support */
#define DRV_USBFS_HOST_SUPPORT                            false


<#elseif (USB_OPERATION_MODE?has_content)
	  && (USB_OPERATION_MODE == "Host")>

/* Disable Device Support */
#define DRV_USBFS_DEVICE_SUPPORT                          false
	
/* Enable Host Support */
#define DRV_USBFS_HOST_SUPPORT                            true

/* Number of NAKs to wait before returning transfer failure */ 
#define DRV_USBFS_HOST_NAK_LIMIT                          2000 

/* Maximum Number of pipes */
#define DRV_USBFS_HOST_PIPES_NUMBER                       ${usb_host.CONFIG_USB_HOST_PIPES_NUMBER}  

/* Attach Debounce duration in milli Seconds */ 
#define DRV_USBFS_HOST_ATTACH_DEBOUNCE_DURATION           ${USB_DRV_HOST_ATTACH_DEBOUNCE_DURATION}

/* Reset duration in milli Seconds */ 
#define DRV_USBFS_HOST_RESET_DURATION                     ${USB_DRV_HOST_RESET_DUARTION}

</#if>

/* Alignment for buffers that are submitted to USB Driver*/ 
#define USB_ALIGN  CACHE_ALIGN
<#--
/*******************************************************************************
 End of File
*/
-->


