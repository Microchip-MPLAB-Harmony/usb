/*******************************************************************************
  USBHS Peripheral Library Template Implementation

  File Name:
    usbhs_ModuleControl_Default.h

  Summary:
    USBHS PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : ModuleControl
    and its Variant : Default
    For following APIs :
        PLIB_USBHS_ResumeEnable
        PLIB_USBHS_ResumeDisable
        PLIB_USBHS_SuspendEnable
        PLIB_USBHS_SuspendDisable
        PLIB_USBHS_ResetEnable
        PLIB_USBHS_ResetDisable
        PLIB_USBHS_VBUSLevelGet
        PLIB_USBHS_HostModeIsEnabled
        PLIB_USBHS_IsBDevice
        PLIB_USBHS_SessionEnable
        PLIB_USBHS_SessionDisable
        PLIB_USBHS_DeviceAddressSet
        PLIB_USBHS_DeviceAttach
        PLIB_USBHS_DeviceDetach
        PLIB_USBHS_ExistsModuleControl

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

#ifndef USBHS_MODULECONTROL_DEFAULT_H
#define USBHS_MODULECONTROL_DEFAULT_H

#include "usbhs_registers.h"

/* MISRA C-2012 Rule 10.3, Rule 10.4, Rule 21.1 
   and Rule 21.2. Deviation record ID -  
    H3_USB_MISRAC_2012_R_10_3_DR_1, 
    H3_USB_MISRAC_2012_R_21_1_DR_1
    H3_USB_MISRAC_2012_R_10_4_DR_1, 
    and H3_USB_MISRAC_2012_R_21_2_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:10 "MISRA C-2012 Rule 10.3" "H3_USB_MISRAC_2012_R_10_3_DR_1" )\
(deviate:10 "MISRA C-2012 Rule 10.4" "H3_USB_MISRAC_2012_R_10_4_DR_1" )
</#if>

//******************************************************************************
/* Function :  USBHS_ResumeEnable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_ResumeEnable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_ResumeEnable function.
*/

PLIB_TEMPLATE void USBHS_ResumeEnable_Default( USBHS_MODULE_ID index )
{
    /* Function enables resume signaling */
    
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->POWERbits.RESUME = 1;
}

//******************************************************************************
/* Function :  USBHS_ResumeDisable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_ResumeDisable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_ResumeDisable function.
*/

PLIB_TEMPLATE void USBHS_ResumeDisable_Default( USBHS_MODULE_ID index )
{
    /* Function disables resume signaling */
    
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->POWERbits.RESUME = 0;
}

//******************************************************************************
/* Function :  USBHS_SuspendEnable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_SuspendEnable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_SuspendEnable function.
*/

PLIB_TEMPLATE void USBHS_SuspendEnable_Default( USBHS_MODULE_ID index )
{
    /* Function enables bus suspend  */
    
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->POWERbits.SUSPMODE = 1;
}

//******************************************************************************
/* Function :  USBHS_SuspendDisable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_SuspendDisable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_SuspendDisable function.
*/

PLIB_TEMPLATE void USBHS_SuspendDisable_Default( USBHS_MODULE_ID index )
{
    /* Function disables bus suspend  */
    
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->POWERbits.SUSPMODE = 0;
}

//******************************************************************************
/* Function :  USBHS_VBUSLevelGet_Default

  Summary:
    Implements Default variant of PLIB_USBHS_VBUSLevelGet 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_VBUSLevelGet function.
*/

PLIB_TEMPLATE USBHS_VBUS_LEVEL USBHS_VBUSLevelGet_Default( USBHS_MODULE_ID index )
{
    USBHS_VBUS_LEVEL vbusLevel = USBHS_VBUS_SESSION_END; 
    
    /* Function returns the current VBUS level */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    vbusLevel = usbhs->DEVCTLbits.w & 0x18U;
    return vbusLevel;
}

//******************************************************************************
/* Function :  USBHS_HostModeIsEnabled_Default

  Summary:
    Implements Default variant of PLIB_USBHS_HostModeIsEnabled 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_HostModeIsEnabled function.
*/

PLIB_TEMPLATE bool USBHS_HostModeIsEnabled_Default( USBHS_MODULE_ID index )
{
    /* Returns true if the Host Mode is enabled. */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    return((bool)(usbhs->DEVCTLbits.HOSTMODE));
}

//******************************************************************************
/* Function :  USBHS_SessionEnable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_SessionEnable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_SessionEnable function.
*/

