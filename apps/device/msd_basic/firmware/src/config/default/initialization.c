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
Copyright (c) 2017-2018 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"


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
// <editor-fold defaultstate="collapsed" desc="DRV_MEMORY Instance 0 Initialization Data">

uint8_t gDrvMemory0EraseBuffer[EFC0_ERASE_BUFFER_SIZE] __attribute__((aligned(32)));

DRV_MEMORY_CLIENT_OBJECT gDrvMemory0ClientObject[DRV_MEMORY_CLIENTS_NUMBER_IDX0] = { 0 };

DRV_MEMORY_BUFFER_OBJECT gDrvMemory0BufferObject[DRV_MEMORY_BUFFER_QUEUE_SIZE_IDX0] = { 0 };

const MEMORY_DEVICE_API drvMemory0DeviceAPI = {
    .Open               = DRV_EFC0_Open,
    .Close              = DRV_EFC0_Close,
    .Status             = DRV_EFC0_Status,
    .SectorErase        = DRV_EFC0_SectorErase,
    .Read               = DRV_EFC0_Read,
    .PageWrite          = DRV_EFC0_PageWrite,
    .GeometryGet        = (GEOMETRY_GET)DRV_EFC0_GeometryGet,
    .TransferStatusGet  = (TRANSFER_STATUS_GET)DRV_EFC0_TransferStatusGet
};

const DRV_MEMORY_INIT drvMemory0InitData =
{
    .memDevIndex          = 0,
    .memoryDevice         = &drvMemory0DeviceAPI,
    .isFsEnabled          = false,
    .ewBuffer             = &gDrvMemory0EraseBuffer[0],
    .clientObjPool        = (uintptr_t)&gDrvMemory0ClientObject[0],
    .bufferObj            = (uintptr_t)&gDrvMemory0BufferObject[0],
    .queueSize            = DRV_MEMORY_BUFFER_QUEUE_SIZE_IDX0,
    .nClientsMax          = DRV_MEMORY_CLIENTS_NUMBER_IDX0
};

// </editor-fold>


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
 
static DRV_USB_VBUS_LEVEL DRV_USBHSV1_VBUS_Comparator(void)
{
    DRV_USB_VBUS_LEVEL retVal = DRV_USB_VBUS_LEVEL_INVALID;

//    if(true == USB_VBUS_INState_Get())
//    {
//        retVal = DRV_USB_VBUS_LEVEL_VALID;
//    }
	retVal = DRV_USB_VBUS_LEVEL_VALID;
    return (retVal);

}

const DRV_USBHSV1_INIT drvUSBInit =
{
    /* Interrupt Source for USB module */
    .interruptSource = USBHS_IRQn,

    /* System module initialization */
    .moduleInit = {0},

    /* USB Controller to operate as USB Device */
    .operationMode = DRV_USBHSV1_OPMODE_DEVICE,

    /* To operate in USB Normal Mode */
    .operationSpeed = DRV_USBHSV1_DEVICE_SPEEDCONF_NORMAL,
    
    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USBHS_REGS,
	
    /* Function to check for VBus */
    .vbusComparator = DRV_USBHSV1_VBUS_Comparator
};


/*** File System Initialization Data ***/


const SYS_FS_MEDIA_MOUNT_DATA sysfsMountTable[SYS_FS_VOLUME_NUMBER] = 
{
	{NULL}
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

    NVIC_Initialize();
	RSWDT_REGS->RSWDT_MR = RSWDT_MR_WDDIS_Msk;	// Disable RSWDT 
	WDT_REGS->WDT_MR = WDT_MR_WDDIS_Msk; 		// Disable WDT 

    sysObj.drvMemory0 = DRV_MEMORY_Initialize((SYS_MODULE_INDEX)DRV_MEMORY_INDEX_0, (SYS_MODULE_INIT *)&drvMemory0InitData);



	/* Initialize USB Driver */ 
    sysObj.drvUSBHSV1Object = DRV_USBHSV1_Initialize(DRV_USBHSV1_INDEX_0, (SYS_MODULE_INIT *) &drvUSBInit);	

    /*** File System Service Initialization Code ***/
    SYS_FS_Initialize( (const void *) sysFSInit );


	 /* Initialize the USB device layer */
    sysObj.usbDevObject0 = USB_DEVICE_Initialize (USB_DEVICE_INDEX_0 , ( SYS_MODULE_INIT* ) & usbDevInitData);
	
	


    APP_Initialize();


}


/*******************************************************************************
 End of File
*/

