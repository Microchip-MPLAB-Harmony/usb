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
 

static uint8_t __attribute__((aligned(512))) endPointTable${INDEX?string}[DRV_USBFS_ENDPOINTS_NUMBER * 32];

<#if (USB_OPERATION_MODE == "Host")> 
static void DRV_USB_VBUSPowerEnable${INDEX?string}(uint8_t port, bool enable)
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
        VBUS_AH_PowerEnable();
    }
    else
    {
        /* Disable the VBUS */
        VBUS_AH_PowerDisable();
    }
}
</#if>

static const DRV_USBFS_INIT drvUSBFSInit${INDEX?string} =
{
     /* Assign the endpoint table */
    .endpointTable= endPointTable${INDEX?string},
    /* Interrupt Source for USB module */
<#if DRV_USB_PLIB == "PERIPHERAL_USB_1" >    
    .interruptSource = INT_SOURCE_USB_1 ,
<#elseif DRV_USB_PLIB == "PERIPHERAL_USB_2" >
    .interruptSource = INT_SOURCE_USB_2 ,
</#if>
   
<#if (USB_OPERATION_MODE == "Device")>
    /* USB Controller to operate as USB Device */
    .operationMode = DRV_USBFS_OPMODE_DEVICE,
<#elseif (USB_OPERATION_MODE == "Host")> 
    /* USB Controller to operate as USB Host */
    .operationMode = DRV_USBFS_OPMODE_HOST,
</#if>
    
    .operationSpeed = USB_SPEED_FULL,
 
    /* Stop in idle */
    .stopInIdle = false,
    
        /* Suspend in sleep */
    .suspendInSleep = false,
<#if (DRV_USB_PLIB?has_content) == true >
<#if DRV_USB_PLIB == "PERIPHERAL_USB_1" >
    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USB_ID_1,
<#elseif DRV_USB_PLIB == "PERIPHERAL_USB_2" >
    .usbID = USB_ID_2,
</#if>
<#else >
#error USB Peripheral is not connected
</#if>
    
<#if (USB_OPERATION_MODE == "Host")> 
    /* USB Host Power Enable. USB Driver uses this function to Enable the VBUS */ 
    .portPowerEnable = DRV_USB_VBUSPowerEnable${INDEX?string},
    
    /* Root hub available current in milliamperes */
    .rootHubAvailableCurrent = 500,
</#if>

};





<#--
/*******************************************************************************
 End of File
*/
-->

