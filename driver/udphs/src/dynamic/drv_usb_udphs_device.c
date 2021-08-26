/*******************************************************************************
  USB Device Driver Implementation of device mode operation routines

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_udphs_device.c

  Summary:
    USB Device Driver Dynamic Implementation of device mode operation routines

  Description:
    The USB device driver provides a simple interface to manage the USB
    modules on Microchip microcontrollers.  This file Implements the
    interface routines for the USB driver when operating in device mode.

    While building the driver from source, ALWAYS use this file in the build if
    device mode operation is required.
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


#include "driver/usb/udphs/src/drv_usb_udphs_local.h"
#include "driver/usb/udphs/drv_usb_udphs.h"

/* Array of endpoint objects. Two objects per endpoint */
DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ gDrvUSBControlEndpoints[DRV_USB_UDPHS_INSTANCES_NUMBER] [2];

/* Array of endpoint objects. Two objects per endpoint */
DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ gDrvUSBNonControlEndpoints[DRV_USB_UDPHS_INSTANCES_NUMBER] [DRV_USB_UDPHS_ENDPOINTS_NUMBER - 1];

/******************************************************************************
 * This structure is a pointer to a set of USB Driver Device mode functions.
 * This set is exported to the device layer when the device layer must use the
 * USB Controller.
 *****************************************************************************/

DRV_USB_DEVICE_INTERFACE gDrvUSBUDPHSDeviceInterface =
{
    .open = DRV_USB_UDPHS_Open,
    .close = DRV_USB_UDPHS_Close,
    .eventHandlerSet = DRV_USB_UDPHS_ClientEventCallBackSet,
    .deviceAddressSet = DRV_USB_UDPHS_DEVICE_AddressSet,
    .deviceCurrentSpeedGet = DRV_USB_UDPHS_DEVICE_CurrentSpeedGet,
    .deviceSOFNumberGet = DRV_USB_UDPHS_DEVICE_SOFNumberGet,
    .deviceAttach = DRV_USB_UDPHS_DEVICE_Attach,
    .deviceDetach = DRV_USB_UDPHS_DEVICE_Detach,
    .deviceEndpointEnable = DRV_USB_UDPHS_DEVICE_EndpointEnable,
    .deviceEndpointDisable = DRV_USB_UDPHS_DEVICE_EndpointDisable,
    .deviceEndpointStall = DRV_USB_UDPHS_DEVICE_EndpointStall,
    .deviceEndpointStallClear = DRV_USB_UDPHS_DEVICE_EndpointStallClear,
    .deviceEndpointIsEnabled = DRV_USB_UDPHS_DEVICE_EndpointIsEnabled,
    .deviceEndpointIsStalled = DRV_USB_UDPHS_DEVICE_EndpointIsStalled,
    .deviceIRPSubmit = DRV_USB_UDPHS_DEVICE_IRPSubmit,
    .deviceIRPCancelAll = DRV_USB_UDPHS_DEVICE_IRPCancelAll,
    .deviceRemoteWakeupStop = DRV_USB_UDPHS_DEVICE_RemoteWakeupStop,
    .deviceRemoteWakeupStart = DRV_USB_UDPHS_DEVICE_RemoteWakeupStart,
    .deviceTestModeEnter = DRV_USB_UDPHS_DEVICE_TestModeEnter

};

// *****************************************************************************

/* Function:
    void _DRV_USB_UDPHS_DEVICE_Initialize
    (
        DRV_USB_UDPHS_OBJ * drvObj,
        SYS_MODULE_INDEX index
    )

  Summary:
    Dynamic implementation of _DRV_USB_UDPHS_DEVICE_Initialize client
    interface function.

  Description:
    This is the dynamic implementation of _DRV_USB_UDPHS_DEVICE_Initialize
    client interface initialization function for USB device.
    Function checks the input handle validity and on success initializes the
    driver object. It also freezes the clock, disables all DMA and Endpoint
    interrupts. The endpoint objects are updated with respective pointers

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

void _DRV_USB_UDPHS_DEVICE_Initialize
(
    DRV_USB_UDPHS_OBJ * drvObj,
    SYS_MODULE_INDEX index
)

{

    udphs_registers_t * usbID;      /* USB instance pointer */
    uint8_t count;                  /* Loop Counter */

    _DRV_USB_UDPHS_InitUTMI();    
    
    /* Get the USB H/W instance pointer */
    usbID = drvObj->usbID;

    if(drvObj->operationSpeed == USB_SPEED_FULL)
    {
        /* Force Full-Speed */
        usbID->UDPHS_TST = UDPHS_TST_SPEED_CFG_FULL_SPEED;
    }
    else
    {
        /* Default - High Speed */
        usbID->UDPHS_TST &= UDPHS_TST_SPEED_CFG_NORMAL;
    }

    usbID->UDPHS_CTRL = UDPHS_CTRL_EN_UDPHS_Msk;

    /* Point the objects for control endpoint. It is a bidirectional
     * endpoint, so only one object is needed */
    drvObj->deviceEndpointObj[0] = &gDrvUSBControlEndpoints[index][0];

    /* Point the objects for non control endpoints.
     * They are unidirectional endpoints, so multidimensional
     * array with one object per endpoint direction */

    for(count = 1; count < DRV_USB_UDPHS_ENDPOINTS_NUMBER ; count++)
    {
        drvObj->deviceEndpointObj[count] = &gDrvUSBNonControlEndpoints[index][count - 1];

		usbID->UDPHS_DMA[count].UDPHS_DMACONTROL = 0;
    }

	/* Configure the pull-up on D+ and disconnect it */
	usbID->UDPHS_CTRL |= UDPHS_CTRL_DETACH_Msk;
	usbID->UDPHS_CTRL |= UDPHS_CTRL_PULLD_DIS_Msk;

	/* Reset IP */
	usbID->UDPHS_CTRL &= ~UDPHS_CTRL_EN_UDPHS_Msk;
	usbID->UDPHS_CTRL |= UDPHS_CTRL_EN_UDPHS_Msk;

    /* Disable all endpoints */
	for (count = 0; count < UDPHS_EPT_NUMBER; count++)
    {
		usbID->UDPHS_EPT[count].UDPHS_EPTCFG &= ~UDPHS_EPTCFG_Msk;
		usbID->UDPHS_EPT[count].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_Msk;
    }

	/* Reset DMA */
	for (count = 1; count < UDPHS_DMA_NUMBER; count++)
    {
		/* DMA stop */
		usbID->UDPHS_DMA[count].UDPHS_DMACONTROL = 0;

		/* Reset DMA channel (Buffer count and Control field) */
		usbID->UDPHS_DMA[count].UDPHS_DMACONTROL = UDPHS_DMACONTROL_LDNXT_DSC_Msk;

		/* Clear DMA channel status (read to clear) */
		usbID->UDPHS_DMA[count].UDPHS_DMASTATUS = usbID->UDPHS_DMA[count].UDPHS_DMASTATUS;
	}

	usbID->UDPHS_CLRINT = UDPHS_CLRINT_Msk;

    drvObj->status = SYS_STATUS_READY;

}/* end of _DRV_USB_UDPHS_DEVICE_Initialize() */

// *****************************************************************************

/* Function:
      void DRV_USB_UDPHS_DEVICE_AddressSet(DRV_HANDLE handle, uint8_t address)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_AddressSet client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_AddressSet
    client interface initialization function for USB device. Function checks
    the input handle validity and on success updates the Device General Control
    Register with the address value and enables the address.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

void DRV_USB_UDPHS_DEVICE_AddressSet
(
    DRV_HANDLE handle,
    uint8_t address
)

{

    udphs_registers_t * usbID;          /* USB instance pointer */
    DRV_USB_UDPHS_OBJ * hDriver;          /* USB driver object pointer */

    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_AddressSet().");
    }
    else
    {
        /* Set the device address */
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Disable the device address */
        usbID->UDPHS_CTRL &= ~UDPHS_CTRL_FADDR_EN_Msk;

        /* Clear the existing address field */
        usbID->UDPHS_CTRL &= ~UDPHS_CTRL_DEV_ADDR_Msk;

        /* Copy the new address to Register */
        usbID->UDPHS_CTRL |= UDPHS_CTRL_DEV_ADDR(address);

        /* Enable the device address */
        usbID->UDPHS_CTRL |= UDPHS_CTRL_FADDR_EN_Msk;
    }
}/* end of DRV_USB_UDPHS_DEVICE_AddressSet() */

// *****************************************************************************

/* Function:
      USB_SPEED DRV_USB_UDPHS_DEVICE_CurrentSpeedGet(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_CurrentSpeedGet client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_CurrentSpeedGet
    client interface initialization function for USB device.
    Function checks the input handle validity and on success returns value to
    indicate HIGH/FULL speed operation.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

USB_SPEED DRV_USB_UDPHS_DEVICE_CurrentSpeedGet
(
    DRV_HANDLE handle
)

{

    DRV_USB_UDPHS_OBJ * hDriver;              /* USB driver object pointer */
    USB_SPEED retVal = USB_SPEED_ERROR;     /* Return value */

    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_CurrentSpeedGet().");
    }
    else
    {
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;

        /* The current speed in contained in the
         * device speed member of the driver object */
         retVal = hDriver->deviceSpeed;
    }

    return (retVal);

}/* end of DRV_USB_UDPHS_DEVICE_CurrentSpeedGet() */

// *****************************************************************************

/* Function:
      void DRV_USB_UDPHS_DEVICE_RemoteWakeupStart(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_RemoteWakeupStart client
    interface function.

  Description:
    This is dynamic implementation of DRV_USB_UDPHS_DEVICE_RemoteWakeupStart
    client interface function for USB device.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

void DRV_USB_UDPHS_DEVICE_RemoteWakeupStart(DRV_HANDLE handle)
{

    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_RemoteWakeupStart().");
    }
    else
    {
        /* Not yet implemented */
    }

}/* end of DRV_USB_UDPHS_DEVICE_RemoteWakeupStart() */

// *****************************************************************************

/* Function:
      void DRV_USB_UDPHS_DEVICE_RemoteWakeupStop(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_RemoteWakeupStop client
    interface function.

  Description:
    This is dynamic implementation of DRV_USB_UDPHS_DEVICE_RemoteWakeupStop
    client interface function for USB device.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

void DRV_USB_UDPHS_DEVICE_RemoteWakeupStop(DRV_HANDLE handle)
{

    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_RemoteWakeupStop().");
    }
    else
    {
        /* Not yet implemented */
    }

}/* end of DRV_USB_UDPHS_DEVICE_RemoteWakeupStop() */

