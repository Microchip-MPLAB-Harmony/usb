/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
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
#include "configuration.h"
#include "definitions.h"
#include "device.h"



// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************
/******************************************************
 * USB Driver Initialization
 ******************************************************/
void DRV_USB_VBUSPowerEnable(uint8_t port, bool enable)
{
    /* Note: USB Host applications should have a way for Enabling/Disabling the 
       VBUS. Applications can use a GPIO to turn VBUS on/off through a switch. 
       In MHC Pin Settings select the pin used as VBUS Power Enable as output and 
       name it to "VBUS_AH". If you a see a build error from this function either 
       you have not configured the VBUS Power Enable in MHC pin settings or the 
       Pin name entered in MHC is not "VBUS_AH". */ 
    if (enable == true)
    {
        /* Enable the VBUS */
        VBUS_AH_PD14_PowerEnable();
        VBUS_AH_PD15_PowerEnable();
        VBUS_AH_PD16_PowerEnable();
    }
    else
    {
        /* Disable the VBUS */
        VBUS_AH_PD14_PowerDisable();
        VBUS_AH_PD15_PowerDisable();
        VBUS_AH_PD16_PowerDisable();
    }
}

DRV_USB_EHCI_INIT drvUSBEHCIInit =
{
    /* Interrupt Source for USB module */
    .interruptSource = (INT_SOURCE)22,

    /* USB base address */
    .usbID = ((uhphs_registers_t*)UHPHS_EHCI_ADDR),
    
    /* Ports Selection */ 
    .bmPortSelect = 0x02,

    /* USB Host Power Enable. USB Driver uses this function to Enable the VBUS */ 
    .portPowerEnable = DRV_USB_VBUSPowerEnable,
    
    /* Root hub available current in milliamperes */    
    .rootHubAvailableCurrent = 500,

    .companionDriverIndex = DRV_USB_OHCI_INDEX_0

};

DRV_USB_OHCI_INIT drvUSBOHCIInit =
{
     /* Interrupt Source for USB module */
    .interruptSource = (INT_SOURCE)22,

    /* USB base address */
    .usbID = ((UhpOhci *)UHPHS_OHCI_ADDR),

     /* Ports Selection */ 
    .bmPortSelect = 0x02,
    
    /* USB Host Power Enable. USB Driver uses this function to Enable the VBUS */ 
    .portPowerEnable = DRV_USB_VBUSPowerEnable,
    
    /* Root hub available current in milliamperes */    
    .rootHubAvailableCurrent = 500
};

// <editor-fold defaultstate="collapsed" desc="File System Initialization Data">

const SYS_FS_MEDIA_MOUNT_DATA sysfsMountTable[SYS_FS_VOLUME_NUMBER] =
{
    {
        .mountName = SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0,
        .devName   = SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0,
        .mediaType = SYS_FS_MEDIA_TYPE_IDX0,
        .fsType   = SYS_FS_TYPE_IDX0
    },
};


const SYS_FS_FUNCTIONS FatFsFunctions =
{
    .mount             = FATFS_mount,
    .unmount           = FATFS_unmount,
    .open              = FATFS_open,
    .read              = FATFS_read,
    .close             = FATFS_close,
    .seek              = FATFS_lseek,
    .fstat             = FATFS_stat,
    .getlabel          = FATFS_getlabel,
    .currWD            = FATFS_getcwd,
    .getstrn           = FATFS_gets,
    .openDir           = FATFS_opendir,
    .readDir           = FATFS_readdir,
    .closeDir          = FATFS_closedir,
    .chdir             = FATFS_chdir,
    .chdrive           = FATFS_chdrive,
    .write             = FATFS_write,
    .tell              = FATFS_tell,
    .eof               = FATFS_eof,
    .size              = FATFS_size,
    .mkdir             = FATFS_mkdir,
    .remove            = FATFS_unlink,
    .setlabel          = FATFS_setlabel,
    .truncate          = FATFS_truncate,
    .chmode            = FATFS_chmod,
    .chtime            = FATFS_utime,
    .rename            = FATFS_rename,
    .sync              = FATFS_sync,
    .putchr            = FATFS_putc,
    .putstrn           = FATFS_puts,
    .formattedprint    = FATFS_printf,
    .testerror         = FATFS_error,
    .formatDisk        = (FORMAT_DISK)FATFS_mkfs,
    .partitionDisk     = FATFS_fdisk,
    .getCluster        = FATFS_getclusters
};


