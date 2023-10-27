/*******************************************************************************
  USB Peripheral Library Template Implementation

  File Name:
    usb_OpModeSelect_Default.h

  Summary:
    USB PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : OpModeSelect
    and its Variant : Default
    For following APIs :
        PLIB_USB_OperatingModeSelect
        PLIB_USB_ExistsOpModeSelect

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

#ifndef USB_OPMODESELECT_DEFAULT_H
#define USB_OPMODESELECT_DEFAULT_H

#include "driver/usb/usbfs/src/templates/usbfs_registers.h"

/* MISRA C-2012 Rule 11.7 deviated:2 Deviation record ID -  H3_MISRAC_2012_R_11_7_DR_1 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma coverity compliance block deviate:2 "MISRA C-2012 Rule 11.7" "H3_MISRAC_2012_R_11_7_DR_1"
//******************************************************************************
/* Function :  USB_OperatingModeSelect_Default

  Summary:
    Implements Default variant of PLIB_USB_OperatingModeSelect

  Description:
    This template implements the Default variant of the PLIB_USB_OperatingModeSelect function.
*/

PLIB_TEMPLATE void USB_OperatingModeSelect_Default( USB_MODULE_ID index , USB_OPMODES opMode )
{
    volatile usb_registers_t   * usb = ((usb_registers_t *)(index));
    usb->UxCON.UxCONbits.USBEN_SOFEN = 0;
    usb->UxCON.UxCONbits.HOSTEN = 0 ;
    usb->UxOTGCON.OTGEN = 0;
    switch ( opMode )
    {
        case USB_OPMODE_NONE:
             break;
            
        case USB_OPMODE_DEVICE:
             usb->UxCON.UxCONbits.USBEN_SOFEN = 1; 
            break;

        case USB_OPMODE_HOST:
            usb->UxCON.UxCONbits.HOSTEN = 1 ;
            break;

        case USB_OPMODE_OTG:
            usb->UxOTGCON.OTGEN = 1;
            break;

        default:
            PLIB_ASSERT( 0, "In USB_OperatingModeSelect, unknown operating mode!" );
            break;
    }
}

//******************************************************************************
/* Function :  USB_ExistsOpModeSelect_Default

  Summary:
    Implements Default variant of PLIB_USB_ExistsOpModeSelect

  Description:
    This template implements the Default variant of the PLIB_USB_ExistsOpModeSelect function.
*/

#define PLIB_USB_ExistsOpModeSelect PLIB_USB_ExistsOpModeSelect
PLIB_TEMPLATE bool USB_ExistsOpModeSelect_Default( USB_MODULE_ID index )
{
    return true;
}

#pragma coverity compliance end_block "MISRA C-2012 Rule 11.7"
#pragma GCC diagnostic pop
/* MISRAC 2012 deviation block end */

#endif /*USB_OPMODESELECT_DEFAULT_H*/

/******************************************************************************
 End of File
*/

