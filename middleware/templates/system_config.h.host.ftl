
<#--
/*******************************************************************************
  MPLAB Harmony System Configuration Header

  File Name:
    system_config.h

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
    
    Created with MPLAB Harmony Version 1.01
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
<#if CONFIG_DRV_USB_HOST_SUPPORT == true >

<#if CONFIG_PIC32MZ == true>

#define DRV_USBHS_HOST_NAK_LIMIT      2000
<#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
#define DRV_USBFS_HOST_NAK_LIMIT      2000
</#if>
<#if CONFIG_PIC32MZ == true>
/* Provides Host pipes number */
#define DRV_USBHS_HOST_PIPES_NUMBER    10
<#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
/* Provides Host pipes number */
#define DRV_USBFS_HOST_PIPES_NUMBER    10
</#if>
<#if CONFIG_PIC32MZ == true>
#define DRV_USBHS_HOST_ATTACH_DEBOUNCE_DURATION 500
<#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
#define DRV_USBFS_HOST_ATTACH_DEBOUNCE_DURATION 500
</#if>
<#if CONFIG_PIC32MZ == true>
#define DRV_USBHS_HOST_RESET_DURATION 100
<#elseif ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
#define DRV_USBFS_HOST_RESET_DURATION 100
</#if>
// *****************************************************************************
// *****************************************************************************
// Section: USB Device Layer Configuration
// *****************************************************************************
// *****************************************************************************
/* Provides Host pipes number */
#define USB_HOST_PIPES_NUMBER    10

// *****************************************************************************
// *****************************************************************************
// Section: USB Host Layer Configuration
// *****************************************************************************
// **************************************************************************
 
/* Total number of devices to be supported */
#define USB_HOST_DEVICES_NUMBER         ${CONFIG_USB_HOST_DEVICE_NUMBER}

/* Target peripheral list entries */
#define  USB_HOST_TPL_ENTRIES           ${CONFIG_USB_HOST_TPL_ENTRY_NUMBER} 

/* Maximum number of configurations supported per device */
#define USB_HOST_DEVICE_INTERFACES_NUMBER     ${CONFIG_USB_HOST_MAX_INTERFACES}    

#define USB_HOST_CONTROLLERS_NUMBER           1

#define USB_HOST_TRANSFERS_NUMBER             10

/* Number of Host Layer Clients */
#define USB_HOST_CLIENTS_NUMBER               1   

<#if CONFIG_USB_HOST_USE_MSD == true>
/* Number of MSD Function driver instances in the application */
#define USB_HOST_MSD_INSTANCES_NUMBER         ${CONFIG_USB_HOST_MSD_NUMBER_OF_INSTANCES}

/* Number of Logical Units */
#define USB_HOST_SCSI_INSTANCES_NUMBER        ${CONFIG_USB_HOST_MSD_NUMBER_OF_INSTANCES}
#define USB_HOST_MSD_LUN_NUMBERS              ${CONFIG_USB_HOST_MSD_NUMBER_OF_INSTANCES}

</#if>
<#if CONFIG_USB_HOST_USE_CDC == true>
/* Number of CDC Function driver instances in the application */
#define USB_HOST_CDC_INSTANCES_NUMBER         ${CONFIG_USB_HOST_CDC_NUMBER_OF_INSTANCES}
#define USB_HOST_CDC_ATTACH_LISTENERS_NUMBER        ${CONFIG_USB_HOST_CDC_ATTACH_LISTENERS_NUMBER}
</#if>
<#if CONFIG_USB_HOST_USE_HUB == true>
/* Number of HUB Function driver instances in the application */
#define USB_HOST_HUB_INSTANCES_NUMBER         ${CONFIG_USB_HOST_HUB_NUMBER_OF_INSTANCES}

/* USB HOST Hub support */
#define USB_HOST_HUB_SUPPORT                  true

/* Number of ports supported per Instance */
#define USB_HOST_HUB_PORTS_NUMBER            8
</#if>
<#if CONFIG_USB_HOST_USE_HID == true>

/* Number of HID Client driver instances in the application */
#define USB_HOST_HID_INSTANCES_NUMBER        ${CONFIG_USB_HOST_HID_NUMBER_OF_INSTANCES}

/* Maximum number of INTERRUPT IN endpoints supported per HID interface */
#define USB_HOST_HID_INTERRUPT_IN_ENDPOINTS_NUMBER ${CONFIG_USB_HOST_HID_INTERRUPT_IN_ENDPOINTS_NUMBER}

/* Number of total usage driver instances registered with HID client driver */
#define USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER  ${CONFIG_USB_HID_TOTAL_USAGE_DRIVER_INSTANCES}

/* Maximum number PUSH items that can be saved in the Global item queue per field
 * per HID interface */
#define USB_HID_GLOBAL_PUSH_POP_STACK_SIZE ${CONFIG_USB_HID_GLOBAL_PUSH_POP_STACK_SIZE}
</#if>

<#if CONFIG_USB_HOST_USE_MOUSE == true>

/* Maximum number Mouse buttons whose value will be captured per HID Mouse device */
#define USB_HOST_HID_MOUSE_BUTTONS_NUMBER ${CONFIG_USB_HOST_HID_MOUSE_BUTTONS_NUMBER}

</#if>

<#if CONFIG_USB_HOST_USE_AUDIO == true>
/* Number of Audio v1.0 Function driver instances in the application */
#define USB_HOST_AUDIO_V1_INSTANCES_NUMBER         ${CONFIG_USB_HOST_AUDIO_NUMBER_OF_INSTANCES}

/* Maximum number of Streaming interfaces provides by any Device that will be
 be connected to this Audio Host */
#define USB_HOST_AUDIO_V1_STREAMING_INTERFACES_NUMBER ${CONFIG_USB_HOST_AUDIO_NUMBER_OF_STREAMING_INTERFACES}

/* Maximum number of Streaming interface alternate settings provided by any 
   Device that will be connected to this Audio Host. (This number includes 
   alternate setting 0) */
#define USB_HOST_AUDIO_V1_STREAMING_INTERFACE_ALTERNATE_SETTINGS_NUMBER ${CONFIG_USB_HOST_AUDIO_NUMBER_OF_STREAMING_INTERFACE_SETTINGS}

/* Maximum number of discrete Sampling frequencies supported by the Attached Audio Device */ 
#define USB_HOST_AUDIO_V1_SAMPLING_FREQUENCIES_NUMBER ${CONFIG_USB_HOST_AUDIO_NUMBER_OF_SAMPLING_FREQUENCIES}
</#if>

</#if>

<#--
/*******************************************************************************
 End of File
*/