// *****************************************************************************

/* Function:
      void DRV_USB_UDPHS_DEVICE_Attach(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_Attach client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_Attach
    client interface function for USB device.
    This function checks if the handle passed is valid and if so, performs the
    device attach operation. EOR, SUSP, WAKEUP, & SOF interrupts are enabled and
    EOR, WAKEUP, & SOF interrupts are cleared.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

void DRV_USB_UDPHS_DEVICE_Attach
(
    DRV_HANDLE handle
)

{

    udphs_registers_t * usbID;                  /* USB instance pointer */
    DRV_USB_UDPHS_OBJ * hDriver;                  /* USB driver object pointer */


    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_Attach().");
    }
    else
	{
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        usbID = hDriver->usbID;

		usbID->UDPHS_CTRL |= UDPHS_CTRL_PULLD_DIS_Msk;
		usbID->UDPHS_CTRL &= ~UDPHS_CTRL_DETACH_Msk;
    }

}/* end of DRV_USB_UDPHS_DEVICE_Attach() */

// *****************************************************************************

/* Function:
      void DRV_USB_UDPHS_DEVICE_Detach(DRV_HANDLE handle)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_Detach client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_Detach
    client interface function for USB device.
    This function checks if the passed handle is valid and if so, performs a
    device detach operation.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

void DRV_USB_UDPHS_DEVICE_Detach(DRV_HANDLE handle)
{


    udphs_registers_t * usbID;                  /* USB instance pointer */
    DRV_USB_UDPHS_OBJ * hDriver;                  /* USB driver object pointer */
    USB_ERROR retVal = USB_ERROR_NONE;
    bool interruptWasEnabled = false;       /* To track interrupt state */
    uint8_t count;

    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Driver Handle is invalid in DRV_USB_UDPHS_DEVICE_Detach().");
    }
    else if(true == ((DRV_USB_UDPHS_OBJ *) handle)->inUse)
    {
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        usbID = hDriver->usbID;
		
        if(false == hDriver->isInInterruptContext)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                /* Disable  the USB Interrupt as this is not called inside ISR */
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);                
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex lock failed in DRV_USB_UDPHS_DEVICE_Detach()");
                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }
        if(retVal == USB_ERROR_NONE)
        {
            /* Update the driver flag indicating detach */
            hDriver->isAttached = false;
            
            /* Disable all endpoints */
			for (count = 0; count < UDPHS_EPT_NUMBER; count++)
		    {
				usbID->UDPHS_EPT[count].UDPHS_EPTCFG &= ~UDPHS_EPTCFG_Msk;
				usbID->UDPHS_EPT[count].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_Msk;
		    }

			/* Configure the pull-up on D+ and disconnect it */
			usbID->UDPHS_CTRL |= UDPHS_CTRL_DETACH_Msk;
			usbID->UDPHS_CTRL |= UDPHS_CTRL_PULLD_DIS_Msk;

			/* Reset IP */
			usbID->UDPHS_CTRL &= ~UDPHS_CTRL_EN_UDPHS_Msk;
			usbID->UDPHS_CTRL |= UDPHS_CTRL_EN_UDPHS_Msk;
			
        }
        if(false == hDriver->isInInterruptContext)
        {
            if(true == interruptWasEnabled)
            {
                /* IF the interrupt was enabled when entering the routine
                 * re-enable it now */
                SYS_INT_SourceEnable(hDriver->interruptSource);

                /* Unlock the mutex */
                OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID);
            }
        }
    }
}/* end of DRV_USB_UDPHS_DEVICE_Detach() */

// *****************************************************************************

/* Function:
      uint16_t DRV_USB_UDPHS_DEVICE_SOFNumberGet(DRV_HANDLE client)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_SOFNumberGet client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_SOFNumberGet
    client interface function for USB device.
    Function checks the validity of the input arguments and on success returns
    the Frame count value.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

uint16_t DRV_USB_UDPHS_DEVICE_SOFNumberGet(DRV_HANDLE handle)
{
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Handle in DRV_USB_UDPHS_DEVICE_SOFNumberGet().");
    }


    return (0);

}/* end of DRV_USB_UDPHS_DEVICE_SOFNumberGet() */

// *****************************************************************************

/* Function:
    void _DRV_USB_UDPHS_DEVICE_IRPQueueFlush
    (
        DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj,
        USB_DEVICE_IRP_STATUS status
    )

  Summary:
    Dynamic implementation of _DRV_USB_UDPHS_DEVICE_IRPQueueFlush function.

  Description:
    This is the dynamic implementation of _DRV_USB_UDPHS_DEVICE_IRPQueueFlush
    function for USB device.
    Function scans for all the IRPs on the endpoint queue and cancels them all.

  Remarks:
    This is a local function and should not be called directly by the
    application.
 */

void _DRV_USB_UDPHS_DEVICE_IRPQueueFlush
(
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj,
    USB_DEVICE_IRP_STATUS status
)

{

    USB_DEVICE_IRP_LOCAL * iterator;                  /* Local IRP object for iterations */

    /* Check if any IRPs are assigned on this endpoint and abort them */
    if(endpointObj->irpQueue != NULL)
    {
        /* Cancel the IRP and deallocate driver IRP
         * objects */

        iterator = endpointObj->irpQueue;

        while (iterator != NULL)
        {
            iterator->status = status;
            if(iterator->callback != NULL)
            {
                iterator->callback((USB_DEVICE_IRP *) iterator);
            }
            iterator = iterator->next;
        }
    }

    /* Set the head pointer to NULL */
    endpointObj->irpQueue = NULL;

}/* end of _DRV_USB_UDPHS_DEVICE_IRPQueueFlush() */

// *****************************************************************************

/* Function:
    void _DRV_USB_UDPHS_DEVICE_EndpointObjectEnable
    (
        DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj,
        uint16_t endpointSize,
        USB_TRANSFER_TYPE endpointType,
        USB_DATA_DIRECTION endpointDirection
    )

  Summary:
    Dynamic implementation of _DRV_USB_UDPHS_DEVICE_EndpointObjectEnable
    function.

  Description:
    This is the dynamic implementation of
    _DRV_USB_UDPHS_DEVICE_EndpointObjectEnable function for USB device.
    Function populates the endpoint object data structure and sets it to
    enabled state.

  Remarks:
    This is a local function and should not be called directly by the
    application.
 */

void _DRV_USB_UDPHS_DEVICE_EndpointObjectEnable
(
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj,
    uint16_t endpointSize,
    USB_TRANSFER_TYPE endpointType,
    USB_DATA_DIRECTION endpointDirection
)

{

    /* This is a helper function */

    endpointObj->irpQueue = NULL;
    endpointObj->maxPacketSize = endpointSize;
    endpointObj->endpointType = endpointType;
    endpointObj->endpointState = DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED;
    endpointObj->endpointDirection = endpointDirection;

}/* end of _DRV_USB_UDPHS_DEVICE_EndpointObjectEnable() */

// *****************************************************************************

/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointEnable
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection,
        USB_TRANSFER_TYPE endpointType,
        uint16_t endpointSize
    )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_EndpointEnable client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_EndpointEnable
    client interface function for USB device.
    Function enables the specified endpoint.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointEnable
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection,
    USB_TRANSFER_TYPE endpointType,
    uint16_t endpointSize
)

