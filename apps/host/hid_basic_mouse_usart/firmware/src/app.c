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
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

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

void APP_USBHostHIDMouseEventHandler(USB_HOST_HID_MOUSE_HANDLE handle, 
        USB_HOST_HID_MOUSE_EVENT event, void * pData)
{
    uint8_t loop = 0;
    
    switch ( event)
    {
        case USB_HOST_HID_MOUSE_EVENT_ATTACH:
            appData.handle = handle;
            appData.nBytesWritten = 0;
            appData.stringReady = false;
            memset(&appData.string, 0, sizeof(appData.string));
            appData.stringSize = 0;
            appData.isMouseAttached = true;
			LED1_On();
            break;

        case USB_HOST_HID_MOUSE_EVENT_DETACH:
            appData.handle = handle;
            appData.nBytesWritten = 0;
            appData.stringReady = false;
            memset(&appData.string, 0, sizeof(appData.string));
            appData.stringSize = 0;
            appData.isMouseAttached = false;
			LED1_Off();
            break;

        case USB_HOST_HID_MOUSE_EVENT_REPORT_RECEIVED:
            appData.handle = handle;
            /* Mouse Data from device */
            memcpy(&appData.data, pData, sizeof(appData.data));

            for(loop = 0; loop < USB_HOST_HID_MOUSE_BUTTONS_NUMBER; loop ++)
            {
                if((appData.data.buttonState[loop]) &&
                        (USB_HID_USAGE_ID_BUTTON1 == appData.data.buttonID[loop]))
                {
                    /* BUTTON1 pressed */
                   LED1_On();
                }
                if((appData.data.buttonState[loop]) &&
                        (USB_HID_USAGE_ID_BUTTON2 == appData.data.buttonID[loop]))
                {LED1_Off();
                    /* BUTTON2 pressed */
                   
                }
            }
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
                   
            if(appData.stringReady)
            {
                /* Write the string to the driver. We will need to check how much
                 * of the string was actually written and update the pointer and
                 * size accordingly */
                
                DRV_USART_WriteBufferAdd(appData.usartDriverHandle, 
                    appData.string , sizeof( appData.string ), &appData.WriteBufHandler );
                
                appData.stringReady = false;
                appData.usartTaskState = APP_USART_STATE_CHECK_FOR_STRING_TO_SEND;
            }
            else
            {
                /* Code flow can come here when there is a device detach while
                 USART write in progress */
                appData.usartTaskState = APP_USART_STATE_CHECK_FOR_STRING_TO_SEND;
            }
            
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

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
            
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_HID_MOUSE_EventHandlerSet(APP_USBHostHIDMouseEventHandler);
            
			USB_HOST_BusEnable(0);
            
            memcpy(&appData.string[0], "***Connect Mouse***\r\n",
                        sizeof("***Connect Mouse***\r\n"));
            appData.stringReady = true;
            appData.stringSize = 64;
                
			appData.state = APP_STATE_WAIT_FOR_HOST_ENABLE;
        
            break;
        }

        /* TODO: implement your application state machine.*/
        
        case APP_STATE_WAIT_FOR_HOST_ENABLE:
        {
            /* Check if the host operation has been enabled */
            if(USB_HOST_BusIsEnabled(0))
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;
        }
        
        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:
        {
            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            if(appData.isMouseAttached)
            {
                appData.state = APP_STATE_DEVICE_ATTACHED;
            }
        }

        break;
        

        case APP_STATE_DEVICE_ATTACHED:
        {
            if(appData.usartTaskState == APP_USART_STATE_CHECK_FOR_STRING_TO_SEND)
            {
                memcpy(&appData.string[0], "---Mouse Connected---\r\n",
                        sizeof("---Mouse Connected---\r\n"));
                appData.stringReady = true;
                appData.stringSize = 64;
                appData.state = APP_STATE_READ_HID;
            }
        }
        break;
        
        case APP_STATE_READ_HID:
        {
            if(appData.isMouseAttached)
            {
                if((appData.data.xMovement != 0) || (appData.data.yMovement != 0)
                        || (appData.data.zMovement != 0))
                {
                    if(appData.usartTaskState == APP_USART_STATE_CHECK_FOR_STRING_TO_SEND)
                    {
                        sprintf((char *)&appData.string[0], "X = %4d  |  Y = %4d  |  Z = %4d\r\n",
                                appData.data.xMovement,appData.data.yMovement,appData.data.zMovement);
                        appData.stringReady = true;
                        appData.stringSize = 64;
                        /* Clear the app data */
                        memset(&appData.data, 0, (size_t)sizeof(appData.data));
                    }
                }
            }
            else
            {
                /* The attached device has been detached */
                appData.state = APP_STATE_DEVICE_DETACHED;
                /* Clear the app data */
                memset(&appData.data, 0, (size_t)sizeof(appData.data));
            }
        }
        break;
        
        case APP_STATE_DEVICE_DETACHED:
        {
            LED1_Off();
            memcpy(&appData.string[0], "***Connect Mouse***\r\n",
                        sizeof("***Connect Mouse***\r\n"));
            appData.stringReady = true;
            appData.stringSize = 64;
                
            appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            break;
        }
        
        case APP_STATE_ERROR:
        {
            break;
        }
        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
    /* Call the USART task routine */
    APP_USART_Tasks();
}

 

/*******************************************************************************
 End of File
 */
