/*******************************************************************************
  USB Device Driver Core Routines

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_udphs.c

  Summary:
    USB Device Driver Dynamic Implementation of Core routines

  Description:
    The USB device driver provides a simple interface to manage the USB
    modules on Microchip microcontrollers.  This file Implements the core
    interface routines for the USB driver. While building the driver from
    source, ALWAYS use this file in the build.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include "driver/usb/udphs/src/drv_usb_udphs_local.h"

/*******************************************************************************
 * Driver instance object
 ******************************************************************************/

DRV_USB_UDPHS_OBJ gDrvUSBUDPHSObj[DRV_USB_UDPHS_INSTANCES_NUMBER];

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_USB_UDPHS_Initialize
    (
       const SYS_MODULE_INDEX index,
       const SYS_MODULE_INIT * const init
    )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_Initialize system interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_Initialize system interface
    function. Function performs the following task:
    - Initializes the necessary USB module as per the instance init data
    - Updates internal data structure for the particular USB instance
    - Returns the USB instance value as a handle to the system

  Remarks:
    See drv_usb_udphs.h for usage information.
*/

SYS_MODULE_OBJ DRV_USB_UDPHS_Initialize
(
    const SYS_MODULE_INDEX  drvIndex,
    const SYS_MODULE_INIT * const init
)
{

    DRV_USB_UDPHS_OBJ * drvObj;
    DRV_USB_UDPHS_INIT * usbInit;
    SYS_MODULE_OBJ retVal = SYS_MODULE_OBJ_INVALID;

    if(drvIndex >= DRV_USB_UDPHS_INSTANCES_NUMBER)
    {
        /* The driver module index specified does not exist in the system */
        SYS_DEBUG(SYS_ERROR_INFO,"\r\nDRV USB UDPHS: Invalid Driver Module Index in DRV_USB_UDPHS_Initialize().");
    }
    else if(gDrvUSBUDPHSObj[drvIndex].inUse == true)
    {
        /* Cannot initialize an object that is already in use. */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nDRV USB UDPHS: Driver is already initialized in DRV_USB_UDPHS_Initialize().");
    }
    else
    {
        usbInit = (DRV_USB_UDPHS_INIT *) init;
        drvObj = &gDrvUSBUDPHSObj[drvIndex];

        /* Create the global mutex and proceed if successful. */
        if(OSAL_RESULT_TRUE == OSAL_MUTEX_Create((OSAL_MUTEX_HANDLE_TYPE *)&drvObj->mutexID))
        {
            /* Populate the driver object with the required data */
            drvObj->inUse = true;
            drvObj->status = SYS_STATUS_BUSY;
            drvObj->usbID = usbInit->usbID;
            drvObj->interruptSource = (INT_SOURCE)usbInit->interruptSource;
            drvObj->isOpened = false;
            drvObj->pEventCallBack = NULL;

            /* Set the starting VBUS level. */
            drvObj->vbusLevel = DRV_USB_VBUS_LEVEL_INVALID;
            drvObj->vbusComparator = usbInit->vbusComparator;
            drvObj->sessionInvalidEventSent = false;
            drvObj->operationSpeed = usbInit->operationSpeed;

            _DRV_USB_UDPHS_DEVICE_INIT(drvObj, drvIndex);

            drvObj->state = DRV_USB_UDPHS_TASK_STATE_INITIALIZE_OPERATION_MODE;
            retVal = (SYS_MODULE_OBJ)drvIndex;
        }
        else
        {
            /* Mutex create failed */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nDRV USB USBFSV1: Mutex create failed in DRV_USBFSV1_Initialize().");
        }

    }
    return (retVal);

} /* end of DRV_USB_UDPHS_Initialize() */


// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_USBHS_Handler(void)

  Summary:
    USB interrupt handler.

  Description:
    USB interrupt handler.

  Remarks:
    See drv_usbfsv1.h for usage information.
