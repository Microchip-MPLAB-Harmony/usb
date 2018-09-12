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


// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************


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
#define DRV_USBHSV1_ENDPOINTS_NUMBER  3 //TODO  Calculate this for Device

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





/* Endpoint Transfer Queue Size combined for Read and write */
#define USB_DEVICE_ENDPOINT_QUEUE_DEPTH_COMBINED    2

/* Maximum instances of HID function driver */
#define USB_DEVICE_HID_INSTANCES_NUMBER     1

/* HID Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_HID_QUEUE_DEPTH_COMBINED 5



// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************


/*** Functions for BSP_LED_2 pin ***/
#define BSP_LED_2Toggle() SYS_PORTS_PinToggle(PORTS_ID_0, PORTS_CHANNEL_C, PORTS_BIT_POS_30)
#define BSP_LED_2On() SYS_PORTS_PinClear(PORTS_ID_0, PORTS_CHANNEL_C, PORTS_BIT_POS_30)
#define BSP_LED_2Off() SYS_PORTS_PinSet(PORTS_ID_0, PORTS_CHANNEL_C, PORTS_BIT_POS_30)
#define BSP_LED_2StateGet() !(SYS_PORTS_PinRead(PORTS_ID_0, PORTS_CHANNEL_C, PORTS_BIT_POS_30))

/*** Functions for BSP_LED_3 pin ***/
#define BSP_LED_3Toggle() SYS_PORTS_PinToggle(PORTS_ID_0, PORTS_CHANNEL_B, PORTS_BIT_POS_2)
#define BSP_LED_3On() SYS_PORTS_PinClear(PORTS_ID_0, PORTS_CHANNEL_B, PORTS_BIT_POS_2)
#define BSP_LED_3Off() SYS_PORTS_PinSet(PORTS_ID_0, PORTS_CHANNEL_B, PORTS_BIT_POS_2)
#define BSP_LED_3StateGet() !(SYS_PORTS_PinRead(PORTS_ID_0, PORTS_CHANNEL_B, PORTS_BIT_POS_2))

/*** Functions for BSP_LED_1 pin ***/
#define BSP_LED_1Toggle() SYS_PORTS_PinToggle(PORTS_ID_0, PORTS_CHANNEL_A, PORTS_BIT_POS_0)
#define BSP_LED_1On() SYS_PORTS_PinClear(PORTS_ID_0, PORTS_CHANNEL_A, PORTS_BIT_POS_0)
#define BSP_LED_1Off() SYS_PORTS_PinSet(PORTS_ID_0, PORTS_CHANNEL_A, PORTS_BIT_POS_0)
#define BSP_LED_1StateGet() !(SYS_PORTS_PinRead(PORTS_ID_0, PORTS_CHANNEL_A, PORTS_BIT_POS_0))

/*** Functions for BSP_SWITCH_0 pin ***/
#define BSP_SWITCH_0StateGet() SYS_PORTS_PinRead(PORTS_ID_0, PORTS_CHANNEL_A, PORTS_BIT_POS_9)

/*** Functions for BSP_USB_VBUS_IN pin ***/
#define BSP_USB_VBUS_INStateGet() SYS_PORTS_PinRead(PORTS_ID_0, PORTS_CHANNEL_C, PORTS_BIT_POS_9)


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
