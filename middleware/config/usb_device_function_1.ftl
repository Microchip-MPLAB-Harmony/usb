config  USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if SYS_CONSOLE_IDX0_USE_USB || SYS_CONSOLE_IDX1_USE_USB
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
       ---endhelp---

       config USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_1_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_1_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR" || USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO" || USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_1_SPEED_HS_IDX${INSTANCE}
       string "Speed"
       depends on DRV_USB_DEVICE_SUPPORT && (PIC32MZ)
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if (PIC32MZ)
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

        config USB_DEVICE_FUNCTION_1_SPEED_FS_IDX${INSTANCE}
       string
       depends on DRV_USB_DEVICE_SUPPORT && (PIC32MX || PIC32MK || PIC32WK)
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---


       config USB_DEVICE_FUNCTION_1_SPEED_HSV1_NORMAL_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       default y if USB_DEVICE_SPEED_HSV1_IDX${INSTANCE} = "USB_SPEED_NORMAL"
       default n

       config USB_DEVICE_FUNCTION_1_SPEED_HSV1_LOWPOWER_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       default y if USB_DEVICE_SPEED_HSV1_IDX${INSTANCE} = "USB_SPEED_LOW_POWER"
       default n

       config USB_DEVICE_FUNCTION_1_SPEED_HSV1_NORMAL_MODE_IDX${INSTANCE}
       string 
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_SPEED_HSV1_NORMAL_IDX${INSTANCE}
       range USB_DEVICE_FUNCTION_SPEED_NORMAL
       default "USB_SPEED_HIGH"
       ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_1_SPEED_HSV1_LOWPOWER_MODE_IDX${INSTANCE}
       string
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_SPEED_HSV1_LOWPOWER_IDX${INSTANCE}
       range USB_DEVICE_FUNCTION_SPEED_LOW_POWER
       default "USB_SPEED_FULL"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---



       config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---



       config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n

	   config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n

config USB_DEVICE_FUNCTION_1_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---


config USB_DEVICE_FUNCTION_1_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_CDC_INT_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Interrupt Endpoint Number"
       range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
       range 1 7  if PIC32MZ
       range 1 9  if DSTBDPIC32CZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_CDC_BULK_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number"
       range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
       range 1 7  if PIC32MZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       depends on (PIC32MX || PIC32MK || PIC32WK || PIC32MZ)
       default 2
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_CDC_BULK_ENDPOINT_OUT_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number (OUT)"
       range 1 9  if DSTBDPIC32CZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       depends on DSTBDPIC32CZ
       default 2
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_CDC_BULK_ENDPOINT_IN_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number (IN)"
       range 1 9  if DSTBDPIC32CZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       depends on DSTBDPIC32CZ
       default 3
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_HID_DEVICE_TYPE_IDX${INSTANCE}
       string
       range USB_DEVICE_HID_TYPE
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       default "Other"
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_HID_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Interrupt Endpoint Number"
       range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
       range 1 7  if PIC32MZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       depends on (PIC32MX || PIC32MK || PIC32WK || PIC32MZ)
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_HID_ENDPOINT_IN_NUMBER_IDX${INSTANCE}
       int "Interrupt Endpoint Number (IN)"
       range 1 9  if DSTBDPIC32CZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       depends on DSTBDPIC32CZ
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_HID_ENDPOINT_OUT_NUMBER_IDX${INSTANCE}
       int "Interrupt Endpoint Number (OUT)"
       range 1 9  if DSTBDPIC32CZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       depends on DSTBDPIC32CZ
       default 2
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---



config USB_DEVICE_FUNCTION_1_MSD_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number"
       range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
       range 1 7  if PIC32MZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       depends on (PIC32MX || PIC32MK || PIC32WK || PIC32MZ)
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_ENDPOINT_IN_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number (IN)"
       range 1 9  if DSTBDPIC32CZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       depends on DSTBDPIC32CZ
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_ENDPOINT_OUT_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number (OUT)"
       range 1 9  if DSTBDPIC32CZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       depends on DSTBDPIC32CZ
       default 2
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_MAX_SECTORS_IDX${INSTANCE}
       string "Max number of sectors to buffer"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       range USB_DEVICE_MSD_MAX_SECTORS
       default "1"
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       range 1 4
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_NUM_LUN0_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default n if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE} = 0
       default y

