/*******************************************************************************
  USB Device Driver Implementation of device mode operation routines

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usbdp_device.c

  Summary:
    USB Device Driver Dynamic Implementation of device mode operation routines

  Description:
    The USB device driver provides a simple interface to manage the USB
    modules on Microchip microcontrollers.  This file Implements the
    interface routines for the USB driver when operating in device mode.

    While building the driver from source, ALWAYS use this file in the build if
    device mode operation is required.
 ******************************************************************************/

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
 ******************************************************************************/
//DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************
#include "driver/usb/usbdp/src/drv_usbdp_local.h"
#include "driver/usb/usbdp/drv_usbdp.h"
#include "interrupts.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* USB device driver instance object */
static DRV_USBDP_OBJ gDrvUSBDPObj[DRV_USBDP_INSTANCES_NUMBER];

/* Array of endpoint objects. Two objects for control endpoint, one for each
 * direction */
static DRV_USBDP_ENDPOINT_OBJ gDrvUSBControlEndpoints[DRV_USBDP_INSTANCES_NUMBER] [2];

/* Array of endpoint objects. One object per non-control endpoint as one
 * endpoint can handle one direction at a time */
static DRV_USBDP_ENDPOINT_OBJ gDrvUSBNonControlEndpoints[DRV_USBDP_INSTANCES_NUMBER] [DRV_USBDP_ENDPOINTS_NUMBER - 1];

/* Array of endpoint types to map the endpoint type as per bit values of the
 * UDP_CSR register */
static const uint8_t gDrvUSBDeviceEndpointTypeMap[2][4] =
{
    {(uint8_t)UDP_CSR_EPTYPE_CTRL_Val, (uint8_t)UDP_CSR_EPTYPE_ISO_OUT_Val, (uint8_t)UDP_CSR_EPTYPE_BULK_OUT_Val, (uint8_t)UDP_CSR_EPTYPE_INT_OUT_Val},
    {(uint8_t)UDP_CSR_EPTYPE_CTRL_Val, (uint8_t)UDP_CSR_EPTYPE_ISO_IN_Val, (uint8_t)UDP_CSR_EPTYPE_BULK_IN_Val, (uint8_t)UDP_CSR_EPTYPE_INT_IN_Val}
};

/*******************************************************************************
 * This structure is a pointer to a set of USB Driver Device mode functions.
 * This set is exported to the device layer when the device layer must use the
 * USB Controller.
 ******************************************************************************/

DRV_USB_DEVICE_INTERFACE gDrvUSBDPInterface =
{
    .open = DRV_USBDP_Open,
    .close = DRV_USBDP_Close,
    .eventHandlerSet = DRV_USBDP_ClientEventCallBackSet,
    .deviceAddressSet = DRV_USBDP_AddressSet,
    .deviceCurrentSpeedGet = DRV_USBDP_CurrentSpeedGet,
    .deviceSOFNumberGet = DRV_USBDP_SOFNumberGet,
    .deviceAttach = DRV_USBDP_Attach,
    .deviceDetach = DRV_USBDP_Detach,
    .deviceEndpointEnable = DRV_USBDP_EndpointEnable,
    .deviceEndpointDisable = DRV_USBDP_EndpointDisable,
    .deviceEndpointStall = DRV_USBDP_EndpointStall,
    .deviceEndpointStallClear = DRV_USBDP_EndpointStallClear,
    .deviceEndpointIsEnabled = DRV_USBDP_EndpointIsEnabled,
    .deviceEndpointIsStalled = DRV_USBDP_EndpointIsStalled,
    .deviceIRPSubmit = DRV_USBDP_IRPSubmit,
    .deviceIRPCancel = DRV_USBDP_IRPCancel,
    .deviceIRPCancelAll = DRV_USBDP_IRPCancelAll,
    .deviceRemoteWakeupStop = DRV_USBDP_RemoteWakeupStop,
    .deviceRemoteWakeupStart = DRV_USBDP_RemoteWakeupStart,
    .deviceTestModeEnter = NULL
};


// *****************************************************************************
/* MISRA C-2012 Rule 10.4 False Positive:9 ,Rule 11.3 deviate:18, Rule 11.6 deviate:17 
   and Rule 11.8 deviate:1.Deviation record ID - H3_USB_MISRAC_2012_R_11_3_DR_1, 
   H3_USB_MISRAC_2012_R_11_6_DR_1 and H3_USB_MISRAC_2012_R_11_8_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(fp:9       "MISRA C-2012 Rule 10.4" "H3_USB_MISRAC_2012_R_10_4_DR_1" )\
(deviate:18 "MISRA C-2012 Rule 11.3" "H3_USB_MISRAC_2012_R_11_3_DR_1" )\
(deviate:17 "MISRA C-2012 Rule 11.6" "H3_USB_MISRAC_2012_R_11_6_DR_1" )\
(deviate:1 "MISRA C-2012 Rule 11.8" "H3_USB_MISRAC_2012_R_11_8_DR_1" )
</#if>
/* Function:
    SYS_MODULE_OBJ DRV_USBDP_Initialize
    (
        const SYS_MODULE_INDEX  drvIndex,
        const SYS_MODULE_INIT * const init
    )

  Summary:
    Dynamic implementation of DRV_USBDP_Initialize system interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_Initialize system interface
    function. Function performs the following task:
    - Initializes the necessary USB module as per the instance init data
    - Updates internal data structure for the particular USB instance
    - Returns the USB instance value as a handle to the system

  Remarks:
    See drv_usbdp.h for usage information.
*/

SYS_MODULE_OBJ DRV_USBDP_Initialize
(
    const SYS_MODULE_INDEX  drvIndex,
    const SYS_MODULE_INIT * const init
)
{

    DRV_USBDP_OBJ * drvObj = (DRV_USBDP_OBJ *)NULL;        /* USB driver object pointer */
    DRV_USBDP_INIT * usbInit = (DRV_USBDP_INIT *)NULL;     /* USB driver initialization pointer */
    SYS_MODULE_OBJ retVal = SYS_MODULE_OBJ_INVALID;        /* Return value */
    uint8_t index;                                         /* Loop counter */

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(drvIndex >= DRV_USBDP_INSTANCES_NUMBER)
    {
        /* Driver module index out of range */
        SYS_DEBUG(SYS_ERROR_INFO,"\r\nUSBDP Driver: Driver module index out of range in DRV_USBDP_Initialize().");
    }
    else if(gDrvUSBDPObj[drvIndex].inUse == true)
    {
        /* Driver already initialized */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBDP Driver: Driver already initialized in DRV_USBDP_Initialize().");
    }
    else
    {
        /* Assign the init data and driver object to the local pointer */
        usbInit = (DRV_USBDP_INIT *) init;
        drvObj = &gDrvUSBDPObj[drvIndex];

        /* Create the global mutex and proceed if successful. */
        if(OSAL_RESULT_TRUE == OSAL_MUTEX_Create((OSAL_MUTEX_HANDLE_TYPE *)&drvObj->mutexID))
        {
            /* Mutex create successful, go ahead with initialization */
            /* Populate the driver instance object with required data */
            drvObj->inUse = true;
            drvObj->status = SYS_STATUS_BUSY;
            drvObj->usbID = usbInit->usbID;
            drvObj->operationSpeed = usbInit->operationSpeed;
            drvObj->isOpened = false;
            drvObj->pEventCallBack = NULL;

            drvObj->sessionInvalidEventSent = false;
            drvObj->interruptSource  = (IRQn_Type)usbInit->interruptSource;
            drvObj->isInInterruptContext = false;

            /* Set the starting VBUS level. */
            drvObj->vbusLevel = DRV_USB_VBUS_LEVEL_INVALID;
            drvObj->vbusComparator = usbInit->vbusComparator;

            /* Assign the endpoint object pointer array with the endpoint
             * objects. Control endpoint is bidirectional endpoint, so only one
             * object is needed. Non control endpoints are unidirectional
             * endpoints, so multidimensional array with one object per endpoint
             * direction is used. By this method, the endpoint objects can later
             * be accessed by using the endpoint number as index irrespective of
             * endpoint number. */
            drvObj->deviceEndpointObj[0] = &gDrvUSBControlEndpoints[drvIndex][0];
            for(index = 1; index < DRV_USBDP_ENDPOINTS_NUMBER ; index++)
            {
                drvObj->deviceEndpointObj[index] = &gDrvUSBNonControlEndpoints[drvIndex][index - 1U];
            }

            /* USB Peripheral is configured in Device mode */
            MATRIX_REGS->CCFG_USBMR |= CCFG_USBMR_USBMODE_Msk;

            /* The USB alarm enables a fast restart signal to the PMC. */
            PMC_REGS->PMC_FSMR |= PMC_FSMR_USBAL_Msk;

            /* Disable the function endpoints de-address the device. */
            drvObj->usbID->UDP_FADDR &= ~UDP_FADDR_Msk;

            /* Disable all USB Peripheral Interrupts. We are not touching the
             * USB System Interrupt as the NVIC is not yet initialized. */
            drvObj->usbID->UDP_IDR = UDP_IDR_Msk;

            /* Initialize device specific flags */
            drvObj->isAttached = false;
            drvObj->status = SYS_STATUS_READY;

            retVal = drvIndex;
        }
        else
        {
            /* Mutex create failed. Send a message and exit. */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBDP Driver: Mutex create failed in DRV_USBDP_Initialize().");
        }
    }
    return (retVal);

} /* end of DRV_USBDP_Initialize() */


// *****************************************************************************
/* Function:
    void DRV_USBDP_Deinitialize( const SYS_MODULE_INDEX  drvIndex )

  Summary:
    Dynamic implementation of DRV_USBDP_Deinitialize system interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_Deinitialize system
    interface function.

  Remarks:
    See drv_usbdp.h for usage information.
*/

void DRV_USBDP_Deinitialize
(
    const SYS_MODULE_INDEX  drvIndex
)
{

    DRV_USBDP_OBJ * hDriver = (DRV_USBDP_OBJ *)NULL;       /* Local Driver Object Handler */

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(drvIndex == SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid driver module index */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSBDP Driver: Invalid driver module index in DRV_USBDP_Deinitialize()");
    }
    else if(drvIndex >= DRV_USBDP_INSTANCES_NUMBER)
    {
        /* Driver module index out of range */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSBDP Driver: Driver module index out of range in DRV_USBDP_Deinitialize()");
    }
    else if(gDrvUSBDPObj[drvIndex].inUse == false)
    {
        /* Driver not in use */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSBDP Driver: Driver not in use in DRV_USBDP_Deinitialize()");
    }
    else
    {
        /* Assign the driver object to the local pointer */
        hDriver = &gDrvUSBDPObj[drvIndex];

        /* Release the USB instance object */
        hDriver->inUse = false;

        /* De-initialize the status*/
        hDriver->status = SYS_STATUS_UNINITIALIZED;

        /* Reset the open flag */
        hDriver->isOpened = false;

        /* Delete the mutex */
        (void) OSAL_MUTEX_Delete((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);

        /* Reset the USB driver event callback to NULL */
        hDriver->pEventCallBack = NULL;

        /* Disable and clear all Interrupts */
        (void) SYS_INT_SourceDisable(hDriver->interruptSource);
        UDP_REGS->UDP_IDR = UDP_IDR_Msk;
        UDP_REGS->UDP_ICR = UDP_ICR_Msk;

        hDriver->usbID->UDP_FADDR &= ~UDP_FADDR_FEN_Msk;
    }
} /* end of DRV_USBDP_Deinitialize() */


// *****************************************************************************
/* Function:
    DRV_HANDLE DRV_USBDP_Open
    (
        const SYS_MODULE_INDEX drvIndex,
        const DRV_IO_INTENT intent
    )

  Summary:
    Dynamic implementation of DRV_USBDP_Open system interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_Open system interface
    function.

  Remarks:
    See drv_usbdp.h for usage information.
*/

DRV_HANDLE DRV_USBDP_Open
(
    const SYS_MODULE_INDEX drvIndex,
    const DRV_IO_INTENT    intent
)
{
    DRV_USBDP_OBJ * drvObj;                             /* Local Driver Object Handler */
    DRV_HANDLE retVal = DRV_HANDLE_INVALID;             /* Return Value */


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(drvIndex >= DRV_USBDP_INSTANCES_NUMBER)
    {
        /* Driver module index out of range */
        SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSBDP Driver: Driver module index out of range in DRV_USBDP_Open().");
    }
    else
    {
        /* Assign the driver object to the local pointer */
        drvObj = &gDrvUSBDPObj[drvIndex];
        if(drvObj->status != SYS_STATUS_READY)
        {
            /* The driver status not ready */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBDP Driver: Driver status not ready in DRV_USBDP_Open().");
        }
        else if((uint32_t)intent != ((uint32_t)DRV_IO_INTENT_EXCLUSIVE | (uint32_t)DRV_IO_INTENT_NONBLOCKING | (uint32_t)DRV_IO_INTENT_READWRITE))
        {
            /* Unsupported IO Intent */
            SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSBDP Driver: Unsupported IO Intent in DRV_USBDP_Open().");
        }
        else
        {
            /* Do Nothing */
        }
        if(drvObj->isOpened == true)
        {
            /* Driver can be opened only once. Multiple calls not allowed */
            SYS_DEBUG(SYS_ERROR_DEBUG, "\r\nUSBDP Driver: Driver can be opened only once. Multiple calls to DRV_USBDP_Open().");
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
}/* end of DRV_USBDP_Open() */

// *****************************************************************************
/* Function:
    SYS_STATUS DRV_USBDP_Status( const SYS_MODULE_OBJ object )

  Summary:
    Dynamic implementation of DRV_USBDP_Status system interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_Status system interface
    function.

  Remarks:
    See drv_usbdp.h for usage information.
*/

SYS_STATUS DRV_USBDP_Status
(
    SYS_MODULE_OBJ object
)
{

    SYS_STATUS retVal = SYS_STATUS_UNINITIALIZED;       /* Return Value */


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(object == SYS_MODULE_OBJ_INVALID)
    {
        /* System Module Object Invalid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: System Module Object Invalid in DRV_USBDP_Status().");
        retVal = SYS_STATUS_ERROR;
    }
    else
    {
        retVal = (SYS_STATUS) gDrvUSBDPObj[object].status;
    }

    /* Return the status of the driver object */
    return (retVal);

}/* end of DRV_USBDP_Status() */

// *****************************************************************************
/* Function:
    void DRV_USBDP_Close( DRV_HANDLE client )

  Summary:
    Dynamic implementation of DRV_USBDP_Close client interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_Close client interface
    function.

  Remarks:
    See drv_usbdp.h for usage information.
*/

void DRV_USBDP_Close
(
    DRV_HANDLE handle
)
{

    DRV_USBDP_OBJ * hDriver;                        /* USB driver object pointer */

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_Close().");
    }
    else
    {
        /* Assign the driver handle to the local pointer */
        hDriver = (DRV_USBDP_OBJ *)handle;

        if(false == hDriver->isOpened)
        {
            /* Driver handle not opened */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBDP Driver: Driver handle not opened in DRV_USBDP_Close().");
        }
        else
        {
            /* Release the client handle */
            hDriver->isOpened = false;
            hDriver->pEventCallBack = NULL;
        }
    }
}/* end of DRV_USBDP_Close() */


// *****************************************************************************
/* Function:
    void DRV_USBDP_Tasks(SYS_MODULE_OBJ object)

  Summary:
    Dynamic implementation of DRV_USBDP_Tasks system interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_Tasks system interface
    function.

  Remarks:
    See drv_usbdp.h for usage information.
*/

void DRV_USBDP_Tasks
(
    SYS_MODULE_OBJ object
)
{
    DRV_USBDP_OBJ * hDriver;                                    /* USB driver object pointer */
    DRV_USB_VBUS_LEVEL vbusLevel = DRV_USB_VBUS_LEVEL_INVALID;  /* USB VBUS Level data */


    /* Assign the driver object to the local pointer */
    hDriver = &gDrvUSBDPObj[object];

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(hDriver->status <= SYS_STATUS_UNINITIALIZED)
    {
        /* Driver is not initialized */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBDP Driver: Driver is not initialized in DRV_USBDP_Tasks().");
    }
    else if ((DRV_USB_EVENT_CALLBACK)NULL != hDriver->pEventCallBack)
    {
        /* We have a valid client call back function. If we have a VBUS
         * comparator function, then check is VBUS level is changed. If there is
         * no VBUS comparator function, then return with VBUS Valid. This is in
         * scenario where the device cannot afford a GPIO for VBUS state and
         * hence it is always valid */
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
                hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBDP_EVENT_DEVICE_SESSION_VALID, NULL);

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
                    hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBDP_EVENT_DEVICE_SESSION_INVALID, NULL);
                    hDriver->sessionInvalidEventSent = true;
                }
            }

            hDriver->vbusLevel = vbusLevel;
        }
    }
    else
    {
        /* Do Nothing */
    }
}/* end of DRV_USBDP_Tasks() */

