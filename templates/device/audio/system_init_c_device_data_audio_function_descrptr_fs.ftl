<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_device_data_audio_function_descrptr_fs.ftl

  Summary:
    USB Device Freemarker Template File

  Description:
    This file contains configurations necessary to run the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
*******************************************************************************/

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
-->
<#if CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE == "Audio v1.0 USB Speaker">

    /* Descriptor for Function 1 - Audio     */ 
    
    /* USB Speaker Standard Audio Control Interface Descriptor	*/
    0x09,                            // Size of this descriptor in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER},                               // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOCONTROL,          // Subclass code (bInterfaceSubclass)
    USB_AUDIO_PR_PROTOCOL_UNDEFINED, // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Speaker Class-specific AC Interface Descriptor  */
    0x09,                           // Size of this descriptor in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_HEADER,               // HEADER descriptor subtype 	(bDescriptorSubtype)
    0x00,0x01,                      /* Audio Device compliant to the USB Audio
                                     * specification version 1.00 (bcdADC) */
    0x28,0x00,                      /* Total number of bytes returned for the
                                     * class-specific AudioControl interface
                                     * descriptor. (wTotalLength). Includes the
                                     * combined length of this descriptor header
                                     * and all Unit and Terminal descriptors. */
    1,                           /* The number of AudioStreaming interfaces
                                     * in the Audio Interface Collection to which
                                     * this AudioControl interface belongs
                                     * (bInCollection) */
    1,                           /* AudioStreaming interface 1 belongs to this
                                     * AudioControl interface. (baInterfaceNr(1))*/

    /* USB Speaker Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,	    // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    0x01,          				    // ID of this Terminal.(bTerminalID)
    0x01,0x01,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x02,                           // Two Channels (bNrChannels)
    0x03,0x00,                      // (wChannelConfig)
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)

    /* USB Speaker Feature Unit Descriptor */
    0x0A,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x05,            				// ID of this Unit ( bUnitID  ).
    0x01,          					// Input terminal is connected to this unit(bSourceID)
    0x01,                           // (bControlSize) //was 0x03
    0x01,                           // (bmaControls(0)) Controls for Master Channel
    0x00,                           // (bmaControls(1)) Controls for Channel 1
    0x00,                           // (bmaControls(2)) Controls for Channel 2
    0x00,			    //  iFeature

    /* USB Speaker Output Terminal Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    0x02,          					// ID of this Terminal.(bTerminalID)
    0x01,0x03,                      // (wTerminalType)See USB Audio Terminal Types.
    0x00,                           // No association (bAssocTerminal)
    0x05,             				// (bSourceID)
    0x00,                           // Unused. (iTerminal)

    /* USB Speaker Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    1,	 // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Speaker Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    1,	 // Interface Number  (bInterfaceNumber)
    0x01,                            // Alternate Setting Number (bAlternateSetting)
    0x01,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /*  USB Speaker Class-specific AS General Interface Descriptor */
    0x07,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    		// GENERAL subtype (bDescriptorSubtype)
    0x01,           				// Unit ID of the Output Terminal.(bTerminalLink)
    0x01,                           // Interface delay. (bDelay)
    0x01,0x00,                      // PCM Format (wFormatTag)

    /*  USB Speaker Type 1 Format Type Descriptor */
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,         // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                           // FORMAT_TYPE_1. (bFormatType)
    0x02,                           // two channel.(bNrChannels)
    0x02,                           // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                           // 16 bits per sample.(bBitResolution)
    0x01,                           // One frequency supported. (bSamFreqType)
    0x80,0xBB,0x00,                 // Sampling Frequency = 48000 Hz(tSamFreq)

    /*  USB Speaker Standard Endpoint Descriptor */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,         // ENDPOINT descriptor (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_OUT_ENDPOINT_NUMBER} | USB_EP_DIRECTION_OUT,                            // Endpoint${CONFIG_USB_DEVICE_FUNCTION_OUT_ENDPOINT_NUMBER}:OUT (bEndpointAddress)
    0x09,                            /* ?(bmAttributes) Isochronous,
                                      * Adaptive, data endpoint */
    (192),0x00,                      // ?(wMaxPacketSize) //48 * 4
    0x01,                            // One packet per frame.(bInterval)
    0x00,                            // Unused. (bRefresh)
    0x00,                            // Unused. (bSynchAddress)

    /* USB Speaker Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,           // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,            // GENERAL subtype. (bDescriptorSubtype)
    0x00,                            /* No sampling frequency control, no pitch
                                        control, no packet padding.(bmAttributes)*/
    0x00,                            // Unused. (bLockDelayUnits)
    0x00,0x00,                       // Unused. (wLockDelay)


