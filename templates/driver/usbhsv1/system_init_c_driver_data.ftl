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
static DRV_USB_VBUS_LEVEL DRV_USBHSV1_VBUS_Comparator(void)
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
<#if (USB_OPERATION_MODE == "Host") && (USB_HOST_VBUS_ENABLE == true)> 
void DRV_USB_VBUSPowerEnable(uint8_t port, bool enable)
{
	/* Note: Most USB Host applications will likely to have a way for 
	   Enabling/Disabling the VBUS. Applications can use a GPIO to turn VBUS
	   on/off through a switch. In MHC Pin Settings select the pin used as 
	   VBUS Power Enable as output and name it to "VBUS_AH". If you a see a 
	   build error from this function either you have not configured the VBUS 
	   Power Enable in MHC pin settings or the Pin name entered in MHC is not 
	   "VBUS_AH". */
	<#if USB_DEVICE_VBUS_SENSE_PIN_NAME?has_content >	   
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
	</#if>
}
</#if>

const DRV_USBHSV1_INIT drvUSBInit =
{
    /* Interrupt Source for USB module */
    .interruptSource = USBHS_IRQn,

    /* System module initialization */
    .moduleInit = {0},

<#if (USB_OPERATION_MODE == "Device")>
    /* USB Controller to operate as USB Device */
    .operationMode = DRV_USBHSV1_OPMODE_DEVICE,
<#elseif (USB_OPERATION_MODE == "Host")>
	/* USB Controller to operate as USB Host */
    .operationMode = DRV_USBHSV1_OPMODE_HOST,
</#if>

    /* To operate in USB Normal Mode */
<#if (USB_SPEED == "High Speed")>
    .operationSpeed = DRV_USBHSV1_DEVICE_SPEEDCONF_NORMAL,
<#elseif (USB_SPEED == "Full Speed")>
	.operationSpeed = DRV_USBHSV1_DEVICE_SPEEDCONF_LOW_POWER,
</#if>    

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USBHS_REGS,
	
<#if (USB_OPERATION_MODE == "Device") && (USB_DEVICE_VBUS_SENSE == true)>     
    /* Function to check for VBus */
    .vbusComparator = DRV_USBHSV1_VBUS_Comparator
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

