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

#include "definitions.h"

#include "driver/usb/drv_usb.h"
#include "driver/usb/uhp/drv_usb_uhp.h"
#include "driver/usb/uhp/src/drv_usb_uhp_local.h"
#include "drv_usb_uhp_ohci_host.h"

#define NOT_CACHED __attribute__((__section__(".region_nocache")))
NOT_CACHED uint8_t USBBufferNoCache[DRV_USB_UHP_PIPES_NUMBER][DRV_USB_UHP_NO_CACHE_BUFFER_LENGTH];


/************************************
 * Prototype
 ***********************************/
extern void UHP_Handler(void);
void DRV_USB_UHP_StartOfFrameControl(DRV_HANDLE client, bool control);/* remove warning was declared but never referenced */
bool DRV_USB_UHP_Resume(DRV_HANDLE handle);  /* remove warning was declared but never referenced */
bool DRV_USB_UHP_Suspend(DRV_HANDLE handle); /* remove warning was declared but never referenced */

/************************************
 * Driver instance object
 ***********************************/

DRV_USB_UHP_OBJ gDrvUSBObj[DRV_USB_UHP_INSTANCES_NUMBER];

/*****************************************
* Pool of pipe objects that is used by
* all driver instances.
*****************************************/
DRV_USB_UHP_HOST_PIPE_OBJ gDrvUSBHostPipeObj[DRV_USB_UHP_PIPES_NUMBER];

// ****************************************************************************
// ****************************************************************************
// Local Functions
// ****************************************************************************
// ****************************************************************************