config USB_DEVICE_FUNCTION_1_LUN0_IDX${INSTANCE}
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN0_IDX${INSTANCE}
       bool "LUN 0"
       default y
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

ifblock USB_DEVICE_FUNCTION_1_LUN0_IDX${INSTANCE}

config  USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA${INSTANCE}
       string "Media Type"
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN0_IDX${INSTANCE}
       range USB_DEVICE_MSD_TYPE
       default "NVM"
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA_NVM${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN0_IDX${INSTANCE}
       select DRV_NVM_NEEDED
	   select USE_DRV_NVM_ERASE_WRITE_NEEDED
       set USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} to "msd_basic_demo" if USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA_NVM${INSTANCE}
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA${INSTANCE} = "NVM"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA_SPIFLASH${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN0_IDX${INSTANCE}
       select USE_DRV_SST25_NEEDED
       set USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} to "msd_spiflash_demo" if USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA_SPIFLASH${INSTANCE}
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA${INSTANCE} = "SPI_FLASH"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA_SDCARD${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN0_IDX${INSTANCE}
       select DRV_SDCARD_NEEDED
       set USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} to "msd_basic_sdcard_demo" if USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA_SDCARD${INSTANCE}
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN0_MEDIA${INSTANCE} = "SDCARD"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

endif


config USB_DEVICE_FUNCTION_1_MSD_NUM_LUN1_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default n if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE} = 0
	   default n if USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE} = 1
	   default n if USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE} = 0
       default y

config USB_DEVICE_FUNCTION_1_LUN1_IDX${INSTANCE}
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN1_IDX${INSTANCE}
       bool "LUN 1"
       default y
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

ifblock USB_DEVICE_FUNCTION_1_LUN1_IDX${INSTANCE}

config  USB_DEVICE_FUNCTION_1_MSD_LUN1_MEDIA${INSTANCE}
       string "Media Type"
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN1_IDX${INSTANCE}
       range USB_DEVICE_MSD_TYPE
       default "NVM"
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_LUN1_MEDIA_NVM${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN1_IDX${INSTANCE}
       select DRV_NVM_NEEDED
	   select USE_DRV_NVM_ERASE_WRITE_NEEDED
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN1_MEDIA${INSTANCE} = "NVM"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_LUN1_MEDIA_SPIFLASH${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN1_IDX${INSTANCE}
       select USE_DRV_SST25_NEEDED
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN1_MEDIA${INSTANCE} = "SPI_FLASH"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_LUN1_MEDIA_SDCARD${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN1_IDX${INSTANCE}
       select DRV_SDCARD_NEEDED
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN1_MEDIA${INSTANCE} = "SDCARD"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

endif

config USB_DEVICE_FUNCTION_1_MSD_NUM_LUN2_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default n if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE} = 0
	   default n if USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE} = 2
	   default n if USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE} = 1
	   default n if USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE} = 0
       default y

config USB_DEVICE_FUNCTION_1_LUN2_IDX${INSTANCE}
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN2_IDX${INSTANCE}
       bool "LUN 2"
       default y
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

ifblock USB_DEVICE_FUNCTION_1_LUN2_IDX${INSTANCE}