<#elseif CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE == "Audio v1.0 USB Microphone">
   /* USB Microphone Standard Audio Control Interface Descriptor    */
   0x09,                            // Size of this descriptor in bytes (bLength)
   USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
   USB_DEVICE_AUDIO_CONTROL_INTERFACE_ID,
   //0,                               //USB_DEVICE_AUDIO_CONTROL_INTERFACE_ID,
                                    // Interface Number  (bInterfaceNumber)
   ${CONFIG_USB_DEVICE_FUNCTION_INTERFACE_NUMBER},                            // Alternate Setting Number (bAlternateSetting)
   0x00,                            // Number of endpoints in this intf (bNumEndpoints)
   USB_AUDIO_CLASS_CODE,
                                    // Class code  (bInterfaceClass)
   USB_AUDIO_AUDIOCONTROL,          // Subclass code (bInterfaceSubclass)
   USB_AUDIO_PR_PROTOCOL_UNDEFINED,
                                    // Protocol code  (bInterfaceProtocol)
   0x00,                            // Interface string index (iInterface)

    /* USB Microphone Class-specific AC Interface Descriptor  */
    0x09,                           // Size of this descriptor in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_HEADER,               // HEADER descriptor subtype     (bDescriptorSubtype)
    0x00,0x01,                      /* Audio Device compliant to the USB Audio
                                     * specification version 1.00 (bcdADC) */

    0x29,0x00,                      /* Total number of bytes returned for the
                                     * class-specific AudioControl interface
                                     * descriptor. (wTotalLength). Includes the
                                     * combined length of this descriptor header
                                     * and all Unit and Terminal descriptors. */
    0x01,                           /* The number of AudioStreaming interfaces
                                     * in the Audio Interface Collection to which
                                     * this AudioControl interface belongs
                                     * (bInCollection) */
    0x01,                           /* AudioStreaming interface 1 belongs to this
                                     * AudioControl interface. (baInterfaceNr(1))*/

    /* USB Microphone Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,          // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    APP_ID_INPUT_TERMINAL_MIC,      // ID of this Terminal.(bTerminalID)
    0x01, 0x02,                     // (wTerminalType) Microphone
    0x00,                           // No association (bAssocTerminal)
    0x01,                           // One Channel (bNrChannels)
    0x04,0x00,                      // (wChannelConfig)
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)

    /* USB Microphone Feature Unit Descriptor */
#if 1
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    APP_ID_FEATURE_UNIT_MIC,        // ID of this Unit ( bUnitID  ).
    APP_ID_INPUT_TERMINAL_MIC,      // Input terminal is connected to this unit(bSourceID)
    0x02,                           // (bControlSize) //was 0x03
    0x01,0x00,                           // (bmaControls(0)) Controls for Master Channel //0x48
    0x00,0x00,                           // (bmaControls(1)) Controls for Channel 1
    0x00,                //  iFeature
