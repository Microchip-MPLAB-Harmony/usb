/*******************************************************************************
 USB Audio Class Function Driver - Read and Write functions. 

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_audio.c

  Summary:
    USB audio class function driver.

  Description:
    USB audio class function driver.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "usb/usb_device_audio_v1_0.h"
#include "usb/src/usb_device_audio_local.h"


// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Audio function instance objects.

  Summary:
    Holds all of the variables required by audio instance.

  Description:
    This structure holds all of the variables required by audio instance.

  Remarks:
    None.
*/

USB_DEVICE_AUDIO_INSTANCE gUsbDeviceAudioInstance[USB_DEVICE_AUDIO_INSTANCES_NUMBER];


// *****************************************************************************
/* AUDIO Device IRPs

  Summary:
    Array of AUDIO Device IRP.

  Description:
    Array of AUDIO Device IRP. This array of IRP will be shared by read, write and
    notification data requests.

  Remarks:
    This array is private to the USB stack.
 */

USB_DEVICE_IRP gUSBDeviceAudioIRP[USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED];

/* Create a variable for holding Audio IRP mutex Handle and status */
USB_DEVICE_AUDIO_COMMON_DATA_OBJ gUSBDeviceAudioCommonDataObj;
// *****************************************************************************
/* AUDIO Device IRP Data

  Summary:
    Array of AUDIO Device IRP Data.

  Description:
    Array of AUDIO Device IRP. This array of IRP will be shared by read, write and
    notification data requests.

  Remarks:
    This array is private to the USB stack.
 */
USB_DEVICE_AUDIO_IRP_DATA gUSBDeviceAudioIrpData [USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED];

// *****************************************************************************
/* AUDIO Device IRP Unique Identifier

  Summary:
    USB Device Audio Unique Buffer Identifier.

  Description:
    This global variable is used along with the buffer index to create a unique
    buffer handle for each Audio Buffer Request.

  Remarks:
    Private to the USB Audio Function Driver.
 */

static uint32_t gUSBDeviceAudioUniqueBufferID = 0;

/* ******************************************************************************
  Function:
    USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_EventHandlerSet
    (
        USB_DEVICE_AUDIO_INDEX instanceIndex ,
        USB_DEVICE_AUDIO_EVENT_HANDLER eventHandler ,
        uintptr_t context
    );
    
  Summary:
    This function registers an event handler for the specified Audio function
    driver instance. 

  Description:
  Refer  usb_device_audio_v1_0.h for detailed description of this function. 
  
  */ 
USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_EventHandlerSet
(
    USB_DEVICE_AUDIO_INDEX instanceIndex  ,
    USB_DEVICE_AUDIO_EVENT_HANDLER eventHandler ,
    uintptr_t context
)
{
    USB_DEVICE_AUDIO_RESULT error = USB_DEVICE_AUDIO_RESULT_ERROR_PARAMETER_INVALID;

    if(eventHandler != NULL)
    {
        gUsbDeviceAudioInstance[instanceIndex ].appEventCallBack = eventHandler;
        gUsbDeviceAudioInstance[instanceIndex ].userData = context;
        error = USB_DEVICE_AUDIO_RESULT_OK;
    }
    return error;
}

// *****************************************************************************
/* Function:
    USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_Read ( USB_DEVICE_AUDIO_INDEX iAudio ,
                                      uint8_t interfaceNum ,
                                      void * data ,
                                      size_t size )

  Summary:
    Reads the data received from the Audio Interface for
 *  specified instance of the USB device layer.

  Description:
    Reads the data received from the Audio Interface for
 *  specified instance of the USB device layer.

 *
  Parameters:
    USB_DEVICE_AUDIO_INDEX iAudio    - Audio function driver Index number
 *
 *  uint8_t interfaceNum    - Audio streaming or Control Interface number
 *
 *  data - pointer to the data buffer where read data will be stored.
 *  size - Size of the data buffer. Refer to the description section for more
           details on how the size affects the transfer.
 *
 *
  Returns:
    USB_DEVICE_AUDIO_RESULT

*/

USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_Read
(
    USB_DEVICE_AUDIO_INDEX instanceIndex ,
    USB_DEVICE_AUDIO_TRANSFER_HANDLE* transferHandle,
    uint8_t interfaceNumber ,
    void * data ,
    size_t size
)
{
        USB_DEVICE_AUDIO_RESULT audioResult;
        USB_DEVICE_AUDIO_INSTANCE * thisAudioDevice;
        thisAudioDevice = &gUsbDeviceAudioInstance[instanceIndex];

         /* Make sure that we are with in the queue size for this instance */
        if(thisAudioDevice->currentQSizeRead >= thisAudioDevice->queueSizeRead)
        {
            SYS_ASSERT(false, "Read Queue is full");
            return(USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL);
        }
    audioResult =   F_USB_DEVICE_AUDIO_Transfer(instanceIndex, transferHandle, interfaceNumber, data, size, USB_DEVICE_AUDIO_READ );
        return audioResult; 
}


//******************************************************************************

// *****************************************************************************
// *****************************************************************************
/* Function:
    USB_ERROR USB_DEVICE_AUDIO_Write ( USB_DEVICE_AUDIO_INDEX instanceIndex ,
                                         uint8_t interfaceNum ,
                                         USB_DEVICE_AUDIO_DATA_BUFFER_OBJECT* bufferObj )

  Summary:
    sends to the Audio Interface for
 *  the specified instance of the USB device layer.

  Description:
    sends to the Audio Interface for
 *  the specified instance of the USB device layer.

 *
  Parameters:
    USB_DEVICE_AUDIO_INDEX instanceIndex    - Audio function driver Index number
 *
 *  uint8_t interfaceNum    - Audio streaming or Control Interface number
 *
 *  USB_DEVICE_AUDIO_DATA_BUFFER_OBJECT* bufferObj - pointer to the buffer where received data
 *
 * is to be stored.
 *
 *
  Returns:
    USB_DEVICE_AUDIO_RESULT

*/


USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_Write
(
    USB_DEVICE_AUDIO_INDEX instanceIndex ,
    USB_DEVICE_AUDIO_TRANSFER_HANDLE* transferHandle,
    uint8_t interfaceNumber ,
    void * data ,
    size_t size
)
{
        USB_DEVICE_AUDIO_RESULT audioResult;
        USB_DEVICE_AUDIO_INSTANCE * thisAudioDevice;
        thisAudioDevice = &gUsbDeviceAudioInstance[instanceIndex];

        /* Make sure that we are with in the queue size for this instance */
        if(thisAudioDevice->currentQSizeWrite >= thisAudioDevice->queueSizeWrite)
        {
            SYS_ASSERT(false, "Write Queue is full");
            return(USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL);
        }
     audioResult =   F_USB_DEVICE_AUDIO_Transfer(instanceIndex, transferHandle, interfaceNumber, data, size, USB_DEVICE_AUDIO_WRITE );
        return audioResult;
}


