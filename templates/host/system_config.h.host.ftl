
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
<#elseif CONFIG_USB_DRIVER_INTERFACE == "DRV_USBHS_HOST_INTERFACE">
#define DRV_USBHS_ENDPOINTS_NUMBER 							1
<#elseif CONFIG_USB_DRIVER_INTERFACE == "DRV_USBFSV1_HOST_INTERFACE">
#define DRV_USBFSV1_ENDPOINTS_NUMBER                        1
<#elseif CONFIG_USB_DRIVER_INTERFACE == "DRV_USB_UHP_HOST_INTERFACE">
#define DRV_USB_UHP_ENDPOINTS_NUMBER                        1
</#if>

/* Total number of devices to be supported */
#define USB_HOST_DEVICES_NUMBER                             ${CONFIG_USB_HOST_DEVICE_NUMNBER}

/* Size of Endpoint 0 buffer */
#define USB_DEVICE_EP0_BUFFER_SIZE                          64

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


