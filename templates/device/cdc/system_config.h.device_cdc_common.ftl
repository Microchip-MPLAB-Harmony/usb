<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.device_cdc_common.ftl

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
/* Maximum instances of CDC function driver */
#define USB_DEVICE_CDC_INSTANCES_NUMBER                     ${__INSTANCE_COUNT}


<#assign queuedepthCombined = 0>
<#list 1..10 as x>
  <#if x == 1>
  <#assign queuedepthCombined = usb_device_cdc_0.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
				                + usb_device_cdc_0.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_0.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == 2>
  <#assign queuedepthCombined = queuedepthCombined 
					            + usb_device_cdc_1.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
								+ usb_device_cdc_1.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_1.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == 3>
  <#assign queuedepthCombined = queuedepthCombined 
								+ usb_device_cdc_2.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE
								+ usb_device_cdc_2.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_2.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == 4>
  <#assign queuedepthCombined = queuedepthCombined 
								+ usb_device_cdc_3.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
								+ usb_device_cdc_3.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_3.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == 5>
  <#assign queuedepthCombined = queuedepthCombined 
								+ usb_device_cdc_4.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
								+ usb_device_cdc_4.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_4.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == 6>
  <#assign queuedepthCombined = queuedepthCombined 
								+ usb_device_cdc_5.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
								+ usb_device_cdc_5.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_5.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == 7>
  <#assign queuedepthCombined = queuedepthCombined 
								+ usb_device_cdc_6.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
								+ usb_device_cdc_6.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_6.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == 8>
  <#assign queuedepthCombined = queuedepthCombined 
								+ usb_device_cdc_7.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
								+ usb_device_cdc_7.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_7.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == 9>
  <#assign queuedepthCombined = queuedepthCombined 
								+ usb_device_cdc_8.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
								+ usb_device_cdc_8.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_8.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == 10>
  <#assign queuedepthCombined = queuedepthCombined 
								+ usb_device_cdc_9.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
								+ usb_device_cdc_9.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE
								+ usb_device_cdc_9.CONFIG_USB_DEVICE_FUNCTION_SERIAL_NOTIFIACATION_Q_SIZE>
  </#if>
  <#if x == __INSTANCE_COUNT>
    <#break>
  </#if>
</#list>
/* CDC Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED                 ${queuedepthCombined}
<#--
/*******************************************************************************
 End of File
*/
-->

