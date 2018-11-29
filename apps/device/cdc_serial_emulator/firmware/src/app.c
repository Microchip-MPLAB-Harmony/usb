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
uint8_t APP_MAKE_BUFFER_DMA_READY readBuffer[APP_READ_BUFFER_SIZE] ;
uint8_t APP_MAKE_BUFFER_DMA_READY uartReceivedData;

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

void APP_WriteCallback(uintptr_t context)
{
	appData.isUSARTWriteComplete = true;
}

void APP_ReadCallback(uintptr_t context)
{
	appData.isUSARTReadComplete = true;
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

             USB_DEVICE_ControlSend(appDataObject->usbDevHandle,
                    &appDataObject->getLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */
            appData.isSetLineCodingCommandInProgress = true;
            appData.isBaudrateDataReceived = false;
            USB_DEVICE_ControlReceive(appDataObject->usbDevHandle,
                    &appDataObject->setLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            appDataObject->controlLineStateData.dtr = controlLineStateData->dtr;
            appDataObject->controlLineStateData.carrier =
                    controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appDataObject->usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            breakData = (uint16_t *)pData;
            appDataObject->breakData = *breakData;

            /* Complete the control transfer by sending a ZLP  */
            USB_DEVICE_ControlStatus(appDataObject->usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:

            /* This means that the host has sent some data*/
            appDataObject->isCDCReadComplete = true;
            appDataObject->readLength =
                    ((USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE*)pData)->length;
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. */
            if (appData.isSetLineCodingCommandInProgress == true)
            {
               /* We have received set line coding command from the Host.
                * DRV_USART_BaudSet() function is not interrupt safe and it
                * should not be called here. It is called in APP_Tasks()
                * function. The ACK for Status stage of the control transfer is
                * send in the APP_Tasks() function.  */
                appData.isSetLineCodingCommandInProgress = false;
                appData.isBaudrateDataReceived = true;
            }
            else
            {
				/* ACK the Status stage of the Control transfer */
                USB_DEVICE_ControlStatus(appDataObject->usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);
            }
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We dont
             * do much with this data in this demo. */
            break;
        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_ABORTED:
            /* The control transfer has been aborted */
            if (appData.isSetLineCodingCommandInProgress == true)
            {
                appData.isSetLineCodingCommandInProgress = false;
                appData.isBaudrateDataReceived = false;
            }

            break;
        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:

            /* This means that the data write got completed. We can schedule
             * the next read. */

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

                USB_DEVICE_CDC_EventHandlerSet(USB_DEVICE_CDC_INDEX_0,
                        APP_USBDeviceCDCEventHandler, (uintptr_t)&appData);

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
        appData.isCDCReadComplete = true;
        appData.isCDCWriteComplete = true;

        retVal = true;

        appData.isSetLineCodingCommandInProgress = false;
        appData.isBaudrateDataReceived = false;
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
    USART_SERIAL_SETUP UsartSetup;

    UsartSetup.baudRate = appData.setLineCodingData.dwDTERate;
    UsartSetup.parity = appData.setLineCodingData.bParityType;
    UsartSetup.dataWidth = appData.setLineCodingData.bDataBits;
    UsartSetup.stopBits = appData.setLineCodingData.bCharFormat;

//    USART1_SerialSetup(&UsartSetup, CHIP_FREQ_CPU_MAX);


//    resultUsartBaurateSet = DRV_USART_BaudSet(appData.usartHandle, appData.setLineCodingData.dwDTERate);

    if (true == USART1_SerialSetup(&UsartSetup, CHIP_FREQ_CPU_MAX))
    {
        /* Baudrate is changed successfully. Update Baudrate info in the
         * Get line coding structure. */
        appData.getLineCodingData.dwDTERate = appData.setLineCodingData.dwDTERate;

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
    appData.isCDCReadComplete = true;

    /*Initialize the write complete flag*/
    appData.isCDCWriteComplete = true;

    /*Initialize the buffer pointers */
    appData.readBuffer = &readBuffer[0];

    appData.uartReceivedData = &uartReceivedData;

    appData.uartTxCount = 0;

    /* Initialize the Set Line coding flags */
    appData.isBaudrateDataReceived = false;
    appData.isSetLineCodingCommandInProgress = false;

	/* Register callback functions */
	USART1_WriteCallbackRegister(APP_WriteCallback, 0);
	USART1_ReadCallbackRegister(APP_ReadCallback, 0);

    APP_LED0_Off();
    APP_LED1_Off();
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
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.usbDevHandle,  APP_USBDeviceEventHandler, 0);

                /* Application waits for device configuration. */
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
                
                APP_LED0_Off();
                APP_LED1_Off();
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

                APP_LED0_Off();
                APP_LED1_On();
                
                appData.state = APP_STATE_CHECK_CDC_READ;
                appData.isCDCReadComplete = false;

				appData.isUSARTReadComplete = false;

                USART1_Read(&appData.uartReceivedData, 1);

                USB_DEVICE_CDC_Read (appData.cdcInstance, &(appData.readTransferHandle),
                        appData.readBuffer, APP_READ_BUFFER_SIZE);
            }
            break;

        case APP_STATE_CHECK_CDC_READ:

            if(APP_StateReset())
            {
                break;
            }

            /* If CDC read is complete, send the received data to the UART. */
            if(appData.isCDCReadComplete == true)
            {
                if(true == USART1_Write(&appData.readBuffer[0], appData.readLength))
				{
					appData.isCDCReadComplete = false;

					appData.isUSARTWriteComplete = false;

                    APP_LED0_Toggle();
                    APP_LED1_Toggle();

                    /* This means we have sent all the data. We schedule the next
                     * CDC Read. */
                    USB_DEVICE_CDC_Read (appData.cdcInstance, &appData.readTransferHandle,
                        appData.readBuffer, APP_READ_BUFFER_SIZE);

                    appData.state = APP_STATE_CHECK_UART_RECEIVE;

				}
            }
            else
            {
                /* We did not get any data from CDC. Check if any data was
                 * received from the UART. */
                appData.state = APP_STATE_CHECK_UART_RECEIVE;
            }
            break;

        case APP_STATE_CHECK_UART_RECEIVE:

            if(APP_StateReset())
            {
                break;
            }

            /* Check if a character was received on the UART */
            if(appData.isUSARTReadComplete == true)
            {
                APP_LED0_Toggle();
                APP_LED1_Toggle();

                USB_DEVICE_CDC_Write(0, &appData.writeTransferHandle,
                        &appData.uartReceivedData, 1,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);

				appData.isUSARTReadComplete = false;

                USART1_Read(&appData.uartReceivedData, 1);

            }
			else
			{
				appData.state = APP_STATE_CHECK_CDC_READ;
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
