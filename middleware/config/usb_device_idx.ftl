config USB_DEVICE_INSTANCES_NUMBER_GT_${INSTANCE+1}
    bool
	depends on DRV_USB_DEVICE_SUPPORT
    <#if INSTANCE != 0>
	default n if USB_DEVICE_INSTANCES_NUMBER_GT_${INSTANCE} = n
    </#if>
	default n if USB_DEVICE_INSTANCES_NUMBER = ${INSTANCE+1}
	default y

config USB_DEVICE_INST_IDX${INSTANCE}
    depends on DRV_USB_DEVICE_SUPPORT
    <#if INSTANCE != 0> && USB_DEVICE_INSTANCES_NUMBER_GT_${INSTANCE}
    </#if>
    bool "USB Device Instance ${INSTANCE}"
    default y
    ---help---
    ---endhelp---

ifblock USB_DEVICE_INST_IDX${INSTANCE}		
	config USB_DEVICE_SPEED_FS_IDX${INSTANCE}
        string
        depends on DRV_USB_DEVICE_SUPPORT && (PIC32MX || PIC32MK || PIC32WK)
        range USB_DEVICE_SPEED_FS
        default "USB_SPEED_FULL"
        ---help---
        IDH_HTML_USB_DEVICE_INIT
        ---endhelp---
        
	
	config USB_DEVICE_SPEED_HSV1_IDX${INSTANCE}
        string "Device Speed Configuration"
        depends on DRV_USB_DEVICE_SUPPORT && DSTBDPIC32CZ
		range USB_DEVICE_SPEED_HSV1
        default "USB_SPEED_NORMAL"
        ---help---
        IDH_HTML_USB_DEVICE_INIT

        ---endhelp---
			
			
config USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE}
        int "Number of Functions Registered to this Device Instance"
        depends on DRV_USB_DEVICE_SUPPORT
        range 0 10
        default 1
        ---help---
        IDH_HTML_USB_DEVLAYER_Function_Driver_Registration_Table
        ---endhelp---

config USB_DEVICE_FUNCTION_1_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 0
        default y

config USB_DEVICE_FUNCTION_1_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_GT_IDX${INSTANCE}
        bool "Function 1"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

ifblock USB_DEVICE_FUNCTION_1_IDX${INSTANCE}
source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_1.ftl" 1 instances
   endif

 config USB_DEVICE_FUNCTION_2_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 1
        default n if USB_DEVICE_FUNCTION_1_GT_IDX${INSTANCE} = n
        default y
		
   config USB_DEVICE_FUNCTION_2_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_2_GT_IDX${INSTANCE}
        bool "Function 2"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_2_IDX${INSTANCE}
    source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_2.ftl"  1 instances
   endif

 
   
    config USB_DEVICE_FUNCTION_3_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 2
        default n if USB_DEVICE_FUNCTION_2_GT_IDX${INSTANCE} = n
        default y

	config USB_DEVICE_FUNCTION_3_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_3_GT_IDX${INSTANCE}
        bool "Function 3"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

	ifblock USB_DEVICE_FUNCTION_3_IDX${INSTANCE}
      source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_3.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_4_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 3
        default n if USB_DEVICE_FUNCTION_3_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_4_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_4_GT_IDX${INSTANCE}
        bool "Function 4"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_4_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_4.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_5_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 4
        default n if USB_DEVICE_FUNCTION_4_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_5_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_5_GT_IDX${INSTANCE}
        bool "Function 5"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_5_IDX${INSTANCE}
        source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_5.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_6_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 5
        default n if USB_DEVICE_FUNCTION_5_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_6_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_6_GT_IDX${INSTANCE}
        bool "Function 6"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_6_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_6.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_7_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 6
        default n if USB_DEVICE_FUNCTION_6_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_7_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_7_GT_IDX${INSTANCE}
        bool "Function 7"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_7_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_7.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_8_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 7
        default n if USB_DEVICE_FUNCTION_7_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_8_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_8_GT_IDX${INSTANCE}
        bool "Function 8"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_8_IDX${INSTANCE}
        source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_8.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_9_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 8
        default n if USB_DEVICE_FUNCTION_8_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_9_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_9_GT_IDX${INSTANCE}
        bool "Function 9"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_9_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_9.ftl"  1 instances
   endif
   
config USB_DEVICE_FUNCTION_10_GT_IDX${INSTANCE}
        bool
	depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 9
        default n if USB_DEVICE_FUNCTION_9_GT_IDX${INSTANCE} = n
        default y
		
config USB_DEVICE_FUNCTION_10_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_10_GT_IDX${INSTANCE}
        bool "Function 10"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_10_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_10.ftl"  1 instances
   endif