// *****************************************************************************
/* MISRA C-2012 Rule 10.4 False Positive:6 Deviation record ID -  H3_MISRAC_2012_R_10_4_DR_1 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma coverity compliance block fp:6 "MISRA C-2012 Rule 10.4" "H3_MISRAC_2012_R_10_4_DR_1"
/* Function:
    USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_StatusSend 
    (
        USB_DEVICE_AUDIO_INDEX instanceIndex,
        USB_DEVICE_AUDIO_TRANSFER_HANDLE* transferHandle,
        USB_AUDIO_INTERRUPT_STATUS_WORD* status
     )

  Summary:
   This function requests a Status write to the USB Device Audio Function Driver 
   Layer.

  Description:
    This function requests a status write to the USB Device Audio Function
    Driver Layer. The function places a requests with driver to arm the status 
    interrupt Endpoint with the status provided, the request will get serviced 
    as data is requested by the USB Host. A handle to the request is returned in 
    the transferHandle parameter. The termination of the request is indicated by
    the USB_DEVICE_AUDIO_EVENT_STATUS_SEND_COMPLETE event.
    
    The transfer handle expires when event handler for the
    USB_DEVICE_AUDIO_EVENT_STATUS_SEND_COMPLETE exits. If the Status Send 
    request could not be accepted, the function returns an error code and 
    transferHandle will contain the value 
    USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID.

  Remarks:
    Refer to usb_device_audio_v1_0.h for usage information.
*/
USB_DEVICE_AUDIO_RESULT USB_DEVICE_AUDIO_StatusSend 
(
    USB_DEVICE_AUDIO_INDEX instanceIndex,
    USB_DEVICE_AUDIO_TRANSFER_HANDLE* transferHandle,
    USB_AUDIO_INTERRUPT_STATUS_WORD* status
)
{
    uint32_t cnt;
    USB_DEVICE_IRP *irp;
    USB_DEVICE_AUDIO_IRP_DATA *audioIrpData;
    USB_DEVICE_AUDIO_INSTANCE *thisAudioInstance = NULL; 
    USB_ENDPOINT epStruct;
    OSAL_RESULT osalError;
    USB_ERROR irpErr;
    bool irpFound = false; 
    OSAL_CRITSECT_DATA_TYPE IntState;
    USB_DEVICE_AUDIO_EP_INSTANCE * endpoint = NULL;
    USB_DEVICE_AUDIO_RESULT audioResult = USB_DEVICE_AUDIO_RESULT_OK;
    
    /* Check the validity of the function driver index */
    if ((uint32_t)USB_DEVICE_AUDIO_INSTANCES_NUMBER <= instanceIndex )
    {
        /* Invalid handle */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB Device Audio v1.0 : Invalid Audio Index");
        audioResult = USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_INVALID;
    }
    else 
    {
        /* Get handle to Audio Instance Structure */  
        thisAudioInstance = &gUsbDeviceAudioInstance[instanceIndex];
        * transferHandle = USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID;
        
		/* Get pointer to the Status Interrupt Endpoint */ 
        endpoint = &thisAudioInstance->infCollection.intEp;
        
        if ((thisAudioInstance->infCollection.isIntEpExists != true)
            || (endpoint->status != true))
        
        {
			/* This means status interrupt endpoint does not exists in the descriptors */ 
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Device Audio v1.0 : Instance %d: Instance not configured", instanceIndex);
            audioResult = USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_NOT_CONFIGURED; 
        }
        
        /* Make sure that we are with in the queue size for this instance */
        else if(thisAudioInstance->currentQSizeStatusSend >= thisAudioInstance->queueSizeStatusSend)
        {
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Device Audio v1.0 : Instance %d: Queue Full", instanceIndex);
            audioResult = USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL; 
        }
    
        /* Check if user passed valid buffer */
        else if ( status == NULL )
        {
            /* Status data should not be null. It should point to valid data location
               return error */
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Device Audio v1.0 : Instance %d: invalid buffer", instanceIndex);
            audioResult = USB_DEVICE_AUDIO_RESULT_ERROR_INVALID_BUFFER;
        }
        else
        {
            /* Do Nothing */
        }
    }
    if (audioResult == USB_DEVICE_AUDIO_RESULT_OK)
    {  
        /*Obtain mutex to get access to a shared resource, check return value*/
        osalError = OSAL_MUTEX_Lock(&gUSBDeviceAudioCommonDataObj.mutexAUDIOIRP, OSAL_WAIT_FOREVER);
        if(osalError == OSAL_RESULT_TRUE)
        {
            /* Loop and find a free IRP in the Q */
            for ( cnt = 0; cnt < (uint32_t)USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED; cnt++ )
            {
                /* get the IRP status */
                if(gUSBDeviceAudioIRP[cnt].status <
                        (USB_DEVICE_IRP_STATUS)USB_DEVICE_IRP_FLAG_DATA_PENDING)
                {
                    irpFound = true; 
                    
                    irp = &gUSBDeviceAudioIRP[cnt];
                    audioIrpData =&gUSBDeviceAudioIrpData[cnt];

                    /* Increment the Unique Buffer ID for the request. If the ID reaches
                    0xFFFF, we let it roll over. This avoid confusing the generated
                    unique buffer handle with a invalid buffer handle. */

                    gUSBDeviceAudioUniqueBufferID ++;
                    gUSBDeviceAudioUniqueBufferID = (gUSBDeviceAudioUniqueBufferID == 0xFFFFU) ? 0U : gUSBDeviceAudioUniqueBufferID ;

                    /* Retrieve endpoint address */ 
                    epStruct = endpoint->epAddr;

                    /* Fill IRP object with the pointer to the data that is to 
                       be transferred to the Host*/
                    irp->data = status;

                    /* Fill IRP object with size of the data that is to be 
                       transferred to the USB host*/
                    irp->size = 2;
                    
                    /* Save Audio function driver instance */
                    audioIrpData->iAudio = instanceIndex; 

                    /* Provide function address to call back when IRP is 
                       complete */
                    irp->callback = F_USB_DEVICE_AUDIO_StatusSendIRPCallBack;
                    
                    /* Request driver to complete the transfer */
                    irp->flags = USB_DEVICE_IRP_FLAG_DATA_PENDING; 

                    /* Save Audio Instance Number. We will need this to retrieve
                       data when we get IRP call back */
                    irp->userData = (gUSBDeviceAudioUniqueBufferID << 16) | instanceIndex;

                    /* Save transfer handle */
                    *transferHandle = (USB_DEVICE_AUDIO_TRANSFER_HANDLE) ((gUSBDeviceAudioUniqueBufferID << 16) | cnt);

                    /* Prevent other tasks pre-empting this sequence of code */
                    IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                    
                    /* Update the read queue size */
                    thisAudioInstance->currentQSizeStatusSend++;
                    
                    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState);
            
        
                    /* Submit IRP */ 
                    irpErr = USB_DEVICE_IRPSubmit ( thisAudioInstance->devLayerHandle ,
                                            epStruct , irp);
                                            
                    /* Check if IRP submit is failed */
                    if ( USB_ERROR_NONE != irpErr )
                    {
                        /* Prevent other tasks pre-empting this sequence of code */
                        IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                
                        /* Update the read queue size */
                        thisAudioInstance->currentQSizeStatusSend--;
                        
                        OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState); 
                        
                        /* Invalidate the Transfer handle.  */
                        *transferHandle = USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID;
                    }
                
                    /*Release mutex, done with shared resource*/
                    osalError = OSAL_MUTEX_Unlock(&gUSBDeviceAudioCommonDataObj.mutexAUDIOIRP);
                    if(osalError == OSAL_RESULT_TRUE)
                    {
                        audioResult = (USB_DEVICE_AUDIO_RESULT)irpErr; 
                    }
                    else
                    {
                        /*Do not proceed lock was not obtained, or error occurred, let user know about error*/
                        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Device Audio v1.0 : Instance %d: Mutex Release failed", instanceIndex);
                        audioResult = USB_DEVICE_AUDIO_RESULT_ERROR;
                    }
                    /* We have finished processing the IRP. Now exit from the 
                       for loop */ 
                    break; 
                }
            }
            if (irpFound == false)
            {
                /*Release mutex here as above loop does not release the mutex if 
                  no free IRP was found */
                osalError = OSAL_MUTEX_Unlock(&gUSBDeviceAudioCommonDataObj.mutexAUDIOIRP);
                if(osalError == OSAL_RESULT_TRUE)
                {
                   /* There was no free IRP. Queue was full  */
                  SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Device Audio v1.0 : Instance %d: Queue was full", instanceIndex);
                  audioResult = USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL; 
                }  
                else
                {
                    /* Do not proceed, unlock was not complete, or error occurred, let 
                       user know about error */
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Device Audio v1.0 : Instance %d: Mutex Release failed", instanceIndex);
                    audioResult = USB_DEVICE_AUDIO_RESULT_ERROR; 
                }
            }
        }
        else
        {
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Device Audio v1.0 : Instance %d: Mutex Lock failed", instanceIndex);
            audioResult = USB_DEVICE_AUDIO_RESULT_ERROR; 
        }     
    }
    
    /* Return result */ 
    return audioResult; 
}



