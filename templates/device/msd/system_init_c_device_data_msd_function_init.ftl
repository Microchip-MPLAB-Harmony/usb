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

/* MISRA C-2012 Rule 10.3, 11.1 and 11.8 deviated below. Deviation record ID -  
   H3_MISRAC_2012_R_10_3_DR_1, H3_MISRAC_2012_R_11_1_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:43 "MISRA C-2012 Rule 10.3" "H3_MISRAC_2012_R_10_3_DR_1" )\
(deviate:5 "MISRA C-2012 Rule 11.1" "H3_MISRAC_2012_R_11_1_DR_1" )\
(deviate:5 "MISRA C-2012 Rule 11.8" "H3_MISRAC_2012_R_11_8_DR_1" )   
</#if>
/***********************************************
 * Sector buffer needed by for the MSD LUN.
 ***********************************************/
static uint8_t sectorBuffer[512 * USB_DEVICE_MSD_NUM_SECTOR_BUFFERS] USB_ALIGN;

/***********************************************
 * CBW and CSW structure needed by for the MSD
 * function driver instance.
 ***********************************************/
<#assign usb_fs_device = ["SAMD21","SAMDA1","SAMD5", "SAME5", "LAN9255","SAML21", "SAML22",  "SAMD11", "SAMR21", "SAMR30", "SAMR34", "SAMR35", "PIC32CM", "PIC32CX", "SAMG55", "PIC32MK", "PIC32MX", "PIC32MM", "PIC32MZ1025W", "WFI32E01" ]>
<#--  ** List of highspeed devices <#assign usb_hs_device = ["SAMV70", "SAMV71","SAME70", "SAMS70","SAMA5D2", "SAM9X60","SAM9X7", "SAMA7","PIC32MZ", "PIC32CZ","PIC32CK"]> -->
<#assign flag = false> 
<#list usb_fs_device as device>
<#if __PROCESSOR?contains(device) >
<#assign flag = true> 
</#if>
</#list>
<#if flag == true>
static uint8_t msdCBW${CONFIG_USB_DEVICE_FUNCTION_INDEX}[64] USB_ALIGN; 
<#else>
static uint8_t msdCBW${CONFIG_USB_DEVICE_FUNCTION_INDEX}[512] USB_ALIGN;
</#if>
static USB_MSD_CSW msdCSW${CONFIG_USB_DEVICE_FUNCTION_INDEX} USB_ALIGN;
<#assign ROW_BUFFER_ENABLE = false>
<#if (__PROCESSOR?contains("PIC32MZ") == true) || (__PROCESSOR?contains("PIC32CZ") == true) || (__PROCESSOR?contains("PIC32CK") == true)>
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
/***************************************************************************
 * The USB Device MSD function driver will use this buffer to cache the data 
 * received from the USB Host before sending it to the media. 
 ****************************************************************************/
static uint8_t flashRowBackupBuffer [DRV_MEMORY_DEVICE_PROGRAM_SIZE] USB_ALIGN;
    </#if>
</#if>


/*******************************************
 * MSD Function Driver initialization
 *******************************************/
static USB_DEVICE_MSD_MEDIA_INIT_DATA USB_ALIGN  msdMediaInit${CONFIG_USB_DEVICE_FUNCTION_INDEX}[${CONFIG_USB_DEVICE_FUNCTION_MSD_LUN}] =
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
                    <#if ROW_BUFFER_ENABLE == true >
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
static const USB_DEVICE_MSD_INIT msdInit${CONFIG_USB_DEVICE_FUNCTION_INDEX} =
{
    .numberOfLogicalUnits = ${CONFIG_USB_DEVICE_FUNCTION_MSD_LUN},
    .msdCBW = (USB_MSD_CBW*)&msdCBW${CONFIG_USB_DEVICE_FUNCTION_INDEX},
    .msdCSW = &msdCSW${CONFIG_USB_DEVICE_FUNCTION_INDEX},
    .mediaInit = &msdMediaInit${CONFIG_USB_DEVICE_FUNCTION_INDEX}[0]
};

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.3"
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.1"
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>     
</#if> 
/* MISRAC 2012 deviation block end */
<#--
/*******************************************************************************
 End of File
*/
-->