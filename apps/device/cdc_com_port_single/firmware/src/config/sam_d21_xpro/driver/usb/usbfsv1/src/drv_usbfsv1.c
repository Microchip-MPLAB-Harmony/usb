/*******************************************************************************
  USB Controller Driver Core Routines.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usbfsv1.c

  Summary:
    USB Controller Driver Core Routines intended for Dynamic implementation.

  Description:
    The USB Controller driver provides a simple interface to manage the USB
    modules on Microchip microcontrollers.  This file Implements the core
    interface routines to be used both by the client(USB Host or Device layer)
    and the system for communicating with USB Controller driver.  While building
    the driver from source, ALWAYS use this file in the build.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute Software
only when embedded on a Microchip microcontroller or digital  signal  controller
that is integrated into your product or third party  product  (pursuant  to  the
sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include "usb/src/usb_external_dependencies.h"
#include "driver/usb/usbfsv1/src/drv_usbfsv1_local.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/******************************************************
 * Hardware instance, endpoint table and client object
 * lumped together as group to save memory.
 ******************************************************/
DRV_USBFSV1_OBJ gDrvUSBObj [DRV_USBFSV1_INSTANCES_NUMBER];

/******************************************************
 * Control Endpoint IN/OUT buffers needed by the USB
 * controller
 ******************************************************/
COMPILER_WORD_ALIGNED uint8_t gDrvEP0BufferBank0[USB_DEVICE_EP0_BUFFER_SIZE];
COMPILER_WORD_ALIGNED uint8_t gDrvEP0BufferBank1[USB_DEVICE_EP0_BUFFER_SIZE];

// *****************************************************************************
// *****************************************************************************
// Section: USB Controller Driver Interface Implementations
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_USBFSV1_Initialize
    (
        const SYS_MODULE_INDEX index,
        const SYS_MODULE_INIT * const init
    )

  Summary:
    Initializes the USB Driver.

  Description:
    This function initializes the USB Driver, making it ready for clients to
    open. The driver initialization does not complete when this function
    returns. The DRV_USBFSV1_Tasks function must called periodically to complete
    the driver initialization. The DRV_USBHS_Open function will fail if the
    driver was not initialized or if initialization has not completed.

  Remarks:
    See drv_usbfsv1.h for usage information.
*/