#endif
    /* USB Microphone Output Terminal Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    APP_ID_OUTPUT_TERMINAL_MIC,          // ID of this Terminal.(bTerminalID)
    0x01,0x01,                       // (wTerminalType) (Stream) See USB Audio Terminal Types.
    0x00,                            // No association (bAssocTerminal)
    APP_ID_FEATURE_UNIT_MIC,             // (bSourceID)
    0x00,

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    USB_DEVICE_AUDIO_STREAMING_INTERFACE_ID_1,	 // Interface Number  (bInterfaceNumber)
    //1,                               // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    USB_DEVICE_AUDIO_STREAMING_INTERFACE_ID_1,	 // Interface Number  (bInterfaceNumber)
    //1,                               // Interface Number  (bInterfaceNumber)
    0x01,                            // Alternate Setting Number (bAlternateSetting)
    0x01,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /*  USB Microphone Class-specific AS General Interface Descriptor */
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,           // GENERAL subtype (bDescriptorSubtype)
    APP_ID_OUTPUT_TERMINAL_MIC,       // Unit ID of the Output Terminal.(bTerminalLink)
    0x00,                            // Interface delay. (bDelay)
    0x01,0x00,                       // PCM Format (wFormatTag)

    /*  USB Microphone Type 1 Format Type Descriptor */
    0x0B,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,          // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                            // FORMAT_TYPE_1. (bFormatType)
    0x01,                            // one channel.(bNrChannels)
    0x02,                            // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                            // 16 bits per sample.(bBitResolution)
    0x01,                            // One frequency supported. (bSamFreqType)
    0x80,0x3E,0x00,                  // Sampling Frequency = 16000 Hz(tSamFreq)

    /*  USB Microphone Standard Endpoint Descriptor */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,         // ENDPOINT descriptor (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_IN_ENDPOINT_NUMBER} | USB_EP_DIRECTION_IN,                            // Endpoint${CONFIG_USB_DEVICE_FUNCTION_IN_ENDPOINT_NUMBER}:IN (bEndpointAddress)
    0x01,                            //vorher: 0x09/* ?(bmAttributes) Isochronous,
                                      /* Asynchronized, data endpoint */
    0x20,0x00,                      // ?(wMaxPacketSize) //32=16*2  (2ms@16000)
    0x01,                            // One packet per frame.(bInterval)
    0x00,                            // Unused. (bRefresh)
    0x00,                            // Unused. (bSynchAddress)

    /* USB Microphone Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,           // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,            // GENERAL subtype. (bDescriptorSubtype)
    0x00,                            /* No sampling frequency control, no pitch
                                        control, no packet padding.(bmAttributes)*/
    0x00,                            // Unused. (bLockDelayUnits)
    0x00,0x00                        // Unused. (wLockDelay)

<#elseif CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE == "Audio v1.0 USB Headset">

