/*******************************************************************************
   USB Host Port Driver Core Routines

   Company:
    Microchip Technology Inc.

   File Name:
    drv_usb_uhp.c

   Summary:
    USB Host port Driver Implementation 


   Description:
    The USB device driver provides a simple interface to manage the USB
    modules on Microchip microcontrollers.  This file Implements the core
    interface routines for the USB driver.

    While building the driver from source, ALWAYS use this file in the build.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************



#ifndef __inline__
    #define __inline__    inline
#endif

#include "definitions.h"

#include "driver/usb/drv_usb.h"

#include "driver/usb/uhp/src/drv_usb_uhp_local.h"

extern uint8_t  USBBufferAligned[512];  /* 4K page aligned, see Table 3-17. qTD Buffer Pointer(s) (DWords 3-7) */
#define ID_UHPHS    (41)       /**< \brief USB High Speed Host Port (UHPHS) */

/************************************
 * Driver instance object
 ***********************************/

DRV_USB_UHP_OBJ gDrvUSBObj[DRV_USB_UHP_INSTANCES_NUMBER];

/*********************************
 * Array of endpoint objects. Two
 * objects per endpoint
 ********************************/

DRV_USB_UHP_DEVICE_ENDPOINT_OBJ gDrvUSBEndpoints [DRV_USB_UHP_INSTANCES_NUMBER] [DRV_USB_UHP_ENDPOINTS_NUMBER * 2];

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_USB_UHP_Initialize
    (
       const SYS_MODULE_INDEX index,
       const SYS_MODULE_INIT * const init
    )

   Summary:
    Dynamic implementation of DRV_USB_UHP_Initialize system interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_Initialize system interface
    function. Function performs the following task:
    - Initializes the necessary USB module as per the instance init data
    - Updates internal data structure for the particular USB instance
    - Returns the USB instance value as a handle to the system

   Remarks:
    See drv_uhp.h for usage information.
 */
