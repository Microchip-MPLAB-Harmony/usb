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
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "usb/src/usb_device_printer_local.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

uint8_t APP_MAKE_BUFFER_DMA_READY printerReadBuffer[APP_READ_BUFFER_SIZE];
uint8_t APP_MAKE_BUFFER_DMA_READY printerWriteBuffer[APP_WRITE_BUFFER_SIZE];

#define LANG_PARSE_ACK  0x1
#define LANG_PARSE_NACK 0x0

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
 * USB PRINTER Device Events - Application Event Handler
 *******************************************************/
void APP_BufferEventHandler(
    DRV_USART_BUFFER_EVENT bufferEvent, 
    DRV_USART_BUFFER_HANDLE bufferHandle, 
    uintptr_t context 
)
{
    switch(bufferEvent)
    {
        case DRV_USART_BUFFER_EVENT_COMPLETE:
            if(appData.isUSARTWriteInProgress == true)
            {
                appData.isUSARTWriteInProgress = false;
            }
            break;
        
        case DRV_USART_BUFFER_EVENT_ERROR:        
            appData.usarterrorStatus = true;
            break;        

        default:        
            break;        
    }
}

USB_DEVICE_PRINTER_EVENT_RESPONSE APP_USBDevicePrinterEventHandler
(
    USB_DEVICE_PRINTER_INDEX index,
    USB_DEVICE_PRINTER_EVENT event,
    void * pData,
    uintptr_t userData
)
{
    APP_DATA * appDataObject;
    USB_DEVICE_PRINTER_EVENT_DATA_READ_COMPLETE * eventDataRead;
    uint8_t prntrStatus;
    
    appDataObject = (APP_DATA *)userData;

    switch(event)
    {
        case USB_DEVICE_PRINTER_GET_PORT_STATUS:

            /* This means the host wants to know the printer's current status,
             * in a format which is compatible with the status register of a 
             * standard PC parallel port. This is a control transfer request. 
             * Use the USB_DEVICE_ControlSend() function to send the data to
             * host. */
            prntrStatus = (uint8_t)(((appDataObject->portStatus.errorStatus << 3) & 0x08) | 
                ((appDataObject->portStatus.selectStatus << 4) & 0x10) |
                ((appDataObject->portStatus.paperEmptyStatus << 5) & 0x20));
            
            USB_DEVICE_ControlSend( appDataObject->deviceHandle, &prntrStatus, 1 );

            break;

        case USB_DEVICE_PRINTER_EVENT_READ_COMPLETE:

            /* This means that the host has sent some data*/
            eventDataRead = (USB_DEVICE_PRINTER_EVENT_DATA_READ_COMPLETE *)pData;
            appDataObject->isReadComplete = true;
            appDataObject->numBytesRead = eventDataRead->length; 

            break;
            
        case USB_DEVICE_PRINTER_EVENT_WRITE_COMPLETE:

            /* This means that the data write got completed */

            appDataObject->isWriteComplete = true;
            break;

        default:
            break;            
           
    }

    return USB_DEVICE_PRINTER_EVENT_RESPONSE_NONE;
}

/***********************************************
 * Application USB Device Layer Event Handler.
 ***********************************************/