// *****************************************************************************

/* Function:
      void DRV_USBDP_AddressSet(DRV_HANDLE handle, uint8_t address)

  Summary:
    Dynamic implementation of DRV_USBDP_AddressSet client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USBDP_AddressSet
    client interface initialization function for USB device. Function checks
    the input handle validity and on success updates the Device General Control
    Register with the address value and enables the address.

  Remarks:
    See drv_usbdp.h for usage information.
 */

void DRV_USBDP_AddressSet
(
    DRV_HANDLE handle,
    uint8_t address
)
{

    DRV_USBDP_OBJ * hDriver;                            /* USB driver object pointer */
    udp_registers_t * usbID;                            /* USB instance pointer */
    uint32_t regValue;                                  /* USB shadow register */


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_AddressSet().");
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Disable the device address */
        usbID->UDP_GLB_STAT &= ~UDP_GLB_STAT_FADDEN_Msk;

        /* Clear the Function Enable bit to disable the endpoints */
        usbID->UDP_FADDR &= ~UDP_FADDR_FEN_Msk;

        /* Copy new address to FADDR */
        regValue = usbID->UDP_FADDR;
        regValue &= ~UDP_FADDR_FADD_Msk;
        regValue |= UDP_FADDR_FADD(address);
        usbID->UDP_FADDR = regValue;

        /* Enable the new address */
        usbID->UDP_FADDR |= UDP_FADDR_FEN_Msk;
    }
}/* end of DRV_USBDP_AddressSet() */

// *****************************************************************************

/* Function:
      USB_SPEED DRV_USBDP_CurrentSpeedGet(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USBDP_CurrentSpeedGet client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_CurrentSpeedGet
    client interface initialization function for USB device.
    Function checks the input handle validity and on success returns value to
    indicate HIGH/FULL speed operation.

  Remarks:
    See drv_usbdp.h for usage information.
 */

USB_SPEED DRV_USBDP_CurrentSpeedGet
(
    DRV_HANDLE handle
)
{

    DRV_USBDP_OBJ * hDriver;                            /* USB driver object pointer */
    USB_SPEED retVal = USB_SPEED_ERROR;                 /* Return value */


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_CurrentSpeedGet().");
    }
    else
    {
        hDriver = (DRV_USBDP_OBJ *) handle;

        /* The current speed in contained in the
         * device speed member of the driver object */
         retVal = hDriver->deviceSpeed;
    }

    return (retVal);

}/* end of DRV_USBDP_CurrentSpeedGet() */

// *****************************************************************************

/* Function:
      void DRV_USBDP_RemoteWakeupStart(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USBDP_RemoteWakeupStart client
    interface function.

  Description:
    This is dynamic implementation of DRV_USBDP_RemoteWakeupStart
    client interface function for USB device.

  Remarks:
    See drv_usbdp.h for usage information.
 */

void DRV_USBDP_RemoteWakeupStart
(
    DRV_HANDLE handle
)
{

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_RemoteWakeupStart().");
    }
    else
    {
        /* Not yet supported. Do nothing. */
    }

}/* end of DRV_USBDP_RemoteWakeupStart() */

// *****************************************************************************

/* Function:
      void DRV_USBDP_RemoteWakeupStop(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USBDP_RemoteWakeupStop client
    interface function.

  Description:
    This is dynamic implementation of DRV_USBDP_RemoteWakeupStop
    client interface function for USB device.

  Remarks:
    See drv_usbdp.h for usage information.
 */

void DRV_USBDP_RemoteWakeupStop(DRV_HANDLE handle)
{

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_RemoteWakeupStop().");
    }
    else
    {
        /* Not yet supported. Do nothing. */
    }

}/* end of DRV_USBDP_RemoteWakeupStop() */

// *****************************************************************************

/* Function:
      void DRV_USBDP_Attach(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USBDP_Attach client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_Attach
    client interface function for USB device.
    This function checks if the handle passed is valid and if so, performs the
    device attach operation. EOR, SUSP, WAKEUP, & SOF interrupts are enabled and
    EOR, WAKEUP, & SOF interrupts are cleared.

  Remarks:
    See drv_usbdp.h for usage information.
 */

void DRV_USBDP_Attach
(
    DRV_HANDLE handle
)
{

    DRV_USBDP_OBJ * hDriver;                            /* USB driver object pointer */
    udp_registers_t * usbID;                            /* USB instance pointer */


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_Attach().");
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Update the driver flag indicating attach */
        hDriver->isAttached = true;

        /* Enable the USB device by clearing the TXVDIS bit.
         * And Set the PUON bit to enable the Pull-Up. */
        usbID->UDP_TXVC &= ~UDP_TXVC_TXVDIS_Msk;
        usbID->UDP_TXVC |= UDP_TXVC_PUON_Msk;

        /* Enables all interrupts except RESUME. RESUMEIF will be enabled only
         * on getting SUSPEND */
        usbID->UDP_IER = ( UDP_IER_RXSUSP_Msk |
                            UDP_IER_WAKEUP_Msk |
                            UDP_IER_RXRSM_Msk |
                            UDP_IER_SOFINT_Msk);

        /* Enable the USB interrupt */
        SYS_INT_SourceEnable(hDriver->interruptSource);
    }

}/* end of DRV_USBDP_Attach() */

// *****************************************************************************

/* Function:
      void DRV_USBDP_Detach(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USBDP_Detach client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_Detach
    client interface function for USB device.
    This function checks if the passed handle is valid and if so, performs a
    device detach operation.

  Remarks:
    See drv_usbdp.h for usage information.
 */

void DRV_USBDP_Detach
(
    DRV_HANDLE handle
)
{

    DRV_USBDP_OBJ * hDriver;                      /* USB driver object pointer */
    udp_registers_t * usbID;                        /* USB instance pointer */
    uint32_t regValue;                              /* USB shadow register */
    bool interruptWasEnabled = false;               /* USB interrupt status holder */
    USB_ERROR retVal = USB_ERROR_NONE;              /* Return value */


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_Detach().");
    }
    else if(false == ((DRV_USBDP_OBJ *)handle)->inUse)
    {
        /* Driver Object not in use */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Driver Object not in use in DRV_USBDP_Detach().");
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Check if we are in interrupt context. This is to ensure that this function
         * is interrupt and thread safe. If we are not in interrupt, we disable
         * the interrupt and grab the mutex. If we are already in interrupt
         * context, there is no need to disable interrupt or grab mutex */
        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Mutex lock failed in DRV_USBDP_Detach()");
                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }
        if(retVal == USB_ERROR_NONE)
        {
            /* Update the driver flag indicating detach */
            hDriver->isAttached = false;

            /* Disable the endpoints first */
            (void) DRV_USBDP_EndpointDisable((DRV_HANDLE)hDriver, DRV_USB_DEVICE_ENDPOINT_ALL);

            /* Clear and disable all the interrupts */
            usbID->UDP_IDR = UDP_IDR_Msk;
            usbID->UDP_ICR = UDP_ICR_Msk;

            /* Reset the endpoints */
            usbID->UDP_RST_EP = UDP_RST_EP_Msk;
            usbID->UDP_RST_EP &= ~UDP_RST_EP_Msk;

            /* Disable the function address enable */
            usbID->UDP_GLB_STAT &= ~UDP_GLB_STAT_FADDEN_Msk;
            usbID->UDP_FADDR &= ~UDP_FADDR_FEN_Msk;

            /* Disable the USB device by setting the TXVDIS bit.
             * And Clear the PUON bit to disable the Pull-Up. */
            regValue = usbID->UDP_TXVC;
            regValue &= ~UDP_TXVC_Msk;
            regValue |= UDP_TXVC_TXVDIS_Msk;
            usbID->UDP_TXVC = regValue;

            if(hDriver->isInInterruptContext == false)
            {
                /* We were not in interrupt context. Restore the interrupt 
                 * status and unlock the Mutex */
                SYS_INT_SourceRestore(hDriver->interruptSource, interruptWasEnabled);
                (void) OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);
            }
        }
    }

}/* end of DRV_USBDP_Detach() */

// *****************************************************************************

/* Function:
      uint16_t DRV_USBDP_SOFNumberGet(DRV_HANDLE client)

  Summary:
    Dynamic implementation of DRV_USBDP_SOFNumberGet client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_SOFNumberGet
    client interface function for USB device.
    Function checks the validity of the input arguments and on success returns
    the Frame count value.

  Remarks:
    See drv_usbdp.h for usage information.
 */

uint16_t DRV_USBDP_SOFNumberGet(DRV_HANDLE handle)
{
    DRV_USBDP_OBJ * hDriver;              /* USB driver object pointer */
    udp_registers_t * usbID;                /* USB instance pointer */
    uint16_t retVal = 0;                    /* Return value */


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_SOFNumberGet().");
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Get the Frame count */
        retVal = (uint16_t)(usbID->UDP_FRM_NUM & UDP_FRM_NUM_FRM_NUM_Msk);

    }
    return retVal;

}/* end of DRV_USBDP_SOFNumberGet() */

// *****************************************************************************

/* Function:
    void F_DRV_USBDP_IRPQueueFlush
    (
        DRV_USBDP_ENDPOINT_OBJ * endpointObj,
        USB_DEVICE_IRP_STATUS status
    )

  Summary:
    Dynamic implementation of F_DRV_USBDP_IRPQueueFlush function.

  Description:
    This is the dynamic implementation of F_DRV_USBDP_IRPQueueFlush
    function for USB device.
    Function scans for all the IRPs on the endpoint queue and cancels them all.

  Remarks:
    This is a local function and should not be called directly by the
    application.
 */

void F_DRV_USBDP_IRPQueueFlush
(
    DRV_USBDP_ENDPOINT_OBJ * endpointObj,
    USB_DEVICE_IRP_STATUS status
)

