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

/* Data structure for this application */
APP_DATA appData;

/* Mouse IN Report data that will be transmitted when USB is in Device mode */
MOUSE_REPORT mouseReport USB_ALIGN;
MOUSE_REPORT mouseReportPrevious USB_ALIGN;

/* This is the string that will written to the file when USB is in Host mode */
 const uint8_t writeData[12]  __attribute__((aligned(16))) = "Hello World ";


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void APP_USBDeviceHIDEventHandler
(
    USB_DEVICE_HID_INDEX hidInstance,
    USB_DEVICE_HID_EVENT event,
    void * eventData,
    uintptr_t userData
)
{
    /* Start of local variables */
    APP_DATA * appDataObject = (APP_DATA *)userData;
    /* End of local variables */

    switch(event)
    {
        case USB_DEVICE_HID_EVENT_REPORT_SENT:

            /* This means the mouse report was sent.
             We are free to send another report */

            appDataObject->isMouseReportSendBusy = false;
            break;

        case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:

            /* Don't care for other event in this demo */
            break;

        case USB_DEVICE_HID_EVENT_SET_IDLE:

             /* Acknowledge the Control Write Transfer */
           USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            /* save Idle rate received from Host */
            appDataObject->idleRate = ((USB_DEVICE_HID_EVENT_DATA_SET_IDLE*)eventData)->duration;
            break;

        case USB_DEVICE_HID_EVENT_GET_IDLE:

            /* Host is requesting for Idle rate. Now send the Idle rate */
            USB_DEVICE_ControlSend(appDataObject->deviceHandle, &(appDataObject->idleRate),1);

            /* On successfully receiving Idle rate, the Host would acknowledge
               back with a Zero Length packet. The HID function driver returns
               an event USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the
               application upon receiving this Zero Length packet from Host.
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates
               this control transfer event is complete */

            break;

        case USB_DEVICE_HID_EVENT_SET_PROTOCOL:
            /* Host is trying set protocol. Now receive the protocol and save */
            appDataObject->activeProtocol = *(USB_HID_PROTOCOL_CODE *)eventData;

              /* Acknowledge the Control Write Transfer */
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case  USB_DEVICE_HID_EVENT_GET_PROTOCOL:

            /* Host is requesting for Current Protocol. Now send the Idle rate */
             USB_DEVICE_ControlSend(appDataObject->deviceHandle, &(appDataObject->activeProtocol), 1);

             /* On successfully receiving Idle rate, the Host would acknowledge
               back with a Zero Length packet. The HID function driver returns
               an event USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the
               application upon receiving this Zero Length packet from Host.
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates
               this control transfer event is complete */
             break;

        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT:
            break;

        default:
            break;
    }
}

/*******************************************************************************
  Function:
    void APP_USBDeviceEventHandler (USB_DEVICE_EVENT event,
        USB_DEVICE_EVENT_DATA * eventData)

  Summary:
    Event callback generated by USB device layer.

  Description:
    This event handler will handle all device layer events.

  Parameters:
    None.

  Returns:
    None.
*/

void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, void * eventData, uintptr_t context)
{
    USB_DEVICE_EVENT_DATA_CONFIGURED * configurationValue;
    switch(event)
    {
        case USB_DEVICE_EVENT_SOF:
            /* This event is used for switch debounce. This flag is reset
             * by the switch process routine. */
            appData.sofEventHasOccurred = true;
            appData.setIdleTimer++;
            break;
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:
        
            /* Device got deconfigured */
            
            appData.isConfigured = false;
            appData.isMouseReportSendBusy = false;
            appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            appData.emulateMouse = true;
            LED1_Off ( );
            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Device is configured */
            configurationValue = (USB_DEVICE_EVENT_DATA_CONFIGURED *)eventData;
            if(configurationValue->configurationValue == 1)
            {
                appData.isConfigured = true;
                
                LED1_On (  );
              
                /* Register the Application HID Event Handler. */

                USB_DEVICE_HID_EventHandlerSet(appData.hidInstance,
                        APP_USBDeviceHIDEventHandler, (uintptr_t)&appData);
            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:
            appData.powerDetected = true;
           
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            appData.powerDetected = false;
            LED1_Off (  );
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            LED1_Off (  );
            break;

        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;

    } 
}

USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        default:
            break;
                    
    }
    
    return(USB_HOST_EVENT_RESPONSE_NONE);
}