SYS_MODULE_OBJ DRV_USB_UHP_Initialize
(
    const SYS_MODULE_INDEX       drvIndex,
    const SYS_MODULE_INIT *const init
)
{
    DRV_USB_UHP_OBJ          *drvObj;
    volatile uhphs_registers_t *usbID;
    DRV_USB_UHP_INIT           *usbInit;
    SYS_MODULE_OBJ             retVal = SYS_MODULE_OBJ_INVALID;

    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[0m\n\rUSB HOST UHP example");

    if (drvIndex >= DRV_USB_UHP_INSTANCES_NUMBER)
    {
        /* The driver module index specified does not exist in the system */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB USB_UHP: Invalid Driver Module Index in DRV_USB_UHP_Initialize().");
    }
    else if (gDrvUSBObj[drvIndex].inUse == true)
    {
        /* Cannot initialize an object that is already in use. */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB USB_UHP: Driver is already initialized in DRV_USB_UHP_Initialize().");
    }
    else
    {
        usbInit = (DRV_USB_UHP_INIT *)init;
        usbID   = usbInit->usbID;
        drvObj  = &gDrvUSBObj[drvIndex];

        /* Try creating the global mutex. */
        if (OSAL_RESULT_TRUE != OSAL_MUTEX_Create(&drvObj->mutexID))
        {
            /* Could not create the mutual exclusion */
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB Host Layer: Could not create Mutex in DRV_USB_UHP_Initialize().");
        }
        else
        {
            /* Populate the driver object with the required data */
            drvObj->inUse          = true;
            drvObj->status         = SYS_STATUS_BUSY;
            drvObj->usbID          = usbID;

            /* Assign the endpoint table */
            drvObj->endpointTable   = &gDrvUSBEndpoints[drvIndex][0];
            drvObj->interruptSource = usbInit->interruptSource;

            /* The root hub information is applicable for host mode operation. */
            drvObj->rootHubInfo.rootHubAvailableCurrent = usbInit->rootHubAvailableCurrent;
            drvObj->rootHubInfo.portIndication          = usbInit->portIndication;
            drvObj->rootHubInfo.portOverCurrentDetect   = usbInit->portOverCurrentDetect;
            drvObj->rootHubInfo.portPowerEnable         = usbInit->portPowerEnable;

            drvObj->isOpened       = false;
            drvObj->pEventCallBack = NULL;

            drvObj->sessionInvalidEventSent = false;

            PMC_REGS->CKGR_UCKR = CKGR_UCKR_UPLLCOUNT_Msk | CKGR_UCKR_UPLLEN_Msk;
            // Wait for PLLA,UPLL stabilization LOCK bit in PMC_SR    
            while (!(PMC_REGS->PMC_SR & PMC_SR_LOCKU_Msk) );  

            PMC_REGS->PMC_PCR = PMC_PCR_PID(ID_UHPHS);
            PMC_REGS->PMC_PCR = PMC_PCR_PID(ID_UHPHS) | PMC_PCR_CMD_Msk | PMC_PCR_EN_Msk | PMC_PCR_GCKCSS_UPLL_CLK;

            /* Set the state to indicate that the delay will be started */
            drvObj->state = DRV_USB_UHP_TASK_STATE_WAIT_FOR_CLOCK_USABLE;
            retVal        = (SYS_MODULE_OBJ)drvIndex;
        }
        _DRV_USB_UHP_InterruptSourceEnable(drvObj->interruptSource);
    }
    return(retVal);
} /* end of DRV_USB_UHP_Initialize() */


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_HOST_ControlTransferProcess
    (
        DRV_USB_UHP_OBJ * hDriver
    )

  Summary:
    Control Transfer Process.
	
  Description:
    This function is called every time there is an endpoint 0 interrupt.
    This means that a stage of the current control IRP has been completed.
    This function is called from an interrupt context

  Remarks:
   
*/
static void DRV_USB_UHP_HOST_ControlTransferProcess(DRV_USB_UHP_OBJ *hDriver)
{
    USB_HOST_IRP_LOCAL                *irp;
    DRV_USB_UHP_HOST_PIPE_OBJ       *pipe, *iterPipe;
    DRV_USB_UHP_HOST_TRANSFER_GROUP *transferGroup;
    bool     endIRP = false;
    bool     foundIRP = false;
    volatile uhphs_registers_t *usbID = hDriver->usbID;
    uint8_t *pResult;
    uint32_t i;

    transferGroup = &hDriver->controlTransferGroup;

    /* First check if the IRP was aborted */
    irp  = transferGroup->currentIRP;
    pipe = transferGroup->currentPipe;

    /* If current IRP is null, or current pipe is null then we have unknown
     * failure. We just quit.  Nothing we can do.*/
    if ((irp == NULL)
     || (pipe == NULL)
     || (pipe == (DRV_USB_UHP_HOST_PIPE_OBJ *)DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID))
    {
        /* This means the pipe was closed. We don't do anything */
        return;
    }

    /* We should check the current state of the IRP and then proceed accordingly */
    /* If here means, we have a valid IRP and pipe.  Check the status register.
     * The IRP could have been aborted. This would be known in the temp state.
     */
    switch (irp->tempState)
    {
        case DRV_USB_UHP_HOST_IRP_STATE_ABORTED:
            /* This means the application has aborted this IRP.*/
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rIRP state aborted");
            endIRP      = true;
            irp->status = USB_HOST_IRP_STATUS_ABORTED;
            break;

        case DRV_USB_UHP_HOST_IRP_STATE_PROCESSING:

            if (hDriver->int_xfr_qtd_complete == 1)
            {
                hDriver->int_xfr_qtd_complete = 0;
                irp->status = USB_HOST_IRP_STATUS_COMPLETED;
                endIRP = true;
                /* Disable async list */
                usbID->UHPHS_USBCMD &= ~UHPHS_USBCMD_ASE_Msk; /* async enable = 0 */
            }
            break;

        default:
            break;
    }

    if (endIRP == true)
    {
        pResult = irp->data;

        if (irp->completedBytes != 0)
        {
            for (i = 0; i < irp->size; i++)
            {
                *(uint8_t *)(pResult + i) = USBBufferAligned[i];
            }
        }
        /* This means we need to end the IRP */
        pipe->irpQueueHead = NULL;

        if (irp->callback != NULL)
        {
            /* Invoke the call back*/
            irp->callback((USB_HOST_IRP *)(uint32_t)irp);
        }

        irp = NULL;

        /* Now we need to check if any more IRPs are in this group are pending.
         * We start searching from the current pipe onwards. If we dont find
         * another pipe with an IRP, we should land back on the current pipe and
         * check if we have a IRP to process */

        iterPipe = transferGroup->currentPipe->next;
        for (i = 0; i < transferGroup->nPipes; i++)
        {
            if (iterPipe == NULL)
            {
                /* We have reached the end of the pipe group. Rewind the pipe
                 * iterator to the start of the pipe group. */
            }

            /* This means that we have a valid pipe.  Now check if there is irp
             * to be processed. */

            if (iterPipe->irpQueueHead != NULL)
            {
                foundIRP = true;
                transferGroup->currentPipe = iterPipe;
                transferGroup->currentIRP  = iterPipe->irpQueueHead;
                break;
            }

            iterPipe = iterPipe->next;
        }

        if (foundIRP)
        {
            /* This means we have found another IRP to process. We must load the
             * endpoint. */
            irp         = transferGroup->currentIRP;
            pipe        = transferGroup->currentPipe;
            irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
            irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;
        }
        else
        {
            /* This means we don't have an IRP. Set the current IRP and current
             * pipe to NULL to indicate that we don't have any active IRP */
            transferGroup->currentPipe = NULL;
            transferGroup->currentIRP  = NULL;
        }
    }
}/* end of DRV_USB_UHP_HOST_ControlTransferProcess() */


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_HOST_NonControlTransferProcess
    (
        DRV_USB_UHP_OBJ * hDriver
        uint8_t hostPipe
   )

  Summary:
    Non Control Transfer Process.
	
  Description:
    This function processes non-zero endpoint transfers which
    could be any of bulk, interrupt and isochronous transfers

  Remarks:
*/
void DRV_USB_UHP_HOST_NonControlTransferProcess
(
    DRV_USB_UHP_OBJ * hDriver,
    uint8_t hostPipe
)
{
    /* This function processes non-zero endpoint transfers which
     * could be any of bulk, interrupt and isochronous transfers */

    DRV_USB_UHP_HOST_ENDPOINT_OBJ * endpointTable;
    USB_HOST_IRP_LOCAL                *irp;
    DRV_USB_UHP_HOST_PIPE_OBJ       *pipe;
    bool     endIRP = false;
    volatile uhphs_registers_t *usbID = hDriver->usbID;
    bool endIRPOut = false;
    uint8_t *pResult;
    uint32_t i;

    endpointTable = &(hDriver->hostEndpointTable[hostPipe]);
    usbID = hDriver->usbID;
    pipe = endpointTable->endpoint.pipe; 

    if((endpointTable->endpoint.inUse == false)
    || (pipe == NULL)
    || (pipe == (DRV_USB_UHP_HOST_PIPE_OBJ *)DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID)
    || (pipe->irpQueueHead == NULL))
    {
        /* This means the pipe was closed. We don't do anything */
        return;
    }

    irp = pipe->irpQueueHead;
   
    /* We should check the current state of the IRP and then proceed accordingly */
    switch (irp->tempState)
    {
        case DRV_USB_UHP_HOST_IRP_STATE_ABORTED:
            /* This means the application has aborted this IRP.*/
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rIRP state aborted");
            endIRP      = true;
            irp->status = USB_HOST_IRP_STATUS_ABORTED;
            break;

        case DRV_USB_UHP_HOST_IRP_STATE_PROCESSING:

            if (hDriver->int_xfr_qtd_complete == 1)
            {
                hDriver->int_xfr_qtd_complete = 0;

                if(irp->completedBytes >= irp->size)
                {
                    endIRP = true;
                    endIRPOut = true;
                    irp->status = USB_HOST_IRP_STATUS_COMPLETED;
                }
                else
                {
                    /* This means we have more data to send */
                    endIRP = false;
                }
                /* Disable async list */
                usbID->UHPHS_USBCMD &= ~UHPHS_USBCMD_ASE_Msk; /* async enable = 0 */
            }
            break;

        default:
            break;
    }

    if (endIRP)
    {
        pResult = irp->data;

        if (irp->completedBytes != 0)
        {
            for (i = 0; i < irp->size; i++)
            {
                *(uint8_t *)(pResult + i) = USBBufferAligned[i];
            }
        }
        /* This means we need to end the IRP */
        pipe->irpQueueHead = irp->next;
        if (irp->callback != NULL)
        {
            /* Invoke the call back*/
            irp->callback((USB_HOST_IRP *)(uint32_t)irp);
        }

        irp = pipe->irpQueueHead;

        if((irp != NULL) && (!(irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)) && (endIRPOut != false) )
        {
            /* We do have another IRP to process. */
            irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
        }

        /* A IRP could have been submitted in the callback. If that is the
         * case and the IRP status would indicate that it already in
         * progress. If the IRP in the queue head is not in progress then we
         * should initiate the transaction */
        if((irp != NULL) && (!(irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)) && (endIRPOut == false) )
        {
            irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;  
        }
    }
}/* end of DRV_USB_UHP_HOST_NonControlTransferProcess() */


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_HOST_TransferProcess(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    Dynamic implementation of USB HOST Transfer Process system interface function.

   Description:
    This is the dynamic implementation of USB HOST Transfer Process system interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */
void DRV_USB_UHP_HOST_TransferProcess(DRV_USB_UHP_OBJ *hDriver)
{
    /* This function is called every time there is an endpoint 0
     * interrupt. This means that a stage of the current control IRP has been
     * completed. This function is called from an interrupt context */
    USB_HOST_IRP_LOCAL                *irp;
    DRV_USB_UHP_HOST_PIPE_OBJ       *pipe;
    DRV_USB_UHP_HOST_TRANSFER_GROUP *transferGroup;

    transferGroup = &hDriver->controlTransferGroup;

    /* First check if the IRP was aborted */
    irp  = transferGroup->currentIRP;
    pipe = transferGroup->currentPipe;

    /* If current IRP is null, or current pipe is null then we have unknown
     * failure. We just quit.  Nothing we can do.*/
    if ((irp == NULL)
     || (pipe == NULL)
     || (pipe == (DRV_USB_UHP_HOST_PIPE_OBJ *)DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID))
    {
        /* This means the pipe was closed. We don't do anything */
    }
    else
    {
        if(pipe->hostPipeN == 0)
        {
            DRV_USB_UHP_HOST_ControlTransferProcess(hDriver);
        }       
        else
        {
            /* Problem here, we don't know the pipe used */
            /* We don't have an interruption for that */
            DRV_USB_UHP_HOST_NonControlTransferProcess(hDriver, hDriver->hostPipeInUse);
        }
    }
}


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_Tasks(SYS_MODULE_OBJ object)

   Summary:
    Dynamic implementation of DRV_USB_UHP_Tasks system interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_Tasks system interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */
void DRV_USB_UHP_Tasks(SYS_MODULE_OBJ object)
{
    DRV_USB_UHP_OBJ *hDriver;

    hDriver = &gDrvUSBObj[object];

    if (hDriver->status <= SYS_STATUS_UNINITIALIZED)
    {
        /* Driver is not initialized */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rDriver is not initialized");
    }
    else
    {
        /* Check the tasks state and maintain */
        switch (hDriver->state)
        {
            case DRV_USB_UHP_TASK_STATE_WAIT_FOR_CLOCK_USABLE:

                /* The operation mode can be initialized */
                hDriver->state = DRV_USB_UHP_TASK_STATE_INITIALIZE_OPERATION_MODE;

                break;

            case DRV_USB_UHP_TASK_STATE_INITIALIZE_OPERATION_MODE:

                /* Host mode specific driver initialization */
                _DRV_USB_UHP_HOST_INIT(hDriver, object);

                /* Clear and enable the interrupts */
                _DRV_USB_UHP_InterruptSourceClear(hDriver->interruptSource);
                _DRV_USB_UHP_InterruptSourceEnable(hDriver->interruptSource);

                /* Indicate that the object is ready
                * and change the state to running */
                hDriver->status = SYS_STATUS_READY;
                hDriver->state  = DRV_USB_UHP_TASK_STATE_RUNNING;
                break;

            case DRV_USB_UHP_TASK_STATE_RUNNING:

                /* The module is in a running state. In the polling mode the
                 * driver ISR tasks are called here. We also
                 * check for the VBUS level and generate events if a client
                 * event handler is registered. */

                /* Host mode specific polled
                 * task routines can be called here */
                _DRV_USB_UHP_HOST_ATTACH_DETACH_STATE_MACHINE(hDriver);

                /* Polled mode driver tasks routines are really the same as the
                 * the ISR task routines called in the driver task routine */
                _DRV_USB_UHP_Tasks_ISR(object);
                
                DRV_USB_UHP_HOST_TransferProcess(hDriver);
                break;

            default:
                break;
        }
    }
}/* end of DRV_USB_UHP_Tasks() */

// *****************************************************************************
/* Function:
    void DRV_USB_UHP_Deinitialize( const SYS_MODULE_OBJ object )

   Summary:
    Dynamic implementation of DRV_USB_UHP_Deinitialize system interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_Deinitialize
    system interface function.

   Remarks:
    See drv_uhp.h for usage information.
 */
void DRV_USB_UHP_Deinitialize
(
    const SYS_MODULE_INDEX object
)
{
    DRV_USB_UHP_OBJ *drvObj;

    if (object == (SYS_MODULE_INDEX)SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid object */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: Invalid object in DRV_USB_UHP_Deinitialize()");
    }
    else
    {
        if (object >= DRV_USB_UHP_INSTANCES_NUMBER)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: Invalid object in DRV_USB_UHP_Deinitialize()");
        }
        else
        {
            if (gDrvUSBObj[object].inUse == false)
            {
                /* Cannot de-initialize an object that is not already in use. */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: Driver not initialized in DRV_USB_UHP_Deinitialize()");
            }
            else
            {
                drvObj = &gDrvUSBObj[object];

                /* Populate the driver object with the required data */

                drvObj->inUse  = false;
                drvObj->status = SYS_STATUS_UNINITIALIZED;

                /* Clear and disable the interrupts. Assigning to a value has
                 * been implemented to remove compiler warning in polling mode.
                 */

                _DRV_USB_UHP_InterruptSourceDisable(drvObj->interruptSource);
                SYS_INT_SourceStatusClear(drvObj->interruptSource);

                drvObj->isOpened       = false;
                drvObj->pEventCallBack = NULL;

                /* Delete the mutex */
                if (OSAL_MUTEX_Delete(&drvObj->mutexID) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: Could not delete mutex in DRV_USB_UHP_Deinitialize()");
                }
            }
        }
    }
} /* end of DRV_USB_UHP_Deinitialize() */

// *****************************************************************************
/* Function:
    SYS_STATUS DRV_USB_UHP_Status( const SYS_MODULE_OBJ object )

   Summary:
    Dynamic implementation of DRV_USB_UHP_Status system interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_Status system interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */
SYS_STATUS DRV_USB_UHP_Status(SYS_MODULE_OBJ object)
{
    SYS_STATUS retVal = gDrvUSBObj[object].status;

    if (object == SYS_MODULE_OBJ_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: System Module Object is invalid in DRV_USB_UHP_Status().");
        retVal = SYS_STATUS_ERROR;
    }

    /* Return the status of the driver object */
    return(retVal);
}/* end of DRV_USB_UHP_Status() */


// *****************************************************************************
/* Function:
    SYS_STATUS DRV_USB_UHP_Open( const SYS_MODULE_INDEX iDriver,
                                   const DRV_IO_INTENT    ioIntent )

   Summary:
    Dynamic implementation of DRV_USB_UHP_Open system interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_Open system interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */
DRV_HANDLE DRV_USB_UHP_Open
(
    const SYS_MODULE_INDEX iDriver,
    const DRV_IO_INTENT    ioIntent
)
{
    DRV_USB_UHP_OBJ *drvObj;
    DRV_HANDLE        retVal = DRV_HANDLE_INVALID;

    /* The iDriver value should be valid. It should be less the number of driver
     * object instances.  */

    if (iDriver >= DRV_USB_UHP_INSTANCES_NUMBER)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Bad Driver Index in DRV_USB_USB_UHP_Open().");
    }
    else
    {
        drvObj = &gDrvUSBObj[iDriver];

        if (drvObj->status == SYS_STATUS_READY)
        {
            if (ioIntent != (DRV_IO_INTENT_EXCLUSIVE|DRV_IO_INTENT_NONBLOCKING |DRV_IO_INTENT_READWRITE))
            {
                /* The driver only supports this mode */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Unsupported IO Intent in DRV_USB_USB_UHP_Open().");
            }
            else
            {
                if (drvObj->isOpened)
                {
                    /* Driver supports exclusive open only */
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Driver can be opened only once. Multiple calls to DRV_USB_USB_UHP_Open().");
                }
                else
                {
                    /* Clear prior value */
                    drvObj->pEventCallBack = NULL;

                    /* Store the handle in the driver object client table and update
                     * the number of clients*/
                    drvObj->isOpened = true;
                    retVal           = ((DRV_HANDLE)drvObj);
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Driver opened successfully in DRV_USB_UHP_Open().");
                }
            }
        }
    }
    /* Return the client object */
    return(retVal);
}/* end of DRV_USB_UHP_Open() */

