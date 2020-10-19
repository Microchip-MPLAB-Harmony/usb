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
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
<#if (USB_DRV_EHCI_MENU)?has_content == true>
    <#if USB_DRV_OHCI_MENU == true>
	
/*** USB EHCI Driver Configurations ***/

/* Maximum USB driver instances */
#define DRV_USB_EHCI_INSTANCES_NUMBER                     1

/* Attach Debounce duration in milli Seconds */ 
#define DRV_USB_EHCI_ATTACH_DEBOUNCE_DURATION           500

/* Reset duration in milli Seconds */ 
#define DRV_USB_EHCI_RESET_DURATION                     100

/* Maximum Control Transfer Size */
#define DRV_USB_EHCI_CONTROL_TRANSFER_BUFFER_SIZE ${USB_DRV_HOST_EHCI_BUFFER_SIZE_CONTROL}

/* Maximum Non Control Transfer Size */ 
#define DRV_USB_EHCI_TRANSFER_BUFFER_SIZE  ${USB_DRV_HOST_EHCI_BUFFER_SIZE}
    </#if>

<#else>

/* EHCI Driver is not used */  
#define DRV_USB_EHCI_INSTANCES_NUMBER                     0
</#if>
	


<#if (USB_DRV_OHCI_MENU)?has_content == true>
	<#if USB_DRV_OHCI_MENU == true>
/*** USB OHCI Driver Configurations ***/

#define DRV_USB_OHCI_INSTANCES_NUMBER                        1

/* Attach Debounce duration in milli Seconds */ 
#define DRV_USB_OHCI_ATTACH_DEBOUNCE_DURATION           500

/* Reset duration in milli Seconds */ 
#define DRV_USB_OHCI_RESET_DURATION                     100

/* Maximum Control Transfer Size */
#define DRV_USB_OHCI_CONTROL_TRANSFER_BUFFER_SIZE ${USB_DRV_HOST_OHCI_BUFFER_SIZE_CONTROL}

/* Maximum Non Control Transfer Size */ 
#define DRV_USB_OHCI_TRANSFER_BUFFER_SIZE  ${USB_DRV_HOST_OHCI_BUFFER_SIZE}
    </#if>
</#if>


/* Alignment for buffers that are submitted to USB Driver*/ 
#ifndef USB_ALIGN
#define USB_ALIGN __ALIGNED(32)
#endif 
<#--
/*******************************************************************************
 End of File
*/
-->