void APP_SYSFSEventHandler(SYS_FS_EVENT event, void * eventData, uintptr_t context)
{
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:
            appData.deviceIsConnected = true;
            break;
            
        case SYS_FS_EVENT_UNMOUNT:
            appData.deviceIsConnected = false;
            
            break;
            
        default:
            break;
    }
}



// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/********************************************************
 * Application switch press routine
 ********************************************************/

void APP_ProcessSwitchPress(void)
{
    /* This function checks if the switch is pressed and then
     * debounces the switch press*/
    if(SWITCH1_STATE_PRESSED == (SWITCH1_Get()))
    {
        if(appData.ignoreSwitchPress)
        {
            /* This means the key press is in progress */
            if(appData.sofEventHasOccurred)
            {
                /* A timer event has occurred. Update the debounce timer */
                appData.switchDebounceTimer ++;
                appData.sofEventHasOccurred = false;
                if(appData.switchDebounceTimer == APP_USB_SWITCH_DEBOUNCE_COUNT)
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
    if(SWITCH2_STATE_PRESSED == (SWITCH2_Get()))
    {
        appData.isSwitch2Pressed = true;
        if (( appData.currentRole == APP_NONE ) || ( appData.currentRole == APP_HOST ))
        {
            appData.roleSwitch = true;
            appData.currentRole = APP_DEVICE ;
            USB_HOST_BusDisable(0);
            appData.state = APP_STATE_SELECT_USB_ROLE;
        }
        
        
    }
    
    if(SWITCH3_STATE_PRESSED == (SWITCH3_Get()))
    {
        appData.isSwitch3Pressed = true;
        if (( appData.currentRole == APP_NONE ) || ( appData.currentRole == APP_DEVICE ))
        {
            appData.roleSwitch = true;
            appData.currentRole = APP_HOST ;
            appData.state = APP_STATE_SELECT_USB_ROLE;
        }
        
        
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
    appData.state = APP_STATE_INIT;
    
    appData.deviceHandle  = USB_DEVICE_HANDLE_INVALID;
    appData.isConfigured = false;
    appData.emulateMouse = true;
    appData.hidInstance = 0;
    appData.isMouseReportSendBusy = false;
    appData.isSwitchPressed = false;
    appData.ignoreSwitchPress = false;
    appData.deviceIsConnected = false;
    appData.currentRole = APP_NONE ;
    appData.roleSwitch = false;
    appData.deviceIsConnected = false;
    appData.isSwitch2Pressed = false;
    appData.isSwitch3Pressed = false;
    appData.powerDetected = false;
    
}

int8_t dir_table[] ={-4,-4,-4, 0, 4, 4, 4, 0};
/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    static int8_t   vector = 0;
    static uint8_t  movement_length = 0;
    static bool     sent_dont_move = false;

    if(appData.state > APP_STATE_INIT)
    {
        APP_ProcessSwitchPress();
    }
    
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            SYS_FS_EventHandlerSet(APP_SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            
		    /* Open the device layer */
            appData.deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0,
                    DRV_IO_INTENT_READWRITE );

            if(appData.deviceHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceHandle,
                        APP_USBDeviceEventHandler, 0);

                appData.state = APP_STATE_SELECT_USB_ROLE;
            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }

            break;
        }
        
        case APP_STATE_SELECT_USB_ROLE:
            
            if(appData.roleSwitch)
            {
                LED1_Off (  );
                LED2_Off (  );
                LED3_Off (  );
                
                appData.isConfigured = false;
                appData.emulateMouse = true;
                appData.hidInstance = 0;
                appData.isMouseReportSendBusy = false;
                appData.isSwitchPressed = false;
                appData.ignoreSwitchPress = false;
                appData.deviceIsConnected = false;
                appData.deviceIsConnected = false;
 
                if(appData.currentRole == APP_HOST  )
                {
                    if  ( appData.isSwitch3Pressed == true  )
                    {
                        appData.state = APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
                        USB_HOST_BusEnable(0);
                        if ( appData.powerDetected == false )
                        {
                            USB_DEVICE_Detach(appData.deviceHandle);
                        }
                        appData.currentRole = APP_HOST ;
                        appData.isSwitch3Pressed = false;
                        appData.roleSwitch = false;
                    }
                    
                }
                if(appData.currentRole == APP_DEVICE  )
                {
                    if ( appData.isSwitch2Pressed == true  )
                    {
                        appData.state = APP_STATE_WAIT_FOR_BUS_DISABLE_COMPLETE;
                        USB_HOST_BusDisable(0);
                        appData.isSwitch2Pressed  = false ;
                        appData.currentRole = APP_DEVICE ;
                        appData.roleSwitch = false;
                    }
                }
            }
        break;
            
        case APP_STATE_WAIT_FOR_BUS_DISABLE_COMPLETE:
            
            if(USB_HOST_RESULT_TRUE == USB_HOST_BusIsDisabled(0))
            {
                appData.state = APP_STATE_WAIT_FOR_POWER_DETECT;
            }
            
            break;
            
        case APP_STATE_WAIT_FOR_POWER_DETECT :
            
            if ( appData.powerDetected == true )
            {
                /* VBUS was detected. We can attach the device */
                USB_DEVICE_Attach(appData.deviceHandle);
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            break;

        case APP_STATE_WAIT_FOR_CONFIGURATION:

            /* Check if the device is configured. The 
             * isConfigured flag is updated in the
             * Device Event Handler */

            if(appData.isConfigured)
            {
                appData.state = APP_STATE_MOUSE_EMULATE;
            }
            break;

        case APP_STATE_MOUSE_EMULATE:
            /* The following logic rotates the mouse icon when
             * a switch is pressed */

            if(appData.isSwitchPressed)
            {
                /* Toggle the mouse emulation with each switch press */
                appData.emulateMouse ^= 1;
                appData.isSwitchPressed = false;
            }

            if(appData.emulateMouse)
            {
                sent_dont_move = false;

                if(movement_length > 50)
                {
                    appData.mouseButton[0] = MOUSE_BUTTON_STATE_RELEASED;
                    appData.mouseButton[1] = MOUSE_BUTTON_STATE_RELEASED;
                    appData.xCoordinate =(int8_t)dir_table[vector & 0x07] ;
                    appData.yCoordinate =(int8_t)dir_table[(vector+2) & 0x07];
                    vector ++;
                    movement_length = 0;
                }
            }
            else
            { 
                appData.mouseButton[0] = MOUSE_BUTTON_STATE_RELEASED;
                appData.mouseButton[1] = MOUSE_BUTTON_STATE_RELEASED;
                appData.xCoordinate = 0;
                appData.yCoordinate = 0;
            }

            if(!appData.isMouseReportSendBusy)
            {
                if(((sent_dont_move == false) && (!appData.emulateMouse)) || (appData.emulateMouse))
                {

                    /* This means we can send the mouse report. The
                       isMouseReportBusy flag is updated in the HID Event Handler. */

                    appData.isMouseReportSendBusy = true;

                    /* Create the mouse report */

                    MOUSE_ReportCreate(appData.xCoordinate, appData.yCoordinate,
                            appData.mouseButton, &mouseReport);

                    if(memcmp((const void *)&mouseReportPrevious, (const void *)&mouseReport,
                            (size_t)sizeof(mouseReport)) == 0)
                    {
                        /* Reports are same as previous report. However mouse reports
                         * can be same as previous report as the co-ordinate positions are relative.
                         * In that case it needs to be send */
                        if((appData.xCoordinate == 0) && (appData.yCoordinate == 0))
                        {
                            /* If the coordinate positions are 0, that means there
                             * is no relative change */
                            if(appData.idleRate == 0)
                            {
                                appData.isMouseReportSendBusy = false;
                            }
                            else
                            {
                                /* Check the idle rate here. If idle rate time elapsed
                                 * then the data will be sent. Idle rate resolution is
                                 * 4 msec as per HID specification; possible range is
                                 * between 4msec >= idlerate <= 1020 msec.
                                 */
                                if(appData.setIdleTimer * APP_USB_CONVERT_TO_MILLISECOND
                                        >= appData.idleRate * 4)
                                {
                                    /* Send REPORT as idle time has elapsed */
                                    appData.isMouseReportSendBusy = true;
                                }
                                else
                                {
                                    /* Do not send REPORT as idle time has not elapsed */
                                    appData.isMouseReportSendBusy = false;
                                }
                            }
                        }

                    }
                    if(appData.isMouseReportSendBusy == true)
                    {
                        /* Copy the report sent to previous */
                        memcpy((void *)&mouseReportPrevious, (const void *)&mouseReport,
                                (size_t)sizeof(mouseReport));
                        
                        /* Send the mouse report. */
                        USB_DEVICE_HID_ReportSend(appData.hidInstance,
                            &appData.reportTransferHandle, (uint8_t*)&mouseReport,
                            sizeof(MOUSE_REPORT));
                        appData.setIdleTimer = 0;
                    }
                    movement_length ++;
                    sent_dont_move = true;
                }
            }

            break;
            
        case APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:

            if( USB_HOST_RESULT_TRUE == USB_HOST_BusIsEnabled(0) )
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;
        
        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:

            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            if(appData.deviceIsConnected)
            {
                LED3_On(  );
                LED2_Off(  );
                appData.state = APP_STATE_DEVICE_CONNECTED;
            }

            break;

        case APP_STATE_DEVICE_CONNECTED:

            /* Device was connected. We can try mounting the disk */
            appData.state = APP_STATE_OPEN_FILE;
            break;

        case APP_STATE_OPEN_FILE:

            /* Try opening the file for append */
            appData.fileHandle = SYS_FS_FileOpen("/mnt/myDrive1/file.txt", (SYS_FS_FILE_OPEN_APPEND_PLUS));
            if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                /* Could not open the file. Error out*/
                appData.state = APP_STATE_ERROR;

            }
            else
            {
                /* File opened successfully. Write to file */
                appData.state = APP_STATE_WRITE_TO_FILE;

            }
            break;

        case APP_STATE_WRITE_TO_FILE:

            /* Try writing to the file */
            if (SYS_FS_FileWrite( appData.fileHandle, (const void *) writeData, 12 ) == -1)
            {
                /* Write was not successful. Close the file
                 * and error out.*/
                SYS_FS_FileClose(appData.fileHandle);
                appData.state = APP_STATE_ERROR;

            }
            else
            {
                /* We are done writing. Close the file */
                appData.state = APP_STATE_CLOSE_FILE;
            }

            break;

        case APP_STATE_CLOSE_FILE:

            /* Close the file */
            SYS_FS_FileClose(appData.fileHandle);

            /* The test was successful. Lets idle. */
            appData.state = APP_STATE_IDLE;
            break;

        case APP_STATE_IDLE:

            /* The application comes here when the demo has completed
             * successfully. Provide LED indication. Wait for device detach
             * and if detached, wait for attach. */

            LED3_Off( );
            LED2_On(  );
            if(appData.deviceIsConnected == false)
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
                LED2_Off();
            }
            break;

        case APP_STATE_ERROR:

            break;

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