{
    USB_DEVICE_IRP_LOCAL * iterator = NULL;     /* IRP pointer for parsing */


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(endpointObj != NULL)
    {
        /* Check if any IRPs are assigned on this endpoint and abort them */
        if(endpointObj->irpQueue != NULL)
        {
            /* Scan for all the IRPs on this endpoint. Cancel the IRP and
             * deallocate driver IRP objects */
            iterator = endpointObj->irpQueue;
            while(iterator != NULL)
            {
                iterator->status = status;
                if(iterator->callback != NULL)
                {
                    iterator->callback((USB_DEVICE_IRP *)iterator);
                }
                iterator = iterator->next;
            }
        }

        /* Set the head pointer to NULL */
        endpointObj->irpQueue = NULL;
    }

}/* end of F_DRV_USBDP_IRPQueueFlush() */

// *****************************************************************************

/* Function:
    void F_DRV_USBDP_EndpointObjectEnable
    (
        DRV_USBDP_ENDPOINT_OBJ * endpointObj,
        uint16_t endpointSize,
        USB_TRANSFER_TYPE endpointType,
        USB_DATA_DIRECTION endpointDirection
    )

  Summary:
    Dynamic implementation of F_DRV_USBDP_EndpointObjectEnable
    function.

  Description:
    This is the dynamic implementation of
    F_DRV_USBDP_EndpointObjectEnable function for USB device.
    Function populates the endpoint object data structure and sets it to
    enabled state.

  Remarks:
    This is a local function and should not be called directly by the
    application.
 */

void F_DRV_USBDP_EndpointObjectEnable
(
    DRV_USBDP_ENDPOINT_OBJ * endpointObj,
    uint16_t endpointSize,
    USB_TRANSFER_TYPE endpointType
)

{
    uint32_t endPointRead;
    /* This is a helper function to have the endpoint related information handy */
    endpointObj->irpQueue        = NULL;
    endpointObj->maxPacketSize   = endpointSize;
    endpointObj->endpointType    = endpointType;
    endPointRead = (uint32_t)endpointObj->endpointState  | (uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED;
    endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endPointRead;

}/* end of F_DRV_USBDP_EndpointObjectEnable() */

// *****************************************************************************
/* MISRA C-2012 Rule 20.7 deviated:43 Deviation record ID -  H3_USB_MISRAC_2012_R_20_7_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance block deviate:43 "MISRA C-2012 Rule 20.7" "H3_USB_MISRAC_2012_R_20_7_DR_1" 
</#if>
/* Function:
    USB_ERROR DRV_USBDP_EndpointEnable
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection,
        USB_TRANSFER_TYPE transferType,
        uint16_t endpointSize
    )

  Summary:
    Dynamic implementation of DRV_USBDP_EndpointEnable client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_EndpointEnable
    client interface function for USB device.
    Function enables the specified endpoint.

  Remarks:
    See drv_usbdp.h for usage information.
 */

USB_ERROR DRV_USBDP_EndpointEnable
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection,
    USB_TRANSFER_TYPE transferType,
    uint16_t endpointSize
)

{
    /* This function can be called from from the USB ISR. Because an endpoint
     * can be owned by one client only, we don't need mutex protection in this
     * function */
    
    DRV_USBDP_OBJ * hDriver;                    /* USB driver object pointer */
    udp_registers_t * usbID;                    /* USB instance pointer */
    DRV_USBDP_ENDPOINT_OBJ * endpointObj;       /* Endpoint object pointer */
    uint8_t direction;                          /* Endpoint direction */
    uint8_t endpoint;                           /* Endpoint number */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */


    /* Get the endpoint number and its direction */
    endpoint = endpointAndDirection & DRV_USBDP_ENDPOINT_NUMBER_MASK;
    direction = (uint8_t)((endpointAndDirection & DRV_USBDP_ENDPOINT_DIRECTION_MASK) != 0U);

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(endpoint >= DRV_USBDP_ENDPOINTS_NUMBER)
    {
        /* Endpoint number out of range */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Endpoint number out of range in DRV_USBDP_EndpointEnable().");
        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_EndpointEnable().");
        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Get the endpoint object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        /* Continue with endpoint enable based on the type of endpoint. If it is
         * Control, both directions have to be enabled. Else enable only the
         * direction that was received */
        if(transferType == USB_TRANSFER_TYPE_CONTROL)
        {
            /* There are two endpoint objects for a control endpoint.
             * Enable the first endpoint object */
            F_DRV_USBDP_EndpointObjectEnable(endpointObj, endpointSize, USB_TRANSFER_TYPE_CONTROL);

            endpointObj++;

             /* Enable the second endpoint object */
            F_DRV_USBDP_EndpointObjectEnable(endpointObj, endpointSize, USB_TRANSFER_TYPE_CONTROL);

            /* Reset the endpoints to their default values */
            usbID->UDP_RST_EP = UDP_RST_EP_EP0_Msk;
            usbID->UDP_RST_EP = 0;

            /* Clear the direction and endpoint types */
            USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_DIR_Msk | UDP_CSR_EPTYPE_Msk)

            /* Enable the endpoint as control endpoint */
            USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_EPEDS_Msk)

            /* Enable the interrupt for this endpoint */
            usbID->UDP_IER = UDP_IER_EP0INT_Msk;
        }
        else
        {
            /* Enable the non-zero endpoint object */
            F_DRV_USBDP_EndpointObjectEnable(endpointObj, endpointSize, transferType);

            /* Reset the endpoints to their default values */
            usbID->UDP_RST_EP = (UDP_RST_EP_EP0_Msk << endpoint);
            usbID->UDP_RST_EP = 0;

            /* Clear the direction and endpoint types */
            USBDP_CSR_CLR_BITS(usbID, endpoint, UDP_CSR_EPTYPE_Msk)

            /* Enable the endpoint as control endpoint */
            USBDP_CSR_SET_BITS(usbID, endpoint, UDP_CSR_EPTYPE(gDrvUSBDeviceEndpointTypeMap[direction][transferType]) | UDP_CSR_EPEDS_Msk)

            /* Enable the interrupt for this endpoint */
            usbID->UDP_IER = UDP_IER_EP0INT_Msk << endpoint;
        }
    }
    return (retVal);

}/* end of DRV_USBDP_EndpointEnable() */

// *****************************************************************************

/* Function:
    USB_ERROR DRV_USBDP_EndpointDisable
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    Dynamic implementation of DRV_USBDP_EndpointDisable client
    interface function.

  Description:
    This is dynamic implementation of DRV_USBDP_EndpointDisable
    client interface function for USB device.
    Function disables the specified endpoint.

  Remarks:
    See drv_usbdp.h for usage information.
 */

USB_ERROR DRV_USBDP_EndpointDisable
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{
    /* This routine disables the specified endpoint. It does not check if there 
     * is any ongoing communication on the bus through the endpoint */

    DRV_USBDP_OBJ * hDriver;                    /* USB driver object pointer */
    udp_registers_t * usbID;                    /* USB instance pointer */
    DRV_USBDP_ENDPOINT_OBJ * endpointObj;       /* Endpoint object pointer */
    uint8_t index;                          /* Loop counter */
    uint8_t endpoint;                           /* Endpoint number */
    bool interruptWasEnabled = false;           /* USB interrupt status holder */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */
    uint32_t endpointRead;


    /* Get the endpoint number */
    endpoint = endpointAndDirection & DRV_USBDP_ENDPOINT_NUMBER_MASK;

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((endpoint >= DRV_USBDP_ENDPOINTS_NUMBER) && (endpointAndDirection != DRV_USBDP_ENDPOINT_ALL))
    {
        /* Endpoint number out of range */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Endpoint number out of range in DRV_USBDP_EndpointDisable().");
        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_EndpointDisable().");
        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Check if we are in interrupt context. This is to ensure that this function
         * is interrupt and thread safe. If we are not in interrupt, we disable
         * the interrupt and grab the mutex. If we are already in interrupt 
         * context, there is no need to disable interrupt or grab mutex */
        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Mutex lock failed in DRV_USBDP_EndpointDisable()");
                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }

        if(retVal == USB_ERROR_NONE)
        {
            /* Disable the endpoints based on the input. Either all the 
             * endpoints or control endpoint or specific non control endpoint */
            if(endpointAndDirection == DRV_USBDP_ENDPOINT_ALL)
            {
                /* Disable all the endpoint interrupts */
                usbID->UDP_IDR = (  UDP_IDR_EP0INT_Msk | 
                                    UDP_IDR_EP1INT_Msk | 
                                    UDP_IDR_EP2INT_Msk | 
                                    UDP_IDR_EP3INT_Msk | 
                                    UDP_IDR_EP4INT_Msk | 
                                    UDP_IDR_EP5INT_Msk);

                /* Reset the endpoints to their default values */
                usbID->UDP_RST_EP = UDP_RST_EP_Msk;
                usbID->UDP_RST_EP = 0;

                /* Get the Endpoint Object for OUT Direction */
                endpointObj = hDriver->deviceEndpointObj[0];

                /* Update the endpoint database */
                endpointRead = (uint32_t)endpointObj->endpointState  & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED);
                endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;
                /* Get the Endpoint Object for IN Direction */
                endpointObj++;

                /* Update the endpoint database */
                endpointRead = (uint32_t)endpointObj->endpointState  & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED);
                endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;

                /* Get the Object for non Control endpoints */
                endpointObj = hDriver->deviceEndpointObj[1];

                for(index = 1; index < DRV_USBDP_ENDPOINTS_NUMBER; index ++)
                {
                    /* Update the endpoint database */
                    endpointRead = (uint32_t)endpointObj->endpointState  & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED);
                    endpointObj->endpointState  = (DRV_USBDP_ENDPOINT_STATE)endpointRead;

                    /* Get the next Endpoint Object */
                    endpointObj++;
                    
                    /* Clear the Control Status Register */
                    usbID->UDP_CSR[index] = 0;
                }

            }
            else
            {
                if(endpoint == 0U)
                {
                    /* Disable control endpoint and update the endpoint database. */
                    
                    /* Get the Endpoint Object for OUT Direction */
                    endpointObj = hDriver->deviceEndpointObj[0];

                    /* Update the endpoint database */
                    endpointRead = (uint32_t)endpointObj->endpointState  & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED);
                    endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;

                    /* Get the Endpoint Object for IN Direction */
                    endpointObj++;

                    /* Update the endpoint database */
                    endpointRead = (uint32_t)endpointObj->endpointState  & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED);
                    endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;

                    /* Disable the endpoint interrupts */
                    usbID->UDP_IDR = UDP_IDR_EP0INT_Msk;
                    
                    /* Reset the endpoint to its default values */
                    usbID->UDP_RST_EP = UDP_RST_EP_Msk;
                    usbID->UDP_RST_EP = 0;
                    
                    /* Clear the Control Status Register */
                    usbID->UDP_CSR[0] = 0;
                }
                else
                {
                    /* Disable specific non control endpoint */                    
                    endpointObj = hDriver->deviceEndpointObj[endpoint];

                    /* Update the endpoint database */
                    endpointRead = (uint32_t)endpointObj->endpointState  & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED);
                    endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;

                    /* Disable the endpoint interrupts */
                    usbID->UDP_IDR = UDP_IDR_EP0INT_Msk << endpoint;
                    
                    /* Reset the endpoint to its default values */
                    usbID->UDP_RST_EP = (UDP_RST_EP_EP0_Msk << endpoint);
                    usbID->UDP_RST_EP = 0;
                    
                    /* Clear the Control Status Register */
                    usbID->UDP_CSR[endpoint] = 0;
                }
            }
            if(hDriver->isInInterruptContext == false)
            {
                /* We were not in interrupt context. Restore the interrupt 
                 * status and unlock the Mutex */
                SYS_INT_SourceRestore(hDriver->interruptSource, interruptWasEnabled);
                (void) OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);
            }
        }
    }

    return(retVal);
}/* end of DRV_USBDP_EndpointDisable() */

// *****************************************************************************

/* Function:
    bool DRV_USBDP_EndpointIsEnabled
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    Dynamic implementation of DRV_USBDP_EndpointIsEnabled client
    interface function.

  Description:
    This is the dynamic implementation of
    DRV_USBDP_EndpointIsEnabled client interface function for
    USB device.
    Function returns the state of specified endpoint(true\false) signifying
    whether the endpoint is enabled or not.

  Remarks:
    See drv_usbdp.h for usage information.
 */

bool DRV_USBDP_EndpointIsEnabled
(
    DRV_HANDLE client,
    USB_ENDPOINT endpointAndDirection
)

{

    DRV_USBDP_OBJ * hDriver;                    /* USB driver object pointer */
    DRV_USBDP_ENDPOINT_OBJ * endpointObj;       /* Endpoint object pointer */
    uint8_t endpoint;                           /* Endpoint Number */
    bool retVal = false;                        /* Return value */


    /* Get the endpoint number */
    endpoint = endpointAndDirection & DRV_USBDP_ENDPOINT_NUMBER_MASK;

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(endpoint >= DRV_USBDP_ENDPOINTS_NUMBER)
    {
        /* Endpoint number out of range */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Endpoint number out of range in DRV_USBDP_EndpointIsEnabled().");
    }
    else if((DRV_HANDLE_INVALID == client) || ((DRV_HANDLE)NULL == client))
    {
        /* Invalid Driver client */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver client in DRV_USBDP_EndpointIsEnabled().");
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) client;
        
        /* Get the Endpoint Object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if(((uint32_t)endpointObj->endpointState & (uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED) == (uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED)
        {
            retVal = true;
        }
        else
        {
            /* return false */
        }
    }

    return (retVal);

}/* end of DRV_USBDP_EndpointIsEnabled() */


