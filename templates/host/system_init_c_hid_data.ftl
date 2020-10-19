<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_hid_data.ftl

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
<#if CONFIG_USB_HID_TOTAL_USAGE_DRIVER_INSTANCES gt 0>
<#if CONFIG_USB_HOST_USE_KEYBOARD == true>
USB_HOST_HID_USAGE_DRIVER_INTERFACE usageDriverInterfaceKeyboard =
{
  .initialize = NULL,
  .deinitialize = NULL,
  .usageDriverEventHandler = _USB_HOST_HID_KEYBOARD_EventHandler,
  .usageDriverTask = _USB_HOST_HID_KEYBOARD_Task
};
</#if>
<#if CONFIG_USB_HOST_USE_MOUSE == true>
USB_HOST_HID_USAGE_DRIVER_INTERFACE usageDriverInterfaceMouse =
{
  .initialize = NULL,
  .deinitialize = NULL,
  .usageDriverEventHandler = _USB_HOST_HID_MOUSE_EventHandler,
  .usageDriverTask = _USB_HOST_HID_MOUSE_Task
};
</#if>
USB_HOST_HID_USAGE_DRIVER_TABLE_ENTRY usageDriverTableEntry[${CONFIG_USB_HID_TOTAL_USAGE_DRIVER_INSTANCES}] =
{
<#if CONFIG_USB_HOST_USE_KEYBOARD == true>
    {
        .usage = (USB_HID_USAGE_PAGE_GENERIC_DESKTOP_CONTROLS << 16) | USB_HID_GENERIC_DESKTOP_KEYBOARD,
        .initializeData = NULL,
        .interface = &usageDriverInterfaceKeyboard
    },
</#if>
<#if CONFIG_USB_HOST_USE_MOUSE == true>
	{
        .usage = (USB_HID_USAGE_PAGE_GENERIC_DESKTOP_CONTROLS << 16) | USB_HID_USAGE_MOUSE,
        .initializeData = NULL,
        .interface = &usageDriverInterfaceMouse
    },
</#if>
};
</#if>

USB_HOST_HID_INIT hidInitData =
{
    .nUsageDriver = ${CONFIG_USB_HID_TOTAL_USAGE_DRIVER_INSTANCES},
<#if CONFIG_USB_HID_TOTAL_USAGE_DRIVER_INSTANCES gt 0>
    .usageDriverTable = usageDriverTableEntry
<#else>

	/* No usage drivers added. At least one Usage driver must be added */ 
	.usageDriverTable = NULL
</#if>
};

<#--
/*******************************************************************************
 End of File
*/
-->