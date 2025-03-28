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

#include "usb/src/usb_external_dependencies.h"
#include "driver/usb/usbfsv1/src/drv_usbfsv1_local.h"
#include "interrupts.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/******************************************************
 * Hardware instance, endpoint table and client object
 * lumped together as group to save memory.
 ******************************************************/
static DRV_USBFSV1_OBJ gDrvUSBFSV1Obj [DRV_USBFSV1_INSTANCES_NUMBER];

// *****************************************************************************
// *****************************************************************************
// Section: USB Controller Driver Interface Implementations
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* MISRA C-2012 Rule 11.3, and 11.8 deviated below. Deviation record ID -  
    H3_USB_MISRAC_2012_R_11_3_DR_1, H3_USB_MISRAC_2012_R_11_8_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:1 "MISRA C-2012 Rule 11.3" "H3_USB_MISRAC_2012_R_11_1_DR_1" )\
(deviate:1 "MISRA C-2012 Rule 11.8" "H3_USB_MISRAC_2012_R_11_8_DR_1" )
</#if>
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
    the driver initialization. The DRV_USBFSV1_Open function will fail if the
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
    uint32_t regValue;
    volatile uint32_t usbCalibValue;
    uint16_t usbPadValue;

    if(drvIndex >= DRV_USBFSV1_INSTANCES_NUMBER)
    {
        /* The driver module index specified does not exist in the system */
        SYS_DEBUG(SYS_ERROR_INFO,"\r\nDRV USB USBFSV1: Invalid Driver Module Index in DRV_USBFSV1_Initialize().");
    }
    else if(gDrvUSBFSV1Obj[drvIndex].inUse == true)
    {
        /* Cannot initialize an object that is already in use. */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nDRV USB USBFSV1: Driver is already initialized in DRV_USBFSV1_Initialize().");
    }
    else
    {
        /* Assign to the local pointer the init data passed */
        usbInit = (DRV_USBFSV1_INIT *) init;
        drvObj = &gDrvUSBFSV1Obj[drvIndex];

        /* Create the global mutex and proceed if successful. */
        if((OSAL_RESULT)OSAL_RESULT_TRUE == OSAL_MUTEX_Create((OSAL_MUTEX_HANDLE_TYPE *)&drvObj->mutexID))
        {
            /* Populate the driver instance object with required data */
            drvObj->inUse = true;
            drvObj->status = SYS_STATUS_BUSY;
            drvObj->usbID = usbInit->usbID;
            drvObj->operationMode = usbInit->operationMode;
            drvObj->isOpened = false;
            drvObj->pEventCallBack = NULL;

            drvObj->sessionInvalidEventSent = false;
            drvObj->interruptSource  = usbInit->interruptSource;
            drvObj->interruptSource1  = usbInit->interruptSource1;
            drvObj->interruptSource2  = usbInit->interruptSource2;
            drvObj->interruptSource3  = usbInit->interruptSource3;
            drvObj->isInInterruptContext = false;

            /* Set the configuration */
            drvObj->usbID->HOST.USB_CTRLA = USB_CTRLA_SWRST_Msk;
            while ((drvObj->usbID->HOST.USB_SYNCBUSY & USB_SYNCBUSY_SWRST_Msk) != 0U)
            {
                /* Do Nothing */
            }
            
            /* Change QOS values to have the best performance and correct USB behaviour */
            drvObj->usbID->DEVICE.USB_QOSCTRL = (USB_QOSCTRL_DQOS(2) | USB_QOSCTRL_CQOS(2));
            
            /* Write linearity calibration in BIASREFBUF and bias calibration in BIASCOMP */     
            usbCalibValue = DRV_USBFSV1_READ_PADCAL_VALUE;   
            
            usbPadValue = (uint16_t)(usbCalibValue & 0x001FU);
            if(usbPadValue == 0x001FU)
            {
                usbPadValue = 5;
            }
            drvObj->usbID->DEVICE.USB_PADCAL |= USB_PADCAL_TRANSN(usbPadValue);
            
            usbPadValue = (uint16_t)((usbCalibValue >> 5) & 0x001FU);
            if(usbPadValue == 0x001FU)
            {
                usbPadValue = 29;
            }
            drvObj->usbID->DEVICE.USB_PADCAL |= USB_PADCAL_TRANSP(usbPadValue);
            
            usbPadValue = (uint16_t)((usbCalibValue >> 10) & 0x0007U);
            if(usbPadValue == 0x0007U)
            {
                usbPadValue = 3;
            }
            drvObj->usbID->DEVICE.USB_PADCAL |= USB_PADCAL_TRIM(usbPadValue);

            if(usbInit->runInStandby == true)
            {
                drvObj->usbID->HOST.USB_CTRLA |= USB_CTRLA_RUNSTDBY_Msk;
            }

            if(drvObj->operationMode == DRV_USBFSV1_OPMODE_HOST)
            {
                /* Host mode specific driver initialization */
                M_DRV_USBFSV1_HOST_INIT(drvObj, drvIndex, usbInit);
            }
            else if(drvObj->operationMode == DRV_USBFSV1_OPMODE_DEVICE)
            {
                drvObj->usbID->DEVICE.USB_DESCADD = (uint32_t)(drvObj->endpointDescriptorTable);

                /* Set the starting VBUS level. */
                drvObj->vbusLevel = DRV_USB_VBUS_LEVEL_INVALID;
                drvObj->vbusComparator = usbInit->vbusComparator;

                regValue = drvObj->usbID->DEVICE.USB_CTRLB;
                regValue &= ~USB_DEVICE_CTRLB_SPDCONF_Msk;                

                if (USB_SPEED_FULL == usbInit->operationSpeed)
                {
                    regValue |= USB_DEVICE_CTRLB_SPDCONF(USB_DEVICE_CTRLB_SPDCONF_FS_Val);
                }
                else if(USB_SPEED_LOW == usbInit->operationSpeed)
                {
                    regValue |= USB_DEVICE_CTRLB_SPDCONF(USB_DEVICE_CTRLB_SPDCONF_LS_Val);
                }
                else
                {
                    /* Do Nothing */
                }
                
                drvObj->usbID->DEVICE.USB_CTRLB = (uint16_t)regValue;

                /* Device mode specific driver initialization */
                M_DRV_USBFSV1_DEVICE_INIT(drvObj, drvIndex);

            }
            else
            {
                /* Do nothing */
            }

            /* Enable the USB device mode operation. */
            drvObj->usbID->HOST.USB_CTRLA |= USB_CTRLA_ENABLE_Msk;

            while ((drvObj->usbID->HOST.USB_SYNCBUSY & USB_SYNCBUSY_ENABLE_Msk) == USB_SYNCBUSY_ENABLE_Msk)
            {
                /* Do Nothing */
            }

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
} /* end of DRV_USBFSV1_Initialize() */


<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.3"
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */
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

    hDriver = &gDrvUSBFSV1Obj[object];

    if(hDriver->status <= SYS_STATUS_UNINITIALIZED)
    {
        /* Driver is not initialized */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nDRV USB USBFSV1: Driver not yet initialized in DRV_USBFSV1_Tasks().");
    }
    else
    {
        /* We check for the VBUS level and generate events if a client
         * event handler is registered. */

        if(hDriver->pEventCallBack != NULL && (hDriver->operationMode == DRV_USBFSV1_OPMODE_DEVICE))
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
                    hDriver->pEventCallBack(hDriver->hClientArg, (DRV_USB_EVENT)DRV_USBFSV1_EVENT_DEVICE_SESSION_VALID, NULL);

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
                        hDriver->pEventCallBack(hDriver->hClientArg, (DRV_USB_EVENT)DRV_USBFSV1_EVENT_DEVICE_SESSION_INVALID, NULL);
                        hDriver->sessionInvalidEventSent = true;
                    }
                }

                hDriver->vbusLevel = vbusLevel;
            }
        }
        else if(hDriver->operationMode == DRV_USBFSV1_OPMODE_HOST)
        {
            /* Host mode specific polled
             * task routines can be called here */

             M_DRV_USBFSV1_HOST_ATTACH_DETACH_STATE_MACHINE(hDriver);
        }
        else
        {
            /* Do Nothing */
        }
    }
}/* end of DRV_USBFSV1_Tasks() */

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

    if(object == SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid object */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSB USBFSV1 Driver: Invalid object in DRV_USBFSV1_Deinitialize()");
    }
    else if(object >= DRV_USBFSV1_INSTANCES_NUMBER)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSB USBFSV1 Driver: Invalid object in DRV_USBFSV1_Deinitialize()");
    }
    else if(gDrvUSBFSV1Obj[object].inUse == false)
    {
        /* Cannot de-initialize an object that is not in use. */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSB USBFSV1 Driver: Driver not initialized in DRV_USBFSV1_Deinitialize()");
    }
    else
    {
        hDriver = &gDrvUSBFSV1Obj[object];

        /* Release the USB instance object */
        hDriver->inUse = false;

        /* De-initialize the status*/
        hDriver->status = SYS_STATUS_UNINITIALIZED;

        /* Reset the open flag */
        hDriver->isOpened = false;

        /* Delete the mutex */
        (void) OSAL_MUTEX_Delete((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);

        hDriver->pEventCallBack = NULL;

        /* Clear and disable the interrupts */
        M_DRV_USBFSV1_SYS_INT_SourceDisable(
                (INT_SOURCE)hDriver->interruptSource,
                (INT_SOURCE)hDriver->interruptSource1,
                (INT_SOURCE)hDriver->interruptSource2,
                (INT_SOURCE)hDriver->interruptSource3 );

        M_DRV_USBFSV1_SYS_INT_SourceStatusClear(
                (INT_SOURCE)hDriver->interruptSource,
                (INT_SOURCE)hDriver->interruptSource1,
                (INT_SOURCE)hDriver->interruptSource2,
                (INT_SOURCE)hDriver->interruptSource3 );

        hDriver->usbID->DEVICE.USB_CTRLA &= ~USB_CTRLA_ENABLE_Msk;

        while (hDriver->usbID->DEVICE.USB_SYNCBUSY == USB_SYNCBUSY_ENABLE_Msk)
        {
            /* Do Nothing */
        }
    }

} /* end of DRV_USBFSV1_Deinitialize() */

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
    uint32_t temp_32;

    /* Check if the specified driver index is in valid range */
    if(drvIndex >= DRV_USBFSV1_INSTANCES_NUMBER)
    {
        SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB USBFSV1 Driver: Bad Driver Index in DRV_USBFSV1_Open().");
    }
    else
    {
        drvObj = &gDrvUSBFSV1Obj[drvIndex];
        temp_32 = ((uint32_t)DRV_IO_INTENT_EXCLUSIVE | (uint32_t)DRV_IO_INTENT_NONBLOCKING | (uint32_t)DRV_IO_INTENT_READWRITE);
        if(drvObj->status != SYS_STATUS_READY)
        {
            /* The driver status not ready */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB USBFSV1 Driver: Driver status not ready in DRV_USBFSV1_Open().");
        }
        else if(ioIntent != (DRV_IO_INTENT)temp_32)
        {
            /* The driver only supports this mode */
            SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB USBFSV1 Driver: Unsupported IO Intent in DRV_USBFSV1_Open().");
        }
        else
        {
            /* Do Nothing */
        }
        if(drvObj->isOpened == true)
        {
            /* Driver supports exclusive open only */
            SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSB USBFSV1 Driver: Driver can be opened only once. Multiple calls to DRV_USBFSV1_Open().");
        }
        else
        {
            /* Open the Driver Object */
            drvObj->isOpened = true;

            /* Clear prior value */
            drvObj->pEventCallBack = NULL;

            /* Handle is the pointer to the client object */
            retVal = ((DRV_HANDLE) drvObj);
        }
    }
    
    /* Return handle */
    return (retVal);
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

    if(object == SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid object */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFSV1 Driver: System Module Object is invalid in DRV_USBFSV1_Status().");
        retVal = SYS_STATUS_ERROR;
    }
    else
    {
        retVal = gDrvUSBFSV1Obj[object].status;
    }

    /* Return the status of the driver object */
    return (retVal);

}/* end of DRV_USBFSV1_Status() */

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
    DRV_USBFSV1_OBJ * hDriver;

    if(handle == DRV_HANDLE_INVALID)
    {
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB USBFSV1 Driver: Invalid driver handle in DRV_USBFSV1_Close().");
    }
    else
    {
        /* Reset the relevant parameters */
        hDriver = (DRV_USBFSV1_OBJ *)handle;

        if(false == hDriver->isOpened)
        {
            /* Cannot close a driver handle that is not open */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB USBFSV1 Driver: Driver handle not opened in DRV_USBFSV1_Close().");
        }
        else
        {
            /* Release the client handle */
            hDriver->isOpened = false;
            hDriver->pEventCallBack = NULL;
        }
    }
}/* end of DRV_USBFSV1_Close() */

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
    DRV_USBFSV1_OBJ * drvObj;

    if(object == SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid object */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSB USBFSV1 Driver: Invalid object in DRV_USBFSV1_Tasks_ISR()");
    }
    else
    {
        drvObj = &gDrvUSBFSV1Obj[object];

        if(drvObj->status != SYS_STATUS_READY)
        {
            /* Invalid object */
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSB USBFSV1 Driver: Invalid System Status in DRV_USBFSV1_Tasks_ISR()");
        }
        else
        {

            /* Clear the interrupt */
            M_DRV_USBFSV1_SYS_INT_SourceStatusClear(
                    (INT_SOURCE)drvObj->interruptSource,
                    (INT_SOURCE)drvObj->interruptSource1,
                    (INT_SOURCE)drvObj->interruptSource2,
                    (INT_SOURCE)drvObj->interruptSource3 );

            /* We are entering an interrupt context */
            drvObj->isInInterruptContext = true;

            switch(drvObj->operationMode)
            {
                case DRV_USBFSV1_OPMODE_DEVICE:

                    /* Driver is running in Device Mode */
                    M_DRV_USBFSV1_DEVICE_TASKS_ISR(drvObj);
                    break;

                case DRV_USBFSV1_OPMODE_HOST:

                    /* Driver is running in Host Mode */
                    M_DRV_USBFSV1_HOST_TASKS_ISR(drvObj);
                    break;

                case DRV_USBFSV1_OPMODE_OTG:
                    /* OTG mode is not supported yet */
                    break;

                case DRV_USBFSV1_OPMODE_DUAL_ROLE:
                    /* Dual mode is not supported yet */
                    break;

                default:
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBFSV1 Driver: What mode are you trying?");
                    break;
            }

            /* We are exiting an interrupt context */
            drvObj->isInInterruptContext = false;
        }
    }

}/* end of DRV_USBFSV1_Tasks_ISR() */


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

