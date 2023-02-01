<#--
/*******************************************************************************
  USB Driver Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_init_c_driver_data.ftl

  Summary:
    USB Driver Freemarker Template File

  Description:
    This file contains configurations necessary to run the system.  It
    implements USB driver initialize data. 
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
/******************************************************
 * USB Driver Initialization
 ******************************************************/
<#if (USB_OPERATION_MODE == "DualRole") && (USB_HOST_VBUS_ENABLE == true) ||
  (USB_OPERATION_MODE == "Host") && (USB_HOST_VBUS_ENABLE == true)>

void DRV_USB_VBUSPowerEnable(uint8_t port, bool enable)
{
    /* Note: When operating in Host mode, the application can specify a Root 
       hub port enable function. The USB Host Controller driver initi data 
       structure has a member for specifying the root hub enable function. 
       This parameter should point to Root hub port enable function. If this 
       parameter is NULL, it implies that the port is always enabled. 
   
       This function generated by MHC to let the user enable the root hub. 
       User must use the MHC pin configuration utility to configure the pin 
       used for enabling VBUS  */
<#if USB_HOST_VBUS_ENABLE_PIN_NAME?has_content >	   
    if (enable == true)
    {
        /* Enable the VBUS */
        ${USB_HOST_VBUS_ENABLE_PIN_NAME}_PowerEnable();
    }
    else
    {
        /* Disable the VBUS */
        ${USB_HOST_VBUS_ENABLE_PIN_NAME}_PowerDisable();
    }
</#if>
}
</#if>

const DRV_USBHS_INIT drvUSBInit =
{
<#if (__PROCESSOR?contains("PIC32MZ") == true)>
    /* Interrupt Source for USB module */
    .interruptSource = INT_SOURCE_USB,

    /* Interrupt Source for USB module */
    .interruptSourceUSBDma = INT_SOURCE_USB_DMA,
<#elseif (__PROCESSOR?contains("PIC32CK") == true)>
    /* Interrupt Source for USBHS module */
    .interruptSource = USBHS_IRQn,

    /* Interrupt Source for USBHS module DMA */
    .interruptSourceUSBDma = USBHS_IRQn,
</#if>
    /* System module initialization */
    .moduleInit = {0},

<#if (USB_OPERATION_MODE == "Device")>
    /* USB Controller to operate as USB Device */
    .operationMode = DRV_USBHS_OPMODE_DEVICE,
<#elseif (USB_OPERATION_MODE == "Host")>
    /* USB Controller to operate as USB Host */
    .operationMode = DRV_USBHS_OPMODE_HOST,
<#elseif (USB_OPERATION_MODE == "DualRole")>
    /* USB Controller to operate as USB Host and Device */
    .operationMode = DRV_USB_OPMODE_DUAL_ROLE,
</#if>

<#if (USB_SPEED == "High Speed")>
    /* Enable High Speed Operation */
    .operationSpeed = USB_SPEED_HIGH,
<#elseif (USB_SPEED == "Full Speed")>
    /* Enable Full Speed Operation */
    .operationSpeed = USB_SPEED_FULL,
</#if>
    
    /* Stop in idle */
    .stopInIdle = true,

    /* Suspend in sleep */
    .suspendInSleep = false,

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USBHS_ID_0,

<#if (USB_OPERATION_MODE == "Host") ||  (USB_OPERATION_MODE == "DualRole")> 
<#if (USB_HOST_VBUS_ENABLE == true)> 
    /* USB Host Power Enable. USB Driver uses this function to Enable the VBUS */ 
    .portPowerEnable = DRV_USB_VBUSPowerEnable,
<#else>
    .portPowerEnable = NULL,
</#if>

    /* Root hub available current in milliamperes */
    .rootHubAvailableCurrent = 500,
</#if>
};

<#--
/*******************************************************************************
 End of File
*/
-->

