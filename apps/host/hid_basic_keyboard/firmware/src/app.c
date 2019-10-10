/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Usage ID to Key map table */
const char *keyValue[] = { 
                          "No event indicated",
                          "ErrorRoll Over",
                          "POSTFail",
                          "Error Undefined",
                          "a",
                          "b",
                          "c",
                          "d",
                          "e",
                          "f",
                          "g",
                          "h",
                          "i",
                          "j",
                          "k",
                          "l",
                          "m",
                          "n",
                          "o",
                          "p",
                          "q",
                          "r",
                          "s",
                          "t",
                          "u",
                          "v",
                          "w",
                          "x",
                          "y",
                          "z",
                          "1",
                          "2",
                          "3",
                          "4",
                          "5",
                          "6",
                          "7",
                          "8",
                          "9",
                          "0",
                          "ENTER",
                          "ESCAPE",
                          "BACKSPACE",
                          "TAB",
                          "SPACEBAR",
                          "-",
                          "=",
                          "[",
                          "]",
                          "\\0",
                          "~",
                          ";",
                          "'",
                          "GRAVE ACCENT",
                          ",",
                          ".",
                          "/",
                          "CAPS LOCK",
                          "F1",
                          "F2",
                          "F3",
                          "F4",
                          "F5",
                          "F6",
                          "F7",
                          "F8",
                          "F9",
                          "F10",
                          "F11",
                          "F12"
                      };

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Routines
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************
 * USB HOST Layer Events - Host Event Handler
 *******************************************************/

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
    switch(event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        default:
            break;
    }
    return USB_HOST_EVENT_RESPONSE_NONE;
}

/*******************************************************
 * USB HOST HID Layer Events - Application Event Handler
 *******************************************************/