config  USB_DEVICE_FUNCTION_1_MSD_LUN2_MEDIA${INSTANCE}
       string "Media Type"
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN2_IDX${INSTANCE}
       range USB_DEVICE_MSD_TYPE
       default "NVM"
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_LUN2_MEDIA_NVM${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN2_IDX${INSTANCE}
       select DRV_NVM_NEEDED
	   select USE_DRV_NVM_ERASE_WRITE_NEEDED
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN2_MEDIA${INSTANCE} = "NVM"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

config USB_DEVICE_FUNCTION_1_MSD_LUN2_MEDIA_SPIFLASH${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN2_IDX${INSTANCE}
       select USE_DRV_SST25_NEEDED
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN2_MEDIA${INSTANCE} = "SPI_FLASH"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

       config USB_DEVICE_FUNCTION_1_MSD_LUN2_MEDIA_SDCARD${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN2_IDX${INSTANCE}
       select DRV_SDCARD_NEEDED
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN2_MEDIA${INSTANCE} = "SDCARD"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---
endif

	   config USB_DEVICE_FUNCTION_1_MSD_NUM_LUN3_IDX${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default n if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE} = 0
	   default n if USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE} = 3
	   default n if USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE} = 2
	   default n if USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE} = 1
	   default n if USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE} = 0
       default y

       config USB_DEVICE_FUNCTION_1_LUN3_IDX${INSTANCE}
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN3_IDX${INSTANCE}
       bool "LUN 3"
       default y
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

ifblock USB_DEVICE_FUNCTION_1_LUN3_IDX${INSTANCE}

       config  USB_DEVICE_FUNCTION_1_MSD_LUN3_MEDIA${INSTANCE}
       string "Media Type"
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN3_IDX${INSTANCE}
       range USB_DEVICE_MSD_TYPE
       default "NVM"
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

	   config USB_DEVICE_FUNCTION_1_MSD_LUN3_MEDIA_NVM${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN3_IDX${INSTANCE}
       select DRV_NVM_NEEDED
	   select USE_DRV_NVM_ERASE_WRITE_NEEDED
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN3_MEDIA${INSTANCE} = "NVM"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

       config USB_DEVICE_FUNCTION_1_MSD_LUN3_MEDIA_SPIFLASH${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN3_IDX${INSTANCE}
       select USE_DRV_SST25_NEEDED
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN3_MEDIA${INSTANCE} = "SPI_FLASH"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---

       config USB_DEVICE_FUNCTION_1_MSD_LUN3_MEDIA_SDCARD${INSTANCE}
       bool
       depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_MSD_NUM_LUN3_IDX${INSTANCE}
       select DRV_SDCARD_NEEDED
       default y if USB_DEVICE_FUNCTION_1_MSD_LUN3_MEDIA${INSTANCE} = "SDCARD"
       default n
       ---help---
       IDH_HTML_USB_DEVICE_MSD_INIT
       ---endhelp---
	   
endif

       config USB_DEVICE_FUNCTION_1_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

         config USB_DEVICE_FUNCTION_1_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_1_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on DRV_USB_DEVICE_SUPPORT
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_1_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on DRV_USB_DEVICE_SUPPORT
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---

	   config USB_DEVICE_FUNCTION_1_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_1_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_1_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on DRV_USB_DEVICE_SUPPORT
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_1_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on DRV_USB_DEVICE_SUPPORT
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---

        config USB_DEVICE_FUNCTION_1_VENDOR_ENDPOINT_NUMBER_IDX${INSTANCE}
        int "Endpoint Number"
        range 1 15 if (PIC32MX || PIC32MK || PIC32WK)
        range 1 7  if PIC32MZ
        depends on DRV_USB_DEVICE_SUPPORT
        depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
        depends on (PIC32MX || PIC32MK || PIC32WK || PIC32MZ)
        default 1
        ---help---
        IDH_HTML_USB_DEVICE_INIT 
        ---endhelp---

       config USB_DEVICE_FUNCTION_1_VENDOR_ENDPOINT_OUT_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number (OUT)"
       range 1 9  if DSTBDPIC32CZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       depends on DSTBDPIC32CZ
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_1_VENDOR_ENDPOINT_IN_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number (IN)"
       range 1 9  if DSTBDPIC32CZ
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       depends on DSTBDPIC32CZ
       default 2
       ---help---
       IDH_HTML_USB_DEVICE_INIT 
       ---endhelp---
