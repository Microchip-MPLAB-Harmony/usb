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
 
<#if (USB_DEVICE_VBUS_SENSE == true)>
static DRV_USB_VBUS_LEVEL DRV_USB_UDPHS_VBUS_Comparator(void)
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

const DRV_USB_UDPHS_INIT drvUSBInit =
{
    /* Interrupt Source for USB module */
    .interruptSource = UDPHS_IRQn,

    /* System module initialization */
    .moduleInit = {0},

    /* To operate in USB Normal Mode */
<#if (USB_SPEED == "High Speed")>
    .operationSpeed = USB_SPEED_HIGH,
<#elseif (USB_SPEED == "Full Speed")>
	.operationSpeed = USB_SPEED_FULL,
</#if>    

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = UDPHS_REGS,
	
<#if (USB_DEVICE_VBUS_SENSE == true)>     
    /* Function to check for VBus */
    .vbusComparator = DRV_USB_UDPHS_VBUS_Comparator
<#else>
	/* Function to check for VBus */
    .vbusComparator = NULL
</#if>
};

<#--
/*******************************************************************************
 End of File
*/
-->

