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

APP_DATA USB_ALIGN appData;

/* This is the string that will written to the file */
 USB_ALIGN uint8_t prompt[8]  = "\r\nLED : ";

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler 
(
    USB_HOST_EVENT event, 
    void * eventData,
    uintptr_t context
)
{
    /* This function is called by the USB Host whenever a USB Host Layer event
     * has occurred. In this example we only handle the device unsupported event
     * */

    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            
            /* The attached device is not supported for some reason */
            break;
            
        default:
            break;
                    
    }
    
    return(USB_HOST_EVENT_RESPONSE_NONE);
}

void APP_USBHostCDCAttachEventListener(USB_HOST_CDC_OBJ cdcObj, uintptr_t context)
{
    /* This function gets called when the CDC device is attached. Update the
     * application data structure to let the application know that this device
     * is attached */
    
    appData.deviceIsAttached = true;
    appData.cdcObj = cdcObj;
}


USB_HOST_CDC_EVENT_RESPONSE APP_USBHostCDCEventHandler
(
    USB_HOST_CDC_HANDLE cdcHandle,
    USB_HOST_CDC_EVENT event,
    void * eventData,
    uintptr_t context
)
{
    /* This function is called when a CDC Host event has occurred. A pointer to
     * this function is registered after opening the device. See the call to
     * USB_HOST_CDC_EventHandlerSet() function. */

    USB_HOST_CDC_EVENT_ACM_SET_LINE_CODING_COMPLETE_DATA * setLineCodingEventData;
    USB_HOST_CDC_EVENT_ACM_SET_CONTROL_LINE_STATE_COMPLETE_DATA * setControlLineStateEventData;
    USB_HOST_CDC_EVENT_WRITE_COMPLETE_DATA * writeCompleteEventData;
    USB_HOST_CDC_EVENT_READ_COMPLETE_DATA * readCompleteEventData;
    
    switch(event)
    {
        case USB_HOST_CDC_EVENT_ACM_SET_LINE_CODING_COMPLETE:
            
            /* This means the application requested Set Line Coding request is
             * complete. */
            setLineCodingEventData = (USB_HOST_CDC_EVENT_ACM_SET_LINE_CODING_COMPLETE_DATA *)(eventData);
            appData.controlRequestDone = true;
            appData.controlRequestResult = setLineCodingEventData->result;
            break;
            
        case USB_HOST_CDC_EVENT_ACM_SET_CONTROL_LINE_STATE_COMPLETE:
            
            /* This means the application requested Set Control Line State 
             * request has completed. */
            setControlLineStateEventData = (USB_HOST_CDC_EVENT_ACM_SET_CONTROL_LINE_STATE_COMPLETE_DATA *)(eventData);
            appData.controlRequestDone = true;
            appData.controlRequestResult = setControlLineStateEventData->result;
            break;
            
        case USB_HOST_CDC_EVENT_WRITE_COMPLETE:
            
            /* This means an application requested write has completed */
            appData.writeTransferDone = true;
            writeCompleteEventData = (USB_HOST_CDC_EVENT_WRITE_COMPLETE_DATA *)(eventData);
            appData.writeTransferResult = writeCompleteEventData->result;
            break;
            
        case USB_HOST_CDC_EVENT_READ_COMPLETE:
            
            /* This means an application requested write has completed */
            appData.readTransferDone = true;
            readCompleteEventData = (USB_HOST_CDC_EVENT_READ_COMPLETE_DATA *)(eventData);
            appData.readTransferResult = readCompleteEventData->result;
            break;
            
        case USB_HOST_CDC_EVENT_DEVICE_DETACHED:
            
            /* The device was detached */
            appData.deviceWasDetached = true;
            break;
            
        default:
            break;
    }
    
    return(USB_HOST_CDC_EVENT_RESPONE_NONE);
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
         /* Initialize the application state machine */
    
    appData.state =  APP_STATE_BUS_ENABLE;
    appData.cdcHostLineCoding.dwDTERate     = APP_HOST_CDC_BAUDRATE_SUPPORTED;
    appData.cdcHostLineCoding.bDataBits     = (uint8_t)APP_HOST_CDC_NO_OF_DATA_BITS;
    appData.cdcHostLineCoding.bParityType   = (uint8_t)APP_HOST_CDC_PARITY_TYPE;
    appData.cdcHostLineCoding.bCharFormat   = (uint8_t)APP_HOST_CDC_STOP_BITS;
    appData.controlLineState.dtr = 0;
    appData.controlLineState.carrier = 0;
    appData.deviceIsAttached = false;
    appData.deviceWasDetached = false;
    appData.readTransferDone = false;
    appData.writeTransferDone = false;
    appData.controlRequestDone = false;
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
   USB_HOST_CDC_RESULT result;
   
   if(appData.deviceWasDetached)
   {
       /* This means the device is not attached. Reset the application state */
       
       appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
       appData.readTransferDone = false;
       appData.writeTransferDone = false;
       appData.controlRequestDone = false;
       appData.deviceWasDetached = false;
   }
   
    switch (appData.state)
    {
        case APP_STATE_BUS_ENABLE:
        
            /* In this state the application enables the USB Host Bus. Note
             * how the CDC Attach event handler are registered before the bus
             * is enabled. */
            
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, (uintptr_t)0);
            USB_HOST_CDC_AttachEventHandlerSet(APP_USBHostCDCAttachEventListener, (uintptr_t) 0);
            USB_HOST_BusEnable(0);
            appData.state = APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
            break;
        
        case APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
            
            /* In this state we wait for the Bus enable to complete */
            if(USB_HOST_BusIsEnabled(0))
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;
            
        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:
            
            /* In this state the application is waiting for the device to be
             * attached */
            if(appData.deviceIsAttached)
            {
                /* A device is attached. We can open this device */
                appData.state = APP_STATE_OPEN_DEVICE;
                appData.deviceIsAttached = false;
            }
            break;
            
        case APP_STATE_OPEN_DEVICE:
            
            /* In this state the application opens the attached device */
            appData.cdcHostHandle = USB_HOST_CDC_Open(appData.cdcObj);
            if(appData.cdcHostHandle != USB_HOST_CDC_HANDLE_INVALID)
            {
                /* The driver was opened successfully. Set the event handler
                 * and then go to the next state. */
                USB_HOST_CDC_EventHandlerSet(appData.cdcHostHandle, APP_USBHostCDCEventHandler, (uintptr_t)0);
                appData.state = APP_STATE_SET_LINE_CODING;
            }
            break;
            
        case APP_STATE_SET_LINE_CODING:
            
            /* Here we set the Line coding. The control request done flag will
             * be set to true when the control request has completed. */
            
            appData.controlRequestDone = false;
            result = USB_HOST_CDC_ACM_LineCodingSet(appData.cdcHostHandle, NULL, &appData.cdcHostLineCoding);
            
            if(result == USB_HOST_CDC_RESULT_SUCCESS)
            {
                /* We wait for the set line coding to complete */
                appData.state = APP_STATE_WAIT_FOR_SET_LINE_CODING;
            }
                            
            break;
            
        case APP_STATE_WAIT_FOR_SET_LINE_CODING:
            
            if(appData.controlRequestDone)
            {
                if(appData.controlRequestResult != USB_HOST_CDC_RESULT_SUCCESS)
                {
                    /* The control request was not successful. */
                    appData.state = APP_STATE_ERROR;
                }
                else
                {
                    /* Next we set the Control Line State */
                    appData.state = APP_STATE_SEND_SET_CONTROL_LINE_STATE;
                }
            }
            break;
            
        case APP_STATE_SEND_SET_CONTROL_LINE_STATE:
            
            /* Here we set the control line state */
            appData.controlRequestDone = false;
            result = USB_HOST_CDC_ACM_ControlLineStateSet(appData.cdcHostHandle, NULL, 
                    &appData.controlLineState);
            
            if(result == USB_HOST_CDC_RESULT_SUCCESS)
            {
                /* We wait for the set line coding to complete */
                appData.state = APP_STATE_WAIT_FOR_SET_CONTROL_LINE_STATE;
            }
            
            break;
            
        case APP_STATE_WAIT_FOR_SET_CONTROL_LINE_STATE:
            
            /* Here we wait for the control line state set request to complete */
            if(appData.controlRequestDone)
            {
                if(appData.controlRequestResult != USB_HOST_CDC_RESULT_SUCCESS)
                {
                    /* The control request was not successful. */
                    appData.state = APP_STATE_ERROR;
                }
                else
                {
                    /* Next we set the Control Line State */
                    appData.state = APP_STATE_SEND_PROMPT_TO_DEVICE;
                }
            }
            
            break;
            
        case APP_STATE_SEND_PROMPT_TO_DEVICE:
            
            /* The prompt is sent to the device here. The write transfer done
             * flag is updated in the event handler. */
            
            appData.writeTransferDone = false;
            result = USB_HOST_CDC_Write(appData.cdcHostHandle, NULL, ( void * )prompt, 8);
            
            if(result == USB_HOST_CDC_RESULT_SUCCESS)
            {
                appData.state = APP_STATE_WAIT_FOR_PROMPT_SEND_COMPLETE;
            }
            break;
            
        case APP_STATE_WAIT_FOR_PROMPT_SEND_COMPLETE:
            
            /* Here we check if the write transfer is done */
            if(appData.writeTransferDone)
            {
                if(appData.writeTransferResult == USB_HOST_CDC_RESULT_SUCCESS)
                {
                    /* Now to get data from the device */
                    appData.state = APP_STATE_GET_DATA_FROM_DEVICE;
                }
                else
                {
                    /* Try sending the prompt again. */
                    appData.state = APP_STATE_SEND_PROMPT_TO_DEVICE;
                }
            }
            
            break;
            
        case APP_STATE_GET_DATA_FROM_DEVICE:
            
            /* Here we request data from the device */
            appData.readTransferDone = false;
            result = USB_HOST_CDC_Read(appData.cdcHostHandle, NULL, appData.inDataArray, 8);
            if(result == USB_HOST_CDC_RESULT_SUCCESS)
            {
                appData.state = APP_STATE_WAIT_FOR_DATA_FROM_DEVICE;
            }
            break;
           
        case APP_STATE_WAIT_FOR_DATA_FROM_DEVICE:
            
            /* Wait for data from device. If the data has arrived, then toggle
             * the LED. */
            if(appData.readTransferDone)
            {
                if(appData.readTransferResult == USB_HOST_CDC_RESULT_SUCCESS)
                {
                   if ( appData.inDataArray[0] == '1')
                    {
                        /* Switch on LED  */
                        LED1_On();
                    }
                   else
                   {
                        /* Switch off LED  */
                        LED1_Off();
                   }
                    
                    /* Send the prompt to the device and wait
                     * for data again */
                    appData.state = APP_STATE_SEND_PROMPT_TO_DEVICE;
                }
            }
            
        case APP_STATE_ERROR:
            /* An error has occurred */
            break;
            
        default:
            break;
    }
}
 

/*******************************************************************************
 End of File
 */