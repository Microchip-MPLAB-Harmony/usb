/*******************************************************************************
 USB Printer Class Function Driver

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_printer.c

  Summary:
    USB Printer class function driver.

  Description:
    USB Printer class function driver.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "usb/usb_device_printer.h"
#include "usb/src/usb_device_printer_local.h"
#include "usb/src/usb_external_dependencies.h"


// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Printer Device function driver structure

  Summary:
    Defines the function driver structure required for the device layer.

  Description:
    This data type defines the function driver structure required for the
    device layer.

  Remarks:
    This structure is private to the USB stack.
*/

const USB_DEVICE_FUNCTION_DRIVER printerFunctionDriver =
{

    /* Printer init function */
    .initializeByDescriptor         = F_USB_DEVICE_PRINTER_Initialization ,

    /* Printer de-init function */
    .deInitialize                   = F_USB_DEVICE_PRINTER_Deinitialization ,

    /* EP0 activity callback */
    .controlTransferNotification    = F_USB_DEVICE_PRINTER_ControlTransferHandler,

    /* Printer tasks function */
    .tasks                          = NULL,

    /* Printer Global Initialize */
    .globalInitialize = F_USB_DEVICE_PRINTER_GlobalInitialize
};

// *****************************************************************************
/* Printer Device IRPs

  Summary:
    Array of Printer Device IRP. 

  Description:
    Array of Printer Device IRP. This array of IRP will be shared by read, write and
    notification data requests.

  Remarks:
    This array is private to the USB stack.
*/

static USB_DEVICE_IRP gUSBDevicePrinterIRP[USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED];

/* Create a variable for holding Printer IRP mutex Handle and status */
static USB_DEVICE_PRINTER_COMMON_DATA_OBJ gUSBDevicePrinterCommonDataObj;
 

// *****************************************************************************
/* Printer Instance structure

  Summary:
    Defines the Printer instance(s).

  Description:
    This data type defines the Printer instance(s). The number of instances is
    defined by the application using USB_DEVICE_PRINTER_INSTANCES_NUMBER.

  Remarks:
    This structure is private to the PRINTER.
*/

static USB_DEVICE_PRINTER_INSTANCE gUSBDevicePRINTERInstance[USB_DEVICE_PRINTER_INSTANCES_NUMBER];

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************
// ******************************************************************************
/* MISRA C-2012 Rule 10.4 False Positive:7 Deviation record ID -  H3_USB_MISRAC_2012_R_10_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block fp:7 "MISRA C-2012 Rule 10.4" "H3_USB_MISRAC_2012_R_10_4_DR_1" 
</#if>
/* Function:
    void F_USB_DEVICE_PRINTER_GlobalInitialize ( void )

  Summary:
    This function initializes resources required for printer 
    function driver instance.

  Description:
    This function initializes resources of Printer function driver instance.
    This function is called by the USB Device layer during Initalization.

  Remarks:
    This is local function and should not be called directly by the application.
*/
void F_USB_DEVICE_PRINTER_GlobalInitialize (void)
{
    OSAL_RESULT osal_err;
    
    /* Create Mutex for Printer IRP objects if not created already */
    if (gUSBDevicePrinterCommonDataObj.isMutexPrinterIrpInitialized == false)
    {
        /* This means that mutex where not created. Create them. */
        osal_err = OSAL_MUTEX_Create(&gUSBDevicePrinterCommonDataObj.mutexPrinterIRP);

        if(osal_err != OSAL_RESULT_TRUE)
        {
            /*do not proceed lock was not created, let user know about error*/
            return;
        }

         /* Set this flag so that global mutex get allocated only once */
         gUSBDevicePrinterCommonDataObj.isMutexPrinterIrpInitialized = true;
    }
}

