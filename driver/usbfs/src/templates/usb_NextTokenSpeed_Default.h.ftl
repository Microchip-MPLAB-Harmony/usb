/*******************************************************************************
  USB Peripheral Library Template Implementation

  File Name:
    usb_NextTokenSpeed_Default.h

  Summary:
    USB PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : NextTokenSpeed
    and its Variant : Default
    For following APIs :
        PLIB_USB_TokenSpeedSelect
        PLIB_USB_ExistsNextTokenSpeed

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

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

#ifndef USB_NEXTTOKENSPEED_DEFAULT_H
#define USB_NEXTTOKENSPEED_DEFAULT_H

#include "driver/usb/usbfs/src/templates/usbfs_registers.h"

/* MISRA C-2012  Rule 10.3.
   Deviation record ID -  H3_USB_MISRAC_2012_R_10_3_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:4 "MISRA C-2012 Rule 10.3" "H3_USB_MISRAC_2012_R_10_3_DR_1" )
</#if>
//******************************************************************************
/* Function :  USB_TokenSpeedSelect_Default

  Summary:
    Implements Default variant of PLIB_USB_TokenSpeedSelect

  Description:
    This template implements the Default variant of the PLIB_USB_TokenSpeedSelect function.
*/

PLIB_TEMPLATE void USB_TokenSpeedSelect_Default( USB_MODULE_ID index , USB_TOKEN_SPEED tokenSpeed )
{
    volatile usb_registers_t   * usb = ((usb_registers_t *)(index));
    usb->UxADDR.LSPDEN =   ( tokenSpeed & 0x01 )^ 0x01 ;

}

//******************************************************************************
/* Function :  USB_ExistsNextTokenSpeed_Default

  Summary:
    Implements Default variant of PLIB_USB_ExistsNextTokenSpeed

  Description:
    This template implements the Default variant of the PLIB_USB_ExistsNextTokenSpeed function.
*/

#define PLIB_USB_ExistsNextTokenSpeed PLIB_USB_ExistsNextTokenSpeed
PLIB_TEMPLATE bool USB_ExistsNextTokenSpeed_Default( USB_MODULE_ID index )
{
    return true;
}

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.3"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */

#endif /*USB_NEXTTOKENSPEED_DEFAULT_H*/

/******************************************************************************
 End of File
*/

