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

/*******************************************************************************
********************************************************************************
   Section: Included Files
********************************************************************************
********************************************************************************/
#include "usb/usb_device_audio_v2_0.h"
#include "usb/src/usb_device_audio_v2_local.h"



/*******************************************************************************
********************************************************************************
   Section: File Scope or Global Data Types
********************************************************************************
********************************************************************************/

/*******************************************************************************/
/* Audio function instance objects.

  Summary:
    Holds all of the variables required by audio instance.

  Description:
    This structure holds all of the variables required by audio instance.

  Remarks:
    None.
*/

USB_DEVICE_AUDIO_V2_INSTANCE 
                gUsbDeviceAudioV2Instance[USB_DEVICE_AUDIO_V2_INSTANCES_NUMBER];

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

USB_DEVICE_IRP gUSBDeviceAudioV2IRP[USB_DEVICE_AUDIO_V2_QUEUE_DEPTH_COMBINED];

/* Create a variable for holding Audio IRP mutex Handle and status */
USB_DEVICE_AUDIO_V2_COMMON_DATA_OBJ gUSBDeviceAudioV2CommonDataObj;

/*******************************************************************************/
/* AUDIO Device IRP Data

  Summary:
    Array of AUDIO Device IRP Data.

  Description:
    Array of AUDIO Device IRP. This array of IRP will be shared by read, write and
    notification data requests.

  Remarks:
    This array is private to the USB stack.
 */

USB_DEVICE_AUDIO_V2_IRP_DATA 
            gUSBDeviceAudioV2IrpData [USB_DEVICE_AUDIO_V2_QUEUE_DEPTH_COMBINED];

/*******************************************************************************
  AUDIO Device IRP Unique Identifier

  Summary:
    USB Device Audio Unique Buffer Identifier.

  Description:
    This global variable is used along with the buffer index to create a unique
    buffer handle for each Audio Buffer Request.

  Remarks:
    Private to the USB Audio Function Driver.
 */

static uint32_t gUSBDeviceAudioUniqueBufferID = 0;

/*******************************************************************************
  Function:
    USB_DEVICE_AUDIO_V2_RESULT USB_DEVICE_AUDIO_V2_EventHandlerSet
    (
        USB_DEVICE_AUDIO_V2_INDEX instanceIndex  ,
        USB_DEVICE_AUDIO_V2_EVENT_HANDLER eventHandler ,
        uintptr_t context
    );
    
  Summary:
    This function registers an event handler for the specified Audio function
    driver instance. 

  Description:
  Refer  usb_device_audio_v2_0.h for detailed description of this function. 
  
  */ 

USB_DEVICE_AUDIO_V2_RESULT USB_DEVICE_AUDIO_V2_EventHandlerSet
(
    USB_DEVICE_AUDIO_V2_INDEX instanceIndex  ,
    USB_DEVICE_AUDIO_V2_EVENT_HANDLER eventHandler ,
    uintptr_t context
)
{
    USB_DEVICE_AUDIO_V2_RESULT error = USB_DEVICE_AUDIO_V2_RESULT_ERROR_PARAMETER_INVALID;

    if(eventHandler != NULL)
    {
        gUsbDeviceAudioV2Instance[instanceIndex ].appEventCallBack = eventHandler;
        gUsbDeviceAudioV2Instance[instanceIndex ].userData = context;
        error = USB_DEVICE_AUDIO_V2_RESULT_OK;
    }
    return error;
}

/*******************************************************************************
  Function:
    USB_DEVICE_AUDIO_V2_RESULT USB_DEVICE_AUDIO_V2_Read 
                                      ( USB_DEVICE_AUDIO_V2_INDEX instanceIndex ,
                                        uint8_t interfaceNumber ,
                                        void * data ,
                                        size_t size )

  Summary:
    Reads the data received from the Audio Interface for
    specified instance of the USB device layer.

  Description:
    Reads the data received from the Audio Interface for
    specified instance of the USB device layer.

  Parameters:
    USB_DEVICE_AUDIO_V2_INDEX instanceIndex    - Audio function driver Index number
 
    uint8_t interfaceNumber    - Audio streaming or Control Interface number
  
    data - pointer to the data buffer where read data will be stored.
    size - Size of the data buffer. Refer to the description section for more
           details on how the size affects the transfer.
 
  Returns:
    USB_DEVICE_AUDIO_V2_RESULT

*/

