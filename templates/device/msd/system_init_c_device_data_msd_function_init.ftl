<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_device_data_hid_function_init.ftl

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
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
uint8_t sectorBuffer[512 * USB_DEVICE_MSD_NUM_SECTOR_BUFFERS] __attribute__((aligned(16)));

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${CONFIG_USB_DEVICE_FUNCTION_INDEX} __attribute__((aligned(16)));
USB_MSD_CSW msdCSW${CONFIG_USB_DEVICE_FUNCTION_INDEX} __attribute__((aligned(16)));

/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer [DRV_NVM_ROW_SIZE];

/*******************************************
 * MSD Function Driver initialization
 *******************************************/
USB_DEVICE_MSD_MEDIA_INIT_DATA __attribute__((aligned(16))) msdMediaInit${CONFIG_USB_DEVICE_FUNCTION_INDEX}[1] =
{
    {
        DRV_NVM_INDEX_0,
        512,
        sectorBuffer,
        flashRowBackupBuffer,
        (void *)diskImage,
        {
            0x00,    // peripheral device is connected, direct access block device
            0x80,    // removable
            0x04,    // version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,    // response is in format specified by SPC-2
            0x1F,    // additional length
            0x00,    // sccs etc.
            0x00,    // bque=1 and cmdque=0,indicates simple queueing 00 is obsolete,
                     // but as in case of other device, we are just using 00
            0x00,    // 00 obsolete, 0x80 for basic task queueing
            {
                'M','i','c','r','o','c','h','p'
            },
            {
                'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' '
            },
            {
                '0','0','0','1'
            }
        },
        {
            DRV_NVM_IsAttached,
            DRV_NVM_Open,
            DRV_NVM_Close,
            DRV_NVM_GeometryGet,
            DRV_NVM_Read,
            DRV_NVM_EraseWrite,
            DRV_NVM_IsWriteProtected,
            DRV_NVM_EventHandlerSet,
            NULL
        }
    },
    
    
    
};

    
/**************************************************
 * USB Device Function Driver Init Data
 **************************************************/
const USB_DEVICE_MSD_INIT msdInit${CONFIG_USB_DEVICE_FUNCTION_INDEX} =
{
    .numberOfLogicalUnits = 1,
    .msdCBW = &msdCBW${CONFIG_USB_DEVICE_FUNCTION_INDEX},
    .msdCSW = &msdCSW${CONFIG_USB_DEVICE_FUNCTION_INDEX},
    .mediaInit = &msdMediaInit${CONFIG_USB_DEVICE_FUNCTION_INDEX}[0]
};
<#--
/*******************************************************************************
 End of File
*/
-->