config USB_DEVICE_ENDPOINT_QUEUE_SIZE_READ_IDX${INSTANCE}
    int "Endpoint Read Queue Size"
    depends on DRV_USB_DEVICE_SUPPORT
    depends on USB_DEVICE_USE_ENDPOINT_FUNCTIONS
    default 1
    ---help---
    IDH_HTML_USB_DEVICE_INIT 
    ---endhelp---

config USB_DEVICE_ENDPOINT_QUEUE_SIZE_WRITE_IDX${INSTANCE}
    int "Endpoint Write Queue Size"
    depends on DRV_USB_DEVICE_SUPPORT
    depends on USB_DEVICE_USE_ENDPOINT_FUNCTIONS
    default 1
    ---help---
    IDH_HTML_USB_DEVICE_INIT 
    ---endhelp---
    
config USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE}
    string "Product ID Selection"
    depends on DRV_USB_DEVICE_SUPPORT 
    range USB_DEVICE_PRODCUCT_ID_ENUM
    default "User Defined Product ID"
    ---help---
    ---endhelp---
        
config USB_DEVICE_VENDOR_ID_IDX${INSTANCE}
    string "Enter Vendor ID"
    depends on DRV_USB_DEVICE_SUPPORT
    default "0x0000" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "User Defined Product ID"
    default "0x04D8"
    ---help---
    Enter Vendor ID in Hex format with '0x' prefix. eg:-0x4545
    ---endhelp---

config USB_DEVICE_PRODUCT_ID_IDX${INSTANCE}
    string "Enter Product ID"
    depends on DRV_USB_DEVICE_SUPPORT
    default "0x0000" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "User Defined Product ID"
    default "0x0064" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_speaker_demo"
    default "0x0065" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_microphone_demo"
    default "0x00FF" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_headset_demo"
    default "0x00FF" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_headset_multiple_sampling_demo"
    default "0xABCD" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "mac_audio_hi_res_demo"
    default "0x0208" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_com_port_dual_demo"
    default "0x000A" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_com_port_single_demo"
    default "0x0057" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_msd_basic_demo"
    default "0x000A" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_demo"
    default "0x0057" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_msd_demo"
    default "0x003F" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_basic_demo"
    default "0x005E" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_joystick_demo"
    default "0x0055" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_keyboard_demo"
    default "0x0000" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_mouse_demo"
    default "0x0054" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_msd_demo"
    default "0x0009" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_demo"
    default "0x0009" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_sdcard_demo"
    default "0x0009" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_spiflash_demo"
    default "0x0009" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_multiple_luns_demo"
    default "0x0053" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "vendor_demo"
    ---help---
    Enter Product ID in Hex format with '0x' prefix. eg:-0x5454
    ---endhelp---

config USB_DEVICE_MANUFACTURER_STRING_IDX${INSTANCE}
    string "Manufacturer String" 
    depends on DRV_USB_DEVICE_SUPPORT
    default "" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "User Defined Product ID"
    default "Microchip Technology Inc."
    ---help---
    ---endhelp---
        
config USB_DEVICE_PRODUCT_STRING_DESCRIPTOR_IDX${INSTANCE}
    string "Product String" 
    depends on DRV_USB_DEVICE_SUPPORT
    default "" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "User Defined Product ID"
    default "Harmony USB Speaker Example" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_speaker_demo"
    default "Harmony USB Microphone Example" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_microphone_demo"
    default "Harmony USB Headset Example" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_headset_demo"
    default "Harmony USB Headset Multiple Sampling Rate Example" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_headset_multiple_sampling_demo"
    default "Harmony USB Speaker 2.0" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "mac_audio_hi_res_demo"
    default "CDC Dual COM Port Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_com_port_dual_demo"
    default "Simple CDC Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_com_port_single_demo"
    default "CDC + MSD Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_msd_basic_demo"
    default "Simple CDC Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_demo"
    default "CDC + MSD Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_msd_demo"
    default "Simple HID Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_basic_demo"
    default "HID Joystick Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_joystick_demo"
    default "HID Keyboard Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_keyboard_demo"
    default "HID Mouse Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_mouse_demo"
    default "HID + MSD Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_msd_demo"
    default "Simple MSD Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_demo"
    default "Simple MSD Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_sdcard_demo"
    default "MSD SPI Flash Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_spiflash_demo"
    default "Multiple LUN MSD Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_multiple_luns_demo"
    default "Simple WinUSB Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "vendor_demo"
    ---help---
    Enter product string
    ---endhelp---
        
config USB_DEVICE_MSD_DISK_IMAGE_ADD_SUPPORT
    bool
    depends on DRV_USB_DEVICE_SUPPORT
    depends on USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_msd_demo" || 
    USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_demo" ||
    USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_msd_basic_demo" ||
    USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_msd_demo" ||
    USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_multiple_luns_demo"
    select USB_DEVICE_MSD_DISK_IMAGE_FILE_NEEDED
    default y if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_msd_demo" || 
    USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_demo" ||
    USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_msd_basic_demo" ||
    USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_msd_demo" ||
    USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_multiple_luns_demo"
    default n
endif