// *****************************************************************************
/* Function:
    static void DRV_USB_UHP_ControlTransferProcess
    (
        DRV_USB_UHP_OBJ *hDriver
    )

  Summary:
    Control Transfer Process.

  Description:
    This function is called every time there is an endpoint 0 interrupt.
    This means that a stage of the current control IRP has been completed.
    This function is called from an interrupt context

  Remarks:

*/
static void _DRV_USB_UHP_ControlTransferProcess(DRV_USB_UHP_OBJ *hDriver, uint8_t hostPipeInUse)
{
    USB_HOST_IRP_LOCAL *irp;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe, *iterPipe;
    DRV_USB_UHP_HOST_TRANSFER_GROUP *transferGroup;
    bool endIRP = false;
    bool foundIRP = false;
    uint8_t *pResult;
    uint32_t i;

    transferGroup = &hDriver->controlTransferGroup;

    /* First check if the IRP was aborted */
    irp  = transferGroup->currentIRP;
    pipe = transferGroup->currentPipe;

    /* If current IRP is null, or current pipe is null then we have unknown
     * failure. We just quit.  Nothing we can do.*/

    if((irp != NULL) && (pipe != NULL) && (pipe != (DRV_USB_UHP_HOST_PIPE_OBJ *)DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID))
    {

        /* We should check the current state of the IRP and then proceed accordingly */
        /* If here means, we have a valid IRP and pipe.  Check the status register.
         * The IRP could have been aborted. This would be known in the temp state.
         */

        if ( hDriver->hostEndpointTable[hostPipeInUse].endpoint.intXfrQtdComplete == 0xFF )
        {
            /* This means the packet was stalled. */
            endIRP = true;
            irp->status = USB_HOST_IRP_STATUS_ERROR_STALL;
            irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;
            hDriver->hostEndpointTable[hostPipeInUse].endpoint.intXfrQtdComplete = 0;
            hDriver->controlTransferGroup.currentIRP = NULL;
        }
        else if (irp->tempState == DRV_USB_UHP_HOST_IRP_STATE_ABORTED)
        {
            /* This means the application has aborted this IRP. */
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rIRP state aborted");
            endIRP = true;
            irp->status = USB_HOST_IRP_STATUS_ABORTED;
        }
        else if( (irp->tempState == DRV_USB_UHP_HOST_IRP_STATE_PROCESSING)
            && (DRV_USB_UHP_OHCI_HOST_EDIsFinish(hostPipeInUse) == 1) )
        {
            if (hDriver->hostEndpointTable[hostPipeInUse].endpoint.intXfrQtdComplete == 1)
            {
                hDriver->hostEndpointTable[hostPipeInUse].endpoint.intXfrQtdComplete = 0;
                irp->status = USB_HOST_IRP_STATUS_COMPLETED;
                endIRP = true;
                DRV_USB_UHP_OHCI_HOST_DisableControlList(hDriver);
            }
        }

        if(endIRP == true)
        {
            if( (irp->flags & 0x80) == 0x80 )
            {
                /* Device to Host: IN */
                if (irp->completedBytes != 0)
                {
                    /* Check the real bytes received */
                    DRV_USB_UHP_OHCI_HOST_ReceivedSize( &(irp->size) );
                    pResult = irp->data;
                    for (i = 0; i < irp->size; i++)
                    {
                        *(uint8_t *)(pResult + i) = USBBufferNoCache[hostPipeInUse][i];
                    }
                }
            }
            /* This means this IRP needs to be terminated and new one started. */

            if(irp->callback != NULL)
            {
                /* Invoke the call back*/
                irp->callback((USB_HOST_IRP *)irp);
            }

            /* Expire the IRP */

            pipe->irpQueueHead = irp->next;

            /* Now we need to check if any more IRPs are in this group are pending.
             * We start searching from the current pipe onwards. If we dont find
             * another pipe with an IRP, we should land back on the current pipe and
             * check if we have a IRP to process */

            iterPipe = transferGroup->currentPipe->next;
            for(i = 0; i < transferGroup->nPipes; i ++)
            {
                if(iterPipe == NULL)
                {
                    /* We have reached the end of the pipe group. Rewind the pipe
                     * iterator to the start of the pipe group. */

                    iterPipe = transferGroup->pipe;
                }

                /* This means that we have a valid pipe.  Now check if there is irp
                 * to be processed. */

                if(iterPipe->irpQueueHead != NULL)
                {
                    foundIRP = true;
                    transferGroup->currentPipe = iterPipe;
                    transferGroup->currentIRP  = iterPipe->irpQueueHead;
                    break;
                }

                iterPipe = iterPipe->next;
            }

            if(foundIRP)
            {
                /* This means we have found another IRP to process. We must load the
                 * endpoint. */

                irp = transferGroup->currentIRP;
                pipe = transferGroup->currentPipe;
                irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
                irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;
            }
            else
            {
                /* This means we dont have an IRP. Set the current IRP and current
                 * pipe to NULL to indicate that we dont have any active IRP */

                transferGroup->currentPipe = NULL;
                transferGroup->currentIRP  = NULL;
            }
        }
    }
    else
    {
        /* This means the pipe was closed. We don't do anything */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rError ControlTransferProcess: %d \033[0m", (int)hostPipeInUse);
    }
}/* end of _DRV_USB_UHP_ControlTransferProcess() */


// *****************************************************************************
/* Function:
    void _DRV_USB_UHP_NonControlTransferProcess
    (
        DRV_USB_UHP_OBJ *hDriver
        uint8_t hostPipe
   )

  Summary:
    Non Control Transfer Process.

  Description:
    This function processes non-zero endpoint transfers which
    could be any of bulk, interrupt and isochronous transfers

  Remarks:
*/
static void _DRV_USB_UHP_NonControlTransferProcess
(
    DRV_USB_UHP_OBJ *hDriver,
    uint8_t hostPipe
)
{
    /* This function processes non-zero endpoint transfers which
     * could be any of bulk, interrupt and isochronous transfers */

    DRV_USB_UHP_HOST_ENDPOINT_OBJ *endpointTable;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe;
    USB_HOST_IRP_LOCAL *irp;
    bool endIRP = false;
    bool endIRPOut = false;
    uint8_t *pResult;
    uint32_t i;

    endpointTable = &(hDriver->hostEndpointTable[hostPipe]);
    pipe = endpointTable->endpoint.pipe;

    if((endpointTable->endpoint.inUse == false)
    || (pipe == NULL)
    || (pipe == (DRV_USB_UHP_HOST_PIPE_OBJ *)DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID)
    || (pipe->irpQueueHead == NULL))
    {
        /* This means the pipe was closed. We don't do anything */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rError NonControlTransferProcess: %d \033[0m", (int)hostPipe);
    }
    else
    {
        irp = pipe->irpQueueHead;

        /* We should check the current state of the IRP and then proceed accordingly */
        if ( hDriver->hostEndpointTable[hostPipe].endpoint.intXfrQtdComplete == 0xFF )
        {
            /* This means the packet was stalled */
            endIRP = true;
            irp->status = USB_HOST_IRP_STATUS_ERROR_STALL;

            irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;
            hDriver->hostEndpointTable[hostPipe].endpoint.intXfrQtdComplete = 0;
            hDriver->controlTransferGroup.currentIRP = NULL;
        }
        else if ( irp->tempState == DRV_USB_UHP_HOST_IRP_STATE_ABORTED )
        {
            /* This means the application has aborted this IRP.*/
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rIRP state aborted");
            endIRP = true;
            irp->status = USB_HOST_IRP_STATUS_ABORTED;
        }
        else if ( irp->tempState == DRV_USB_UHP_HOST_IRP_STATE_PROCESSING )
        {
            if (hDriver->hostEndpointTable[hostPipe].endpoint.intXfrQtdComplete == 1)
            {
                hDriver->hostEndpointTable[hostPipe].endpoint.intXfrQtdComplete = 0;

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

                DRV_USB_UHP_OHCI_HOST_DisableBulkList(hDriver);
            }
        }

        if(endIRP)
        {
            pResult = irp->data;
            for (i = 0; i < irp->size; i++)
            {
                *(uint8_t *)(pResult + i) = USBBufferNoCache[hostPipe][i];
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
    }
}/* end of _DRV_USB_UHP_NonControlTransferProcess() */



// ****************************************************************************
// ****************************************************************************
// External Functions
// ****************************************************************************
// ****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_USB_UHP_Initialize
    (
       const SYS_MODULE_INDEX index,
       const SYS_MODULE_INIT *const init
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
    const SYS_MODULE_INDEX drvIndex,
    const SYS_MODULE_INIT *const init
)
{
    DRV_USB_UHP_OBJ *drvObj;
    DRV_USB_UHP_INIT *usbInit;
    SYS_MODULE_OBJ retVal = SYS_MODULE_OBJ_INVALID;

    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[0m\n\rUSB HOST UHP example");

    if (drvIndex >= DRV_USB_UHP_INSTANCES_NUMBER)
    {
        /* The driver module index specified does not exist in the system */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid Driver Module Index in DRV_USB_UHP_Initialize().");
    }
    else if (gDrvUSBObj[drvIndex].inUse == true)
    {
        /* Cannot initialize an object that is already in use. */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Driver is already initialized in DRV_USB_UHP_Initialize().");
    }
    else
    {
        usbInit = (DRV_USB_UHP_INIT *)init;
        drvObj  = &gDrvUSBObj[drvIndex];

        /* Try creating the global mutex. */
        if (OSAL_RESULT_TRUE != OSAL_MUTEX_Create(&drvObj->mutexID))
        {
            /* Could not create the mutual exclusion */
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB UHP: Could not create Mutex in DRV_USB_UHP_Initialize().");
        }
        else
        {
            /* Populate the driver object with the required data */
            drvObj->inUse     = true;
            drvObj->status    = SYS_STATUS_BUSY;
            drvObj->usbIDOHCI = ((UhpOhci *)UHPHS_OHCI_ADDR); /* OHCI */

            /* Assign the endpoint table */
            drvObj->interruptSource = usbInit->interruptSource;

            /* The root hub information is applicable for host mode operation. */
            drvObj->rootHubInfo.rootHubAvailableCurrent = usbInit->rootHubAvailableCurrent;
            drvObj->rootHubInfo.portIndication          = usbInit->portIndication;
            drvObj->rootHubInfo.portOverCurrentDetect   = usbInit->portOverCurrentDetect;
            drvObj->rootHubInfo.portPowerEnable         = usbInit->portPowerEnable;

            drvObj->isOpened = false;
            drvObj->pEventCallBack = NULL;

            drvObj->sessionInvalidEventSent = false;

            PMC_UCKR_UPLLEN();

            /* Set the state to indicate that the delay will be started */
            drvObj->state = DRV_USB_UHP_TASK_STATE_WAIT_FOR_CLOCK_USABLE;
            retVal = (SYS_MODULE_OBJ)drvIndex;
        }
        _DRV_USB_UHP_InterruptSourceEnable(drvObj->interruptSource);
    }
    return(retVal);
} /* end of DRV_USB_UHP_Initialize() */

// *****************************************************************************
/* Function:
    void DRV_USB_UHP_HostInitialize
    (
        DRV_USB_UHP_OBJ *const pUSBDrvObj,
        const SYS_MODULE_INDEX index
    )

  Summary:
    This function initializes the driver for host mode operation.

  Description:
    Function performs the following tasks:
      - Informs the USB module with SOF threshold in bit times
      - Enables VBUS power and initializes the module in HOST mode
      - Resets the BDT table data structure with init value
      - Configures EP0 register for the specific USB module
      - Enables the USB attach interrupt

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/
void DRV_USB_UHP_HostInitialize
(
    DRV_USB_UHP_OBJ *drvObj,
    SYS_MODULE_INDEX index
)
{
    /* No device attached */
    drvObj->deviceAttached = false;
    /* Initialize the device handle */
    drvObj->attachedDeviceObjHandle = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;
    drvObj->blockPipe = 0;
    drvObj->portNumber = 0xFF;

    DRV_USB_UHP_OHCI_HOST_Init_simpl(drvObj);
    /* Initialize the host specific members in the driver object */
    drvObj->isResetting = false;
    drvObj->usbHostDeviceInfo = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;
    drvObj->operationEnabled = false;
}/* end of DRV_USB_UHP_HostInitialize() */


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_ResetStateMachine(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    Reset State Machine

   Description:
    Reset State Machine

   Remarks:
    Refer to .h for usage information.
 */
void DRV_USB_UHP_ResetStateMachine(DRV_USB_UHP_OBJ *hDriver)
{
    volatile UhpOhci *usbIDOHCI = hDriver->usbIDOHCI;

    /* Check if reset is needed */
    switch (hDriver->resetState)
    {
        case DRV_USB_UHP_HOST_RESET_STATE_NO_RESET:
            /* No reset signaling is request */
            break;

        case DRV_USB_UHP_HOST_RESET_STATE_START:
            if( (*((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) & UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk) == UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk )
            {
                /* ClearPortEnable */
                *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk;
                /* Initiate port reset. (write) SetPortReset */
                *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_PRS_Msk;
                hDriver->resetState = DRV_USB_UHP_HOST_RESET_STATE_WAIT_FOR_COMPLETE;
            }
            break;

        case DRV_USB_UHP_HOST_RESET_STATE_WAIT_FOR_COMPLETE:
            /* PortResetStatusChange (bit 20) */
            if( (*((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) & UHP_OHCI_HCRHPORTSTATUS0_PRSC_Msk) == UHP_OHCI_HCRHPORTSTATUS0_PRSC_Msk )
            {
                /* This bit is set at the end of the 10-ms port reset signal.
                 * port reset is complete */
                *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_PRSC_Msk;

                if( (*((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) & UHP_OHCI_HCRHPORTSTATUS0_LSDA_Msk) == UHP_OHCI_HCRHPORTSTATUS0_LSDA_Msk )
                {
                    /* LowSpeedDeviceAttached */
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rDRV USB_UHP: LS device connected");
                    hDriver->deviceSpeed = USB_SPEED_LOW;
                }
                else
                {
                    /* High speed or full speed device connected */
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rDRV USB_UHP: FS device connected");
                    hDriver->deviceSpeed = USB_SPEED_FULL;
                }
                /* Mandatory for some USB key high speed used in full speed */
                /* 500ms is needed for a full speed pendrive */
                if (SYS_TIME_DelayMS( 500 /* ms */, &hDriver->timerHandle) == SYS_TIME_SUCCESS)
                {
                    hDriver->resetState = DRV_USB_UHP_HOST_RESET_STATE_COMPLETE;
                }
                else
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO,"\r\nDRV USB_UHP: Handle error ");
                }
            }
            break;

        case DRV_USB_UHP_HOST_RESET_STATE_COMPLETE:
            /* Normally, the Host Controller Driver must ensure that the Host Controller stays in USBSUSPEND state for at least 5 ms
             * and then exits this state to either the USBRESUME or the USBRESET state. */
            if (SYS_TIME_DelayIsComplete(hDriver->timerHandle) == true)
            {
                /* port reset is complete */
                hDriver->isResetting = false;
                hDriver->resetState = DRV_USB_UHP_HOST_RESET_STATE_NO_RESET;
            }
            break;

        default:
            break;
    }
} /* End of DRV_USB_UHP_ResetStateMachine */


/* Function:
    void DRV_USB_UHP_AttachDetachStateMachine(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    Initiate the USB reset on the current port number

   Description:
    Initiate the USB reset on the current port number

   Remarks:
    Refer to .h for usage information.
 */
void DRV_USB_UHP_AttachDetachStateMachine(DRV_USB_UHP_OBJ *hDriver)
{
    /* In the host mode, we perform attach de-bouncing */
    bool interruptWasEnabled;

    switch (hDriver->attachState)
    {
        case DRV_USB_UHP_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH:

            /* If no device is attached, then there is nothing to do
             * If device is attached, then move to next state */
            if (hDriver->deviceAttached)
            {
                hDriver->attachState = DRV_USB_UHP_HOST_ATTACH_STATE_DETECTED_DEBOUNCE;
            }
            break;

        case DRV_USB_UHP_HOST_ATTACH_STATE_DETECTED_DEBOUNCE:

            /* Delta T3: TATTDB > 100ms */
            if (SYS_TIME_DelayMS( 200  /* ms Delta T3 */, &hDriver->timerHandle) == SYS_TIME_SUCCESS)
            {
                hDriver->attachState = DRV_USB_UHP_HOST_ATTACH_STATE_DETECTED;
            }
            else
            {
                SYS_DEBUG_PRINT(SYS_ERROR_INFO,"\r\nDRV USB_UHP: Handle error 3");
            }
            break;

    case DRV_USB_UHP_HOST_ATTACH_STATE_DETECTED:

            if (SYS_TIME_DelayIsComplete(hDriver->timerHandle) == true)
            {
                /* Disable the driver interrupt as
                 * we do not want this section to be interrupted. */
                interruptWasEnabled = _DRV_USB_UHP_InterruptSourceDisable(hDriver->interruptSource);

                if (hDriver->deviceAttached)
                {
                    /* Yes the device is still attached. Enumerate
                     * this device. usbHostDeviceInfo is the ID of
                     * this root hub. */
                    hDriver->attachedDeviceObjHandle = USB_HOST_DeviceEnumerate(hDriver->usbHostDeviceInfo, 0);
                    hDriver->attachState = DRV_USB_UHP_HOST_ATTACH_STATE_READY;
                }
                else
                {
                    /* The device is not attached any more. This was a false attach
                     */
                    hDriver->attachState = DRV_USB_UHP_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH;
                }

                if (interruptWasEnabled)
                {
                    /* Re-enable the interrupt if it was originally enabled. */
                    _DRV_USB_UHP_InterruptSourceEnable(hDriver->interruptSource);
                }
            }
            break;

        case DRV_USB_UHP_HOST_ATTACH_STATE_READY:

            /* De-bouncing is done and device ready. We can check
             * here if the device is detached */
            if (!hDriver->deviceAttached)
            {
                /* Device is not attached */
                hDriver->attachState = DRV_USB_UHP_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH;
            }
            break;

        default:
            break;
    }
} /* End of DRV_USB_UHP_AttachDetachStateMachine */

// *****************************************************************************
/* Function:
    void UHP_Handler( void )

   Summary:
    Dynamic implementation of UHP_Handler client interface function.

   Description:
    This is the dynamic implementation of UHP_Handler client interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */
void UHP_Handler(void)
{
    DRV_USB_UHP_Tasks_ISR(sysObj.drvUSBObject);
}

// *****************************************************************************
/* Function:
    void DRV_USB_UHP_IRPCancel(USB_HOST_IRP *inputIRP)

  Summary:
    Cancels the specified IRP.

  Description:
    This function attempts to cancel the specified IRP. If the IRP is queued and
    its processing has not started, it will be cancelled successfully. If the
    IRP in progress, the ongoing transaction will be allowed to complete.

  Remarks:
    See .h for usage information.
*/
void DRV_USB_UHP_IRPCancel
(
    USB_HOST_IRP *inputIRP
)
{
    /* This function cancels an IRP */

    USB_HOST_IRP_LOCAL *irp = (USB_HOST_IRP_LOCAL *) inputIRP;
    DRV_USB_UHP_OBJ *hDriver;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe;
    bool interruptWasEnabled = false;

    if (irp->pipe == DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID)
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid pipe");
    }
    else if(irp->status <= USB_HOST_IRP_STATUS_COMPLETED_SHORT)
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: IRP is not pending or in progress");
    }
    else
    {
        pipe    = (DRV_USB_UHP_HOST_PIPE_OBJ *)irp->pipe;
        hDriver = (DRV_USB_UHP_OBJ *)pipe->hClient;

        if(!hDriver->isInInterruptContext)
        {
            if(OSAL_MUTEX_Lock(&(hDriver->mutexID), OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
            {
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex lock failed");
            }
            interruptWasEnabled = _DRV_USB_UHP_InterruptSourceDisable(hDriver->interruptSource);
        }

        if(irp->previous == NULL)
        {
            /* This means this was the first irp in the queue. Update the pipe
             * queue head directly */

            pipe->irpQueueHead = irp->next;
             if(irp->next != NULL)
            {
                irp->next->previous = NULL;
            }
        }
        else
        {
            /* Remove the IRP from the linked list */
            irp->previous->next = irp->next;

            if(irp->next != NULL)
            {
                /* This applies if this is not the last
                 * irp in the linked list */
                irp->next->previous = irp->previous;
            }
        }

        if(irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)
        {
            /* If the irp is already in progress then
             * we set the temporary state. This will get
             * caught in _DRV_USB_UHP_HOST_ControlXferProcess()
             * and _DRV_USB_UHP_HOST_NonControlIRPProcess() functions. */

            irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_ABORTED;
        }
        else
        {
            irp->status = USB_HOST_IRP_STATUS_ABORTED;
            if(irp->callback != NULL)
            {
                irp->callback((USB_HOST_IRP *)irp);
            }
        }

        if(!hDriver->isInInterruptContext)
        {
            if(interruptWasEnabled)
            {
                _DRV_USB_UHP_InterruptSourceEnable(hDriver->interruptSource);
            }

            if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
            {
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex unlock failed");
            }
        }
    }
}/* end of DRV_USB_UHP_IRPCancel() */

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

    if(hDriver->status <= SYS_STATUS_UNINITIALIZED)
    {
        /* Driver is not initialized */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rDriver is not initialized");
    }
    else
    {
        /* Check the tasks state and maintain */
        switch(hDriver->state)
        {
            case DRV_USB_UHP_TASK_STATE_WAIT_FOR_CLOCK_USABLE:

                /* Wait for PLLA,UPLL stabilization LOCK bit in PMC_SR     */
                if(IS_LOCKU_ENABLE())
                {
                    /* The operation mode can be initialized */
                    hDriver->state = DRV_USB_UHP_TASK_STATE_INITIALIZE_OPERATION_MODE;
                }
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
                DRV_USB_UHP_AttachDetachStateMachine(hDriver);

                /* Polled mode driver tasks routines are really the same as the
                 * the ISR task routines called in the driver task routine */
                _DRV_USB_UHP_Tasks_ISR(object);
                break;

            default:
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB USB_UHP: Unsupported driver operation mode in DRV_USB_UHP_Tasks().");
                break;
        }
    }
}/* end of DRV_USB_UHP_Tasks() */


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_StartOfFrameControl(DRV_HANDLE client, bool control)

  Summary:
    SOF

  Description:
    Management of SOF: nothing to do

  Remarks:
    See drv_xxx.h for usage information.
*/
void DRV_USB_UHP_StartOfFrameControl(DRV_HANDLE client, bool control)
{
    /* At the point this function does not do any thing.
     * The Start of frame signaling in this HCD is controlled
     * automatically by the module. */
}/* end of DRV_USB_UHP_StartOfFrameControl() */

// *****************************************************************************
/* Function:
    USB_SPEED DRV_USB_UHP_DeviceCurrentSpeedGet(DRV_HANDLE client)

  Summary:
    Current speed

  Description:
    Get current usb speed of the connected device

  Remarks:
    See drv_xxx.h for usage information.
*/
USB_SPEED DRV_USB_UHP_DeviceCurrentSpeedGet(DRV_HANDLE client)
{
    /* This function returns the current device speed */
    DRV_USB_UHP_OBJ *hDriver;

    if ((client == DRV_HANDLE_INVALID) || (((DRV_USB_UHP_OBJ *)client) == NULL))
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid client");
    }

    hDriver = (DRV_USB_UHP_OBJ *)client;
    return(hDriver->deviceSpeed);
}/* end of DRV_USB_UHP_DeviceCurrentSpeedGet() */



// *****************************************************************************
/* Function:
    void DRV_USB_UHP_TransferProcess(DRV_USB_UHP_OBJ *hDriver, uint8_t hostPipeInUse)

   Summary:
    Dynamic implementation of USB HOST Transfer Process system interface function.

   Description:
    This is the dynamic implementation of USB HOST Transfer Process system interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */
void DRV_USB_UHP_TransferProcess(DRV_USB_UHP_OBJ *hDriver, uint8_t hostPipeInUse)
{
    /* This function is called every time there is an endpoint 0
     * interrupt. This means that a stage of the current control IRP has been
     * completed. This function is called from an interrupt context */

    if(hostPipeInUse == 0)
    {
        _DRV_USB_UHP_ControlTransferProcess(hDriver, hostPipeInUse);
    }
    else
    {
        _DRV_USB_UHP_NonControlTransferProcess(hDriver, hostPipeInUse);
    }
}


/* **************************************************************************** */
/* Function:
    bool DRV_USB_UHP_EventsDisable
    (
        DRV_HANDLE handle
    );

   Summary:
    Disables host mode events.

   Description:
    This function disables the host mode events. This function is called by the
    Host Layer when it wants to execute code atomically.

   Remarks:
    Refer to .h for usage information.
 */

bool DRV_USB_UHP_EventsDisable
(
    DRV_HANDLE handle
)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj;
    bool result = false;

    if((DRV_HANDLE_INVALID != handle) && (0 != handle))
    {
        pUSBDrvObj = (DRV_USB_UHP_OBJ *)(handle);
        result     = _DRV_USB_UHP_InterruptSourceDisable(pUSBDrvObj->interruptSource);
    }

    return(result);
}

/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_EventsEnable
    (
        DRV_HANDLE handle
        bool eventRestoreContext
    );

   Summary:
    Restores the events to the specified the original value.

   Description:
    This function will restore the enable disable state of the events.
    eventRestoreContext should be equal to the value returned by the
    DRV_USB_UHP_EventsDisable() function.

   Remarks:
    Refer to .h for usage information.
 */

void DRV_USB_UHP_EventsEnable
(
    DRV_HANDLE handle,
    bool eventContext
)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj;

    if((DRV_HANDLE_INVALID != handle) && (0 != handle))
    {
        pUSBDrvObj = (DRV_USB_UHP_OBJ *)(handle);
        if(false == eventContext)
        {
            _DRV_USB_UHP_InterruptSourceDisable(pUSBDrvObj->interruptSource);
        }
        else
        {
            _DRV_USB_UHP_InterruptSourceEnable(pUSBDrvObj->interruptSource);
        }
    }
}


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
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: Invalid object in DRV_USB_UHP_Deinitialize()");
    }
    else
    {
        if (object >= DRV_USB_UHP_INSTANCES_NUMBER)
        {
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: Invalid object in DRV_USB_UHP_Deinitialize()");
        }
        else
        {
            if (gDrvUSBObj[object].inUse == false)
            {
                /* Cannot de-initialize an object that is not already in use. */
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: Driver not initialized in DRV_USB_UHP_Deinitialize()");
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
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: Could not delete mutex in DRV_USB_UHP_Deinitialize()");
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
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB_UHP Driver: System Module Object is invalid in DRV_USB_UHP_Status().");
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
    DRV_USB_UHP_OBJ *drvObj = NULL;
    DRV_HANDLE handle = DRV_HANDLE_INVALID;

    /* The iDriver value should be valid. It should be less the number of driver
     * object instances.  */

    if (iDriver < DRV_USB_UHP_INSTANCES_NUMBER)
    {
        drvObj = &gDrvUSBObj[iDriver];

        if ( SYS_STATUS_READY == drvObj->status )
        {
            if(ioIntent != (DRV_IO_INTENT_EXCLUSIVE|DRV_IO_INTENT_NONBLOCKING | DRV_IO_INTENT_READWRITE))
            {
                /* The driver only supports this mode */
                SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\r\nUSB USB_UHP Driver: Unsupported IO Intent in DRV_USB_USB_UHP_Open().");
            }
            else
            {
                if(drvObj->isOpened)
                {
                    /* Driver supports exclusive open only */
                    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\r\nUSB USB_UHP Driver: Driver can be opened only once. Multiple calls to DRV_USB_USB_UHP_Open().");
                }
                else
                {
                    /* Clear prior value */
                    drvObj->pEventCallBack = NULL;

                    /* Store the handle in the driver object client table and update
                     * the number of clients*/
                    drvObj->isOpened = true;
                    handle = ((DRV_HANDLE)drvObj);
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Driver opened successfully in DRV_USB_UHP_Open().");
                }
            }
        }
    }
    else
    {
        SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\r\nUSB USB_UHP Driver: Bad Driver Index in DRV_USB_USB_UHP_Open().");
    }

    /* Return the client object */

    return (handle);
}/* end of DRV_USB_UHP_Open() */


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_Close( DRV_HANDLE client )

   Summary:
    Dynamic implementation of DRV_USB_USB_UHP_Close client interface function.

   Description:
    This is the dynamic implementation of DRV_USB_USB_UHP_Close client interface
    function.

   Remarks:
    See drv_uhp.h for usage information.
 */

void DRV_USB_UHP_Close
(
    DRV_HANDLE client
)
{
    DRV_USB_UHP_OBJ *hDriver;

    if(client == DRV_HANDLE_INVALID)
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Bad Client Handle in DRV_USB_UHP_Close().");
    }
    else
    {
        hDriver = (DRV_USB_UHP_OBJ *)client;

        if(!(hDriver->isOpened))
        {
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Invalid client handle in DRV_USB_UHP_Close().");
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

    hDriver = &gDrvUSBObj[object];

    hDriver->isInInterruptContext = true;

    DRV_USB_UHP_OHCI_HOST_Tasks_ISR(hDriver);

    hDriver->isInInterruptContext = false;
}/* end of DRV_USB_UHP_Tasks_ISR() */



// *****************************************************************************
/* Function:
    bool DRV_USB_UHP_Resume(DRV_HANDLE handle)

   Summary:
    Dynamic implementation of DRV_USB_UHP_Resume
    client interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_Resume client
    interface function. Function resumes a suspended BUS.

   Remarks:
    See drv_uhp.h for usage information.
 */
bool DRV_USB_UHP_Resume
(
    DRV_HANDLE handle
)
{
    DRV_USB_UHP_OBJ *pusbdrvObj = (DRV_USB_UHP_OBJ *)NULL;
    volatile UhpOhci *usbIDOHCI;
    bool retVal = false;

    /* Check if the handle is valid */
    if ((handle == DRV_HANDLE_INVALID))
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Bad Client or client closed in DRV_USB_UHP_Resume().");
    }
    else
    {
        pusbdrvObj = (DRV_USB_UHP_OBJ *)handle;
        usbIDOHCI = pusbdrvObj->usbIDOHCI;
        /* ClearSuspendStatus: initiate a resume */
        *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + (pusbdrvObj->hostEndpointTable[0].endpoint.pipe->hubPort))
              = UHP_OHCI_HCRHPORTSTATUS0_POCI_Msk;
        retVal = true;
    }

    return retVal;
}/* end of DRV_USB_UHP_Resume() */


// *****************************************************************************
/* Function:
    bool DRV_USB_UHP_Suspend(DRV_HANDLE handle)

   Summary:
    Dynamic implementation of DRV_USB_UHP_Suspend
    client interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_Suspend client
    interface function. Function suspends USB BUS.

   Remarks:
    See drv_uhp.h for usage information.
 */
bool DRV_USB_UHP_Suspend
(
    DRV_HANDLE handle
)
{
    DRV_USB_UHP_OBJ *pusbdrvObj = (DRV_USB_UHP_OBJ *)NULL;
    volatile UhpOhci *usbIDOHCI;
    bool retVal = false;

    /* Check if the handle is valid */
    if ((handle == DRV_HANDLE_INVALID))
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Bad Client or client closed in DRV_USB_UHP_Suspend().");
    }
    else
    {
        pusbdrvObj = (DRV_USB_UHP_OBJ *)handle;
        usbIDOHCI = pusbdrvObj->usbIDOHCI;
        usbIDOHCI->UHP_OHCI_HCCONTROL &= ~UHP_OHCI_HCCONTROL_CLE_Msk;
        usbIDOHCI->UHP_OHCI_HCCONTROL &= ~UHP_OHCI_HCCONTROL_BLE_Msk;
        usbIDOHCI->UHP_OHCI_HCCONTROL &= ~UHP_OHCI_HCCONTROL_PLE_Msk;
        usbIDOHCI->UHP_OHCI_HCCONTROL &= ~UHP_OHCI_HCCONTROL_IE_Msk;
        /* SetPortSuspend */
        /* ClearPortEnable: clear the PortEnableStatus bit: 0 = port is disabled */
        *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + (pusbdrvObj->hostEndpointTable[0].endpoint.pipe->hubPort)) = UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk;
        /* SetPortSuspend: no SOF */
        *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + (pusbdrvObj->hostEndpointTable[0].endpoint.pipe->hubPort)) = UHP_OHCI_HCRHPORTSTATUS0_PSS_Msk;
        retVal = true;
    }
    return(retVal);
}/* end of DRV_USB_UHP_Suspend() */