// *****************************************************************************
/* Function:
    void DRV_USB_USB_UHP_Close( DRV_HANDLE client )

   Summary:
    Dynamic implementation of DRV_USB_USB_UHP_Close client interface function.

   Description:
    This is the dynamic implementation of DRV_USB_USB_UHP_Close client interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */
void DRV_USB_UHP_Close(DRV_HANDLE client)
{
    DRV_USB_UHP_OBJ *hDriver;

    if (client == DRV_HANDLE_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Bad Client Handle in DRV_USB_UHP_Close().");
    }
    else
    {
        hDriver = (DRV_USB_UHP_OBJ *)client;

        if (!(hDriver->isOpened))
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Invalid client handle in DRV_USB_UHP_Close().");
        }
        else
        {
            /* Give back the client */
            hDriver->isOpened       = false;
            hDriver->pEventCallBack = NULL;
        }
    }
}/* end of DRV_USB_USB_UHP_Close() */

// *****************************************************************************
/* Function:
    void UHPHS_Handler( void )

   Summary:
    Dynamic implementation of UHPHS_Handler client interface function.

   Description:
    This is the dynamic implementation of UHPHS_Handler client interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */
void UHPHS_Handler(void)
{
    DRV_USB_UHP_Tasks_ISR(sysObj.usbHostObject0);
}

// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_USB_UHP_Tasks_ISR( SYS_MODULE_OBJ object )

   Summary:
    Dynamic implementation of DRV_USB_UHP_Tasks_ISR system interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_Tasks_ISR system interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */
void DRV_USB_UHP_Tasks_ISR(SYS_MODULE_OBJ object)
{
    DRV_USB_UHP_OBJ *hDriver;

    hDriver = &gDrvUSBObj[0];

    hDriver->isInInterruptContext = true;

    _DRV_USB_UHP_HOST_TASKS_ISR(hDriver);

    SYS_INT_SourceStatusClear(hDriver->interruptSource);
    hDriver->isInInterruptContext = false;
}/* end of DRV_USB_UHP_Tasks_ISR() */


// *****************************************************************************
/* Function:
    bool DRV_USB_UHP_HOST_Resume(DRV_HANDLE handle)

   Summary:
    Dynamic implementation of DRV_USB_UHP_HOST_Resume
    client interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_HOST_Resume client
    interface function. Function resumes a suspended BUS.

   Remarks:
    See drv_uhp.h for usage information.
 */
bool DRV_USB_UHP_HOST_Resume
(
    DRV_HANDLE handle
)
{
    DRV_USB_UHP_OBJ          *pusbdrvObj = (DRV_USB_UHP_OBJ *)NULL;
    volatile uhphs_registers_t *usbID;
    bool retVal = false;

    /* Check if the handle is valid */
    if ((handle == DRV_HANDLE_INVALID))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Bad Client or client closed in DRV_USB_UHP_HOST_Resume().");
    }
    else
    {
        pusbdrvObj = (DRV_USB_UHP_OBJ *)handle;
        usbID      = pusbdrvObj->usbID;
        /* Function enables resume signaling */
        *((uint32_t *)&(usbID->UHPHS_PORTSC_0) + (pusbdrvObj->hostEndpointTable[0].endpoint.pipe->hubPort))
              |= UHPHS_PORTSC_0_FPR_Msk;  /* Force Port Resume */
        retVal = true;
    }

    return retVal;
}/* end of DRV_USB_UHP_HOST_Resume() */

