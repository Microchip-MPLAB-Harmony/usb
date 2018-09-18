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
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
-->
<#if CONFIG_USB_DEVICE_FUNCTION_AUDIO_DEVICE_TYPE == "Audio v1.0 USB Speaker">

	/* Descriptor for Function 1 - Audio     */ 
    
	/* USB Speaker Standard Audio Control Interface Descriptor	*/
	0x09,                            // Size of this descriptor in bytes (bLength)
	USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
	0,
                                    // Interface Number  (bInterfaceNumber)
	0x00,                            // Alternate Setting Number (bAlternateSetting)
	0x00,                            // Number of endpoints in this intf (bNumEndpoints)
	USB_AUDIO_CLASS_CODE,
                                    // Class code  (bInterfaceClass)
	USB_AUDIO_AUDIOCONTROL,          // Subclass code (bInterfaceSubclass)
	USB_AUDIO_PR_PROTOCOL_UNDEFINED,
                                    // Protocol code  (bInterfaceProtocol)
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
    0x01,                            // OUT Endpoint 1. (bEndpointAddress)
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
    0x00,0x00,                        // Unused. (wLockDelay)
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->