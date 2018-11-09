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
/*******************************************************
 * Application definitions.
 *******************************************************/

#define APP_HOST_CDC_BAUDRATE_SUPPORTED 9600UL
#define APP_HOST_CDC_PARITY_TYPE        0
#define APP_HOST_CDC_STOP_BITS          0
#define APP_HOST_CDC_NO_OF_DATA_BITS    8


// *****************************************************************************
/* Application States

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
   /* Application opens the host layer */
    APP_STATE_BUS_ENABLE,

    /* Application waits for the host to get enabled */
    APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE,

    /* Application runs the CDC and MSD Application tasks */
    APP_STATE_RUN_MSD_CDC_TASKS,

    /* Application is in error state */
    APP_STATE_ERROR

} APP_STATES;


// *****************************************************************************
/* Application States

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
   

    /* Application waits for CDC Device Attach */
    APP_CDC_STATE_WAIT_FOR_DEVICE_ATTACH,

    /* CDC Device is Attached */
    APP_CDC_STATE_OPEN_DEVICE,
            
    /* Set the Line Coding */
    APP_CDC_STATE_SET_LINE_CODING,

    /* Application waits to get the device line coding */
    APP_CDC_STATE_WAIT_FOR_GET_LINE_CODING,

    /* Application sets the line coding */
    APP_CDC_STATE_SEND_SET_LINE_CODING,

    /* Appliction waits till set line coding is done */
    APP_CDC_STATE_WAIT_FOR_SET_LINE_CODING,

    /* Application sets the contol line state */
    APP_CDC_STATE_SEND_SET_CONTROL_LINE_STATE,

    /* Application waits for the set control line state to complete */
    APP_CDC_STATE_WAIT_FOR_SET_CONTROL_LINE_STATE,

    /* Application sends the prompt to the device */
    APP_CDC_STATE_SEND_PROMPT_TO_DEVICE,

    /* Application waits for prompt send complete */
    APP_CDC_STATE_WAIT_FOR_PROMPT_SEND_COMPLETE,

    /* Application request to get data from device */
    APP_CDC_STATE_GET_DATA_FROM_DEVICE,

    /* Application waits for data from device */
    APP_CDC_STATE_WAIT_FOR_DATA_FROM_DEVICE,

    /* Application has received data from the device */
    APP_CDC_STATE_DATA_RECEIVED_FROM_DEVICE,

    /* Application is in error state */
    APP_CDC_STATE_ERROR
            
} APP_CDC_STATES;

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
    /* The application's current state */
    APP_CDC_STATES state;

    /* Array to hold read data */
    uint8_t inDataArray[64];
    
    /* CDC Object */
    USB_HOST_CDC_OBJ cdcObj;
    
    /* True if a device is attached */
    bool deviceIsAttached;
    
    /* True if control request is done */
    bool controlRequestDone;
    
    /* Control Request Result */
    USB_HOST_CDC_RESULT controlRequestResult;

    /* A CDC Line Coding object */
    USB_CDC_LINE_CODING cdcHostLineCoding;
    
    /* A Control Line State object*/
    USB_CDC_CONTROL_LINE_STATE controlLineState;
    
    /* Handle to the CDC device. */
    USB_HOST_CDC_HANDLE cdcHostHandle;
    
    /* True when a write transfer has complete */
    bool writeTransferDone;
    
    /* Write Transfer Result */
    USB_HOST_CDC_RESULT writeTransferResult;
    
     /* True when a read transfer has complete */
    bool readTransferDone;
    
    /* Read Transfer Result */
    USB_HOST_CDC_RESULT readTransferResult;
    
    /* True if device was detached */
    bool deviceWasDetached;
    
} APP_CDC_DATA;



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
    /* The application's current state */
    APP_STATES state;

   

} APP_DATA;



// *****************************************************************************
/* Application States

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	
    APP_MSD_STATE_WAIT_FOR_DEVICE_ATTACH,
    APP_MSD_STATE_DEVICE_CONNECTED,
    APP_MSD_STATE_MOUNT_DISK,
    APP_MSD_STATE_UNMOUNT_DISK,
    APP_MSD_STATE_OPEN_FILE,
    APP_MSD_STATE_WRITE_TO_FILE,
    APP_MSD_STATE_CLOSE_FILE,
    APP_MSD_STATE_IDLE,
    APP_MSD_STATE_ERROR
} APP_MSD_STATES;

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
 /* The application's current state */
  
    /* SYS_FS File handle for 1st file */
    SYS_FS_HANDLE fileHandle;

    /* Application's current state */
    APP_MSD_STATES state;

    /* Application data buffer */
    uint8_t data[1024];

    /* Number of bytes written */
    uint32_t nBytesWritten;

    /* Number of bytes read */
    uint32_t nBytesRead;

    bool deviceIsConnected;
    
} APP_MSD_DATA;

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


void APP_MSD_Tasks (  );
void APP_CDC_Tasks ( );

void APP_MSD_Initialize (APP_MSD_DATA * appMSDData);
void APP_CDC_Initialize ( APP_CDC_DATA * appCDCData);


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler 
(
    USB_HOST_EVENT event, 
    void * eventData,
    uintptr_t context
);

void APP_USBHostCDCAttachEventListener
(
    USB_HOST_CDC_OBJ cdcObj, 
    uintptr_t context
);

USB_HOST_CDC_EVENT_RESPONSE APP_USBHostCDCEventHandler
(
    USB_HOST_CDC_HANDLE cdcHandle,
    USB_HOST_CDC_EVENT event,
    void * eventData,
    uintptr_t context
);
	



#endif /* _APP_H */
/*******************************************************************************
 End of File
 */