// *****************************************************************************

/* Function:
      USB_ERROR DRV_USBDP_EndpointStall
      (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
      )

  Summary:
    Dynamic implementation of DRV_USBDP_EndpointStall client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_EndpointStall
    client interface function for USB device.
    Function sets the STALL state of the specified endpoint.

  Remarks:
    See drv_usbdp.h for usage information.
 */

USB_ERROR DRV_USBDP_EndpointStall
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{

    DRV_USBDP_OBJ * hDriver;                    /* USB driver object pointer */
    udp_registers_t * usbID;                    /* USB instance pointer */
    DRV_USBDP_ENDPOINT_OBJ * endpointObj;       /* Endpoint object pointer */
    bool interruptWasEnabled = false;           /* USB interrupt status holder */
    uint8_t endpoint;                           /* Endpoint number */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */
    uint32_t endpointRead;


    /* Get the endpoint number */
    endpoint = endpointAndDirection & DRV_USBDP_ENDPOINT_NUMBER_MASK;

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(endpoint >= DRV_USBDP_ENDPOINTS_NUMBER)
    {
        /* Endpoint number out of range */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Endpoint number out of range in DRV_USBDP_EndpointStall().");
        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_EndpointStall().");
        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Get the endpoint object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        /* Check if we are in interrupt context. This is to ensure that this function
         * is interrupt and thread safe. If we are not in interrupt, we disable
         * the interrupt and grab the mutex. If we are already in interrupt
         * context, there is no need to disable interrupt or grab mutex */
        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Mutex lock failed in DRV_USBDP_EndpointStall()");
                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }
        if(retVal == USB_ERROR_NONE)
        {
            if(endpoint == 0U)
            {
                USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_FORCESTALL_Msk);

                /* Flush the IRPs of this endpoint from the queue */
                F_DRV_USBDP_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT);

                /* For control endpoint we stall both directions */
                endpointRead = (uint32_t)endpointObj->endpointState | (uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED;
                endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;

                endpointObj++;
                
                F_DRV_USBDP_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT);

                /* For control endpoint we stall both directions */
                endpointRead = (uint32_t)endpointObj->endpointState | (uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED;
                endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;
            }
            else
            {
                /* For non zero endpoints we stall the specified direction. */
                USBDP_CSR_SET_BITS(usbID, endpoint, UDP_CSR_FORCESTALL_Msk)

                F_DRV_USBDP_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT);

                endpointRead = (uint32_t)endpointObj->endpointState | (uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED;
                endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;
            }

            if(hDriver->isInInterruptContext == false)
            {
                /* We were not in interrupt context. Restore the interrupt 
                 * status and unlock the Mutex */
                SYS_INT_SourceRestore(hDriver->interruptSource, interruptWasEnabled);
                (void) OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);
            }
        }
    }
    return(retVal);

}/* end of DRV_USBDP_EndpointStall() */

// *****************************************************************************

/* Function:
      USB_ERROR DRV_USBDP_EndpointStallClear
      (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
      )

  Summary:
    Dynamic implementation of DRV_USBDP_EndpointStallClear client
    interface function.

  Description:
    This is the dynamic implementation of
    DRV_USBDP_EndpointStallClear client interface function for
    USB device.
    Function clears the STALL state of the specified endpoint and resets the
    data toggle value.

  Remarks:
    See drv_usbdp.h for usage information.
 */

USB_ERROR DRV_USBDP_EndpointStallClear
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{

    DRV_USBDP_OBJ * hDriver;                    /* USB driver object pointer */
    udp_registers_t * usbID;                    /* USB instance pointer */
    DRV_USBDP_ENDPOINT_OBJ * endpointObj;       /* Endpoint object pointer */
    bool interruptWasEnabled = false;           /* USB interrupt status holder */
    uint8_t endpoint;                           /* Endpoint number */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */
    uint32_t endpointRead ;


    /* Get the endpoint number */
    endpoint = endpointAndDirection & DRV_USBDP_ENDPOINT_NUMBER_MASK;

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(endpoint >= DRV_USBDP_ENDPOINTS_NUMBER)
    {
        /* Endpoint number out of range */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Endpoint number out of range in DRV_USBDP_EndpointStallClear().");
        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)NULL == handle))
    {
        /* Invalid Driver Handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_EndpointStallClear().");
        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Get the endpoint object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        /* Check if we are in interrupt context. This is to ensure that this function
         * is interrupt and thread safe. If we are not in interrupt, we disable
         * the interrupt and grab the mutex. If we are already in interrupt
         * context, there is no need to disable interrupt or grab mutex */
        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Mutex lock failed in DRV_USBDP_EndpointStallClear()");
                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }
        if(retVal == USB_ERROR_NONE)
        {
            if(endpoint == 0U)
            {
                /* For zero endpoint we stall both directions */
                USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_FORCESTALL_Msk)

                /* Update the endpoint object with stall Clear for endpoint 0 */
                endpointRead = (uint32_t)endpointObj->endpointState & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED);
                endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;

                endpointObj++;
                endpointRead = (uint32_t)endpointObj->endpointState & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED);
                endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;

            }
            else
            {
                /* For zero endpoint we stall both directions */
                USBDP_CSR_CLR_BITS(usbID, endpoint, UDP_CSR_FORCESTALL_Msk)

                /* Update the objects with stall Clear for non-zero endpoint */
                endpointRead = (uint32_t)endpointObj->endpointState & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED);
                endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endpointRead;

                F_DRV_USBDP_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_TERMINATED_BY_HOST);
            }

            if(hDriver->isInInterruptContext == false)
            {
                /* We were not in interrupt context. Restore the interrupt 
                 * status and unlock the Mutex */
                SYS_INT_SourceRestore(hDriver->interruptSource, interruptWasEnabled);
                (void) OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);
            }
        }
    }
    return(retVal);

}/* end of DRV_USBDP_EndpointStallClear() */

// *****************************************************************************

/* Function:
    bool DRV_USBDP_EndpointIsStalled(DRV_HANDLE client,
                                        USB_ENDPOINT endpoint)

  Summary:
    Dynamic implementation of DRV_USBDP_EndpointIsStalled client
    interface function.

  Description:
    This is the dynamic implementation of
    DRV_USBDP_EndpointIsStalled client interface function for
    USB device.
    Function returns the state of specified endpoint(true\false) signifying
    whether the endpoint is STALLed or not.

  Remarks:
    See drv_usbdp.h for usage information.
 */

bool DRV_USBDP_EndpointIsStalled
(
    DRV_HANDLE client,
    USB_ENDPOINT endpoint
)