*/
void DRV_USB_UDPHS_Handler(void)
{
    DRV_USB_UDPHS_Tasks_ISR(sysObj.drvUSBUDPHSObject);
}

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_Tasks(SYS_MODULE_OBJ object)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_Tasks system interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_Tasks system interface
    function.

  Remarks:
    See drv_usb_udphs.h for usage information.
*/

void DRV_USB_UDPHS_Tasks
(
	SYS_MODULE_OBJ object
)

{
    DRV_USB_UDPHS_OBJ * hDriver;
    DRV_USB_VBUS_LEVEL vbusLevel = DRV_USB_VBUS_LEVEL_INVALID;

    if(object >= DRV_USB_UDPHS_INSTANCES_NUMBER)
    {
        /* The object is invalid */
        SYS_DEBUG(SYS_ERROR_INFO,"\r\nDRV USB UDPHS: Invalid Object Index in in DRV_USB_UDPHS_Tasks().");
    }
    else if(gDrvUSBUDPHSObj[object].status <= SYS_STATUS_UNINITIALIZED)
    {
        /* Driver is not yet initialized */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nDRV USB UDPHS: Driver is not yet initialized in DRV_USB_UDPHS_Tasks().");
    }
    else
    {
        hDriver = &gDrvUSBUDPHSObj[object];

        /* Check the tasks state and maintain */
        switch(hDriver->state)
        {
            case DRV_USB_UDPHS_TASK_STATE_WAIT_FOR_CLOCK_USABLE:

                hDriver->state = DRV_USB_UDPHS_TASK_STATE_RUNNING;

                break;

            case DRV_USB_UDPHS_TASK_STATE_INITIALIZE_OPERATION_MODE:

                hDriver->state = DRV_USB_UDPHS_TASK_STATE_RUNNING;

                break;

            case DRV_USB_UDPHS_TASK_STATE_RUNNING:

                /* The module is in a running state. We check for the
                 * VBUS level and generate events if a client
                 * event handler is registered. */

                if(hDriver->pEventCallBack != NULL)
                {
                    /* We have a valid client call back function. Check if
                     * VBUS level has changed */

                    if( hDriver->vbusComparator != NULL)
                    {
                        vbusLevel = hDriver->vbusComparator();
                    }
                    else
                    {
                        vbusLevel = DRV_USB_VBUS_LEVEL_VALID;
                    }

                    if(hDriver->vbusLevel != vbusLevel)
                    {
                        /* This means there was a change in the level */
                        if(vbusLevel == DRV_USB_VBUS_LEVEL_VALID)
                        {
                            /* We have a valid VBUS level */
                            hDriver->pEventCallBack(hDriver->hClientArg, (DRV_USB_EVENT)DRV_USB_UDPHS_EVENT_DEVICE_SESSION_VALID, NULL);

                            /* We should be ready for send session invalid event
                             * to the application when they happen.*/
                            hDriver->sessionInvalidEventSent = false;

                        }
                        else
                        {
                            /* Any thing other than valid is considered invalid.
                             * This event may occur multiple times, but we send
                             * it only once. */
                            if(!hDriver->sessionInvalidEventSent)
                            {
                                hDriver->pEventCallBack(hDriver->hClientArg, (DRV_USB_EVENT)DRV_USB_UDPHS_EVENT_DEVICE_SESSION_INVALID, NULL);
                                hDriver->sessionInvalidEventSent = true;
                            }
                        }

                        hDriver->vbusLevel = vbusLevel;
                    }
                }

                break;

            default:
                break;
        }
    }


}/* end of DRV_USB_UDPHS_Tasks() */
// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_Deinitialize( const SYS_MODULE_OBJ object )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_Deinitialize system interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_Deinitialize
    system interface function.

  Remarks:
    See drv_usb_udphs.h for usage information.
*/

