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
#include "toolchain_specifics.h"

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



// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
/* TIME System Service Configuration Options */
#define SYS_TIME_INDEX_0                     0
#define SYS_TIME_MAX_TIMERS                  5
#define SYS_TIME_HW_COUNTER_WIDTH            16
#define SYS_TIME_HW_COUNTER_PERIOD           65535U
#define SYS_TIME_HW_COUNTER_HALF_PERIOD	     (SYS_TIME_HW_COUNTER_PERIOD>>1)
#define SYS_TIME_CPU_CLOCK_FREQUENCY         119996416
#define SYS_TIME_COMPARE_UPDATE_EXECUTION_CYCLES      (188)



// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/* USART Driver Instance 0 Configuration Options */
#define DRV_USART_INDEX_0                  0
#define DRV_USART_CLIENTS_NUMBER_IDX0      1
#define DRV_USART_QUEUE_SIZE_IDX0          5

/* USART Driver Global Configuration Options */
#define DRV_USART_INSTANCES_NUMBER         1


// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************
/*** USB Driver Configuration ***/

/* Maximum USB driver instances */
#define DRV_USB_UHP_INSTANCES_NUMBER                         1

/* Interrupt mode enabled */
#define DRV_USB_UHP_INTERRUPT_MODE                           true

/* Number of NAKs to wait before returning transfer failure */ 
#define DRV_USB_UHP_NAK_LIMIT                          2000 

/* Maximum Number of pipes */
#define DRV_USB_UHP_PIPES_NUMBER                       10  

/* Attach Debounce duration in milli Seconds */ 
#define DRV_USB_UHP_ATTACH_DEBOUNCE_DURATION           500

/* Reset duration in milli Seconds */ 
#define DRV_USB_UHP_RESET_DURATION                     100

/* Maximum Transfer Size */ 
#define DRV_USB_UHP_NO_CACHE_BUFFER_LENGTH  512

/* Alignment for buffers that are submitted to USB Driver*/ 
#ifndef USB_ALIGN
#define USB_ALIGN __ALIGNED(32)
#endif 

// *****************************************************************************
// *****************************************************************************
// Section: USB Host Layer Configuration
// *****************************************************************************
// **************************************************************************

/* Number of Endpoints used */
#define DRV_USB_UHP_ENDPOINTS_NUMBER                        1

#define USB_HOST_DEVICES_NUMBER                             1

/* Size of Endpoint 0 buffer */
#define USB_DEVICE_EP0_BUFFER_SIZE                          64

/* Target peripheral list entries */
#define  USB_HOST_TPL_ENTRIES                               1 

/* Maximum number of configurations supported per device */
#define USB_HOST_DEVICE_INTERFACES_NUMBER                   5    

#define USB_HOST_CONTROLLERS_NUMBER                         1

#define USB_HOST_TRANSFERS_NUMBER                           10

/* Provides Host pipes number */
#define USB_HOST_PIPES_NUMBER                               10

/* Number of Host Layer Clients */
#define USB_HOST_CLIENTS_NUMBER                             1   

/* Number of HID Client driver instances in the application */
#define USB_HOST_HID_INSTANCES_NUMBER        1

/* Maximum number of INTERRUPT IN endpoints supported per HID interface */
#define USB_HOST_HID_INTERRUPT_IN_ENDPOINTS_NUMBER 1

/* Number of total usage driver instances registered with HID client driver */
#define USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER  1

/* Maximum number PUSH items that can be saved in the Global item queue per field
 * per HID interface */
#define USB_HID_GLOBAL_PUSH_POP_STACK_SIZE 1

/* Maximum number Mouse buttons whose value will be captured per HID Mouse device */
#define USB_HOST_HID_MOUSE_BUTTONS_NUMBER 5



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
