/*******************************************************************************
 USB Audio Class Function Driver

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_audio_v2_0.c

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

/*******************************************************************************
   Function:
    void F_USB_DEVICE_AUDIO_V2_IRPCancelAll(USB_DEVICE_AUDIO_V2_INDEX iAudio )
   Summary:
    This function cancels all pending Read and Write requests placed to
    USB Device Audio function driver.

   Description:
    This function cancels all pending Read and Write requests placed to
    USB Device Audio function driver.
   Precondition:
    The function driver should have been configured.
   Parameters:
    iAudio - USB Device Audio Function Driver instance.

   Returns:
    None
   Example:

   Remarks:
     None
 */

void F_USB_DEVICE_AUDIO_V2_IRPCancelAll(USB_DEVICE_AUDIO_V2_INDEX iAudio );

/*******************************************************************************
  AUDIO Device function driver structure

  Summary:
    Defines the function driver structure required for the device layer.

  Description:
    This data type defines the function driver structure required for the
    device layer.

  Remarks:
    This structure is private to the USB stack.
 */
const USB_DEVICE_FUNCTION_DRIVER audioV2FunctionDriver =
{

    /* AUDIO init function */
    .initializeByDescriptor             = &F_USB_DEVICE_AUDIO_V2_Initialize ,

    /* AUDIO de-init function */
    .deInitialize           = &F_USB_DEVICE_AUDIO_V2_Deinitialize ,

    /* AUDIO set-up packet handler */
    .controlTransferNotification     = &F_USB_DEVICE_AUDIO_V2_ControlTransferHandler,

    /* AUDIO tasks function */
    .tasks                  = NULL,

    /* Audio Global Initialize function */
    .globalInitialize = F_USB_DEVICE_AUDIO_V2_GlobalInitialize
        
};

/*******************************************************************************
********************************************************************************/


/*******************************************************************************
   Function:
   void F_USB_DEVICE_AUDIO_V2_ControlTransferHandler
   (
       SYS_MODULE_INDEX iAudio,
       USB_DEVICE_EVENT controlEvent,
       USB_SETUP_PACKET * controlEventData
   )

  Summary:
    Audio control transfer handler.

  Description:
    Audio control transfer handler. This is the callback the device layer calls
    when there is a SETUP packet that is targeted to this particular instance
    of Audio.

  Returns:
    This is a local function and should not be called directly by the
    application.
*/

void F_USB_DEVICE_AUDIO_V2_ControlTransferHandler
(
    SYS_MODULE_INDEX iAudio,
    USB_DEVICE_EVENT controlEvent,
    USB_SETUP_PACKET * controlEventData
)
{
    /* Obtain pointer to the Audio Instance that is being addressed*/
    USB_DEVICE_AUDIO_V2_INSTANCE *thisAudioInstance = 
                                             &gUsbDeviceAudioV2Instance[iAudio];

    /* check the validity of the function driver index */
    if ( USB_DEVICE_AUDIO_V2_INSTANCES_NUMBER <= iAudio )
    {
        /* invalid handle */
        SYS_ASSERT (false, "invalid Audio Index");
    }

    /* Check Control Event  */
    switch(controlEvent)
    {
        case USB_DEVICE_EVENT_CONTROL_TRANSFER_SETUP_REQUEST:
            /* A new SETUP packet received from Host*/
            F_USB_DEVICE_AUDIO_V2_SetupPacketHandler(iAudio,controlEventData);
            break;

        case USB_DEVICE_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:
            /* A Control Transfer data stage completed. Notify the application */
            thisAudioInstance->appEventCallBack
            (
                iAudio,
                USB_DEVICE_AUDIO_V2_EVENT_CONTROL_TRANSFER_DATA_RECEIVED,
                NULL,
                0
            );        
            break;

        case USB_DEVICE_EVENT_CONTROL_TRANSFER_DATA_SENT:
            /* The Control Transfer data was sent the Host. 
             * Notify the application */
            thisAudioInstance->appEventCallBack
            (
                iAudio,
                USB_DEVICE_AUDIO_V2_EVENT_CONTROL_TRANSFER_DATA_SENT,
                NULL,
                0
            );
            break;
        default:
            /* Do Nothing */
            break;
    }
}