void DRV_USB_UDPHS_Deinitialize
(
    const SYS_MODULE_INDEX  object
)
{
    DRV_USB_UDPHS_OBJ * drvObj;

    if(object == (SYS_MODULE_INDEX)SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid object */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSB UDPHS Driver: Invalid object in DRV_USB_UDPHS_Deinitialize()");
    }
    else
    {
        if( object >= DRV_USB_UDPHS_INSTANCES_NUMBER)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSB UDPHS Driver: Invalid object in DRV_USB_UDPHS_Deinitialize()");
        }
        else
        {
            if(gDrvUSBUDPHSObj[object].inUse == false)
            {
                /* Cannot de-initialize an object that is not already in use. */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSB UDPHS Driver: Driver not initialized in DRV_USB_UDPHS_Deinitialize()");
            }
            else
            {
                drvObj = &gDrvUSBUDPHSObj[object];

                /* Populate the driver object with the required data */

                drvObj->inUse   = false;
                drvObj->status  = SYS_STATUS_UNINITIALIZED;

                /* Clear and disable the interrupts. Assigning to a value has
                 * been implemented to remove compiler warning in polling mode.
                   */

                drvObj->usbID->UDPHS_CTRL = UDPHS_CTRL_EN_UDPHS_Msk;

                SYS_INT_SourceDisable(drvObj->interruptSource);
                SYS_INT_SourceStatusClear(drvObj->interruptSource);

                drvObj->isOpened = false;
                drvObj->pEventCallBack = NULL;

                /* Delete the mutex */
                if(OSAL_MUTEX_Delete(&drvObj->mutexID) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSB UDPHS Driver: Could not delete mutex in DRV_USB_UDPHS_Deinitialize()");
                }
            }
        }
    }

} /* end of DRV_USB_UDPHS_Deinitialize() */

// *****************************************************************************
/* Function:
    SYS_STATUS DRV_USB_UDPHS_Status( const SYS_MODULE_OBJ object )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_Status system interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_Status system interface
    function.

  Remarks:
    See drv_usb_udphs.h for usage information.
*/

SYS_STATUS DRV_USB_UDPHS_Status ( SYS_MODULE_OBJ object )
{
    SYS_STATUS retVal = SYS_STATUS_ERROR;

    if(object == SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid System Object Module */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: System Module Object is invalid in DRV_USB_UDPHS_Status().");
    }
    else
    {
        retVal = gDrvUSBUDPHSObj[object].status;
    }
    return (retVal);

}/* end of DRV_USB_UDPHS_Status() */


// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_USB_UDPHS_Open
	(
		const SYS_MODULE_INDEX iDriver,
    	const DRV_IO_INTENT ioIntent
	)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_Open system interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_Open system interface
    function.

  Remarks:
    See drv_usb_udphs.h for usage information.
*/

DRV_HANDLE DRV_USB_UDPHS_Open
(
    const SYS_MODULE_INDEX iDriver,
    const DRV_IO_INTENT    ioIntent
)
{
    DRV_USB_UDPHS_OBJ * drvObj;
    DRV_HANDLE retVal = DRV_HANDLE_INVALID;

    if(iDriver >= DRV_USB_UDPHS_INSTANCES_NUMBER)
    {
        /* Invalid Driver Index number */
        SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB UDPHS Driver: Bad Driver Index in DRV_USB_UDPHS_Open().");
    }
    else if(gDrvUSBUDPHSObj[iDriver].status != SYS_STATUS_READY)
    {
        /* Invalid Driver Object Status */
        SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB UDPHS Driver: Invalid Driver Object Status in DRV_USB_UDPHS_Open().");
    }
    else if(ioIntent != (DRV_IO_INTENT_EXCLUSIVE | DRV_IO_INTENT_NONBLOCKING | DRV_IO_INTENT_READWRITE))
    {
        /* The driver supports only this mode */
        SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB UDPHS Driver: Unsupported IO Intent in DRV_USB_UDPHS_Open().");
    }
    else
    {
        drvObj = &gDrvUSBUDPHSObj[iDriver];
        if(drvObj->isOpened == true)
        {
            /* Driver supports exclusive open only */
            SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB UDPHS Driver: Driver can be opened only once. Multiple calls to DRV_USB_UDPHS_Open().");
        }
        else
        {
            /* Clear prior value */
            drvObj->pEventCallBack = NULL;

            /* Store the handle in the driver object client table and update
             * the number of clients*/
            drvObj->isOpened = true;
            retVal = ((DRV_HANDLE)drvObj);
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Driver opened successfully in DRV_USB_UDPHS_Open().");
        }
    }

    return (retVal);

}/* end of DRV_USB_UDPHS_Open() */

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_Close( DRV_HANDLE client )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_Close client interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_Close client interface
    function.

  Remarks:
    See drv_usb_udphs.h for usage information.
