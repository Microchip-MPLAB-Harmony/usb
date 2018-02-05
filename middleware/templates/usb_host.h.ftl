<#--
/*******************************************************************************
  USB Host Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host.h.ftl

  Summary:
    USB Host Freemarker Template File

  Description:

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
// *****************************************************************************
// *****************************************************************************
// Section: USB Device Layer Configuration
// *****************************************************************************
// *****************************************************************************

<#-- Instance 0 -->
/* Maximum Host layer instances */
#define USB_HOST_INSTANCES_NUMBER  ${CONFIG_USB_HOST_INSTANCES_NUMBER}

/* Provides Host pipes number */
#define DRV_USB_HOST_PIPES_NUMBER    ${CONFIG_DRV_USB_HOST_PIPES_NUMBER}


/* NAK Limit for Control transfer data stage and Status Stage */
#define DRV_USB_HOST_NAK_LIMIT		${CONFIG_DRV_USB_HOST_NAK_LIMIT}


// *****************************************************************************
// *****************************************************************************
// Section: USB Host Layer Configuration
// *****************************************************************************
// **************************************************************************

/* Target peripheral list entries */
#define  USB_HOST_TPL_ENTRIES                ${CONFIG_USB_HOST_TPL_ENTRIES} 

/* Total number of drivers in this application */
#define USB_HOST_MAX_DRIVER_SUPPORTED        ${CONFIG_USB_HOST_MAX_DRIVER_SUPPORTED}    

/* Total number of devices to be supported */
#define USB_HOST_MAX_DEVICES_NUMBER         ${CONFIG_USB_HOST_MAX_DEVICES_NUMBER}

/* Maximum number of configurations supported per device */
#define USB_HOST_MAX_CONFIGURATION          ${CONFIG_USB_HOST_MAX_CONFIGURATION}      

/* Maximum number of interfaces supported per device */
#define USB_HOST_MAX_INTERFACES             ${CONFIG_USB_HOST_MAX_INTERFACES}

/* Number of Host Layer Clients */
#define USB_HOST_CLIENTS_NUMBER             ${CONFIG_USB_HOST_CLIENTS_NUMBER}   

<#if CONFIG_USB_HOST_USE_MSD == true>
/* Number of MSD Function driver instances in the application */
#define USB_HOST_MSD_INSTANCES_NUMBER         ${CONFIG_USB_HOST_MSD_NUMBER_OF_INSTANCES}



/* Number of Logical Units */
#define USB_HOST_SCSI_INSTANCES_NUMBER          ${CONFIG_USB_HOST_MSD_NUMBER_OF_INSTANCES}
</#if>


<#if CONFIG_USB_HOST_USE_CDC == true>
/* Number of MSD Function driver instances in the application */
#define USB_HOST_CDC_INSTANCES_NUMBER         ${CONFIG_USB_HOST_CDC_NUMBER_OF_INSTANCES}
</#if>

<#if CONFIG_USB_HOST_USE_HID == true>
/* Number of HID Client driver instances in the application */
#define USB_HOST_HID_INSTANCES_NUMBER         ${CONFIG_USB_HOST_HID_NUMBER_OF_INSTANCES}


</#if>

<#if CONFIG_USB_HOST_USE_HUB == true>
/* Number of HUB Client driver instances in the application */
#define USB_HOST_HUB_INSTANCES_NUMBER         ${CONFIG_USB_HOST_HUB_NUMBER_OF_INSTANCES}
</#if>

/*******************************************************************************
 End of File
*/


