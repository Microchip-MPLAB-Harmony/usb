<#--
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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
 -->
<#if CONFIG_USB_HOST_USE_CDC == true>
extern USB_HOST_CLASS_DRIVER cdcDriver;

</#if>

<#if CONFIG_USB_HOST_USE_HID == true>
extern USB_HOST_CLASS_DRIVER hidDriver;

</#if>
<#if CONFIG_USB_HOST_USE_MSD == true>
extern USB_HOST_CLASS_DRIVER msdDriver;
 
</#if>

<#if CONFIG_USB_HOST_USE_HUB == true>
extern USB_HOST_CLASS_DRIVER  hubDriver;
 
</#if>
<#if CONFIG_DRV_USB_HOST_SUPPORT == true>
const USB_HOST_TARGET_PERIPHERAL_LIST USBTPList[${CONFIG_USB_HOST_TPL_ENTRIES}] =
{
<#if CONFIG_USB_HOST_USE_MSD == true>
    {TPL_MATCH_CL_SC_P(0x08, 0x06, 0x50), TPL_FLAG_CLASS_SUBCLASS_PROTOCOL, &msdDriver},
 
</#if>
<#if CONFIG_USB_HOST_USE_CDC == true>
    {TPL_MATCH_CL_SC_P(0x02, 0x02, 0x01), TPL_FLAG_CLASS_SUBCLASS_PROTOCOL, &cdcDriver},

</#if>
<#if CONFIG_USB_HOST_USE_HID == true>
    {TPL_MATCH_CL_SC_P(0x03, 0x01, 0x02), TPL_FLAG_CLASS_SUBCLASS_PROTOCOL, &hidDriver},

</#if>
<#if CONFIG_USB_HOST_USE_HUB == true>
    {TPL_MATCH_CL_SC_P(0x09, 0x00, 0x00), TPL_FLAG_CLASS_SUBCLASS_PROTOCOL, &hidDriver},

</#if>

};

</#if>
<#-- Instance 0 -->
<#if CONFIG_DRV_USB_HOST_SUPPORT == true>
/****************************************************
 * Endpoint Table needed by the controller driver .
 ****************************************************/

uint8_t __attribute__((aligned(512))) endpointTable[USB_HOST_ENDPOINT_TABLE_SIZE];

/*** USB Host Initialization Data ***/

const USB_HOST_INIT usbHostInitData =
{
<#if CONFIG_USB_HOST_HOST_POWER_STATE_IDX0?has_content>
    .moduleInit =  { ${CONFIG_USB_HOST_HOST_POWER_STATE_IDX0} },

</#if>	
    /* Identifies peripheral (PLIB-level) ID */
<#if CONFIG_PIC32MZ == true >
    .usbID = 0,

<#else>
<#if CONFIG_DRV_USB_HOST_MODULE_ID_IDX0?has_content> 
    .usbID = ${CONFIG_DRV_USB_HOST_MODULE_ID_IDX0},

</#if>
</#if>
    /* Stop in idle */
<#if CONFIG_USB_HOST_SLEEP_IN_IDLE_IDX0 == true>
    .stopInIdle = true,

<#else>
    .stopInIdle = false,

</#if>
    /* Suspend in sleep */
<#if CONFIG_DRV_USB_HOST_SUSPEND_IN_SLEEP_IDX0 == true >
    .suspendInSleep = true, 

	<#else>
    .suspendInSleep = false, 

</#if>
    /* Endpoint Table */
    .endpointTable = endpointTable,

    /* USB Module interrupt */
<#if CONFIG_DRV_USB_HOST_INTERRUPT_SOURCE_IDX0?has_content && CONFIG_DRV_USB_INTERRUPT_MODE == true>
    .interruptSource = ${CONFIG_DRV_USB_HOST_INTERRUPT_SOURCE_IDX0},
	
	<#if CONFIG_PIC32MZ == true >

    /* Interrupt Source for USB module */
    .interruptSourceUSBDma = INT_SOURCE_USB_1_DMA,
</#if>

</#if>
    /* Number of entries in the TPL table */
<#if CONFIG_USB_HOST_TPL_ENTRIES?has_content>
    .nTPLEntries = ${CONFIG_USB_HOST_TPL_ENTRIES},

</#if>
    /* Speed at which this Host should operate */

	    /* USB Host Speed */
<#if ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
    .usbSpeed = USB_SPEED_FULL,
<#elseif CONFIG_PIC32MZ == true >
<#if CONFIG_USB_HOST_SPEED_HS_IDX0?has_content>
    .usbSpeed = ${CONFIG_USB_HOST_SPEED_HS_IDX0},
</#if>
</#if>
	
	
    /* Pointer to the TPL table */
<#if CONFIG_USB_HOST_TPL_TABLE_IDX0?has_content>
    .tplList = (USB_HOST_TARGET_PERIPHERAL_LIST *) ${CONFIG_USB_HOST_TPL_TABLE_IDX0},

</#if>
};
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