{

    DRV_USBDP_OBJ * hDriver;                    /* USB driver object pointer */
    DRV_USBDP_ENDPOINT_OBJ * endpointObj;       /* Endpoint object pointer */
    uint8_t direction;                          /* Endpoint direction */
    uint8_t endpoint_t;                           /* Endpoint number */
    bool retVal = false;                        /* Return value */


    /* Get the endpoint number and its direction */
    endpoint_t = endpoint & DRV_USBDP_ENDPOINT_NUMBER_MASK;
    direction = (uint8_t)((endpoint & DRV_USBDP_ENDPOINT_DIRECTION_MASK) != 0);

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(endpoint_t >= DRV_USBDP_ENDPOINTS_NUMBER)
    {
        /* Endpoint number out of range */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Endpoint number out of range in DRV_USBDP_EndpointIsStalled().");
    }
    else if((DRV_HANDLE_INVALID == client) || ((DRV_HANDLE)NULL == client))
    {
        /* Invalid Driver client */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver client in DRV_USBDP_EndpointIsStalled().");
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = ((DRV_USBDP_OBJ *) client);
        endpointObj = hDriver->deviceEndpointObj[endpoint_t];

        if(endpoint_t == 0U)
        {
            /* For control endpoint direction specific endpoint object can be 
             * retrieved by using the direction. endpointObj[0] -> OUT Direction
             * endpointObj[1] -> IN Direction */
            endpointObj += direction;
        }

        if(((uint32_t)endpointObj->endpointState & (uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED) == (uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED)
        {
            /* Endpoint is stalled, return true */
            retVal = true;
        }
        else
        {
            /* Return false */
        }
    }

    return(retVal);

}/* end of DRV_USBDP_EndpointIsStalled() */

// *****************************************************************************

/* Function:
    USB_ERROR DRV_USBDP_IRPSubmit
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection,
        USB_DEVICE_IRP * irp
    )

  Summary:
    Dynamic implementation of DRV_USBDP_IRPSubmit client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USBDP_IRPSubmit
    client interface function for USB device.
    Function checks the validity of the input arguments and on success adds the
    IRP to endpoint object queue linked list.

  Remarks:
    See drv_usbdp.h for usage information.
 */
USB_ERROR DRV_USBDP_IRPSubmit
(
    DRV_HANDLE client,
    USB_ENDPOINT endpointAndDirection,
    USB_DEVICE_IRP * irp
)
{

    DRV_USBDP_OBJ * hDriver;                    /* USB driver object pointer */
    udp_registers_t * usbID;                    /* USB instance pointer */
    uint32_t remainder_t;                         /* Variable to hold size remainder */
    DRV_USBDP_ENDPOINT_OBJ * endpointObj;       /* Endpoint object pointer */
    bool interruptWasEnabled = false;           /* USB interrupt status holder */
    USB_DEVICE_IRP_LOCAL * irp_t;                 /* Local irp pointer */
    uint16_t index;                             /* Loop counter */
    uint16_t byteCount = 0;                     /* To hold received byte count */
    uint16_t endpoint0DataStageSize;            /* Size of Endpoint 0 data stage */
    uint8_t endpoint0DataStageDirection;        /* Direction of Endpoint 0 data stage */
    uint8_t direction;                          /* Endpoint direction */
    uint8_t endpoint;                           /* Endpoint number */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */
    USB_DEVICE_IRP_LOCAL * iterator;


    /* Get the endpoint number and its direction */
    endpoint = endpointAndDirection & DRV_USBDP_ENDPOINT_NUMBER_MASK;
    direction = (uint8_t)((endpointAndDirection & DRV_USBDP_ENDPOINT_DIRECTION_MASK) != 0U);

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(endpoint >= DRV_USBDP_ENDPOINTS_NUMBER)
    {
        /* Endpoint number out of range */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Endpoint number out of range in DRV_USBDP_IRPSubmit().");
        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if((DRV_HANDLE_INVALID == client) || ((DRV_HANDLE)(NULL) == client))
    {
        /* Invalid Driver client */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver client in DRV_USBDP_IRPSubmit().");
        retVal =  USB_ERROR_PARAMETER_INVALID;
    }
    else if(((USB_DEVICE_IRP_LOCAL *) irp)->status > USB_DEVICE_IRP_STATUS_SETUP)
    {
        /* Device IRP is already in use */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Device IRP is already in use in DRV_USBDP_IRPSubmit().");
        retVal = USB_ERROR_DEVICE_IRP_IN_USE;
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) client;
        usbID = hDriver->usbID;
                
        /* Assign the irp to be submitted to local pointer */
        irp_t = (USB_DEVICE_IRP_LOCAL *) irp;
        
        /* Fetch the respective endpoint object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];
        
        if(endpoint == 0U)
        {
            /* For control endpoint direction specific endpoint object can be 
             * retrieved by using the direction. endpointObj[0] -> OUT Direction
             * endpointObj[1] -> IN Direction */
            endpointObj += direction;
        }

        if(((uint32_t)endpointObj->endpointState & (uint32_t)DRV_USBDP_ENDPOINT_STATE_ENABLED) == 0U)
        {
            /* This means the endpoint is disabled */
            retVal = USB_ERROR_ENDPOINT_NOT_CONFIGURED;
        }
        else
        {
            /* Check the size of the IRP. If data direction is HOST_TO_DEVICE,
             * then IRP size must be multiple of maxPacketSize, i.e. multiple
             * of endpoint size. */
            remainder_t = irp_t->size % endpointObj->maxPacketSize;

            if((remainder_t != 0U) && ((uint8_t)USB_DATA_DIRECTION_HOST_TO_DEVICE == direction))
            {
                /* Direction is HOST_TO_DEVICE and it is not exact multiple of 
                 * maxPacketSize. Hence this is an error condition. */
                retVal = USB_ERROR_PARAMETER_INVALID;
            }
            else
            {
                if((remainder_t == 0U) && ((uint8_t)USB_DATA_DIRECTION_DEVICE_TO_HOST == direction))
                {
                    /* Direction is DEVICE_TO_HOST, IRP size is a multiple of
                     * endpoint size and not 0. If data complete flag is set,
                     * then we must send a ZLP */
                    if(((irp_t->flags & USB_DEVICE_IRP_FLAG_DATA_COMPLETE) == USB_DEVICE_IRP_FLAG_DATA_COMPLETE) && (irp_t->size != 0U))
                    {
                        /* This means a ZLP should be sent after the data is sent */
                        irp_t->flags |= USB_DEVICE_IRP_FLAG_SEND_ZLP;
                    }
                }

                /* Check if we are in interrupt context. This is to ensure that this function
                 * is interrupt and thread safe. If we are not in interrupt, we disable
                 * the interrupt and grab the mutex. If we are already in interrupt
                 * context, there is no need to disable interrupt or grab mutex */
                if(hDriver->isInInterruptContext == false)
                {
                    if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
                    {
                        interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
                    }
                    else
                    {
                        /* There was an error in getting the mutex */
                        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Mutex lock failed in DRV_USBDP_IRPSubmit()");
                        retVal = USB_ERROR_OSAL_FUNCTION;
                    }
                }

                if(retVal == USB_ERROR_NONE)
                {
                    irp_t->next = NULL;

                    /* Mark the IRP status as pending */
                    irp_t->status = USB_DEVICE_IRP_STATUS_PENDING;


                    if((uint8_t)USB_DATA_DIRECTION_DEVICE_TO_HOST == direction)
                    {
                        /* If the data is moving from device to host then 
                         * pending bytes is remaining data to be sent to the 
                         * host. */
                        irp_t->nPendingBytes = irp_t->size;
                    }
                    else
                    {
                        /* If the data is moving from host to device, 
                         * nPendingBytes tracks the amount of data received so 
                         * far */
                        irp_t->nPendingBytes = 0;
                    }
                    if(endpointObj->irpQueue == NULL)
                    {
                        /* Queue is empty. Update the irp status as in progress */
                        irp_t->status = USB_DEVICE_IRP_STATUS_IN_PROGRESS;
                        irp_t->previous = NULL;

                        /* Assign the irp to the queue */
                        endpointObj->irpQueue = irp_t;

                        if(endpoint == 0U)
                        {
                            if(direction == (uint8_t)USB_DATA_DIRECTION_HOST_TO_DEVICE)
                            {
                                switch(hDriver->endpoint0State)
                                {

                                    case DRV_USBDP_EP0_STATE_EXPECTING_SETUP_FROM_HOST:

                                        /* This is the default initialization value of Endpoint
                                         * 0.  In this state EPO is waiting for the setup packet
                                         * from the host. The IRP is already added to the queue.
                                         * When the host send the Setup packet, this IRP will be
                                         * processed in the interrupt. This means we don't have
                                         * to do anything in this state. */

                                        break;


                                    case DRV_USBDP_EP0_STATE_WAITING_FOR_SETUP_IRP_FROM_CLIENT:

                                        /* In this state, the driver has received the Setup
                                         * packet from the host, but was waiting for an IRP from
                                         * the client. The driver now has the IRP. We can unload
                                         * the setup packet into the IRP */

                                        /* Copy the data from FIFO to irp data buffer */
                                        for(index = 0; index < 8U; index++)
                                        {
                                            ((uint8_t *)irp->data)[index] = (uint8_t)usbID->UDP_FDR[0];
                                        }

                                        /* FIFO is free. Clear the Setup Interrupt flag */
                                        USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_RXSETUP_Msk)
                                        
                                        /* Re-enable the setup interrupt. */
                                        usbID->UDP_IER = UDP_IER_EP0INT_Msk;

                                        /* Analyze the setup packet. We need to check if the
                                         * control transfer contains a data stage and if so,
                                         * what is its direction. */
                                        endpoint0DataStageSize = *((uint8_t *)irp->data + 6);
                                        endpoint0DataStageDirection = (uint8_t)((*((uint8_t *)irp->data) & DRV_USBDP_ENDPOINT_DIRECTION_MASK) != 0U);

                                        if(endpoint0DataStageSize == 0U)
                                        {
                                            /* This means there is no data stage. We wait for
                                             * the client to submit the status IRP. */
                                            hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT;
                                        }
                                        else
                                        {
                                            /* This means there is a data stage. Analyze the direction. */
                                            if(endpoint0DataStageDirection == (uint8_t)USB_DATA_DIRECTION_DEVICE_TO_HOST)
                                            {
                                                /* Device to Host - we wait for the client to submit an transmit IRP */
                                                hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT;
                                            }
                                            else
                                            {
                                                /* Host to Device - We wait for the host to send the data. */
                                                hDriver->endpoint0State = DRV_USBDP_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST;
                                            }
                                        }

                                        /* Indicate that this is a setup IRP */
                                        irp_t->status = USB_DEVICE_IRP_STATUS_SETUP;

                                        /* Mention size of irp. This is needed for client */
                                        irp_t->size = 8;

                                        /* Update the IRP queue so that the client can submit an
                                         * IRP in the IRP callback. */
                                        endpointObj->irpQueue = irp_t->next;

                                        /* IRP callback if it is not NULL */
                                        if(irp_t->callback != NULL)
                                        {
                                            irp_t->callback((USB_DEVICE_IRP *)irp_t);
                                        }

                                        break;


                                    case DRV_USBDP_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST:
                                    case DRV_USBDP_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE:


                                        break;

                                    case DRV_USBDP_EP0_STATE_WAITING_FOR_RX_DATA_IRP_FROM_CLIENT:

                                        /* In this state, the host sent a data stage packet, an
                                        * interrupt occurred but there was no RX data stage
                                        * IRP. The RX IRP is now being submitted. We should
                                        * unload the fifo. */

                                        byteCount = (uint16_t)((usbID->UDP_CSR[0] & UDP_CSR_RXBYTECNT_Msk) >> UDP_CSR_RXBYTECNT_Pos);

                                        if((irp_t->nPendingBytes + byteCount) > irp_t->size)
                                        {
                                            /* This is not acceptable as it may corrupt the ram location */
                                            byteCount = (uint16_t)(irp_t->size - irp_t->nPendingBytes);
                                        }

                                        /* Copy the data from FIFO to irp data buffer */
                                        for(index = 0; index < byteCount; index++)
                                        {
                                            ((uint8_t *)irp_t->data)[index] = (uint8_t)usbID->UDP_FDR[0];
                                        }

                                        /* Update the pending byte count */
                                        irp_t->nPendingBytes += byteCount;

                                        if(irp_t->nPendingBytes >= irp_t->size)
                                        {
                                            /* This means we have received all the data that
                                             * we were supposed to receive */
                                            irp_t->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                                            /* Change endpoint state to waiting to the
                                             * status stage */
                                            hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE;

                                            /* Clear and re-enable the interrupt */
                                            USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_RX_DATA_BK0_Msk)
                                            usbID->UDP_IER = UDP_IER_EP0INT_Msk;

                                            /* Update the queue, update irp-size to indicate
                                             * how much data was received from the host. */
                                            irp_t->size = irp_t->nPendingBytes;

                                            endpointObj->irpQueue = irp_t->next;

                                            if(irp_t->callback != NULL)
                                            {
                                                irp_t->callback((USB_DEVICE_IRP *)irp_t);
                                            }
                                        }
                                        else if(byteCount < endpointObj->maxPacketSize)
                                        {
                                            /* This means we received a short packet. We
                                             * should end the transfer. */
                                            irp_t->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;

                                            /* The data stage is complete. We now wait
                                             * for the status stage. */
                                            hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE;

                                            /* Clear and enable the interrupt. */
                                            USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_RX_DATA_BK0_Msk)
                                            usbID->UDP_IER = UDP_IER_EP0INT_Msk;

                                            /* Update the queue, update irp-size to indicate
                                             * how much data was received from the host. */
                                            irp_t->size = irp_t->nPendingBytes;
                                            endpointObj->irpQueue = irp_t->next;

                                            if(irp_t->callback != NULL)
                                            {
                                                irp_t->callback((USB_DEVICE_IRP *)irp_t);
                                            }
                                        }
                                        else
                                        {
                                            /* We are expecting more packets. Clear the flag 
                                             * to indicate FIFO is ready to receive more data 
                                             * and enable the interrupt */
                                            USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_RX_DATA_BK0_Msk)
                                            usbID->UDP_IER = UDP_IER_EP0INT_Msk;
                                        }

                                        break;

                                    case DRV_USBDP_EP0_STATE_WAITING_FOR_RX_STATUS_IRP_FROM_CLIENT:

                                        /* This means the host has already sent an RX status
                                         * stage but there was not IRP to receive this. We have
                                         * the IRP now. We change the EP0 state to waiting for
                                         * the next setup from the host. */

                                        hDriver->endpoint0State = DRV_USBDP_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

                                        /* Update the IRP status */
                                        irp_t->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                                        /* Clear the flag to indicate FIFO is ready to receive more data */
                                        USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_RX_DATA_BK0_Msk)

                                        /* Update the queue */
                                        endpointObj->irpQueue = irp_t->next;

                                        if(irp_t->callback != NULL)
                                        {
                                            irp_t->callback((USB_DEVICE_IRP *)irp_t);
                                        }


                                        break;

                                    case DRV_USBDP_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT:


                                        break;

                                    default:
                                        /* Do Nothing */
                                        break;
                                }

                            }
                            else
                            {       // Device to Host

                                switch(hDriver->endpoint0State)
                                {

                                    case DRV_USBDP_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT:

                                        /* Driver is waiting for an IRP from the client and has
                                         * received it. Determine the transaction size. */

                                        if(irp_t->nPendingBytes < endpointObj->maxPacketSize)
                                        {
                                            /* Data to be sent is less than endpoint size. It means, 
                                             * this is the last transaction in the transfer or total
                                             * size itself is less than endpoint size */
                                            byteCount = (uint16_t)irp_t->nPendingBytes;
                                        }
                                        else
                                        {
                                            /* Data to be sent is more than the endpoint size.
                                             * Send only the size of endpoint in this iteration. 
                                             * This could be first or a continuing transaction in the
                                             * transfer */
                                            byteCount = endpointObj->maxPacketSize;
                                        }

                                        /* Change the direction as this is control endpoint */
                                        USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_DIR_Msk)
                                                
                                        /* Copy the data from irp data buffer to FIFO */
                                        for(index = 0; index < byteCount; index++)
                                        {
                                            usbID->UDP_FDR[0] = ((uint8_t *)irp_t->data)[index];
                                        }

                                        /* Update the nPendingBytes, reduce by byteCount */
                                        irp_t->nPendingBytes -= byteCount;

                                        /* Update the EP0 state - Move to TX_DATA_STAGE state */
                                        hDriver->endpoint0State = DRV_USBDP_EP0_STATE_TX_DATA_STAGE_IN_PROGRESS;

                                        /* Clear the TXPKTRDY flag. Now controller can send the data 
                                         * when host request for it */
                                        USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_TXPKTRDY_Msk)

                                        break;

                                    case DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT:

                                        /* This means the driver is expecting the client to
                                         * submit a TX status stage IRP. */
                                        hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE;

                                        USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_TXPKTRDY_Msk)

                                        break;


                                    default:
                                        /* Do Nothing */
                                        break;

                                }
                            }
                        }
                        else
                        {   // Non Control Endpoint

                            if(direction == (uint8_t)USB_DATA_DIRECTION_DEVICE_TO_HOST)
                            {
                                /* Sending from Device to Host */
                                if(irp_t->nPendingBytes <= endpointObj->maxPacketSize)
                                {
                                    /* Data to be sent is less than endpoint size. It means, 
                                     * this is the last transaction in the transfer or total
                                     * size itself is less than endpoint size */
                                    byteCount = (uint16_t)irp_t->nPendingBytes;
                                }
                                else
                                {
                                    /* Data to be sent is more than the endpoint size.
                                     * Send only the size of endpoint in this iteration. 
                                     * This could be first or a continuing transaction in the
                                     * transfer */
                                    byteCount = endpointObj->maxPacketSize;
                                }

                                /* Copy the data from irp data buffer to FIFO */
                                for(index = 0; index < byteCount; index++)
                                {
                                    usbID->UDP_FDR[endpoint] = ((uint8_t *)irp_t->data)[index];
                                }

                                /* Update the nPendingBytes, reduce by byteCount */
                                irp_t->nPendingBytes -= byteCount;

                                /* Clear the TXPKTRDY flag. Now controller can send the data 
                                 * when host request for it */
                                USBDP_CSR_SET_BITS(usbID, endpoint, UDP_CSR_TXPKTRDY_Msk)
                                USBDP_CSR_CLR_BITS(usbID, endpoint, UDP_CSR_TXCOMP_Msk)

                                /* The rest of the IRP processing takes place in ISR */
                            }
                            else
                            {
                                /* direction is Host to Device */
                                if(((usbID->UDP_CSR[endpoint] & UDP_CSR_RX_DATA_BK0_Msk) == UDP_CSR_RX_DATA_BK0_Msk) ||
                                   ((usbID->UDP_CSR[endpoint] & UDP_CSR_RX_DATA_BK1_Msk) == UDP_CSR_RX_DATA_BK1_Msk))
                                {
                                    /* Data is already available in the FIFO */
                                    byteCount = (uint16_t)((usbID->UDP_CSR[endpoint] & UDP_CSR_RXBYTECNT_Msk) >> UDP_CSR_RXBYTECNT_Pos);

                                    /* This is not acceptable as it may corrupt the ram location */
                                    if((irp_t->nPendingBytes + byteCount) > irp_t->size)
                                    {
                                        byteCount = (uint16_t)(irp_t->size - irp_t->nPendingBytes);
                                    }

                                    /* Need to copy data from FIFO to irp data buffer. Since we may 
                                     * get multiple packets, we use nPendingBytes as index as it holds
                                     * count of data received from host */
                                    for(index = (uint16_t)irp_t->nPendingBytes; index < ((uint16_t)irp_t->nPendingBytes + byteCount); index++)
                                    {
                                        ((uint8_t *)irp_t->data)[index] = (uint8_t)usbID->UDP_FDR[endpoint];
                                    }

                                    /* Update the pending byte count with total number of bytes received */
                                    irp_t->nPendingBytes += byteCount;

                                    if((irp_t->nPendingBytes < irp_t->size) && (byteCount >= endpointObj->maxPacketSize))
                                    {
                                        /* Total bytes received is less than irp size and current 
                                         * data packet is equal to / greater than endpoint size.
                                         * This means we are expecting more data from host. Just clear
                                         * the banks and re-enable the interrupts */
                                        USBDP_CSR_CLR_BITS(usbID, endpoint, UDP_CSR_RX_DATA_BK0_Msk | UDP_CSR_RX_DATA_BK1_Msk)
                                        usbID->UDP_IER = (UDP_IER_EP0INT_Msk << endpoint);
                                    }
                                    else
                                    {
                                        /* We have received all the data from the host. Based on nPendingBytes
                                         * decide if it is short packet or complete packet */
                                        if(irp_t->nPendingBytes >= irp_t->size)
                                        {
                                            /* All data received. Update irp status accordingly */
                                            irp_t->status = USB_DEVICE_IRP_STATUS_COMPLETED;
                                        }
                                        else
                                        {
                                            /* Short Packet received. Update irp status accordingly */
                                            irp_t->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;
                                        }

                                        /* No more data expected. Clear and free up the Bank for transaction */
                                        USBDP_CSR_CLR_BITS(usbID, endpoint, UDP_CSR_RX_DATA_BK0_Msk | UDP_CSR_RX_DATA_BK1_Msk)

                                        /* Re-enable the interrupt */
                                        usbID->UDP_IER = (UDP_IER_EP0INT_Msk << endpoint);

                                        /* Update the irpQueue and irp size. */
                                        endpointObj->irpQueue = irp_t->next;
                                        irp_t->size = irp_t->nPendingBytes;

                                        if(irp_t->callback != NULL)
                                        {
                                            irp_t->callback((USB_DEVICE_IRP *)irp_t);
                                        }
                                    }
                                }
                                else
                                {
                                    /* Host has not sent any data and IRP is already added
                                     * to the queue. IRP will be processed in the ISR */
                                }
                            }/* End of non zero RX IRP submit */
                        }/* End of non zero IRP submit */
                    }
                    else
                    {
                        /* This means we should surf the linked list to get to the last entry . */
                        iterator = endpointObj->irpQueue;
                        while (iterator->next != NULL)
                        {
                            iterator = iterator->next;
                        }
                        iterator->next = irp_t;
                        irp_t->previous = iterator;
                    }

                    if(hDriver->isInInterruptContext == false)
                    {
                        /* We were not in interrupt context. Restore the interrupt 
                         * status and unlock the Mutex */
                        SYS_INT_SourceRestore(hDriver->interruptSource, interruptWasEnabled);
                        (void) OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);
                    }
                }
            }
        }
    }

    return (retVal);

}/* end of DRV_USBDP_IRPSubmit() */