/* Descriptor for Function 1 - Audio     */ 
    
    /* USB Headset Standard Audio Control Interface Descriptor          */
    0x09,                            // Size of this descriptor in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    0,
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)				 
    USB_AUDIO_AUDIOCONTROL,          // Subclass code (bInterfaceSubclass)
    USB_AUDIO_PR_PROTOCOL_UNDEFINED, // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Headset Class-specific AC Interface Descriptor  */
    0x0A,                           // Size of this descriptor in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_HEADER,               // HEADER descriptor subtype      (bDescriptorSubtype)
    0x00,0x01,                      /* Audio Device compliant to the USB Audio
                                     * specification version 1.00 (bcdADC) */
									 
    0x64,0x00,                      /* Total number of bytes returned for the
                                     * class-specific AudioControl interface
                                     * descriptor. (wTotalLength). Includes the
                                     * combined length of this descriptor header
                                     * and all Unit and Terminal descriptors. */

    2,                              /* The number of AudioStreaming interfaces
                                     * in the Audio Interface Collection to which
                                     * this AudioControl interface belongs
                                     * (bInCollection) */

    1,                              /* AudioStreaming interface 1 belongs to this
                                     * AudioControl interface. (baInterfaceNr(1))*/
    2,                              /* AudioStreaming interface 2 belongs to this
                                     * AudioControl interface. (baInterfaceNr(2))*/

    /* USB Headset Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,                // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_INPUT_TERMINAL_ID,          // ID of this Terminal.(bTerminalID)
    0x01,0x01,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x02,                           // Two Channels (bNrChannels)
    0x03,0x00,                      // (wChannelConfig)
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)
    
    /* USB Headset Microphone Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,                // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_INPUT_TERMINAL_MICROPHONE_ID,          // ID of this Terminal.(bTerminalID)
    0x01,0x02,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x01,                           // Two Channels (bNrChannels)
    0x04,0x00,                      // (wChannelConfig) 0x0004
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)

    /* USB Headset Feature Unit ID8 Descriptor */
    0x0D,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,             // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_FEATURE_UNIT_ID,                // ID of this Unit ( bUnitID  ).
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_MIXER_UNIT_ID,                  // Input terminal is connected to this unit(bSourceID)
    0x02,                               // (bControlSize) //was 0x03
    0x01,0x00,                          // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                          // (bmaControls(1)) Controls for Channel 1
    0x00,0x00,                          // (bmaControls(2)) Controls for Channel 2
    0x00,                               //  iFeature
    
    /* USB Headset Feature Unit ID5 Descriptor */
    0x0B,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,             // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_FEATURE_UNIT_MICROPHONE_ID,     // ID of this Unit ( bUnitID  ).
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_INPUT_TERMINAL_MICROPHONE_ID,   // Input terminal is connected to this unit(bSourceID)
    0x02,                               // (bControlSize) //was 0x03
    0x01,0x00,                          // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                          // (bmaControls(1)) Controls for Channel 1
    0x00,                               //  iFeature
    
    /* USB Headset Feature Unit ID7 Descriptor */
    0x0B,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,             // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_FEATURE_UNIT_SIDE_TONING_ID,    // ID of this Unit ( bUnitID  ).
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_INPUT_TERMINAL_MICROPHONE_ID,   // Input terminal is connected to this unit(bSourceID)
    0x02,                               // (bControlSize) //was 0x03
    0x01,0x00,                          // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                          // (bmaControls(1)) Controls for Channel 1
    0x00,                               //  iFeature
    
    /* USB Headset Mixer Unit ID8 Descriptor */
    0x0D,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_MIXER_UNIT,               // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_MIXER_UNIT_ID,                  // ID of this Unit ( bUnitID  ).
    2,                                  // Number of input pins
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_INPUT_TERMINAL_ID,              // sourceID 1
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_FEATURE_UNIT_SIDE_TONING_ID,    // sourceID 2
    0x02,                               // number of channels
    0x03,0x00,                          // channel config
    0x00,                               // iChannelNames
    0x00,                               // bmControls
    0x00,                               // iMixer
    
    /* USB Headset Output Terminal Descriptor */
    0x09,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,             // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,          // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_OUTPUT_TERMINAL_ID,             // ID of this Terminal.(bTerminalID)
    0x02,0x03,                          // (wTerminalType)See USB Audio Terminal Types.
    0x00,                               // No association (bAssocTerminal)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_FEATURE_UNIT_ID,                // (bSourceID)
    0x00,                               // Unused. (iTerminal)

    /* USB Headset Output Terminal Microphone Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_OUTPUT_TERMINAL_MICROPHONE_ID,          // ID of this Terminal.(bTerminalID)
    0x01,0x01,                       // (wTerminalType)See USB Audio Terminal Types.
    0x00,                            // No association (bAssocTerminal)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_FEATURE_UNIT_MICROPHONE_ID,             // (bSourceID)
    0x00,                            // Unused. (iTerminal)

    /* USB Headset Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    1,        // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Headset Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    1,                              // Interface Number  (bInterfaceNumber)
    0x01,                            // Alternate Setting Number (bAlternateSetting)
    0x01,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /*  USB Headset Class-specific AS General Interface Descriptor */
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    // GENERAL subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_INPUT_TERMINAL_ID,           // Unit ID of the Output Terminal.(bTerminalLink)
    0x01,                            // Interface delay. (bDelay)
    0x01,0x00,                       // PCM Format (wFormatTag)

    /*  USB Headset Type 1 Format Type Descriptor */
    0x11,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,          // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                            // FORMAT_TYPE_1. (bFormatType)
    0x02,                            // two channel.(bNrChannels)
//    0x01,
    0x02,                            // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                            // 16 bits per sample.(bBitResolution)
    0x03,                            // two frequency supported. (bSamFreqType)
    
    0x80,0x3E,0x00,                  // 16000
//    0xC0,0x5D,0x00,                  // 24000
    0x00,0x7D,0x00,                  // 32000
    0x80,0xBB,0x00,                  // Sampling Frequency = 48000 Hz(tSamFreq)

    /*  USB Headset Standard Endpoint Descriptor */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,         // ENDPOINT descriptor (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_OUT_ENDPOINT_NUMBER} | USB_EP_DIRECTION_OUT,                            // Endpoint${CONFIG_USB_DEVICE_FUNCTION_OUT_ENDPOINT_NUMBER}:OUT  (bEndpointAddress)
    0x09,                            /* ?(bmAttributes) Isochronous,
                                      * asynchronous, data endpoint */
    0xC0,0x00,                      // ?(wMaxPacketSize) //48 * 4