USB_DEVICE_AUDIO_V2_RESULT USB_DEVICE_AUDIO_V2_Read
(
    USB_DEVICE_AUDIO_V2_INDEX instanceIndex ,
    USB_DEVICE_AUDIO_V2_TRANSFER_HANDLE* transferHandle,
    uint8_t interfaceNumber ,
    void * data ,
    size_t size
)
{
        USB_DEVICE_AUDIO_V2_RESULT audioResult;
        USB_DEVICE_AUDIO_V2_INSTANCE * thisAudioDevice;
        thisAudioDevice = &gUsbDeviceAudioV2Instance[instanceIndex];

         /* Make sure that we are with in the queue size for this instance */
        if(thisAudioDevice->currentQSizeRead >= thisAudioDevice->queueSizeRead)
        {
            SYS_ASSERT(false, "Read Queue is full");
            return(USB_DEVICE_AUDIO_V2_RESULT_ERROR_TRANSFER_QUEUE_FULL);
        }
    audioResult =   F_USB_DEVICE_AUDIO_V2_Transfer(instanceIndex, transferHandle, 
                           interfaceNumber, data, size, USB_DEVICE_AUDIO_V2_READ );
        return audioResult; 
}


/*******************************************************************************

********************************************************************************
********************************************************************************
  Function:
    USB_ERROR USB_DEVICE_AUDIO_V2_Write 
                          ( USB_DEVICE_AUDIO_V2_INDEX instanceIndex ,
                            uint8_t interfaceNumber ,
                            USB_DEVICE_AUDIO_V2_DATA_BUFFER_OBJECT* bufferObj )

  Summary:
    sends to the Audio Interface for
    the specified instance of the USB device layer.

  Description:
    sends to the Audio Interface for
    the specified instance of the USB device layer.

   Parameters:
    USB_DEVICE_AUDIO_V2_INDEX instanceIndex    - Audio function driver Index number
 
    uint8_t interfaceNumber    - Audio streaming or Control Interface number
 
    USB_DEVICE_AUDIO_V2_DATA_BUFFER_OBJECT* bufferObj - pointer to the buffer 
    where received data is to be stored.
 
  Returns:
    USB_DEVICE_AUDIO_V2_RESULT

 */

USB_DEVICE_AUDIO_V2_RESULT USB_DEVICE_AUDIO_V2_Write
(
    USB_DEVICE_AUDIO_V2_INDEX instanceIndex ,
    USB_DEVICE_AUDIO_V2_TRANSFER_HANDLE* transferHandle,
    uint8_t interfaceNumber ,
    void * data ,
    size_t size
)
{
    USB_DEVICE_AUDIO_V2_RESULT audioResult;
    USB_DEVICE_AUDIO_V2_INSTANCE * thisAudioDevice;
    thisAudioDevice = &gUsbDeviceAudioV2Instance[instanceIndex];

     /* Make sure that we are with in the queue size for this instance */
    if(thisAudioDevice->currentQSizeWrite >= thisAudioDevice->queueSizeWrite)
    {
        SYS_ASSERT(false, "Write Queue is full");
        return(USB_DEVICE_AUDIO_V2_RESULT_ERROR_TRANSFER_QUEUE_FULL);
    }
    audioResult =   F_USB_DEVICE_AUDIO_V2_Transfer(instanceIndex, transferHandle, 
                          interfaceNumber, data, size, USB_DEVICE_AUDIO_V2_WRITE );
    return audioResult;
}
/*******************************************************************************/


/*******************************************************************************
  Function:
    USB_DEVICE_AUDIO_V2_RESULT F_USB_DEVICE_AUDIO_V2_Transfer
    (
        USB_DEVICE_AUDIO_V2_INDEX iAudio,
        USB_DEVICE_AUDIO_V2_TRANSFER_HANDLE *transferHandle,
        uint8_t interfaceNum ,
        void * data ,
        size_t size,
        USB_DEVICE_AUDIO_V2_TRANSFER_DIRECTION direction
    ); 
    
  Summary:
    This function schedules an Audio transfer. This is a local function and should
    be called by applications directly. 

********************************************************************************/

