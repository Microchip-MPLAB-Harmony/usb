/*******************************************************************************
  USB Peripheral Library Template Implementation

  File Name:
    usb_BDTBaseAddress_Default.h

  Summary:
    USB PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : BDTBaseAddress
    and its Variant : Default
    For following APIs :
        PLIB_USB_BDTBaseAddressGet
        PLIB_USB_BDTBaseAddressSet
        PLIB_USB_ExistsBDTBaseAddress

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

#ifndef USB_BDTBASEADDRESS_DEFAULT_H
#define USB_BDTBASEADDRESS_DEFAULT_H

#include "driver/usb/usbfs/src/templates/usbfs_registers.h"

/* MISRA C-2012 Rule 10.1, Rule 10.3, Rule 10.4, Rule 10.6,
   Rule 11.6, Rule 11.7, Rule 12.2. Deviation record ID -  
    H3_MISRAC_2012_R_10_1_DR_1, H3_MISRAC_2012_R_10_3_DR_1, 
    H3_MISRAC_2012_R_10_4_DR_1, H3_MISRAC_2012_R_10_6_DR_1,
    H3_MISRAC_2012_R_11_6_DR_1, H3_MISRAC_2012_R_11_7_DR_1, 
    and H3_MISRAC_2012_R_12_2_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:10 "MISRA C-2012 Rule 10.1" "H3_MISRAC_2012_R_10_1_DR_1" )\
(deviate:10 "MISRA C-2012 Rule 10.3" "H3_MISRAC_2012_R_10_3_DR_1" )\
(deviate:10 "MISRA C-2012 Rule 10.4" "H3_MISRAC_2012_R_10_4_DR_1" )\
(deviate:2 "MISRA C-2012 Rule 10.6" "H3_MISRAC_2012_R_10_6_DR_1" )\
(deviate:3 "MISRA C-2012 Rule 11.6" "H3_MISRAC_2012_R_11_6_DR_1" )\
(deviate:2 "MISRA C-2012 Rule 11.7" "H3_MISRAC_2012_R_11_7_DR_1" )\
(deviate:2 "MISRA C-2012 Rule 12.2" "H3_MISRAC_2012_R_12_2_DR_1" )
</#if>

//******************************************************************************
/* Function :  USB_BDTBaseAddressGet_Default

  Summary:
    Implements Default variant of PLIB_USB_BDTBaseAddressGet 

  Description:
    This template implements the Default variant of the
    PLIB_USB_BDTBaseAddressGet function.
*/

PLIB_TEMPLATE void* USB_BDTBaseAddressGet_Default( USB_MODULE_ID index )
{
    uint32_t retval;
    volatile usb_registers_t * usb = ((usb_registers_t *)(index));

    retval = ( ( usb->UxBDTP3.BDTPTRU << 24 ) | ( usb->UxBDTP2.BDTPTRH << 16 ) | ( usb->UxBDTP1.BDTPTRL << 9 ) );

    return ( void * )retval;
}

//******************************************************************************
/* Function :  USB_BDTBaseAddressSet_Default

  Summary:
    Implements Default variant of PLIB_USB_BDTBaseAddressSet 

  Description:
    This template implements the Default variant of the
    PLIB_USB_BDTBaseAddressSet function.
*/

PLIB_TEMPLATE void USB_BDTBaseAddressSet_Default( USB_MODULE_ID index , void* address )
{
    volatile usb_registers_t * usb = ((usb_registers_t *)(index));
    uint32_t value = (uint32_t) address;
    usb->UxBDTP3.BDTPTRU = ( value >> 24 ) & 0xFF ;
    usb->UxBDTP2.BDTPTRH = ( value >> 16 ) & 0xFF ;
    usb->UxBDTP1.BDTPTRL = ( value >> 9 ) & 0x7F;
}

//******************************************************************************
/* Function :  USB_ExistsBDTBaseAddress_Default

  Summary:
    Implements Default variant of PLIB_USB_ExistsBDTBaseAddress

  Description:
    This template implements the Default variant of the
    PLIB_USB_ExistsBDTBaseAddress function.
*/

#define PLIB_USB_ExistsBDTBaseAddress PLIB_USB_ExistsBDTBaseAddress
PLIB_TEMPLATE bool USB_ExistsBDTBaseAddress_Default( USB_MODULE_ID index )
{
    return true;
}

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.1"
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.3"
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.4"
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.6"
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.6"
#pragma coverity compliance end_block "MISRA C-2012 Rule 11.7"
#pragma coverity compliance end_block "MISRA C-2012 Rule 12.2"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */

#endif /*USB_BDTBASEADDRESS_DEFAULT_H*/

/******************************************************************************
 End of File
*/