SYS_MODULE_OBJ DRV_USBFSV1_Initialize
(
    const SYS_MODULE_INDEX  drvIndex,
    const SYS_MODULE_INIT * const init
)
{

    DRV_USBFSV1_OBJ * drvObj = (DRV_USBFSV1_OBJ *)NULL;
    DRV_USBFSV1_INIT * usbInit = (DRV_USBFSV1_INIT *)NULL;
    SYS_MODULE_OBJ retVal = SYS_MODULE_OBJ_INVALID;

	if(drvIndex >= DRV_USBFSV1_INSTANCES_NUMBER)
	{
        /* The driver module index specified does not exist in the system */
        SYS_DEBUG(SYS_ERROR_INFO,"\r\nDRV USB USBFSV1: Invalid Driver Module Index in DRV_USBFSV1_Initialize().");
	}
	else if(gDrvUSBObj[drvIndex].inUse == true)
    {
        /* Cannot initialize an object that is already in use. */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nDRV USB USBFSV1: Driver is already initialized in DRV_USBFSV1_Initialize().");
    }
    else
    {
        /* Assign to the local pointer the init data passed */
        usbInit = (DRV_USBFSV1_INIT *) init;
        drvObj = &gDrvUSBObj[drvIndex];

        /* Create the global mutex and proceed if successful. */   
        if(OSAL_RESULT_TRUE == OSAL_MUTEX_Create(&drvObj->mutexID))
        {            
            /* Populate the driver instance object with required data */
            drvObj->status = SYS_STATUS_BUSY;
            drvObj->state = DRV_USBFSV1_TASK_STATE_INITIALIZE_OPERATION_MODE;
            drvObj->usbID = usbInit->usbID;
            drvObj->operationMode = usbInit->operationMode;
            drvObj->isOpened = false;
            drvObj->pEventCallBack = NULL;

            /* Set the starting VBUS level. */
            drvObj->vbusLevel = DRV_USB_VBUS_LEVEL_INVALID;
            drvObj->vbusComparator = usbInit->vbusComparator;

            drvObj->sessionInvalidEventSent = false;
            drvObj->interruptSource  = usbInit->interruptSource;

            /* Assign the endpoint table */
            drvObj->endpointBufferPtr[0][0] = gDrvEP0BufferBank0;
            drvObj->endpointBufferPtr[0][1] = gDrvEP0BufferBank1;

            /* Set the configuration */
            if(drvObj->operationMode == DRV_USBFSV1_OPMODE_DEVICE)
            {
                drvObj->usbID->DEVICE.USB_CTRLA |= USB_CTRLA_MODE_DEVICE;
            }
            else if(drvObj->operationMode == DRV_USBFSV1_OPMODE_HOST)
            {
                drvObj->usbID->DEVICE.USB_CTRLA |= USB_CTRLA_MODE_HOST;
            }
            else
            {
                /* This can never happen */
            }

            drvObj->usbID->DEVICE.USB_CTRLA |= USB_CTRLA_RUNSTDBY_Msk;

            drvObj->usbID->DEVICE.USB_DESCADD = (uint32_t)(drvObj->endpointDescriptorTable);

            if (USB_SPEED_FULL == usbInit->operationSpeed)
            {
                drvObj->usbID->DEVICE.USB_CTRLB = USB_DEVICE_CTRLB_SPDCONF(USB_DEVICE_CTRLB_SPDCONF_FS_Val);
            }
            else if(USB_SPEED_LOW == usbInit->operationSpeed)
            {
                drvObj->usbID->DEVICE.USB_CTRLB = USB_DEVICE_CTRLB_SPDCONF(USB_DEVICE_CTRLB_SPDCONF_LS_Val);
            }

            /* Enable the USB device by clearing the . This function
             * also enables the D+ pull up resistor.  */
            drvObj->usbID->DEVICE.USB_CTRLA |= USB_CTRLA_ENABLE_Msk;

            while (drvObj->usbID->DEVICE.USB_SYNCBUSY == USB_SYNCBUSY_ENABLE_Msk);

            /* Enable interrupts for this USB module */
            SYS_INT_SourceEnable(drvObj->interruptSource);

            drvObj->status = SYS_STATUS_READY;
            retVal = drvIndex;
        }
        else
        {
            /* Mutex create failed */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nDRV USB USBFSV1: Mutex create failed in DRV_USBFSV1_Initialize().");
        }
	}

    return (retVal);
}

// *****************************************************************************
/* Function:
    void DRV_USBFSV1_Tasks
    (
        SYS_MODULE_OBJ object
    )

  Summary:
    Maintains the driver's state machine through.

  Description:
    Maintains the driver's state machine when the driver is configured for
    polled mode. This function should be called from the system tasks routine.

  Remarks:
    Refer to drv_usbfsv1.h for usage information.
*/

