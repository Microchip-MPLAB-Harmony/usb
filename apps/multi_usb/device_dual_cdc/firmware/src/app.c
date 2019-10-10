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
#define APP_READ_BUFFER_SIZE 64
#define APP_USB_SWITCH_DEBOUNCE_COUNT_FS                    200
#define APP_USB_SWITCH_DEBOUNCE_COUNT_HS                    1280
uint8_t __attribute__((aligned(16))) switchPromptUSB[] = "\r\nPUSH BUTTON PRESSED";

uint8_t CACHE_ALIGN readBuffer[2][APP_READ_BUFFER_SIZE];

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



/*******************************************************
 * USB CDC Device Events - Application Event Handler
 *******************************************************/

USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler
(
    USB_DEVICE_CDC_INDEX index ,
    USB_DEVICE_CDC_EVENT event ,
    void * pData,
    uintptr_t userData
)
{
    APP_USB_DEVICE_OBJECT * appDataObject;
    appDataObject = (APP_USB_DEVICE_OBJECT *)userData;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;
    USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE * eventDataRead; 

    switch ( event )
    {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

            USB_DEVICE_ControlSend(appDataObject->deviceHandle,
                    &appDataObject->comObject.getLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */

            USB_DEVICE_ControlReceive(appDataObject->deviceHandle,
                    &appDataObject->comObject.setLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            appDataObject->comObject.controlLineStateData.dtr = controlLineStateData->dtr;
            appDataObject->comObject.controlLineStateData.carrier = controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            appDataObject->comObject.breakData = ((USB_DEVICE_CDC_EVENT_DATA_SEND_BREAK *)pData)->breakDuration;
            
            /* Complete the control transfer by sending a ZLP  */
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:

            /* This means that the host has sent some data*/
            eventDataRead = (USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE *)pData;
            appDataObject->comObject.isReadComplete = true;
            appDataObject->comObject.numBytesRead = eventDataRead->length;
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. For now we accept all the data */

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We dont
             * do much with this data in this demo. */
            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:

            /* This means that the data write got completed. We can schedule
             * the next read. */

            appDataObject->comObject.isWriteComplete = true;
            break;

        default:
            break;
    }

    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
}

/***********************************************
 * Application USB Device Layer Event Handler.
 ***********************************************/
void APP_USBDeviceEventHandler ( USB_DEVICE_EVENT event, void * eventData, uintptr_t context )
{
    USB_DEVICE_EVENT_DATA_CONFIGURED *configuredEventData;
    APP_USB_DEVICE_OBJECT* appUsbDeviceObject = (APP_USB_DEVICE_OBJECT*)context; 
    switch ( event )
    {
        case USB_DEVICE_EVENT_SOF:
            /* This event is used for switch debounce. This flag is reset
             * by the switch process routine. */
            appUsbDeviceObject->sofEventHasOccurred = true; 
            break;

        case USB_DEVICE_EVENT_RESET:
            
            /* Update LED to show reset state */
            if (appUsbDeviceObject->comObject.cdcInstance == 0)
            {                   
                /* Update LED to show configured state */
                LED1_Off();
            }
            else 
            {
                LED2_Off();
            }
            appUsbDeviceObject->isConfigured = false; 

            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Check the configuration. We only support configuration 1 */
            configuredEventData = (USB_DEVICE_EVENT_DATA_CONFIGURED*)eventData;
            if ( configuredEventData->configurationValue == 1)
            {
                /* Register the CDC Device application event handler here.
                * Note how the appData object pointer is passed as the
                * user data */

                USB_DEVICE_CDC_EventHandlerSet(appUsbDeviceObject->comObject.cdcInstance, APP_USBDeviceCDCEventHandler, (uintptr_t)appUsbDeviceObject);

                /* Mark that the device is now configured */
                appUsbDeviceObject->isConfigured = true;
                
                if (appUsbDeviceObject->comObject.cdcInstance == 0)
                {                   
                    /* Update LED to show configured state */
                     LED1_On();       
                }
                else if (appUsbDeviceObject->comObject.cdcInstance == 1)
                {
                    /* Update LED to show configured state */
                     LED2_On();
                }
            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS was detected. We can attach the device */
            USB_DEVICE_Attach(appUsbDeviceObject->deviceHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS is not available any more. Detach the device. */
            USB_DEVICE_Detach(appUsbDeviceObject->deviceHandle);
            if (appUsbDeviceObject->comObject.cdcInstance == 0)
            {                   
                /* Update LED to show configured state */
                LED1_Off(  );
            }
            else 
            {
                LED2_Off(  ); 
            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            break;

        case USB_DEVICE_EVENT_RESUMED:
            break;
            
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

void APP_ProcessSwitchPress(APP_USB_DEVICE_OBJECT* deviceObject)
{
    /* This function checks if the switch is pressed and then
     * debounces the switch press*/
    bool switchPressed;
    
    if (deviceObject->comObject.cdcInstance == 0)
    {
        switchPressed = ( SWITCH_STATE_PRESSED == SWITCH_Get() ? true : false ) ;
    }
    else
    {
        switchPressed = ( SWITCH_STATE_PRESSED == SWITCH2_Get() ? true : false ) ;
    }
    if(switchPressed)
    {
        if(deviceObject->ignoreSwitchPress)
        {
            /* This means the key press is in progress */
            if(deviceObject->sofEventHasOccurred)
            {
                /* A timer event has occurred. Update the debounce timer */
                deviceObject->switchDebounceTimer ++;
                deviceObject->sofEventHasOccurred = false;
                if (USB_DEVICE_ActiveSpeedGet(deviceObject->deviceHandle) == USB_SPEED_FULL)
                {
                    deviceObject->debounceCount = APP_USB_SWITCH_DEBOUNCE_COUNT_FS;
                }
                else if (USB_DEVICE_ActiveSpeedGet(deviceObject->deviceHandle) == USB_SPEED_HIGH)
                {
                    deviceObject->debounceCount = APP_USB_SWITCH_DEBOUNCE_COUNT_HS;
                }
                if(deviceObject->switchDebounceTimer == deviceObject->debounceCount)
                {
                    /* Indicate that we have valid switch press. The switch is
                     * pressed flag will be cleared by the application tasks
                     * routine. We should be ready for the next key press.*/
                    deviceObject->isSwitchPressed = true;
                    deviceObject->switchDebounceTimer = 0;
                    deviceObject->ignoreSwitchPress = false;
                }
            }
        }
        else
        {
            /* We have a fresh key press */
            deviceObject->ignoreSwitchPress = true;
            deviceObject->switchDebounceTimer = 0;
        }
    }
    else
    {
        /* No key press. Reset all the indicators. */
        deviceObject->ignoreSwitchPress = false;
        deviceObject->switchDebounceTimer = 0;
        deviceObject->sofEventHasOccurred = false;
    }

}
/*****************************************************
 * This function is called in every step of the
 * application state machine.
 *****************************************************/

bool APP_StateReset(APP_USB_DEVICE_OBJECT* deviceObject)
{
    /* This function returns true if the device
     * was reset  */

    bool retVal;

    if(deviceObject->isConfigured == false)
    {
        deviceObject->state = APP_STATE_WAIT_FOR_CONFIGURATION;
        deviceObject->comObject.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        deviceObject->comObject.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        deviceObject->comObject.isReadComplete = true;
        deviceObject->comObject.isWriteComplete = true;
        retVal = true;
    }
    else
    {
        retVal = false;
    }

    return(retVal);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
void __InitializeDeviceObject(uint8_t index)
{
    /* Device Layer Handle  */
    appData.deviceObject[index].deviceHandle = USB_DEVICE_HANDLE_INVALID ;

    /* Device configured status */
    appData.deviceObject[index].isConfigured = false;

    appData.deviceObject[index].comObject.cdcInstance = index; 
    /* Initial get line coding state */
    appData.deviceObject[index].comObject.getLineCodingData.dwDTERate = 9600;
    appData.deviceObject[index].comObject.getLineCodingData.bParityType =  0;
    appData.deviceObject[index].comObject.getLineCodingData.bParityType = 0;
    appData.deviceObject[index].comObject.getLineCodingData.bDataBits = 8;

    /* Read Transfer Handle */
    appData.deviceObject[index].comObject.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Write Transfer Handle */
    appData.deviceObject[index].comObject.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Intialize the read complete flag */
    appData.deviceObject[index].comObject.isReadComplete = true;

    /*Initialize the write complete flag*/
    appData.deviceObject[index].comObject.isWriteComplete = true;

    /* Initialize Ignore switch flag */
    appData.deviceObject[index].ignoreSwitchPress = false;

    /* Reset the switch debounce counter */
    appData.deviceObject[index].switchDebounceTimer = 0;

    /* Reset other flags */
    appData.deviceObject[index].sofEventHasOccurred = false;
    appData.deviceObject[index].isSwitchPressed = false;

    /* Set up the read buffer */
    appData.deviceObject[index].comObject.readBuffer = &readBuffer[index][0];
}
/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    __InitializeDeviceObject(0); 
    __InitializeDeviceObject(1);      
}



/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks (void )
{
    /* Update the application state machine based
     * on the current state */

    switch(appData.state)
    {
        case APP_STATE_INIT:

            /* Open the device layer */
            appData.deviceObject[0].deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );
            appData.deviceObject[1].deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_1, DRV_IO_INTENT_READWRITE );
            
            
           if((appData.deviceObject[0].deviceHandle != USB_DEVICE_HANDLE_INVALID) 
                && 
                    (appData.deviceObject[1].deviceHandle != USB_DEVICE_HANDLE_INVALID))
            {
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceObject[0].deviceHandle, APP_USBDeviceEventHandler, (uintptr_t)&appData.deviceObject[0]);
                
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceObject[1].deviceHandle, APP_USBDeviceEventHandler,(uintptr_t)&appData.deviceObject[1]);
                
                appData.state = APP_STATE_RUN; 
                appData.deviceObject[0].state = APP_STATE_WAIT_FOR_CONFIGURATION;
                appData.deviceObject[1].state = APP_STATE_WAIT_FOR_CONFIGURATION;
                
            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }

            break;
        case APP_STATE_RUN:
             
             _AppTaskUsbDevice(&appData.deviceObject[0]);
             _AppTaskUsbDevice(&appData.deviceObject[1]);
            break; 
        case APP_STATE_ERROR:
            break;
        default:
            break;
    }
}
void _AppTaskUsbDevice(APP_USB_DEVICE_OBJECT* deviceObject)
{
    int i; 
    switch(deviceObject->state)
    {
        case APP_STATE_WAIT_FOR_CONFIGURATION:
            
            /* Check if the device was configured */
            if(deviceObject->isConfigured)
            {
               deviceObject->state = APP_STATE_SCHEDULE_READ; 
            }
            
          
            break;
        case APP_STATE_SCHEDULE_READ:

            if(APP_StateReset(deviceObject))
            {
                break;
            }

            /* If a read is complete, then schedule a read
             * else wait for the current read to complete */

            deviceObject->state = APP_STATE_WAIT_FOR_READ_COMPLETE;
            if(deviceObject->comObject.isReadComplete == true)
            {
                deviceObject->comObject.isReadComplete = false;
                deviceObject->comObject.readTransferHandle =  USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

                USB_DEVICE_CDC_Read (deviceObject->comObject.cdcInstance,
                        &deviceObject->comObject.readTransferHandle, deviceObject->comObject.readBuffer,
                        APP_READ_BUFFER_SIZE);
                
                if(deviceObject->comObject.readTransferHandle == USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID)
                {
                    deviceObject->state = APP_STATE_ERROR;
                    break;
                }
            }

            break;

        case APP_STATE_WAIT_FOR_READ_COMPLETE:
        case APP_STATE_CHECK_SWITCH_PRESSED:

            if(APP_StateReset(deviceObject))
            {
                break;
            }

            APP_ProcessSwitchPress(deviceObject);

            /* Check if a character was received or a switch was pressed.
             * The isReadComplete flag gets updated in the CDC event handler. */

            if(deviceObject->comObject.isReadComplete || deviceObject->isSwitchPressed)
            {
                deviceObject->state = APP_STATE_SCHEDULE_WRITE;
            }

            break;

        
        case APP_STATE_SCHEDULE_WRITE:

            if(APP_StateReset(deviceObject))
            {
                break;
            }

            /* Setup the write */

            deviceObject->comObject.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
            deviceObject->comObject.isWriteComplete = false;
            deviceObject->state = APP_STATE_WAIT_FOR_WRITE_COMPLETE;

            if(deviceObject->isSwitchPressed)
            {
                /* If the switch was pressed, then send the switch prompt*/
                deviceObject->isSwitchPressed = false;
                USB_DEVICE_CDC_Write(deviceObject->comObject.cdcInstance,
                        &deviceObject->comObject.writeTransferHandle, switchPromptUSB, 23,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
            }
            else
            {
                /* Else echo each received character by adding 1 */
                for(i=0; i<deviceObject->comObject.numBytesRead; i++)
                {
                    if((deviceObject->comObject.readBuffer[i] != 0x0A) 
                            && (deviceObject->comObject.readBuffer[i] != 0x0D))
                    {
                        deviceObject->comObject.readBuffer[i] = deviceObject->comObject.readBuffer[i] + 1;
                    }
                }
                USB_DEVICE_CDC_Write(deviceObject->comObject.cdcInstance,
                        &deviceObject->comObject.writeTransferHandle,
                        deviceObject->comObject.readBuffer, 
                        deviceObject->comObject.numBytesRead,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
            }

            break;

        case APP_STATE_WAIT_FOR_WRITE_COMPLETE:

            if(APP_StateReset(deviceObject))
            {
                break;
            }

            /* Check if a character was sent. The isWriteComplete
             * flag gets updated in the CDC event handler */

            if(deviceObject->comObject.isWriteComplete == true)
            {
                deviceObject->state = APP_STATE_SCHEDULE_READ;
            }

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