/*******************************************************************************
   Function:
   void F_USB_DEVICE_AUDIO_V2_SetupPacketHandler
   (
       USB_DEVICE_AUDIO_V2_INDEX iAudio ,
       USB_SETUP_PACKET * controlEventData
   )

  Summary:
    Handles a fresh SETUP packet received from Host.

  Description:
    Handles a fresh SETUP packet received from Host.

  Returns:
    This is a local function and should not be called directly by the
    application.
*/

void F_USB_DEVICE_AUDIO_V2_SetupPacketHandler
(
    USB_DEVICE_AUDIO_V2_INDEX iAudio ,
    USB_SETUP_PACKET * controlEventData
)
{
    uint8_t audioControlInterfaceId;
    uint8_t interfaceId;
    uint8_t curAlternateSetting;
    uint8_t prevAlternateSetting;
    uint8_t streamIntfcIndex;
    uint8_t noOfEndpoints;
    USB_ENDPOINT ep; 
    uint16_t maxPacketSize;
    USB_DEVICE_HANDLE usbDeviceHandle;
    USB_DEVICE_AUDIO_V2_EVENT_DATA_SET_ALTERNATE_INTERFACE interfaceInfo;
    USB_ERROR endpointEnableResult;
    USB_ERROR endpointDisableResult; 
    USB_DEVICE_AUDIO_V2_STREAMING_INTERFACE_ALTERNATE_SETTING *pCurAlternateStng;
    USB_DEVICE_AUDIO_V2_STREAMING_INTERFACE_ALTERNATE_SETTING *pPrevAlternateStng;
    USB_DEVICE_AUDIO_V2_STREAMING_INTERFACE *pStreamingInterface;
    
    uint16_t adjustedMaxPacketSize =1;

    /* Obtain pointer to the Audio Instance that is being addressed*/
    USB_DEVICE_AUDIO_V2_INSTANCE* audioInstance = 
                                             &gUsbDeviceAudioV2Instance[iAudio];

    /* Obtain pointer to the Audio Interface collection*/
    USB_DEVICE_AUDIO_V2_INTERFACE_COLLECTION *curInfCollection =
                                                &(audioInstance->infCollection);

    /* Get the Device Layer handle */
    usbDeviceHandle = gUsbDeviceAudioV2Instance[iAudio].devLayerHandle;
    
    /* Check if the request is a standard interface request*/
    if (  (controlEventData->RequestType == (uint8_t)USB_SETUP_REQUEST_TYPE_STANDARD)
       && (controlEventData->Recipient == (uint8_t)USB_SETUP_REQUEST_RECIPIENT_INTERFACE))

    {
        /* We have received Standard Set request */

        /* retrieve interface number from the Setup packet */
        interfaceId = controlEventData->bIntfID;

        /* retrieve audio Control interface number*/
        audioControlInterfaceId = curInfCollection->bControlInterfaceNum;
        
        switch(controlEventData->bRequest)
        {
            case USB_REQUEST_SET_INTERFACE:
                curAlternateSetting = controlEventData->bAltID;
                if (interfaceId == audioControlInterfaceId)
                {
                    /*SET INTERFACE command was received to Audio Control
                      Interface */
                    curInfCollection->bControlAltSetng = curAlternateSetting;
                }
                else
                {
                    /*An Audio Streaming interface has received SET INTERFACE
                      command */
                    streamIntfcIndex = interfaceId - audioControlInterfaceId - 1U;

                    /* Get pointer to the current audio streaming interface */
                    pStreamingInterface = 
                               &(curInfCollection->streamInf[streamIntfcIndex]);
                    
                    /* Get pointer to the Interface Alternate setting. */
                    pCurAlternateStng = 
                    &(pStreamingInterface->alterntSetting[curAlternateSetting]);
                    
                    /* Find out how many endpoint are present for this Interface
                     * Alternate setting */
                    noOfEndpoints = pCurAlternateStng->numEndPoints;

                    if ((noOfEndpoints != 0U) && (curAlternateSetting != 0U))
                    {
                        /* We have to enable the endpoint only if this alternate
                        * setting has at least one endpoint and the alternate
                        * setting is a non zero value */
                        
                        /*Retrieve endpoint size */
                        ep = pCurAlternateStng->isoDataEp.epAddr;
                        
                        /* retrieve max packet size. */
                        maxPacketSize
                             = pCurAlternateStng->isoDataEp.epMaxPacketSize;

                        /* maxpacket size should be adjusted to upper power of
                        *  two. As per the PIC32MZ USB module requirement.*/
                        if (maxPacketSize != 0U)
                        {
                            while(adjustedMaxPacketSize < maxPacketSize)
                            {
                                adjustedMaxPacketSize <<= 1;
                            }
                        }
                        else
                        {
                            adjustedMaxPacketSize = 0;
                        }

                        /* Enable Isochronous Data Endpoint */
                        endpointEnableResult = (USB_ERROR)USB_DEVICE_EndpointEnable
                        (
                            usbDeviceHandle ,
                            0,
                            ep ,
                            USB_TRANSFER_TYPE_ISOCHRONOUS ,
                            adjustedMaxPacketSize
                        );
                        if (endpointEnableResult != USB_ERROR_NONE)
                        {
                            SYS_ASSERT (false, "Endpoint not Enabled");
                        }

                        if (noOfEndpoints == 2U)
                        {
                            adjustedMaxPacketSize =1;

                            /* If number of Endpoints is Two, then it is sure
                             * that this alternate setting reports a Isochronous
                             * Sync Endpoint. Enable the Sync Endpoint. */

                             /*Retrieve endpoint size */
                             ep = pCurAlternateStng->isoSyncEp.epAddr;

                             /* retrieve max packet size. */
                             maxPacketSize
                             = pCurAlternateStng->isoSyncEp.epMaxPacketSize;

                             /* maxpacket size should be adjusted to upper power of
                             two. As per the PIC32MZ USB module requirement.*/
                             if (maxPacketSize != 0U)
                             {
                                   while(adjustedMaxPacketSize < maxPacketSize)
                                   {
                                       adjustedMaxPacketSize <<= 1;
                                   }
                              }
                              else
                              {
                                   adjustedMaxPacketSize = 0;
                              }
                               /* Enable Isochronous Data Endpoint */
                               endpointEnableResult = (USB_ERROR)USB_DEVICE_EndpointEnable
                               (
                                   usbDeviceHandle ,
                                   0,
                                   ep ,
                                   USB_TRANSFER_TYPE_ISOCHRONOUS ,
                                   adjustedMaxPacketSize
                               );
                               if (endpointEnableResult != USB_ERROR_NONE)
                               {
                                   SYS_ASSERT (false, "Endpoint not Enabled");
                               }
                        }

                        /* Change Audio Instance object state to Initialized. */
                        pStreamingInterface->state
                                    = USB_DEVICE_AUDIO_V2_STRMNG_INTFC_INITIALIZED;
                    }
                    else /* alternateSetting  = 0 */
                    {
                        if (pStreamingInterface->state
                                   == USB_DEVICE_AUDIO_V2_STRMNG_INTFC_INITIALIZED)
                        {
                            /* Disable the Endpoint in the previous Active 
                             * Alternate setting. */
                            prevAlternateSetting = 
                                             pStreamingInterface->activeSetting;

                            /* get a pointer to the previous alternate setting.*/
                            pPrevAlternateStng = 
                            &(pStreamingInterface->alterntSetting[prevAlternateSetting]);

                            /*Get endpoint no of the previous alternate setting*/
                            ep = pPrevAlternateStng->isoDataEp.epAddr;

                            /* Disable the Endpoint */
                            (void) USB_DEVICE_IRPCancelAll(usbDeviceHandle,  ep);
                            endpointDisableResult = (USB_ERROR)USB_DEVICE_EndpointDisable
                                                    (
                                                        usbDeviceHandle,
                                                        ep
                                                    );
                            if (endpointDisableResult != USB_ERROR_NONE)
                            {
                                SYS_ASSERT (false, "Endpoint not Disabled");
                            }


                            if (pPrevAlternateStng->numEndPoints == 2U)
                            {
                                /* If number of Endpoints is Two, then it is sure
                                * that this alternate setting reports a Isochronous
                                * Sync Endpoint. Disable the Sync Endpoint. */
								ep = pPrevAlternateStng->isoSyncEp.epAddr;

                                /* Disable the Endpoint */
                                (void) USB_DEVICE_IRPCancelAll(usbDeviceHandle,  ep);
                                endpointDisableResult =(USB_ERROR)USB_DEVICE_EndpointDisable
                                        (
                                            usbDeviceHandle,
                                            pPrevAlternateStng->isoSyncEp.epAddr
                                        );
                                if (endpointDisableResult != USB_ERROR_NONE)
                                {
                                    SYS_ASSERT (false, "Endpoint not Disabled");
                                }
                            }
                            curInfCollection->streamInf[streamIntfcIndex].state
                                = USB_DEVICE_AUDIO_V2_STRMNG_INTFC_NOT_INITIALIZED;
                        }  
                    }

                    /* Remember the new alternate setting received */
                    curInfCollection->streamInf[streamIntfcIndex].activeSetting 
                    = curAlternateSetting;


                    /* Notify application about the SET_INTERFACE request */
                    interfaceInfo.interfaceNumber = interfaceId;
                    interfaceInfo.interfaceAlternateSetting = curAlternateSetting;
                    audioInstance->appEventCallBack
                    (
                        iAudio,
                        USB_DEVICE_AUDIO_V2_EVENT_INTERFACE_SETTING_CHANGED,
                        &interfaceInfo,
                        0
                    );
                }
                /* Send an Acknowledgement to the Host */ 
                (void) USB_DEVICE_ControlStatus( usbDeviceHandle,
                                USB_DEVICE_CONTROL_STATUS_OK);
            break;
            case USB_REQUEST_GET_INTERFACE:
                if (interfaceId == audioControlInterfaceId)
                {
                    curAlternateSetting = curInfCollection->bControlAltSetng;
                }
                else
                {
                    streamIntfcIndex = interfaceId - audioControlInterfaceId - 1U;
                    curAlternateSetting
                    = curInfCollection->streamInf[streamIntfcIndex].activeSetting;
                }

                (void) USB_DEVICE_ControlSend( usbDeviceHandle,
                        (void *)&curAlternateSetting, 1);
                break;
            default:
                /* Do Nothing */
                break;

        }      
    }/* end of if((controlEventData->bmRequestType == */
    else if ( (controlEventData->RequestType == (uint8_t)USB_SETUP_REQUEST_TYPE_CLASS )
            && (controlEventData->Recipient == (uint8_t)USB_SETUP_REQUEST_RECIPIENT_INTERFACE))
    {
        if ( audioInstance->appEventCallBack != NULL)
        {
            /* inform the application about the request */
            /* the application needs to handle both EP and entity specific 
            *  requests */

            if(controlEventData->bRequest == AUDIO_V2_CUR)
            {
                audioInstance->appEventCallBack
                (
                    iAudio,
                    USB_DEVICE_AUDIO_V2_CUR_ENTITY_SETTINGS_RECEIVED,
                    controlEventData,
                    0
                );
            }
            else if(controlEventData->bRequest == AUDIO_V2_RANGE)
            {
                audioInstance->appEventCallBack
                (
                    iAudio,
                    USB_DEVICE_AUDIO_V2_RANGE_ENTITY_SETTINGS_RECEIVED,
                    controlEventData,
                    0
                );

            }
            else
            {
                /* Do Nothing */
            }
        }        
    }
    else
    {
        /* Do Nothing */
    }
}