// ******************************************************************************
/* MISRA C-2012 Rule 11.3 deviate:2, and 11.8 deviate:1. Deviation record ID -  
    H3_USB_MISRAC_2012_R_11_3_DR_1, H3_USB_MISRAC_2012_R_11_8_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance block \
(deviate:2 "MISRA C-2012 Rule 11.3" "H3_USB_MISRAC_2012_R_11_3_DR_1" )\
(deviate:1 "MISRA C-2012 Rule 11.8" "H3_USB_MISRAC_2012_R_11_8_DR_1" )
</#if>
/* Function:
    void F_USB_DEVICE_PRINTER_Initialization 

  Summary:
    USB Device printer function called by the device layer during Set Configuration
    processing.
  
  Description:
    USB Device printer function called by the device layer during Set Configuration
    processing.

  Remarks:
    This is local function and should not be called directly by the application.
*/
void F_USB_DEVICE_PRINTER_Initialization 
( 
    SYS_MODULE_INDEX iPRN ,
    DRV_HANDLE deviceHandle ,
    void* funcDriverInitData  ,
    uint8_t infNum ,
    uint8_t altSetting ,
    uint8_t descType ,
    uint8_t * pDesc 
)
{
    /* This function is called by the Device Layer when it comes across an
     * interface descriptor or an endpoint belonging to a Printer interface */
    USB_DEVICE_PRINTER_INSTANCE * prnInstance;
    USB_ENDPOINT_DESCRIPTOR *pEPDesc;
    USB_INTERFACE_DESCRIPTOR *pInfDesc;
    USB_DEVICE_PRINTER_INIT *prnInitializationData;
    uint32_t count; 

    /* Check the validity of the function driver index */
    if (iPRN >= (uint16_t)USB_DEVICE_PRINTER_INSTANCES_NUMBER)
    {
        /* Assert on invalid Printer index */
        SYS_DEBUG(0, "USB Device Printer: Invalid index");
        return;
    }

    prnInstance = &gUSBDevicePRINTERInstance[iPRN];
    prnInitializationData = (USB_DEVICE_PRINTER_INIT *)funcDriverInitData ;
    prnInstance->queueSizeWrite = prnInitializationData->queueSizeWrite;
    prnInstance->queueSizeRead = prnInitializationData->queueSizeRead;
    prnInstance->currentQSizeWrite = 0;
    prnInstance->currentQSizeRead = 0;   

    switch(descType)
    {
        case USB_DESCRIPTOR_ENDPOINT:
            {    
                /* Initialize the endpoint */
                pEPDesc = ( USB_ENDPOINT_DESCRIPTOR* ) pDesc;

                /* Remember the USB Device Layer handle */
                prnInstance->deviceHandle = deviceHandle;

                if( pEPDesc->transferType == (uint32_t)USB_TRANSFER_TYPE_BULK )
                {
                    if(pEPDesc->dirn == (uint32_t)USB_DATA_DIRECTION_DEVICE_TO_HOST)
                    {
                        if(prnInstance->interfaceType != USB_DEVICE_PRINTER_UNIDIRECTIONAL)
                        {
                            /* Bulk IN endpoint. For the device this is TX endpoint */
                            prnInstance->bulkEndpointTx.address = pEPDesc->bEndpointAddress;
                            prnInstance->bulkEndpointTx.maxPacketSize =  pEPDesc->wMaxPacketSize;
                            
                            /* Enable the TX endpoint */
                            (void) USB_DEVICE_EndpointEnable(deviceHandle, 0, pEPDesc->bEndpointAddress, (USB_TRANSFER_TYPE)pEPDesc->transferType, pEPDesc->wMaxPacketSize);
                            /* Indicate that the endpoint is configured */
                            prnInstance->bulkEndpointTx.isConfigured = true;
                        }
                        else
                        {
                            SYS_ASSERT( false, "USB DEVICE Printer: Uni-directional interface does not support BulkIN endpoint. Please check the descriptors.");
                        }
                    }
                    else
                    {
                        /* Enable the receive endpoint */
                        prnInstance->bulkEndpointRx.address = pEPDesc->bEndpointAddress;
                        prnInstance->bulkEndpointRx.maxPacketSize = pEPDesc->wMaxPacketSize;

                        /* Enable the endpoint */
                        (void) USB_DEVICE_EndpointEnable(deviceHandle, 0, pEPDesc->bEndpointAddress, (USB_TRANSFER_TYPE)pEPDesc->transferType,pEPDesc->wMaxPacketSize);
                        /* Indicate that the endpoint is configured */
                        prnInstance->bulkEndpointRx.isConfigured = true;
                        /* Now the device layer has opened the bulk endpoint */
                    }
                }
                else
                {
                    SYS_ASSERT( false, "USB DEVICE Printer: Does not support anything other than Bulk endpoints. Please check the descriptors.");
                }              
                break;
            }
        /* Interface descriptor passed */
        case USB_DESCRIPTOR_INTERFACE:
            {
                pInfDesc = ( USB_INTERFACE_DESCRIPTOR * )pDesc;
 
                if ( ( pInfDesc->bInterfaceClass == USB_PRINTER_INTERFACE_CLASS_CODE ) &&
                        ( pInfDesc->bInterfaceSubClass == USB_PRINTER_INTERFACE_SUBCLASS_CODE ) )
                {
                    /* Save the interface type */
                    if ( pInfDesc->bInterfaceProtocol == 0x1U )
                    {
                        prnInstance->interfaceType = USB_DEVICE_PRINTER_UNIDIRECTIONAL;
                    }
                    else if ( pInfDesc->bInterfaceProtocol == 0x2U )
                    {
                        prnInstance->interfaceType = USB_DEVICE_PRINTER_BIDIRECTIONAL;
                    }
                    else if ( pInfDesc->bInterfaceProtocol == 0x3U )
                    {
                        prnInstance->interfaceType = USB_DEVICE_PRINTER_IEEE1284_4;
                    }
                    else
                    {
                        /* Ignore anything else */
                        SYS_DEBUG(0, "USB Device Printer: Invalid Printer Interface Type. Please check the Interface descriptor" );
                    }
                    
                    prnInstance->deviceIDLength = prnInitializationData->length;
                    for (count = 0; count < prnInstance->deviceIDLength; count++)
                    {
                        prnInstance->deviceIDString[count] = prnInitializationData->deviceID_String[count];
                    }

                    /* Initialize some of the IRP members to an initial value. Note that the
                    * user data of each IRP is set to point to the Device Instance */

                    prnInstance->irpRx.userData = (uintptr_t)prnInstance;
                    prnInstance->irpTx.userData = (uintptr_t)prnInstance;
                    prnInstance->irpRx.status = USB_DEVICE_IRP_STATUS_COMPLETED;
                    prnInstance->irpTx.status = USB_DEVICE_IRP_STATUS_COMPLETED;
                    
                    /* The Host may set an alternate interface on this instance. Initialize the
                    * alternate setting to zero */
                    prnInstance->alternateSetting = 0;
                }
                else
                {
                    /* Ignore anything else */
                    SYS_DEBUG(0, "USB Device Printer: Invalid interface descriptor" );
                }
                break;
            }
        default:
            SYS_ASSERT( false, "USB DEVICE Printer: Please check the descriptors");
            break;
    }

}