{
    udphs_registers_t * usbID;                  /* USB instance pointer */
    DRV_USB_UDPHS_OBJ * hDriver;                  /* USB driver object pointer */
    uint8_t direction;                          /* Endpoint Direction */
    uint8_t endpoint;                           /* Endpoint Number */
    uint8_t fifoSize = 0;                       /* FIFO size */
    uint16_t defaultEndpointSize = 8;           /* Default size of Endpoint */
    uint32_t regEPTCFG = 0;                     /* Register Value holder */
    bool interruptWasEnabled = false;           /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */
    DRV_USB_UDPHS_ENDPOINT_BANKS bankCount = DRV_USB_UDPHS_ENDPOINT_BANKS_ZERO;   /* Number of Banks to be used for Endpoints */

    /* Endpoint object pointer */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj;


    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else if(endpointSize < 8 || endpointSize > 1024)
    {
        /* Endpoint size is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid endpoint size in DRV_USB_UDPHS_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else
    {   /* Handle and endpoint size are valid, Enable endpoint */

        /* Extract the Endpoint number and its direction */
        endpoint = endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_NUMBER_MASK;
        direction = ((endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_DIRECTION_MASK) != 0);
        bankCount = (DRV_USB_UDPHS_ENDPOINT_BANKS) DRV_USB_UDPHS_NUMBER_OF_BANKS;

        /* Get the endpoint object */
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        usbID = hDriver->usbID;
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        /* Find upper 2 power number of endpointSize */
        if(endpointSize)
        {
            while (defaultEndpointSize < endpointSize)
            {
                fifoSize++;
                defaultEndpointSize <<= 1;
            }
        }

        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                /* Disable  the USB Interrupt as this is not called inside ISR */
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex lock failed in DRV_USB_UDPHS_DEVICE_EndpointEnable()");

                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }

        if(retVal == USB_ERROR_NONE)
        {

            /* There are two endpoint objects for a control endpoint.
             * Enable the first endpoint object */

            _DRV_USB_UDPHS_DEVICE_EndpointObjectEnable
            (
                endpointObj, endpointSize, endpointType, USB_DATA_DIRECTION_HOST_TO_DEVICE
            );

            if(endpoint == 0)
            {
                endpointObj++;

                 /* Enable the second endpoint object */

                _DRV_USB_UDPHS_DEVICE_EndpointObjectEnable
                (
                    endpointObj, endpointSize, endpointType, USB_DATA_DIRECTION_DEVICE_TO_HOST
                );
            }

            /* Endpoint Size */
            regEPTCFG |= UDPHS_EPTCFG_EPT_SIZE(fifoSize);

            /* Endpoint Type */
            regEPTCFG |= UDPHS_EPTCFG_EPT_TYPE(endpointType);

            /* Bank Count */
            regEPTCFG |= UDPHS_EPTCFG_BK_NUMBER(bankCount);

            if(endpoint == 0)
            {
                /* Endpoint Direction */
                regEPTCFG |= UDPHS_EPTCFG_EPT_DIR(0);
            }
            else
            {
                /* Endpoint Direction */
                regEPTCFG |= UDPHS_EPTCFG_EPT_DIR(direction);
            }

            /* Copy the configuration to the EPTCFG register */
            usbID->UDPHS_EPT[endpoint].UDPHS_EPTCFG = regEPTCFG;

            if((usbID->UDPHS_EPT[endpoint].UDPHS_EPTCFG & UDPHS_EPTCFG_EPT_MAPD_Msk) == UDPHS_EPTCFG_EPT_MAPD_Msk)
            {

                /* Enable endpoint according to the device configuration */
                usbID->UDPHS_EPT[endpoint].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_EPT_ENABL_Msk;

                if(endpoint == 0)
                {
                    /* Enable RX_SETUP interrupt */
                    usbID->UDPHS_EPT[endpoint].UDPHS_EPTCTLENB = (UDPHS_EPTCTLENB_RX_SETUP_Msk);
                }
                else
                {
                    if(direction == USB_DATA_DIRECTION_DEVICE_TO_HOST)
                    {
                        usbID->UDPHS_EPT[endpoint].UDPHS_EPTCTLENB = (UDPHS_EPTCTLENB_TX_COMPLT_Msk);
                    }
                    else
                    {
                        usbID->UDPHS_EPT[endpoint].UDPHS_EPTCTLENB = (UDPHS_EPTCTLENB_RXRDY_TXKL_Msk);
                    }
                }

                /* Enable the Endpoint Interrupt */
                usbID->UDPHS_IEN |= UDPHS_IEN_EPT_0_Msk << (endpoint);
            }
            else
            {
                /* Endpoint configuration is not successful */
                retVal = USB_ERROR_ENDPOINT_NOT_CONFIGURED;
            }
        }

        /* Restore the interrupt enable status if this was modified. */
        if(hDriver->isInInterruptContext == false)
        {
            /* Release the mutex */
            if(OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID) == OSAL_RESULT_TRUE)
            {
                if(interruptWasEnabled)
                {
                    /* Enable the interrupt only if it was disabled */
                    SYS_INT_SourceEnable(hDriver->interruptSource);
                }
            }
            else
            {
                /* There was an error in releasing the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex unlock failed in DRV_USB_UDPHS_DEVICE_EndpointEnable().");

                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }
    }

    return (retVal);

}/* end of DRV_USB_UDPHS_DEVICE_EndpointEnable() */

// *****************************************************************************

/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointDisable
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_EndpointDisable client
    interface function.

  Description:
    This is dynamic implementation of DRV_USB_UDPHS_DEVICE_EndpointDisable
    client interface function for USB device.
    Function disables the specified endpoint.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointDisable
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{
    /* This routine disables the specified endpoint.
     * It does not check if there is any ongoing
     * communication on the bus through the endpoint
     */

    udphs_registers_t * usbID;                  /* USB instance pointer */
    DRV_USB_UDPHS_OBJ * hDriver;                  /* USB driver object pointer */
    uint32_t loopIndex;                         /* Endpoint loop counter */
    uint8_t endpoint;                           /* Endpoint Number */
    bool interruptWasEnabled = false;           /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */

    /* Endpoint object pointer */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_EndpointDisable().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {   /* Handle is valid, disable endpoint */

        /* Extract the Endpoint number */
        endpoint = endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_NUMBER_MASK;

        /* Get the driver object handle pointer and USB HW instance*/
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        usbID = hDriver->usbID;
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                /* Disable  the USB Interrupt as this is not called inside ISR */
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex lock failed in DRV_USB_UDPHS_DEVICE_EndpointDisable()");

                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }

        if(retVal == USB_ERROR_NONE)
        {
            if(endpointAndDirection == DRV_USB_UDPHS_DEVICE_ENDPOINT_ALL)
            {
                endpointObj = hDriver->deviceEndpointObj[0];
                endpointObj->endpointState  &= ~DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED;
                endpointObj++;
                endpointObj->endpointState  &= ~DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED;
                endpointObj++;

                usbID->UDPHS_EPT[0].UDPHS_EPTCTLDIS |= UDPHS_EPTCTLDIS_EPT_DISABL_Msk;

                for(loopIndex = 1; loopIndex < DRV_USB_UDPHS_ENDPOINTS_NUMBER; loopIndex ++)
                {
                    endpointObj = hDriver->deviceEndpointObj[loopIndex];

                    /* Update the endpoint database */
                    endpointObj->endpointState  &= ~DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED;

                    usbID->UDPHS_EPT[loopIndex].UDPHS_EPTCTLDIS |= UDPHS_EPTCTLDIS_EPT_DISABL_Msk;
                }
            }
            else
            {
                /* Disable the endpoint and update the endpoint database. */
                endpointObj->endpointState &= ~DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED;

                if(endpoint == 0)
                {
                    endpointObj++;

                    endpointObj->endpointState &= ~DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED;
                }

                /* Disable the Endpoint */
                usbID->UDPHS_EPT[endpoint].UDPHS_EPTCTLDIS |= UDPHS_EPTCTLDIS_EPT_DISABL_Msk;
            }

            /* Restore the interrupt enable status if this was modified. */
            if(hDriver->isInInterruptContext == false)
            {
                /* Release the mutex */
                if(OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID) == OSAL_RESULT_TRUE)
                {
                    if(interruptWasEnabled)
                    {
                        /* Enable the interrupt only if it was disabled */
                        SYS_INT_SourceEnable(hDriver->interruptSource);
                    }
                }
                else
                {
                    /* There was an error in releasing the mutex */
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex unlock failed in DRV_USB_UDPHS_DEVICE_EndpointDisable().");

                    retVal = USB_ERROR_OSAL_FUNCTION;
                }
            }
        }
    }

    return (retVal);
}/* end of DRV_USB_UDPHS_DEVICE_EndpointDisable() */

// *****************************************************************************

/* Function:
    bool DRV_USB_UDPHS_DEVICE_EndpointIsEnabled
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_EndpointIsEnabled client
    interface function.

  Description:
    This is the dynamic implementation of
    DRV_USB_UDPHS_DEVICE_EndpointIsEnabled client interface function for
    USB device.
    Function returns the state of specified endpoint(true\false) signifying
    whether the endpoint is enabled or not.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

bool DRV_USB_UDPHS_DEVICE_EndpointIsEnabled
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{

    DRV_USB_UDPHS_OBJ * hDriver;                  /* USB driver object pointer */
    uint8_t endpoint;                           /* Endpoint Number */
    bool retVal = true;                         /* Return value */

    /* Endpoint object pointer */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj;


    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_EndpointIsEnabled().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Extract the Endpoint number */
        endpoint = endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_NUMBER_MASK;

        /* Get the driver object handle pointer and USB HW instance*/
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if((endpointObj->endpointState & DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED) == 0)
        {
            retVal = false;
        }
        else
        {
            /* return true */
        }
    }

    return (retVal);

}/* end of DRV_USB_UDPHS_DEVICE_EndpointIsEnabled() */


// *****************************************************************************

/* Function:
      USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointStall
      (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
      )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_EndpointStall client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_EndpointStall
    client interface function for USB device.
    Function sets the STALL state of the specified endpoint.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointStall
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{

    udphs_registers_t * usbID;                  /* USB instance pointer */
    DRV_USB_UDPHS_OBJ * hDriver;                  /* USB driver object pointer */
    uint8_t endpoint;                           /* Endpoint Number */
    bool interruptWasEnabled = false;           /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */

    /* Endpoint object pointer */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj;


    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_EndpointStall().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Extract the Endpoint number */
        endpoint = endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_NUMBER_MASK;

        /* Get the endpoint object */
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        usbID = hDriver->usbID;
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                /* Disable  the USB Interrupt as this is not called inside ISR */
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex lock failed in DRV_USB_UDPHS_DEVICE_EndpointStall()");

                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }

        if(retVal == USB_ERROR_NONE)
        {
            endpointObj->endpointState |= DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_STALLED;

            _DRV_USB_UDPHS_DEVICE_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT);

            if(endpoint == 0)
            {
                /* While stalling endpoint 0 we stall both directions */
                endpointObj++;

                endpointObj->endpointState |= DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_STALLED;

                _DRV_USB_UDPHS_DEVICE_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT);
            }

            /* Force a Stall to the Endpoint */
            usbID->UDPHS_EPT[endpoint].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_FRCESTALL_Msk;

            /* Restore the interrupt enable status if this was modified. */
            if(hDriver->isInInterruptContext == false)
            {
                /* Release the mutex */
                if(OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID) == OSAL_RESULT_TRUE)
                {
                    if(interruptWasEnabled)
                    {
                        /* Enable the interrupt only if it was disabled */
                        SYS_INT_SourceEnable(hDriver->interruptSource);
                    }
                }
                else
                {
                    /* There was an error in releasing the mutex */
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex unlock failed in DRV_USB_UDPHS_DEVICE_EndpointStall().");

                    retVal = USB_ERROR_OSAL_FUNCTION;
                }
            }
        }
    }

    return (retVal);
}/* end of DRV_USB_UDPHS_DEVICE_EndpointStall() */

// *****************************************************************************

/* Function:
      USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointStallClear
      (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
      )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_EndpointStallClear client
    interface function.

  Description:
    This is the dynamic implementation of
    DRV_USB_UDPHS_DEVICE_EndpointStallClear client interface function for
    USB device.
    Function clears the STALL state of the specified endpoint and resets the
    data toggle value.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointStallClear
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{
    udphs_registers_t * usbID;                  /* USB instance pointer */
    DRV_USB_UDPHS_OBJ * hDriver;                  /* USB driver object pointer */
    uint8_t endpoint;                           /* Endpoint Number */
    bool interruptWasEnabled = false;           /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;          /* Return value */

    /* Endpoint object pointer */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj;


    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_EndpointStallClear().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Extract the Endpoint number */
        endpoint = endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_NUMBER_MASK;

        /* Get the endpoint object */
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        usbID = hDriver->usbID;
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                /* Disable  the USB Interrupt as this is not called inside ISR */
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex lock failed in DRV_USB_UDPHS_DEVICE_EndpointStallClear()");

                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }

        if(retVal == USB_ERROR_NONE)
        {
            /* Update the endpoint object with stall Clear */
            endpointObj->endpointState &= ~DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_STALLED;

            _DRV_USB_UDPHS_DEVICE_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_TERMINATED_BY_HOST);

            if(endpoint == 0)
            {

                endpointObj++;

                endpointObj->endpointState &= ~DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_STALLED;

                _DRV_USB_UDPHS_DEVICE_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_TERMINATED_BY_HOST);
            }

            /* Clear the STALL request */
            usbID->UDPHS_EPT[endpoint].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_FRCESTALL_Msk;

            /* Clear the STALL request */
            usbID->UDPHS_EPT[endpoint].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TOGGLESQ_Msk;

            /* Restore the interrupt enable status if this was modified. */
            if(hDriver->isInInterruptContext == false)
            {
                /* Release the mutex */
                if(OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID) == OSAL_RESULT_TRUE)
                {
                    if(interruptWasEnabled)
                    {
                        /* Enable the interrupt only if it was disabled */
                        SYS_INT_SourceEnable(hDriver->interruptSource);
                    }
                }
                else
                {
                    /* There was an error in releasing the mutex */
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex unlock failed in DRV_USB_UDPHS_DEVICE_EndpointStallClear().");

                    retVal = USB_ERROR_OSAL_FUNCTION;
                }
            }
        }
    }

    return (retVal);
}/* end of DRV_USB_UDPHS_DEVICE_EndpointStallClear() */