//    0x40, 0x00,                     // 16*4
    0x01,                            // One packet per frame.(bInterval)
    0x00,                            // Unused. (bRefresh)
    0x00,                            // Unused. (bSynchAddress)

    /* USB Speaker Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,           // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,            // GENERAL subtype. (bDescriptorSubtype)
    0x01,                            /* Turn on sampling frequency control, no pitch
                                        control, no packet padding.(bmAttributes)*/
    0x00,                            // Unused. (bLockDelayUnits)
    0x00,0x00,                        // Unused. (wLockDelay)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    0x02,                              // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    0x02,                           // Interface Number  (bInterfaceNumber)
    0x01,                           // Alternate Setting Number (bAlternateSetting)
    0x01,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                               // Interface string index (iInterface)

    /*  USB Microphone Class-specific AS General Interface Descriptor */
    0x07,                               // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,                  // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,                     // GENERAL subtype (bDescriptorSubtype)
    USB_DEVICE_AUDIO_IDX${CONFIG_USB_DEVICE_FUNCTION_INDEX}_DESCRIPTOR_OUTPUT_TERMINAL_MICROPHONE_ID,           // Unit ID of the Output Terminal.(bTerminalLink)
    0x00,                            // Interface delay. (bDelay)
    0x01,0x00,                       // PCM Format (wFormatTag)

    /*  USB Microphone Type 1 Format Type Descriptor */
    0x11,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,                   // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,          // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                            // FORMAT_TYPE_1. (bFormatType)
    0x01,                            // one channel.(bNrChannels)
    0x02,                            // 2 bytes per audio subframe.(bSubFrameSize)
    0x10,                            // 16 bits per sample.(bBitResolution)
    0x03,                            // One frequency supported. (bSamFreqType)
    0x80,0x3E,0x00,                  // Sampling Frequency = 16000 Hz(tSamFreq)
//    0xC0,0x5D,0x00,                  // 24000
    0x00,0x7D,0x00,                  // 32000
    0x80,0xBB,0x00,                  // Sampling Frequency = 48000 Hz(tSamFreq)

    /*  USB Microphone Standard Endpoint Descriptor */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,         // ENDPOINT descriptor (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_IN_ENDPOINT_NUMBER} | USB_EP_DIRECTION_IN,                            // Endpoint${CONFIG_USB_DEVICE_FUNCTION_IN_ENDPOINT_NUMBER}:IN (bEndpointAddress)
    0x0D,                            /* ?(bmAttributes) Isochronous,
                                      * asynchronous, data endpoint */
    0x60, 0x00,                      // ?(wMaxPacketSize) //48 * 2
//    0x40, 0x00,
    0x01,                            // One packet per frame.(bInterval)
    0x00,                            // Unused. (bRefresh)
    0x00,                            // Unused. (bSynchAddress)

    /* USB Microphone Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,           // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,            // GENERAL subtype. (bDescriptorSubtype)
    0x01,                            /* Turn on sampling frequency control, no pitch
                                        control, no packet padding.(bmAttributes)*/
    0x00,                            // Unused. (bLockDelayUnits)
    0x00,0x00,                        // Unused. (wLockDelay)
	