/* ******************************************************************************
  Function:
    USB_DEVICE_AUDIO_RESULT F_USB_DEVICE_AUDIO_Transfer
    (
        USB_DEVICE_AUDIO_INDEX iAudio,
        USB_DEVICE_AUDIO_TRANSFER_HANDLE *transferHandle,
        uint8_t interfaceNum ,
        void * data ,
        size_t size,
        USB_DEVICE_AUDIO_TRANSFER_DIRECTION direction
    ); 
    
  Summary:
    This function schedules an Audio transfer. This is a local function and should
    be called by applications directly. 

  */ 
// *****************************************************************************
USB_DEVICE_AUDIO_RESULT F_USB_DEVICE_AUDIO_Transfer
(
    USB_DEVICE_AUDIO_INDEX iAudio,
    USB_DEVICE_AUDIO_TRANSFER_HANDLE *transferHandle,
    uint8_t interfaceNum ,
    void * data ,
    size_t size,
    USB_DEVICE_AUDIO_TRANSFER_DIRECTION direction
)
{
    /* Holds Audio Stream Interface array index for the corresponding streaming interface interfaceNum */
    uint8_t streamInfIndex;

    /*Holds audio Control Interface ID of audio function instance iAudio*/
    uint8_t audioControlIntrfcID;

    /*Holds value of active alternate settings for this interface*/
    uint8_t activeAlternateSetting;
    
    uint8_t dataEndpoint;
    
    uint8_t syncEndpoint;

    uint8_t cnt;

    USB_ENDPOINT epStruct;

    OSAL_CRITSECT_DATA_TYPE IntState;

    USB_ERROR irpErr;

    USB_DEVICE_AUDIO_EP_INSTANCE* tempEndpointInstance=NULL;
    
    OSAL_RESULT osalError;

    USB_DEVICE_IRP * irp;

    USB_DEVICE_AUDIO_IRP_DATA *audioIrpData;

    /* Get a pointer to the current USB audio instance that is being addressed*/
    USB_DEVICE_AUDIO_INSTANCE * thisAudioInstance = &gUsbDeviceAudioInstance[iAudio];

    /* Initially send the transfer handle to invalid */
    *transferHandle = USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID;

    /* check the validity of the function driver index */
    if ( (uint32_t)USB_DEVICE_AUDIO_INSTANCES_NUMBER <= iAudio )
    {
        /* invalid handle */
        return USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_INVALID;
    }

    /* Check if user passed valid buffer */
    if ( data == NULL )
    {
        /* Write data should not be null. It should point to valid data location
         return error */
        return USB_DEVICE_AUDIO_RESULT_ERROR_INVALID_BUFFER;
    }

    /* Retrieve Audio Control Interface ID. We are going to use it to locate the Audio streaming
     * Interface array index from the interfaceNum passed to this function */
    audioControlIntrfcID = thisAudioInstance->infCollection.bControlInterfaceNum;

    /*Find out the array streaming interface */
    streamInfIndex = interfaceNum - audioControlIntrfcID- 1U;

    /*Retrieve the active alternate setting of the interface from Audio Instance object */
    activeAlternateSetting = thisAudioInstance->infCollection.streamInf[streamInfIndex].activeSetting;
    
    dataEndpoint = 
            thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoDataEp.epAddr;
    syncEndpoint =
            thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoSyncEp.epAddr;

    if (activeAlternateSetting == 0U)
    {
        SYS_ASSERT ( false , "alternate setting 0 does not allow Data Payload" );
        return USB_DEVICE_AUDIO_RESULT_ERROR_INSTANCE_NOT_CONFIGURED; 
    }

    /* Check if the interface number passed to this function is valid*/
    if (interfaceNum != thisAudioInstance->infCollection.streamInf[streamInfIndex].interfaceNum)
    {
        SYS_ASSERT ( false , "Invalid interface number " );
        return USB_DEVICE_AUDIO_RESULT_ERROR_INVALID_INTERFACE_ID;
    }
    
    /* Checking the direction of the transfer and if the write transfer is for the data endpoint */
    if (direction == USB_DEVICE_AUDIO_WRITE) 
    {
        if (((syncEndpoint & 0x0FU) != 0U) && ((syncEndpoint & (uint8_t )0x80U)!=0U))
        {
            tempEndpointInstance = 
                &(thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoSyncEp);
        }
        
        else if (((dataEndpoint & 0x0FU) != 0U) && ((dataEndpoint & (uint8_t )0x80U) != 0U))
        {
            tempEndpointInstance = 
                &(thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoDataEp);
        }
        else
        {
            /* Do Nothing */
        }
    }
    
    else 
    {
        if(((syncEndpoint & 0x0FU) != 0U) && ((syncEndpoint & (uint8_t )0x80U) == 0U))
        {
            tempEndpointInstance = 
                &(thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoSyncEp);
        }

        else if (((dataEndpoint & 0x0FU) != 0U) && ((dataEndpoint & (uint8_t )0x80U) == 0U))
        {
            tempEndpointInstance = 
                &(thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoDataEp);
        }
        else
        {
            /* Do Nothing */
        }
    }

    /*Obtain mutex to get access to a shared resource, check return value*/
    osalError = OSAL_MUTEX_Lock(&gUSBDeviceAudioCommonDataObj.mutexAUDIOIRP, OSAL_WAIT_FOREVER);
    if(osalError != OSAL_RESULT_TRUE)
    {
      /*Do not proceed lock was not obtained, or error occurred, let user know about error*/
      return (USB_DEVICE_AUDIO_RESULT_ERROR);
    }

    /* Loop and find a free IRP in the Q */
    for ( cnt = 0; cnt < (uint32_t)USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED; cnt ++ )
    {
        /* get the IRP status */
        if(gUSBDeviceAudioIRP[cnt].status <
                (USB_DEVICE_IRP_STATUS)USB_DEVICE_IRP_FLAG_DATA_PENDING)
        {
            irp = &gUSBDeviceAudioIRP[cnt];
            audioIrpData =&gUSBDeviceAudioIrpData[cnt];

            /* Increment the Unique Buffer ID for the request. If the ID reaches
            0xFFFF, we let it roll over. This avoid confusing the generated
            unique buffer handle with a invalid buffer handle. */

            gUSBDeviceAudioUniqueBufferID ++;
            gUSBDeviceAudioUniqueBufferID = (gUSBDeviceAudioUniqueBufferID == 0xFFFFU) ? 0U : gUSBDeviceAudioUniqueBufferID ;

        
        /* Retrieve endpoint address */ 
        epStruct = tempEndpointInstance->epAddr;

        /* Fill IRP object with the pointer to the data that is to be transferred to the Host*/
        irp->data = data;

        /* Fill IRP object with size of the data that is to be transferred to the USB host*/
        irp->size = size;

        /* Save Interface ID */
        audioIrpData->interfaceNum = interfaceNum;

        /* Save Data direction */
        audioIrpData->direction = direction;

        /* Save Audio Function driver instance */
        audioIrpData->iAudio = iAudio;

        /* Provide function address to call back when IRP is complete */
        irp->callback = F_USB_DEVICE_AUDIO_TransferIRPCallBack;

        /* Save array index. We will need this to retrieve data when we get IRP call back */
        irp->userData = (gUSBDeviceAudioUniqueBufferID << 16) | cnt;

        /* Save transfer handle */
        *transferHandle = (USB_DEVICE_AUDIO_TRANSFER_HANDLE) ((gUSBDeviceAudioUniqueBufferID << 16) | cnt);

        if (direction == USB_DEVICE_AUDIO_READ )
        {
            /* Prevent other tasks pre-empting this sequence of code */
            IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
            /* Update the read queue size */
            thisAudioInstance->currentQSizeRead++;
            OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState);
            
        }
        else
        {
            /* Prevent other tasks pre-empting this sequence of code */
            IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
            /* Update the Write queue size */
            thisAudioInstance->currentQSizeWrite++;
            OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState);
            
        }
        /* Submit IRP */ 
        irpErr = USB_DEVICE_IRPSubmit ( thisAudioInstance->devLayerHandle ,
                                            epStruct , irp);
        /* check if IRP submit is success */
        if ( USB_ERROR_NONE != irpErr )
        {
            if (direction == USB_DEVICE_AUDIO_READ )
            {
                /* Prevent other tasks pre-empting this sequence of code */
                IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                /* Update the read queue size */
                thisAudioInstance->currentQSizeRead--;
                OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState);
            }
            else
            {
                /* Prevent other tasks pre-empting this sequence of code */
                IntState = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                /* Update the Write queue size */
                thisAudioInstance->currentQSizeWrite--;
                OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, IntState);
                
            }
                /* Invalidate the Transfer handle.  */
                *transferHandle = USB_DEVICE_AUDIO_TRANSFER_HANDLE_INVALID;
        }

            /*Release mutex, done with shared resource*/
            osalError = OSAL_MUTEX_Unlock(&gUSBDeviceAudioCommonDataObj.mutexAUDIOIRP);
            if(osalError != OSAL_RESULT_TRUE)
            {
                /*Do not proceed lock was not obtained, or error occurred, let user know about error*/
                return (USB_DEVICE_AUDIO_RESULT_ERROR);
            }

            return((USB_DEVICE_AUDIO_RESULT)irpErr);
        }
    }

/*Release mutex, done with shared resource*/
    osalError = OSAL_MUTEX_Unlock(&gUSBDeviceAudioCommonDataObj.mutexAUDIOIRP);
    if(osalError != OSAL_RESULT_TRUE)
    {
    /*Do not proceed, unlock was not complete, or error occurred, 
     * let user know about error*/
        return (USB_DEVICE_AUDIO_RESULT_ERROR);
    }
    
    /* If here means we could not find a spare IRP */
    return(USB_DEVICE_AUDIO_RESULT_ERROR_TRANSFER_QUEUE_FULL);   
}

#pragma coverity compliance end_block "MISRA C-2012 Rule 10.4"
#pragma GCC diagnostic pop
/* MISRAC 2012 deviation block end */
/*******************************************************************************
 End of File
 */