/*******************************************************************************/


/*******************************************************************************
  Function:
    void F_USB_DEVICE_AUDIO_V2_GlobalInitialize ( void )

  Summary:
    This function initializes resourses required common to all instances of AUDIO
    function driver.

  Description:
    This function initializes resourses common to all instances of AUDIO function
    driver. This function is called by the USB Device layer during Initalization.

  Remarks:
    This is local function and should not be called directly by the application.
*/

void F_USB_DEVICE_AUDIO_V2_GlobalInitialize (void)
{
    OSAL_RESULT osal_err;

    /* Create Mutex for CDC IRP objects if not created already */
    if (gUSBDeviceAudioV2CommonDataObj.isMutexAudioIrpInitialized == false)
    {
        /* This means that mutexes where not created. Create them. */
        osal_err = 
               OSAL_MUTEX_Create(&gUSBDeviceAudioV2CommonDataObj.mutexAUDIOIRP);

        if(osal_err != (OSAL_RESULT)OSAL_RESULT_TRUE)
        {
            /*do not proceed lock was not created, let user know about error*/
            return;
        }

         /* Set this flag so that global mutexes get allocated only once */
        gUSBDeviceAudioV2CommonDataObj.isMutexAudioIrpInitialized = true;
    }
}

/* MISRA C-2012 Rule 11.3 deviated:4 Deviation record ID -  H3_MISRAC_2012_R_11_3_DR_1 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma coverity compliance block deviate:4 "MISRA C-2012 Rule 11.3" "H3_MISRAC_2012_R_11_3_DR_1" 
/*******************************************************************************
   This function is called by the device layer. It gets called multiple times as 
*  and when the USB device layer starts parsing the descriptors */

