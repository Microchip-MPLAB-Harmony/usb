<#--
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
 -->
 /*******************************************************************************
  USB Host Initialization File

  File Name:
    usb_host_init_data.c

  Summary:
    This file contains source code necessary to initialize USB Host Stack.

  Description:
    This file contains source code necessary to initialize USB Host Stack.
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
#include "configuration.h"
#include "definitions.h" 

/* MISRA C-2012 Rule 11.8 deviated:2 and 20.7 devaited:4 deviated below. Deviation record ID -  
    H3_USB_MISRAC_2012_R_11_8_DR_1, H3_USB_MISRAC_2012_R_20_7_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:3 "MISRA C-2012 Rule 11.8" "H3_USB_MISRAC_2012_R_11_1_DR_1" )\
(deviate:4 "MISRA C-2012 Rule 20.7" "H3_USB_MISRAC_2012_R_20_7_DR_1" )
</#if>
${LIST_USB_HOST_CLIENT_INIT_DATA}
static const USB_HOST_TPL_ENTRY USBTPList[${CONFIG_USB_HOST_TPL_ENTRY_NUMBER}] = 
{
${LIST_USB_HOST_TPL_ENTRY}
};

<#if (core.DeviceFamily?has_content == true) 
    && (core.DeviceFamily == "SAMA5D2" 
     || core.DeviceFamily == "SAM9X60"
     || core.DeviceFamily == "SAM9X7"
     || core.DeviceFamily == "SAMA7G5"
	 || core.DeviceFamily == "SAMA7D65") >
static const USB_HOST_HCD hcdTable[2] = 
{
    {
         /* EHCI Driver Index */ 
        .drvIndex = DRV_USB_EHCI_INDEX_0,

        /* Pointer to the USB Driver Functions. */
        .hcdInterface = DRV_USB_EHCI_INTERFACE,
    },
    {
        /* OHCI Driver Index */ 
        .drvIndex = DRV_USB_OHCI_INDEX_0,

         /* Pointer to the USB Driver Interface. */
        .hcdInterface = DRV_USB_OHCI_INTERFACE
    }
};
<#else>
static const USB_HOST_HCD hcdTable = 
{
    /* Index of the USB Driver used by the Host Layer */
    .drvIndex = ${CONFIG_USB_DRIVER_INDEX},

    /* Pointer to the USB Driver Functions. */
    .hcdInterface = ${CONFIG_USB_DRIVER_INTERFACE},

};
</#if>

const USB_HOST_INIT usbHostInitData = 
{
    .nTPLEntries = ${CONFIG_USB_HOST_TPL_ENTRY_NUMBER} ,
    .tplList = (USB_HOST_TPL_ENTRY *)USBTPList,
    .hostControllerDrivers = (USB_HOST_HCD *)&hcdTable    
};
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.8"
#pragma coverity compliance end_block "MISRA C-2012 Rule 20.7"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */

// </editor-fold>
<#--
/*******************************************************************************
 End of File
*/
-->
