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
/******************************************************
 * USB Driver Initialization
 ******************************************************/
 
 <#if (USB_OPERATION_MODE == "Device")>
static DRV_USB_VBUS_LEVEL DRV_USBHSV1_VBUS_Comparator(void)
{
    DRV_USB_VBUS_LEVEL retVal = DRV_USB_VBUS_LEVEL_INVALID;

    /*if(true == USB_VBUS_INState_Get())
    {
        retVal = DRV_USB_VBUS_LEVEL_VALID;
    }*/
	retVal = DRV_USB_VBUS_LEVEL_VALID;
    return (retVal);

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
	
<#if (USB_OPERATION_MODE == "Device")>    
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