void DRV_USBFSV1_Tasks
(
    SYS_MODULE_OBJ object
)
{
	DRV_USBFSV1_OBJ * hDriver;
	DRV_USB_VBUS_LEVEL vbusLevel = DRV_USB_VBUS_LEVEL_INVALID;

    hDriver = &gDrvUSBObj[object];

	if(hDriver->status <= SYS_STATUS_UNINITIALIZED)
	{
		/* Driver is not initialized */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nDRV USB USBFSV1: Driver not yet initialized in DRV_USBFSV1_Tasks().");
	}
	else
	{
        /* Check the tasks state and maintain */
        switch(hDriver->state)
        {
            case DRV_USBFSV1_TASK_STATE_INITIALIZE_OPERATION_MODE:

                /* Setup the USB Module based on the selected
                * mode */

                switch(hDriver->operationMode)
                {
                    case DRV_USBFSV1_OPMODE_DEVICE:

                        /* Device mode specific driver initialization */
                        _DRV_USBFSV1_DEVICE_INIT(hDriver, object);
                    break;

                    case DRV_USBFSV1_OPMODE_HOST:

                        /* Host mode specific driver initialization */
                        _DRV_USBFSV1_HOST_INIT(hDriver, object);

                    break;

                    case DRV_USBFSV1_OPMODE_OTG:

                    break;

                    default:

                        SYS_DEBUG(SYS_ERROR_INFO, "\r\nDRV USB USBFSV1: Unsupported driver operation mode in DRV_USBFSV1_Tasks().");

                    break;
                }

                /* Clear and enable the interrupts */
                SYS_INT_SourceEnable(hDriver->interruptSource);
                SYS_INT_SourceStatusClear(hDriver->interruptSource);

                /* Indicate that the object is ready
                 * and change the state to running */

                hDriver->status = SYS_STATUS_READY;
                hDriver->state = DRV_USBFSV1_TASK_STATE_RUNNING;

                break;

            case DRV_USBFSV1_TASK_STATE_RUNNING:

                /* The module is in a running state. We check
                 * for the VBUS level and generate events if a client
                 * event handler is registered. */

                if(hDriver->pEventCallBack != NULL && hDriver->operationMode == DRV_USBFSV1_OPMODE_DEVICE)
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
                            hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBFSV1_EVENT_DEVICE_SESSION_VALID, NULL);

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
                                hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBFSV1_EVENT_DEVICE_SESSION_INVALID, NULL);
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
}

// *****************************************************************************
/* Function:
    void DRV_USBFSV1_Deinitialize( const SYS_MODULE_OBJ object )

  Summary:
    This function deinitializes the USBFSV1 driver instance.

  Description:
    This function deinitializes the USBFSV1 driver instance.

  Remarks:
    A typical USB application may not to called this function.
*/

void DRV_USBFSV1_Deinitialize
(
    const SYS_MODULE_OBJ  object
)
{
    DRV_USBFSV1_OBJ * hDriver = NULL;

    if((object != SYS_MODULE_OBJ_INVALID) && (object < DRV_USBFSV1_INSTANCES_NUMBER))
    {
        /* Object is valid */
        if(gDrvUSBObj[object].inUse == true)
        {
            hDriver = &gDrvUSBObj[object];

            /* Release the USB instance object */
            hDriver->inUse = false;

            /* Reset the open flag */
            hDriver->isOpened = false;

            /* Delete the mutex */
            OSAL_MUTEX_Delete(&hDriver->mutexID);

            /* De-initialize the status*/
            hDriver->status = SYS_STATUS_UNINITIALIZED;

            hDriver->pEventCallBack = NULL;

            /* Clear and disable the interrupts */
            SYS_INT_SourceDisable(hDriver->interruptSource);
            SYS_INT_SourceStatusClear(hDriver->interruptSource);

			/* Enable the USB device by clearing the . This function
			 * also enables the D+ pull up resistor.  */
			hDriver->usbID->DEVICE.USB_CTRLA &= ~USB_CTRLA_ENABLE_Msk;

			while (hDriver->usbID->DEVICE.USB_SYNCBUSY == USB_SYNCBUSY_ENABLE_Msk);

        }
        else
        {
            /* Cannot de-initialize an object that is not in use. */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBFSV1 Driver: Instance not in use");
        }
    }
    else
    {
        /* Invalid object */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBFSV1 Driver: Invalid System Module Object");
    }
}

// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_USBFSV1_Open
    (
        const SYS_MODULE_INDEX drvIndex,
        const DRV_IO_INTENT ioIntent
    )

  Summary:
    Opens the specified USB Driver instance and returns a handle to it.

  Description:
    This function opens the specified USB Driver instance and provides a handle
    that must be provided to all other client-level operations to identify the
    caller and the instance of the driver. The intent flag is ignored.  Any
    other setting of the intent flag will return a invalid driver handle. A
    driver instance can only support one client. Trying to open a driver that
    has an existing client will result in an unsuccessful function call.

  Remarks:
    See drv_usbfsv1.h for usage information.
*/

DRV_HANDLE DRV_USBFSV1_Open
(
    const SYS_MODULE_INDEX drvIndex,
    const DRV_IO_INTENT ioIntent
)
{
    DRV_USBFSV1_OBJ * drvObj;
    DRV_HANDLE retVal = DRV_HANDLE_INVALID;

    /* Check if the specified driver index is in valid range */
    if(drvIndex >= DRV_USBFSV1_INSTANCES_NUMBER)
    {
        SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB USBFSV1 Driver: Bad Driver Index in DRV_USBFSV1_Open().");
    }
    else
    {
        drvObj = &gDrvUSBObj[drvIndex];
        if(drvObj->status != SYS_STATUS_READY)
        {
            /* The driver status not ready */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB USBFSV1 Driver: Driver status not ready in DRV_USBFSV1_Open().");            
        }
        else if(ioIntent != (DRV_IO_INTENT_EXCLUSIVE|DRV_IO_INTENT_NONBLOCKING | DRV_IO_INTENT_READWRITE))
        {
            /* The driver only supports this mode */
            SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB USBFSV1 Driver: Unsupported IO Intent in DRV_USBFSV1_Open().");
        }
        if(drvObj->isOpened == true)
        {
            /* Driver supports exclusive open only */
            SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB USBFSV1 Driver: Driver can be opened only once. Multiple calls to DRV_USBFSV1_Open().");
        }
        else
        {
            gDrvUSBObj[drvIndex].isOpened = true;
            
            /* Handle is the pointer to the client object */
            retVal = ((DRV_HANDLE)&(gDrvUSBObj[drvIndex]));
        }        
    }

    /* Return handle */
    return retVal;
}

// *****************************************************************************
/* Function:
    SYS_STATUS DRV_USBFSV1_Status( const SYS_MODULE_OBJ object )

  Summary:
    Provides the current status of the USB Driver module.

  Description:
    This function provides the current status of the USB Driver module.

  Remarks:
    See drv_usbfsv1.h for usage information.
*/

SYS_STATUS DRV_USBFSV1_Status
(
    const SYS_MODULE_OBJ object
)
{
    SYS_STATUS retVal = SYS_STATUS_UNINITIALIZED;

    /* Check if USB instance object is valid */
    if((object != SYS_MODULE_OBJ_INVALID) || (object < DRV_USBFSV1_INSTANCES_NUMBER))
    {
        retVal = gDrvUSBObj[object].status;
    }
    else
    {
        /* Invalid object */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBFSV1 Driver: Invalid object");
    }

    /* Return the status of the driver object */
    return retVal;
}

// *****************************************************************************
/* Function:
    void DRV_USBFSV1_Close( DRV_HANDLE client )

  Summary:
    Closes an opened-instance of the  USB Driver.

  Description:
    This function closes an opened-instance of the  USB Driver, invalidating the
    handle.

  Remarks:
    See drv_usbfsv1.h for usage information.
*/

void DRV_USBFSV1_Close
(
    DRV_HANDLE handle
)
{
    DRV_USBFSV1_OBJ * hDriver = (DRV_USBFSV1_OBJ *)NULL;

    /* Check if the handle is valid */
    if((handle != DRV_HANDLE_INVALID) && (handle != (DRV_HANDLE)(NULL)))
    {
        /* Reset the relevant parameters */
        hDriver = (DRV_USBFSV1_OBJ *)handle;
        if(hDriver->isOpened)
        {
            hDriver->isOpened = false;
            hDriver->pEventCallBack = NULL;
        }
        else
        {
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBFSV1 Driver: Client Handle already closed");
        }
    }
    else
    {
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBFSV1 Driver: Bad Client Handle");
    }
}

// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_USBFSV1_Tasks_ISR( SYS_MODULE_OBJ object )

  Summary:
    Maintains the driver's Interrupt state machine and implements its ISR.

  Description:
    This function is used to maintain the driver's internal Interrupt state
    machine and implement its ISR for interrupt-driven implementations.

  Remarks:
    See drv_usbfsv1.h for usage information.
*/

void DRV_USBFSV1_Tasks_ISR
(
    SYS_MODULE_OBJ object
)
{
    DRV_USBFSV1_OBJ * drvObj = (DRV_USBFSV1_OBJ *)NULL;
    bool sysInterruptStatus;

    drvObj = &gDrvUSBObj[object];

    /* Save current interrupt state and disable them */
	sysInterruptStatus = SYS_INT_Disable();

    /* We are entering an interrupt context */
    drvObj->isInInterruptContext = true;

    /* Clear the interrupt */
    SYS_INT_SourceStatusClear(drvObj->interruptSource);

    switch(drvObj->operationMode)
    {
        case DRV_USBFSV1_OPMODE_DEVICE:

            /* Driver is running in Device Mode */
            _DRV_USBFSV1_DEVICE_TASKS_ISR(drvObj);
            break;

        case DRV_USBFSV1_OPMODE_HOST:

            /* Driver is running in Host Mode */
            _DRV_USBFSV1_HOST_TASKS_ISR(drvObj);
            break;

        case DRV_USBFSV1_OPMODE_OTG:
            /* OTG mode is not supported yet */
            break;

        default:
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBFSV1 Driver: What mode are you trying?");
            break;
    }

    /* We are exiting an interrupt context */
    drvObj->isInInterruptContext = false;

    /* Restore the interrupt state */
	SYS_INT_Restore(sysInterruptStatus);
}


// *****************************************************************************
/* Function:
    void USB_Handler(void)

  Summary:
    USB interrupt handler.
    
  Description:
    USB interrupt handler.

  Remarks:
    See drv_usbfsv1.h for usage information.
*/

void USB_Handler(void)
{
	DRV_USBFSV1_Tasks_ISR(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */

// *****************************************************************************
/* Function:
    void DRV_USBFSV1_ClientEventCallBackSet
    (
        DRV_HANDLE handle,
        uintptr_t hReferenceData,
        DRV_USBFSV1_EVENT_CALLBACK eventCallBack
    )

  Summary:
    This function sets up the event callback function that is invoked by the USB
    controller driver to notify the client of USB bus events.

  Description:
    This function sets up the event callback function that is invoked by the USB
    controller driver to notify the client of USB bus events. The callback is
    disabled by either not calling this function after the DRV_USBFSV1_Open
    function has been called or by setting the myEventCallBack argument as NULL.
    When the callback function is called, the hReferenceData argument is
    returned.

  Remarks:
    See drv_usbfsv1.h for usage information.
*/

void DRV_USBFSV1_ClientEventCallBackSet
(
    DRV_HANDLE handle,
    uintptr_t hReferenceData,
    DRV_USB_EVENT_CALLBACK eventCallBack
)
{
    DRV_USBFSV1_OBJ * pUSBDrvObj = (DRV_USBFSV1_OBJ *)handle;

    /* Check if the handle is valid or opened */
    if((handle != DRV_HANDLE_INVALID) && (handle != (DRV_HANDLE)(NULL)) && (pUSBDrvObj->isOpened == true))
    {
        /* Assign event call back and reference data */
        pUSBDrvObj->hClientArg = hReferenceData;
        pUSBDrvObj->pEventCallBack = eventCallBack;

    }
    else
    {
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBFSV1 Driver: Bad Client or client closed");
    }
}/* end of DRV_USBFSV1_ClientEventCallBackSet() */