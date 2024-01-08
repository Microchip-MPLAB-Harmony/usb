/*******************************************************************************
  USBHS Peripheral Library Template Implementation

  File Name:
    usbhs_RxEPStatus_Default.h

  Summary:
    USBHS PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : RxEPStatus
    and its Variant : Default
    For following APIs :
        PLIB_USBHS_RxEPStatusGet
        PLIB_USBHS_RxEPStatusClear
        PLIB_USBHS_ExistsRxEPStatus

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/

//DOM-IGNORE-END

#ifndef USBHS_RXEPSTATUS_DEFAULT_H
#define USBHS_RXEPSTATUS_DEFAULT_H

#include "usbhs_registers.h"

/* MISRA C-2012 Rule 10.1, Rule 10.4, Rule 21.1 and Rule 21.2 Deviation record ID -  
    H3_USB_MISRAC_2012_R_10_1_DR_1,  H3_USB_MISRAC_2012_R_10_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:10 "MISRA C-2012 Rule 10.1" "H3_USB_MISRAC_2012_R_10_1_DR_1" )\
(deviate:10 "MISRA C-2012 Rule 10.4" "H3_USB_MISRAC_2012_R_10_4_DR_1" )
</#if>

//******************************************************************************
/* Function :  USBHS_RxEPStatusGet_Default

  Summary:
    Implements Default variant of PLIB_USBHS_RxEPStatusGet 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_RxEPStatusGet function.
*/

PLIB_TEMPLATE uint8_t USBHS_RxEPStatusGet_Default
( 
    USBHS_MODULE_ID index, 
    uint8_t endpoint 
)
{
    /* Return the RX endpoint status */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)(index + 0x1000);
    return(usbhs->EPCSR[endpoint].RXCSRL_DEVICEbits.w);
}

//******************************************************************************
/* Function :  USBHS_RxEPStatusClear_Default

  Summary:
    Implements Default variant of PLIB_USBHS_RxEPStatusClear 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_RxEPStatusClear function.
*/

PLIB_TEMPLATE void USBHS_RxEPStatusClear_Default
( 
    USBHS_MODULE_ID index, 
    uint8_t endpoint, 
    USBHS_RXEP_ERROR error 
)
{
    /* Clear the error in the status register */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)(index + 0x1000);
    usbhs->EPCSR[endpoint].RXCSRL_DEVICEbits.w &= (~(error)); 
}

//******************************************************************************
/* Function :  USBHS_RxEPINTokenSend_Default

  Summary:
    Implements Default variant of PLIB_USBHS_RxEPINTokenSend 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_RxEPINTokenSend function.
*/

PLIB_TEMPLATE void USBHS_RxEPINTokenSend_Default( USBHS_MODULE_ID index, uint8_t endpoint )
{
    /* Causes the module to send an IN Token in host mode */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)(index + 0x1000);
    usbhs->EPCSR[endpoint].RXCSRL_HOSTbits.REQPKT = 1; 
}

//******************************************************************************
/* Function :  USBHS_ExistsRxEPStatus_Default

  Summary:
    Implements Default variant of PLIB_USBHS_ExistsRxEPStatus

  Description:
    This template implements the Default variant of the PLIB_USBHS_ExistsRxEPStatus function.
*/

#define PLIB_USBHS_ExistsRxEPStatus PLIB_USBHS_ExistsRxEPStatus
PLIB_TEMPLATE bool USBHS_ExistsRxEPStatus_Default( USBHS_MODULE_ID index )
{
    return true;
}

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.1"
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.4"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */

#endif /*USBHS_RXEPSTATUS_DEFAULT_H*/

/******************************************************************************
 End of File
*/