void F_USB_DEVICE_AUDIO_V2_Initialize
(
    SYS_MODULE_INDEX iAudio, 
    DRV_HANDLE usbDeviceHandle,
    void* funcDriverInit, 
    uint8_t interfaceNumber, 
    uint8_t alternateSetting,
    uint8_t descriptorType, 
    uint8_t * pDescriptor 
)
{
    /*pointer to Standard Endpoint Descriptor */
    USB_ENDPOINT_DESCRIPTOR *pEPDesc;

    /* pointer to Standard Interface descriptor */
    USB_INTERFACE_DESCRIPTOR *pStdInfDesc;
    
    /* pointer to Class Specific Interface Descriptor */
    USB_AUDIO_V2_CS_AC_INTERFACE_HEADER_DESCRIPTOR *pCsInfDesc;

    USB_DEVICE_AUDIO_V2_INIT * audioInit;
    uint8_t epAddressAndDirection;
    uint16_t maxPacketSize;
    uint8_t cnt;
    uint8_t audioControlInterfaceId;
    uint8_t strmIntrfcIndex;
    USB_DEVICE_AUDIO_V2_STREAMING_INTERFACE_ALTERNATE_SETTING *curAlternateStng;
    USB_DEVICE_AUDIO_V2_INSTANCE * audioInstance = &gUsbDeviceAudioV2Instance[iAudio];
    USB_DEVICE_AUDIO_V2_INTERFACE_COLLECTION *curInfCollection =
            &(audioInstance->infCollection);
    audioInit = ((USB_DEVICE_AUDIO_V2_INIT *)funcDriverInit);
    audioInstance->queueSizeRead = audioInit->queueSizeRead;
    audioInstance->queueSizeWrite = audioInit->queueSizeWrite;
    audioInstance->currentQSizeRead = 0;
    audioInstance->currentQSizeWrite = 0; 

     /* check the type of descriptor passed by device layer */
    switch ( descriptorType )
    { 
        case USB_DESCRIPTOR_INTERFACE: //Interface Descriptor
            
            /*save the device handle to the corresponding Audio Instance object*/
            audioInstance->devLayerHandle = usbDeviceHandle;

            /* save the Audio Index to the corresponding Audio Instance object */
            audioInstance->audioIndex = iAudio;

            pStdInfDesc = (USB_INTERFACE_DESCRIPTOR*) pDescriptor;

            /* check if it is USB audio interface descriptor*/
            if (pStdInfDesc->bInterfaceClass == USB_AUDIO_V2_CLASS_CODE)
            {
                /*Check if it is an Audio Control Interface descriptor*/
                switch (pStdInfDesc->bInterfaceSubClass)
                {
                    case (uint8_t)USB_AUDIO_V2_AUDIOCONTROL:
                        audioInstance->flags.allFlags = 0;
                        /* The program control reached here means that the 
                         * device layer has detected an Standard Audio Control 
                         * interface descriptor. Now we have to save all the 
                         * fields present in the Standard Audio Control 
                         * interface descriptor to the corresponding Audio 
                         * Instance Object. */

                        /*save the Interface number to the corresponding Audio 
                            Instance Object.*/
                        curInfCollection->bControlInterfaceNum = 
                                                  pStdInfDesc->bInterfaceNumber;

                        /* save the no of endpoints present in the Audio Control 
                         * Interface. If there are endpoints present as part of 
                         * AC interface descriptor that must be an 
                         * Interrupt endpoint.*/
                        curInfCollection->isIntEpExists = ( pStdInfDesc->bNumEndPoints != 0U);
                        break;
                
                
                    case (uint8_t)USB_AUDIO_V2_AUDIOSTREAMING:
                        /* We have received an Audio Streaming Interface 
                         * descriptor. Save the interface number to an array of 
                         * Audio streaming interfaces. For an audio function the
                         * interface numbers starts from Audio Control 
                         * Interface ID.*/

                        /* save number of endpoints present in the streaming 
                         * interface */
                        audioControlInterfaceId = 
                                        curInfCollection-> bControlInterfaceNum;
                        //find out audio streaming interface array index.
                        strmIntrfcIndex = 
                                      interfaceNumber-audioControlInterfaceId-1U;

                        /*save no of endpoints present in the streaming 
                         * interface*/
                        curAlternateStng 
                        = &(audioInstance->infCollection.streamInf[strmIntrfcIndex].alterntSetting[alternateSetting]); 
                        curAlternateStng->numEndPoints = pStdInfDesc->bNumEndPoints;
                        break;
                    default :
                        /* Do Nothing */
                        break; 
                }
            }
        break; 
    
        /* Class Specific Audio Control Interface Descriptor. */ 
        case (uint8_t)USB_AUDIO_V2_CS_INTERFACE:
            /* if we have already received Audio Control Interface related 
             * descriptors then ignore all further Class specific descriptors */ 
            if (audioInstance->flags.audioControlInterfaceReady ==  1U)
            {
                return;
            }
            pCsInfDesc = (USB_AUDIO_V2_CS_AC_INTERFACE_HEADER_DESCRIPTOR*) pDescriptor;

            /* check if this Class specific descriptor belongs to an AC 
             * Interface*/
            if (interfaceNumber == audioInstance->infCollection.bControlInterfaceNum)
            {
                if  ((uint8_t)USB_AUDIO_V2_HEADER == pCsInfDesc->bDescriptorSubtype)
                {
                    SYS_ASSERT((pCsInfDesc->bInCollection != USB_DEVICE_AUDIO_V2_MAX_STREAMING_INTERFACES ),
                        "Maximum number of streaming interfaces defined does not match descriptor value");

                    /* save number of audio steaming interfaces */
                    //audioInstance->infCollection.numStreamingInf = pCsInfDesc->bInCollection;
					audioInstance->infCollection.numStreamingInf = 1;

                    /* save interface numbers of the all available audio 
                     * streaming and Midi interfaces.*/
                    for (cnt = 0; 
                            cnt < USB_DEVICE_AUDIO_V2_MAX_STREAMING_INTERFACES; 
                            cnt++ )
                    {
                        /*audioInstance->infCollection.streamInf[cnt].interfaceNum
                            = pCsInfDesc->bInCollection;*/
						audioInstance->infCollection.streamInf[cnt].interfaceNum
                                = 1;
                    }

                    /* save audio specification number. */
                    audioInstance->infCollection.bcdADC = pCsInfDesc->bcdADC;
                    audioInstance->flags.audioControlInterfaceReady = 1;
                } 
            } 
            break; 

        case (uint8_t)USB_DESCRIPTOR_ENDPOINT:
            /* we have received an Endpoint descriptor from device Layer*/
            pEPDesc = ( USB_ENDPOINT_DESCRIPTOR* ) pDescriptor;
            audioControlInterfaceId = 
                              audioInstance->infCollection.bControlInterfaceNum;
            strmIntrfcIndex = interfaceNumber - audioControlInterfaceId - 1U;
       
            // check if this Endpoint belongs to an audio Streaming interface
            if ((interfaceNumber == audioInstance->infCollection.streamInf[strmIntrfcIndex].interfaceNum))
            {
                /* Save the ep address */
                epAddressAndDirection = pEPDesc->bEndpointAddress;

                /* Save max packet size */
                maxPacketSize = (( USB_ENDPOINT_DESCRIPTOR* ) 
                                                  pDescriptor )->wMaxPacketSize;
       
                if (pEPDesc->usageType == USB_USAGE_DATA_ENDPOINT)
                {
                    /* Save ep address to the data interface */
                    audioInstance->infCollection.streamInf[strmIntrfcIndex].alterntSetting[alternateSetting].isoDataEp.epAddr 
                            = epAddressAndDirection;

                    /* Save max packet size to the data interface */
                    audioInstance->infCollection.streamInf[strmIntrfcIndex].alterntSetting[alternateSetting].isoDataEp.epMaxPacketSize 
                            = maxPacketSize;
                }
                else if (pEPDesc->usageType == USB_USAGE_FEEDBACK_ENDPOINT)
                {
                    /* Save ep address to the Sync interface */
                    audioInstance->infCollection.streamInf[strmIntrfcIndex].alterntSetting[alternateSetting].isoSyncEp.epAddr 
                            = epAddressAndDirection;

                    /* Save max packet size to the Sync interface */
                    audioInstance->infCollection.streamInf[strmIntrfcIndex].alterntSetting[alternateSetting].isoSyncEp.epMaxPacketSize 
                            = maxPacketSize;
                }
                else
                {
                    /* Do Nothing */
                }
            }
            break;

        default:
            /* SYS_ASSERT(false, "Unknown descriptor type");*/
            break; 
    }
}