// *****************************************************************************

/* Function:
    USB_ERROR DRV_USBDP_IRPCancelAll
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    Dynamic implementation of DRV_USBDP_IRPCancelAll client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USBDP_IRPCancelAll
    client interface function for USB device.
    Function checks the validity of the input arguments and on success cancels
    all the IRPs on the specific endpoint object queue.

  Remarks:
    See drv_usbdp.h for usage information.
 */

USB_ERROR DRV_USBDP_IRPCancelAll
(
    DRV_HANDLE client,
    USB_ENDPOINT endpointAndDirection
)

{

    DRV_USBDP_OBJ * hDriver;                    /* USB driver object pointer */
    DRV_USBDP_ENDPOINT_OBJ * endpointObj;       /* Endpoint object pointer */
    bool interruptWasEnabled = false;           /* USB interrupt status holder */
    uint8_t direction;                          /* Endpoint direction */
    uint8_t endpoint;                           /* Endpoint number */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */


    /* Get the endpoint number and its direction */
    endpoint = endpointAndDirection & DRV_USBDP_ENDPOINT_NUMBER_MASK;
    direction = (uint8_t)((endpointAndDirection & DRV_USBDP_ENDPOINT_DIRECTION_MASK) != 0U);

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(endpoint >= DRV_USBDP_ENDPOINTS_NUMBER)
    {
        /* Endpoint number out of range */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Endpoint number out of range in DRV_USBDP_IRPCancelAll().");
        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if((DRV_HANDLE_INVALID == client) || ((DRV_HANDLE)NULL == client))
    {
        /* Invalid Driver client */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_IRPCancelAll().");
        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) client;

        /* Get the endpoint object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];
        
        if(endpoint == 0U)
        {
            /* For control endpoint direction specific endpoint object can be 
             * retrieved by using the direction. endpointObj[0] -> OUT Direction
             * endpointObj[1] -> IN Direction */
            endpointObj += direction;
        }

        /* Check if we are in interrupt context. This is to ensure that this function
         * is interrupt and thread safe. If we are not in interrupt, we disable
         * the interrupt and grab the mutex. If we are already in interrupt
         * context, there is no need to disable interrupt or grab mutex */
        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Mutex lock failed in DRV_USBDP_IRPCancelAll()");
                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }

        if(retVal == USB_ERROR_NONE)
        {
            /* Flush the endpoint */
            F_DRV_USBDP_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED);

            if(hDriver->isInInterruptContext == false)
            {
                /* We were not in interrupt context. Restore the interrupt 
                 * status and unlock the Mutex */
                SYS_INT_SourceRestore(hDriver->interruptSource, interruptWasEnabled);
                (void) OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);
            }
        }
    }

    return(retVal);

}/* end of DRV_USBDP_IRPCancelAll() */

// *****************************************************************************

/* Function:
    USB_ERROR DRV_USBDP_IRPCancel
    (
        DRV_HANDLE client,
        USB_DEVICE_IRP * irp
    )

  Summary:
    Dynamic implementation of DRV_USBDP_IRPCancel client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USBDP_IRPCancel
    client interface function for USB device.  Function checks the validity of
    the input arguments and on success cancels  the specific IRP.
    An IRP that was in the queue but that has been processed yet will be
    canceled successfully and the IRP callback function will be called from
    this function with USB_DEVICE_IRP_STATUS_ABORTED status. The application
    can release the data buffer memory used by the IRP when this callback
    occurs. If the IRP was in progress (a transaction in on the bus) when the
    cancel function was called, the IRP will be canceled only when an ongoing
    or the next transaction has completed. The IRP callback function will then
    be called in an interrupt context. The application should not release the
    related data buffer unless the IRP callback has occurred.

  Remarks:
    See drv_usbdp.h for usage information.
 */

USB_ERROR DRV_USBDP_IRPCancel
(
    DRV_HANDLE client,
    USB_DEVICE_IRP * irp
)

{

    DRV_USBDP_OBJ * hDriver;                    /* USB driver object pointer */
    USB_DEVICE_IRP_LOCAL * irpToCancel;         /* Local irp pointer */
    bool interruptWasEnabled = false;           /* USB interrupt status holder */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_HANDLE_INVALID == client) || ((DRV_HANDLE)NULL == client))
    {
        /* Invalid Driver client */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Handle in DRV_USBDP_IRPCancel().");
        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else if(irp == NULL)
    {
        /* Invalid IRP Pointer */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid IRP Pointer in DRV_USBDP_IRPCancel().");
        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Assign the driver handler to the local pointer */
        hDriver = ((DRV_USBDP_OBJ *)client);

        /* Assign the irp handle to the local pointer */
        irpToCancel = (USB_DEVICE_IRP_LOCAL *) irp;

        if(irpToCancel->status <= USB_DEVICE_IRP_STATUS_COMPLETED_SHORT)
        {
            /* This IRP has either completed or has been aborted.*/
            retVal = USB_ERROR_PARAMETER_INVALID;
        }
        else
        {
            /* Check if we are in interrupt context. This is to ensure that this function
             * is interrupt and thread safe. If we are not in interrupt, we disable
             * the interrupt and grab the mutex. If we are already in interrupt
             * context, there is no need to disable interrupt or grab mutex */
            if(hDriver->isInInterruptContext == false)
            {
                if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
                {
                    interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
                }
                else
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Mutex lock failed in DRV_USBDP_IRPCancel().");
                    retVal = USB_ERROR_OSAL_FUNCTION;
                }
            }

            if(retVal == USB_ERROR_NONE)
            {
                /* The code will come here both when the IRP is NOT the 1st
                 * in queue as well as when it is at the HEAD. We will change
                 * the IRP status for either scenario but will give the callback
                 * only if it is NOT at the HEAD of the queue.
                 *
                 * What it means for HEAD IRP case is it will be caught in USB
                 * ISR and will be further processed in ISR. This is done to
                 * make sure that the user cannot release the IRP buffer before
                 * ABORT callback*/

                /* Mark the IRP status as aborted */
                irpToCancel->status = USB_DEVICE_IRP_STATUS_ABORTED;

                /* No data for this IRP was sent or received */
                irpToCancel->size = 0;

                if(irpToCancel->previous != NULL)
                {
                    /* This means this is not the HEAD IRP in the IRP queue.
                     * Can be removed from the endpoint object queue safely.*/
                    irpToCancel->previous->next = irpToCancel->next;

                    if(irpToCancel->next != NULL)
                    {
                        /* If this is not the last IRP in the queue then update
                         * the previous link connection for the next IRP */
                        irpToCancel->next->previous = irpToCancel->previous;
                    }

                    irpToCancel->previous = NULL;
                    irpToCancel->next = NULL;

                    if(irpToCancel->callback != NULL)
                    {
                        irpToCancel->callback((USB_DEVICE_IRP *) irpToCancel);
                    }
                }

                if(hDriver->isInInterruptContext == false)
                {
                    /* We were not in interrupt context. Restore the interrupt 
                     * status and unlock the Mutex */
                    SYS_INT_SourceRestore(hDriver->interruptSource, interruptWasEnabled);
                    (void) OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);
                }
            }
        }
    }

    return retVal;

}/* End of DRV_USBDP_IRPCancel() */

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.4"
</#if>
/* MISRAC 2012 deviation block end */
// *****************************************************************************
/* Function:
      void F_DRV_USBDP_Tasks_ISR(DRV_USBDP_OBJ * hDriver)

  Summary:
    Dynamic implementation of F_DRV_USBDP_Tasks_ISR ISR handler
    function.

  Description:
    This is the dynamic implementation of F_DRV_USBDP_Tasks_ISR ISR
    handler function for USB device.
    Function will get called automatically due to USB interrupts in interrupt
    mode.
    This function performs necessary action based on the interrupt and clears
    the interrupt after that. The USB device layer callback is called with the
    interrupt event details, if callback function is registered.

  Remarks:
    This is a local function and should not be called directly by the
    application.
 */


void F_DRV_USBDP_Tasks_ISR
(
    DRV_USBDP_OBJ * hDriver
)

