/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
uint8_t APP_MAKE_BUFFER_DMA_READY cdcReadBuffer[APP_READ_BUFFER_SIZE] ;
uint8_t APP_MAKE_BUFFER_DMA_READY uartReadBuffer[APP_READ_BUFFER_SIZE];

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

void APP_BufferEventHandler(
    DRV_USART_BUFFER_EVENT bufferEvent, 
    DRV_USART_BUFFER_HANDLE bufferHandle, 
    uintptr_t context 
)
{
    switch(bufferEvent)
    {
        case DRV_USART_BUFFER_EVENT_COMPLETE:
            
            if(appData.readBufferHandler == bufferHandle)
            {
                if(appData.isUSARTReadInProgress == true)
                {
                    appData.isUSARTReadInProgress = false;
                }
            }
            
            if(appData.writeBufferHandler == bufferHandle)
            {
                if(appData.isUSARTWriteInProgress == true)
                {
                    appData.isUSARTWriteInProgress = false;
                }
            }
            
            break;
        
        case DRV_USART_BUFFER_EVENT_ERROR:  
            
            appData.errorStatus = true;
            
            break;        

        default:        
            break;        
    }
}

/*******************************************************
 * USB CDC Device Events - Application Event Handler
 *******************************************************/
USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler
(
    USB_DEVICE_CDC_INDEX index ,
    USB_DEVICE_CDC_EVENT event ,
    void* pData,
    uintptr_t userData
)
{
    USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE * eventDataRead;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;
    APP_DATA * appDataObject = (APP_DATA *)userData;

    switch ( event )
    {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

             USB_DEVICE_ControlSend(appDataObject->usbDevHandle,
                    &appDataObject->getLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */
            appDataObject->isSetLineCodingCommandInProgress = true;
            appDataObject->isBaudrateDataReceived = false;
            USB_DEVICE_ControlReceive(appDataObject->usbDevHandle,
                    &appDataObject->setLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            appDataObject->controlLineStateData.dtr = controlLineStateData->dtr;
            appDataObject->controlLineStateData.carrier = controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appDataObject->usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            appDataObject->breakData = ((USB_DEVICE_CDC_EVENT_DATA_SEND_BREAK *) pData)->breakDuration;
            
            /* Complete the control transfer by sending a ZLP  */
            USB_DEVICE_ControlStatus(appDataObject->usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:

            /* This means that the host has sent some data*/
            eventDataRead = (USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE *)pData;
            
            if(eventDataRead->status != USB_DEVICE_CDC_RESULT_ERROR)
            {
                appDataObject->isCDCReadInProgress = false;
                
                appDataObject->readLength = eventDataRead->length;
            }
//            else
//            {
//                appDataObject->errorStatus = true;
//            }
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. */
            if (appDataObject->isSetLineCodingCommandInProgress == true)
            {
               /* We have received set line coding command from the Host.
                * DRV_USART_BaudSet() function is not interrupt safe and it
                * should not be called here. It is called in APP_Tasks()
                * function. The ACK for Status stage of the control transfer is
                * send in the APP_Tasks() function.  */
                appDataObject->isSetLineCodingCommandInProgress = false;
                appDataObject->isBaudrateDataReceived = true;
                appDataObject->getLineCodingData.dwDTERate = appDataObject->setLineCodingData.dwDTERate;
                appDataObject->getLineCodingData.bParityType = appDataObject->setLineCodingData.bParityType;
                appDataObject->getLineCodingData.bDataBits = appDataObject->setLineCodingData.bDataBits;
                appDataObject->getLineCodingData.bCharFormat = appDataObject->setLineCodingData.bCharFormat;
            }
            else
            {
				/* ACK the Status stage of the Control transfer */
                USB_DEVICE_ControlStatus(appDataObject->usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);
            }
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We don't
             * do much with this data in this demo. */
            break;
            
        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_ABORTED:
        
            /* The control transfer has been aborted */
            if (appDataObject->isSetLineCodingCommandInProgress == true)
            {
                appDataObject->isSetLineCodingCommandInProgress = false;
                appDataObject->isBaudrateDataReceived = false;
            }

            break;
        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:

            /* This means that the data write got completed. We can schedule
             * the next read. */
            appDataObject->isCDCWriteInProgress = false;

            break;

        default:
            break;
    }

    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
}

/*********************************************
 * Application USB Device Layer Event Handler
 *********************************************/

void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, void * eventData, uintptr_t context)
{
    uint8_t configurationValue;
    switch(event)
    {
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:

            /* USB device is reset or device is deconfigured.
             * This means that USB device layer is about to deininitialize
             * all function drivers. Update LEDs to indicate
             * reset/deconfigured state. */

            appData.deviceIsConfigured = false;

            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Check the configuration */
            configurationValue = ((USB_DEVICE_EVENT_DATA_CONFIGURED *)eventData)->configurationValue;
            if (configurationValue == 1)
            {
                /* The device is in configured state. Update LED indication */

                /* Register the CDC Device application event handler here.
                 * Note how the appData object pointer is passed as the
                 * user data */

                USB_DEVICE_CDC_EventHandlerSet(USB_DEVICE_CDC_INDEX_0, APP_USBDeviceCDCEventHandler, (uintptr_t)&appData);

                /* mark that set configuration is complete */
                appData.deviceIsConfigured = true;

            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:

            /* Update LEDs */
            break;


        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS is detected. Attach the device */
            USB_DEVICE_Attach(appData.usbDevHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS is removed. Detach the device */
            USB_DEVICE_Detach (appData.usbDevHandle);
            appData.deviceIsConfigured = false;

            LED_Off();            
            break;

        /* These events are not used in this demo. */
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

/*****************************************************
 * This function is called in every step of the
 * application state machine.
 *****************************************************/

bool APP_StateReset(void)
{
    /* This function returns true if the device
     * was reset  */

    bool retVal;

    if(appData.deviceIsConfigured == false)
    {
        appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
        appData.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.isCDCReadInProgress = false;
        appData.isCDCWriteInProgress = false;

        appData.isSetLineCodingCommandInProgress = false;
        appData.isBaudrateDataReceived = false;
        LED_Off();
        
        retVal = true;
    }
    else
    {
        retVal = false;
    }

    return(retVal);
}

/***************************************************************************
 * This function Handles the Set Line coding command from Host.
 ***************************************************************************/
void _APP_SetLineCodingHandler(void)
{    
    DRV_USART_SERIAL_SETUP UsartSetup;

    UsartSetup.baudRate = appData.getLineCodingData.dwDTERate;
        
    switch(appData.getLineCodingData.bParityType)
    {
        case USB_CDC_LINE_CODING_PARITY_NONE:
            
            UsartSetup.parity = DRV_USART_PARITY_NONE;            
            break;
            
        case USB_CDC_LINE_CODING_PARITY_EVEN:
            
            UsartSetup.parity = DRV_USART_PARITY_EVEN;   
            break;
            
        case USB_CDC_LINE_CODING_PARITY_ODD:
            
            UsartSetup.parity = DRV_USART_PARITY_ODD;   
            break;
            
        case USB_CDC_LINE_CODING_PARITY_MARK:
            
            UsartSetup.parity = DRV_USART_PARITY_MARK;   
            break;
            
        case USB_CDC_LINE_CODING_PARITY_SPACE:
            
            UsartSetup.parity = DRV_USART_PARITY_SPACE;   
            break;
            
        default:
            UsartSetup.parity = DRV_USART_PARITY_NONE;    
            break;
    }
        
    switch(appData.getLineCodingData.bCharFormat)
    {
        case USB_CDC_LINE_CODING_STOP_1_BIT:
            
            UsartSetup.stopBits = DRV_USART_STOP_1_BIT;            
            break;
            
        case USB_CDC_LINE_CODING_STOP_1_5_BIT:
            
            UsartSetup.stopBits = DRV_USART_STOP_1_5_BIT;   
            break;
            
        case USB_CDC_LINE_CODING_STOP_2_BIT:
            
            UsartSetup.stopBits = DRV_USART_STOP_2_BIT;   
            break;
            
        default:
            UsartSetup.stopBits = DRV_USART_STOP_1_BIT;    
            break;
    }
        
    switch(appData.getLineCodingData.bDataBits)
    {
        case USB_CDC_LINE_CODING_DATA_5_BIT:
            
            UsartSetup.dataWidth = DRV_USART_DATA_5_BIT;            
            break;
            
        case USB_CDC_LINE_CODING_DATA_6_BIT:
            
            UsartSetup.dataWidth = DRV_USART_DATA_6_BIT;   
            break;
            
        case USB_CDC_LINE_CODING_DATA_7_BIT:
            
            UsartSetup.dataWidth = DRV_USART_DATA_7_BIT;   
            break;
            
        case USB_CDC_LINE_CODING_DATA_8_BIT:
            
            UsartSetup.dataWidth = DRV_USART_DATA_8_BIT;   
            break;
            
        case USB_CDC_LINE_CODING_DATA_16_BIT:
            
            UsartSetup.dataWidth = DRV_USART_DATA_9_BIT;   
            break;
            
        default:
            UsartSetup.dataWidth = DRV_USART_DATA_8_BIT;    
            break;
    }

    DRV_USART_ReadQueuePurge(appData.usartHandle);
    DRV_USART_WriteQueuePurge(appData.usartHandle);
    DRV_USART_ReadAbort(appData.usartHandle);
    
    if (true == DRV_USART_SerialSetup(appData.usartHandle, &UsartSetup))
    {
        /* Baudrate is changed successfully. Update Baudrate info in the
         * Get line coding structure. */
                
        DRV_USART_ReadBufferAdd(appData.usartHandle, appData.uartReadBuffer, 1, &appData.readBufferHandler);
        
        /* Acknowledge the Status stage of the Control transfer */
        USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);
    }
    else
    {
        /* Baudrate was not set. There are two ways that an unsupported
         * baud rate could be handled.  The first is just to ignore the
         * request and ACK the control transfer.  That is what is currently
         * implemented below. */
         USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);


        /* The second possible method is to stall the STATUS stage of the
         * request. STALLing the STATUS stage will cause an exception to be
         * thrown in the requesting application. Some programs, like
         * HyperTerminal, handle the exception properly and give a pop-up
         * box indicating that the request settings are not valid.  Any
         * application that does not handle the exception correctly will
         * likely crash when this request fails.  For the sake of example
         * the code required to STALL the status stage of the request is
         * provided below.  It has been left out so that this demo does not
         * cause applications without the required exception handling to
         * crash.*/
         //USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
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
     /* Device Layer Handle  */
    appData.usbDevHandle = USB_DEVICE_HANDLE_INVALID;

    /* USART Driver Handle */
    appData.usartHandle = DRV_HANDLE_INVALID;

    /* CDC Instance index for this app object00*/
    appData.cdcInstance = USB_DEVICE_CDC_INDEX_0;

    /* app state */
    appData.state = APP_STATE_INIT;

    /* device configured status */
    appData.deviceIsConfigured = false;

    /* Initial get line coding state */
    appData.getLineCodingData.dwDTERate = 9600;
    appData.getLineCodingData.bDataBits = 8;
    appData.getLineCodingData.bParityType = 0;
    appData.getLineCodingData.bCharFormat = 0;

    /* Read Transfer Handle */
    appData.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Write Transfer Handle */
    appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Initialize the read complete flag */
    appData.isCDCReadInProgress = false;

    /*Initialize the write complete flag*/
    appData.isCDCWriteInProgress = false;

    /*Initialize the buffer pointers */
    appData.cdcReadBuffer = &cdcReadBuffer[0];

    appData.uartReadBuffer = &uartReadBuffer[0];
    
    memset(cdcReadBuffer, 0, sizeof(cdcReadBuffer));
    memset(uartReadBuffer, 0, sizeof(uartReadBuffer));
    

    appData.uartTxCount = 0;
    
    appData.usartHandle = DRV_HANDLE_INVALID;
    appData.readBufferHandler = DRV_USART_BUFFER_HANDLE_INVALID;  
    appData.writeBufferHandler = DRV_USART_BUFFER_HANDLE_INVALID;  

    /* Initialize the Set Line coding flags */
    appData.isBaudrateDataReceived = false;
    appData.isSetLineCodingCommandInProgress = false;

    appData.isUSARTWriteInProgress = false;
    appData.isUSARTReadInProgress = false;
    
    LED_Off();
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    if ((appData.deviceIsConfigured) && (appData.isBaudrateDataReceived))
    {
		 appData.isBaudrateDataReceived = false;
        _APP_SetLineCodingHandler();
    }

    /* Update the application state machine based
     * on the current state */
    switch(appData.state)
    {
        case APP_STATE_INIT:

            /* Open the device layer */
            appData.usbDevHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );

            if(appData.usbDevHandle != USB_DEVICE_HANDLE_INVALID)
            {
                appData.usartHandle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE);
                
                if (appData.usartHandle != DRV_HANDLE_INVALID)
                {
                    DRV_USART_BufferEventHandlerSet(appData.usartHandle, APP_BufferEventHandler, 0);
                    
                    /* Register a callback with device layer to get event notification (for end point 0) */
                    USB_DEVICE_EventHandlerSet(appData.usbDevHandle,  APP_USBDeviceEventHandler, 0);

                    /* Application waits for device configuration. */
                    appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
                }
                else
                {
                    appData.state = APP_STATE_ERROR;
                }

            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }

            break;

        case APP_STATE_WAIT_FOR_CONFIGURATION:

            /* Check if the device is configured */
            if(appData.deviceIsConfigured)
            {
                /* Schedule the first read from CDC function driver */
                appData.state = APP_STATE_SERIAL_EMULATOR;                
                appData.isCDCReadInProgress = true;
                appData.isUSARTReadInProgress = true;            
                appData.isCDCWriteInProgress = false;
                appData.isUSARTWriteInProgress = false;
                
                LED_On();
                
                USB_DEVICE_CDC_Read (appData.cdcInstance, &appData.readTransferHandle, appData.cdcReadBuffer, APP_READ_BUFFER_SIZE);
                
                DRV_USART_ReadBufferAdd(appData.usartHandle, appData.uartReadBuffer, 1, &appData.readBufferHandler);
            }
            break;

        case APP_STATE_SERIAL_EMULATOR:

            if(APP_StateReset())
            {
                break;
            }

            if(appData.isCDCReadInProgress == false)
            {
                /* If CDC read is complete, send the received data to the UART. */
                appData.isUSARTWriteInProgress = true;
                
                DRV_USART_WriteBufferAdd(appData.usartHandle, appData.cdcReadBuffer, appData.readLength, &appData.writeBufferHandler);
                
                if (appData.writeBufferHandler != DRV_USART_BUFFER_HANDLE_INVALID)
                {
					appData.isCDCReadInProgress = true;
                    
                    /* This means we have sent all the data. We schedule the next CDC Read. */
                    USB_DEVICE_CDC_Read (appData.cdcInstance, &appData.readTransferHandle, appData.cdcReadBuffer, APP_READ_BUFFER_SIZE);
                }        
                else
                {
                    appData.isUSARTWriteInProgress = false;
                }
                    
            }
            
            /* Check if a character was received on the UART */
            if(appData.isUSARTReadInProgress == false)
            {
                appData.isCDCWriteInProgress = true;

				appData.isUSARTReadInProgress = true;
            
                USB_DEVICE_CDC_Write(appData.cdcInstance, &appData.writeTransferHandle, appData.uartReadBuffer, 1, USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                                
                DRV_USART_ReadBufferAdd(appData.usartHandle, (void *)appData.uartReadBuffer, 1, &appData.readBufferHandler);
                
            }
            
            if(appData.errorStatus == true)
            {
                appData.state = APP_STATE_ERROR;
            }
            
            break;

        case APP_STATE_ERROR:
            
            LED_Off();
            
            break;
        default:
            break;
    }
}


/*******************************************************************************
 End of File
 */
