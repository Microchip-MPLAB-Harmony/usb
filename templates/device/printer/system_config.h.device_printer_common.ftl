<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.device_printer_common.ftl

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
Copyright (c) 2019 released Microchip Technology Inc.  All rights reserved.

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
/* Maximum instances of Printer function driver */
#define USB_DEVICE_PRINTER_INSTANCES_NUMBER          ${__INSTANCE_COUNT}U 

<#assign queuedepthCombined = 0>
<#list 1..10 as x>
  <#if x == 1>
  <#assign queuedepthCombined = usb_device_printer_0.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_printer_0.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 2>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_printer_1.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_printer_1.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 3>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_printer_2.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE
                                + usb_device_printer_2.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 4>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_printer_3.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_printer_3.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 5>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_printer_4.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_printer_4.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 6>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_printer_5.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_printer_5.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 7>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_printer_6.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_printer_6.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 8>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_printer_7.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_printer_7.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 9>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_printer_8.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_printer_8.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 10>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_printer_9.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_printer_9.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == __INSTANCE_COUNT>
    <#break>
  </#if>
</#list>
/* Printer Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED                 ${queuedepthCombined}U

/* MISRA C-2012 Rule 5.4 deviated:1, Deviation record ID -  H3_MISRAC_2012_R_5_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:1 "MISRA C-2012 Rule 5.4" "H3_MISRAC_2012_R_5_4_DR_1" )   
</#if>

/* Length of the Device ID string including length in the first two bytes */
#define USB_DEVICE_PRINTER_DEVICE_ID_STRING_LENGTH    ${CONFIG_USB_PRINTER_DEVICE_ID_STRING?length?number+2}U


/* Device ID string including length in the first two bytes */
#define USB_DEVICE_PRINTER_DEVICE_ID_STRING <#if CONFIG_USB_PRINTER_DEVICE_ID_STRING?length gt 0 > {0,${CONFIG_USB_PRINTER_DEVICE_ID_STRING?length?number+2},<#list 1..CONFIG_USB_PRINTER_DEVICE_ID_STRING?length as index>'${CONFIG_USB_PRINTER_DEVICE_ID_STRING?substring(index-1,index)}'<#if index_has_next>,</#if></#list>},</#if>

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.4"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>    
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->

