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
#include "string.h"
#include "definitions.h"
#include "configuration.h"
#include "keyboard.h"

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT=0,

	/* Application waits for configuration in this state */
    APP_STATE_WAIT_FOR_CONFIGURATION,

    /* Application checks if an output report is available */
    APP_STATE_CHECK_FOR_OUTPUT_REPORT,

    /* Application updates the switch states */
    APP_STATE_SWITCH_PROCESS,

    /* Application checks if it is still configured*/
    APP_STATE_CHECK_IF_CONFIGURED,

    /* Application emulates keyboard */
    APP_STATE_EMULATE_KEYBOARD,

    /* Application error state */
    APP_STATE_ERROR

} APP_STATES;


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

    /* Handle to the device layer */
    USB_DEVICE_HANDLE deviceHandle;

    /* Application HID instance */
    USB_DEVICE_HID_INDEX hidInstance;

    /* Keyboard modifier keys*/
    KEYBOARD_MODIFIER_KEYS keyboardModifierKeys;

    /* Key code array*/
    KEYBOARD_KEYCODE_ARRAY keyCodeArray;

    /* Is device configured */
    bool isConfigured;

    /* Switch state*/
    bool ignoreSwitchPress;

    /* Tracks switch press*/
    bool isSwitchPressed;

    /* Track the send report status */
    bool isReportSentComplete;

    /* Track if a report was received */
    bool isReportReceived;

    /* USB HID current Idle */
    uint8_t idleRate;

    /* Flag determines SOF event occurrence */
    bool sofEventHasOccurred;

    /* Receive transfer handle */
    USB_DEVICE_HID_TRANSFER_HANDLE receiveTransferHandle;

    /* Send transfer handle */
    USB_DEVICE_HID_TRANSFER_HANDLE sendTransferHandle;

    /* Key code to be sent */
    USB_HID_KEYBOARD_KEYPAD key;

    /* USB HID active Protocol */
    USB_HID_PROTOCOL_CODE activeProtocol;

    /* Switch de-bounce timer */
    unsigned int switchDebounceTimer;
    
    /* Tracks De-bounce timer */
    unsigned int debounceCount;

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

void APP_Tasks( void );



#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

