/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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
//DOM-IGNORE-END

#ifndef _APP_H
#define _APP_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"

#include "usb/usb_chapter_9.h"
#include "usb/usb_device.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
/* Switch De-bounce count in 10 milli Seconds unit. Value of 20 means a 
 * De-bounce count of 200 milli Seconds. */
#define APP_USB_SWITCH_DEBOUNCE_COUNT 20 
// *****************************************************************************
/* USB Task states

  Summary:
    USB Task states enumeration

  Description:
    This enumeration defines the valid USB Task states. These states
    determine the behavior of the USB Task at various times.
*/

typedef enum
{
    /* Initial state. */
    APP_USB_STATE_INIT = 0,

    /* Device Attach state */
    APP_USB_STATE_DEVICE_ATTACH,

    /* Running state */
    APP_USB_STATE_RUNNING,

    /* Idle state */
    APP_USB_STATE_IDLE

} APP_USB_STATES;

// *****************************************************************************
/* FS Task States 

  Summary:
    FS Task states enumeration

  Description:
    This enumeration defines the valid FS Task states. These states
    determine the behavior of the FS Task at various times.
*/

typedef enum
{
    /* Initial state. */
    APP_FS_STATE_INIT = 0,

    /* Wait for SW1 switch press */
    APP_FS_STATE_WAIT_FOR_SWITCH_PRESS,

    /* Wait for SW1 switch release */
    APP_FS_STATE_WAIT_FOR_SWITCH_RELEASE,

    /* Mount the FS */
    APP_FS_STATE_MOUNT_DISK,

    /* Open the file */
    APP_FS_STATE_OPEN_FILE,

    /* Read from the file */
    APP_FS_STATE_READ_FILE,

    /* Check for End of File. */
    APP_FS_STATE_CHECK_EOF,

    /* Close the file */
    APP_FS_STATE_CLOSE_FILE,

    /* Unmount the FS */
    APP_FS_STATE_UNMOUNT_DISK,
    
    /* Re attach USB Device Host as File operation is complete  */        
    APP_FS_STATE_RE_ATTACH_USB, 

    /* Error state */
    APP_FS_STATE_ERROR,
            
    APP_FS_STATE_SET_TIMER_CALLBACK

} APP_FS_STATES;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* USB Sub task's current state */
    APP_USB_STATES usbState;

    /* FS Sub task's current state */
    APP_FS_STATES fsState;

    /* USB Device Handle */
    USB_DEVICE_HANDLE usbDeviceHandle;
    
    /* Flag to track the USB Connect Status */
    bool isUsbConnected;   

    /* Flag to track the FS Running Status */
    bool isFsRunning;

    /* SYS_FS File handle */
    SYS_FS_HANDLE fileHandle;

    /* Application data buffer */
    uint8_t fsBuffer[8];

    /* Variable to track the number of LEDs changes requested */
    uint8_t numLedChange;
    
    /* System Timer Handle */ 
    SYS_TIME_HANDLE timerHandle; 
    
    /* System Timer Handle */ 
    SYS_TIME_HANDLE timerHandlePeriodic; 

    /* Switch 1 De-bounce Timer */ 
    uint32_t switchDebounceTimer; 
    
    /* Flag indicates switch 1 is pressed */ 
    bool isSwitch1Pressed; 
    
    /* Flag used for switch de-bouncing */ 
    bool ignoreSwitchPress; 
    
    /* Indicates timer event has occurred. Used for switch de-bouncing  */ 
    bool isTimerEventOccured; 

} APP_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony Demo application initialization routine

  Description:
    This routine initializes Harmony Demo application.  This function opens
    the necessary drivers, initializes the timer and registers the application
    callback with the USART driver.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks ( void );

// *****************************************************************************
// *****************************************************************************
// Section: Application Local function prototypes 
// *****************************************************************************
void APP_SysTimerCallback10ms ( uintptr_t context);
void APP_ProcessSwitchPress(void);


#endif /* _APP_H */
/*******************************************************************************
 End of File
 */