PLIB_TEMPLATE void USBHS_SessionEnable_Default( USBHS_MODULE_ID index )
{
    /* Function enables a session  */
    
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->DEVCTLbits.SESSION = 1;
}


//******************************************************************************
/* Function :  USBHS_IsBDevice_Default

  Summary:
    Implements Default variant of PLIB_USBHS_IsBDevice 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_IsBDevice function.
*/

PLIB_TEMPLATE bool USBHS_IsBDevice_Default( USBHS_MODULE_ID index )
{
    /* Returns true if the Host Mode is enabled. */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    return((bool)(usbhs->DEVCTLbits.BDEV));
}

//******************************************************************************
/* Function :  USBHS_SessionDisable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_SessionDisable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_SessionDisable function.
*/

PLIB_TEMPLATE void USBHS_SessionDisable_Default( USBHS_MODULE_ID index )
{
    /* Clears the session bit */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->DEVCTLbits.SESSION = 0;
}

//******************************************************************************
/* Function :  USBHS_ResetEnable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_ResetEnable 

  Description:
    This template implements the Default variant of the PLIB_USBHS_ResetEnable function.
*/

PLIB_TEMPLATE void USBHS_ResetEnable_Default( USBHS_MODULE_ID index )
{
    /* Sets the reset bit */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->POWERbits.RESET = 1;
}

//******************************************************************************
/* Function :  USBHS_ResetDisable_Default

  Summary:
    Implements Default variant of PLIB_USBHS_ResetDisable 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_ResetDisable function.
*/

PLIB_TEMPLATE void USBHS_ResetDisable_Default( USBHS_MODULE_ID index )
{
   /* Sets the reset bit */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->POWERbits.RESET = 0;
}

//******************************************************************************
/* Function :  USBHS_DeviceAddressSet_Default

  Summary:
    Implements Default variant of PLIB_USBHS_DeviceAddressSet 

  Description:
    This template implements the Default variant of the PLIB_USBHS_DeviceAddressSet function.
*/

PLIB_TEMPLATE void USBHS_DeviceAddressSet_Default
( 
    USBHS_MODULE_ID index, 
    uint8_t address 
)
{
    /* Clears the reset bit */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->FADDRbits.FUNC = address;
}

//******************************************************************************
/* Function :  USBHS_DeviceAttach_Default

  Summary:
    Implements Default variant of PLIB_USBHS_DeviceAttach 

  Description:
    This template implements the Default variant of the 
    PLIB_USBHS_DeviceAttach function.
*/

PLIB_TEMPLATE void USBHS_DeviceAttach_Default( USBHS_MODULE_ID index, uint32_t speed )
{
    /* Attach the device at the specified speed */
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);

    /* Full speed value is 0x2 and high speed value is 0x1.
     * So if high speed is specified, then HSEN will be 1
     * and if full speed is specified, the HSEN will be 0 */
     // Force Device mode with ID pin override
    
    usbhs->POWERbits.HSEN = (speed & 0x1U);
    usbhs->POWERbits.SOFTCONN = 1;
  
    usbhs->INTRTXEbits.EP0IE = 0;
     

}

//******************************************************************************
/* Function :  USBHS_DeviceDetach_Default

  Summary:
    Implements Default variant of PLIB_USBHS_DeviceDetach 

  Description:
    This template implements the Default variant of the PLIB_USBHS_DeviceDetach function.
*/

PLIB_TEMPLATE void USBHS_DeviceDetach_Default( USBHS_MODULE_ID index)
{
   /* Detach the device. */
    
    volatile usbhs_registers_sw_t * usbhs = (usbhs_registers_sw_t *)((uint32_t)index + 0x1000U);
    usbhs->POWERbits.SOFTCONN = 0;
}

//******************************************************************************
/* Function :  USBHS_ExistsModuleControl_Default

  Summary:
    Implements Default variant of PLIB_USBHS_ExistsModuleControl

  Description:
    This template implements the Default variant of the PLIB_USBHS_ExistsModuleControl function.
*/

#define PLIB_USBHS_ExistsModuleControl PLIB_USBHS_ExistsModuleControl
PLIB_TEMPLATE bool USBHS_ExistsModuleControl_Default( USBHS_MODULE_ID index )
{
    return true;
}

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.3"
#pragma coverity compliance end_block "MISRA C-2012 Rule 10.4"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */

#endif /*USBHS_MODULECONTROL_DEFAULT_H*/

/******************************************************************************
 End of File
*/