// *****************************************************************************
/* Function:
    void DRV_USB_USB_UHP_ClientEventCallBackSet
    (
        DRV_HANDLE   handle,
        uintptr_t    hReferenceData,
        DRV_USB_UHP_EVENT_CALLBACK eventCallBack
    )

  Summary:
    This function sets up the event callback function that is invoked by the USB
    controller driver to notify the client of USB bus events.

  Description:
    This function sets up the event callback function that is invoked by the USB
    controller driver to notify the client of USB bus events. The callback is
    disabled by either not calling this function after the DRV_USB_UHP_Open
    function has been called or by setting the myEventCallBack argument as NULL.
    When the callback function is called, the hReferenceData argument is
    returned.

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
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Bad Client Handle in DRV_USB_UHP_ClientEventCallBackSet().");
    }
    else
    {
        pusbDrvObj = (DRV_USB_UHP_OBJ *)client;

        if (!pusbDrvObj->isOpened)
        {
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB USB_UHP Driver: Invalid client handle in DRV_USB_UHP_ClientEventCallBackSet().");
        }
        else
        {
            /* Assign event call back and reference data */
            pusbDrvObj->hClientArg     = hReferenceData;
            pusbDrvObj->pEventCallBack = eventCallBack;
        }
    }

}/* end of DRV_USB_UHP_ClientEventCallBackSet() */


