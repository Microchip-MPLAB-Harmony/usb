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

/*** DEVCFG0 ***/
#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx1
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   G3
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   G3
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL

/*** DEVCFG1 ***/
#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       ON
#pragma config POSCMOD =    EC
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF

/*** DEVCFG2 ***/
#pragma config FPLLIDIV =   DIV_1
#pragma config FPLLRNG =    RANGE_8_16_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_32
#pragma config FPLLODIV =   DIV_4
#pragma config DSBOREN =    ON
#pragma config DSWDTPS =    DSPS32
#pragma config DSWDTOSC =   LPRC
#pragma config DSWDTEN =    OFF
#pragma config FDSEN =      ON
#pragma config BORSEL =     HIGH
#pragma config UPLLEN =     ON

/*** DEVCFG3 ***/
#pragma config USERID =     0xffff
#pragma config FUSBIDIO2 =   ON
#pragma config FVBUSIO2 =  ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO1 =   ON
#pragma config FVBUSIO1 =  ON
#pragma config PWMLOCK =  OFF

/*** BF1SEQ ***/
#pragma config TSEQ =       0x0
#pragma config CSEQ =       0xffff





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
 

uint8_t __attribute__((aligned(512))) endPointTable0[DRV_USBFS_ENDPOINTS_NUMBER * 32];

void DRV_USB_VBUSPowerEnable0(uint8_t port, bool enable)
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
		VBUS_AH_PowerEnable();
	}
	else
	{
		/* Disable the VBUS */
		VBUS_AH_PowerDisable();
	}
}

const DRV_USBFS_INIT drvUSBFSInit0 =
{
	 /* Assign the endpoint table */
    .endpointTable= endPointTable0,
	/* Interrupt Source for USB module */
	.interruptSource = INT_SOURCE_USB_1 ,
   
	/* USB Controller to operate as USB Host */
    .operationMode = DRV_USBFS_OPMODE_HOST,
	
	.operationSpeed = USB_SPEED_FULL,
 
	/* Stop in idle */
    .stopInIdle = false,
	
	    /* Suspend in sleep */
    .suspendInSleep = false,
    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USB_ID_1,
	
	/* USB Host Power Enable. USB Driver uses this function to Enable the VBUS */ 
	.portPowerEnable = DRV_USB_VBUSPowerEnable0,
	
    /* Root hub available current in milliamperes */
    .rootHubAvailableCurrent = 500,

};






/******************************************************
 * USB Driver Initialization
 ******************************************************/
 

uint8_t __attribute__((aligned(512))) endPointTable1[DRV_USBFS_ENDPOINTS_NUMBER * 32];


const DRV_USBFS_INIT drvUSBFSInit1 =
{
	 /* Assign the endpoint table */
    .endpointTable= endPointTable1,
	/* Interrupt Source for USB module */
	.interruptSource = INT_SOURCE_USB_2 ,
   
    /* USB Controller to operate as USB Device */
    .operationMode = DRV_USBFS_OPMODE_DEVICE,
	
	.operationSpeed = USB_SPEED_FULL,
 
	/* Stop in idle */
    .stopInIdle = false,
	
	    /* Suspend in sleep */
    .suspendInSleep = false,
	.usbID = USB_ID_2,
	

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
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)CORETIMER_CallbackSet,
    .timerStart = (SYS_TIME_PLIB_START)CORETIMER_Start,
    .timerStop = (SYS_TIME_PLIB_STOP)CORETIMER_Stop ,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)CORETIMER_FrequencyGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)NULL,
    .timerCompareSet = (SYS_TIME_PLIB_COMPARE_SET)CORETIMER_CompareSet,
    .timerCounterGet = (SYS_TIME_PLIB_COUNTER_GET)CORETIMER_CounterGet,
};

const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = 0,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************



/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{
    /* Start out with interrupts disabled before configuring any modules */
    __builtin_disable_interrupts();

  
    CLK_Initialize();

    /* Configure CP0.K0 for optimal performance (cached instruction pre-fetch) */
    __builtin_mtc0(16, 0,(__builtin_mfc0(16, 0) | 0x3));

    /* Configure Wait States and Prefetch */
    CHECONbits.PFMWS = 1;
    CHECONbits.PREFEN = 0;



	GPIO_Initialize();

    CORETIMER_Initialize();
	BSP_Initialize();


    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);


	 /* Initialize the USB device layer */
    sysObj.usbDevObject0 = USB_DEVICE_Initialize (USB_DEVICE_INDEX_0 , ( SYS_MODULE_INIT* ) & usbDevInitData0);
	
	

	/* Initialize USB Driver */ 
    sysObj.drvUSBFSObject0 = DRV_USBFS_Initialize(DRV_USBFS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBFSInit0);	

	/* Initialize USB Driver */ 
    sysObj.drvUSBFSObject1 = DRV_USBFS_Initialize(DRV_USBFS_INDEX_1, (SYS_MODULE_INIT *) &drvUSBFSInit1);	

	/* Initialize the USB Host layer */
    sysObj.usbHostObject0 = USB_HOST_Initialize (( SYS_MODULE_INIT *)& usbHostInitData );	

    /*** File System Service Initialization Code ***/
    SYS_FS_Initialize( (const void *) sysFSInit );


    APP_Initialize();


    EVIC_Initialize();

	/* Enable global interrupts */
    __builtin_enable_interrupts();


}


/*******************************************************************************
 End of File
*/