*/

void DRV_USB_UDPHS_Close( DRV_HANDLE client )
{
    DRV_USB_UDPHS_OBJ * hDriver;

    if(client == DRV_HANDLE_INVALID)
    {
        /* Invalid Client Handle */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Client Handle in DRV_USB_UDPHS_Close().");
    }
    else
    {
        hDriver = (DRV_USB_UDPHS_OBJ *) client;

        if(hDriver->isOpened != true)
        {
            /* Driver Object is not open - Cannot close an object that is not open */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Driver Object is not open in DRV_USB_UDPHS_Close().");
        }
        else
        {
            /* Give back the client */
            hDriver->isOpened = false;
            hDriver->pEventCallBack = NULL;
        }
    }

}/* end of DRV_USB_UDPHS_Close() */


// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_USB_UDPHS_Tasks_ISR( SYS_MODULE_OBJ object )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_Tasks_ISR system interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_Tasks_ISR system interface
    function.

  Remarks:
    See drv_usb_udphs.h for usage information.
*/

void DRV_USB_UDPHS_Tasks_ISR( SYS_MODULE_OBJ object )
{
    DRV_USB_UDPHS_OBJ * hDriver;

    if(object == SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid System Object Module */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: System Module Object is invalid in DRV_USB_UDPHS_Tasks_ISR().");
    }
    else
    {
        hDriver = &gDrvUSBUDPHSObj[object];
        hDriver->isInInterruptContext = true;

        _DRV_USB_UDPHS_DEVICE_TASKS_ISR(hDriver);

        SYS_INT_SourceStatusClear(hDriver->interruptSource);
        hDriver->isInInterruptContext = false;
    }

}/* end of DRV_USB_UDPHS_Tasks_ISR() */

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_ClientEventCallBackSet
    (
        DRV_HANDLE   handle,
        uintptr_t    hReferenceData,
        DRV_USB_UDPHS_EVENT_CALLBACK eventCallBack
    )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_ClientEventCallBackSet client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_ClientEventCallBackSet
    client interface function.

  Remarks:
    See drv_usb_udphs.h for usage information.
*/

void DRV_USB_UDPHS_ClientEventCallBackSet
(
    DRV_HANDLE   client,
    uintptr_t    hReferenceData,
    DRV_USB_EVENT_CALLBACK eventCallBack
)
{
    DRV_USB_UDPHS_OBJ * pusbDrvObj;

    /* Check if the handle is valid */
    if(client == DRV_HANDLE_INVALID)
    {
        /* Driver handle is invalid */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_ClientEventCallBackSet().");
    }
    else
    {
        pusbDrvObj = (DRV_USB_UDPHS_OBJ *) client;

        if(pusbDrvObj->isOpened != true)
        {
            /* Cannot set callback for a driver object that is not opened */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Driver handle not open in DRV_USB_UDPHS_ClientEventCallBackSet().");
        }
        else
        {
            /* Assign event call back and reference data */
            pusbDrvObj->hClientArg = hReferenceData;
            pusbDrvObj->pEventCallBack = eventCallBack;
        }
    }

}/* end of DRV_USB_UDPHS_ClientEventCallBackSet() */