// *****************************************************************************

/* Function:
    bool DRV_USB_UHP_HOST_Suspend(DRV_HANDLE handle)

   Summary:
    Dynamic implementation of DRV_USB_UHP_HOST_Suspend
    client interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_HOST_Suspend client
    interface function. Function suspends USB BUS.

   Remarks:
    See drv_uhp.h for usage information.
 */
bool DRV_USB_UHP_HOST_Suspend
(
    DRV_HANDLE handle
)
{
    DRV_USB_UHP_OBJ          *pusbdrvObj = (DRV_USB_UHP_OBJ *)NULL;
    volatile uhphs_registers_t *usbID;
    bool retVal = false;

    /* Check if the handle is valid */
    if ((handle == DRV_HANDLE_INVALID))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Bad Client or client closed in DRV_USB_UHP_HOST_Suspend().");
    }
    else
    {
        pusbdrvObj = (DRV_USB_UHP_OBJ *)handle;
        usbID      = pusbdrvObj->usbID;
        /* Suspend the bus */
        /* Turn USB HOST OFF */
        pusbdrvObj->usbID->UHPHS_USBCMD &= ~UHPHS_USBCMD_RS_Msk; /* Stop = 0 */
        while ((usbID->UHPHS_USBCMD & UHPHS_USBCMD_RS_Msk) == UHPHS_USBCMD_RS_Msk)
        {
        }
        retVal = true;
    }
    return(retVal);
}/* end of DRV_USB_UHP_HOST_Suspend() */