// *****************************************************************************

/* Function:
    bool DRV_USB_UDPHS_DEVICE_EndpointIsStalled(DRV_HANDLE client,
                                        USB_ENDPOINT endpointAndDirection)

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_EndpointIsStalled client
    interface function.

  Description:
    This is the dynamic implementation of
    DRV_USB_UDPHS_DEVICE_EndpointIsStalled client interface function for
    USB device.
    Function returns the state of specified endpoint(true\false) signifying
    whether the endpoint is STALLed or not.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

bool DRV_USB_UDPHS_DEVICE_EndpointIsStalled
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{
    DRV_USB_UDPHS_OBJ * hDriver;                  /* USB driver object pointer */
    uint8_t endpoint;                           /* Endpoint Number */
    bool retVal = true;                         /* Return value */

    /* Endpoint object pointer */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj;


    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_EndpointIsStalled().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        /* Extract the Endpoint number */
        endpoint = endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_NUMBER_MASK;

        /* Get the endpoint object */
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if((endpointObj->endpointState & DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_STALLED) != DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_STALLED)
        {
            retVal = false;
        }
        else
        {
            /* return true */
        }
    }
    return (retVal);

}/* end of DRV_USB_UDPHS_DEVICE_EndpointIsStalled() */

// *****************************************************************************

/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_IRPSubmit
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection,
        USB_DEVICE_IRP * inputIRP
    )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_IRPSubmit client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_IRPSubmit
    client interface function for USB device.
    Function checks the validity of the input arguments and on success adds the
    IRP to endpoint object queue linked list.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */
USB_ERROR DRV_USB_UDPHS_DEVICE_IRPSubmit
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection,
    USB_DEVICE_IRP * inputIRP
)
{

    udphs_registers_t * usbID;                      /* USB instance pointer */
    DRV_USB_UDPHS_OBJ * hDriver;                      /* USB driver object pointer */
    USB_DEVICE_IRP_LOCAL * irp;                     /* Pointer to irp data structure */
    uint8_t direction;                              /* Endpoint Direction */
    uint8_t endpoint;                               /* Endpoint Number */
    uint16_t count;                                 /* Loop Counter */
    uint16_t offset;                                /* Buffer Offset */
    volatile uint8_t * fifoAddPtr;                  /* pointer variable for local use */
    uint8_t * data;                                 /* pointer to irp data array */
    uint16_t endpoint0DataStageDirection;           /* Direction of Endpoint 0 Data Stage */
    uint16_t endpoint0DataStageSize;                /* Size of Endpoint 0 Data Stage */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj;  /* Endpoint object pointer */
    uint16_t byteCount = 0;                         /* To hold received byte count */
    bool interruptWasEnabled = false;               /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;              /* Return value */


    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalide Driver Handle in DRV_USB_UDPHS_DEVICE_IRPSubmit().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else if(NULL == inputIRP)
    {
        /* This means that the IRP is in use */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid inputIRP in DRV_USB_UDPHS_DEVICE_IRPSubmit().");

        retVal = USB_ERROR_DEVICE_IRP_IN_USE;
    }
    else if(inputIRP->status > USB_DEVICE_IRP_STATUS_SETUP)
    {
        /* This means that the IRP is in use */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Device IRP is already in use in DRV_USB_UDPHS_DEVICE_IRPSubmit().");

        retVal = USB_ERROR_DEVICE_IRP_IN_USE;
    }
    else
    {

        /* Extract the Endpoint number and its direction */
        endpoint = endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_NUMBER_MASK;
        direction = ((endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_DIRECTION_MASK) != 0);

        irp = (USB_DEVICE_IRP_LOCAL *) inputIRP;

        /* Get the endpoint object */
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;
        usbID = hDriver->usbID;
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if(endpoint == 0)
        {
            endpointObj += direction;
        }

        if(DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED != (endpointObj->endpointState & DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED))
        {
            /* This means the endpoint is not enabled */
            retVal = USB_ERROR_ENDPOINT_NOT_CONFIGURED;
        }
        else
        {
            /* Check the size of the IRP. If the endpoint receives data from
             * the host, then IRP size must be multiple of maxPacketSize. If
             * the send ZLP flag is set, then size must be multiple of
             * endpoint size. */

            if((irp->size % endpointObj->maxPacketSize) == 0)
            {
                /* The IRP size is either 0 or a exact multiple of maxPacketSize */

                if(USB_DATA_DIRECTION_DEVICE_TO_HOST == direction)
                {
                    /* If the IRP size is an exact multiple of endpoint size and
                     * size is not 0 and if data complete flag is set,
                     * then we must send a ZLP */
                    if(((irp->flags & USB_DEVICE_IRP_FLAG_DATA_COMPLETE) == USB_DEVICE_IRP_FLAG_DATA_COMPLETE) && (irp->size != 0))
                    {
                        /* This means a ZLP should be sent after the data is sent */

                        irp->flags |= USB_DEVICE_IRP_FLAG_SEND_ZLP;
                    }
                }
            }
            else
            {
                /* Not exact multiple of maxPacketSize */
                if(USB_DATA_DIRECTION_HOST_TO_DEVICE == direction)
                {
                    /* For receive IRP it needs to exact multiple of maxPacketSize.
                     * Hence this is an error condition. */
                    retVal = USB_ERROR_PARAMETER_INVALID;
                }
            }

            if(retVal == USB_ERROR_NONE)
            {
                /* Now we check if the interrupt context is active. If so the we dont need
                 * to get a mutex or disable interrupts.  If this were being done in non
                 * interrupt context, we, then we would disable the interrupt. In which case
                 * we would get the mutex and then disable the interrupt */
                if(hDriver->isInInterruptContext == false)
                {
                    if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
                    {
                        /* Disable  the USB Interrupt as this is not called inside ISR */
                        interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
                    }
                    else
                    {
                        /* There was an error in getting the mutex */
                        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex lock failed in DRV_USB_UDPHS_DEVICE_IRPSubmit()");

                        retVal = USB_ERROR_OSAL_FUNCTION;
                    }
                }
            }

            if(retVal == USB_ERROR_NONE)
            {
                irp->next = NULL;

                /* Mark the IRP status as pending */
                irp->status = USB_DEVICE_IRP_STATUS_PENDING;

                /* If the data is moving from device to host then pending bytes is data
                 * remaining to be sent to the host. If the data is moving from host to
                 * device, nPendingBytes tracks the amount of data received so far */

                if(USB_DATA_DIRECTION_DEVICE_TO_HOST == direction)
                {
                    irp->nPendingBytes = irp->size;
                }
                else
                {
                    irp->nPendingBytes = 0;
                }

                /* Get the last object in the endpoint object IRP Queue */
                if(endpointObj->irpQueue == NULL)
                {
                    /* Queue is empty */
                    endpointObj->irpQueue = irp;
                    irp->previous = NULL;

                    /* Because this is the first IRP in the queue then we we must arm the
                     * endpoint entry in the BDT. */
                    irp->status = USB_DEVICE_IRP_STATUS_IN_PROGRESS;

                    if(endpoint == 0)
                    {
                        if(direction == USB_DATA_DIRECTION_HOST_TO_DEVICE)
                        {
                            switch(hDriver->endpoint0State)
                            {
                                case DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST:

                                    /* This is the default initialization value of Endpoint
                                     * 0.  In this state EPO is waiting for the setup packet
                                     * from the host. The IRP is already added to the queue.
                                     * When the host send the Setup packet, this IRP will be
                                     * processed in the interrupt. This means we dont have
                                     * to do anything in this state. */

                                    break;

                                case DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_SETUP_IRP_FROM_CLIENT:

                                    /* In this state, the driver has received the Setup
                                     * packet from the host, but was waiting for an IRP from
                                     * the client. The driver now has the IRP. We can unload
                                     * the setup packet into the IRP */

                                    fifoAddPtr = ENDPOINT_FIFO_ADDRESS(0);

                                	data = (uint8_t *)irp->data;

                                    __DMB();

	                                for(count = 0; count < 8; count++)
	                                {
	                                    *((uint8_t *)(data + count)) = *fifoAddPtr++;
	                                }

                                    __DMB();

                                    irp->nPendingBytes += count;

                                    /* Clear the Setup Interrupt flag and also re-enable the
                                     * setup interrupt. */
                                    usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RX_SETUP_Msk;

                                    usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RX_SETUP_Msk;

                                    endpoint0DataStageSize = *((uint16_t *) (data + 6));
                                    endpoint0DataStageDirection = ((data[0] & DRV_USB_UDPHS_ENDPOINT_DIRECTION_MASK) != 0);

                                    if(endpoint0DataStageSize == 0)
                                    {
                                        /* This means there is no data stage. We wait for
                                         * the client to submit the status IRP. */
                                        hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT;
                                    }
                                    else if(endpoint0DataStageDirection == USB_DATA_DIRECTION_DEVICE_TO_HOST)
                                    {
                                        /* If data is moving from device to host, then
                                         * we wait for the client to submit an transmit
                                         * IRP */
                                        hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT;
                                    }
                                    else
                                    {
                                        /* Data is moving from host to device. We wait
                                         * for the host to send the data. */
                                        hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST;
                                    }

                                    /* Indicate that this is a setup IRP */
                                    irp->status = USB_DEVICE_IRP_STATUS_SETUP;

                                    irp->size = 8;

                                    /* Update the IRP queue so that the client can submit an
                                     * IRP in the IRP callback. */
                                    endpointObj->irpQueue = irp->next;

                                    irp->status = USB_DEVICE_IRP_STATUS_SETUP;

                                    /* IRP callback */
                                    if(irp->callback != NULL)
                                    {
                                        irp->callback((USB_DEVICE_IRP *)irp);
                                    }
                                    break;

                                case DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST:
                                case DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE:


                                    break;

                                case DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_DATA_IRP_FROM_CLIENT:

                                    fifoAddPtr = ENDPOINT_FIFO_ADDRESS(0);

                                    byteCount = ((usbID->UDPHS_EPT[0].UDPHS_EPTSTA & UDPHS_EPTSTA_BYTE_COUNT_Msk) >> UDPHS_EPTSTA_BYTE_COUNT_Pos);

                                    data = (uint8_t *)irp->data;

                                    data = (uint8_t *)&data[irp->nPendingBytes];

                                    if((irp->nPendingBytes + byteCount) > irp->size)
                                    {
                                        /* This is not acceptable as it may corrupt the ram location */
                                        byteCount = irp->size - irp->nPendingBytes;
                                    }

                                    __DMB();

                                    for(count = 0; count < byteCount; count++)
                                    {
                                        *((uint8_t *)(data + count)) = *fifoAddPtr++;
                                    }

                                    __DMB();

                                    irp->nPendingBytes += byteCount;

                                    if(irp->nPendingBytes >= irp->size)
                                    {
                                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                                        hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE;

                                        /* Update the queue, update irp-size to indicate
                                         * how much data was received from the host. */
                                        irp->size = irp->nPendingBytes;

                                        endpointObj->irpQueue = irp->next;

                                        if(irp->callback != NULL)
                                        {
                                            irp->callback((USB_DEVICE_IRP *)irp);
                                        }
                                    }
                                    else if(byteCount < endpointObj->maxPacketSize)
                                    {
                                        /* This means we received a short packet. We
                                         * should end the transfer. */
                                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;

                                        hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE;

                                        /* Update the queue, update irp-size to indicate
                                         * how much data was received from the host. */
                                        irp->size = irp->nPendingBytes;

                                        endpointObj->irpQueue = irp->next;

                                        if(irp->callback != NULL)
                                        {
                                            irp->callback((USB_DEVICE_IRP *)irp);
                                        }
                                    }

                                    break;

                                case DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_IRP_FROM_CLIENT:

                                    /* This means the host has already sent an RX status
                                     * stage but there was not IRP to receive this. We have
                                     * the IRP now. We change the EP0 state to waiting for
                                     * the next setup from the host. */

                                    irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                                    hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

                                    endpointObj->irpQueue = irp->next;

                                    if(irp->callback != NULL)
                                    {
                                        irp->callback((USB_DEVICE_IRP *)irp);
                                    }

                                    break;

                                case DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT:


                                    break;

                                default:

                                    break;
                            }
                        }
                        else
                        {

                            /* This means data is moving device to host. */
                            switch(hDriver->endpoint0State)
                            {
                                case DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT:

                                    /* Data IN stage of control transfer.
                                     * Driver is waiting for an IRP from the client and has
                                     * received it. Determine the transaction size. */
                                    if(irp->nPendingBytes < endpointObj->maxPacketSize)
                                    {
                                        /* This is the last transaction in the transfer. */
                                        byteCount = irp->nPendingBytes;
                                    }
                                    else
                                    {
                                        /* This is first or a continuing transaction in the
                                         * transfer and the transaction size must be
                                         * maxPacketSize */

                                        byteCount = endpointObj->maxPacketSize;
                                    }

                                    fifoAddPtr = ENDPOINT_FIFO_ADDRESS(0);

                                    data = (uint8_t *)irp->data;

                                    __DMB();

                                    for(count = 0; count < byteCount; count++)
                                    {
                                        *fifoAddPtr++ = *((uint8_t *)(data + count));
                                    }

                                    __DMB();

                                    irp->nPendingBytes -= byteCount;

                                    hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_TX_DATA_STAGE_IN_PROGRESS;

                                    /* Clear the flag and enable the interrupt. The rest of
                                     * the IRP should really get processed in the ISR.
                                     * */

                                    usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                                    usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;

                                    usbID->UDPHS_EPT[0].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                                    break;

                                case DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT:

                                    hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE;

                                    usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                                    usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;

                                    usbID->UDPHS_EPT[0].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                                    break;

                                default:

                                    break;

                            }
                        }
                    }
                    else
                    {
                        /* Non zero endpoint irp */
                        if(direction == USB_DATA_DIRECTION_DEVICE_TO_HOST)
                        {

                            /* Data IN stage of control transfer.
                             * Driver is waiting for an IRP from the client and has
                             * received it. Determine the transaction size. */
                            if(irp->nPendingBytes < endpointObj->maxPacketSize)
                            {
                                /* This is the last transaction in the transfer. */
                                byteCount = irp->nPendingBytes;
                            }
                            else
                            {
                                /* This is first or a continuing transaction in the
                                 * transfer and the transaction size must be
                                 * maxPacketSize */

                                byteCount = endpointObj->maxPacketSize;
                            }

                            fifoAddPtr = ENDPOINT_FIFO_ADDRESS(endpoint);

                            offset = irp->size - irp->nPendingBytes;

                            data = (uint8_t *)irp->data;

                            data = (uint8_t *)(data + offset);

                            __DMB();

                            for(count = 0; count < byteCount; count++)
                            {
                                *fifoAddPtr++ = *((uint8_t *)(data + count));
                            }

                            __DMB();

                            irp->nPendingBytes -= byteCount;

                            /* Clear the flag and enable the interrupt. The rest of
                             * the IRP should really get processed in the ISR.
                             * */
                            usbID->UDPHS_EPT[endpoint].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                            usbID->UDPHS_EPT[endpoint].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;

                            usbID->UDPHS_EPT[endpoint].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                        }
                        else
                        {
                            /* direction is Host to Device */
                            if((usbID->UDPHS_EPT[endpoint].UDPHS_EPTSTA & UDPHS_EPTSTA_RXRDY_TXKL_Msk) == UDPHS_EPTSTA_RXRDY_TXKL_Msk)
                            {

                                fifoAddPtr = ENDPOINT_FIFO_ADDRESS(endpoint);

                                byteCount = ((usbID->UDPHS_EPT[endpoint].UDPHS_EPTSTA & UDPHS_EPTSTA_BYTE_COUNT_Msk) >> UDPHS_EPTSTA_BYTE_COUNT_Pos);

                                data = (uint8_t *)irp->data;

                                data = (uint8_t *)&data[irp->nPendingBytes];

                                if((irp->nPendingBytes + byteCount) > irp->size)
                                {
                                    /* This is not acceptable as it may corrupt the ram location */
                                    byteCount = irp->size - irp->nPendingBytes;
                                }

                                __DMB();

                                for(count = 0; count < byteCount; count++)
                                {
                                    *((uint8_t *)(data + count)) = *fifoAddPtr++;
                                }

                                __DMB();

                                irp->nPendingBytes += byteCount;

                                if((irp->nPendingBytes < irp->size) && (byteCount >= endpointObj->maxPacketSize))
                                {

                                    usbID->UDPHS_EPT[endpoint].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL_Msk;

                                    usbID->UDPHS_EPT[endpoint].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RXRDY_TXKL_Msk;
                                }
                                else
                                {
                                    if(irp->nPendingBytes >= irp->size)
                                    {
                                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;
                                    }
                                    else
                                    {
                                        /* Short Packet */
                                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;
                                    }

                                    endpointObj->irpQueue = irp->next;

                                    irp->size = irp->nPendingBytes;

                                    if(irp->callback != NULL)
                                    {
                                        irp->callback((USB_DEVICE_IRP *)irp);
                                    }

                                    usbID->UDPHS_EPT[endpoint].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL_Msk;

                                    usbID->UDPHS_EPT[endpoint].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RXRDY_TXKL_Msk;
                                }
                            }
                        }/* End of non zero RX IRP submit */

                    }/* End of non zero IRP submit */
                }
                else
                {
                    /* This means we should surf the linked list to get to the last entry . */

                    USB_DEVICE_IRP_LOCAL * iterator;
                    iterator = endpointObj->irpQueue;
                    while (iterator->next != NULL)
                    {
                        iterator = iterator->next;
                    }
                    iterator->next = irp;
                    irp->previous = iterator;
                    irp->status = USB_DEVICE_IRP_STATUS_PENDING;
                }
            }

            if(hDriver->isInInterruptContext == false)
            {
                /* Release the mutex */
                if(OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID) == OSAL_RESULT_TRUE)
                {
                    if(interruptWasEnabled)
                    {
                        /* Enable the interrupt only if it was disabled */
                        SYS_INT_SourceEnable(hDriver->interruptSource);
                    }
                }
                else
                {
                    /* There was an error in releasing the mutex */
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex unlock failed in DRV_USB_UDPHS_DEVICE_IRPSubmit().");

                    retVal = USB_ERROR_OSAL_FUNCTION;
                }
            }
        }
    }
    return (retVal);

}/* end of DRV_USB_UDPHS_DEVICE_IRPSubmit() */

// *****************************************************************************

/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_IRPCancelAll
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_IRPCancelAll client
    interface function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_IRPCancelAll
    client interface function for USB device.
    Function checks the validity of the input arguments and on success cancels
    all the IRPs on the specific endpoint object queue.

  Remarks:
    See drv_usb_udphs.h for usage information.
 */

USB_ERROR DRV_USB_UDPHS_DEVICE_IRPCancelAll
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{

    DRV_USB_UDPHS_OBJ * hDriver;              /* USB driver object pointer */
    uint8_t endpoint;                       /* Endpoint Number */
    bool interruptWasEnabled = false;       /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;      /* Return value */

    /* Endpoint object pointer */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj;


    /* Extract the Endpoint number */
    endpoint = endpointAndDirection & DRV_USB_UDPHS_ENDPOINT_NUMBER_MASK;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_IRPCancelAll().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else if(endpoint >= DRV_USB_UDPHS_MAX_ENDPOINT_NUMBER)
    {
        /* The endpoint number must be a valid endpoint */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Unsupported endpoint in DRV_USB_UDPHS_DEVICE_IRPCancelAll().");

        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else
    {   /* Handle and Endpoint formation are valid, Cancel IRPs */
        hDriver = (DRV_USB_UDPHS_OBJ *) handle;

        /* Get the endpoint object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                /* Disable  the USB Interrupt as this is not called inside ISR */
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex lock failed in DRV_USB_UDPHS_DEVICE_IRPCancelAll()");

                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }

        if(retVal == USB_ERROR_NONE)
        {
            /* Flush the endpoint */
            _DRV_USB_UDPHS_DEVICE_IRPQueueFlush( endpointObj, USB_DEVICE_IRP_STATUS_ABORTED);

            /* Restore the interrupt enable status if this was modified. */
            if(hDriver->isInInterruptContext == false)
            {
                /* Release the mutex */
                if(OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID) == OSAL_RESULT_TRUE)
                {
                    if(interruptWasEnabled)
                    {
                        /* Enable the interrupt only if it was disabled */
                        SYS_INT_SourceEnable(hDriver->interruptSource);
                    }
                }
                else
                {
                    /* There was an error in releasing the mutex */
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex unlock failed in DRV_USB_UDPHS_DEVICE_IRPCancelAll().");

                    retVal = USB_ERROR_OSAL_FUNCTION;
                }
            }
        }
    }

    return (retVal);

}/* end of DRV_USB_UDPHS_DEVICE_IRPCancelAll() */

