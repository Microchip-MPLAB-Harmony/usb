<#--
/*******************************************************************************
  USB Framework Interrupt Handler Template File

  File Name:
    usb_interrupt.s

  Summary:
    This file contains source code necessary to place the USB ISR.

  Description:
    
 *******************************************************************************/

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
<#macro USB_ASM_INT USB_HANDLER USB_HANDLER_DMA>
/* USB Device Interrupt */
<#if ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
<@RTOS_ISR VECTOR = CONFIG_INT_VECT_USB_1 NAME = USB_HANDLER PRIORITY = CONFIG_DRV_USB_INTERRUPT_PRIORITY_IDX0/>
<#else>
<@RTOS_ISR VECTOR = CONFIG_INT_VECT_USB NAME = USB_HANDLER PRIORITY = CONFIG_DRV_USB_INTERRUPT_PRIORITY_IDX0/>
<@RTOS_ISR VECTOR = CONFIG_INT_VECT_USB_DMA NAME = USB_HANDLER_DMA PRIORITY = CONFIG_DRV_USBDMA_INTERRUPT_PRIORITY_IDX0/>
</#if>
</#macro>

<#if CONFIG_DRV_USB_INTERRUPT_MODE == true>
<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
<@USB_ASM_INT USB_HANDLER="USBInstance0" USB_HANDLER_DMA="USBInstance0_USBDMA"/>
<#else>
<@USB_ASM_INT USB_HANDLER="_USB_stub" USB_HANDLER_DMA="USBInstance0_USBDMA"/>
</#if>
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