// *****************************************************************************

/* Function:
    void DRV_USB_USB_UHP_ClientEventCallBackSet
    (
        DRV_HANDLE   handle,
        uintptr_t    hReferenceData,
        DRV_USB_UHP_EVENT_CALLBACK eventCallBack
    )

   Summary:
    Dynamic implementation of DRV_USB_USB_UHP_ClientEventCallBackSet client interface
    function.

   Description:
    This is the dynamic implementation of DRV_USB_USB_UHP_ClientEventCallBackSet
    client interface function.

   Remarks:
    See drv_uhp.h for usage information.
 */
void DRV_USB_UHP_ClientEventCallBackSet
(
    DRV_HANDLE             client,
    uintptr_t              hReferenceData,
    DRV_USB_EVENT_CALLBACK eventCallBack
)
{
    DRV_USB_UHP_OBJ *pusbDrvObj;

    if (client == DRV_HANDLE_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Bad Client Handle in DRV_USB_UHP_ClientEventCallBackSet().");
    }
    else
    {
        pusbDrvObj = (DRV_USB_UHP_OBJ *)client;

        if (!pusbDrvObj->isOpened)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Invalid client handle in DRV_USB_UHP_ClientEventCallBackSet().");
        }
        else
        {
            /* Assign event call back and reference data */
            pusbDrvObj->hClientArg     = hReferenceData;
            pusbDrvObj->pEventCallBack = eventCallBack;
        }
    }
}/* end of DRV_USB_USB_UHP_ClientEventCallBackSet() */
