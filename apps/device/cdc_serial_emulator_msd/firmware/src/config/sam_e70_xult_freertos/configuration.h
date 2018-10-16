/*******************************************************************************
  System Configuration Header

  File Name:
    configuration.h

  Summary:
    Build-time configuration header for the system defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017-2018 released Microchip Technology Inc.  All rights reserved.

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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

#include "user.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
#define DCACHE_CLEAN_BY_ADDR(data, size)       SCB_CleanDCache_by_Addr((uint32_t *)data, size)
#define DCACHE_INVALIDATE_BY_ADDR(data, size)  SCB_InvalidateDCache_by_Addr((uint32_t *)data, size)

#define DATA_CACHE_ENABLED                     true
/* File System Service Configuration */

#define SYS_FS_MEDIA_NUMBER               1

#define SYS_FS_VOLUME_NUMBER              1

#define SYS_FS_AUTOMOUNT_ENABLE           false
#define SYS_FS_MAX_FILES                  1
#define SYS_FS_MAX_FILE_SYSTEM_TYPE       1
#define SYS_FS_MEDIA_MAX_BLOCK_SIZE       512
#define SYS_FS_MEDIA_MANAGER_BUFFER_SIZE  2048
#define SYS_FS_FILE_NAME_LEN              255
#define SYS_FS_CWD_STRING_LEN             1







// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/* Memory Driver Global Configuration Options */
#define DRV_MEMORY_INSTANCES_NUMBER         2

/* Memory Driver Instance 0 Configuration */
#define DRV_MEMORY_INDEX_0                   0
#define DRV_MEMORY_CLIENTS_NUMBER_IDX0       1
#define DRV_MEMORY_BUFFER_QUEUE_SIZE_IDX0    1
#define DRV_MEMORY_DEVICE_START_ADDRESS      0x500000
#define DRV_MEMORY_DEVICE_MEDIA_SIZE         1024UL




// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************
/*** USB Driver Configuration ***/

/* Maximum USB driver instances */
#define DRV_USBHSV1_INSTANCES_NUMBER  1

/* Interrupt mode enabled */
#define DRV_USBHSV1_INTERRUPT_MODE    true




/* Number of Endpoints used */
#define DRV_USBHSV1_ENDPOINTS_NUMBER  6 //TODO  Calculate this for Device

/* Enables Device Support */
#define DRV_USBHSV1_DEVICE_SUPPORT    true

/* Disable Host Support */
#define DRV_USBHSV1_HOST_SUPPORT      false




/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER     1

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE      64

/* Enable SOF Events */ 
#define USB_DEVICE_SOF_EVENT_ENABLE     


/* Number of Logical Units */
#define USB_DEVICE_MSD_LUNS_NUMBER      1

/* Size of disk image (in KB) in Program Flash Memory */
#define DRV_NVM_BLOCK_MEMORY_SIZE       36


/* Maximum instances of MSD function driver */
#define USB_DEVICE_MSD_INSTANCES_NUMBER     1

#define USB_DEVICE_MSD_NUM_SECTOR_BUFFERS 1


/* Endpoint Transfer Queue Size combined for Read and write */
//#define USB_DEVICE_ENDPOINT_QUEUE_DEPTH_COMBINED    2

/* Maximum instances of CDC function driver */
#define USB_DEVICE_CDC_INSTANCES_NUMBER     1

/* CDC Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED 3


/*** USB Device Stack Configuration ***/

#define USB_DEVICE_MSD_NUM_SECTOR_BUFFERS 1


// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************



/*** Application Instance 0 Configuration ***/

#define APP_READ_BUFFER_SIZE 512

/* Macro defines USB internal DMA Buffer criteria*/

#define APP_MAKE_BUFFER_DMA_READY  __attribute__((aligned(16)))

/* Macros defines board specific led */

#define APP_USB_LED_1    BSP_LED_1

/* Macros defines board specific led */

#define APP_USB_LED_2    BSP_LED_2

/* Macros defines board specific led */

#define APP_USB_LED_3    BSP_LED_3

/* Macros defines board specific switch */

#define APP_USB_SWITCH_1    BSP_SWITCH_0

/* Number of Endpoints used */

#define APP_EP_BULK_IN  2

/* Number of Endpoints used */

#define APP_EP_BULK_OUT 1
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // CONFIGURATION_H
/*******************************************************************************
 End of File
*/