// ****************************************************************************
/* Function:
    void DRV_USB_UHP_EndpointToggleClear
    (
        DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
    )

  Summary:
    Facilitates in resetting of endpoint data toggle to 0 for Non Control
    endpoints.

  Description:
    Facilitates in resetting of endpoint data toggle to 0 for Non Control
    endpoints.

  Remarks:
    Refer to drv_usb_uhp_host.h for usage information.
*/

void DRV_USB_UHP_EndpointToggleClear
(
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
)
{
    DRV_USB_UHP_HOST_PIPE_OBJ * pPipe = NULL;
    DRV_USB_UHP_OBJ * hDriver = NULL;

    if ((pipeHandle != DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID) && ((DRV_USB_UHP_HOST_PIPE_HANDLE)NULL != pipeHandle))
    {
        pPipe = (DRV_USB_UHP_HOST_PIPE_OBJ *)pipeHandle;
        hDriver = (DRV_USB_UHP_OBJ *)pPipe->hClient;

        if( (hDriver->hostEndpointTable[pPipe->hostEndpoint].endpoint.pipe->endpointAndDirection & 0x80) == 0 )
        {
            /* Host to Device: OUT */
            /* Clear the Data Toggle for TX Endpoint */
            hDriver->hostEndpointTable[pPipe->hostEndpoint].endpoint.staticDToggleOut = 0;
        }
        else
        {
            /* IN */
            /* Clear the Data Toggle for RX Endpoint */
            hDriver->hostEndpointTable[pPipe->hostEndpoint].endpoint.staticDToggleIn = 0;
        }
    }
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid Pipe handle");
    }

} /* end of DRV_USB_UHP_EndpointToggleClear() */


