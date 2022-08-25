/*******************************************************************************
  USBHS Peripheral Library Template Implementation

  File Name:
    usbhs_USBIDControl_Default.h

  Summary:
    USBHS PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : USBIDControl
    and its Variant : Default
    For following APIs :
        PLIB_USBHS_ExistsUSBIDControl
        PLIB_USBHS_USBIDOverrideEnable
        PLIB_USBHS_USBIDOverrideDisable
        PLIB_USBHS_USBIDOverrideValueSet
        PLIB_USBHS_PhyIDMonitoringEnable
        PLIB_USBHS_PhyIDMonitoringDisable

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _USBHS_USBIDCONTROL_DEFAULT_H
#define _USBHS_USBIDCONTROL_DEFAULT_H

#include "usbhs_registers.h"

//******************************************************************************
/* Function :  USBHS_ExistsUSBIDControl_Default

  Summary:
    Implements Default variant of PLIB_USBHS_ExistsUSBIDControl

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_ExistsUSBIDControl function.
*/

#define PLIB_USBHS_ExistsUSBIDControl PLIB_USBHS_ExistsUSBIDControl
PLIB_TEMPLATE bool USBHS_ExistsUSBIDControl_Default( USBHS_MODULE_ID index )
{
    return true;
}

//******************************************************************************
/* Function :  USBHS_USBIDOverrideEnable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_USBIDOverrideEnable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_USBIDOverrideEnable function.
*/

PLIB_TEMPLATE void USBHS_USBIDOverrideEnable_Default( USBHS_MODULE_ID index )
{
    /* Get a pointer to the USBCRCON register and then set the USBIDOVEN bits.
     * The USBCRCON register does not have SET, CLR registers. */

    volatile usbhs_registers_t *usbhs = (usbhs_registers_t*)index;
    usbhs->ENDPOINT0.USBHS_CTRLA |= USBHS_CTRLA_IDOVEN(1);
}

//******************************************************************************
/* Function :  USBHS_USBIDOverrideDisable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_USBIDOverrideDisable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_USBIDOverrideDisable function.
*/

PLIB_TEMPLATE void USBHS_USBIDOverrideDisable_Default( USBHS_MODULE_ID index )
{

    volatile usbhs_registers_t *usbhs = (usbhs_registers_t*)index;
    usbhs->ENDPOINT0.USBHS_CTRLA |= USBHS_CTRLA_IDOVEN(0);
}

//******************************************************************************
/* Function :  USBHS_USBIDOverrideValueSet_Default

  Summary:
    Implements Default variant of PLIB_USBHS_USBIDOverrideValueSet 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_USBIDOverrideValueSet function.
*/

PLIB_TEMPLATE void USBHS_USBIDOverrideValueSet_Default
( 
    USBHS_MODULE_ID index , 
    USBHS_USBID_OVERRIDE_VALUE idValue
)
{

    volatile usbhs_registers_t *usbhs = (usbhs_registers_t*)index;
    usbhs->ENDPOINT0.USBHS_CTRLA |= USBHS_CTRLA_IDVAL(idValue);

}

//******************************************************************************
/* Function :  USBHS_PhyIDMonitoringEnable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_PhyIDMonitoringEnable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_PhyIDMonitoringEnable function.
*/

PLIB_TEMPLATE void USBHS_PhyIDMonitoringEnable_Default( USBHS_MODULE_ID index )
{
    /* Enables PHY ID monitoring. */
    volatile usbhs_registers_t *usbhs = (usbhs_registers_t*)index;
    usbhs->ENDPOINT0.USBHS_CTRLA |= USBHS_CTRLA_IDOVEN(0); 
}

//******************************************************************************
/* Function :  USBHS_PhyIDMonitoringDisable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_PhyIDMonitoringDisable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_PhyIDMonitoringDisable function.
*/

PLIB_TEMPLATE void USBHS_PhyIDMonitoringDisable_Default( USBHS_MODULE_ID index )
{
    /* Enables PHY ID monitoring. */
    volatile usbhs_registers_t *usbhs = (usbhs_registers_t*)index;
    usbhs->ENDPOINT0.USBHS_CTRLA |= USBHS_CTRLA_IDOVEN(1); 
}


#endif /*_USBHS_USBIDCONTROL_DEFAULT_H*/

/******************************************************************************
 End of File
*/