#pragma coverity compliance end_block "MISRA C-2012 Rule 11.3"
#pragma GCC diagnostic pop
/* MISRAC 2012 deviation block end */
/*******************************************************************************/

void F_USB_DEVICE_AUDIO_V2_Deinitialize ( SYS_MODULE_INDEX funcDriverIndex  )
{
    /* Cancel all IRPs on the owned endpoints and then
     * disable the endpoint */

    if(funcDriverIndex  >= USB_DEVICE_AUDIO_V2_INSTANCES_NUMBER)
    {
        SYS_ASSERT(false," Invalid instance");
        return;
    }
   F_USB_DEVICE_AUDIO_V2_IRPCancelAll(funcDriverIndex );
}

/*******************************************************************************/

void F_USB_DEVICE_AUDIO_V2_TransferIRPCallBack ( USB_DEVICE_IRP * irp )
{
   /* This function is called when a Audio Read IRP has
    * terminated */

    USB_DEVICE_AUDIO_V2_EVENT_DATA_READ_COMPLETE readEventData;
    USB_DEVICE_AUDIO_V2_INSTANCE *thisAudioInstance;
    USB_DEVICE_AUDIO_V2_INDEX iAudio;
    uint8_t cnt;
    USB_DEVICE_AUDIO_V2_IRP_DATA *audioIrpData;
    USB_DEVICE_AUDIO_V2_EVENT event;

    /* The user data field of the IRP contains the array index of the Audio IRP
     * in the lower 16 bits and the and the unique Identifier in the upper 16 
     * bits.Mask the upper 16 bits to the Audio IRP index associated with this 
     * IRP */

    cnt = (uint8_t)(irp->userData & 0xFFFFU);

    /* get a pointer to the Audio IRP data */
    audioIrpData = &gUSBDeviceAudioV2IrpData[cnt];

    /* Retrieve Audio Instance Number */
    iAudio = audioIrpData->iAudio;

    /*Get a pointer to the Audio Instance */
    thisAudioInstance = &gUsbDeviceAudioV2Instance[iAudio];

     /* Populate the event handler for this transfer. This was stored in the
      * userData field of the IRP when the IRP was submitted. */
    readEventData.handle = ( USB_DEVICE_AUDIO_V2_TRANSFER_HANDLE ) irp->userData;

    /* update the size written */
    readEventData.length = (uint16_t)irp->size;
    
    /* Get transfer status */
    if ((irp->status == USB_DEVICE_IRP_STATUS_COMPLETED) 
        || (irp->status == USB_DEVICE_IRP_STATUS_COMPLETED_SHORT))
    {
        /* Transfer completed successfully */
        readEventData.status = USB_DEVICE_AUDIO_V2_RESULT_OK; 
    }
    else if (irp->status == USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT)
    {
        /* Transfer cancelled due to Endpoint Halt */
        readEventData.status = USB_DEVICE_AUDIO_V2_RESULT_ERROR_ENDPOINT_HALTED; 
    }
    else if (irp->status == USB_DEVICE_IRP_STATUS_TERMINATED_BY_HOST)
    {
        /* Transfer Cancelled by Host (Host sent a Clear feature )*/
        readEventData.status = 
                            USB_DEVICE_AUDIO_V2_RESULT_ERROR_TERMINATED_BY_HOST; 
    }
    else
    {
        /* Transfer was not completed successfully */
        readEventData.status = USB_DEVICE_AUDIO_V2_RESULT_ERROR; 
    }

    /* update Interface Number */ 
    readEventData.interfaceNum = audioIrpData->interfaceNum;

    if (audioIrpData->direction == USB_DEVICE_AUDIO_V2_READ)
    {
        event = USB_DEVICE_AUDIO_V2_EVENT_READ_COMPLETE;
        thisAudioInstance->currentQSizeRead--;
    }
    else
    {
        event = USB_DEVICE_AUDIO_V2_EVENT_WRITE_COMPLETE;
        thisAudioInstance->currentQSizeWrite--; 
    }

    /* Send an event to the application */ 
    if (thisAudioInstance->appEventCallBack != NULL)
    {
        thisAudioInstance->appEventCallBack(iAudio,
                                            event,
                                            &readEventData,
                                            thisAudioInstance->userData);
    }
}