// *****************************************************************************
// *****************************************************************************
// Root Hub Driver Routines
// *****************************************************************************
// *****************************************************************************


/* **************************************************************************** */
/* Function:
    bool DRV_USB_UHP_ROOT_HUB_OperationIsEnabled(DRV_HANDLE hClient)

   Summary:
    Root hub enable

   Description:
    return true if the HUB is operational and enabled.

   Remarks:
    Refer to .h for usage information.
 */
bool DRV_USB_UHP_ROOT_HUB_OperationIsEnabled(DRV_HANDLE hClient)
{
    DRV_USB_UHP_OBJ *hDriver;

    if ((hClient == DRV_HANDLE_INVALID) || (((DRV_USB_UHP_OBJ *)hClient) == NULL))
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid client");
    }

    hDriver = (DRV_USB_UHP_OBJ *)hClient;
    return(hDriver->operationEnabled);
}/* end of DRV_USB_UHP_ROOT_HUB_OperationIsEnabled() */


/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_ROOT_HUB_Initialize
    (
        DRV_HANDLE handle,
        USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo,
    )

   Summary:
    This function instantiates the root hub driver.

   Description:
    This function initializes the root hub driver. It is called by the host
    layer at the time of processing root hub devices. The host layer assigns a
    USB_HOST_DEVICE_OBJ_HANDLE reference to this root hub driver. This
    identifies the relationship between the root hub and the host layer.

   Remarks:
    None.
