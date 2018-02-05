<#if CONFIG_USB_DEVICE_FUNCTION_3_IDX0 == true>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX0 == true>
    /* Descriptor for Function 3 - CDC     */ 
    <#if CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 2 
      || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 3 
      || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 4 
      || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 5 
      || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 6
      || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 7 
      || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 8
      || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 9 
      || CONFIG_USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX0?number == 10      >
    /* Interface Association Descriptor: CDC Function 1*/
    0x08,   // Size of this descriptor in bytes
    0x0B,   // Interface association descriptor type
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},   // The first associated interface
    0x02,   // Number of contiguous associated interface
    0x02,   // bInterfaceClass of the first interface
    0x02,   // bInterfaceSubclass of the first interface
    0x01,   // bInterfaceProtocol of the first interface
    0x00,   // Interface string index
    </#if>
    
    /* Interface Descriptor */

    0x09,                                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                       // Descriptor Type
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},                                           // Interface Number
    0x00,                                           // Alternate Setting Number
    0x01,                                           // Number of endpoints in this interface
    USB_CDC_COMMUNICATIONS_INTERFACE_CLASS_CODE,    // Class code
    USB_CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL,        // Subclass code
    USB_CDC_PROTOCOL_AT_V250,                       // Protocol code
    0x00,                                           // Interface string index

    /* CDC Class-Specific Descriptors */

    sizeof(USB_CDC_HEADER_FUNCTIONAL_DESCRIPTOR),               // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_HEADER,                                  // Type of functional descriptor
    0x20,0x01,                                                  // CDC spec version

    sizeof(USB_CDC_ACM_FUNCTIONAL_DESCRIPTOR),                  // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_ABSTRACT_CONTROL_MANAGEMENT,             // Type of functional descriptor
    USB_CDC_ACM_SUPPORT_LINE_CODING_LINE_STATE_AND_NOTIFICATION,// bmCapabilities of ACM

    sizeof(USB_CDC_UNION_FUNCTIONAL_DESCRIPTOR_HEADER) + 1,     // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_UNION,                                   // Type of functional descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},                                                       // com interface number
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number+1},

    sizeof(USB_CDC_CALL_MANAGEMENT_DESCRIPTOR),                 // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_CALL_MANAGEMENT,                         // Type of functional descriptor
    0x00,                                                       // bmCapabilities of CallManagement
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number+1},                                                       // Data interface number

    /* Interrupt Endpoint (IN)Descriptor */

    0x07,                           // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_CDC_INT_ENDPOINT_NUMBER_IDX0}| USB_EP_DIRECTION_IN,      // EndpointAddress ( EP${CONFIG_USB_DEVICE_FUNCTION_3_CDC_INT_ENDPOINT_NUMBER_IDX0} IN INTERRUPT)
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes type of EP (INTERRUPT)
    0x10,0x00,                      // Max packet size of this EP
    0x02,                           // Interval (in ms)

    /* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // INTERFACE descriptor type
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number+1},      // Interface Number
    0x00,                               // Alternate Setting Number
    0x02,                               // Number of endpoints in this interface
    USB_CDC_DATA_INTERFACE_CLASS_CODE,  // Class code
    0x00,                               // Subclass code
    USB_CDC_PROTOCOL_NO_CLASS_SPECIFIC, // Protocol code
    0x00,                               // Interface string index

    /* Bulk Endpoint (OUT)Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_CDC_BULK_ENDPOINT_NUMBER_IDX0}|USB_EP_DIRECTION_OUT,     // EndpointAddress ( EP${CONFIG_USB_DEVICE_FUNCTION_3_CDC_BULK_ENDPOINT_NUMBER_IDX0} OUT)
    USB_TRANSFER_TYPE_BULK,     // Attributes type of EP (BULK)
    <#if usbDeviceSpeed == 1>
    0x00, 0x02,                 // Max packet size of this EP
    <#elseif usbDeviceSpeed == 2 >
    0x40,0x00,                  // Max packet size of this EP
    </#if>
    0x00,                       // Interval (in ms)

     /* Bulk Endpoint (IN)Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_CDC_BULK_ENDPOINT_NUMBER_IDX0}|USB_EP_DIRECTION_IN,      // EndpointAddress ( EP${CONFIG_USB_DEVICE_FUNCTION_3_CDC_BULK_ENDPOINT_NUMBER_IDX0} IN )
    0x02,                       // Attributes type of EP (BULK)
    <#if usbDeviceSpeed == 1>
    0x00, 0x02,                 // Max packet size of this EP
    <#elseif usbDeviceSpeed == 2 >
    0x40,0x00,                  // Max packet size of this EP
    </#if>
    0x00,                       // Interval (in ms)
<#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX0 == true>
     /* Descriptor for Function 3 - HID     */ 	
	/* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // INTERFACE descriptor type
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},                                  // Interface Number
    0,                                  // Alternate Setting Number
    2,                                  // Number of endpoints in this interface
    USB_HID_CLASS_CODE,                 // Class code
    USB_HID_SUBCLASS_CODE_NO_SUBCLASS , // Subclass code
    USB_HID_PROTOCOL_CODE_NONE,         // No Protocol
    0,                                  // Interface string index

    /* HID Class-Specific Descriptor */

    0x09,                           // Size of this descriptor in bytes
    USB_HID_DESCRIPTOR_TYPES_HID,   // HID descriptor type
    0x11,0x01,                      // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    1,                              // Number of class descriptors, see usbcfg.h
    USB_HID_DESCRIPTOR_TYPES_REPORT,// Report descriptor type
    USB_DEVICE_16bitTo8bitArrange(sizeof(hid_rpt${usbDeviceHIDReportCount})),   // Size of the report descriptor

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_HID_ENDPOINT_NUMBER_IDX0} | USB_EP_DIRECTION_IN,      // EndpointAddress
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // Size
    0x01,                           // Interval

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_HID_ENDPOINT_NUMBER_IDX0} | USB_EP_DIRECTION_OUT,     // EndpointAddress
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // size
    0x01,                           // Interval
    
	<#assign usbDeviceHIDReportCount = usbDeviceHIDReportCount+1>
    
    <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_MSD_IDX0 == true>
     /* Descriptor for Function 3 - MSD     */ 
    /* Interface Descriptor */

    9,                              // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},                              // Interface Number
    0,                              // Alternate Setting Number
    2,                              // Number of endpoints in this intf
    USB_MSD_CLASS_CODE,             // Class code
    USB_MSD_SUBCLASS_CODE_SCSI_TRANSPARENT_COMMAND_SET, // Subclass code
    USB_MSD_PROTOCOL,               // Protocol code
    0,                              // Interface string index

    /* Endpoint Descriptor */

    7,                          // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_MSD_ENDPOINT_NUMBER_IDX0} | USB_EP_DIRECTION_IN, // EndpointAddress 
    USB_TRANSFER_TYPE_BULK,     // Attributes type of EP (BULK)
    <#if usbDeviceSpeed == 1>
    0x00, 0x02,                 // Max packet size of this EP
    <#elseif usbDeviceSpeed == 2 >
    0x40,0x00,                  // Max packet size of this EP
    </#if>
    0x00,                       // Interval (in ms)


    7,                          // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_MSD_ENDPOINT_NUMBER_IDX0} | USB_EP_DIRECTION_OUT,// EndpointAddress 
    USB_TRANSFER_TYPE_BULK,     // Attributes type of EP (BULK)
    <#if usbDeviceSpeed == 1>
    0x00, 0x02,                 // Max packet size of this EP
    <#elseif usbDeviceSpeed == 2 >
    0x40,0x00,                  // Max packet size of this EP
    </#if>
    0x00,                        // Interval (in ms)
    
    <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX0 == true>
	<#if CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "usb_speaker_demo">
    /* Descriptor for Function 3 - Audio     */ 
        /* USB Speaker Standard Audio Control Interface Descriptor	*/
	0x09,                            // Size of this descriptor in bytes (bLength)
	USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
	${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},
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

    ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0},                           /* The number of AudioStreaming interfaces
                                     * in the Audio Interface Collection to which
                                     * this AudioControl interface belongs
                                     * (bInCollection) */

    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},                           /* AudioStreaming interface 1 belongs to this
                                     * AudioControl interface. (baInterfaceNr(1))*/

    /* USB Speaker Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,	    // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    0x01,          // ID of this Terminal.(bTerminalID)
    0x01,0x01,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x02,                           // Two Channels (bNrChannels)
    0x03,0x00,                      // (wChannelConfig)
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)

    /* USB Speaker Feature Unit Descriptor */
    0x0A,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x05,            // ID of this Unit ( bUnitID  ).
    0x01,          // Input terminal is connected to this unit(bSourceID)
    0x01,                           // (bControlSize) //was 0x03
    0x01,                           // (bmaControls(0)) Controls for Master Channel
    0x00,                           // (bmaControls(1)) Controls for Channel 1
    0x00,                           // (bmaControls(2)) Controls for Channel 2
    0x00,			    //  iFeature

    /* USB Speaker Output Terminal Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    0x02,          // ID of this Terminal.(bTerminalID)
    0x01,0x03,                       // (wTerminalType)See USB Audio Terminal Types.
    0x00,                            // No association (bAssocTerminal)
    0x05,             // (bSourceID)
    0x00,                            // Unused. (iTerminal)

    /* USB Speaker Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},	 // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Speaker Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},	 // Interface Number  (bInterfaceNumber)
    0x01,                            // Alternate Setting Number (bAlternateSetting)
    0x01,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /*  USB Speaker Class-specific AS General Interface Descriptor */
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    // GENERAL subtype (bDescriptorSubtype)
    0x01,           // Unit ID of the Output Terminal.(bTerminalLink)
    0x01,                            // Interface delay. (bDelay)
    0x01,0x00,                       // PCM Format (wFormatTag)

    /*  USB Speaker Type 1 Format Type Descriptor */
    0x0B,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,          // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                            // FORMAT_TYPE_1. (bFormatType)
    0x02,                            // two channel.(bNrChannels)
    0x02,                            // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                            // 16 bits per sample.(bBitResolution)
    0x01,                            // One frequency supported. (bSamFreqType)
    0x80,0xBB,0x00,                  // Sampling Frequency = 48000 Hz(tSamFreq)

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
	
	<#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "usb_microphone_demo">
	/* Descriptor for Function 1 - Audio     */ 
    
   /* USB Speaker Standard Audio Control Interface Descriptor	*/
	0x09,                            // Size of this descriptor in bytes (bLength)
	USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
	${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},
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

    ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0},                           /* The number of AudioStreaming interfaces
                                     * in the Audio Interface Collection to which
                                     * this AudioControl interface belongs
                                     * (bInCollection) */

    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},                           /* AudioStreaming interface 1 belongs to this
                                     * AudioControl interface. (baInterfaceNr(1))*/

    /* USB Microphone Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,	    // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    0x04,          // ID of this Terminal.(bTerminalID)
    0x01,0x02,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x01,                           // One Channels (bNrChannels)
    0x04,0x00,                      // (wChannelConfig)
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)

    /* USB Microphone Feature Unit Descriptor */
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x05,        // ID of this Unit ( bUnitID  ).
    0x04,      // Input terminal is connected to this unit(bSourceID)
    0x02,                           // (bControlSize) //was 0x03
    0x01, 0x00,                      // (bmaControls(0)) Controls for Master Channel
    0x00, 0x00,                      // (bmaControls(1)) Controls for Channel 1
    0x00,			    			//  iFeature

    /* USB Microphone Output Terminal Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    0x06,          // ID of this Terminal.(bTerminalID)
    0x01,0x01,                       // (wTerminalType)See USB Audio Terminal Types.
    0x00,                            // No association (bAssocTerminal)
    0x05,             // (bSourceID)
    0x00,                            // Unused. (iTerminal)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},	 // Interface Number  (bInterfaceNumber)
    0x00,                            // Alternate Setting Number (bAlternateSetting)
    0x00,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                            // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},	 // Interface Number  (bInterfaceNumber)
    0x01,                            // Alternate Setting Number (bAlternateSetting)
    0x01,                            // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,            // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,        // Subclass code (bInterfaceSubclass)
    0x00,                            // Protocol code  (bInterfaceProtocol)
    0x00,                            // Interface string index (iInterface)

    /*  USB Microphone Class-specific AS General Interface Descriptor */
    0x07,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    // GENERAL subtype (bDescriptorSubtype)
    0x06,           // Unit ID of the Output Terminal.(bTerminalLink)
    0x00,                            // Interface delay. (bDelay)
    0x01,0x00,                       // PCM Format (wFormatTag)

    /*  USB Microphone Type 1 Format Type Descriptor */
    0x0B,                            // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     // CS_INTERFACE Descriptor Type (bDescriptorType)
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
    0x81,                            // IN Endpoint 1. (bEndpointAddress)
    0x05,                            /* ?(bmAttributes) Isochronous,
                                      * Adaptive, data endpoint */
    0x20,0x00,                      // ?(wMaxPacketSize) 32
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
    0x00,0x00,                        // Unused. (wLockDelay)
    
	<#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "usb_headset_demo">
	/* Descriptor for Function 1 - Audio     */ 
    
	/* USB Headset Standard Audio Control Interface Descriptor	*/
	0x09,                            // Size of this descriptor in bytes (bLength)
	USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
	${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},
	0x00,                            // Alternate Setting Number (bAlternateSetting)
	0x00,                            // Number of endpoints in this intf (bNumEndpoints)
	USB_AUDIO_CLASS_CODE,			 // Class code  (bInterfaceClass)
                                     
	USB_AUDIO_AUDIOCONTROL,          // Subclass code (bInterfaceSubclass)
	USB_AUDIO_PR_PROTOCOL_UNDEFINED, // Protocol code  (bInterfaceProtocol)
	0x00,                            // Interface string index (iInterface)

	/* USB Headset Class-specific AC Interface Descriptor  */
    0x0A,                           // Size of this descriptor in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_HEADER,               // HEADER descriptor subtype 	(bDescriptorSubtype)
    0x00,0x01,                      /* Audio Device compliant to the USB Audio
                                     * specification version 1.00 (bcdADC) */

    0x64,0x00,                      /* Total number of bytes returned for the
                                     * class-specific AudioControl interface
                                     * descriptor. (wTotalLength). Includes the
                                     * combined length of this descriptor header
                                     * and all Unit and Terminal descriptors. */

    ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0},                           
									/* The number of AudioStreaming interfaces
                                     * in the Audio Interface Collection to which
                                     * this AudioControl interface belongs
                                     * (bInCollection) */

    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},                           /* AudioStreaming interface 1 belongs to this
                                     * AudioControl interface. (baInterfaceNr(1))*/
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 2},                           
									/* AudioStreaming interface 2 belongs to this
                                     * AudioControl interface. (baInterfaceNr(2))*/

    /* USB Headset Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,	    // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    0x01,          // ID of this Terminal.(bTerminalID)
    0x01,0x01,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x02,                           // Two Channels (bNrChannels)
    0x03,0x00,                      // (wChannelConfig)
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)
    
    /* USB Headset Microphone Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,	    // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    0x04,          // ID of this Terminal.(bTerminalID)
    0x01,0x02,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x01,                           // One Channel (bNrChannels)
    0x04,0x00,                      // (wChannelConfig) 0x0004
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)

    /* USB Headset Feature Unit ID8 Descriptor */
    0x0D,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x02,            // ID of this Unit ( bUnitID  ).
    0x08,          	// Input terminal is connected to this unit(bSourceID)
    0x02,                           // (bControlSize) 
    0x01,0x00,                      // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                      // (bmaControls(1)) Controls for Channel 1
    0x00,0x00,                      // (bmaControls(2)) Controls for Channel 2
    0x00,			    			//  iFeature
    
    /* USB Headset Feature Unit ID5 Descriptor */
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x05, // ID of this Unit ( bUnitID  ).
    0x04,// Input terminal is connected to this unit(bSourceID)
    0x02,                           // (bControlSize) //was 0x03
    0x01,0x00,                      // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                      // (bmaControls(1)) Controls for Channel 1
    0x00,                   		//  iFeature
    
    /* USB Headset Feature Unit ID7 Descriptor */
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x07,// ID of this Unit ( bUnitID  ).
    0x04,// Input terminal is connected to this unit(bSourceID)
    0x02,                           // (bControlSize) //was 0x03
    0x01,0x00,                      // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                      // (bmaControls(1)) Controls for Channel 1
    0x00,			    			//  iFeature
    
    /* USB Headset Mixer Unit ID8 Descriptor */
    0x0D,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_MIXER_UNIT,         	// FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x08,            	// ID of this Unit ( bUnitID  ).
    2,                      		// Number of input pins
    0x01, 			// sourceID 1
    0x07,// sourceID 2
    0x02,                           // number of channels
    0x03,0x00,                      // channel config
    0x00,			    			//  iChannelNames
    0x00,               			//bmControls
    0x00,               			//iMixer
    
    /* USB Headset Output Terminal Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    0x03,         // ID of this Terminal.(bTerminalID)
    0x02,0x03,                      // (wTerminalType)See USB Audio Terminal Types.
    0x00,                           // No association (bAssocTerminal)
    0x02,            // (bSourceID)
    0x00,                           // Unused. (iTerminal)

    /* USB Headset Output Terminal Microphone Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    0x06,// ID of this Terminal.(bTerminalID)
    0x01,0x01,                       // (wTerminalType)See USB Audio Terminal Types.
    0x00,                            // No association (bAssocTerminal)
    0x05,  // (bSourceID)
    0x00,                            // Unused. (iTerminal)

    /* USB Headset Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},
									// Interface Number  (bInterfaceNumber)
    0x00,                           // Alternate Setting Number (bAlternateSetting)
    0x00,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                           // Interface string index (iInterface)

    /* USB Headset Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                         	// Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},
									// Interface Number  (bInterfaceNumber)
    0x01,                           // Alternate Setting Number (bAlternateSetting)
    0x01,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                           // Interface string index (iInterface)

    /*  USB Headset Class-specific AS General Interface Descriptor */
    0x07,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    		// GENERAL subtype (bDescriptorSubtype)
    0x01,          // Unit ID of the Output Terminal.(bTerminalLink)
    0x01,                           // Interface delay. (bDelay)
    0x01,0x00,                      // PCM Format (wFormatTag)

    /*  USB Headset Type 1 Format Type Descriptor */
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,         // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                           // FORMAT_TYPE_1. (bFormatType)
    0x02,                           // two channel.(bNrChannels)
    0x02,                           // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                           // 16 bits per sample.(bBitResolution)
    0x01,                           // One frequency supported. (bSamFreqType)
    0x80,0x3E,0x00,                 // Sampling Frequency = 16000 Hz(tSamFreq)

    /*  USB Headset Standard Endpoint Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,        // ENDPOINT descriptor (bDescriptorType)
    0x01,                           // OUT Endpoint 1. (bEndpointAddress)
    0x09,                           /* ?(bmAttributes) Isochronous,
                                     * asynchronous, data endpoint */
    0x40,0x00,                      // ?(wMaxPacketSize) //48 * 4
    0x01,                           // One packet per frame.(bInterval)
    0x00,                           // Unused. (bRefresh)
    0x00,                           // Unused. (bSynchAddress)

    /* USB Speaker Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,          // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,           // GENERAL subtype. (bDescriptorSubtype)
    0x00,                           /* No sampling frequency control, no pitch
                                     * control, no packet padding.(bmAttributes)*/
    0x00,                           // Unused. (bLockDelayUnits)
    0x00,0x00,                      // Unused. (wLockDelay)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 2},
									// Interface Number  (bInterfaceNumber)
    0x00,                           // Alternate Setting Number (bAlternateSetting)
    0x00,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                           // Interface string index (iInterface)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 2},
									// Interface Number  (bInterfaceNumber)
    0x01,                           // Alternate Setting Number (bAlternateSetting)
    0x01,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                         	// Interface string index (iInterface)

    /*  USB Microphone Class-specific AS General Interface Descriptor */
    0x07,                         	// Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    		// GENERAL subtype (bDescriptorSubtype)
    0x06,           // Unit ID of the Output Terminal.(bTerminalLink)
    0x00,                           // Interface delay. (bDelay)
    0x01,0x00,                      // PCM Format (wFormatTag)

    /*  USB Microphone Type 1 Format Type Descriptor */
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,         // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                           // FORMAT_TYPE_1. (bFormatType)
    0x01,                           // one channel.(bNrChannels)
    0x02,                           // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                           // 16 bits per sample.(bBitResolution)
    0x01,                           // One frequency supported. (bSamFreqType)
    0x80,0x3E,0x00,                 // Sampling Frequency = 16000 Hz(tSamFreq)

    /*  USB Microphone Standard Endpoint Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,        // ENDPOINT descriptor (bDescriptorType)
    0x081,                           // OUT Endpoint 1. (bEndpointAddress)
    0x0D,                           /* ?(bmAttributes) Isochronous,
                                     * asynchronous, data endpoint */
    0x20, 0x00,                     // ?(wMaxPacketSize) //48 * 2
    0x01,                           // One packet per frame.(bInterval)
    0x00,                           // Unused. (bRefresh)
    0x00,                           // Unused. (bSynchAddress)

    /* USB Microphone Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,          // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,           // GENERAL subtype. (bDescriptorSubtype)
    0x00,                           /* No sampling frequency control, no pitch
                                       control, no packet padding.(bmAttributes)*/
    0x00,                           // Unused. (bLockDelayUnits)
    0x00,0x00,                      // Unused. (wLockDelay)
	
	<#elseif CONFIG_USB_DEVICE_PRODUCT_ID_SELECT_IDX0 == "usb_headset_multiple_sampling_demo">
	/* Descriptor for Function 1 - Audio     */ 
    
	/* USB Headset Standard Audio Control Interface Descriptor	*/
	0x09,                            // Size of this descriptor in bytes (bLength)
	USB_DESCRIPTOR_INTERFACE,        // INTERFACE descriptor type (bDescriptorType)
	${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},
	0x00,                            // Alternate Setting Number (bAlternateSetting)
	0x00,                            // Number of endpoints in this intf (bNumEndpoints)
	USB_AUDIO_CLASS_CODE,			 // Class code  (bInterfaceClass)
                                     
	USB_AUDIO_AUDIOCONTROL,          // Subclass code (bInterfaceSubclass)
	USB_AUDIO_PR_PROTOCOL_UNDEFINED, // Protocol code  (bInterfaceProtocol)
	0x00,                            // Interface string index (iInterface)

	/* USB Headset Class-specific AC Interface Descriptor  */
    0x0A,                           // Size of this descriptor in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,         // CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_HEADER,               // HEADER descriptor subtype 	(bDescriptorSubtype)
    0x00,0x01,                      /* Audio Device compliant to the USB Audio
                                     * specification version 1.00 (bcdADC) */

    0x64,0x00,                      /* Total number of bytes returned for the
                                     * class-specific AudioControl interface
                                     * descriptor. (wTotalLength). Includes the
                                     * combined length of this descriptor header
                                     * and all Unit and Terminal descriptors. */

    ${CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0},                           
									/* The number of AudioStreaming interfaces
                                     * in the Audio Interface Collection to which
                                     * this AudioControl interface belongs
                                     * (bInCollection) */

    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},                           /* AudioStreaming interface 1 belongs to this
                                     * AudioControl interface. (baInterfaceNr(1))*/
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 2},                           
									/* AudioStreaming interface 2 belongs to this
                                     * AudioControl interface. (baInterfaceNr(2))*/

    /* USB Headset Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,	    // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    0x01,          					// ID of this Terminal.(bTerminalID)
    0x01,0x01,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x02,                           // Two Channels (bNrChannels)
    0x03,0x00,                      // (wChannelConfig)
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)
    
    /* USB Headset Microphone Input Terminal Descriptor */
    0x0C,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_INPUT_TERMINAL,	    // INPUT_TERMINAL descriptor subtype (bDescriptorSubtype)
    0x04,          					// ID of this Terminal.(bTerminalID)
    0x01,0x02,                      // (wTerminalType)
    0x00,                           // No association (bAssocTerminal)
    0x01,                           // One Channel (bNrChannels)
    0x04,0x00,                      // (wChannelConfig) 0x0004
    0x00,                           // Unused.(iChannelNames)
    0x00,                           // Unused. (iTerminal)

    /* USB Headset Feature Unit ID8 Descriptor */
    0x0D,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x02,            				// ID of this Unit ( bUnitID  ).
    0x08,          					// Input terminal is connected to this unit(bSourceID)
    0x02,                           // (bControlSize) 
    0x01,0x00,                      // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                      // (bmaControls(1)) Controls for Channel 1
    0x00,0x00,                      // (bmaControls(2)) Controls for Channel 2
    0x00,			    			//  iFeature
    
    /* USB Headset Feature Unit ID5 Descriptor */
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x05, 							// ID of this Unit ( bUnitID  ).
    0x04,							// Input terminal is connected to this unit(bSourceID)
    0x02,                           // (bControlSize) //was 0x03
    0x01,0x00,                      // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                      // (bmaControls(1)) Controls for Channel 1
    0x00,                   		//  iFeature
    
    /* USB Headset Feature Unit ID7 Descriptor */
    0x0B,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FEATURE_UNIT,         // FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x07,							// ID of this Unit ( bUnitID  ).
    0x04,							// Input terminal is connected to this unit(bSourceID)
    0x02,                           // (bControlSize) //was 0x03
    0x01,0x00,                      // (bmaControls(0)) Controls for Master Channel
    0x00,0x00,                      // (bmaControls(1)) Controls for Channel 1
    0x00,			    			//  iFeature
    
    /* USB Headset Mixer Unit ID8 Descriptor */
    0x0D,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_MIXER_UNIT,         	// FEATURE_UNIT  descriptor subtype (bDescriptorSubtype)
    0x08,            				// ID of this Unit ( bUnitID  ).
    2,                      		// Number of input pins
    0x01, 							// sourceID 1
    0x07,							// sourceID 2
    0x02,                           // number of channels
    0x03,0x00,                      // channel config
    0x00,			    			//  iChannelNames
    0x00,               			//bmControls
    0x00,               			//iMixer
    
    /* USB Headset Output Terminal Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    0x03,         					// ID of this Terminal.(bTerminalID)
    0x02,0x03,                      // (wTerminalType)See USB Audio Terminal Types.
    0x00,                           // No association (bAssocTerminal)
    0x02,            				// (bSourceID)
    0x00,                           // Unused. (iTerminal)

    /* USB Headset Output Terminal Microphone Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,    		// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_OUTPUT_TERMINAL,      // OUTPUT_TERMINAL  descriptor subtype (bDescriptorSubtype)
    0x06,							// ID of this Terminal.(bTerminalID)
    0x01,0x01,                      // (wTerminalType)See USB Audio Terminal Types.
    0x00,                           // No association (bAssocTerminal)
    0x05,  							// (bSourceID)
    0x00,                           // Unused. (iTerminal)

    /* USB Headset Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},
									// Interface Number  (bInterfaceNumber)
    0x00,                           // Alternate Setting Number (bAlternateSetting)
    0x00,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                           // Interface string index (iInterface)

    /* USB Headset Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                         	// Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 1},
									// Interface Number  (bInterfaceNumber)
    0x01,                           // Alternate Setting Number (bAlternateSetting)
    0x01,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                           // Interface string index (iInterface)

    /*  USB Headset Class-specific AS General Interface Descriptor */
    0x07,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    		// GENERAL subtype (bDescriptorSubtype)
    0x01,          					// Unit ID of the Output Terminal.(bTerminalLink)
    0x01,                           // Interface delay. (bDelay)
    0x01,0x00,                      // PCM Format (wFormatTag)

    /*  USB Headset Type 1 Format Type Descriptor */
    0x11,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,         // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                           // FORMAT_TYPE_1. (bFormatType)
    0x02,                           // two channel.(bNrChannels)
    0x02,                           // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                           // 16 bits per sample.(bBitResolution)
    0x03,                           // Three frequency supported. (bSamFreqType)
    0x80,0x3E,0x00,                 // Sampling Frequency = 16000 Hz(tSamFreq)
	0x00,0x7D,0x00,                 // Sampling Frequency = 32000 Hz(tSamFreq)
	0x80,0xBB,0x00,                 // Sampling Frequency = 48000 Hz(tSamFreq)

    /*  USB Headset Standard Endpoint Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,        // ENDPOINT descriptor (bDescriptorType)
    0x01,                           // OUT Endpoint 1. (bEndpointAddress)
    0x09,                           /* ?(bmAttributes) Isochronous,
                                     * asynchronous, data endpoint */
    0xC0,0x00,                      // ?(wMaxPacketSize) //48 * 4
    0x01,                           // One packet per frame.(bInterval)
    0x00,                           // Unused. (bRefresh)
    0x00,                           // Unused. (bSynchAddress)

    /* USB Speaker Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,          // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,           // GENERAL subtype. (bDescriptorSubtype)
    0x01,                           /* Turn on sampling frequency control, no pitch
                                     * control, no packet padding.(bmAttributes)*/
    0x00,                           // Unused. (bLockDelayUnits)
    0x00,0x00,                      // Unused. (wLockDelay)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 0) */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 2},
									// Interface Number  (bInterfaceNumber)
    0x00,                           // Alternate Setting Number (bAlternateSetting)
    0x00,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                           // Interface string index (iInterface)

    /* USB Microphone Standard AS Interface Descriptor (Alt. Set. 1) */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_INTERFACE,       // INTERFACE descriptor type (bDescriptorType)
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0?number + 2},
									// Interface Number  (bInterfaceNumber)
    0x01,                           // Alternate Setting Number (bAlternateSetting)
    0x01,                           // Number of endpoints in this intf (bNumEndpoints)
    USB_AUDIO_CLASS_CODE,           // Class code  (bInterfaceClass)
    USB_AUDIO_AUDIOSTREAMING,       // Subclass code (bInterfaceSubclass)
    0x00,                           // Protocol code  (bInterfaceProtocol)
    0x00,                         	// Interface string index (iInterface)

    /*  USB Microphone Class-specific AS General Interface Descriptor */
    0x07,                         	// Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_AS_GENERAL,    		// GENERAL subtype (bDescriptorSubtype)
    0x06,           				// Unit ID of the Output Terminal.(bTerminalLink)
    0x00,                           // Interface delay. (bDelay)
    0x01,0x00,                      // PCM Format (wFormatTag)

    /*  USB Microphone Type 1 Format Type Descriptor */
    0x11,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_INTERFACE,     	// CS_INTERFACE Descriptor Type (bDescriptorType)
    USB_AUDIO_FORMAT_TYPE ,         // FORMAT_TYPE subtype. (bDescriptorSubtype)
    0x01,                           // FORMAT_TYPE_1. (bFormatType)
    0x01,                           // one channel.(bNrChannels)
    0x02,                           // 2 byte per audio subframe.(bSubFrameSize)
    0x10,                           // 16 bits per sample.(bBitResolution)
    0x03,                           // Three frequency supported. (bSamFreqType)
    0x80,0x3E,0x00,                 // Sampling Frequency = 16000 Hz(tSamFreq)
	0x00,0x7D,0x00,                 // Sampling Frequency = 32000 Hz(tSamFreq)
	0x80,0xBB,0x00,                 // Sampling Frequency = 48000 Hz(tSamFreq)
	
    /*  USB Microphone Standard Endpoint Descriptor */
    0x09,                           // Size of the descriptor, in bytes (bLength)
    USB_DESCRIPTOR_ENDPOINT,        // ENDPOINT descriptor (bDescriptorType)
    0x081,                           // OUT Endpoint 1. (bEndpointAddress)
    0x0D,                           /* ?(bmAttributes) Isochronous,
                                     * asynchronous, data endpoint */
    0x60, 0x00,                     // ?(wMaxPacketSize) //48 * 2
    0x01,                           // One packet per frame.(bInterval)
    0x00,                           // Unused. (bRefresh)
    0x00,                           // Unused. (bSynchAddress)

    /* USB Microphone Class-specific Isoc. Audio Data Endpoint Descriptor*/
    0x07,                           // Size of the descriptor, in bytes (bLength)
    USB_AUDIO_CS_ENDPOINT,          // CS_ENDPOINT Descriptor Type (bDescriptorType)
    USB_AUDIO_EP_GENERAL,           // GENERAL subtype. (bDescriptorSubtype)
    0x01,                           /* Turn on sampling frequency control, no pitch
                                       control, no packet padding.(bmAttributes)*/
    0x00,                           // Unused. (bLockDelayUnits)
    0x00,0x00,                      // Unused. (wLockDelay)
	
	</#if>
    
	<#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_VENDOR_IDX0 == true>
     /* Descriptor for Function 1 - Vendor     */ 
    /* Interface Descriptor */

    0x09,                       // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,   // INTERFACE descriptor type
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},                          // Interface Number
    0,                          // Alternate Setting Number
    2,                          // Number of endpoints in this intf
    0xFF,                       // Class code
    0xFF,                       // Subclass code
    0xFF,                       // Protocol code
    0,                          // Interface string index

    /* Endpoint Descriptor 1 */

    0x07,                       // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_VENDOR_ENDPOINT_NUMBER_IDX0} |USB_EP_DIRECTION_OUT,   // EndpointAddress
    USB_TRANSFER_TYPE_BULK,     // Attributes
    <#if usbDeviceSpeed == 1>
    0x00, 0x02,                 // Max packet size of this EP
    <#elseif usbDeviceSpeed == 2 >
    0x40,0x00,                  // Max packet size of this EP
    </#if>
    1,                          // Interval

    /* Endpoint Descriptor 2 */

    0x07,                       // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    ${CONFIG_USB_DEVICE_FUNCTION_3_VENDOR_ENDPOINT_NUMBER_IDX0} |USB_EP_DIRECTION_IN,    // EndpointAddress
    USB_TRANSFER_TYPE_BULK,     // Attributes
    <#if usbDeviceSpeed == 1>
    0x00, 0x02,                 // Max packet size of this EP
    <#elseif usbDeviceSpeed == 2 >
    0x40,0x00,                  // Max packet size of this EP
    </#if>
    1,                           // Interval
  
     <#elseif CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
	/* Interface Association Descriptor: */
    0x08,									// bLength
    USB_AUDIO_V2_DESCRIPTOR_IA,				// bDescriptorType
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},
											// bFirstInterface
    ${CONFIG_USB_DEVICE_FUNCTION_3_NUMBER_OF_INTERFACES_IDX0},
											// bInterfaceCount
    AUDIO_V2_FUNCTION,						// bFunctionClass   (Audio Device Class)
    AUDIO_V2_FUNCTION_SUBCLASS_UNDEFINED,	// bFunctionSubClass
    AUDIO_V2_AF_VERSION_02_00,				// bFunctionProtocol
    0x00,									// iFunction
    // Interface Descriptor:
    0x09,									// bLength
    USB_DESCRIPTOR_INTERFACE,				// bDescriptorType
    ${CONFIG_USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX0},	
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
    0x28,					// bClockID
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
    0x11,					// bClockID
    0x01,									// bNrInPins
    0x28,					// baCSourceID(1)
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
    0x11,					// bCSourceID
    0x02,									// bNrChannels
    0x00, 0x00, 0x00, 0x00,					// bmChannelConfig N/A Front Left/Right
    0x00,									// iChannelNames
    0x00, 0x00,								// bmControls
    0x00,									// iTerminal
    // AC Feature Unit Descriptor:
    0x12,									// bLength
    AUDIO_V2_CS_INTERFACE,					// bDescriptorType
    AUDIO_V2_FEATURE_UNIT,					// bDescriptorSubtype
    0x0A,					// bUnitID
    0x02,					// bSourceID
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
    0x0A,					// bSourceID
    0x11,					// bCSourceID
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
    0x02,					// bTerminalLink/Input Terminal
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

</#if>