void APP_USBDeviceEventHandler 
(
    USB_DEVICE_EVENT event, 
    void * eventData, 
    uintptr_t context 
)
{
    USB_DEVICE_EVENT_DATA_CONFIGURED *configuredEventData;

    switch(event)
    {
        case USB_DEVICE_EVENT_SOF:

            /* This event is used for switch de-bounce. This flag is reset
             * by the switch process routine. */
            appData.sofEventHasOccurred = true;
            
            break;

        case USB_DEVICE_EVENT_RESET:

            /* Update LED to show reset state */
            LED_Off();

            appData.isConfigured = false;

            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Check the configuration. We only support configuration 1 */
            configuredEventData = (USB_DEVICE_EVENT_DATA_CONFIGURED*)eventData;
            
            if ( configuredEventData->configurationValue == 1)
            {
                /* Update LED to show configured state */
                LED_On();
                
                /* Register the Printer Device application event handler here.
                 * Note how the appData object pointer is passed as the
                 * user data */

                USB_DEVICE_PRINTER_EventHandlerSet(USB_DEVICE_PRINTER_INDEX_0, APP_USBDevicePrinterEventHandler, (uintptr_t)&appData);

                /* Mark that the device is now configured */
                appData.isConfigured = true;
            }
            
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS was detected. We can attach the device */
            USB_DEVICE_Attach(appData.deviceHandle);
            
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS is not available any more. Detach the device. */
            USB_DEVICE_Detach(appData.deviceHandle);
            
            LED_Off();
            
            break;

        case USB_DEVICE_EVENT_SUSPENDED:

            /* Switch LED to show suspended state */
            LED_Off();
            
            break;

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

void APP_ProcessSwitchPress(void)
{
    /* This function checks if the switch is pressed and then
     * debounces the switch press */
    
    if(SWITCH_STATE_PRESSED == (SWITCH_Get()))
    {
        if(appData.ignoreSwitchPress)
        {
            /* This means the key press is in progress */
            if(appData.sofEventHasOccurred)
            {
                /* A timer event has occurred. Update the debounce timer */
                appData.switchDebounceTimer ++;
                appData.sofEventHasOccurred = false;
                
                if (USB_DEVICE_ActiveSpeedGet(appData.deviceHandle) == USB_SPEED_FULL)
                {
                    appData.debounceCount = APP_USB_SWITCH_DEBOUNCE_COUNT_FS;
                }
                else if (USB_DEVICE_ActiveSpeedGet(appData.deviceHandle) == USB_SPEED_HIGH)
                {
                    appData.debounceCount = APP_USB_SWITCH_DEBOUNCE_COUNT_HS;
                }
                if(appData.switchDebounceTimer == appData.debounceCount)
                {
                    /* Indicate that we have valid switch press. The switch is
                     * pressed flag will be cleared by the application tasks
                     * routine. We should be ready for the next key press.*/
                    appData.isSwitchPressed = true;
                    appData.switchDebounceTimer = 0;
                    appData.ignoreSwitchPress = false;
                }
            }
        }
        else
        {
            /* We have a fresh key press */
            appData.ignoreSwitchPress = true;
            appData.switchDebounceTimer = 0;
        }
    }
    else
    {
        /* No key press. Reset all the indicators. */
        appData.ignoreSwitchPress = false;
        appData.switchDebounceTimer = 0;
        appData.sofEventHasOccurred = false;
    }
}


uint8_t App_LangParseCheck(void)
{
    /* Implement language parser check API here and return
     * the status accordingly 
     */
    return LANG_PARSE_ACK;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize(void)

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize(void)
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    /* Device Layer Handle  */
    appData.deviceHandle = USB_DEVICE_HANDLE_INVALID ;

    /* Device configured status */
    appData.isConfigured = false;

    /* Initialize Printer Port Status.
     * Whenever Host request for GET_PORT_STATUS, 
     * the printer should send the updated current status. 
     */
    appData.portStatus.errorStatus = 0;
    appData.portStatus.paperEmptyStatus =  1;
    appData.portStatus.selectStatus = 0;
            
    /* Read Transfer Handle */
    appData.readTransferHandle = USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID;

    /* Write Transfer Handle */
    appData.writeTransferHandle = USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID;

    /* Initialize the read complete flag */
    appData.isReadComplete = true;

    /*Initialize the write complete flag*/
    appData.isWriteComplete = true;

    /* Initialize Ignore switch flag */
    appData.ignoreSwitchPress = false;

    /* Reset the switch debounce counter */
    appData.switchDebounceTimer = 0;

    /* Reset other flags */
    appData.sofEventHasOccurred = false;
    
    /* To know status of Switch */
    appData.isSwitchPressed = false;

    /* Set up the read buffer */
    appData.prntrReadBuffer = &printerReadBuffer[0];

    /* Set up the read buffer */
    appData.prntrWriteBuffer = &printerWriteBuffer[0]; 
    
    appData.usartHandle = DRV_HANDLE_INVALID;
    appData.isUSARTWriteInProgress = false;
    appData.usarterrorStatus = false;
}


/******************************************************************************
  Function:
    void APP_Tasks(void)

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks(void)
{
    /* Update the application state machine based
     * on the current state */
    
    switch(appData.state)
    {
        case APP_STATE_INIT:
            appData.usartHandle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE);
            
            /* Open the device layer */
            appData.deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );
            
            if(appData.deviceHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceHandle, APP_USBDeviceEventHandler, 0);
                if(appData.usartHandle != DRV_HANDLE_INVALID)
                {
                    DRV_USART_BufferEventHandlerSet(appData.usartHandle, APP_BufferEventHandler, 0);
                    appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
                }
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
                /* If the device is configured then lets start reading */
                appData.state = APP_STATE_SCHEDULE_READ;
            }
            
            break;

        case APP_STATE_SCHEDULE_READ:

            /* If a read is complete, then schedule a read
             * else wait for the current read to complete */
            
            /* Block by waiting for UARD write 
             * completion status. This is useful if the data receiver is a 
             * slow performing system*/

            if(appData.isReadComplete == true && appData.isUSARTWriteInProgress == false)
            {
                appData.isReadComplete = false;
                appData.readTransferHandle =  USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID;

                USB_DEVICE_PRINTER_Read (USB_DEVICE_PRINTER_INDEX_0,
                        &appData.readTransferHandle, appData.prntrReadBuffer,
                        APP_READ_BUFFER_SIZE);
                
                appData.state = APP_STATE_WAIT_FOR_READ_COMPLETE;
                
                if(appData.readTransferHandle == USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }

            break;

        case APP_STATE_WAIT_FOR_READ_COMPLETE:
            
            if(appData.isReadComplete == true)
            {
                /* appData.prntrReadBuffer has the data received from
                 the Host. Send the data to Printer language parser
                 and wait for the ack */
                if (appData.isUSARTWriteInProgress == false)
                {
                    appData.isUSARTWriteInProgress = true;
                    DRV_USART_WriteBufferAdd(appData.usartHandle, (void*)&appData.prntrReadBuffer[0], appData.numBytesRead, &appData.usartbufferHandler);
                    appData.state = APP_STATE_WAIT_FOR_LANGAUGE_PARSER_ACK;
                }
            }
            
            break;
        
        case APP_STATE_WAIT_FOR_LANGAUGE_PARSER_ACK:    

            /* If ack is received, schedule next read from the Host.
             * Otherwise, wait till ack is received
             */
            
            /* Always expect data from the Host, 
             * if nothing is scheduled for write 
             */
            
            /* Place holder for ack check */
            if(LANG_PARSE_ACK == App_LangParseCheck())
            {
                appData.state = APP_STATE_SCHEDULE_READ;
            }                

            break;
            
        case APP_STATE_CHECK_SWITCH_PRESSED:

            APP_ProcessSwitchPress();

            /* Check if the switch was pressed. */

            if(appData.isSwitchPressed)
            {
                appData.state = APP_STATE_SCHEDULE_WRITE;
            }

            break;


        case APP_STATE_SCHEDULE_WRITE:

            /* Setup the write */

            appData.writeTransferHandle = USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID;
            appData.state = APP_STATE_WAIT_FOR_WRITE_COMPLETE;
            /* Reset Switch Pressed status */
            appData.isSwitchPressed = false;
            
            /* Do nothing.*/
                
            break;

        case APP_STATE_WAIT_FOR_WRITE_COMPLETE:

            /* The isWriteCompleteflag gets updated in the Printer event handler */

            if(appData.isWriteComplete == true)
            {
                appData.state = APP_STATE_SCHEDULE_READ;
            }

            break;

        case APP_STATE_ERROR:
        default:
            
            break;
    }
}
/*******************************************************************************
 End of File
 */