// ******************************************************************************
/* Function:
    void F_USB_DEVICE_PRINTER_Deinitialization ( SYS_MODULE_INDEX iPRN )
 
  Summary:
    De-initializes the function driver instance.
  
  Description:
    De-initializes the function driver instance.

  Remarks:
    This is local function and should not be called directly by the application.
*/

void F_USB_DEVICE_PRINTER_Deinitialization ( SYS_MODULE_INDEX iPRN )
{
    /* Cancel all IRPs on the owned endpoints and then 
     * disable the endpoint */

    USB_DEVICE_PRINTER_INSTANCE * prnInstance;

    prnInstance = &gUSBDevicePRINTERInstance[iPRN];

    if(iPRN >= (uint16_t)USB_DEVICE_PRINTER_INSTANCES_NUMBER)
    {
        /* Assert on invalid Printer index */
        SYS_DEBUG(0, "USB Device Printer: Invalid index");
        return;
    } 

     /* Cancel all RX IRPs and close the OUT endpoint */
    (void) USB_DEVICE_IRPCancelAll( prnInstance->deviceHandle, prnInstance->bulkEndpointRx.address );
    (void) USB_DEVICE_EndpointDisable( prnInstance->deviceHandle, prnInstance->bulkEndpointRx.address );
    
    if(prnInstance->interfaceType != USB_DEVICE_PRINTER_UNIDIRECTIONAL)
    {
        /* Cancel all TX IRPs and close the IN endpoint*/
        (void) USB_DEVICE_IRPCancelAll( prnInstance->deviceHandle, prnInstance->bulkEndpointTx.address );
        (void) USB_DEVICE_EndpointDisable( prnInstance->deviceHandle, prnInstance->bulkEndpointTx.address );
    }
}

// ******************************************************************************
/* Function:
    void F_USB_DEVICE_PRINTER_ControlTransferHandler 
    (
        SYS_MODULE_INDEX iPRN ,
        USB_DEVICE_EVENT controlTransferEvent,
        USB_SETUP_PACKET * setupRequest
    )
 
  Summary:
    Control Transfer Handler for class specific control transfer.
  
  Description:
    This is the Control Transfer Handler for class specific control transfer. The
    device layer calls this functions for control transfer that are targeted to
    an interface or endpoint that is owned by this function driver.

  Remarks:
    This is local function and should not be called directly by the application.
*/

