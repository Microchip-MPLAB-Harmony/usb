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
/*** USB Driver Configuration ***/

/* Maximum USB driver instances */
#define DRV_USBFSV1_INSTANCES_NUMBER                        1

<#if (USB_OPERATION_MODE?has_content)
	  && (USB_OPERATION_MODE == "Device")>

/* Enables Device Support */
#define DRV_USBFSV1_DEVICE_SUPPORT                          true
	
/* Disable Host Support */
#define DRV_USBFSV1_HOST_SUPPORT                            false
<#elseif (USB_OPERATION_MODE?has_content)
	  && (USB_OPERATION_MODE == "Host")>

/* Disable Device Support */
#define DRV_USBFSV1_DEVICE_SUPPORT                          false
	
/* Enable Host Support */
#define DRV_USBFSV1_HOST_SUPPORT                            true

/* Number of NAKs to wait before returning transfer failure */ 
#define DRV_USBFSV1_HOST_NAK_LIMIT                          2000 

/* Maximum Number of pipes */
#define DRV_USBFSV1_HOST_PIPES_NUMBER                       10 

/* Attach Debounce duration in milli Seconds */ 
#define DRV_USBFSV1_HOST_ATTACH_DEBOUNCE_DURATION           ${USB_DRV_HOST_ATTACH_DEBOUNCE_DURATION}

/* Reset duration in milli Seconds */ 
#define DRV_USBFSV1_HOST_RESET_DURATION                     ${USB_DRV_HOST_RESET_DUARTION}
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->