const SYS_FS_REGISTRATION_TABLE sysFSInit [ SYS_FS_MAX_FILE_SYSTEM_TYPE ] =
{
    {
        .nativeFileSystemType = FAT,
        .nativeFileSystemFunctions = &FatFsFunctions
    }
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)TC0_CH0_TimerCallbackRegister,
    .timerStart = (SYS_TIME_PLIB_START)TC0_CH0_TimerStart,
    .timerStop = (SYS_TIME_PLIB_STOP)TC0_CH0_TimerStop ,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)TC0_CH0_TimerFrequencyGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)TC0_CH0_TimerPeriodSet,
    .timerCompareSet = (SYS_TIME_PLIB_COMPARE_SET)TC0_CH0_TimerCompareSet,
    .timerCounterGet = (SYS_TIME_PLIB_COUNTER_GET)TC0_CH0_TimerCounterGet,
};

const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = TC0_IRQn,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************
/*******************************************************************************
  Function:
    void SYSC_Disable ( void )

  Summary:
    Disables ununsed SYSC peripherals

  Remarks:
 */
static void SYSC_Disable( void )
{
    //save context and disable write protection
    uint32_t sysc_wpmr = SYSCWP_REGS->SYSCWP_SYSC_WPMR &
      (SYSCWP_SYSC_WPMR_WPEN_Msk | SYSCWP_SYSC_WPMR_WPITEN_Msk);
    SYSCWP_REGS->SYSCWP_SYSC_WPMR = SYSCWP_SYSC_WPMR_WPKEY_PASSWD &
                                    ~(SYSCWP_SYSC_WPMR_WPITEN_Msk |
                                    SYSCWP_SYSC_WPMR_WPITEN_Msk);


    /* ----------------------------   RTC  -------------------------------*/
    //Disable interrupts
    RTC_REGS->RTC_IDR = RTC_IDR_Msk;

    //Clear interrupt status
    RTC_REGS->RTC_SCCR = RTC_SCCR_Msk;

    /* ----------------------------   RTT  -------------------------------*/
    //Disable Timer and interrupt
    uint32_t rtt_mr = RTT_REGS->RTT_MR;
    RTT_REGS->RTT_MR = rtt_mr & ~(RTT_MR_RTTDIS_Msk | RTT_MR_RTTINCIEN_Msk);

    //Clear status
    RTT_REGS->RTT_SR;

    /* ----------------------------   RSTC  ------------------------------*/
    // Disable interrupt
    uint32_t rstc_mr = RSTC_REGS->RSTC_MR & (RSTC_MR_ENGCLR_Msk |
                                             RSTC_MR_ERSTL_Msk |
                                             RSTC_MR_URSTIEN_Msk |
                                             RSTC_MR_URSTASYNC_Msk |
                                             RSTC_MR_SCKSW_Msk |
                                             RSTC_MR_URSTEN_Msk);
    rstc_mr = rstc_mr & (~RSTC_MR_URSTIEN_Msk);
    RSTC_REGS->RSTC_MR = RSTC_MR_KEY_PASSWD | rstc_mr;

   //Context restore SYSC write protect registers
   SYSCWP_REGS->SYSCWP_SYSC_WPMR = (SYSCWP_SYSC_WPMR_WPKEY_PASSWD | sysc_wpmr);
}




/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{
	SYSC_Disable( );

  
    CLK_Initialize();

	PIO_Initialize();



	BSP_Initialize();
	PIT_TimerInitialize();

    MMU_Initialize();

    INT_Initialize();
    
    /* Disable WDT   */
    WDT_REGS->WDT_MR = WDT_MR_WDDIS_Msk;

 
    TC0_CH0_TimerInitialize(); 
     
    


    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);

     /* Initialize USB Driver */ 
    sysObj.drvUSBEHCIObject = DRV_USB_EHCI_Initialize (DRV_USB_EHCI_INDEX_0, (SYS_MODULE_INIT *) &drvUSBEHCIInit);
    sysObj.drvUSBOHCIObject = DRV_USB_OHCI_Initialize (DRV_USB_OHCI_INDEX_0, (SYS_MODULE_INIT *) &drvUSBOHCIInit);

	/* Initialize the USB Host layer */
    sysObj.usbHostObject0 = USB_HOST_Initialize (( SYS_MODULE_INIT *)& usbHostInitData );	

    /*** File System Service Initialization Code ***/
    SYS_FS_Initialize( (const void *) sysFSInit );


    APP_Initialize();



}


/*******************************************************************************
 End of File
*/