void F_USB_DEVICE_PRINTER_ControlTransferHandler 
(
    SYS_MODULE_INDEX iPRN ,
    USB_DEVICE_EVENT controlTransferEvent,
    USB_SETUP_PACKET * setupPacket
)
{
    USB_DEVICE_PRINTER_INSTANCE * prnInstance;
    
    /* Check the validity of the function driver index */
    if (iPRN >= (uint16_t)USB_DEVICE_PRINTER_INSTANCES_NUMBER)
    {
        /* Assert on invalid Printer index */
        SYS_DEBUG(0, "USB Device Printer: Invalid index");
        return;
    }

    /* Get a local reference */
    prnInstance = &gUSBDevicePRINTERInstance[iPRN];

    if(controlTransferEvent == USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST)
    {
        if(( setupPacket->Recipient == 0x01U) && (setupPacket->RequestType == 0x00U))
        {
            /* This means the recipient of the control transfer is interface
             * and this is a standard request type */

            switch(setupPacket->bRequest)
            {
                case USB_REQUEST_SET_INTERFACE:

                    /* If the host does a Set Interface, we simply acknowledge
                     * it. We also remember the interface that was set */
                    prnInstance->alternateSetting = setupPacket->W_Value.byte.LB;
                    (void) USB_DEVICE_ControlStatus( prnInstance->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
                    break;

                case USB_REQUEST_GET_INTERFACE:

                    /* The host is requesting for the current interface setting
                     * number. Return the one that the host set */
                    (void) USB_DEVICE_ControlSend( prnInstance->deviceHandle, &prnInstance->alternateSetting, 1);
                    break;

                default:
                    /* Do Nothing */
                    break; 
            }
        }
        else if(( setupPacket->bmRequestType & USB_PRINTER_REQUEST_CLASS_SPECIFIC ) != 0U)
        {
            /* We have got setup request */
            switch ( setupPacket->bRequest )
            {
                case (uint8_t)USB_PRINTER_GET_DEVICE_ID:

                    /* Return the device ID string that is compatible with IEEE 1284. */
                    (void) USB_DEVICE_ControlSend( prnInstance->deviceHandle, &prnInstance->deviceIDString, prnInstance->deviceIDLength );

                    break;

                case (uint8_t)USB_PRINTER_GET_PORT_STATUS:

                    /* First make sure all request parameters are correct as per the Class Definition for Printing Devices
                     * wValue to be == 0x0000, and wLengh == 1 */
                    if((setupPacket->wValue != 0U) || (setupPacket->wLength != 1U))
                    {
                        (void) USB_DEVICE_ControlStatus( prnInstance->deviceHandle,
                                USB_DEVICE_CONTROL_STATUS_ERROR );
                        return ;
                    }

                    /* Send this event to application. The application should
                    * issue a control send request to send this request to 
                    * the host. */
                    if(prnInstance->appEventCallBack != NULL)
                    {
                        prnInstance->appEventCallBack(iPRN, 
                                USB_DEVICE_PRINTER_GET_PORT_STATUS, 
                                NULL, prnInstance->userData);
                    }

                    break;

                case  (uint8_t)USB_PRINTER_SOFT_RESET:

                    /* First make sure all request parameters are correct */
                   if((setupPacket->wValue != 0U) || (setupPacket->wLength != 0U))
                    {
                        (void) USB_DEVICE_ControlStatus( prnInstance->deviceHandle,
                                USB_DEVICE_CONTROL_STATUS_ERROR );
                        return ;
                    }

                    /* Cancel all the IRPs */ 
                    (void) USB_DEVICE_IRPCancelAll(prnInstance->deviceHandle, prnInstance->bulkEndpointRx.address);
                    if(USB_DEVICE_EndpointIsStalled(prnInstance->deviceHandle, prnInstance->bulkEndpointRx.address))
                    {
                       /* Clear stalled OUT endpoint */
                       USB_DEVICE_EndpointStallClear(prnInstance->deviceHandle, prnInstance->bulkEndpointRx.address);
                    }

                    if(prnInstance->interfaceType != USB_DEVICE_PRINTER_UNIDIRECTIONAL)
                    {
                        /* Cancel all the IRPs */
                        (void) USB_DEVICE_IRPCancelAll(prnInstance->deviceHandle, prnInstance->bulkEndpointTx.address);
                        if(USB_DEVICE_EndpointIsStalled(prnInstance->deviceHandle, prnInstance->bulkEndpointTx.address))
                        {
                            /* Clear stalled IN endpoint */
                            USB_DEVICE_EndpointStallClear(prnInstance->deviceHandle, prnInstance->bulkEndpointTx.address);
                        }
                    }

                    (void) USB_DEVICE_ControlStatus( prnInstance->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK );
                   break;

                default:

                   /* Stall other requests. */
                   (void) USB_DEVICE_ControlStatus( prnInstance->deviceHandle, USB_DEVICE_CONTROL_STATUS_ERROR );
                   break;
            } 
        }
        else
        {
            /* Do Nothing */
        }
    }
}

// ******************************************************************************
/* Function:
    void F_USB_DEVICE_PRINTER_ReadIRPCallback (USB_DEVICE_IRP * irp )
 
  Summary:
    IRP call back for Data Read IRPs.
  
  Description:
    This is IRP call back for Read IRP submitted.

  Remarks:
    This is local function and should not be called directly by the application.
*/

void F_USB_DEVICE_PRINTER_ReadIRPCallback (USB_DEVICE_IRP * irp )
{
    USB_DEVICE_PRINTER_INSTANCE * prnInstance;

    /* This function is called when an IRP has
     * terminated */
    
    USB_DEVICE_PRINTER_EVENT_DATA_READ_COMPLETE readEventData;

    /* The user data field of the IRP contains the Printer instance
     * that submitted this IRP */
    prnInstance = &gUSBDevicePRINTERInstance[irp->userData];

    /* populate the event handler for this transfer */
    readEventData.handle = ( USB_DEVICE_PRINTER_TRANSFER_HANDLE ) irp;

    /* update the size written */
    readEventData.length = irp->size;
    
    /* Get transfer status */
    if ((irp->status == USB_DEVICE_IRP_STATUS_COMPLETED) 
        || (irp->status == USB_DEVICE_IRP_STATUS_COMPLETED_SHORT))
    {
        /* Transfer completed successfully */
        readEventData.status = USB_DEVICE_PRINTER_RESULT_OK; 
    }
    else if (irp->status == USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT)
    {
        /* Transfer cancelled due to Endpoint Halt */
        readEventData.status = USB_DEVICE_PRINTER_RESULT_ERROR_ENDPOINT_HALTED; 
    }
    else if (irp->status == USB_DEVICE_IRP_STATUS_TERMINATED_BY_HOST)
    {
        /* Transfer Cancelled by Host (Host sent a Clear feature )*/
        readEventData.status = USB_DEVICE_PRINTER_RESULT_ERROR_TERMINATED_BY_HOST; 
    }
    else
    {
        /* Transfer was not completed successfully */
        readEventData.status = USB_DEVICE_PRINTER_RESULT_ERROR; 
    }

    /* update the queue size */
    prnInstance->currentQSizeRead --;

    /* valid application event handler present? */
    if ( prnInstance->appEventCallBack  != NULL)
    {
        /* inform the application */
        prnInstance->appEventCallBack ( (USB_DEVICE_PRINTER_INDEX)(irp->userData) , 
                   USB_DEVICE_PRINTER_EVENT_READ_COMPLETE , 
                   &readEventData, prnInstance->userData);
    }

}

// ******************************************************************************
/* Function:
    void F_USB_DEVICE_PRINTER_WriteIRPCallback (USB_DEVICE_IRP * irp )
 
  Summary:
    IRP call back for Data Write IRPs.
  
  Description:
    This is IRP call back for Write IRP submitted.
    
  Remarks:
    This is local function and should not be called directly by the application.
*/

void F_USB_DEVICE_PRINTER_WriteIRPCallback (USB_DEVICE_IRP * irp )
{
    USB_DEVICE_PRINTER_INSTANCE * prnInstance;

    /* This function is called when a Printer Write IRP has
     * terminated */
    
    USB_DEVICE_PRINTER_EVENT_DATA_WRITE_COMPLETE writeEventData;

    /* The user data field of the IRP contains the Printer instance
     * that submitted this IRP */
    prnInstance = &gUSBDevicePRINTERInstance[irp->userData];

    /* populate the event handler for this transfer */
    writeEventData.handle = ( USB_DEVICE_PRINTER_TRANSFER_HANDLE ) irp;

    /* update the size written */
    writeEventData.length = irp->size;
    
    /* Get transfer status */
    if ((irp->status == USB_DEVICE_IRP_STATUS_COMPLETED) 
        || (irp->status == USB_DEVICE_IRP_STATUS_COMPLETED_SHORT))
    {
        /* Transfer completed successfully */
        writeEventData.status = USB_DEVICE_PRINTER_RESULT_OK; 
    }
    else if (irp->status == USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT)
    {
        /* Transfer cancelled due to Endpoint Halt */
        writeEventData.status = USB_DEVICE_PRINTER_RESULT_ERROR_ENDPOINT_HALTED; 
    }
    else if (irp->status == USB_DEVICE_IRP_STATUS_TERMINATED_BY_HOST)
    {
        /* Transfer Cancelled by Host (Host sent a Clear feature )*/
        writeEventData.status = USB_DEVICE_PRINTER_RESULT_ERROR_TERMINATED_BY_HOST; 
    }
    else
    {
        /* Transfer was not completed successfully */
        writeEventData.status = USB_DEVICE_PRINTER_RESULT_ERROR; 
    }

    /* Update the queue size*/
    prnInstance->currentQSizeWrite --;

    /* valid application event handler present? */
    if ( prnInstance->appEventCallBack != NULL)
    {
        /* inform the application */
        prnInstance->appEventCallBack ( (USB_DEVICE_PRINTER_INDEX)(irp->userData) , 
                   USB_DEVICE_PRINTER_EVENT_WRITE_COMPLETE , 
                   &writeEventData, prnInstance->userData);
    }

}

// *****************************************************************************
// *****************************************************************************
// Section: Printer Interface Function Definitions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Function:
    USB_DEVICE_PRINTER_Read() 

  Summary:
    This function requests a data read from the USB Device Printer Function 
    Driver Layer.

  Description:
    The function places a requests with driver, the request will get
    serviced as data is made available by the USB Host. A handle to the request
    is returned in the transferHandle parameter. The termination of the request
    is indicated by the USB_DEVICE_PRINTER_EVENT_READ_COMPLETE event. The amount of
    data read and the transfer handle associated with the request is returned
    along with the event in the pData parameter of the event handler. The
    transfer handle expires when event handler for the
    USB_DEVICE_PRINTER_EVENT_READ_COMPLETE exits. If the read request could not be
    accepted, the function returns an error code and transferHandle will contain
    the value USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID.

    If the size parameter is not a multiple of maxPacketSize or is 0, the
    function returns USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID in transferHandle
    and returns an error code as a return value. If the size parameter is a
    multiple of maxPacketSize and the host send less than maxPacketSize data in
    any transaction, the transfer completes and the function driver will issue a
    USB_DEVICE_PRINTER_EVENT_READ_COMPLETE event along with the
    USB_DEVICE_PRINTER_EVENT_READ_COMPLETE_DATA data structure. If the size
    parameter is a multiple of maxPacketSize and the host sends maxPacketSize
    amount of data, and total data received does not exceed size, then the
    function driver will wait for the next packet. 
  
  Remarks:
    This is a global function and can be called from application.
*/   

USB_DEVICE_PRINTER_RESULT USB_DEVICE_PRINTER_Read 
(
    USB_DEVICE_PRINTER_INDEX instanceIndex ,
    USB_DEVICE_PRINTER_TRANSFER_HANDLE * transferHandle ,
    void * data , size_t size
)
{
    uint32_t cnt;
    uint32_t remainderValue;
    USB_DEVICE_IRP * irp;
    USB_DEVICE_PRINTER_ENDPOINT * endpoint;
    USB_DEVICE_PRINTER_INSTANCE * prnInstance;
    OSAL_RESULT osalError;
    USB_ERROR irpError;
    OSAL_CRITSECT_DATA_TYPE IntState;

    /* Check the validity of the function driver index */
    
    if (  instanceIndex >= (uint32_t)USB_DEVICE_PRINTER_INSTANCES_NUMBER  )
    {
        /* Invalid PRINTER index */
        SYS_ASSERT(false, "Invalid PRINTER Device Index");
        return USB_DEVICE_PRINTER_RESULT_ERROR_INSTANCE_INVALID;
    }

    prnInstance = &gUSBDevicePRINTERInstance[instanceIndex];
    endpoint = &prnInstance->bulkEndpointRx;
    *transferHandle = USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID;

    /* Check if the endpoint is configured */
    if(!(endpoint->isConfigured))
    {
        /* This means that the endpoint is not configured yet */
        SYS_ASSERT(false, "Endpoint not configured");
        return (USB_DEVICE_PRINTER_RESULT_ERROR_INSTANCE_NOT_CONFIGURED);
    }

    /* For read the size should be a multiple of endpoint size*/
    remainderValue = size % endpoint->maxPacketSize;

    if((size == 0U) || (remainderValue != 0U))
    {
        /* Size is not valid */
        SYS_ASSERT(false, "Invalid size in IRP read");
        return(USB_DEVICE_PRINTER_RESULT_ERROR_TRANSFER_SIZE_INVALID);
    }

    /* Make sure that we are with in the queue size for this instance */
    if(prnInstance->currentQSizeRead >= prnInstance->queueSizeRead)
    {
        SYS_ASSERT(false, "Read Queue is full");
        return(USB_DEVICE_PRINTER_RESULT_ERROR_TRANSFER_QUEUE_FULL);
    }

    /*Obtain mutex to get access to a shared resource, check return value*/
    osalError = OSAL_MUTEX_Lock(&gUSBDevicePrinterCommonDataObj.mutexPrinterIRP, OSAL_WAIT_FOREVER);
    if(osalError != OSAL_RESULT_TRUE)
    {
      /*Do not proceed lock was not obtained, or error occurred, let user know about error*/
      return (USB_DEVICE_PRINTER_RESULT_ERROR);
    }

    /* Loop and find a free IRP in the Q */
    for ( cnt = 0; cnt < USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED; cnt ++ )
    {
        if(gUSBDevicePrinterIRP[cnt].status <
                (USB_DEVICE_IRP_STATUS)USB_DEVICE_IRP_FLAG_DATA_PENDING)
        {
            /* This means the IRP is free. Configure the IRP
             * update the current queue size and then submit */

            irp = &gUSBDevicePrinterIRP[cnt];
            irp->data = data;
            irp->size = size;
            irp->userData = (uintptr_t) instanceIndex;
            irp->callback = F_USB_DEVICE_PRINTER_ReadIRPCallback;
            
            /* Prevent other tasks pre-empting this sequence of code */ 
            IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
            /* Update the read queue size */ 
            prnInstance->currentQSizeRead++;
            OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState);
            
            *transferHandle = (USB_DEVICE_PRINTER_TRANSFER_HANDLE)irp;
            irpError = USB_DEVICE_IRPSubmit(prnInstance->deviceHandle,
                    endpoint->address, irp);

            /* If IRP Submit function returned any error, then invalidate the
               Transfer handle.  */
            if (irpError != USB_ERROR_NONE )
            {
                /* Prevent other tasks pre-empting this sequence of code */ 
                IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                /* Update the read queue size */ 
                prnInstance->currentQSizeRead--;
                OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState);
                *transferHandle = USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID;
            }
            
            /*Release mutex, done with shared resource*/
            osalError = OSAL_MUTEX_Unlock(&gUSBDevicePrinterCommonDataObj.mutexPrinterIRP);
            if(osalError != OSAL_RESULT_TRUE)
            {
                /*Do not proceed unlock was not complete, or error occurred, let user know about error*/
                return (USB_DEVICE_PRINTER_RESULT_ERROR);
            }
            
            return((USB_DEVICE_PRINTER_RESULT)irpError);
        }
    }
    
    /*Release mutex, done with shared resource*/
    osalError = OSAL_MUTEX_Unlock(&gUSBDevicePrinterCommonDataObj.mutexPrinterIRP);
    if(osalError != OSAL_RESULT_TRUE)
    {
        /*Do not proceed unlock was not complete, or error occurred, let user know about error*/
        return (USB_DEVICE_PRINTER_RESULT_ERROR);
    }
    /* If here means we could not find a spare IRP */
    return(USB_DEVICE_PRINTER_RESULT_ERROR_TRANSFER_QUEUE_FULL);
}