// *****************************************************************************

/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_IRPCancel
    (
        DRV_HANDLE client,
        USB_DEVICE_IRP * irp
    )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_IRPCancel client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_IRPCancel
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
    See drv_usb_udphs.h for usage information.
 */

USB_ERROR DRV_USB_UDPHS_DEVICE_IRPCancel
(
    DRV_HANDLE handle,
    USB_DEVICE_IRP * irp
)

{

    DRV_USB_UDPHS_OBJ * hDriver;              /* USB driver object pointer */
    USB_DEVICE_IRP_LOCAL * irpToCancel;     /* Pointer to irp data structure */
    bool interruptWasEnabled = false;       /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;      /* Return value */


    /* Check if the handle is valid */
    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Invalid Driver Handle in DRV_USB_UDPHS_DEVICE_IRPCancel().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else if(irp == NULL)
    {
        /* IRP is NULL, send appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: IRP is invalid in DRV_USB_UDPHS_DEVICE_IRPCancel().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {

        irpToCancel = (USB_DEVICE_IRP_LOCAL *) irp;
        hDriver = ((DRV_USB_UDPHS_OBJ *) handle);

        if(hDriver->isInInterruptContext == false)
        {
            if(OSAL_MUTEX_Lock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
            {
                /* Disable  the USB Interrupt as this is not called inside ISR */
                interruptWasEnabled = SYS_INT_SourceDisable(hDriver->interruptSource);
            }
            else
            {
                /* There was an error in getting the mutex */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex lock failed in DRV_USB_UDPHS_DEVICE_IRPCancel()");

                retVal = USB_ERROR_OSAL_FUNCTION;
            }
        }

        if(retVal == USB_ERROR_NONE)
        {

            if(irpToCancel->status <= USB_DEVICE_IRP_STATUS_COMPLETED_SHORT)
            {
                /* This IRP has either completed or has been aborted.*/
                retVal = USB_ERROR_PARAMETER_INVALID;
            }
            else
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
                        Can be removed from the endpoint object queue safely.*/
                    irpToCancel->previous->next = irpToCancel->next;

                    if(irpToCancel->next != NULL)
                    {
                        /* If this is not the last IRP in the queue then update
                            the previous link connection for the next IRP */
                        irpToCancel->next->previous = irpToCancel->previous;
                    }

                    irpToCancel->previous = NULL;
                    irpToCancel->next = NULL;

                    if(irpToCancel->callback != NULL)
                    {
                        irpToCancel->callback((USB_DEVICE_IRP *) irpToCancel);
                    }
                }
            }

            /* Restore the interrupt enable status if this was modified. */
            if(hDriver->isInInterruptContext == false)
            {
                /* Release the mutex */
                if(OSAL_MUTEX_Unlock((OSAL_MUTEX_HANDLE_TYPE *)&hDriver->mutexID) == OSAL_RESULT_TRUE)
                {
                    if(interruptWasEnabled)
                    {
                        /* Enable the interrupt only if it was disabled */
                        SYS_INT_SourceEnable(hDriver->interruptSource);
                    }
                }
                else
                {
                    /* There was an error in releasing the mutex */
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Mutex unlock failed in DRV_USB_UDPHS_DEVICE_IRPCancel().");

                    retVal = USB_ERROR_OSAL_FUNCTION;
                }
            }
        }
    }


    return (retVal);

}/* End of DRV_USB_UDPHS_DEVICE_IRPCancel() */

// *****************************************************************************
/* Function:
      void _DRV_USB_UDPHS_DEVICE_Tasks_ISR(DRV_USB_UDPHS_OBJ * hDriver)

  Summary:
    Dynamic implementation of _DRV_USB_UDPHS_DEVICE_Tasks_ISR ISR handler
    function.

  Description:
    This is the dynamic implementation of _DRV_USB_UDPHS_DEVICE_Tasks_ISR ISR
    handler function for USB device.
    Function will get called automatically due to USB interrupts in interrupt
    mode.
    In polling mode this function will be routinely called from USB driver
    DRV_USB_UDPHS_Tasks() function.
    This function performs necessary action based on the interrupt and clears
    the interrupt after that. The USB device layer callback is called with the
    interrupt event details, if callback function is registered.

  Remarks:
    This is a local function and should not be called directly by the
    application.
 */

void _DRV_USB_UDPHS_DEVICE_Tasks_ISR
(
	DRV_USB_UDPHS_OBJ * hDriver
)