<#elseif CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE == "Audio v1.0 USB Headset Multi Sampling rates">	
	/* USB Speaker Standard Audio Control Interface Descriptor	*/
	0x09,                            // Size of this descriptor in bytes (bLength)
	USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
   USB_DEVICE_AUDIO_CONTROL_INTERFACE_ID,
   //0,                               //USB_DEVICE_AUDIO_CONTROL_INTERFACE_ID,
                                    // Interface Number  (bInterfaceNumber)
	0x00,                            // Alternate Setting Number (bAlternateSetting)
	0x00,                            // Number of endpoints in this intf (bNumEndpoints)
	USB_AUDIO_CLASS_CODE,
                                    // Class code  (bInterfaceClass)
	USB_AUDIO_AUDIOCONTROL,          // Subclass code (bInterfaceSubclass)
	USB_AUDIO_PR_PROTOCOL_UNDEFINED,
                                    // Protocol code  (bInterfaceProtocol)
	0x00,                            // Interface string index (iInterface)

	/* USB Microphone Class-specific AC Interface Descriptor  */
    0x09,                           // Size of this descriptor in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_HEADER,               // HEADER descriptor subtype 	(bDescriptorSubtype)
    0x00,0x01,                      /* Audio Device compliant to the USB Audio
                                     * specification version 1.00 (bcdADC) */

    0x29,0x00,                      /* Total number of bytes returned for the
                                     * class-specific AudioControl interface
                                     * descriptor. (wTotalLength). Includes the
                                     * combined length of this descriptor header
                                     * and all Unit and Terminal descriptors. */
    0x01,                           /* The number of AudioStreaming interfaces
                                     * in the Audio Interface Collection to which
                                     * this AudioControl interface belongs
                                     * (bInCollection) */
    0x01,                           /* AudioStreaming interface 1 belongs to this
                                     * AudioControl interface. (baInterfaceNr(1))*/

    /* USB Microphone Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,	    // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    APP_ID_INPUT_TERMINAL_MIC,      // ID of this Terminal.(bTerminalID)
    0x01, 0x02,                     // (wTerminalType) Microphone
    0x00,                           // No association (bAssocTerminal)
    0x01,                           // One Channel (bNrChannels)
    0x04,0x00,                      // (wChannelConfig)
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)

    /* USB Microphone Feature Unit Descriptor */
#if 1
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    APP_ID_FEATURE_UNIT_MIC,        // ID of this Unit ( bUnitID  ).
    APP_ID_INPUT_TERMINAL_MIC,      // Input terminal is connected to this unit(bSourceID)
    0x02,                           // (bControlSize) //was 0x03
    0x01,0x00,                           // (bmaControls(0)) Controls for Master Channel //0x48
    0x00, 0x00,                      // (bmaControls(1)) Controls for Channel 1
    0x00,			    			//  iFeature
#endif
    /* USB Microphone Output Terminal Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    APP_ID_OUTPUT_TERMINAL_MIC,          // ID of this Terminal.(bTerminalID)
    0x01,0x01,                       // (wTerminalType) (Stream) See USB Audio Terminal Types.
    0x00,                            // No association (bAssocTerminal)
    APP_ID_FEATURE_UNIT_MIC,             // (bSourceID)
    0x00,

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    USB_DEVICE_AUDIO_STREAMING_INTERFACE_ID_1,	 // Interface Number  (bInterfaceNumber)
    //1,                               // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    USB_DEVICE_AUDIO_STREAMING_INTERFACE_ID_1,	 // Interface Number  (bInterfaceNumber)
    //1,                               // Interface Number  (bInterfaceNumber)
    0x01,                            // Alternate Setting Number (bAlternateSetting)
    0x01,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /*  USB Microphone Class-specific AS General Interface Descriptor */
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    		// GENERAL subtype (bDescriptorSubtype)
    APP_ID_OUTPUT_TERMINAL_MIC,       // Unit ID of the Output Terminal.(bTerminalLink)
    0x00,                            // Interface delay. (bDelay)
    0x01,0x00,                       // PCM Format (wFormatTag)

    /*  USB Microphone Type 1 Format Type Descriptor */
    0x0B,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,          // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                            // FORMAT_TYPE_1. (bFormatType)
    0x01,                            // one channel.(bNrChannels)
    0x02,                            // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                            // 16 bits per sample.(bBitResolution)
    0x01,                            // One frequency supported. (bSamFreqType)
    //0x80,0x3E,0x00,                  // Sampling Frequency = 16000 Hz(tSamFreq)
    0x80,0xBB,0x00,                  // Sampling Frequency = 48000 Hz(tSamFreq)

    /*  USB Microphone Standard Endpoint Descriptor */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,         // ENDPOINT descriptor (bDescriptorType)
    0x81,                            // IN Endpoint 2. (bEndpointAddress)
    0x01,                            //vorher: 0x09/* ?(bmAttributes) Isochronous,
                                      /* Asynchronized, data endpoint */
    //0x20,0x00,                      // ?(wMaxPacketSize) //32=16*2  (2ms@16000)
    //0x60,0x00,                      // ?(wMaxPacketSize) //48 * 2  (2ms@48000) 
    0xC0,0x00,                      // ?(wMaxPacketSize) //48 * 2 * 2  (2ms@48000) 
    0x01,                            // One packet per frame.(bInterval)
    0x00,                            // Unused. (bRefresh)
    0x00,                            // Unused. (bSynchAddress)

    /* USB Microphone Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,           // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,            // GENERAL subtype. (bDescriptorSubtype)
    0x00,                            /* No sampling frequency control, no pitch
                                        control, no packet padding.(bmAttributes)*/
    0x00,                            // Unused. (bLockDelayUnits)
    0x00,0x00                        // Unused. (wLockDelay)
	
