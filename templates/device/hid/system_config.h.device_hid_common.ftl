<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.device_hid_common.ftl

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
/* Maximum instances of HID function driver */
#define USB_DEVICE_HID_INSTANCES_NUMBER     ${__INSTANCE_COUNT} 

<#assign queuedepthCombined = 0>
<#list 1..10 as x>
  <#if x == 1>
  <#assign queuedepthCombined = usb_device_hid_0.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_hid_0.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 2>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_hid_1.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_hid_1.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 3>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_hid_2.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE
                                + usb_device_hid_2.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 4>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_hid_3.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_hid_3.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 5>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_hid_4.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_hid_4.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 6>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_hid_5.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_hid_5.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 7>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_hid_6.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_hid_6.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 8>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_hid_7.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_hid_7.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 9>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_hid_8.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_hid_8.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == 10>
  <#assign queuedepthCombined = queuedepthCombined 
                                + usb_device_hid_9.CONFIG_USB_DEVICE_FUNCTION_READ_Q_SIZE 
                                + usb_device_hid_9.CONFIG_USB_DEVICE_FUNCTION_WRITE_Q_SIZE>
  </#if>
  <#if x == __INSTANCE_COUNT>
    <#break>
  </#if>
</#list>
/* HID Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_HID_QUEUE_DEPTH_COMBINED                 ${queuedepthCombined}
<#--
/*******************************************************************************
 End of File
*/
-->