*/

void DRV_USB_UHP_ROOT_HUB_Initialize
(
    DRV_HANDLE handle,
    USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo
)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        pUSBDrvObj->usbHostDeviceInfo = usbHostDeviceInfo;
    }
}

/* **************************************************************************** */
/* Function:
    uint8_t DRV_USB_UHP_ROOT_HUB_PortNumbersGet(DRV_HANDLE handle);

   Summary:
    Returns the number of ports this root hub contains.

   Description:
    This function returns the number of ports that this root hub contains.

   Remarks:
    None.
 */

uint8_t DRV_USB_UHP_ROOT_HUB_PortNumbersGet(DRV_HANDLE handle)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    uint8_t result = 0;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        result = 1;
    }

    return(result);
}

/* **************************************************************************** */
/* Function:
    uint32_t DRV_USB_UHP_ROOT_HUB_MaximumCurrentGet(DRV_HANDLE);

   Summary:
    Returns the maximum amount of current that this root hub can provide on the
    bus.

   Description:
    This function returns the maximum amount of current that this root hubn can
    provide on the bus.

   Remarks:
    Refer to .h for usage information.
 */

uint32_t DRV_USB_UHP_ROOT_HUB_MaximumCurrentGet(DRV_HANDLE handle)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    uint32_t result = 0;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        result = pUSBDrvObj->rootHubInfo.rootHubAvailableCurrent;
    }

    return(result);
}