// *****************************************************************************
/* Function:
     USB_DEVICE_PRINTER_Write 

  Summary:
    This function requests a data write to the USB Device Printer Function Driver 
    Layer.

  Description:
    This function requests a data write to the USB Device Printer Function Driver
    Layer. The function places a requests with driver, the request will get
    serviced as data is requested by the USB Host. A handle to the request is
    returned in the transferHandle parameter. The termination of the request is
    indicated by the USB_DEVICE_PRINTER_EVENT_WRITE_COMPLETE event. The amount of
    data written and the transfer handle associated with the request is returned
    along with the event in writeCompleteData member of the pData parameter in
    the event handler. The transfer handle expires when event handler for the
    USB_DEVICE_PRINTER_EVENT_WRITE_COMPLETE exits.  If the read request could not be
    accepted, the function returns an error code and transferHandle will contain
    the value USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID.

  Remarks:
    Refer to usb_device_printer.h for usage information.
*/

USB_DEVICE_PRINTER_RESULT USB_DEVICE_PRINTER_Write 
(
    USB_DEVICE_PRINTER_INDEX instanceIndex ,
    USB_DEVICE_PRINTER_TRANSFER_HANDLE * transferHandle ,
    const void * data , size_t size ,
    USB_DEVICE_PRINTER_TRANSFER_FLAGS flags 
)
{
    uint32_t cnt;
    uint32_t remainderValue;
    USB_DEVICE_IRP * irp;
    USB_DEVICE_IRP_FLAG irpFlag = 0;
    USB_DEVICE_PRINTER_INSTANCE * prnInstance;
    USB_DEVICE_PRINTER_ENDPOINT * endpoint;
    OSAL_RESULT osalError;
    USB_ERROR irpError; 
    OSAL_CRITSECT_DATA_TYPE IntState;

    /* Check the validity of the function driver index */
    
    if (  instanceIndex >= (uint32_t)USB_DEVICE_PRINTER_INSTANCES_NUMBER  )
    {
        /* Invalid Printer index */
        SYS_ASSERT(false, "Invalid PRINTER Device Index");
        return USB_DEVICE_PRINTER_RESULT_ERROR_INSTANCE_INVALID;
    }

    /* Initialize the transfer handle, get the instance object
     * and the transmit endpoint */

    * transferHandle = USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID;
    prnInstance = &gUSBDevicePRINTERInstance[instanceIndex];
    endpoint = &prnInstance->bulkEndpointTx;

    if(!(endpoint->isConfigured))
    {
        /* This means that the endpoint is not configured yet */
        SYS_ASSERT(false, "Endpoint not configured");
        return (USB_DEVICE_PRINTER_RESULT_ERROR_INSTANCE_NOT_CONFIGURED);
    }

    if(size == 0U) 
    {
        /* Size cannot be zero */
        return (USB_DEVICE_PRINTER_RESULT_ERROR_TRANSFER_SIZE_INVALID);
    }

    /* Check the flag */

    if(((uint32_t)flags & (uint32_t)USB_DEVICE_PRINTER_TRANSFER_FLAGS_MORE_DATA_PENDING) != 0U)
    {
        if(size < endpoint->maxPacketSize)
        {
            /* For a data pending flag, we must atleast get max packet
             * size worth data */

            return(USB_DEVICE_PRINTER_RESULT_ERROR_TRANSFER_SIZE_INVALID);
        }

        remainderValue = size % endpoint->maxPacketSize;
        
        if(remainderValue != 0U)
        {
            size -= remainderValue;
        }

        irpFlag = USB_DEVICE_IRP_FLAG_DATA_PENDING;
    }
    else if(((uint32_t)flags & (uint32_t)USB_DEVICE_PRINTER_TRANSFER_FLAGS_DATA_COMPLETE) != 0U)
    {
        irpFlag = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;
    }
    else
    {
        /* Do Nothing */
    }

    if(prnInstance->currentQSizeWrite >= prnInstance->queueSizeWrite)
    {
        SYS_ASSERT(false, "Write Queue is full");
        return(USB_DEVICE_PRINTER_RESULT_ERROR_TRANSFER_QUEUE_FULL);
    }

    /*Obtain mutex to get access to a shared resource, check return value*/
    osalError = OSAL_MUTEX_Lock(&gUSBDevicePrinterCommonDataObj.mutexPrinterIRP, OSAL_WAIT_FOREVER);
    if(osalError != OSAL_RESULT_TRUE)
    {
      /*Do not proceed lock was not obtained, or error occurred, let user know about error*/
      return (USB_DEVICE_PRINTER_RESULT_ERROR);
    }

    /* loop and find a free IRP in the Q */
    for ( cnt = 0; cnt < USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED; cnt ++ )
    {
        if(gUSBDevicePrinterIRP[cnt].status <
                (USB_DEVICE_IRP_STATUS)USB_DEVICE_IRP_FLAG_DATA_PENDING)
        {
            /* This means the IRP is free */

            irp         = &gUSBDevicePrinterIRP[cnt];
            irp->data   = (void *)data;
            irp->size   = size;

            irp->userData   = (uintptr_t) instanceIndex;
            irp->callback   = F_USB_DEVICE_PRINTER_WriteIRPCallback;
            irp->flags      = irpFlag;

            /* Prevent other tasks pre-empting this sequence of code */ 
            IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
            /* Update the Write queue size */ 
            prnInstance->currentQSizeWrite++;
            OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState);
            
            *transferHandle = (USB_DEVICE_PRINTER_TRANSFER_HANDLE)irp;

            irpError = USB_DEVICE_IRPSubmit(prnInstance->deviceHandle,
                    endpoint->address, irp);

            /* If IRP Submit function returned any error, then invalidate the
               Transfer handle.  */
            if (irpError != USB_ERROR_NONE )
            {
                /* Prevent other tasks pre-empting this sequence of code */ 
                IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                /* Update the Write queue size */ 
                prnInstance->currentQSizeWrite--;
                OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState);
                *transferHandle = USB_DEVICE_PRINTER_TRANSFER_HANDLE_INVALID;
            }
            /*Release mutex, done with shared resource*/
            osalError = OSAL_MUTEX_Unlock(&gUSBDevicePrinterCommonDataObj.mutexPrinterIRP);
            if(osalError != OSAL_RESULT_TRUE)
            {
                /*Do not proceed unlock was not complete, or error occurred, let user know about error*/
                return (USB_DEVICE_PRINTER_RESULT_ERROR);
            }

            return((USB_DEVICE_PRINTER_RESULT)irpError);
        }
    }
    
    /*Release mutex, done with shared resource*/
    osalError = OSAL_MUTEX_Unlock(&gUSBDevicePrinterCommonDataObj.mutexPrinterIRP);
    if(osalError != OSAL_RESULT_TRUE)
    {
        /*Do not proceed unlock was not complete, or error occurred, let user know about error*/
        return (USB_DEVICE_PRINTER_RESULT_ERROR);
    }
    /* If here means we could not find a spare IRP */
    return(USB_DEVICE_PRINTER_RESULT_ERROR_TRANSFER_QUEUE_FULL);
}

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.4"
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.3"
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */

USB_DEVICE_PRINTER_RESULT USB_DEVICE_PRINTER_EventHandlerSet 
(
    USB_DEVICE_PRINTER_INDEX instanceIndex  ,
    USB_DEVICE_PRINTER_EVENT_HANDLER eventHandler,
    uintptr_t context
)
{
    /* Check the validity of the function driver index */
    if (( instanceIndex  >= (uint32_t)USB_DEVICE_PRINTER_INSTANCES_NUMBER ) )
    {
        /* invalid Printer index */
        return USB_DEVICE_PRINTER_RESULT_ERROR_INSTANCE_INVALID;
    }

    /* Check if the given event handler is valid */
    if ( eventHandler != NULL)
    {
        /* update the event handler for this instance */
        gUSBDevicePRINTERInstance[instanceIndex ].appEventCallBack = eventHandler;
        gUSBDevicePRINTERInstance[instanceIndex ].userData = context;

        /* return success */
        return USB_DEVICE_PRINTER_RESULT_OK;
    }

    else
    {
        /* invalid event handler passed */
        return USB_DEVICE_PRINTER_RESULT_ERROR_PARAMETER_INVALID;
    }
}

/*******************************************************************************
 End of File
 */