{

    udp_registers_t * usbID;                    /* USB instance pointer */
    DRV_USBDP_ENDPOINT_OBJ * endpointObj;       /* Endpoint object pointer */
    USB_DEVICE_IRP_LOCAL * irp;                 /* Local irp pointer */
    USB_SETUP_PACKET * setupPkt;
    uint16_t offset;                            /* Endpoint data offset holder */
    uint16_t index;                             /* Loop counter */
    uint16_t epIndex;                           /* Loop counter */
    uint16_t byteCount = 0;                     /* To hold received byte count */
    uint16_t endpoint0DataStageSize;            /* Size of Endpoint 0 data stage */
    uint8_t endpoint0DataStageDirection;        /* Direction of Endpoint 0 data stage */
    uint32_t endPointRead;


    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if((DRV_USBDP_OBJ *)NULL == hDriver)
    {
        /* Invalid Driver Object */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid Driver Object in F_DRV_USBDP_Tasks_ISR().");
    }
    if(false == hDriver->isOpened)
    {
        /* Driver object is not open. No valid client available */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Driver does not have a client in F_DRV_USBDP_Tasks_ISR().");
    }
    else if((DRV_USB_EVENT_CALLBACK)NULL == hDriver->pEventCallBack)
    {
        /* We need a valid event handler */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBDP Driver: Driver needs a event handler in F_DRV_USBDP_Tasks_ISR().");
    }
    else
    {
        /* Assign the USB hardware id to local pointer */
        usbID = hDriver->usbID;

        if(((usbID->UDP_IMR & UDP_IMR_SOFINT_Msk) == UDP_IMR_SOFINT_Msk) && ((usbID->UDP_ISR & UDP_ISR_SOFINT_Msk) == UDP_ISR_SOFINT_Msk))
        {
            /* We have received a SOF interrupt. Clear the flag and send the event to client */
            usbID->UDP_ICR = UDP_ICR_SOFINT_Msk;
            hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBDP_EVENT_SOF_DETECT, NULL);
        }

        if((((usbID->UDP_IMR & UDP_IMR_WAKEUP_Msk) == UDP_IMR_WAKEUP_Msk) && ((usbID->UDP_ISR & UDP_ISR_WAKEUP_Msk) == UDP_ISR_WAKEUP_Msk)) ||
          (((usbID->UDP_IMR & UDP_IMR_RXRSM_Msk) == UDP_IMR_RXRSM_Msk) && ((usbID->UDP_ISR & UDP_ISR_RXRSM_Msk) == UDP_ISR_RXRSM_Msk)))
        {
            /* We have received a WAKEUP/RXRSM interrupt - clear the flag and
             * send the event to client */

            /* If the application goes to Sleep mode when USB receives a
             * Suspend interrupt, then when the device wakes up, it has to
             * ensure that the clocks are ready for functioning. This is
             * currently not handled by Controller driver. It has to be handled
             * by the application. */

            usbID->UDP_ICR = (UDP_ICR_WAKEUP_Msk | UDP_ICR_RXRSM_Msk);
            usbID->UDP_IDR = (UDP_IDR_WAKEUP_Msk | UDP_IDR_RXRSM_Msk);
            usbID->UDP_IER = UDP_IER_RXSUSP_Msk;

            hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBDP_EVENT_RESUME_DETECT, NULL);
        }

        if(((usbID->UDP_IMR & UDP_IMR_RXSUSP_Msk) == UDP_IMR_RXSUSP_Msk) && ((usbID->UDP_ISR & UDP_ISR_RXSUSP_Msk) == UDP_ISR_RXSUSP_Msk))
        {
            /* We have received a SUSPEND interrupt - clear the flag and send
             * the event to client */

            /* If the application goes to Sleep mode when USB receives a
             * Suspend interrupt, then when the device wakes up, it has to
             * ensure that the clocks are ready for functioning. This is
             * currently not handled by Controller driver. It has to be handled
             * by the application. */

            usbID->UDP_ICR = UDP_ICR_RXSUSP_Msk;
            usbID->UDP_IDR = UDP_IDR_RXSUSP_Msk;
            usbID->UDP_IER = (UDP_IER_WAKEUP_Msk | UDP_IER_RXRSM_Msk);

            hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBDP_EVENT_IDLE_DETECT, NULL);
        }

        if((usbID->UDP_ISR & UDP_ISR_ENDBUSRES_Msk) == UDP_ISR_ENDBUSRES_Msk)
        {
            /* Clear the End of Bus Reset Interrupt */
            usbID->UDP_ICR = UDP_ICR_ENDBUSRES_Msk;

            /* USB Controller is now ready to receive SETUP packet from host */
            hDriver->endpoint0State = DRV_USBDP_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

            /* Update the device connection speed in the USB device object. */
            hDriver->deviceSpeed = USB_SPEED_FULL;
            hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBDP_EVENT_RESET_DETECT, NULL);

            /* Enable the function endpoint to start receiving data packets from
             * host. */
            usbID->UDP_GLB_STAT = 0;
            usbID->UDP_FADDR = 0;
            usbID->UDP_FADDR |= UDP_FADDR_FEN_Msk;

            /* Enable the Suspend and SOF interrupts. */
            usbID->UDP_IER = (UDP_IER_RXSUSP_Msk | UDP_IER_SOFINT_Msk);
        }

        if(((usbID->UDP_IMR & UDP_IMR_EP0INT_Msk) == UDP_IMR_EP0INT_Msk) &&
           ((usbID->UDP_ISR & UDP_ISR_EP0INT_Msk) == UDP_ISR_EP0INT_Msk))
        {

            /* Get the control endpoint object pointer for OUT direction */
            endpointObj = hDriver->deviceEndpointObj[0];

            /* This means we have received packet on a control endpoint. Let's
             * clear the stall condition on both the directions of control
             * endpoint. */
            endPointRead = (uint32_t)endpointObj->endpointState & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED);
            endpointObj->endpointState = (DRV_USBDP_ENDPOINT_STATE)endPointRead;
            endPointRead = (uint32_t)(endpointObj + 1)->endpointState & (~(uint32_t)DRV_USBDP_ENDPOINT_STATE_STALLED);
            (endpointObj + 1)->endpointState = (DRV_USBDP_ENDPOINT_STATE)endPointRead;

            if((usbID->UDP_CSR[0] & UDP_CSR_RXSETUP_Msk) == UDP_CSR_RXSETUP_Msk)
            {
                /* We have received a setup packet. Check if we have a valid 
                 * IRP and then proceed */
                if(endpointObj->irpQueue != NULL)
                {
                    /* Get the current irp from the Queue */
                    irp = endpointObj->irpQueue;

                    /* Copy the data from FIFO to irp data buffer */
                    for(index = 0; index < 8U; index++)
                    {
                        ((uint8_t *)irp->data)[index] = (uint8_t)usbID->UDP_FDR[0];
                    }

                    /* FIFO is free. Clear the Setup Interrupt flag. */
                    USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_RXSETUP_Msk)

                    /* Analyze the setup packet. We need to check if the control
                     * transfer contains a data stage and if so, get the direction. */
                
                    setupPkt = (USB_SETUP_PACKET *)irp->data;
                                            
                    endpoint0DataStageSize = setupPkt->W_Length.Val;

                    endpoint0DataStageDirection = (uint8_t)((setupPkt->bmRequestType & DRV_USBDP_ENDPOINT_DIRECTION_MASK) != 0U);

                    if(endpoint0DataStageSize == 0U)
                    {
                        /* This means there is no data stage. We wait for
                         * the client to submit the status IRP. */
                        hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT;
                    }
                    else
                    {
                        /* This means there is a data stage. Analyze the direction. */
                        if(endpoint0DataStageDirection == (uint8_t)USB_DATA_DIRECTION_DEVICE_TO_HOST)
                        {
                            /* Device to Host - we wait for the client to submit an transmit IRP */
                            hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT;
                        }
                        else
                        {
                            /* Host to Device - We wait for the host to send the data. */
                            hDriver->endpoint0State = DRV_USBDP_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST;
                        }
                    }

                    /* Indicate that this is a setup IRP */
                    irp->status = USB_DEVICE_IRP_STATUS_SETUP;

                    /* Mention size of irp. This is needed for client */
                    irp->size = 8;

                    /* Update the IRP queue so that the client can submit an
                     * IRP in the IRP callback. */
                    endpointObj->irpQueue = irp->next;

                    /* IRP callback if it is not NULL */
                    if(irp->callback != NULL)
                    {
                        irp->callback((USB_DEVICE_IRP *)irp);
                    }
                }
                else
                {
                    /* We do not have an IRP to process setup packet. Disable
                     * the Control Endpoint until an IRP is submitted. This is
                     * to ensure we do not get interrupted infinitely. */
                    usbID->UDP_IDR = UDP_IDR_EP0INT_Msk;
                }

            }
            if((usbID->UDP_CSR[0] & UDP_CSR_TXCOMP_Msk) == UDP_CSR_TXCOMP_Msk)
            {
                /* Get the IN endpoint object pointer of control endpoint */
                endpointObj = endpointObj + 1;

                /* This happens when a transfer is complete - specifically on two conditions:
                 * 1. Transmission of Data stage (Device has successfully sent data to host in the data stage)
                 * 2. Transmission of Status Stage (Device has successfully sent status to host in the status stage)  */

                /* Check if we have a valid IRP in the queue. There should be one, as a Tx transaction
                 * cannot happen without an IRP */
                if(endpointObj->irpQueue != NULL)
                {
                    /* Copy the IRP from queue to local variable */
                    irp = endpointObj->irpQueue;

                    /* Check if we are in Transmission of Status Stage */
                    if(hDriver->endpoint0State == DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE)
                    {
                        /* Transaction Complete. Expecting next Setup Packet from Host */
                        hDriver->endpoint0State = DRV_USBDP_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

                        /* Update status of current IRP */
                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                        /* Copy the next IRP to the queue. */
                        endpointObj->irpQueue = irp->next;

                        /* Reset the IRP size. */
                        irp->size = 0;

                        if(irp->callback != NULL)
                        {
                            irp->callback((USB_DEVICE_IRP *)irp);
                        }

                        /* Clear the interrupt. */
                        USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_TXCOMP_Msk)
                    }
                    else if(irp->nPendingBytes == 0U)
                    {
                        /* This means we are in Transmission of Data stage.
                         * nPendingBytes is 0, it means all TX data has been
                         * sent. Check if ZLP is to be sent, else mark the IRP
                         * as completed. */
                        if((usbID->UDP_CSR[0] & UDP_CSR_TXPKTRDY_Msk) != 0U)
                        {
                            /* This cannot happen. Do nothing, just return */
                        }
                        else if((irp->flags & USB_DEVICE_IRP_FLAG_SEND_ZLP) == USB_DEVICE_IRP_FLAG_SEND_ZLP)
                        {
                            /* Need to send ZLP. Clear the size of buffer and
                             * ready the buffer to send data */
                            irp->flags &= ~USB_DEVICE_IRP_FLAG_SEND_ZLP;

                            /* Change the direction, Set TXPKTRDY to send packet and clear interrupt */
                            USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_DIR_Msk)
                            USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_TXPKTRDY_Msk)
                            USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_TXCOMP_Msk)
                        }
                        else
                        {
                            /* No need to send ZLP, update the endpoint0State */
                            hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE;

                            /* Mark the IRP status as Completed and do irp callback. */
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                            /* Update the IRP queue so that the client can submit an
                             * IRP in the IRP callback. */
                            endpointObj->irpQueue = irp->next;

                            if(irp->callback != NULL)
                            {
                                irp->callback((USB_DEVICE_IRP *)irp);
                            }

                            /* Change the direction */
                            USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_DIR_Msk)

                            /* Set the BK1RDY bit to TX the ZLP to host */
                            USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_TXCOMP_Msk)

                        }
                    }
                    else
                    {
                        /* This means we are in Transmission of Data stage and 
                         * nPendingBytes is not 0 */
                        if(irp->nPendingBytes <= endpointObj->maxPacketSize)
                        {
                            /* Last packet with data less than endpoint size */
                            byteCount = (uint16_t)irp->nPendingBytes;
                        }
                        else
                        {
                            /* We have data more than endpoint size. Send only upto endpoint size */
                            byteCount = endpointObj->maxPacketSize;
                        }

                        /* Change direction of control endpoint */
                        USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_DIR_Msk)

                        /* nPendingBytes holds the number of bytes to be sent. Use 
                         * it to know the starting of next packet of data to be 
                         * sent to host */
                        offset = (uint16_t)(irp->size - irp->nPendingBytes);

                        /* copy byteCount size of data from irp data buffer starting 
                         * at offset location to the FIFO */
                        for(index = offset; index < (offset + byteCount); index++)
                        {
                            usbID->UDP_FDR[0] = ((uint8_t *)irp->data)[index];
                        }

                        /* Updated the nPendingBytes */
                        irp->nPendingBytes -= byteCount;

                        /* Set TXPKTRDY to send data and clear the interrupt */
                        USBDP_CSR_SET_BITS(usbID, 0, UDP_CSR_TXPKTRDY_Msk)
                        USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_TXCOMP_Msk)
                    }
                }
            }
            if((usbID->UDP_CSR[0] & UDP_CSR_RX_DATA_BK0_Msk) == UDP_CSR_RX_DATA_BK0_Msk)
            {
                /* This means we have received data from the host. It can be either 
                 * RX data stage or RX status stage of the control transfer */
                if(hDriver->endpoint0State == DRV_USBDP_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE)
                {
                    /* Check if we have a valid IRP in the queue */
                    if(endpointObj->irpQueue != NULL)
                    {
                        /* Copy the IRP from queue to local variable */
                        irp = endpointObj->irpQueue;

                        /* Update status of current IRP */
                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                        /* Transaction Complete. Expecting next Setup Packet from Host */
                        hDriver->endpoint0State = DRV_USBDP_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

                        /* FIFO should be freed */
                        USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_RX_DATA_BK0_Msk)

                        /* Update the IRP queue so that the client can submit an
                         * IRP in the IRP callback. */
                        endpointObj->irpQueue = irp->next;

                        irp->size = 0;

                        if(irp->callback != NULL)
                        {
                            irp->callback((USB_DEVICE_IRP *)irp);
                        }
                    }
                    else
                    {
                        /* We do not have a irp. Change the status, disable the 
                         * interrupt. The rest will be handled in IRPSubmit */
                        hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_RX_STATUS_IRP_FROM_CLIENT;

                        usbID->UDP_IDR = UDP_IDR_EP0INT_Msk;
                    }
                }
                else
                {
                    /* We are in RX Data Stage. Check if we have an irp from client */
                    if(endpointObj->irpQueue == NULL)
                    {
                        /* We do not have a irp. Change the status, disable the 
                         * interrupt. The rest will be handled in IRPSubmit */
                        hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_RX_DATA_IRP_FROM_CLIENT;

                        usbID->UDP_IDR = UDP_IDR_EP0INT_Msk;
                    }
                    else
                    {
                        /* Get the current irp from the Queue */
                        irp = endpointObj->irpQueue;

                        /* Get count of bytes received from host */
                        byteCount = (uint16_t)((usbID->UDP_CSR[0] & UDP_CSR_RXBYTECNT_Msk) >> UDP_CSR_RXBYTECNT_Pos);

                        /* This is not acceptable as it may corrupt the ram location */
                        if((irp->nPendingBytes + byteCount) > irp->size)
                        {
                            byteCount = (uint16_t)(irp->size - irp->nPendingBytes);
                        }

                        /* nPendingBytes has the number of bytes already received.
                         * Use that as a index pointer to copy data from FIFO to irp 
                         * data buffer */
                        for(index = (uint16_t)irp->nPendingBytes; index < ((uint16_t)irp->nPendingBytes + byteCount); index++)
                        {
                            ((uint8_t *)irp->data)[index] = (uint8_t)usbID->UDP_FDR[0];
                        }

                        /* Update the pending byte count with total number of bytes received */
                        irp->nPendingBytes += byteCount;

                        if((irp->nPendingBytes < irp->size) && (byteCount >= endpointObj->maxPacketSize))
                        {
                            /* Total bytes received is less than irp size and current 
                             * data packet is equal to / greater than endpoint size.
                             * This means we are expecting more data from host. Just clear
                             * the banks and re-enable the interrupts */
                            USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_RX_DATA_BK0_Msk)
                        }
                        else
                        {
                            /* We have received all the data from the host. Based on 
                             * nPendingBytes decide if it is short packet or complete 
                             * packet */
                            if(irp->nPendingBytes >= irp->size)
                            {
                                /* All data received. Update irp status accordingly */
                                irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;
                            }
                            else
                            {
                                /* Short Packet received. Update irp status accordingly */
                                irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;
                            }

                            /* Update the Endpoint 0 State */
                            hDriver->endpoint0State = DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT;

                            /* No more data expected. Clear and free up the Bank for transaction */
                            USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_RX_DATA_BK0_Msk)

                            /* Update the irpQueue and irp size. */
                            endpointObj->irpQueue = irp->next;
                            irp->size = irp->nPendingBytes;

                            if(irp->callback != NULL)
                            {
                                irp->callback((USB_DEVICE_IRP *)irp);
                            }
                        }
                    }
                }
            }

            if((usbID->UDP_CSR[0] & UDP_CSR_STALLSENT_Msk) == UDP_CSR_STALLSENT_Msk)
            {
                /* If control endpoint is stalled and it has received a packet 
                 * from host, then clear the Stall */
                USBDP_CSR_CLR_BITS(usbID, 0, UDP_CSR_STALLSENT_Msk)
            }
        }
        for(epIndex = 1; epIndex < DRV_USBDP_ENDPOINTS_NUMBER; epIndex++)
        {
            /* This means this is non-EP0 interrupt. Read the endpoint status
             * register. */

            if(((usbID->UDP_IMR & (UDP_IMR_EP0INT_Msk << epIndex)) != (UDP_IMR_EP0INT_Msk << epIndex)) ||
               ((usbID->UDP_ISR & (UDP_IMR_EP0INT_Msk << epIndex)) != (UDP_IMR_EP0INT_Msk << epIndex)))
            {
                continue;
            }

            /* Fetch the respective endpoint object */
            endpointObj = hDriver->deviceEndpointObj[epIndex];

            if(((usbID->UDP_CSR[epIndex] & UDP_CSR_RX_DATA_BK0_Msk) == UDP_CSR_RX_DATA_BK0_Msk) ||
               ((usbID->UDP_CSR[epIndex] & UDP_CSR_RX_DATA_BK1_Msk) == UDP_CSR_RX_DATA_BK1_Msk))
            {

                /* This means we have received data from the host. Check if we have 
                 * a valid irp in queue */                
                if(endpointObj->irpQueue == NULL)
                {
                    /* We do not have an irp. Disable the interrupt and exit. 
                     * The rest will be handled in IRPSubmit */
                    usbID->UDP_IDR = (UDP_IDR_EP0INT_Msk << epIndex);
                }
                else
                {
                    /* Get the current irp from the Queue */
                    irp = endpointObj->irpQueue;

                    /* Get the count of data received from host */
                    byteCount = (uint16_t)((usbID->UDP_CSR[epIndex] & UDP_CSR_RXBYTECNT_Msk) >> UDP_CSR_RXBYTECNT_Pos);

                    /* This is not acceptable as it may corrupt the ram location */
                    if((irp->nPendingBytes + byteCount) > irp->size)
                    {
                        byteCount = (uint16_t)(irp->size - irp->nPendingBytes);
                    }

                    /* nPendingBytes has the number of bytes already received.
                     * Use that as a index pointer to copy data from FIFO to irp 
                     * data buffer */
                    for(index = (uint16_t)irp->nPendingBytes; index < ((uint16_t)irp->nPendingBytes + byteCount); index++)
                    {
                        ((uint8_t *)irp->data)[index] = (uint8_t)usbID->UDP_FDR[epIndex];
                    }

                    /* Update the pending byte count with total number of bytes received */
                    irp->nPendingBytes += byteCount;

                    if((irp->nPendingBytes < irp->size) && (byteCount >= endpointObj->maxPacketSize))
                    {
                        /* Total bytes received is less than irp size and current 
                         * data packet is equal to / greater than endpoint size.
                         * This means we are expecting more data from host. Just clear
                         * the banks and exit */
                        USBDP_CSR_CLR_BITS(usbID, epIndex, UDP_CSR_RX_DATA_BK0_Msk | UDP_CSR_RX_DATA_BK1_Msk)
                    }
                    else
                    {
                        /* We have received all the data from the host. Based on nPendingBytes
                         * decide if it is short packet or complete packet */
                        if(irp->nPendingBytes >= irp->size)
                        {
                            /* All data received. Update irp status accordingly */
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;
                        }
                        else
                        {
                            /* Short Packet received. Update irp status accordingly */
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;
                        }

                       

                        /* Update the irpQueue and irp size. */
                        endpointObj->irpQueue = irp->next;
                        irp->size = irp->nPendingBytes;

                        if(irp->callback != NULL)
                        {
                            irp->callback((USB_DEVICE_IRP *)irp);
                        }
                        
                         /* No more data expected. Clear and free up the Bank for transaction */
                        USBDP_CSR_CLR_BITS(usbID, epIndex, UDP_CSR_RX_DATA_BK0_Msk | UDP_CSR_RX_DATA_BK1_Msk)

                        /* Re-enable the interrupt. Just in case */
                        usbID->UDP_IER = (UDP_IER_EP0INT_Msk << epIndex);
                    }
                }
            }
            if((usbID->UDP_CSR[epIndex] & UDP_CSR_TXCOMP_Msk) == UDP_CSR_TXCOMP_Msk)
            {
                /* Check if we have a valid IRP in the queue. There should be one, 
                 * as a Tx transaction cannot happen without an IRP */
                if(endpointObj->irpQueue != NULL)
                {
                    /* Copy the IRP from queue to local variable */
                    irp = endpointObj->irpQueue;

                    if(irp->nPendingBytes == 0U)
                    {
                        /* This means we are in Transmission of Data stage.
                         * nPendingBytes is 0, it means all TX data has been
                         * sent. Check if ZLP is to be sent, else mark the IRP
                         * as completed. */                            
                        if((irp->flags & USB_DEVICE_IRP_FLAG_SEND_ZLP) == USB_DEVICE_IRP_FLAG_SEND_ZLP)
                        {
                            /* Need to send ZLP. Clear the size of buffer and
                             * ready the buffer to send data */
                            irp->flags &= ~USB_DEVICE_IRP_FLAG_SEND_ZLP;

                            /* Set TXPKTRDY to send packet and clear interrupt */
                            USBDP_CSR_SET_BITS(usbID, epIndex, UDP_CSR_TXPKTRDY_Msk)
                            USBDP_CSR_CLR_BITS(usbID, epIndex, UDP_CSR_TXCOMP_Msk)
                        }
                        else
                        {
                            /* No need to send ZLP. Mark the IRP status as 
                             * completed and do irp callback. */
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                            /* Update the IRP queue so that the client can submit an
                             * IRP in the IRP callback. */
                            endpointObj->irpQueue = irp->next;

                            if(irp->callback != NULL)
                            {
                                irp->callback((USB_DEVICE_IRP *)irp);
                            }                                    
                                    
                            if(endpointObj->irpQueue != NULL)
                            {           
                                irp = endpointObj->irpQueue;
                                
                                /* Sending from Device to Host */
                                if(irp->nPendingBytes <= endpointObj->maxPacketSize)
                                {
                                    /* Data to be sent is less than endpoint size. It means, 
                                     * this is the last transaction in the transfer or total
                                     * size itself is less than endpoint size */
                                    byteCount = (uint16_t)irp->nPendingBytes;
                                }
                                else
                                {
                                    /* Data to be sent is more than the endpoint size.
                                     * Send only the size of endpoint in this iteration. 
                                     * This could be first or a continuing transaction in the
                                     * transfer */
                                    byteCount = endpointObj->maxPacketSize;
                                }

                                /* Copy the data from irp data buffer to FIFO */
                                for(index = 0; index < byteCount; index++)
                                {
                                    usbID->UDP_FDR[epIndex] = ((uint8_t *)irp->data)[index];
                                }

                                /* Update the nPendingBytes, reduce by byteCount */
                                irp->nPendingBytes -= byteCount;

                                /* Clear the TXPKTRDY flag. Now controller can send the data 
                                 * when host request for it */
                                USBDP_CSR_SET_BITS(usbID, epIndex, UDP_CSR_TXPKTRDY_Msk)
                                USBDP_CSR_CLR_BITS(usbID, epIndex, UDP_CSR_TXCOMP_Msk)
                            }
                            else
                            {
                                
                                USBDP_CSR_CLR_BITS(usbID, epIndex, UDP_CSR_TXCOMP_Msk)
                            }
                        }
                    }
                    else
                    {
                        /* nPendingBytes is not 0 */
                        if(irp->nPendingBytes <= endpointObj->maxPacketSize)
                        {
                            /* Last packet with data less than endpoint size */
                            byteCount = (uint16_t)irp->nPendingBytes;
                        }
                        else
                        {
                            /* We have data more than endpoint size. Send only 
                             * upto endpoint size */
                            byteCount = endpointObj->maxPacketSize;
                        }

                        /* nPendingBytes holds the number of bytes to be sent. Use 
                         * it to know the starting of next packet of data to be 
                         * sent to host */
                        offset = (uint16_t)(irp->size - irp->nPendingBytes);

                        /* copy byteCount size of data from irp data buffer starting 
                         * at offset location to the FIFO */
                        for(index = offset; index < (offset + byteCount); index++)
                        {
                            usbID->UDP_FDR[epIndex] = ((uint8_t *)irp->data)[index];
                        }

                        /* Updated the nPendingBytes */
                        irp->nPendingBytes -= byteCount;

                        /* Set TXPKTRDY to send data and clear the interrupt */
                        USBDP_CSR_SET_BITS(usbID, epIndex, UDP_CSR_TXPKTRDY_Msk)
                        USBDP_CSR_CLR_BITS(usbID, epIndex, UDP_CSR_TXCOMP_Msk)
                    }
                }
                else
                {
                    usbID->UDP_IDR = (UDP_IDR_EP0INT_Msk << epIndex);                    
                }
            }
        }
    }
}/* end of F_DRV_USBDP_Tasks_ISR() */

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 20.7"
</#if>
/* MISRAC 2012 deviation block end */
// *****************************************************************************
/* Function:
    void DRV_USBDP_Handler( void )

  Summary:
    USB interrupt handler.

  Description:
    USB interrupt handler. This is the default USB interrupt handler which will
    call the DRV_USBDP_Tasks_ISR routine to execute the ISR.

  Remarks:
    See drv_usbdp.h for usage information.
*/

