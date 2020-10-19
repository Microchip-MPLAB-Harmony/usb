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
void DRV_USB_VBUSPowerEnable(uint8_t port, bool enable)
{
    /* Note: USB Host applications should have a way for Enabling/Disabling the 
       VBUS. Applications can use a GPIO to turn VBUS on/off through a switch. 
       In MHC Pin Settings select the pin used as VBUS Power Enable as output and 
       name it to "VBUS_AH". If you a see a build error from this function either 
       you have not configured the VBUS Power Enable in MHC pin settings or the 
       Pin name entered in MHC is not "VBUS_AH". */ 
    if (enable == true)
    {
        /* Enable the VBUS */
<#if core.DeviceFamily == "SAMA5D2">
        VBUS_AH_PowerEnable();
<#elseif core.DeviceFamily == "SAM9X60">
        VBUS_AH_PD14_PowerEnable();
        VBUS_AH_PD15_PowerEnable();
        VBUS_AH_PD16_PowerEnable();
<#elseif core.DeviceFamily == "SAM_G55">
    <#if USB_HOST_VBUS_ENABLE == true>    
        VBUS_AH_PowerEnable();
     </#if>	
</#if>
    }
    else
    {
        /* Disable the VBUS */
<#if core.DeviceFamily == "SAMA5D2">
        VBUS_AH_PowerDisable();
<#elseif core.DeviceFamily == "SAM9X60">
        VBUS_AH_PD14_PowerDisable();
        VBUS_AH_PD15_PowerDisable();
        VBUS_AH_PD16_PowerDisable();
<#elseif core.DeviceFamily == "SAM_G55">
    <#if USB_HOST_VBUS_ENABLE == true>
       VBUS_AH_PowerDisable();
    </#if>
</#if>
    }
}

<#if (USB_DRV_EHCI_MENU)?has_content == true>
    <#if USB_DRV_EHCI_MENU == true>
DRV_USB_EHCI_INIT drvUSBEHCIInit =
{
<#if core.DeviceFamily == "SAMA5D2">
    /* Interrupt Source for USB module */
    .interruptSource = (INT_SOURCE)41,
<#elseif core.DeviceFamily == "SAM9X60">
    /* Interrupt Source for USB module */
    .interruptSource = (INT_SOURCE)22,
</#if>

    /* USB base address */
    .usbID = ((uhphs_registers_t*)UHPHS_EHCI_ADDR),
    
    /* Ports Selection */ 
    .bmPortSelect = ${USB_DRV_HOST_EHCI_PORTS_SELECTION},

    /* USB Host Power Enable. USB Driver uses this function to Enable the VBUS */ 
    .portPowerEnable = DRV_USB_VBUSPowerEnable,
    
    /* Root hub available current in milliamperes */    
    .rootHubAvailableCurrent = 500,

    .companionDriverIndex = DRV_USB_OHCI_INDEX_0

};
    </#if>
</#if>

<#if (USB_DRV_OHCI_MENU)?has_content == true>
    <#if USB_DRV_OHCI_MENU == true>
DRV_USB_OHCI_INIT drvUSBOHCIInit =
{
<#if core.DeviceFamily == "SAMA5D2">
    /* Interrupt Source for USB module */
    .interruptSource = (INT_SOURCE)41,
<#elseif core.DeviceFamily == "SAM9X60">
     /* Interrupt Source for USB module */
    .interruptSource = (INT_SOURCE)22,
<#elseif core.DeviceFamily == "SAM_G55">
    /* Interrupt Source for USB module */
    .interruptSource = (INT_SOURCE)UHP_IRQn,
</#if>

<#if core.DeviceFamily == "SAMA5D2" || core.DeviceFamily == "SAM9X60" >
    /* USB base address */
    .usbID = ((UhpOhci *)UHPHS_OHCI_ADDR),
<#elseif core.DeviceFamily == "SAM_G55">
    /* USB base address */
    .usbID = ((UhpOhci*)0x20400000),   
</#if>	

     /* Ports Selection */ 
    .bmPortSelect = ${USB_DRV_HOST_OHCI_PORTS_SELECTION},
    
    /* USB Host Power Enable. USB Driver uses this function to Enable the VBUS */ 
    .portPowerEnable = DRV_USB_VBUSPowerEnable,
    
    /* Root hub available current in milliamperes */    
    .rootHubAvailableCurrent = 500
};
    </#if>
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->