{

    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * endpointObj;
    udphs_registers_t * usbID;
    USB_DEVICE_IRP_LOCAL * irp;
    USB_SETUP_PACKET * setupPkt;
    uint16_t endpoint0DataStageSize;
    unsigned int endpoint0DataStageDirection;
    uint8_t eptIndex;
    volatile uint8_t * fifoAddPtr;                  /* pointer variable for local use */
    uint16_t count;
    uint8_t * data;
    uint32_t byteCount = 0;


    if(false == hDriver->isOpened)
    {
        /* We need a valid client */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Driver does not have a client in _DRV_USB_UDPHS_DEVICE_Tasks_ISR().");
    }
    else if(NULL == hDriver->pEventCallBack)
    {
        /* We need a valid event handler */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB UDPHS Driver: Driver needs a event handler in _DRV_USB_UDPHS_DEVICE_Tasks_ISR().");
    }
    else
    {
    	usbID = hDriver->usbID;

        /* Check for SOF Interrupt Enable and SOF Interrupt Flag */
        if((UDPHS_IEN_INT_SOF_Msk == (UDPHS_IEN_INT_SOF_Msk & usbID->UDPHS_IEN)) && (UDPHS_INTSTA_INT_SOF_Msk == (UDPHS_INTSTA_INT_SOF_Msk & usbID->UDPHS_INTSTA)))
        {
            /* This means that there was a SOF. */
            usbID->UDPHS_CLRINT = UDPHS_CLRINT_INT_SOF_Msk;
            hDriver->pEventCallBack(hDriver->hClientArg, (DRV_USB_EVENT)DRV_USB_UDPHS_EVENT_SOF_DETECT, NULL);
        }

        /* Check for MSOF Interrupt Enable and MSOF Interrupt Flag */
        if((UDPHS_IEN_MICRO_SOF_Msk == (UDPHS_IEN_MICRO_SOF_Msk & usbID->UDPHS_IEN)) && (UDPHS_INTSTA_MICRO_SOF_Msk == (UDPHS_INTSTA_MICRO_SOF_Msk & usbID->UDPHS_INTSTA)))
        {
            /* Just acknowledge and Do nothing */
            usbID->UDPHS_CLRINT = UDPHS_CLRINT_MICRO_SOF_Msk;

        }

        /* Check for Suspend Interrupt Enable and Suspend Interrupt Flag */
        if((UDPHS_IEN_DET_SUSPD_Msk == (UDPHS_IEN_DET_SUSPD_Msk & usbID->UDPHS_IEN)) && (UDPHS_INTSTA_DET_SUSPD_Msk == (UDPHS_INTSTA_DET_SUSPD_Msk & usbID->UDPHS_INTSTA)))
        {
            /* This means that the bus was SUSPENDED. */
            hDriver->pEventCallBack(hDriver->hClientArg, (DRV_USB_EVENT)DRV_USB_UDPHS_EVENT_IDLE_DETECT, NULL);

            /* Disable Suspend Interrupt */
			usbID->UDPHS_IEN &= ~UDPHS_IEN_DET_SUSPD_Msk;

            /* Enable Wakeup Interrupt */
			usbID->UDPHS_IEN |= UDPHS_IEN_WAKE_UP_Msk |	UDPHS_IEN_ENDOFRSM_Msk;

            /* Acknowledge the suspend interrupt */
			usbID->UDPHS_CLRINT = UDPHS_CLRINT_DET_SUSPD_Msk | UDPHS_CLRINT_WAKE_UP_Msk;

        }

        /* Check for Wakeup Interrupt Enable and Wakeup Interrupt Flag, End Of Resume Interrupt Enable and End Of Resume Interrupt Flag */
        if(((UDPHS_IEN_WAKE_UP_Msk == (UDPHS_IEN_WAKE_UP_Msk & usbID->UDPHS_IEN)) && (UDPHS_INTSTA_WAKE_UP_Msk == (UDPHS_INTSTA_WAKE_UP_Msk & usbID->UDPHS_INTSTA))) ||
           ((UDPHS_IEN_ENDOFRSM_Msk == (UDPHS_IEN_ENDOFRSM_Msk & usbID->UDPHS_IEN)) && (UDPHS_INTSTA_ENDOFRSM_Msk == (UDPHS_INTSTA_ENDOFRSM_Msk & usbID->UDPHS_INTSTA))))
        {
            /* This means End of Resume signal is received. Send this to device layer. */
            hDriver->pEventCallBack(hDriver->hClientArg, (DRV_USB_EVENT)DRV_USB_UDPHS_EVENT_RESUME_DETECT, NULL);

            /* Acknowledge the interrupt */
			usbID->UDPHS_CLRINT = UDPHS_CLRINT_WAKE_UP_Msk | UDPHS_CLRINT_ENDOFRSM_Msk | UDPHS_CLRINT_DET_SUSPD_Msk;

            /* Disable Wakeup Interrupt */
			usbID->UDPHS_IEN &= ~UDPHS_IEN_WAKE_UP_Msk;

            /* Enable Suspend Interrupt */
			usbID->UDPHS_IEN |= UDPHS_IEN_ENDOFRSM_Msk | UDPHS_IEN_DET_SUSPD_Msk;

        }

        /* Check for Upstream Reset Interrupt Enable and Upstream Reset Interrupt Flag */
        if((UDPHS_IEN_UPSTR_RES_Msk == (UDPHS_IEN_UPSTR_RES_Msk & usbID->UDPHS_IEN)) && (UDPHS_INTSTA_UPSTR_RES_Msk == (UDPHS_INTSTA_UPSTR_RES_Msk & usbID->UDPHS_INTSTA)))
        {
			/* Acknowledge interrupt */
			usbID->UDPHS_CLRINT = UDPHS_CLRINT_UPSTR_RES_Msk;

		}

        /* Check for End Of Reset Interrupt Enable and End Of Reset Interrupt Flag */
        if((UDPHS_IEN_ENDRESET_Msk == (UDPHS_IEN_ENDRESET_Msk & usbID->UDPHS_IEN)) && (UDPHS_INTSTA_ENDRESET_Msk == (UDPHS_INTSTA_ENDRESET_Msk & usbID->UDPHS_INTSTA)))
        {
            /* This means that RESET signaling was detected
             * on the bus. This means the packet that we should
             * get on the bus for EP0 should be a setup packet */

            hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

            if((usbID->UDPHS_INTSTA & UDPHS_INTSTA_SPEED_Msk) == UDPHS_INTSTA_SPEED_Msk)
            {
                hDriver->deviceSpeed = USB_SPEED_HIGH;
            }
            else
            {
                hDriver->deviceSpeed = USB_SPEED_FULL;
            }

            hDriver->pEventCallBack(hDriver->hClientArg, (DRV_USB_EVENT)DRV_USB_UDPHS_EVENT_RESET_DETECT, NULL);

            /* Acknowledge the End of Reset interrupt */
			usbID->UDPHS_CLRINT = UDPHS_CLRINT_ENDRESET_Msk;

            /* Acknowledge the Wakeup and Suspend interrupt */
			usbID->UDPHS_CLRINT = UDPHS_CLRINT_WAKE_UP_Msk | UDPHS_CLRINT_DET_SUSPD_Msk;

            /* Enable Suspend Interrupt */
			usbID->UDPHS_IEN |= UDPHS_IEN_DET_SUSPD_Msk;
			usbID->UDPHS_IEN |= UDPHS_IEN_INT_SOF_Msk;
        }


        if(((usbID->UDPHS_EPT[0].UDPHS_EPTCTL & UDPHS_EPTCTL_RX_SETUP_Msk) == UDPHS_EPTCTL_RX_SETUP_Msk) &&
           ((usbID->UDPHS_EPT[0].UDPHS_EPTSTA & UDPHS_EPTSTA_RX_SETUP_Msk) == UDPHS_EPTSTA_RX_SETUP_Msk))
        {

            /* Get the pointer to the endpoint 0 object */
            endpointObj = hDriver->deviceEndpointObj[0];

            /* This means we have received a setup packet. Let's clear the
             * stall condition on the endpoint. */

            usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_FRCESTALL_Msk;

            endpointObj->endpointState &= ~DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_STALLED;
            (endpointObj + 1)->endpointState &= ~DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_STALLED;

            if(endpointObj->irpQueue != NULL)
            {

                irp = endpointObj->irpQueue;

                fifoAddPtr = ENDPOINT_FIFO_ADDRESS(0);

                data = (uint8_t *)irp->data;

                __DMB();

                for(count = 0; count < 8; count++)
                {
                    *((uint8_t *)(data + count)) = *fifoAddPtr++;
                }

                usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RX_SETUP_Msk;
                
                setupPkt = (USB_SETUP_PACKET *)irp->data;
                                        
                endpoint0DataStageSize = setupPkt->W_Length.Val;

                endpoint0DataStageDirection = (uint16_t)((setupPkt->bmRequestType & DRV_USB_UDPHS_ENDPOINT_DIRECTION_MASK) != 0);

                /* Indicate that this is a setup IRP */
                irp->status = USB_DEVICE_IRP_STATUS_SETUP;
                irp->size = 8;

                if(endpoint0DataStageSize == 0)
                {
                    hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT;
                }
                else if(endpoint0DataStageDirection == USB_DATA_DIRECTION_DEVICE_TO_HOST)
                {
                    hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT;
                }
                else
                {
                    hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST;
                }

                endpointObj->irpQueue = irp->next;

                if(irp->callback != NULL)
                {
                    irp->callback((USB_DEVICE_IRP *)irp);
                }

                usbID->UDPHS_EPT[0].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;

                usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RX_SETUP_Msk;

                usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RXRDY_TXKL_Msk;
            }
            else
            {

                /* Received the Setup packet, but we don't have an IRP.
                 * Change endpoint0State to waiting for setup irp */
                hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_SETUP_IRP_FROM_CLIENT;

                /* Disable the RX_SETUP Interrupt until the received
                 * packet is serviced in DRV_USB_UDPHS_DEVICE_IRPSubmit */
                usbID->UDPHS_EPT[0].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_RX_SETUP_Msk;
            }
        }

        if(((usbID->UDPHS_EPT[0].UDPHS_EPTCTL & UDPHS_EPTCTL_TX_COMPLT_Msk) == UDPHS_EPTCTL_TX_COMPLT_Msk) &&
           ((usbID->UDPHS_EPT[0].UDPHS_EPTSTA & UDPHS_EPTSTA_TX_COMPLT_Msk) == UDPHS_EPTSTA_TX_COMPLT_Msk))
        {

            /* Get the pointer to the endpoint 0 object */
            endpointObj = hDriver->deviceEndpointObj[0];
            endpointObj++;

            /* This means a TX IN transaction has been acked by the host.
             * This could happen in Control Read Data Stage or Control Write Status Stage. */

            if(endpointObj->irpQueue != NULL)
            {

                irp = endpointObj->irpQueue;

                if(hDriver->endpoint0State == DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE)
                {

                    hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

                    irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                    endpointObj->irpQueue = irp->next;

                    irp->size = 0;

                    if(irp->callback != NULL)
                    {
                        irp->callback((USB_DEVICE_IRP *)irp);
                    }

                    usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                    usbID->UDPHS_EPT[0].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_TX_COMPLT_Msk;

                    usbID->UDPHS_EPT[0].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                    usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RXRDY_TXKL_Msk;

                }
                else
                {

                    /* TX Data stage of control transfer in progress.
                     * Driver has sent last packet. Send another packet of data
                     * or complete the transaction. */

                    if(irp->nPendingBytes != 0)
                    {
                        if(irp->nPendingBytes < endpointObj->maxPacketSize)
                        {
                            /* This is the last transaction in the transfer. */
                            byteCount = irp->nPendingBytes;
                        }
                        else
                        {
                            /* This is first or a continuing transaction in the
                             * transfer and the transaction size must be
                             * maxPacketSize */

                            byteCount = endpointObj->maxPacketSize;
                        }

                        fifoAddPtr = ENDPOINT_FIFO_ADDRESS(0);

                        data = (uint8_t *)irp->data;

                        data = (uint8_t *)&data[irp->size - irp->nPendingBytes];

                        __DMB();

                        for(count = 0; count < byteCount; count++)
                        {
                            *fifoAddPtr++ = *((uint8_t *)(data + count));
                        }

                        __DMB();

                        irp->nPendingBytes -= byteCount;

                        /* Clear the flag and enable the interrupt. The rest of
                         * the IRP should really get processed in the ISR.
                         * */

                        usbID->UDPHS_EPT[0].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                        usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                        usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;
                    }
                    else if((irp->flags & USB_DEVICE_IRP_FLAG_SEND_ZLP) == USB_DEVICE_IRP_FLAG_SEND_ZLP)
                    {
                        /* All TX data has been sent. Check if ZLP is to be sent,
                        * else mark the IRP as completed. */

                        /* Need to send ZLP. Clear the size of buffer and
                         * ready the buffer to send data */

                        irp->flags &= ~USB_DEVICE_IRP_FLAG_SEND_ZLP;

                        usbID->UDPHS_EPT[0].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                        usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                        usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;
                    }
                    else if((irp->next != NULL) && ((irp->flags & USB_DEVICE_IRP_FLAG_DATA_PENDING) == USB_DEVICE_IRP_FLAG_DATA_PENDING))
                    {
                        irp->flags &= ~USB_DEVICE_IRP_FLAG_DATA_PENDING;

                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                        irp = irp->next;

                        endpointObj->irpQueue = irp;

                        irp->status = USB_DEVICE_IRP_STATUS_IN_PROGRESS;

                        if(irp->nPendingBytes < endpointObj->maxPacketSize)
                        {
                            /* This is the last transaction in the transfer. */
                            byteCount = irp->nPendingBytes;
                        }
                        else
                        {
                            /* This is first or a continuing transaction in the
                             * transfer and the transaction size must be
                             * maxPacketSize */

                            byteCount = endpointObj->maxPacketSize;
                        }

                        fifoAddPtr = ENDPOINT_FIFO_ADDRESS(0);

                        data = (uint8_t *)irp->data;

                        __DMB();

                        for(count = 0; count < byteCount; count++)
                        {
                            *fifoAddPtr++ = *((uint8_t *)(data + count));
                        }

                        __DMB();

                        irp->nPendingBytes -= byteCount;

                        /* Clear the flag and enable the interrupt. The rest of
                         * the IRP should really get processed in the ISR.
                         * */

                        usbID->UDPHS_EPT[0].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                        usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                        usbID->UDPHS_EPT[0].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;
                    }
                    else
                    {

                        hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE;

                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                        endpointObj->irpQueue = irp->next;

                        if(irp->callback != NULL)
                        {
                            irp->callback((USB_DEVICE_IRP *)irp);
                        }

                        usbID->UDPHS_EPT[0].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                        usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                    }
                }
            }
        }

        if(((usbID->UDPHS_EPT[0].UDPHS_EPTCTL & UDPHS_EPTCTL_RXRDY_TXKL_Msk) == UDPHS_EPTCTL_RXRDY_TXKL_Msk) &&
           ((usbID->UDPHS_EPT[0].UDPHS_EPTSTA & UDPHS_EPTSTA_RXRDY_TXKL_Msk) == UDPHS_EPTSTA_RXRDY_TXKL_Msk))
        {

            /* Get the pointer to the endpoint 0 object */
            endpointObj = hDriver->deviceEndpointObj[0];

            /* This means a RX OUT transaction is complete and device has
             * recevied a packet from host.
             * This could happen in Control Write Data Stage or Control Read Status Stage. */

            if(endpointObj->irpQueue != NULL)
            {
                irp = endpointObj->irpQueue;

                if(hDriver->endpoint0State == DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE)
                {
                    hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

                    usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL_Msk;

                    irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                    irp->size = 0;

                    endpointObj->irpQueue = irp->next;

                    if(irp->callback != NULL)
                    {
                        irp->callback((USB_DEVICE_IRP *)irp);
                    }
                }
                else
                {

                    fifoAddPtr = ENDPOINT_FIFO_ADDRESS(0);

                    byteCount = ((usbID->UDPHS_EPT[0].UDPHS_EPTSTA & UDPHS_EPTSTA_BYTE_COUNT_Msk) >> UDPHS_EPTSTA_BYTE_COUNT_Pos);

                    data = (uint8_t *)irp->data;

                    data = (uint8_t *)&data[irp->nPendingBytes];

                    if((irp->nPendingBytes + byteCount) > irp->size)
                    {
                        /* This is not acceptable as it may corrupt the ram location */
                        byteCount = irp->size - irp->nPendingBytes;
                    }

                    __DMB();

                    for(count = 0; count < byteCount; count++)
                    {
                        *((uint8_t *)(data + count)) = *fifoAddPtr++;
                    }

                    __DMB();

                    irp->nPendingBytes += byteCount;

                    if((irp->nPendingBytes < irp->size) && (byteCount >= endpointObj->maxPacketSize))
                    {
                        usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL_Msk;
                    }
                    else
                    {
                        if(irp->nPendingBytes >= irp->size)
                        {
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;
                        }
                        else
                        {
                            /* Short Packet */
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;
                        }

                        hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT;

                        usbID->UDPHS_EPT[0].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL_Msk;

                        endpointObj->irpQueue = irp->next;

                        irp->size = irp->nPendingBytes;

                        if(irp->callback != NULL)
                        {
                            irp->callback((USB_DEVICE_IRP *)irp);
                        }
                    }
                }
            }
            else if(hDriver->endpoint0State == DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE)
            {
                hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_IRP_FROM_CLIENT;

                usbID->UDPHS_EPT[0].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_RXRDY_TXKL_Msk;
            }
            else
            {
                hDriver->endpoint0State = DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_DATA_IRP_FROM_CLIENT;

                usbID->UDPHS_EPT[0].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_RXRDY_TXKL_Msk;
            }
        }
        for(eptIndex = 1; eptIndex < DRV_USB_UDPHS_ENDPOINTS_NUMBER; eptIndex++)
        {
            /* This means this is non-EP0 interrupt. Read the endpoint status
             * register. */

            if(((usbID->UDPHS_IEN & (UDPHS_IEN_EPT_0_Msk << eptIndex)) == 0) ||
               ((usbID->UDPHS_INTSTA & (UDPHS_INTSTA_EPT_0_Msk << eptIndex)) == 0))
            {
                continue;
            }

            if(((usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCTL & UDPHS_EPTCTL_RXRDY_TXKL_Msk) == UDPHS_EPTCTL_RXRDY_TXKL_Msk) &&
               ((usbID->UDPHS_EPT[eptIndex].UDPHS_EPTSTA & UDPHS_EPTSTA_RXRDY_TXKL_Msk) == UDPHS_EPTSTA_RXRDY_TXKL_Msk))
            {

                /* Get the pointer to the endpoint 0 object */
                endpointObj = hDriver->deviceEndpointObj[eptIndex];

                if(endpointObj->irpQueue == NULL)
                {
                    usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_RXRDY_TXKL_Msk;
                }
                else
                {
                    irp = endpointObj->irpQueue;

                    fifoAddPtr = ENDPOINT_FIFO_ADDRESS(eptIndex);

                    byteCount = ((usbID->UDPHS_EPT[eptIndex].UDPHS_EPTSTA & UDPHS_EPTSTA_BYTE_COUNT_Msk) >> UDPHS_EPTSTA_BYTE_COUNT_Pos);

                    data = (uint8_t *)irp->data;

                    data = (uint8_t *)&data[irp->nPendingBytes];

                    if((irp->nPendingBytes + byteCount) > irp->size)
                    {
                        /* This is not acceptable as it may corrupt the ram location */
                        byteCount = irp->size - irp->nPendingBytes;
                    }

                    __DMB();

                    for(count = 0; count < byteCount; count++)
                    {
                        *((uint8_t *)(data + count)) = *fifoAddPtr++;
                    }

                    __DMB();

                    irp->nPendingBytes += byteCount;

                    if((irp->nPendingBytes < irp->size) && (byteCount >= endpointObj->maxPacketSize))
                    {
                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL_Msk;

                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RXRDY_TXKL_Msk;
                    }
                    else
                    {
                        if(irp->nPendingBytes >= irp->size)
                        {
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;
                        }
                        else
                        {
                            /* Short Packet */
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;
                        }

                        endpointObj->irpQueue = irp->next;

                        irp->size = irp->nPendingBytes;

                        if(irp->callback != NULL)
                        {
                            irp->callback((USB_DEVICE_IRP *)irp);
                        }

                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_RXRDY_TXKL_Msk;

                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_RXRDY_TXKL_Msk;
                    }
                }
            }

            if(((usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCTL & UDPHS_EPTCTL_TX_COMPLT_Msk) == UDPHS_EPTCTL_TX_COMPLT_Msk) &&
               ((usbID->UDPHS_EPT[eptIndex].UDPHS_EPTSTA & UDPHS_EPTSTA_TX_COMPLT_Msk) == UDPHS_EPTSTA_TX_COMPLT_Msk))
            {

                /* Get the pointer to the endpoint 0 object */
                endpointObj = hDriver->deviceEndpointObj[eptIndex];

                if(endpointObj->irpQueue != NULL)
                {
                    irp = endpointObj->irpQueue;

                    if(irp->nPendingBytes != 0)
                    {
                        if(irp->nPendingBytes < endpointObj->maxPacketSize)
                        {
                            /* This is the last transaction in the transfer. */
                            byteCount = irp->nPendingBytes;
                        }
                        else
                        {
                            /* This is first or a continuing transaction in the
                             * transfer and the transaction size must be
                             * maxPacketSize */

                            byteCount = endpointObj->maxPacketSize;
                        }

                        fifoAddPtr = ENDPOINT_FIFO_ADDRESS(eptIndex);

                        data = (uint8_t *)irp->data;

                        data = (uint8_t *)&data[irp->size - irp->nPendingBytes];

                        __DMB();

                        for(count = 0; count < byteCount; count++)
                        {
                            *fifoAddPtr++ = *((uint8_t *)(data + count));
                        }

                        __DMB();

                        irp->nPendingBytes -= byteCount;

                        /* Clear the flag and enable the interrupt. The rest of
                         * the IRP should really get processed in the ISR.
                         * */

                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;
                    }
                    else if((irp->flags & USB_DEVICE_IRP_FLAG_SEND_ZLP) == USB_DEVICE_IRP_FLAG_SEND_ZLP)
                    {

                        irp->flags &= ~USB_DEVICE_IRP_FLAG_SEND_ZLP;

                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                        usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;
                    }
                    else
                    {

                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                        endpointObj->irpQueue = irp->next;

                        if(irp->callback != NULL)
                        {
                            irp->callback((USB_DEVICE_IRP *)irp);
                        }

                        if(irp->next != NULL)
                        {

                            irp->flags &= ~USB_DEVICE_IRP_FLAG_DATA_PENDING;

                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                            irp = irp->next;

                            endpointObj->irpQueue = irp;

                            irp->status = USB_DEVICE_IRP_STATUS_IN_PROGRESS;

                            if(irp->nPendingBytes < endpointObj->maxPacketSize)
                            {
                                /* This is the last transaction in the transfer. */
                                byteCount = irp->nPendingBytes;
                            }
                            else
                            {
                                /* This is first or a continuing transaction in the
                                 * transfer and the transaction size must be
                                 * maxPacketSize */

                                byteCount = endpointObj->maxPacketSize;
                            }

                            fifoAddPtr = ENDPOINT_FIFO_ADDRESS(eptIndex);

                            data = (uint8_t *)irp->data;

                            __DMB();

                            for(count = 0; count < byteCount; count++)
                            {
                                *fifoAddPtr++ = *((uint8_t *)(data + count));
                            }

                            __DMB();

                            irp->nPendingBytes -= byteCount;

                            /* Clear the flag and enable the interrupt. The rest of
                             * the IRP should really get processed in the ISR.
                             * */

                            usbID->UDPHS_EPT[eptIndex].UDPHS_EPTSETSTA = UDPHS_EPTSETSTA_TXRDY_Msk;

                            usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCLRSTA = UDPHS_EPTCLRSTA_TX_COMPLT_Msk;

                            usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCTLENB = UDPHS_EPTCTLENB_TX_COMPLT_Msk;
                        }
                        else
                        {
                            usbID->UDPHS_EPT[eptIndex].UDPHS_EPTCTLDIS = UDPHS_EPTCTLDIS_TX_COMPLT_Msk;

                        }
                    }
                }

            }
        }
    }
}/* end of _DRV_USB_UDPHS_DEVICE_Tasks_ISR() */