USB_DEVICE_AUDIO_V2_RESULT F_USB_DEVICE_AUDIO_V2_Transfer
(
    USB_DEVICE_AUDIO_V2_INDEX iAudio,
    USB_DEVICE_AUDIO_V2_TRANSFER_HANDLE *transferHandle,
    uint8_t interfaceNum ,
    void * data ,
    size_t size,
    USB_DEVICE_AUDIO_V2_TRANSFER_DIRECTION direction
)
{
    /* Holds Audio Stream Interface array index for the corresponding streaming 
     * interface interfaceNum */
    uint8_t streamInfIndex;

    /*Holds audio Control Interface ID of audio function instance iAudio*/
    uint8_t audioControlIntrfcID;

    /*Holds value of active alternate settings for this interface*/
    uint8_t activeAlternateSetting;
    
    uint8_t dataEndpoint;
    
    uint8_t syncEndpoint;

    uint8_t cnt;

    USB_ENDPOINT epStruct;

    USB_ERROR irpErr;

    USB_DEVICE_AUDIO_V2_EP_INSTANCE* tempEndpointInstance=NULL;
    
    OSAL_RESULT osalError;

    OSAL_CRITSECT_DATA_TYPE status;

    USB_DEVICE_IRP * irp;

    USB_DEVICE_AUDIO_V2_IRP_DATA *audioIrpData;
    
    /* Get a pointer to the current USB audio instance that is being addressed*/
    USB_DEVICE_AUDIO_V2_INSTANCE * thisAudioInstance = &gUsbDeviceAudioV2Instance[iAudio];

    /* Initially send the transfer handle to invalid */
    *transferHandle = USB_DEVICE_AUDIO_V2_TRANSFER_HANDLE_INVALID;

    /* check the validity of the function driver index */
    if ( USB_DEVICE_AUDIO_V2_INSTANCES_NUMBER <= iAudio )
    {
        /* invalid handle */
        return USB_DEVICE_AUDIO_V2_RESULT_ERROR_INSTANCE_INVALID;
    }

    /* Check if user passed valid buffer */
    if ( data == NULL )
    {
        /* Write data should not be null. It should point to valid data location
         return error */
        return USB_DEVICE_AUDIO_V2_RESULT_ERROR_INVALID_BUFFER;
    }

    /* Retrieve Audio Control Interface ID. We are going to use it to locate the Audio streaming
     * Interface array index from the interfaceNum passed to this function */
    audioControlIntrfcID = thisAudioInstance->infCollection.bControlInterfaceNum;

    /*Find out the array streaming interface */
    streamInfIndex = interfaceNum - audioControlIntrfcID- 1U;

    /*Retrieve the active alternate setting of the interface from Audio Instance object */
    activeAlternateSetting = thisAudioInstance->infCollection.streamInf[streamInfIndex].activeSetting;

    if (activeAlternateSetting == 0U)
    {
        SYS_ASSERT ( false , "alternate setting 0 does not allow Data Payload" );
        return USB_DEVICE_AUDIO_V2_RESULT_ERROR_INSTANCE_NOT_CONFIGURED;
    }
    
    dataEndpoint = 
            thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoDataEp.epAddr;
    syncEndpoint =
            thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoSyncEp.epAddr;

    /* Check if the interface number passed to this function is valid*/
    if (interfaceNum != thisAudioInstance->infCollection.streamInf[streamInfIndex].interfaceNum)
    {
        SYS_ASSERT ( false , "Invalid interface number " );
        return USB_DEVICE_AUDIO_V2_RESULT_ERROR_INVALID_INTERFACE_ID;
    }

    if (direction == USB_DEVICE_AUDIO_V2_WRITE) 
    {
        /* Check if the sync endpoint if valid*/
        if (((syncEndpoint & 0x0FU) != 0U) && ((syncEndpoint & (uint8_t )0x80) != 0U))
        {
            tempEndpointInstance = 
                &(thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoSyncEp);
        }
        
        /* Check if the data endpoint if valid*/
        else if (((dataEndpoint & 0x0FU)!= 0U) && ((dataEndpoint & (uint8_t )0x80) != 0U))
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
        /* Check if the sync endpoint if valid*/
        if(((syncEndpoint & 0x0FU) != 0U) && ((syncEndpoint & (uint8_t )0x80) == 0U))
        {
            tempEndpointInstance = 
                &(thisAudioInstance->infCollection.streamInf[streamInfIndex].alterntSetting[activeAlternateSetting].isoSyncEp);
        }

        /* Check if the data endpoint if valid*/
        else if (((dataEndpoint & 0x0FU) != 0U) && ((dataEndpoint & (uint8_t )0x80) == 0U))
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
    osalError = OSAL_MUTEX_Lock(&gUSBDeviceAudioV2CommonDataObj.mutexAUDIOIRP, OSAL_WAIT_FOREVER);
    if(osalError != (OSAL_RESULT)OSAL_RESULT_TRUE)
    {
      /*Do not proceed lock was not obtained, or error occurred, let user know about error*/
      return (USB_DEVICE_AUDIO_V2_RESULT_ERROR);
    }

    /* Loop and find a free IRP in the Q */
    for ( cnt = 0; cnt < USB_DEVICE_AUDIO_V2_QUEUE_DEPTH_COMBINED; cnt ++ )
    {
        /* get the IRP status */
        if(gUSBDeviceAudioV2IRP[cnt].status <
                (USB_DEVICE_IRP_STATUS)USB_DEVICE_IRP_FLAG_DATA_PENDING)
        {
            irp = &gUSBDeviceAudioV2IRP[cnt];
            audioIrpData =&gUSBDeviceAudioV2IrpData[cnt];

            /* Increment the Unique Buffer ID for the request. If the ID reaches
            *  0xFFFF, we let it roll over. This avoid confusing the generated
            *  unique buffer handle with a invalid buffer handle. */

            gUSBDeviceAudioUniqueBufferID ++;
            gUSBDeviceAudioUniqueBufferID = 
                    (gUSBDeviceAudioUniqueBufferID == 0xFFFFU) ? 
                                             0U : gUSBDeviceAudioUniqueBufferID ;

        
            /* Retrieve endpoint address */
            epStruct = tempEndpointInstance->epAddr;

            /* Fill IRP object with the pointer to the data that is to be 
             * transferred to the Host*/
            irp->data = data;

            /* Fill IRP object with size of the data that is to be transferred 
             * to the USB host*/
            irp->size = size;

            /* Save Interface ID */
            audioIrpData->interfaceNum = interfaceNum;

            /* Save Data direction */
            audioIrpData->direction = direction;

            /* Save Audio Function driver instance */
            audioIrpData->iAudio = iAudio;

            /* Provide function address to call back when IRP is complete */
            irp->callback = F_USB_DEVICE_AUDIO_V2_TransferIRPCallBack;

            /* Save array index. We will need this to retrieve data when we get 
             * IRP call back */
            irp->userData = (gUSBDeviceAudioUniqueBufferID << 16) | cnt;

            /* Save transfer handle */
            *transferHandle = (USB_DEVICE_AUDIO_V2_TRANSFER_HANDLE) ((gUSBDeviceAudioUniqueBufferID << 16) | cnt);

            if (direction == USB_DEVICE_AUDIO_V2_READ )
            {
                status = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                thisAudioInstance->currentQSizeRead++;
                OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, status);
            }
            else
            {
                status = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                thisAudioInstance->currentQSizeWrite++;
                OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, status);
            }
            /* Submit IRP */
            irpErr = USB_DEVICE_IRPSubmit ( thisAudioInstance->devLayerHandle ,
                                                epStruct , irp);
            /* check if IRP submit is success */
            if ( USB_ERROR_NONE != irpErr )
            {
                if (direction == USB_DEVICE_AUDIO_V2_READ )
                {
                    status = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                    thisAudioInstance->currentQSizeRead--;
                    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, status);
                }
                else
                {
                    status = OSAL_CRIT_Enter(OSAL_CRIT_TYPE_HIGH);
                    thisAudioInstance->currentQSizeWrite--;
                    OSAL_CRIT_Leave(OSAL_CRIT_TYPE_HIGH, status);
                }
                /* Invalidate the Transfer handle.  */
                *transferHandle = USB_DEVICE_AUDIO_V2_TRANSFER_HANDLE_INVALID;
            }

            /*Release mutex, done with shared resource*/
            osalError = OSAL_MUTEX_Unlock(
                                 &gUSBDeviceAudioV2CommonDataObj.mutexAUDIOIRP);
            if(osalError != (OSAL_RESULT)OSAL_RESULT_TRUE)
            {
                /*Do not proceed, unlock was not complete, or error occurred, 
                 * let user know about error*/
                return (USB_DEVICE_AUDIO_V2_RESULT_ERROR);
            }

            return((USB_DEVICE_AUDIO_V2_RESULT)irpErr);
    }
    }
    /*Release mutex, done with shared resource*/
    osalError = OSAL_MUTEX_Unlock(&gUSBDeviceAudioV2CommonDataObj.mutexAUDIOIRP);
    if(osalError != (OSAL_RESULT)OSAL_RESULT_TRUE)
    {
    /*Do not proceed, unlock was not complete, or error occurred, 
     * let user know about error*/
        return (USB_DEVICE_AUDIO_V2_RESULT_ERROR);
    }
    /* If here means we could not find a spare IRP */
    return(USB_DEVICE_AUDIO_V2_RESULT_ERROR_TRANSFER_QUEUE_FULL);
}

/*******************************************************************************
 End of File
 */
