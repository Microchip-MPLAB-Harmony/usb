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
<#if !(USB_TASKS?has_content)>
<#assign USB_TASKS = "TASK_CALL_NO_RTOS">
</#if>

<#if (USB_TASKS == "PROTO") && (CONFIG_USB_RTOS == "Standalone")>
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void _USB_Tasks(ULONG thread_input);
<#else>
void _USB_Tasks(void);
</#if>
</#if>
<#if USB_TASKS == "CREATE_TASK">
 <#if CONFIG_USB_RTOS == "Standalone">
 
    /* Create task for gfx state machine*/
<@RTOS_TASK_CREATE RTOS_NAME=CONFIG_3RDPARTY_RTOS_USED TASK_FUNC_NAME="_USB_Tasks" TASK_NAME="USB Tasks" TASK_PRI=CONFIG_USB_RTOS_TASK_PRIORITY TASK_STK_SZ=CONFIG_USB_RTOS_TASK_SIZE/>
 </#if>
</#if>
<#if (USB_TASKS == "TASK_CALL_NO_RTOS") || (USB_TASKS == "TASK_CALL" && CONFIG_USB_RTOS != "Standalone")>
    <#if ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
    
    /* USB FS Driver Task Routine */ 
     DRV_USBFS_Tasks(sysObj.drvUSBObject);
     
    <#elseif CONFIG_PIC32MZ == true>
    /* USB HS Driver Task Routine */ 
     DRV_USBHS_Tasks(sysObj.drvUSBObject);
     
    </#if>
    <#if CONFIG_USB_DEVICE_INST_IDX0 == true>
 
    /* USB Device layer tasks routine */ 
    USB_DEVICE_Tasks(sysObj.usbDevObject0);
    </#if>
    <#if CONFIG_DRV_USB_HOST_SUPPORT == true>
 
    /* USB Host layer task routine.*/
    USB_HOST_Tasks(sysObj.usbHostObject0);

    </#if>
</#if>
<#if USB_TASKS == "LOCAL_FUNCTION">
<#if CONFIG_USB_RTOS == "Standalone">
<#if CONFIG_3RDPARTY_RTOS_USED == "ThreadX">
void _USB_Tasks(ULONG thread_input)
<#else>
void _USB_Tasks(void)
</#if>
{
<#if CONFIG_3RDPARTY_RTOS_USED == "uC/OS-III">
    OS_ERR os_err;
	
</#if> 
    while(1)
    {
        <#if ((CONFIG_PIC32MX == true) || (CONFIG_PIC32MK == true) || (CONFIG_PIC32WK == true))>
        /* USBFS Driver Task Routine */ 
         DRV_USBFS_Tasks(sysObj.drvUSBObject);
         
        <#elseif CONFIG_PIC32MZ == true>
        /* USBHS Driver Task Routine */ 
         DRV_USBHS_Tasks(sysObj.drvUSBObject);
         
        </#if>
 <#if CONFIG_USB_DEVICE_INST_IDX0 == true>
        
        /* USB Device layer tasks routine */ 
        USB_DEVICE_Tasks(sysObj.usbDevObject0);
 </#if>
 
  <#if CONFIG_DRV_USB_HOST_SUPPORT == true>
 
        /* USB Host layer task routine.*/
        USB_HOST_Tasks(sysObj.usbHostObject0);

 </#if>

 <@RTOS_TASK_DELAY RTOS_NAME=CONFIG_3RDPARTY_RTOS_USED TASK_DELAY=CONFIG_USB_RTOS_DELAY/>
    }
 }
</#if>
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->
