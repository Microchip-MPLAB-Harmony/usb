
<#--
/*******************************************************************************
  MPLAB Harmony System Configuration Header

  File Name:
    system_config.h.host.ftl

  Summary:
    Build-time configuration header for the system defined by this MPLAB Harmony
    project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options that are not instantiated
    until used by another MPLAB Harmony module or application.
    
*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END
-->
// *****************************************************************************
// *****************************************************************************
// Section: USB Host Layer Configuration
// *****************************************************************************
// **************************************************************************

/* Number of Endpoints used */
<#if CONFIG_USB_DRIVER_INTERFACE == "DRV_USBHSV1_HOST_INTERFACE">
#define DRV_USBHSV1_ENDPOINTS_NUMBER                        1
<#elseif CONFIG_USB_DRIVER_INTERFACE == "DRV_USBFSV1_HOST_INTERFACE">
#define DRV_USBFSV1_ENDPOINTS_NUMBER                        1
</#if>

/* Total number of devices to be supported */
#define USB_HOST_DEVICES_NUMBER                             ${CONFIG_USB_HOST_DEVICE_NUMNBER}

/* Target peripheral list entries */
#define  USB_HOST_TPL_ENTRIES                               ${CONFIG_USB_HOST_TPL_ENTRY_NUMBER} 

/* Maximum number of configurations supported per device */
#define USB_HOST_DEVICE_INTERFACES_NUMBER                   ${CONFIG_USB_HOST_MAX_INTERFACES}    

#define USB_HOST_CONTROLLERS_NUMBER                         1

#define USB_HOST_TRANSFERS_NUMBER                           10

/* Provides Host pipes number */
#define USB_HOST_PIPES_NUMBER                               10

/* Number of Host Layer Clients */
#define USB_HOST_CLIENTS_NUMBER                             1   
<#--
/*******************************************************************************
 End of File
*/


