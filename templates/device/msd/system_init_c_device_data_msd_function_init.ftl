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
uint8_t sectorBuffer[512 * USB_DEVICE_MSD_NUM_SECTOR_BUFFERS] USB_ALIGN;

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
USB_MSD_CBW msdCBW${CONFIG_USB_DEVICE_FUNCTION_INDEX} USB_ALIGN;
USB_MSD_CSW msdCSW${CONFIG_USB_DEVICE_FUNCTION_INDEX} USB_ALIGN;
<#assign ROW_BUFFER_ENABLE = false>
<#if __PROCESSOR?contains("PIC32MZ") == true>
    <#list 0..4 as i>
        <#assign MSD_ENABLE = "CONFIG_USB_DEVICE_FUNCTION_MSD_LUN_IDX" + i>
        <#if .vars[MSD_ENABLE]?has_content>
            <#if (.vars[MSD_ENABLE] != false)>
                <#assign MEDIA_TYPE = "CONFIG_USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_IDX" + i>
                <#if .vars[MEDIA_TYPE] == "MEDIA_TYPE_NVM">
                    <#assign ROW_BUFFER_ENABLE = true>
                </#if>
            </#if>
        </#if>
    </#list>
    <#if ROW_BUFFER_ENABLE == true>
/***********************************************
 * Because the PIC32MZ flash row size if 2048
 * and the media sector size if 512 bytes, we
 * have to allocate a buffer of size 2048
 * to backup the row. A pointer to this row
 * is passed in the media initialization data
 * structure.
 ***********************************************/
uint8_t flashRowBackupBuffer [DRV_MEMORY_DEVICE_PROGRAM_SIZE] USB_ALIGN;
    </#if>
</#if>


/*******************************************
 * MSD Function Driver initialization
 *******************************************/
USB_DEVICE_MSD_MEDIA_INIT_DATA USB_ALIGN  msdMediaInit${CONFIG_USB_DEVICE_FUNCTION_INDEX}[${CONFIG_USB_DEVICE_FUNCTION_MSD_LUN}] =
{
    <#list 0..4 as i>
        <#assign MSD_ENABLE = "CONFIG_USB_DEVICE_FUNCTION_MSD_LUN_IDX" + i>
            <#if .vars[MSD_ENABLE]?has_content>
                <#if (.vars[MSD_ENABLE] != false)>
    /* LUN ${i} */ 
    {
                    <#assign MEDIA_TYPE = "CONFIG_USB_DEVICE_FUNCTION_MSD_LUN_MEDIA_TYPE_IDX" + i>
                    <#if .vars[MEDIA_TYPE] == "MEDIA_TYPE_SD_CARD_SPI">
        DRV_SDSPI_INDEX_0,
                    <#elseif .vars[MEDIA_TYPE] == "MEDIA_TYPE_SD_CARD_MMC" >
        DRV_SDMMC_INDEX_0,
                    <#else>
        DRV_MEMORY_INDEX_0,
                    </#if>
        512,
        sectorBuffer,
                    <#if ROW_BUFFER_ENABLE == true && __PROCESSOR?contains("PIC32MZ") == true >
        flashRowBackupBuffer,
                    <#else>
        NULL,
                    </#if>
        0,
        {
            0x00,    // peripheral device is connected, direct access block device
            0x80,   // removable
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
                    <#if .vars[MEDIA_TYPE] == "MEDIA_TYPE_SD_CARD_SPI">
            DRV_SDSPI_IsAttached,
            DRV_SDSPI_Open,
            DRV_SDSPI_Close,
            DRV_SDSPI_GeometryGet,
            DRV_SDSPI_AsyncRead,
            DRV_SDSPI_AsyncWrite,
            DRV_SDSPI_IsWriteProtected,
            DRV_SDSPI_EventHandlerSet,
            NULL
                    <#elseif .vars[MEDIA_TYPE] == "MEDIA_TYPE_SD_CARD_MMC">
            DRV_SDMMC_IsAttached,
            DRV_SDMMC_Open,
            DRV_SDMMC_Close,
            DRV_SDMMC_GeometryGet,
            DRV_SDMMC_AsyncRead,
            DRV_SDMMC_AsyncWrite,
            DRV_SDMMC_IsWriteProtected,
            DRV_SDMMC_EventHandlerSet,
            NULL
                    <#else>    
            DRV_MEMORY_IsAttached,
            DRV_MEMORY_Open,
            DRV_MEMORY_Close,
            DRV_MEMORY_GeometryGet,
            DRV_MEMORY_AsyncRead,
            DRV_MEMORY_AsyncEraseWrite,
            DRV_MEMORY_IsWriteProtected,
            DRV_MEMORY_TransferHandlerSet,
            NULL
                    </#if>    
        }
    },
                </#if>
            </#if>            
    </#list> 
};
  
/**************************************************
 * USB Device Function Driver Init Data
 **************************************************/
const USB_DEVICE_MSD_INIT msdInit${CONFIG_USB_DEVICE_FUNCTION_INDEX} =
{
    .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_MSD_LUN},
    .msdCBW = &msdCBW${CONFIG_USB_DEVICE_FUNCTION_INDEX},
    .msdCSW = &msdCSW${CONFIG_USB_DEVICE_FUNCTION_INDEX},
    .mediaInit = &msdMediaInit${CONFIG_USB_DEVICE_FUNCTION_INDEX}[0]
};
<#--
/*******************************************************************************
 End of File
*/
-->