<#elseif CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE == "Audio v2.0 USB Speaker">

/* Interface Association Descriptor: */
    0x08,									// bLength
    USB_AUDIO_V2_DESCRIPTOR_IA,				// bDescriptorType
    0,
											// bFirstInterface
    2,
											// bInterfaceCount
    AUDIO_V2_FUNCTION,						// bFunctionClass   (Audio Device Class)
    AUDIO_V2_FUNCTION_SUBCLASS_UNDEFINED,	// bFunctionSubClass
    AUDIO_V2_AF_VERSION_02_00,				// bFunctionProtocol
    0x00,									// iFunction

    // Interface Descriptor:
    0x09,									// bLength
    USB_DESCRIPTOR_INTERFACE,				// bDescriptorType
    0,	
											// bInterfaceNumber
    0x00,									// bAlternateSetting
    0x00,									// bNumEndPoints
    AUDIO_V2,								// bInterfaceClass   (Audio Device Class)
    AUDIO_V2_AUDIOCONTROL,					// bInterfaceSubClass   (Audio Control Interface)
    AUDIO_V2_IP_VERSION_02_00,				// bInterfaceProtocol
    0x00,									// iInterface

    // AC Interface Header Descriptor:
    0x09,									// bLength
    AUDIO_V2_CS_INTERFACE,					// bDescriptorType
    AUDIO_V2_HEADER,						// bDescriptorSubtype
    0x00, 0x02,								// bcdADC
    AUDIO_V2_IO_BOX,						// bCategory
    0x48, 0x00,								// wTotalLength
    0x00,									// bmControls

    // AC Clock Source Descriptor:
    0x08,									// bLength
    AUDIO_V2_CS_INTERFACE,					// bDescriptorType
    AUDIO_V2_CLOCK_SOURCE,					// bDescriptorSubtype
    0x28,									// bClockID
    0x03,									// bmAttributes: Internal Progamable Clock
    0x07,									// bmControls
											// D[1:0]/11 : Clock Freq Control R/W
											// D[3:2]/01 : Clock Validity Control R
    0x00,									// bAssocTerminal
    0x00,									// iClockSource

    // AC Clock Selector Descriptor:
    0x08,									// bLength
    AUDIO_V2_CS_INTERFACE,					// bDescriptorType
    AUDIO_V2_CLOCK_SELECTOR,				// bDescriptorSubtype
    0x11,									// bClockID
    0x01,									// bNrInPins
    0x28,									// baCSourceID(1)
    0x03,									// bmControls
											/* D[1:0] : Clock Selector Control R/W
											   D[7:4] : Reserved (0) */
    0x00,									// iClockSelector

    // AC Input Terminal Descriptor:
    0x11,									// bLength
    AUDIO_V2_CS_INTERFACE,					// bDescriptorType
    AUDIO_V2_INPUT_TERMINAL,				// bDescriptorSubtype
    0x02,					// bTerminalID
    (uint8_t )USB_AUDIO_V2_TERMTYPE_USB_STREAMING,
    (uint8_t )(USB_AUDIO_V2_TERMTYPE_USB_STREAMING >> 8),	
											// wTerminalType   (USB Streaming)
    0x00,									// bAssocTerminal
    0x11,									// bCSourceID
    0x02,									// bNrChannels
    0x00, 0x00, 0x00, 0x00,					// bmChannelConfig N/A Front Left/Right
    0x00,									// iChannelNames
    0x00, 0x00,								// bmControls
    0x00,									// iTerminal

    // AC Feature Unit Descriptor:
    0x12,									// bLength
    AUDIO_V2_CS_INTERFACE,					// bDescriptorType
    AUDIO_V2_FEATURE_UNIT,					// bDescriptorSubtype
    0x0A,									// bUnitID
    0x02,									// bSourceID
    0x03, 0x00, 0x00, 0x00,					// bmaControls(0): Master Mute R/W
    0x00, 0x00, 0x00, 0x00,					// bmaControls(1)
    0x00, 0x00, 0x00, 0x00,					// bmaControls(2)
    0x00,									// iFeature

    // AC Output Terminal Descriptor:
    0x0C,									// bLength
    AUDIO_V2_CS_INTERFACE,					// bDescriptorType
    AUDIO_V2_OUTPUT_TERMINAL,				// bDescriptorSubtype
    0x14,					// bTerminalID
    (uint8_t )AUDIO_V2_SPEAKER,
    (uint8_t )(AUDIO_V2_SPEAKER>>8),		// wTerminalType   (Speaker)
    0,										// bAssocTerminal
    0x0A,									// bSourceID
    0x11,									// bCSourceID
    0x00, 0x00,								// bmControls
    0x00,									// iTerminal

    // Interface Descriptor:
    0x09,									// bLength
    USB_DESCRIPTOR_INTERFACE,				// bDescriptorType
    0x01,									// bInterfaceNumber
    0x00,									// bAlternateSetting
    0x00,									// bNumEndPoints
    AUDIO_V2,              					// bInterfaceClass: AUDIO
    AUDIO_V2_AUDIOSTREAMING,     			// bInterfaceSubClass: AUDIO_STREAMING
    AUDIO_V2_IP_VERSION_02_00,   			// bInterfaceProtocol: IP_VERSION_02_00
    0x00,									// iInterface

    // Interface Descriptor:
    0x09,									// bLength
    USB_DESCRIPTOR_INTERFACE,				// bDescriptorType
    0x01,									// bInterfaceNumber
    0x01,									// bAlternateSetting
    0x02,									// bNumEndPoints
    AUDIO_V2,              					// bInterfaceClass: AUDIO
    AUDIO_V2_AUDIOSTREAMING,     			// bInterfaceSubClass: AUDIO_STREAMING
    AUDIO_V2_IP_VERSION_02_00,   			// bInterfaceProtocol: IP_VERSION_02_00
    0x00,									// iInterface

    // AS Interface Descriptor:
    0x10,									// bLength
    AUDIO_V2_CS_INTERFACE,					// bDescriptorType
    AUDIO_V2_AS_GENERAL,					// bDescriptorSubtype
    0x02,									// bTerminalLink/Input Terminal
    0x00,									// bmControls
    0x01,									// bFormatType
    (uint8_t )AUDIO_V2_PCM, 0x00, 0x00, 0x00,      
											// bmFormats (note this is a bitmap)
    0x02,									// bNrChannels
    0x00, 0x00, 0x00, 0x00,					// bmChannelConfig: N/A Front Left/Right
    0x00,									// iChannelNames

    // AS Format Type 1 Descriptor:
    0x06,									// bLength
    AUDIO_V2_CS_INTERFACE,					// bDescriptorType
    AUDIO_V2_FORMAT_TYPE,					// bDescriptorSubtype
    0x01,									// bFormatType
    0x04,									// bSubslotSize
    0x18,									// bBitResolution   (24 Bits/sample)
    // Data Endpoint Descriptor:
    0x07,									// bLength
    USB_DESCRIPTOR_ENDPOINT,				// bDescriptorType
    0x01,									// bEndpointAddress   (OUT Endpoint)
    0x05,									// bmAttributes	(Transfer: Isochronous / Synch: Async / Usage: Data)
    0x00, 0x04,								// Endpoint size
    0x01,									// bInterval

    // AS Isochronous Data Endpoint Descriptor:
    0x08,									// bLength
    AUDIO_V2_CS_ENDPOINT,					// bDescriptorType
    AUDIO_V2_EP_GENERAL,					// bDescriptorSubtype
    0x00,									// bmAttributes
    0x35,									// bmControls
    0x00,									// bLockDelayUnits   (Decoded PCM samples)
    0x00, 0x00,								// wLockDelay

    // Feedback Endpoint Descriptor:
    0x07,									// bLength
    USB_DESCRIPTOR_ENDPOINT,				// bDescriptorType
    0x81,									// bEndpointAddress   (IN Endpoint)
    0x11,									// bmAttributes	(Transfer: Isochronous / Synch: None / Usage: Feedback)
    0x04, 0x00,								// Endpoint Size
    0x04,									// bInterval	
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->