void APP_USBHostHIDKeyboardEventHandler(USB_HOST_HID_KEYBOARD_HANDLE handle, 
        USB_HOST_HID_KEYBOARD_EVENT event, void * pData)
{   
    switch ( event)
    {
        case USB_HOST_HID_KEYBOARD_EVENT_ATTACH:
            appData.handle = handle;
            appData.state =  APP_STATE_DEVICE_ATTACHED;
            appData.nBytesWritten = 0;
            appData.stringReady = false;
            memset(&appData.string, 0, sizeof(appData.string));
            memset(&appData.lastData, 0, sizeof(appData.lastData));
            appData.stringSize = 0;
            appData.capsLockPressed = false;
            appData.scrollLockPressed = false;
            appData.numLockPressed = false;
            appData.outputReport = 0;
            break;

        case USB_HOST_HID_KEYBOARD_EVENT_DETACH:
            appData.handle = handle;
            appData.state = APP_STATE_DEVICE_DETACHED;
            appData.nBytesWritten = 0;
            appData.stringReady = false;
            appData.usartTaskState = APP_USART_STATE_CHECK_FOR_STRING_TO_SEND;
            memset(&appData.string, 0, sizeof(appData.string));
            memset(&appData.lastData, 0, sizeof(appData.lastData));
            appData.stringSize = 0;
            appData.capsLockPressed = false;
            appData.scrollLockPressed = false;
            appData.numLockPressed = false;
            appData.outputReport = 0;
            break;

        case USB_HOST_HID_KEYBOARD_EVENT_REPORT_RECEIVED:
            appData.handle = handle;
            appData.state = APP_STATE_READ_HID;
            /* Keyboard Data from device */
            memcpy(&appData.data, pData, sizeof(appData.data));
            break;

        default:
            break;
    }
    return;
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/

void APP_MapKeyToUsage(USB_HID_KEYBOARD_KEYPAD keyCode)
{
    uint8_t outputReport = 0;
    
    outputReport = appData.outputReport;
    
    if((keyCode >= USB_HID_KEYBOARD_KEYPAD_KEYBOARD_A &&
            keyCode <= USB_HID_KEYBOARD_KEYPAD_KEYBOARD_0_AND_CLOSE_PARENTHESIS) || 
            (keyCode == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_CAPS_LOCK) || 
            (keyCode == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_SCROLL_LOCK) || 
            (keyCode == USB_HID_KEYBOARD_KEYPAD_KEYPAD_NUM_LOCK_AND_CLEAR))
    {
        if(keyCode >= USB_HID_KEYBOARD_KEYPAD_KEYBOARD_A &&
            keyCode <= USB_HID_KEYBOARD_KEYPAD_KEYBOARD_0_AND_CLOSE_PARENTHESIS)
        {
            memcpy(&appData.string[appData.currentOffset], keyValue[keyCode],
                            strlen(keyValue[keyCode]));
        }
        else
        {
            if(keyCode == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_CAPS_LOCK)
            {
                /* CAPS LOCK pressed */
                if(appData.capsLockPressed == false)
                {
                    appData.capsLockPressed = true;
                    outputReport = outputReport | 0x2;
                }
                else
                {
                    appData.capsLockPressed = false;
                    outputReport = outputReport & 0xFD;
                }
            }
            if(keyCode == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_SCROLL_LOCK)
            {
                /* SCROLL LOCK pressed */
                if(appData.scrollLockPressed == false)
                {
                    appData.scrollLockPressed = true;
                    outputReport = outputReport | 0x4;
                }
                else
                {
                    appData.scrollLockPressed = false;
                    outputReport = outputReport & 0xFB;
                }
            }
            if(keyCode == USB_HID_KEYBOARD_KEYPAD_KEYPAD_NUM_LOCK_AND_CLEAR)
            {
                /* NUM LOCK pressed */
                if(appData.numLockPressed == false)
                {
                    appData.numLockPressed = true;
                    outputReport = outputReport | 0x1;
                }
                else
                {
                    appData.numLockPressed = false;
                    outputReport = outputReport & 0xFE;
                }
            }
            
            /* Store the changes */
            appData.outputReport = outputReport;
            /* Send the OUTPUT Report */
            USB_HOST_HID_KEYBOARD_ReportSend(appData.handle, outputReport);
        }
        if(appData.capsLockPressed && (appData.data.modifierKeysData.leftShift ||
                appData.data.modifierKeysData.rightShift))
        {
            /* Small case should be displayed */
        }
        /* Check if it is within a - z */
        else if((appData.capsLockPressed || appData.data.modifierKeysData.leftShift ||
                appData.data.modifierKeysData.rightShift) &&
                (keyCode >= USB_HID_KEYBOARD_KEYPAD_KEYBOARD_A &&
                keyCode <= USB_HID_KEYBOARD_KEYPAD_KEYBOARD_Z))
        {
            appData.string[appData.currentOffset] = appData.string[appData.currentOffset] - 32;
        }        
        appData.currentOffset = appData.currentOffset + sizeof(keyValue[keyCode]);
    }
    
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    memset(&appData, 0, sizeof(appData));
    appData.state = APP_STATE_INIT;
    appData.usartTaskState = APP_USART_STATE_DRIVER_OPEN;
    appData.WriteBufHandler = DRV_USART_BUFFER_HANDLE_INVALID;
}


void APP_USART_Tasks(void)
{
    
    switch(appData.usartTaskState)
    {
        case APP_USART_STATE_DRIVER_OPEN:
            
            /* Try to open the USART driver */
            appData.usartDriverHandle = DRV_USART_Open(DRV_USART_INDEX_0,
                    DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
            if(appData.usartDriverHandle != DRV_HANDLE_INVALID)
            {
                /* The driver could be opened. We should be able to send data */
                appData.usartTaskState = APP_USART_STATE_CHECK_FOR_STRING_TO_SEND;
            }
            break;
            
        case APP_USART_STATE_CHECK_FOR_STRING_TO_SEND:
            
            /* In this state we check if the there is string to be transmitted */
            if(appData.stringReady)
            {
                /* Write the string to the driver */
                appData.usartTaskState = APP_USART_STATE_DRIVER_WRITE;
                appData.nBytesWritten = 0;
            }
            
            break;
            
        case APP_USART_STATE_DRIVER_WRITE:
            
            /* Write the string to the driver. We will need to check how much
             * of the string was actually written and update the pointer and
             * size accordingly */
           DRV_USART_WriteBufferAdd(appData.usartDriverHandle, 
                    appData.string , sizeof( appData.string ), &appData.WriteBufHandler );
            
             appData.stringReady = false;
            appData.usartTaskState = APP_USART_STATE_CHECK_FOR_STRING_TO_SEND;
        break;
            
        default:
            break;
    }
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    uint64_t sysCount = 0;
    uint8_t count = 0;
    uint8_t counter = 0;
    bool foundInLast = false;
    
    appData.currentOffset = 0;
    
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_HID_KEYBOARD_EventHandlerSet(APP_USBHostHIDKeyboardEventHandler);
            
			USB_HOST_BusEnable(0);
			appData.state = APP_STATE_WAIT_FOR_HOST_ENABLE;
            break;
			
		case APP_STATE_WAIT_FOR_HOST_ENABLE:
            /* Check if the host operation has been enabled */
            if(USB_HOST_BusIsEnabled(0))
            {
                /* This means host operation is enabled. We can
                 * move on to the next state */
                appData.state = APP_STATE_HOST_ENABLE_DONE;
            }
            break;
        case APP_STATE_HOST_ENABLE_DONE:
            appData.stringSize = 64;
            if(appData.usartTaskState == APP_USART_STATE_CHECK_FOR_STRING_TO_SEND)
            {
                memcpy(&appData.string[0], "\r\n***Connect Keyboard***\r\n",
                            sizeof("\r\n***Connect Keyboard***\r\n"));
                appData.stringReady = true;
                /* The test was successful. Lets idle. */
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;

        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:

            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */

            break;

        case APP_STATE_DEVICE_ATTACHED:
            
            /* Wait for device report */
            if(appData.usartTaskState == APP_USART_STATE_CHECK_FOR_STRING_TO_SEND)
            {
                memcpy(&appData.string[0], "---Keyboard Connected---\r\n",
                        sizeof("---Keyboard Connected---\r\n"));
                appData.stringReady = true;
                appData.stringSize = 64;
                appData.state = APP_STATE_READ_HID;
            }
            break;

        case APP_STATE_READ_HID:
            
            if(appData.usartTaskState == APP_USART_STATE_CHECK_FOR_STRING_TO_SEND)
            {
                /* We need to display only the non modifier keys */
                for(count = 0; count < 6; count++)
                {
                    if(appData.data.nonModifierKeysData[count].event == USB_HID_KEY_PRESSED)
                    {
                        appData.stringReady = false;
                        /* We can send Data to USART but need to check*/
                        appData.stringSize = 64;
                        memset(&appData.string, 0, sizeof(appData.string));
                        for(counter = 0; counter < 6; counter++)
                        {
                            if((appData.lastData.data.nonModifierKeysData[counter].event == USB_HID_KEY_PRESSED)
                                &&((appData.lastData.data.nonModifierKeysData[counter].keyCode == 
                                    appData.data.nonModifierKeysData[count].keyCode)))
                            {
                                sysCount = SYS_TIME_CounterGet ();
                                if(200 <= 1000 * 
                                        (sysCount - appData.lastData.data.nonModifierKeysData[counter].sysCount)
                                        / SYS_TIME_FrequencyGet())
                                {
                                    foundInLast = false;
                                }
                                else
                                {
                                    foundInLast = true;
                                }
                                break;
                            }
                        }
                        if(foundInLast == false)
                        {
                            appData.stringReady = true;
                            APP_MapKeyToUsage(appData.data.nonModifierKeysData[count].keyCode);
                        }
                        else
                        {
                            /* Reset it it false for next iteration */
                            foundInLast = false;
                        }
                    }
                }
                /* Store the present to future */
                memcpy(&appData.lastData.data, &appData.data, sizeof(appData.data));
            }
            break;

        case APP_STATE_DEVICE_DETACHED:
            appData.state = APP_STATE_HOST_ENABLE_DONE;
            break;

        case APP_STATE_ERROR:

            /* The application comes here when the demo
             * has failed. Provide LED indication .*/

            LED1_On();
            break;

        default:
            break;
    }
    /* Call the USART task routine */
    APP_USART_Tasks();

}


/*******************************************************************************
 End of File
 */