<#if __PROCESSOR?matches("ATSAMD2.*") || __PROCESSOR?matches("ATSAML2.*") == true || __PROCESSOR?matches("ATSAMR2.*") == true || __PROCESSOR?matches("ATSAMR3.*") == true || __PROCESSOR?matches("ATSAMD1.*") == true || __PROCESSOR?matches("ATSAMDA.*") == true>
void DRV_USBFSV1_USB_Handler(void)
{
    M_DRV_USBFSV1_ISR_OTHER(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */
<#elseif __PROCESSOR?matches("ATSAMD5.*") == true || __PROCESSOR?matches("PIC32CK.*") == true || __PROCESSOR?matches("PIC32CX.*") == true || __PROCESSOR?matches("LAN9255.*") == true || __PROCESSOR?matches("ATSAME5.*") == true>

#if defined (DRV_USBFSV1_MULTIPLE_ISR_AVAILABLE) && (DRV_USBFSV1_MULTIPLE_ISR_AVAILABLE == true)

void DRV_USBFSV1_OTHER_Handler(void)
{
    M_DRV_USBFSV1_ISR_OTHER(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */

void DRV_USBFSV1_SOF_HSOF_Handler(void)
{
    M_DRV_USBFSV1_ISR_SOF_HSOF(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */

void DRV_USBFSV1_TRCPT0_Handler(void)
{
    M_DRV_USBFSV1_ISR_TRCPT0(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */

void DRV_USBFSV1_TRCPT1_Handler(void)
{
    M_DRV_USBFSV1_ISR_TRCPT1(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */

#endif
<#elseif __PROCESSOR?matches("PIC32CM.*") == true >
<#if USB_INT_SOURCE_NUMBER == 4>
void USB_EORSMDNRSM_Handler(void)
{
    M_DRV_USBFSV1_ISR_OTHER(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */

void USB_SOFHSOF_Handler(void)
{
    M_DRV_USBFSV1_ISR_OTHER(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */

void USB_TRCPT00_Handler(void)
{
    M_DRV_USBFSV1_ISR_OTHER(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */

void USB_TRCPT10_Handler(void)
{
    M_DRV_USBFSV1_ISR_OTHER(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */

<#elseif USB_INT_SOURCE_NUMBER == 1>
void DRV_USBFSV1_USB_Handler(void)
{
    M_DRV_USBFSV1_ISR_OTHER(sysObj.drvUSBFSV1Object);

}/* end of USB_Handler() */
</#if>

</#if>
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
    DRV_HANDLE   handle,
    uintptr_t    hReferenceData,
    DRV_USB_EVENT_CALLBACK eventCallBack
)
{
    DRV_USBFSV1_OBJ * pusbDrvObj;

    if(handle == DRV_HANDLE_INVALID)
    {
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB USBFSV1 Driver: Invalid client handle in DRV_USBFSV1_ClientEventCallBackSet().");
    }
    else
    {
        pusbDrvObj = (DRV_USBFSV1_OBJ *) handle;

        if(false == pusbDrvObj->isOpened)
        {
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSB USBFSV1 Driver: Invalid client handle in DRV_USBFSV1_ClientEventCallBackSet().");
        }
        else
        {
            /* Assign event call back and reference data */
            pusbDrvObj->hClientArg = hReferenceData;
            pusbDrvObj->pEventCallBack = eventCallBack;
        }
    }

}/* end of DRV_USBFSV1_ClientEventCallBackSet() */