/* **************************************************************************** */
/* Function:
    USB_SPEED DRV_USB_UHP_ROOT_HUB_BusSpeedGet(DRV_HANDLE handle);

  Summary:
    Returns the speed at which the bus to which this root hub is connected is
    operating.

  Description:
    This function returns the speed at which the bus to which this root hub is
    connected is operating.

 Remarks:
    None.
*/

USB_SPEED DRV_USB_UHP_ROOT_HUB_BusSpeedGet(DRV_HANDLE handle)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    USB_SPEED speed = USB_SPEED_ERROR;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");

    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        /* Return the bus speed. This is speed at which the root hub is
         * operating. */
        speed = pUSBDrvObj->deviceSpeed;
    }

    return(speed);
}


/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_ROOT_HUB_PortResume(DRV_HANDLE handle, uint8_t port );

  Summary:
    Resumes the specified root hub port.

  Description:
    This function resumes the root hub. The resume duration is defined by
    DRV_USB_UHP_ROOT_HUB_RESUME_DURATION. The status of the resume signalling can
    be checked using the DRV_USB_UHP_ROOT_HUB_PortResumeIsComplete() function.

  Remarks:
    The root hub on this particular hardware only contains one port - port 0.
*/

USB_ERROR DRV_USB_UHP_ROOT_HUB_PortResume(DRV_HANDLE handle, uint8_t port)
{
    /* The functionality is yet to be implemented. */
    return(USB_ERROR_NONE);
}

