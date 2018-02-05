config  USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
       ---endhelp---

config USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

config USB_DEVICE_FUNCTION_8_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

config USB_DEVICE_FUNCTION_8_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR" || USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO" || USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

config USB_DEVICE_FUNCTION_8_SPEED_HS_IDX${INSTANCE}
       string "Speed"
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

config USB_DEVICE_FUNCTION_8_SPEED_FS_IDX${INSTANCE}
       string
       depends on DRV_USB_DEVICE_SUPPORT && (PIC32MX || PIC32MK || PIC32WK)
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n

config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n

config USB_DEVICE_FUNCTION_8_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_CDC_INT_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Interrupt Endpoint Number"
       range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
       range 1 7  if PIC32MZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_CDC_BULK_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number"
       range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
       range 1 7  if PIC32MZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_HID_DEVICE_TYPE_IDX${INSTANCE}
       string
       range USB_DEVICE_HID_TYPE
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX${INSTANCE}
       default "Other"
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_HID_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Endpoint Number"
       range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
       range 1 7  if PIC32MZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX${INSTANCE}
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_MSD_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Endpoint Number"
       range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
       range 1 7  if PIC32MZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX${INSTANCE}
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX${INSTANCE}
       range 0 1
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_MSD_NUM_LUN_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default n if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX${INSTANCE} = 0
       default y

config USB_DEVICE_FUNCTION_8_LUN_IDX${INSTANCE}
       depends on DRV_USB_DEVICE_SUPPORT
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_8_MSD_NUM_LUN_IDX${INSTANCE}
       bool "LUN 0"
       default y
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

ifblock USB_DEVICE_FUNCTION_8_LUN_IDX${INSTANCE}

config  USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA${INSTANCE}
       string "Media Type"
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_8_MSD_NUM_LUN_IDX${INSTANCE}
       range USB_DEVICE_MSD_TYPE
       default "NVM"
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA_NVM${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_8_MSD_NUM_LUN_IDX${INSTANCE}
       select DRV_NVM_NEEDED
	   select USE_DRV_NVM_ERASE_WRITE_NEEDED
       set USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} to "msd_basic_demo" if USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA_NVM${INSTANCE}
       default y if USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA${INSTANCE} = "NVM"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA_SPIFLASH${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_8_MSD_NUM_LUN_IDX${INSTANCE}
       select USE_DRV_SST25_NEEDED
       set USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} to "msd_spiflash_demo" if USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA_SPIFLASH${INSTANCE}
       default y if USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA${INSTANCE} = "SPI_FLASH"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA_SDCARD${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_8_MSD_NUM_LUN_IDX${INSTANCE}
       select DRV_SDCARD_NEEDED
       set USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} to "msd_basic_sdcard_demo" if USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA_SDCARD${INSTANCE}
       default y if USB_DEVICE_FUNCTION_8_MSD_LUN_MEDIA${INSTANCE} = "SDCARD"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

endif

config USB_DEVICE_FUNCTION_8_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on DRV_USB_DEVICE_SUPPORT
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on DRV_USB_DEVICE_SUPPORT
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---

config USB_DEVICE_FUNCTION_8_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on DRV_USB_DEVICE_SUPPORT
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

config USB_DEVICE_FUNCTION_8_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on DRV_USB_DEVICE_SUPPORT
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---

config USB_DEVICE_FUNCTION_8_VENDOR_ENDPOINT_NUMBER_IDX${INSTANCE}
        int "Endpoint Number"
        range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
        range 1 7  if PIC32MZ
        depends on DRV_USB_DEVICE_SUPPORT
        depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
        ---help---
        IDH_HTML_USB_DEVICE_INIT 
        ---endhelp---


