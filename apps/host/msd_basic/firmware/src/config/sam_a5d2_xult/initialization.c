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
/*** File System Initialization Data ***/


//const SYS_FS_MEDIA_MOUNT_DATA sysfsMountTable[SYS_FS_VOLUME_NUMBER] = 
//{
//	{NULL}
//};
//JCB
const SYS_FS_MEDIA_MOUNT_DATA sysfsMountTable[SYS_FS_VOLUME_NUMBER] = 
{
	{
		.mountName = SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0,
		.devName   = SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0, 
		.mediaType = SYS_FS_MEDIA_TYPE_IDX0,
		.fsType   = SYS_FS_TYPE_IDX0   
	},
};





const SYS_FS_REGISTRATION_TABLE sysFSInit [ SYS_FS_MAX_FILE_SYSTEM_TYPE ] =
{
    {
        .nativeFileSystemType = FAT,
        .nativeFileSystemFunctions = &FatFsFunctions
    }
};




// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)TC0_CH0_TimerCallbackRegister,
    .timerCounterGet = (SYS_TIME_PLIB_COUNTER_GET)TC0_CH0_TimerCounterGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)TC0_CH0_TimerPeriodSet,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)TC0_CH0_TimerFrequencyGet,
    .timerCompareSet = (SYS_TIME_PLIB_COMPARE_SET)TC0_CH0_TimerCompareSet,
    .timerStart = (SYS_TIME_PLIB_START)TC0_CH0_TimerStart,
    .timerStop = (SYS_TIME_PLIB_STOP)TC0_CH0_TimerStop 
};

const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = TC0_IRQn,
};

// </editor-fold>

DRV_USB_UHP_INIT drvUSBInit = 
{
    .interruptSource = (INT_SOURCE)41,
    .operationSpeed = USB_SPEED_HIGH,
    .portIndication = NULL,
    .portPowerEnable = NULL,
    .portOverCurrentDetect = NULL, 
    .rootHubAvailableCurrent = 500,
    .usbID = ((uhphs_registers_t*)0x00500000)
};

const USB_HOST_TPL_ENTRY USBTPList[1] = 
{
    /* classCode, subClassCode, protocolCode, initData, driver */
    TPL_INTERFACE_CLASS_SUBCLASS_PROTOCOL(0x08, 0x06, 0x50, NULL, USB_HOST_MSD_INTERFACE) ,
    // classCode:    0x08: Mass Storage
    // subClassCode: 0x06: SCSI transparent command set
    // protocolCode: 0x50: Bulk only
};

const USB_HOST_HCD hcdTable = 
{
    /* Index of the USB Driver used by the Host Layer */
    .drvIndex = DRV_USB_UHP_INDEX_0,
    /* Pointer to the USB Driver Functions. */
    .hcdInterface = DRV_USB_UHP_HOST_INTERFACE
};

const USB_HOST_INIT usbHostInitData = 
{
    .nTPLEntries = 1 ,
    .tplList = (USB_HOST_TPL_ENTRY *)USBTPList,
    .hostControllerDrivers = (USB_HOST_HCD *)&hcdTable
};


/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{
  
    CLK_Initialize();
	PIO_Initialize();


	BSP_Initialize();
	UART1_Initialize();

    MMU_Initialize();
    Matrix_Initialize();

    PLIB_L2CC_Initialize();

    INT_Initialize();
	WDT_REGS->WDT_MR = WDT_MR_WDDIS_Msk; 		// Disable WDT 

 
    TC0_CH0_TimerInitialize(); 
     
    


    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);

    /* Initialize the USB Host layer */
    sysObj.usbHostObject0 = USB_HOST_Initialize ((SYS_MODULE_INIT *)&usbHostInitData );

    /*** File System Service Initialization Code ***/
    SYS_FS_Initialize( (const void *) sysFSInit );

    /* Initialize USB Driver */ 
    sysObj.drvUSBObject = DRV_USB_UHP_Initialize (DRV_USB_UHP_INDEX_0, (SYS_MODULE_INIT *) &drvUSBInit);

    APP_Initialize();



}


/*******************************************************************************
 End of File
*/
