<#--
/*******************************************************************************
  USB Framework Interrupt Handler Template File

  File Name:
    usb_interrupt.c

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
<#if CONFIG_DRV_USB_INTERRUPT_MODE == true>
    <#if CONFIG_USB_DEVICE_INST_IDX0 == true >
		<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
			<#if CONFIG_USE_3RDPARTY_RTOS>
				<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
<#if  CONFIG_ARCH_ARM == true>
void USBHS_Handler ( void )
<#else>
void __ISR(${CONFIG_DRV_USB_ISR_VECTOR_IDX0}, ipl${CONFIG_DRV_USB_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0(void)
</#if>				

				<#else>
					<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
<#if  CONFIG_ARCH_ARM == true>
void USBHS_Handler ( void )
<#else>
void __attribute__( (interrupt(ipl${CONFIG_DRV_USB_INT_PRIO_NUM_IDX0}AUTO), vector(${CONFIG_DRV_USB_ISR_VECTOR_IDX0}))) IntHandlerUSBInstance0_ISR( void );
</#if>				

					</#if>
<#if  CONFIG_ARCH_ARM == true>
void USBHS_Handler ( void )
<#else>
void IntHandlerUSBInstance0(void)
</#if>				

				</#if>
			<#elseif CONFIG_DSTBDPIC32CZ == true>
void USBHS_Handler(void)
			<#else>
void __ISR(${CONFIG_DRV_USB_ISR_VECTOR_IDX0}, ipl${CONFIG_DRV_USB_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0(void)
			</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
    OS_EnterNestableInterrupt();
</#if>
</#if>
		<#if CONFIG_PIC32MZ == true>
    DRV_USBHS_Tasks_ISR(sysObj.drvUSBObject);
		<#elseif CONFIG_DSTBDPIC32CZ == true>
    DRV_USBHSV1_Tasks_ISR(sysObj.usbDevObject0);
		<#elseif  ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
    DRV_USBFS_Tasks_ISR(sysObj.drvUSBObject);
		</#if>
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
    OS_LeaveNestableInterrupt();
</#if>
</#if>
}

		<#if CONFIG_PIC32MZ == true>
			<#if CONFIG_USE_3RDPARTY_RTOS>
				<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR ( ${CONFIG_DRV_USBDMA_ISR_VECTOR_IDX0},ipl${CONFIG_DRV_USBDMA_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0_USBDMA ( void )
				<#else>
    <#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
void __attribute__( (interrupt(ipl${CONFIG_DRV_USBDMA_INT_PRIO_NUM_IDX0}AUTO), vector(${CONFIG_DRV_USBDMA_ISR_VECTOR_IDX0}))) IntHandlerUSBInstance0_USBDMA_ISR( void );
    </#if>
void IntHandlerUSBInstance0_USBDMA ( void )
				</#if>
			<#else>
void __ISR ( ${CONFIG_DRV_USBDMA_ISR_VECTOR_IDX0},ipl${CONFIG_DRV_USBDMA_INT_PRIO_NUM_IDX0}AUTO) _IntHandlerUSBInstance0_USBDMA ( void )
			</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
    OS_EnterNestableInterrupt();
</#if>
</#if>
    DRV_USBHS_Tasks_ISR_USBDMA(sysObj.drvUSBObject);
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
    OS_LeaveNestableInterrupt();
</#if>
</#if>
}
			</#if>
		</#if>
    <#elseif CONFIG_DRV_USB_HOST_SUPPORT == true >
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
<#if ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true))>
void __ISR( _USB_1_VECTOR , IPL4SOFT)_IntHandler_USB_stub ( void )
</#if>
<#if ((CONFIG_PIC32MZ == true) || (CONFIG_PIC32WK == true))>
void __ISR( _USB_VECTOR , IPL4AUTO)_IntHandler_USB_stub ( void )
</#if>
<#if  CONFIG_ARCH_ARM == true>
void USBHS_Handler ( void )
</#if>
<#else>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
<#if ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true))>
void __attribute__( (interrupt(ipl4AUTO), vector(_USB_1_VECTOR))) IntHandler_USB_stub_ISR( void );
</#if>
<#if ((CONFIG_PIC32MZ == true) || (CONFIG_PIC32WK == true))>
void __attribute__( (interrupt(ipl4AUTO), vector(_USB_VECTOR))) IntHandler_USB_stub_ISR( void );
</#if>
<#if  CONFIG_ARCH_ARM == true>
void USBHS_Handler ( void )
</#if>
</#if>
<#if  CONFIG_ARCH_ARM == true>
void USBHS_Handler ( void )
<#else>
void IntHandler_USB_stub ( void )
</#if>
</#if>
<#else> <#-- No RTOS -->
<#if ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true))>
void __ISR( _USB_1_VECTOR , IPL4AUTO)_IntHandler_USB_stub ( void )
</#if>
<#if ((CONFIG_PIC32MZ == true) || (CONFIG_PIC32WK == true)) >
void __ISR( _USB_VECTOR , IPL4AUTO)_IntHandler_USB_stub ( void )
</#if>
<#if  CONFIG_ARCH_ARM == true>
void USBHS_Handler ( void )
</#if>
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
    OS_EnterNestableInterrupt();
</#if>
</#if>
<#if CONFIG_PIC32MZ == true >
    DRV_USBHS_Tasks_ISR(sysObj.drvUSBObject);
</#if>	
<#if ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
    DRV_USBFS_Tasks_ISR(sysObj.drvUSBObject);
</#if>
<#if  CONFIG_ARCH_ARM == true>
    DRV_USBHSV1_Tasks_ISR(sysObj.drvUSBObject);
</#if>	
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
    OS_LeaveNestableInterrupt();
</#if>
</#if>
}

<#if CONFIG_PIC32MZ == true>
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void __ISR ( _USB_DMA_VECTOR, IPL4AUTO)  _IntHandlerUSBInstance0_USBDMA ( void )
<#else>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
void __attribute__( (interrupt(ipl4AUTO), vector(_USB_DMA_VECTOR))) IntHandlerUSBInstance0_USBDMA_ISR( void );
</#if>
void IntHandlerUSBInstance0_USBDMA ( void )
</#if>
<#else>
void __ISR ( _USB_DMA_VECTOR, IPL4AUTO)  _IntHandlerUSBInstance0_USBDMA ( void )
</#if>
{
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context save.  */
   _tx_thread_context_save();
</#if>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
    OS_EnterNestableInterrupt();
</#if>
</#if>
     DRV_USBHS_Tasks_ISR_USBDMA(sysObj.drvUSBObject);
<#if CONFIG_USE_3RDPARTY_RTOS>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
   /* Call ThreadX context restore.  */
   _tx_thread_context_restore();
</#if>
<#if CONFIG_3RDPARTY_RTOS_USED == "embOS">
    OS_LeaveNestableInterrupt();
</#if>
</#if>
}
			</#if>
    </#if>
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->