// *****************************************************************************

/* Function:
      USB_ERROR DRV_USB_UDPHS_DEVICE_TestModeEnter
      (
          DRV_HANDLE handle,
          USB_TEST_MODE_SELECTORS testMode
      )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_TestModeEnter client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_TestModeEnter client
    interface function for USB device. Function set the test mode requested.
    Only 1 test mode can remain set at any given point of time.

  Remarks:
    See drv_usb.h for usage information.
 */

USB_ERROR DRV_USB_UDPHS_DEVICE_TestModeEnter
(
    DRV_HANDLE handle,
    USB_TEST_MODE_SELECTORS testMode
)

{
    USB_ERROR retVal = USB_ERROR_NONE;

    /* Not yet Implemented */

    return (retVal);

}/* end of DRV_USB_UDPHS_DEVICE_TestModeEnter() */

// *****************************************************************************

/* Function:
      USB_ERROR DRV_USB_UDPHS_DEVICE_TestModeExit
      (
          DRV_HANDLE handle,
          USB_TEST_MODE_SELECTORS testMode
      )

  Summary:
    Dynamic implementation of DRV_USB_UDPHS_DEVICE_TestModeExit client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USB_UDPHS_DEVICE_TestModeExit client
    interface function for USB device. Function clears the test mode set.

  Remarks:
    See drv_usb.h for usage information.
 */

USB_ERROR DRV_USB_UDPHS_DEVICE_TestModeExit
(
    DRV_HANDLE handle,
    USB_TEST_MODE_SELECTORS testMode
)

{
    USB_ERROR retVal = USB_ERROR_NONE;

    /* Not yet Implemented */

    return (retVal);

}/* end of DRV_USB_UDPHS_DEVICE_TestModeExit() */

