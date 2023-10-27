/*******************************************************************************
  USB Peripheral Library Template Implementation

  File Name:
    usb_ModulePower_32Bit16Bit.h

  Summary:
    USB PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : ModulePower
    and its Variant : 32Bit16Bit
    For following APIs :
        PLIB_USB_Enable
        PLIB_USB_Disable
        PLIB_USB_ExistsModulePower

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

#ifndef USB_MODULEPOWER_32BIT16BIT_H
#define USB_MODULEPOWER_32BIT16BIT_H

#include "driver/usb/usbfs/src/templates/usbfs_registers.h"

/* MISRA C-2012 Rule 11.7 deviated:2 Deviation record ID -  H3_MISRAC_2012_R_11_7_DR_1 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma coverity compliance block deviate:2 "MISRA C-2012 Rule 11.7" "H3_MISRAC_2012_R_11_7_DR_1"
//******************************************************************************
/* Function :  USB_Enable_32Bit16Bit

  Summary:
    Implements 32Bit16Bit variant of PLIB_USB_Enable 

  Description:
    This template implements the 32Bit16Bit variant of the PLIB_USB_Enable function.
*/

    
PLIB_TEMPLATE void USB_Enable_32Bit16Bit( USB_MODULE_ID index )
{
    volatile usb_registers_t   * usb = ((usb_registers_t *)(index));
    usb->UxPWRC.USBPWR = 1;
    
}

//******************************************************************************
/* Function :  USB_Disable_32Bit16Bit

  Summary:
    Implements 32Bit16Bit variant of PLIB_USB_Disable 

  Description:
    This template implements the 32Bit16Bit variant of the PLIB_USB_Disable function.
*/

PLIB_TEMPLATE void USB_Disable_32Bit16Bit( USB_MODULE_ID index )
{
    volatile usb_registers_t   * usb = ((usb_registers_t *)(index));
    usb->UxPWRC.USBPWR = 0 ;
 
}

//******************************************************************************
/* Function :  USB_ExistsModulePower_32Bit16Bit

  Summary:
    Implements 32Bit16Bit variant of PLIB_USB_ExistsModulePower

  Description:
    This template implements the 32Bit16Bit variant of the PLIB_USB_ExistsModulePower function.
*/

#define PLIB_USB_ExistsModulePower PLIB_USB_ExistsModulePower
PLIB_TEMPLATE bool USB_ExistsModulePower_32Bit16Bit( USB_MODULE_ID index )
{
    return true;
}

#pragma coverity compliance end_block "MISRA C-2012 Rule 11.7"
#pragma GCC diagnostic pop
/* MISRAC 2012 deviation block end */

#endif /*USB_MODULEPOWER_32BIT16BIT_H*/

/******************************************************************************
 End of File
*/

