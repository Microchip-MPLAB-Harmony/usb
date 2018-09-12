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

/* Receive data buffer */
uint8_t receiveDataBuffer[64] __attribute__ ((aligned(16)));

/* Transmit data buffer */
uint8_t  transmitDataBuffer[64] __attribute__ ((aligned(16)));

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

/**************************************************************
 * USB Device HID Events - Application Event Handler.
 **************************************************************/

static void APP_USBDeviceHIDEventHandler
(
    USB_DEVICE_HID_INDEX hidInstance,
    USB_DEVICE_HID_EVENT event, 
    void * eventData, 
    uintptr_t userData
)
{
    APP_DATA * appData = (APP_DATA *)userData;

    switch(event)
    {
        case USB_DEVICE_HID_EVENT_REPORT_SENT:

            /* This means a Report has been sent.  We are free to send next
             * report. An application flag can be updated here. */
            {
                USB_DEVICE_HID_EVENT_DATA_REPORT_SENT * report = 
                        (USB_DEVICE_HID_EVENT_DATA_REPORT_SENT *)eventData;
                if(report->handle == appData->txTransferHandle )
                {
                    // Transfer progressed.
                    appData->hidDataTransmitted = true;
                }
            }
            break;

        case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:

            /* This means Report has been received from the Host. Report
             * received can be over Interrupt OUT or Control endpoint based on
             * Interrupt OUT endpoint availability. An application flag can be
             * updated here. */
            {
                USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED * report = 
                        (USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED *)eventData;
                if(report->handle == appData->rxTransferHandle )
                {
                    // Transfer progressed.
                    appData->hidDataReceived = true;
                }
            }
            break;


        case USB_DEVICE_HID_EVENT_SET_IDLE:

            /* Save Idle rate received from Host */
            appData->idleRate = ((USB_DEVICE_HID_EVENT_DATA_SET_IDLE*)eventData)->duration;

            /* Acknowledge the Control Write Transfer */
            USB_DEVICE_ControlStatus(appData->handleUsbDevice, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_HID_EVENT_GET_IDLE:

            /* Host is requesting for Idle rate. Now send the Idle rate */
            USB_DEVICE_ControlSend(appData->handleUsbDevice, &(appData->idleRate),1);

            /* On successfully receiving Idle rate, the Host would acknowledge
             * back with a Zero Length packet. The HID function driver returns
             * an event USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the
             * application upon receiving this Zero Length packet from Host.
             * USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates
             * this control transfer event is complete */

            break;

        case USB_DEVICE_HID_EVENT_SET_PROTOCOL:

            /* Host is trying set protocol. Now receive the protocol and save */
            appData->activeProtocol = *(USB_HID_PROTOCOL_CODE *)eventData;

            /* Acknowledge the Control Write Transfer */
            USB_DEVICE_ControlStatus(appData->handleUsbDevice, USB_DEVICE_CONTROL_STATUS_OK);
            
            break;

        case  USB_DEVICE_HID_EVENT_GET_PROTOCOL:

            /* Host is requesting for Current Protocol. Now send the Idle rate */
             USB_DEVICE_ControlSend(appData->handleUsbDevice, &(appData->activeProtocol), 1);

             /* On successfully receiving Idle rate, the Host would acknowledge
             * back with a Zero Length packet. The HID function driver returns
             * an event USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the
             * application upon receiving this Zero Length packet from Host.
             * USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates
             * this control transfer event is complete */

            break;

        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This event occurs when the data stage of a control read transfer
             * has completed. This happens after the application uses the
             * USB_DEVICE_ControlSend function to respond to a HID Function
             * Driver Control Transfer Event that requires data to be sent to
             * the host. The pData parameter will be NULL */
            
            break;

        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* This event occurs when the data stage of a control write transfer
             * has completed. This happens after the application uses the
             * USB_DEVICE_ControlReceive function to respond to a HID Function
             * Driver Control Transfer Event that requires data to be received
             * from the host. */
            
            break;
        
        case USB_DEVICE_HID_EVENT_GET_REPORT:

            /* This event occurs when the host issues a GET REPORT command. */
            
            break;

        case USB_DEVICE_HID_EVENT_SET_REPORT:

            /* This event occurs when the host issues a SET REPORT command */
            
            break;

        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_ABORTED:

            /* This event occurs when an ongoing control transfer was aborted.
             * The application must stop any pending control transfer related
             * activities. */
            
            break;
			
        default:

            break;
    }
}

/******************************************************
 * Application USB Device Layer Event Handler
 ******************************************************/

static void APP_USBDeviceEventHandler
(
    USB_DEVICE_EVENT event, 
    void * eventData, 
    uintptr_t context
)
{
    USB_DEVICE_EVENT_DATA_CONFIGURED * configurationValue;
    
    switch(event)
    {
        case USB_DEVICE_EVENT_SOF:
            
            break;
            
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:
        
            /* Device got deconfigured */
            appData.usbDeviceIsConfigured = false;

            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Device is configured */
            configurationValue = (USB_DEVICE_EVENT_DATA_CONFIGURED *)eventData;
            if(configurationValue->configurationValue == 1)
            {
                /* Register the Application HID Event Handler. */
                USB_DEVICE_HID_EventHandlerSet(USB_DEVICE_HID_INDEX_0, APP_USBDeviceHIDEventHandler, (uintptr_t)&appData);
                
                appData.usbDeviceIsConfigured = true;
            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS was detected. We can attach the device */
            USB_DEVICE_Attach(appData.handleUsbDevice);
            
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS is not available any more. Detach the device. */
            USB_DEVICE_Detach(appData.handleUsbDevice);
            appData.usbDeviceIsConfigured = false;      
            
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            
            break;

        case USB_DEVICE_EVENT_CONTROL_TRANSFER_ABORTED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            
            break;

    } 
}

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/******************************************************************************
  Function:
    static void USB_Task (void)
    
   Remarks:
    Services the USB task. 
*/
static void USB_Task (void)
{
    if(appData.usbDeviceIsConfigured)
    {
        /* Write USB HID Application Logic here. Note that this function is
         * being called periodically the APP_Tasks() function. The application
         * logic should be implemented as state machine. It should not block */
        
        switch (appData.stateUSB)
        {
            case USB_STATE_INIT:

                appData.hidDataTransmitted = true;
                appData.txTransferHandle = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
                appData.rxTransferHandle = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
                appData.stateUSB = USB_STATE_SCHEDULE_READ;

                break;
                
            case USB_STATE_SCHEDULE_READ:
                
                /* Place a new read request. */
                appData.hidDataReceived = false;
                USB_DEVICE_HID_ReportReceive (USB_DEVICE_HID_INDEX_0,
                    &appData.rxTransferHandle, appData.receiveDataBuffer, 64 );
                appData.stateUSB = USB_STATE_WAITING_FOR_DATA;
                break;                
                
            case USB_STATE_WAITING_FOR_DATA:
                
                if( appData.hidDataReceived )
                {
                    if (appData.receiveDataBuffer[0]==0x80)
                    {
                        LED_Toggle();
                        appData.stateUSB = USB_STATE_SCHEDULE_READ;
                    }
                    else if (appData.receiveDataBuffer[0]==0x81)
                    {
                        appData.stateUSB = USB_STATE_SEND_REPORT;
                    }
                    else
                    {
                        appData.stateUSB = USB_STATE_SCHEDULE_READ;
                    }
                }
                break;

            case USB_STATE_SEND_REPORT:
                
                if(appData.hidDataTransmitted)
                {
                    appData.transmitDataBuffer[0] = 0x81;
                    if( SWITCH_Get() == SWITCH_STATE_PRESSED )
                    {
                        appData.transmitDataBuffer[1] = 0x00;
                    }
                    else
                    {
                        appData.transmitDataBuffer[1] = 0x01;
                    }
                    appData.hidDataTransmitted = false;
                    USB_DEVICE_HID_ReportSend (USB_DEVICE_HID_INDEX_0,
                        &appData.txTransferHandle, appData.transmitDataBuffer, 64 );
                    appData.stateUSB = USB_STATE_SCHEDULE_READ;
                }
                break;
        }                
    }
    else
    {
        appData.stateUSB = USB_STATE_INIT;
    }
}

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
    appData.state = APP_STATE_INIT;


    {
        /* Initialize USB Mouse Device application data */
        appData.handleUsbDevice       = USB_DEVICE_HANDLE_INVALID;
        appData.usbDeviceIsConfigured = false;
        appData.idleRate              = 0;
        appData.receiveDataBuffer = &receiveDataBuffer[0];
        appData.transmitDataBuffer = &transmitDataBuffer[0];
        appData.stateUSB = USB_STATE_INIT;
    }
    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
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
       

            /* Open the device layer */
            if (appData.handleUsbDevice == USB_DEVICE_HANDLE_INVALID)
            {
                appData.handleUsbDevice = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );
                if(appData.handleUsbDevice != USB_DEVICE_HANDLE_INVALID)
                {
                    appInitialized = true;
                }
                else
                {
                    appInitialized = false;
                }
            }
        
            if (appInitialized)
            {

                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.handleUsbDevice,APP_USBDeviceEventHandler, 0);
            
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            USB_Task();
        
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