void F_USB_DEVICE_AUDIO_V2_IRPCancelAll(USB_DEVICE_AUDIO_V2_INDEX iAudio)
{
    USB_DEVICE_AUDIO_V2_INSTANCE * thisAudioInstance = &gUsbDeviceAudioV2Instance[iAudio];
    uint8_t noOfStreamingInterface; 
    uint8_t count;
    uint8_t activeInterfaceSetting;
    USB_DEVICE_AUDIO_V2_STREAMING_INTERFACE* streamingInterface;
    uint8_t endpointAddress;

    /* Find out how many Streaming interfaces are supported by this 
     * Audio Instance*/
    noOfStreamingInterface = thisAudioInstance->infCollection.numStreamingInf;

    /* We have to cancel IRPs submitted for all of the streams */
    for (count=0; count < noOfStreamingInterface; count++)
    {
        /* Get pointer current streaing interface */
        streamingInterface = &thisAudioInstance->infCollection.streamInf[count];

        /* Find out active alternate setting*/
        activeInterfaceSetting = streamingInterface->activeSetting;

        if (streamingInterface->alterntSetting[activeInterfaceSetting].numEndPoints != 0U)
        {
            /* Retrieve Isochronous Data Endpoint address */
            endpointAddress = 
                  streamingInterface->alterntSetting[activeInterfaceSetting].isoDataEp.epAddr;

            /* Double if this not Endpoint 0*/
            if (endpointAddress !=0U)
            {
              /* Cancel all IRPs on this Endpoint */
              (void) USB_DEVICE_IRPCancelAll
                      (
                          gUsbDeviceAudioV2Instance[iAudio].devLayerHandle,  
                          endpointAddress
                      );
            }
          
            if (streamingInterface->alterntSetting[activeInterfaceSetting].numEndPoints == 2U)
            {
                /*If there are Two Endpoints, second Endpoint is a Sync Endpoint*/
                endpointAddress = 
                        streamingInterface->alterntSetting[activeInterfaceSetting].isoSyncEp.epAddr;

                /* Double if this not Endpoint 0*/
                if (endpointAddress !=0U)
                {
                  /* Cancel all IRPs on this Endpoint */
                  (void) USB_DEVICE_IRPCancelAll
                          (
                              gUsbDeviceAudioV2Instance[iAudio].devLayerHandle,  
                              endpointAddress
                          );
                }
            }
        }
    }
}


/*******************************************************************************/

/*******************************************************************************
 End of File
 */