void DRV_USBDP_Handler(void)
{

    DRV_USBDP_OBJ * drvObj;               /* USB driver object pointer */

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(sysObj.drvUSBDPObject == SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid driver object */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSBDP Driver: Invalid object in DRV_USBDP_Tasks_ISR()");
    }
    else
    {
        /* Assign the driver object to the local pointer */
        drvObj = &gDrvUSBDPObj[sysObj.drvUSBDPObject];

        if(drvObj->status != SYS_STATUS_READY)
        {
            /* Driver status is not ready, means invalid object */
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nUSBDP Driver: Invalid System Status in DRV_USBDP_Tasks_ISR()");
        }
        else
        {
            /* Clear the interrupt status flag */
            SYS_INT_SourceStatusClear(drvObj->interruptSource);

            /* We are entering an interrupt context */
            drvObj->isInInterruptContext = true;

            /* Driver is running in Device Mode */
            M_DRV_USBDP_TASKS_ISR(drvObj);

            /* We are exiting an interrupt context */
            drvObj->isInInterruptContext = false;
        }
    }
}

// *****************************************************************************
/* Function:
    void DRV_USBDP_ClientEventCallBackSet
    (
        DRV_HANDLE   handle,
        uintptr_t    hReferenceData,
        DRV_USBDP_EVENT_CALLBACK myEventCallBack
    )

  Summary:
    Dynamic implementation of DRV_USBDP_ClientEventCallBackSet client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USBDP_ClientEventCallBackSet
    client interface function.

  Remarks:
    See drv_usbdp.h for usage information.
*/

void DRV_USBDP_ClientEventCallBackSet
(
    DRV_HANDLE   handle,
    uintptr_t    hReferenceData,
    DRV_USB_EVENT_CALLBACK myEventCallBack
)
{
    DRV_USBDP_OBJ * hDriver;                                    /* USB driver object pointer */

    /* Validate all the parameters used in the function. Proceed only if they
     * are valid */
    if(handle == DRV_HANDLE_INVALID)
    {
        /* Received client handle is invalid */
        SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid client handle in DRV_USBDP_ClientEventCallBackSet().");
    }
    else
    {
        /* Assign the driver handle to the local pointer */
        hDriver = (DRV_USBDP_OBJ *) handle;

        if(false == hDriver->isOpened)
        {
            /* The client handle is not open. This is invalid */
            SYS_DEBUG(SYS_ERROR_INFO, "\r\nUSBDP Driver: Invalid client handle in DRV_USBDP_ClientEventCallBackSet().");
        }
        else
        {
            /* Assign event call back and reference data */
            hDriver->hClientArg = hReferenceData;
            hDriver->pEventCallBack = myEventCallBack;
        }
    }
}/* end of DRV_USBDP_ClientEventCallBackSet() */

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.3"
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.6"
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */