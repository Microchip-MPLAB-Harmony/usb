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

/* Read Data Buffer */
uint8_t com1ReadBuffer[APP_READ_BUFFER_SIZE] APP_MAKE_BUFFER_DMA_READY;

/* Write Data Buffer. Size is same as read buffer size */
uint8_t com1WriteBuffer[APP_READ_BUFFER_SIZE] APP_MAKE_BUFFER_DMA_READY;

/* Read Data Buffer */
uint8_t com2ReadBuffer[APP_READ_BUFFER_SIZE] APP_MAKE_BUFFER_DMA_READY;

uint8_t com2WriteBuffer[APP_READ_BUFFER_SIZE] APP_MAKE_BUFFER_DMA_READY;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************


/************************************************
 * CDC Function Driver Application Event Handler
 ************************************************/

USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler
(
    USB_DEVICE_CDC_INDEX index ,
    USB_DEVICE_CDC_EVENT event ,
    void* pData,
    uintptr_t userData
)
{

    APP_DATA * appDataObject;
    appDataObject = (APP_DATA *)userData;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;
    uint16_t * breakData;

    switch ( event )
    {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

            USB_DEVICE_ControlSend(appDataObject->deviceHandle,
                    &appDataObject->appCOMPortObjects[index].getLineCodingData,
                    sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */

            USB_DEVICE_ControlReceive(appDataObject->deviceHandle,
                    &appDataObject->appCOMPortObjects[index].setLineCodingData,
                    sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            appDataObject->appCOMPortObjects[index].controlLineStateData.dtr = controlLineStateData->dtr;
            appDataObject->appCOMPortObjects[index].controlLineStateData.carrier = controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            breakData = (uint16_t *)pData;
            appDataObject->appCOMPortObjects[index].breakData = *breakData;
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:

            /* This means that the host has sent some data*/
            appDataObject->appCOMPortObjects[index].isReadComplete = true;

            
            /*let processing USB Task know USB if configured..*/
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. For now we accept all the data */

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We don't
             * do much with this data in this demo. */

            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:

            /* This means that the data write got completed. We can schedule
             * the next read. */

            appDataObject->appCOMPortObjects[index].isWriteComplete = true;

            break;

        default:
            break;
    }
    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
}

/*************************************************
 * Application Device Layer Event Handler
 *************************************************/

void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, void * pData, uintptr_t context)
{
    uint8_t configurationValue;

    switch( event )
    {
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:

            appData.isConfigured = false;

            LED_Off();
            
            break;
            
        case USB_DEVICE_EVENT_CONFIGURED:

            /*do we have access to USB, if not try again*/
            configurationValue = ((USB_DEVICE_EVENT_DATA_CONFIGURED *)pData)->configurationValue;
            if(configurationValue == 1)
            {
                //USBDeviceTask_State = USBDEVICETASK_PROCESSUSBEVENTS_STATE;
                USB_DEVICE_CDC_EventHandlerSet(COM1, APP_USBDeviceCDCEventHandler, (uintptr_t)&appData);
                USB_DEVICE_CDC_EventHandlerSet(COM2, APP_USBDeviceCDCEventHandler, (uintptr_t)&appData);

                appData.isConfigured = true;

                LED_On();
            }
            
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            
            /* Update LED indication */
            LED_Off();
            
            break;

       case USB_DEVICE_EVENT_POWER_DETECTED:
            
            /* VBUS has been detected. We can attach the device */
                        USB_DEVICE_Attach (appData.deviceHandle);
                        break;
            
        case USB_DEVICE_EVENT_POWER_REMOVED:
            
            /* VBUS is not available. We can detach the device */
            USB_DEVICE_Detach(appData.deviceHandle);
            break;

        /* These events are not used in this demo */
        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/************************************************
 * Application State Reset Function
 ************************************************/

void APP_StateReset(void)
{
    appData.appCOMPortObjects[COM1].isReadComplete = false;
    appData.appCOMPortObjects[COM1].isWriteComplete = false;
    appData.appCOMPortObjects[COM2].isReadComplete = false;
    appData.appCOMPortObjects[COM2].isWriteComplete = false;
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
    /* Initialize the application object */

    appData.deviceHandle = USB_DEVICE_HANDLE_INVALID;
    appData.isConfigured = false;
    appData.state = APP_STATE_INIT;

    appData.appCOMPortObjects[COM1].getLineCodingData.dwDTERate = 9600;
    appData.appCOMPortObjects[COM1].getLineCodingData.bDataBits = 8;
    appData.appCOMPortObjects[COM1].getLineCodingData.bParityType = 0;
    appData.appCOMPortObjects[COM1].getLineCodingData.bCharFormat = 0;

    appData.appCOMPortObjects[COM2].getLineCodingData.dwDTERate = 9600;
    appData.appCOMPortObjects[COM2].getLineCodingData.bDataBits = 8;
    appData.appCOMPortObjects[COM2].getLineCodingData.bParityType = 0;
    appData.appCOMPortObjects[COM2].getLineCodingData.bCharFormat = 0;

    appData.appCOMPortObjects[COM1].readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    appData.appCOMPortObjects[COM1].writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    appData.appCOMPortObjects[COM1].isReadComplete = true;
    appData.appCOMPortObjects[COM1].isWriteComplete = false;

    appData.appCOMPortObjects[COM2].readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    appData.appCOMPortObjects[COM2].writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    appData.appCOMPortObjects[COM2].isReadComplete = true;
    appData.appCOMPortObjects[COM2].isWriteComplete = false;
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /* Update the application state machine based
     * on the current state */

    switch(appData.state)
    {
        case APP_STATE_INIT:

            /* Open the device layer */
            appData.deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );

            if(appData.deviceHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceHandle, APP_USBDeviceEventHandler, 0);

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }

            break;

        case APP_STATE_WAIT_FOR_CONFIGURATION:

            /* Check if the device was configured */

            if(appData.isConfigured)
            {
                /* If the device is configured then lets start
                 * the application */

                appData.state = APP_STATE_CHECK_IF_CONFIGURED;

                /* Schedule a read on COM1 and COM2 */
                appData.appCOMPortObjects[COM1].isReadComplete = false;
                appData.appCOMPortObjects[COM2].isReadComplete = false;

                USB_DEVICE_CDC_Read(COM1,
                        &appData.appCOMPortObjects[COM1].readTransferHandle,
                        com1ReadBuffer, APP_READ_BUFFER_SIZE);

                USB_DEVICE_CDC_Read(COM2,
                        &appData.appCOMPortObjects[COM2].readTransferHandle,
                        com2ReadBuffer, APP_READ_BUFFER_SIZE);

            }
            break;

        case APP_STATE_CHECK_IF_CONFIGURED:

            if(appData.isConfigured)
            {
                /* This means this device is still configured */

                appData.state = APP_STATE_CHECK_FOR_READ_COMPLETE;
            }
            else
            {
                APP_StateReset();
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            break;

        case APP_STATE_CHECK_FOR_READ_COMPLETE:

            if(appData.appCOMPortObjects[COM1].isReadComplete == true)
            {
                /* This means we got data on COM1. Write this data to COM2.*/

                appData.appCOMPortObjects[COM1].isReadComplete = false;
                appData.appCOMPortObjects[COM2].isWriteComplete = false;

                USB_DEVICE_CDC_Write(COM2,
                        &appData.appCOMPortObjects[COM2].writeTransferHandle,
                        com1ReadBuffer, 1,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);

            }

            if(appData.appCOMPortObjects[COM2].isReadComplete == true)
            {
                /* This means we got data on COM2. Write this data to COM1 */

                appData.appCOMPortObjects[COM2].isReadComplete = false;
                appData.appCOMPortObjects[COM1].isWriteComplete = false;

                USB_DEVICE_CDC_Write(COM1,
                        &appData.appCOMPortObjects[COM1].writeTransferHandle,
                        com2ReadBuffer, 1,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);

            }
            appData.state = APP_STATE_CHECK_FOR_WRITE_COMPLETE;
            break;

        case APP_STATE_CHECK_FOR_WRITE_COMPLETE:

            /* Check if the write is complete */

            if(appData.appCOMPortObjects[COM2].isWriteComplete)
            {
                USB_DEVICE_CDC_Read(COM1,
                        &appData.appCOMPortObjects[COM1].readTransferHandle,
                        com1ReadBuffer, APP_READ_BUFFER_SIZE);

                appData.appCOMPortObjects[COM1].isReadComplete = false;
                appData.appCOMPortObjects[COM2].isWriteComplete = false;

            }

            if(appData.appCOMPortObjects[COM1].isWriteComplete)
            {
                USB_DEVICE_CDC_Read(COM2,
                        &appData.appCOMPortObjects[COM2].readTransferHandle,
                        com2ReadBuffer, APP_READ_BUFFER_SIZE);

                appData.appCOMPortObjects[COM2].isReadComplete = false;
                appData.appCOMPortObjects[COM1].isWriteComplete = false;

            }

            appData.state = APP_STATE_CHECK_IF_CONFIGURED;
            break;

        case APP_STATE_ERROR:
            break;

        default:
            break;
    }
}
 

/*******************************************************************************
 End of File
 */