/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_ROOT_HUB_PortSuspend(DRV_HANDLE handle, uint8_t port );

   Summary:
    Suspends the specified root hub port.

   Description:
    This function suspends the root hub port.

   Remarks:
    The root hub on this particular hardware only contains one port - port 0.
 */

USB_ERROR DRV_USB_UHP_ROOT_HUB_PortSuspend(DRV_HANDLE handle, uint8_t port)
{
    /* The functionality is yet to be implemented. */
    return (USB_ERROR_NONE);
}


/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_ROOT_HUB_PortResetIsComplete
    (
        DRV_HANDLE handle,
        uint8_t port
    );

   Summary:
    Returns true if the root hub has completed the port reset operation.

   Description:
    This function returns true if the port reset operation has completed. It
    should be called after the DRV_USB_HOST_ROOT_HUB_PortReset() function to
    check if the reset operation has completed.

   Remarks:
    Refer to .h for usage information.
 */

bool DRV_USB_UHP_ROOT_HUB_PortResetIsComplete
(
    DRV_HANDLE handle,
    uint8_t port
)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    bool result = true;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        _DRV_USB_UHP_HOST_RESET_STATE_MACHINE((DRV_USB_UHP_OBJ *)handle);
        /* Return false if the driver is still resetting*/
        result = (pUSBDrvObj->isResetting) ? false : true;
    }

    return(result);
}

/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_ROOT_HUB_PortReset(DRV_HANDLE handle, uint8_t port );

   Summary:
    Resets the specified root hub port.

   Description:
    This function resets the root hub port. The reset duration is defined by
    DRV_USB_UHP_ROOT_HUB_RESET_DURATION. The status of the reset signalling can be
    checked using the DRV_USB_UHP_ROOT_HUB_PortResetIsComplete() function.

   Remarks:
    Refer to .h for usage information.
 */

USB_ERROR DRV_USB_UHP_ROOT_HUB_PortReset(DRV_HANDLE handle, uint8_t port)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    USB_ERROR result = USB_ERROR_NONE;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
        result = USB_ERROR_PARAMETER_INVALID;

    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
        result = USB_ERROR_PARAMETER_INVALID;
    }
    else if(pUSBDrvObj->isResetting)
    {
        /* This means a reset is already in progress. Lets not do anything. */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Reset already in progress");
    }
    else
    {
        /* Start the reset signal. Set the driver flag to indicate the reset
         * signal is in progress. Start generating the reset signal.
         */

        pUSBDrvObj->isResetting = true;
        pUSBDrvObj->resetState  = DRV_USB_UHP_HOST_RESET_STATE_START;
    }

    return(result);
}

/* **************************************************************************** */
/* Function:
    USB_SPEED DRV_USB_UHP_ROOT_HUB_PortSpeedGet
    (
        DRV_HANDLE handle,
        uint8_t port
    );

   Summary:
    Returns the speed of at which the port is operating.

   Description:
    This function returns the speed at which the port is operating.

   Remarks:
    Refer to drv_usb_uhp.h for usage information.
*/

USB_SPEED DRV_USB_UHP_ROOT_HUB_PortSpeedGet(DRV_HANDLE handle, uint8_t port)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    USB_SPEED speed = USB_SPEED_ERROR;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if(!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        /* The driver will not check if a device is connected. It is assumed
         * that the client has issued a port reset before calling this function
         */
        speed = pUSBDrvObj->deviceSpeed;
    }

    return(speed);
}

