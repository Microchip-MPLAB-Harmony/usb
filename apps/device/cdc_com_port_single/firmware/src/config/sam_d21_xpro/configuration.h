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
#define DCACHE_CLEAN_BY_ADDR(data, size)
#define DCACHE_INVALIDATE_BY_ADDR(data, size)

#define DATA_CACHE_ENABLED                         false

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
/* Maximum instances of CDC function driver */
#define USB_DEVICE_CDC_INSTANCES_NUMBER     1

/* CDC Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED 4

/*** USB Driver Configuration ***/

/* Maximum USB driver instances */
#define DRV_USBFSV1_INSTANCES_NUMBER  1

/* Interrupt mode enabled */
#define DRV_USBFSV1_INTERRUPT_MODE    true




/* EP0 size in bytes */
#define USB_DEVICE_EP0_NUMBER_OF_BANKS      2
    
/* Number of Endpoints used */
#define DRV_USBFSV1_ENDPOINTS_NUMBER  7 //TODO  Calculate this for Device 

/* Enables Device Support */
#define DRV_USBFSV1_DEVICE_SUPPORT    true
	
/* Disable Host Support */
#define DRV_USBFSV1_HOST_SUPPORT      false

/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER     1 

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE      64
#define USB_DEVICE_EP1_BUFFER_SIZE      16
#define USB_DEVICE_EP2_BUFFER_SIZE      64

/* Enable SOF Events */ 
#define USB_DEVICE_SOF_EVENT_ENABLE     



typedef enum
{
	// Last transaction on Even Buffer
	USB_PING_PONG_EVEN /*DOM-IGNORE-BEGIN*/ = 0 /*DOM-IGNORE-END*/ ,
	// Last transaction on Odd  Buffer
	USB_PING_PONG_ODD  /*DOM-IGNORE-BEGIN*/ = 1 /*DOM-IGNORE-END*/

} USB_PING_PONG_STATE;

#define USB_PING_PONG_MASK(value)  /*DOM-IGNORE-BEGIN*/ (0x0001 << value)/*DOM-IGNORE-END*/

typedef enum
{
	USB_BUFFER_DATA0, // DATA0/1 = 0
	USB_BUFFER_DATA1  // DATA0/1 = 1

} USB_BUFFER_DATA01;



/* Number of Endpoints used */
#define USB_DEVICE_ENDPOINT_NUMBER_MASK				0x0F
#define USB_DEVICE_ENDPOINT_DIRECTION_MASK			0x80

#define USB_DEVICE_AUTO_ZLP							0

#define APP_MAKE_BUFFER_DMA_READY  __attribute__((aligned(16)))
#define APP_READ_BUFFER_SIZE 512
#define APP_USB_SWITCH_DEBOUNCE_COUNT_FS 260
#define APP_USB_SWITCH_DEBOUNCE_COUNT_HS 500

//#define ENABLE_DEBUG_MESSAGE


#if defined ENABLE_DEBUG_MESSAGE
#define DEBUG_MESSAGE(value)			usart_async_data_write(value)
#else
#define DEBUG_MESSAGE(value)
#endif


/*** LED Macros for LED0 ***/
#define APP_LED0_Off()                LED_Off()
#define APP_LED0_On()                 LED_On()
#define APP_LED0_Toggle()             LED_Toggle()
    
/*** LED Macros for LED1 ***/
#define APP_LED1_Off()                LED_Off()
#define APP_LED1_On()                 LED_On()
#define APP_LED1_Toggle()             LED_Toggle()
                                  
/*** SWITCH Macros for SWITCH1 ***/
    
#define APP_SWITCH0_Get()             SWITCH_Get()
#define APP_SWITCH0_STATE_PRESSED     SWITCH_STATE_PRESSED
#define APP_SWITCH0_STATE_RELEASED    SWITCH_STATE_RELEASED
    
/*** SWITCH Macros for SWITCH2 ***/
#define APP_SWITCH1_Get()             SWITCH_Get()
#define APP_SWITCH1_STATE_PRESSED     SWITCH_STATE_PRESSED
#define APP_SWITCH1_STATE_RELEASED    SWITCH_STATE_RELEASED


// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // CONFIGURATION_H
/*******************************************************************************
 End of File
*/
