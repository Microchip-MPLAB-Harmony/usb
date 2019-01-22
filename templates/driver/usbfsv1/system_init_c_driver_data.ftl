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
 
<#if (USB_OPERATION_MODE == "Device") && (USB_DEVICE_VBUS_SENSE == true)>
static DRV_USB_VBUS_LEVEL DRV_USBFSV1_VBUS_Comparator(void)
{
    DRV_USB_VBUS_LEVEL retVal = DRV_USB_VBUS_LEVEL_INVALID;
    
	<#if USB_DEVICE_VBUS_SENSE_PIN_NAME?has_content >
    if(true == ${USB_DEVICE_VBUS_SENSE_PIN_NAME}_Get())
    {
        retVal = DRV_USB_VBUS_LEVEL_VALID;
    }
	</#if>
	return (retVal);

}
</#if>

const DRV_USBFSV1_INIT drvUSBInit =
{
    /* Interrupt Source for USB module */
    .interruptSource = USB_IRQn,

    /* System module initialization */
    .moduleInit = {0},

<#if (USB_OPERATION_MODE == "Device")>
    /* USB Controller to operate as USB Device */
    .operationMode = DRV_USBFSV1_OPMODE_DEVICE,
<#elseif (USB_OPERATION_MODE == "Host")>
	/* USB Controller to operate as USB Host */
    .operationMode = DRV_USBFSV1_OPMODE_HOST,
</#if>

    /* USB Full Speed Operation */
	.operationSpeed = USB_SPEED_FULL,
    
    /* Stop in idle */
    .stopInIdle = false,

    /* Suspend in sleep */
    .suspendInSleep = false,

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USB_REGS,
	
<#if (USB_OPERATION_MODE == "Device") && (USB_DEVICE_VBUS_SENSE == true)>    
    /* Function to check for VBus */
    .vbusComparator = DRV_USBFSV1_VBUS_Comparator
<#elseif (USB_OPERATION_MODE == "Host")>
	/* Function to check for VBus */
    .vbusComparator = NULL,
            
    /* Root hub available current in milliamperes */
    .rootHubAvailableCurrent = 500,
</#if>
};

<#--
/*******************************************************************************
 End of File
*/
-->

