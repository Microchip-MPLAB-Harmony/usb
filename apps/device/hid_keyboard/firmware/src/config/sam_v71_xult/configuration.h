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
// Section: System Configuration
// *****************************************************************************
// *****************************************************************************
#define DCACHE_CLEAN_BY_ADDR(data, size)       SCB_CleanDCache_by_Addr((uint32_t *)data, size)
#define DCACHE_INVALIDATE_BY_ADDR(data, size)  SCB_InvalidateDCache_by_Addr((uint32_t *)data, size)

#define DATA_CACHE_ENABLED                     true

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
#define DRV_USBHSV1_ENDPOINTS_NUMBER  3

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





/* Maximum instances of HID function driver */
#define USB_DEVICE_HID_INSTANCES_NUMBER     1

/* HID Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_HID_QUEUE_DEPTH_COMBINED 2



// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************

/*** LED Macros for LED0 ***/
#define APP_LED0_Off()                LED0_Off()
#define APP_LED0_On()                 LED0_On()
#define APP_LED0_Toggle()             LED0_Toggle()

/*** LED Macros for LED1 ***/
#define APP_LED1_Off()                LED1_Off()
#define APP_LED1_On()                 LED1_On()
#define APP_LED1_Toggle()             LED1_Toggle()

/*** SWITCH Macros for SWITCH1 ***/

#define APP_SWITCH0_Get()             SWITCH0_Get()
#define APP_SWITCH0_STATE_PRESSED     SWITCH0_STATE_PRESSED
#define APP_SWITCH0_STATE_RELEASED    SWITCH0_STATE_RELEASED

/*** SWITCH Macros for SWITCH2 ***/
#define APP_SWITCH1_Get()             SWITCH1_Get()
#define APP_SWITCH1_STATE_PRESSED     SWITCH1_STATE_PRESSED
#define APP_SWITCH1_STATE_RELEASED    SWITCH1_STATE_RELEASED


/* Tick time in 125 usec units */
#define APP_USB_SWITCH_DEBOUNCE_COUNT (100)
/* Macro defines USB internal DMA Buffer criteria*/
#define APP_MAKE_BUFFER_DMA_READY __attribute__((aligned(16)))
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // CONFIGURATION_H
/*******************************************************************************
 End of File
*/
