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
*/

APP_DATA appData USB_ALIGN;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/


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

// *****************************************************************************
/* Function:
    void APP_Initialize(void)

  Summary:
    This function intialized the application data stucture.

  Description:
    This function intialized the application data stucture.

  Remarks:
    None.
*/    

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_BUS_ENABLE;
    appData.gUSBFirstDeviceConnected = false;
    appData.gUSBSecondDeviceConnected = false;
}

// *****************************************************************************
/* Function:
    USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler 
    (
        USB_HOST_EVENT event, 
        void * eventData, 
        uintptr_t context
    )

  Summary:
    This function processes USB Host Layer Events.

  Description:
    This function processes USB Host Layer Events.

  Remarks:
    None.
*/    

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

// *****************************************************************************
/* Function:
    void APP_SYSFSEventHandler
    (
        SYS_FS_EVENT event, 
        void *mountName, 
        uintptr_t context
    )

  Summary:
    This function processes File System Events.

  Description:
    This function processes File System Events.

  Remarks:
    None.
*/    

void APP_SYSFSEventHandler(SYS_FS_EVENT event, void *mountName, uintptr_t context)
{
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:

            if(0 == strcmp((const char *)mountName,"/mnt/myDrive1"))
            {
                appData.gUSBFirstDeviceConnected  = true;
            }

            else if(0 == strcmp((const char *)mountName,"/mnt/myDrive2"))
            {
                appData.gUSBSecondDeviceConnected = true;
            }
            break;

        case SYS_FS_EVENT_UNMOUNT:

            if(0 == strcmp((const char *)mountName,"/mnt/myDrive1"))
            {
                appData.gUSBFirstDeviceConnected  = false;
            }

            else if(0 == strcmp((const char *)mountName,"/mnt/myDrive2"))
            {
                appData.gUSBSecondDeviceConnected = false;
            }

            LED1_Off();
            break;

        default:
            break;
    }
}

// *****************************************************************************
/* Function:
    void APP_Tasks(void)

  Summary:
    This is the main application task.

  Description:
    This is the main application task.

  Remarks:
    None.
*/    

void APP_Tasks ( void )
{
   /* The application task state machine */

    if(appData.state > APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE)
    {
        /* If the bus has already been enabled, and if any of the devices are
         * not connected then wait for device attach. */

        if(!appData.gUSBFirstDeviceConnected || !appData.gUSBSecondDeviceConnected)
        {
            appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
        }
    }

    switch(appData.state)
    {
        case APP_STATE_BUS_ENABLE:
                      
           /* Set the event handler and enable the bus */
            SYS_FS_EventHandlerSet((void *)APP_SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_BusEnable(USB_HOST_BUS_ALL);
            appData.state = APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
            break;
            
        case APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:

            /* Wait for the bus enable to complete */
            if(USB_HOST_BusIsEnabled(USB_HOST_BUS_ALL))
            {
                appData.state = APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            }

            break;
       
        case APP_STATE_WAIT_FOR_DEVICE_ATTACH:

            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            
            if( appData.gUSBFirstDeviceConnected  &&  appData.gUSBSecondDeviceConnected)
            {
                appData.state = APP_STATE_OPEN_FILE;
            }

            break;

        case APP_STATE_OPEN_FILE:

            /* Check if file.txt is available on drive 1 */
            appData.fileHandle1 = SYS_FS_FileOpen("/mnt/myDrive1/file.txt", (SYS_FS_FILE_OPEN_READ));
            if(appData.fileHandle1 == SYS_FS_HANDLE_INVALID)
            {
                /* Its not available on drive 1. Check if it is available on drive 2 */
                appData.fileHandle1 = SYS_FS_FileOpen("/mnt/myDrive2/file.txt", (SYS_FS_FILE_OPEN_READ));
                if(appData.fileHandle2 == SYS_FS_HANDLE_INVALID)
                {
                    /* Not available on either driver. Error out */
                    appData.state = APP_STATE_ERROR;
                }
                else
                {
                    /* If file.txt was on drive 2, then newfile.txt should be on drive 1 */
                    appData.fileHandle2 = SYS_FS_FileOpen("/mnt/myDrive1/newfile.txt", (SYS_FS_FILE_OPEN_WRITE));

                    if(appData.fileHandle2 == SYS_FS_HANDLE_INVALID)
                    {
                        /* Could not open the file. Error out*/
                        appData.state = APP_STATE_ERROR;
                    }
                    else
                    {
                        /* Both files are open. Proceed to read and write */
                        appData.state = APP_STATE_READ_FROM_FILE;
                    }
                }
            }
            else
            {
                /* file.txt was available on drive 1. Open newfile.txt on drive 2. */
                appData.fileHandle2 = SYS_FS_FileOpen("/mnt/myDrive2/newfile.txt", (SYS_FS_FILE_OPEN_WRITE));
                if(appData.fileHandle2 == SYS_FS_HANDLE_INVALID)
                {
                    /* Could not open the file. Error out*/
                    appData.state = APP_STATE_ERROR;
                }
                else
                {
                    appData.state = APP_STATE_READ_FROM_FILE ;
                }
            }

            break;
            
        case APP_STATE_READ_FROM_FILE:
            
            appData.nBytesRead = SYS_FS_FileRead(appData.fileHandle1, (void *)appData.data, 512) ;
            if(appData.nBytesRead == -1)
            {
                /* Read was not successful. Close the file and error out.*/
                SYS_FS_FileClose(appData.fileHandle1);
                appData.state = APP_STATE_ERROR;
            }
            else
            {
                /* We could read data. Now write it */
                appData.state = APP_STATE_WRITE_TO_FILE;
            }
            
            break;

        case APP_STATE_WRITE_TO_FILE:

           if(SYS_FS_FileWrite(appData.fileHandle2, (const void *)appData.data, appData.nBytesRead) == -1)
            {
                /* There was an error while writing the file.
                 * Close the file and error out. */

                SYS_FS_FileClose(appData.fileHandle2);
                appData.state = APP_STATE_ERROR;
            }
            else
            {
               if(SYS_FS_FileEOF(appData.fileHandle1))
               {
                    /* Reached the end of file. Lets idle. */
                    appData.state = APP_STATE_CLOSE_FILE;
               }
               else
               {
                   /* File has more content. Continue to read */
                   appData.state = APP_STATE_READ_FROM_FILE;
               }
            }

            break;

        case APP_STATE_CLOSE_FILE:

            /* Close the file */
            SYS_FS_FileClose(appData.fileHandle1);
            SYS_FS_FileClose(appData.fileHandle2);

            /* The test was successful. Lets idle. */
            appData.state = APP_STATE_IDLE;
            break;

        case APP_STATE_IDLE:

            /* The application comes here when the demo has completed
             * successfully. Provide LED indication */
            LED1_On(); 
            
            break;

        case APP_STATE_ERROR:

            /* The application comes here when the demo has failed. Provide LED
             * indication. We wait for both devices to be disconnected and 
             * reconnected. */

            appData.gUSBSecondDeviceConnected = false;
            appData.gUSBFirstDeviceConnected = false;
            appData.state =  APP_STATE_WAIT_FOR_DEVICE_ATTACH;
            LED1_Off();

            break;

        default:
            break;
    }
}
 

/*******************************************************************************
 End of File
 */


