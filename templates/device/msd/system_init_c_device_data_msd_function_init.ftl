<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_device_data_msd_function_init.ftl

  Summary:
    USB Device Free-marker Template File

  Description:
    This file contains configurations necessary to run the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
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

/*******************************************
 * MSD Function Driver initialization
 *******************************************/
USB_DEVICE_MSD_MEDIA_INIT_DATA __attribute__((aligned(16))) msdMediaInit${CONFIG_USB_DEVICE_FUNCTION_INDEX}[1] =
{
    {
<#if (USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?has_content)>
        ${USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?keep_before_last("_")}_INDEX_0,
</#if>
        512,
        sectorBuffer,
        NULL,
        (void *)diskImage,
        {
            0x00,    // peripheral device is connected, direct access block device
            0x80,    // removable
            0x04,    // version = 00=> does not conform to any standard, 4=> SPC-2
            0x02,    // response is in format specified by SPC-2
            0x1F,    // additional length
            0x00,    // sccs etc.
            0x00,    // bque=1 and cmdque=0,indicates simple queuing 00 is obsolete,
                     // but as in case of other device, we are just using 00
            0x00,    // 00 obsolete, 0x80 for basic task queuing
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
<#if (USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?has_content)>
            ${USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?keep_before_last("_")}_IsAttached,
            ${USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?keep_before_last("_")}_Open,
            ${USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?keep_before_last("_")}_Close,
            ${USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?keep_before_last("_")}_GeometryGet,
            ${USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?keep_before_last("_")}_AsyncRead,
            ${USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?keep_before_last("_")}_AsyncEraseWrite,
            ${USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?keep_before_last("_")}_IsWriteProtected,
            ${USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_0?keep_before_last("_")}_TransferHandlerSet,
            NULL
</#if>
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