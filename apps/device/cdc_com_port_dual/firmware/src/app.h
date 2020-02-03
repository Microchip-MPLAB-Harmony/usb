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

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
/*** Application Instance 0 Configuration ***/

/* Macro defines Read Buffer Size used in the Application*/
#if DRV_USBFSV1_DEVICE_SUPPORT == true
#define APP_READ_BUFFER_SIZE                                64
#else
#define APP_READ_BUFFER_SIZE                                512
#endif 

/* Macro defines USB internal DMA Buffer criteria*/
#define APP_MAKE_BUFFER_DMA_READY                           __attribute__((aligned(16)))    

// *****************************************************************************
// *****************************************************************************
// Section: Free RTOS Task Priorities
// *****************************************************************************
// *****************************************************************************

#define COM1 USB_DEVICE_CDC_INDEX_0
// *****************************************************************************
// *****************************************************************************
// Section: Free RTOS Task Stack Sizes
// *****************************************************************************
// *****************************************************************************
#define COM2 USB_DEVICE_CDC_INDEX_1

// *****************************************************************************
// *****************************************************************************
// Section: Configuration specific application constants
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Application States

  Summary:
    Application states 

  Description:
    This defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    APP_STATE_INIT=0,

    /* Application waits for device configuration*/
    APP_STATE_WAIT_FOR_CONFIGURATION,

    /* Application checks if the device is still configured*/
    APP_STATE_CHECK_IF_CONFIGURED,

   /* A character is received from host */
    APP_STATE_CHECK_FOR_READ_COMPLETE,

    /* Wait for the transmit to get completed */
    APP_STATE_CHECK_FOR_WRITE_COMPLETE,

    /* Wait for the write to complete */
    APP_STATE_WAIT_FOR_WRITE_COMPLETE,

    /* Application Error state*/
    APP_STATE_ERROR

} APP_STATES;

/******************************************************
 * Application COM Port Object
 ******************************************************/

typedef struct
{
    /* CDC instance number */
    USB_DEVICE_CDC_INDEX cdcInstance;

    /* Set Line Coding Data */
    USB_CDC_LINE_CODING setLineCodingData;

    /* Get Line Coding Data */
    USB_CDC_LINE_CODING getLineCodingData;

    /* Control Line State */
    USB_CDC_CONTROL_LINE_STATE controlLineStateData;

    /* Break data */
    uint16_t breakData;

    /* Read transfer handle */
    USB_DEVICE_CDC_TRANSFER_HANDLE readTransferHandle;

    /* Write transfer handle */
    USB_DEVICE_CDC_TRANSFER_HANDLE writeTransferHandle;

    /* True if a character was read */
    bool isReadComplete;

    /* True if a character was written*/
    bool isWriteComplete;
    
    /* This variable saves number of bytes of data received from the Host. 
     * Application uses this variable to send back same amount of data to Host.*/
    uint32_t readDataLength;

}APP_COM_PORT_OBJECT;

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
    /* Device layer handle returned by device layer open function */
    USB_DEVICE_HANDLE deviceHandle;

    /* Application's current state*/
    APP_STATES state;

    /* Device configured state */
    bool isConfigured;
    APP_COM_PORT_OBJECT appCOMPortObjects[2];
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
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

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


#endif /* _APP_H */
/*******************************************************************************
 End of File
 */


