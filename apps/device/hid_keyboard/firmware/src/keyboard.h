/*******************************************************************************
  USB HID Function Driver

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_hid_keyboard.h

  Summary:
    USB HID KEYBOARD Function Driver

  Description:

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>
//#include "configuration.h"
#include "system/system.h"
//#include "definitions.h"
#include "usb/usb_common.h"
#include "usb/usb_chapter_9.h"
#include "usb/usb_device.h"
#include "usb/usb_device_hid.h"


// *****************************************************************************
// *****************************************************************************
// Section: USB Device HID Keyboard types and definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Keyboard Modifier Key State.

  Summary:
    Keyboard Modifier Key State.

  Description:
    This enumeration defines the possible state of the modifier keys.

  Remarks:
    None.
*/

typedef enum
{
    /* ModKey is not pressed */
    KEYBOARD_MODIFIER_KEY_STATE_RELEASED,

	/* Button is pressed */
    KEYBOARD_MODIFIER_KEY_STATE_PRESSED

}KEYBOARD_MODIFIER_KEY_STATE;

// *****************************************************************************
/* Keyboard Key Code Array.

  Summary:
    Keyboard Key Code Array.

  Description:
    This is the definition of the key code array.

  Remarks:
    None.
*/

typedef struct
{
    /* Key codes */
    USB_HID_KEYBOARD_KEYPAD keyCode[6];

}KEYBOARD_KEYCODE_ARRAY;


// *****************************************************************************
/* Keyboard Modifier Keys

  Summary:
    Keyboard Modifier Keys.

  Description:
    This is the HID Keyboard Modifier keys data type.

  Remarks:
    None.
*/
typedef struct
{
    union
    {
        struct
        {
            unsigned int leftCtrl   : 1;
            unsigned int leftShift  : 1;
            unsigned int leftAlt    : 1;
            unsigned int leftGui    : 1;
            unsigned int rightCtrl  : 1;
            unsigned int rightShift : 1;
            unsigned int rightAlt   : 1;
            unsigned int rightGui   : 1;
        };

        int8_t modifierkeys;
    };

} KEYBOARD_MODIFIER_KEYS;

// *****************************************************************************
/* Keyboard Input Report

  Summary:
    Keyboard Input Report.

  Description:
    This is the HID Keyboard Input Report.

  Remarks:
    None.
*/

typedef struct
{
    uint8_t data[8];

}KEYBOARD_INPUT_REPORT;

// *****************************************************************************
/* Keyboard Output Report

  Summary:
    Keyboard Output Report.

  Description:
    This is the HID Keyboard Output Report. This reports is received by the
    keyboard from the host. The application can use this data type while
    placing a request for the keyboard output report by using the
    USB_DEVICE_HID_ReportReceive() function.

  Remarks:
    None.
*/

typedef union
{
    struct
    {
        unsigned int numLock      :1;
        unsigned int capsLock     :1;
        unsigned int scrollLock   :1;
        unsigned int compose      :1;
        unsigned int kana         :1;
        unsigned int constant     :3;

    }ledState;

    uint8_t data[64];

}KEYBOARD_OUTPUT_REPORT;

// *****************************************************************************
/* Keyboard LED State

  Summary:
    Keyboard LED State.

  Description:
    This is the HID Keyboard LED state.

  Remarks:
    None.
*/

typedef enum
{
    /* This is the LED OFF state */
    KEYBOARD_LED_STATE_OFF = 0,

    /* This is the LED ON state */
    KEYBOARD_LED_STATE_ON = 1

}KEYBOARD_LED_STATE;


// *****************************************************************************
// *****************************************************************************
// Section: USB Device HID Keyboard functions
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
/* Function:
    void KEYBOARD_InputReportCreate
    (
        KEYBOARD_KEYCODE_ARRAY * keyboardKeycodeArray,
        KEYBOARD_MODIFIER_KEYS * keyboardModifierKeys
        KEYBOARD_INPUT_REPORT * keyboardInputReport
    )

  Summary:
    Keyboard Input Report Create function.

  Description:
    This function can be used by the application to create a USB HID Keyboard
    Input Report. The application can then send the keyboard input report to the
    host by using the USB_DEVICE_HID_ReportSend() function.

  Precondition:
    None.

  Parameters:
    keyboardKeycodeArray	- Keycode array containing the keycode to be sent.
	keyboardModifierKeys    - Keyboard Modifier Keys.
    keyboardInputReport     - Output only variable that will contain the
                              keyboard input report.

  Returns:
    None.

  Example:
    <code>
    // This code example shows how the an keyboard input report can be sent to
    // the host and a keyboard output report can be received from the host.
    // Assume that the device is configured and HID instance is 0.

    KEYBOARD_KEYCODE_ARRAY  keyboardKeycodeArray;
    KEYBOARD_MODIFIER_KEYS  modifierkeys;
    KEYBOARD_INPUT_REPORT   keyboardInputReport;
    KEYBOARD_OUTPUT_REPORT  keyboardOutputReport;
    USB_DEVICE_HID_TRANSFER_HANDLE  transferHandle;
    USB_DEVICE_HID_RESULT result;

    // Indicate that the Left Alt Key is pressed

    modifierkeys.leftAlt = USB_HID_KEYBOARD_MODIFIER_KEY_STATE_PRESSED;

    // Indicate the 'A' key was pressed

    keyboardKeycodeArray.keyCode[0] = 4;

    // Now form the keyboard input report

    KEYBOARD_InputReportCreate(&keyboardKeycodeArray,
                &keyboardModifierKeys, &keyboardInputReport);

    result = USB_DEVICE_HID_ReportSend(0, &transferHandle, &keyboardInputReport,
                 sizeof(USB_HID_KEYBOARD_INPUT_REPORT));

    if(USB_DEVICE_HID_RESULT_OK != result)
    {
        // Do error handling here
    }

    // The completion of the report send is indicated by the
    // USB_DEVICE_HID_EVENT_REPORT_SEND_COMPLETE event.

    // To obtain an output report from the host, the following
    // can be done.

    USB_DEVICE_HID_ReportReceive(0, &transferHandle, &keyboardOutputReport,
                    sizeof(USB_HID_KEYBOARD_OUTPUT_REPORT));

    // When output report is received when the
    // USB_DEVICE_HID_EVENT_REPORT_RECEIVE_COMPLETE event occurs.
    // Assuming this event has occured, the following can be done.

    // Check if the NUM LOCK LED should be switched on.

    if(keyboardOutputReport.ledState.numLock == USB_HID_KEYBOARD_LED_STATE_ON)
    {
        // Switch on the num lock LED
    }

    </code>

  Remarks:
    None.
*/

void KEYBOARD_InputReportCreate
(
    KEYBOARD_KEYCODE_ARRAY * keyboardKeycodeArray,
    KEYBOARD_MODIFIER_KEYS * keyboardModifierKeys,
    KEYBOARD_INPUT_REPORT * keyboardInputReport
);

#endif
