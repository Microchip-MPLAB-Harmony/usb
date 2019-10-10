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
    APP_STATE_RUN_HID_CDC_TASKS,

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


typedef enum
{
    /* Application pixels put*/
    APP_HID_STATE_INIT=0,

    /* Application bar fill */
    APP_HID_STATE_GFX_INIT,

    /* Application pixel array put*/
    APP_HID_STATE_PIXELARRAYPUT,

    /* Application pixel array get*/
    APP_HID_STATE_PIXELARRAYGET,

    /* Application pixel array get*/
    APP_HID_STATE_PIXELPUT,

     /* Application color set*/
    APP_HID_STATE_COLORSET,

     /* Application instance set*/
    APP_HID_STATE_INSTANCESET,

     /* Application page set*/
    APP_HID_STATE_PAGESET,

     /* Application page set*/
    APP_HID_STATE_LAYER,

     /* Application pixel get*/
    APP_HID_STATE_PIXELGET,

     /* Application alphablend window*/
    APP_HID_STATE_ALPHABLENDWINDOW,

    /* Application done */
    APP_HID_STATE_GFX_DONE,

    APP_STATE_OPEN_HOST_LAYER,
    APP_STATE_WAIT_FOR_HOST_ENABLE,
    APP_HID_STATE_WAIT_FOR_DEVICE_ATTACH,
    APP_HID_STATE_DEVICE_ATTACHED,
    APP_HID_STATE_READ_HID,
    APP_HID_STATE_DEVICE_DETACHED,
    APP_HID_STATE_CHANGE_DEVICE_PARAMETERS,
    APP_HID_STATE_ERROR

} APP_HID_STATES;


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


typedef struct
{
    /* Application's current state*/
    APP_HID_STATES state;
    
    USB_HOST_HID_MOUSE_HANDLE handle;

    /* Application data buffer */
    USB_HOST_HID_MOUSE_DATA data;

} APP_HID_DATA;

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

void APP_USBHostHIDMouseEventHandler(USB_HOST_HID_MOUSE_HANDLE handle, 
        USB_HOST_HID_MOUSE_EVENT event, void * pData);
	
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

void APP_Tasks( void );

void APP_HID_Tasks (  );
void APP_CDC_Tasks ( );

void APP_HID_Initialize ();
void APP_CDC_Initialize ( APP_CDC_DATA * appCDCData);



#endif /* _APP_H */
/*******************************************************************************
 End of File
 */

