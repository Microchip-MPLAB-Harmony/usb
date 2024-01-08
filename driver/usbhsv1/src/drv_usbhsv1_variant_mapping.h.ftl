/*******************************************************************************
  USB Driver Feature Variant Implementations

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usbhsv1_variant_mapping.h

  Summary:
    USB Driver Feature Variant Implementations

  Description:
    This file implements the functions which differ based on different parts
    and various implementations of the same feature.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef DRV_USBHSV1_VARIANT_MAPPING_H
#define DRV_USBHSV1_VARIANT_MAPPING_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"

/**********************************************
 * Macro Mapping
 **********************************************/
/* MISRA C-2012 Rule 5.4 deviated:1 Deviation record ID -  H3_USB_MISRAC_2012_R_5_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate:1 "MISRA C-2012 Rule 5.4" "H3_USB_MISRAC_2012_R_5_4_DR_1"
</#if>

/* With v1.04 the USB Driver implementation has been been split such
 * multiple USB Driver can be included in the same application. But to
 * continue support for application developed before v1.04, we should
 * map the DRV_USB configuration macros to DRV_USBHS macros */

#if (!defined(DRV_USBHSV1_INSTANCES_NUMBER))
    #error "DRV_USBHSV1_INSTANCES_NUMBER must be defined"
#endif

#if (!defined(DRV_USBHSV1_INTERRUPT_MODE))

/************************************************
 * This version of the driver does not support
 * mode.
 ***********************************************/



    /* Multi client operation in static is not supported */

    
    
        /* Map internal macros and functions to the static 
         * single open variant */
        




    /* This means that dynamic operation is requested */



// *****************************************************************************
// *****************************************************************************
// Section: USB Driver Static Object Generation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Macro: _DRV_USBHS_OBJ_MAKE_NAME(name)

  Summary:
    Creates an instance-specific static object name.

  Description:
     This macro creates the instance-specific name of the specified static object
     by inserting the index number into the name.

  Remarks:
    This macro does not affect the dynamic objects.
*/


/**********************************************
 * These macros allow variables to be compiled
 * based on dynamic or static buil.
 *********************************************/






  
// *****************************************************************************
/* Interrupt Source Control

  Summary:
    Macros to enable, disable or clear the interrupt source

  Description:
    These macros enable, disable, or clear the interrupt source.

    The macros get mapped to the respective SYS module APIs if the configuration
    option DRV_USBHSV1_INTERRUPT_MODE is set to true.

  Remarks:
    These macros are mandatory.
*/

    #error "Interrupt mode must be defined and must be either true or false"
#endif

#if (DRV_USBHSV1_INTERRUPT_MODE == true)

    #define M_DRV_USBHSV1_InterruptSourceEnable(source)                  SYS_INT_SourceEnable( source )
    #define M_DRV_USBHSV1_InterruptSourceDisable(source)                 SYS_INT_SourceDisable( source )
    #define M_DRV_USBHSV1_InterruptSourceClear(source)                    SYS_INT_SourceStatusClear( source )
    #define M_DRV_USBHSV1_InterruptSourceStatusGet(source)               SYS_INT_SourceStatusGet( source )
    #define M_DRV_USBHSV1_InterruptSourceStatusSet(source)               SYS_INT_SourceStatusSet( source )
    #define M_DRV_USBHSV1_InterruptVectorPrioritySet(source, priority)   SYS_INT_VectorPrioritySet(source, priority)
    #define M_DRV_USBHSV1_Tasks_ISR(object)
 
#endif

#if (DRV_USBHSV1_INTERRUPT_MODE == false)

    #define M_DRV_USBHSV1_InterruptSourceEnable(source)
    #define M_DRV_USBHSV1_InterruptSourceDisable(source)     false
    #define M_DRV_USBHSV1_InterruptSourceClear(source)       SYS_INT_SourceStatusClear( source )
    #define M_DRV_USBHSV1_InterruptSourceStatusGet(source)   SYS_INT_SourceStatusGet( source )
    #define M_DRV_USBHSV1_Tasks_ISR(object)                  DRV_USBHSV1_Tasks_ISR(object)

#endif

/**********************************************
 * Sets up driver mode-specific init routine
 * based on selected support.
 *********************************************/

#ifndef DRV_USBHSV1_DEVICE_SUPPORT
    #error "DRV_USBHSV1_DEVICE_SUPPORT must be defined and be either true or false"
#endif

#ifndef DRV_USBHSV1_HOST_SUPPORT
    #error "DRV_USBHSV1_HOST_SUPPORT must be defined and be either true or false"
#endif

#if (DRV_USBHSV1_DEVICE_SUPPORT == true)
    #define M_DRV_USBHSV1_DEVICE_INIT(x, y)      F_DRV_USBHSV1_DEVICE_Initialize(x , y)
    #define M_DRV_USBHSV1_DEVICE_TASKS_ISR(x)    F_DRV_USBHSV1_DEVICE_Tasks_ISR(x)
#elif (DRV_USBHSV1_DEVICE_SUPPORT == false)
    #define M_DRV_USBHSV1_DEVICE_INIT(x, y)  
    #define M_DRV_USBHSV1_DEVICE_TASKS_ISR(x)
#endif
 
#if (DRV_USBHSV1_HOST_SUPPORT == true)
    #define M_DRV_USBHSV1_HOST_INIT(x, y)    F_DRV_USBHSV1_HOST_Initialize(x , y)
    #define M_DRV_USBHSV1_HOST_TASKS_ISR(x)  F_DRV_USBHSV1_HOST_Tasks_ISR(x)
    #define M_DRV_USBHSV1_HOST_ATTACH_DETACH_STATE_MACHINE(x)  F_DRV_USBHSV1_HOST_AttachDetachStateMachine(x)
    #define M_DRV_USBHSV1_HOST_RESET_STATE_MACINE(x)  F_DRV_USBHSV1_HOST_ResetStateMachine(x)
#elif (DRV_USBHSV1_HOST_SUPPORT == false)
    #define M_DRV_USBHSV1_HOST_INIT(x, y)  
    #define M_DRV_USBHSV1_HOST_TASKS_ISR(x)
    #define M_DRV_USBHSV1_HOST_ATTACH_DETACH_STATE_MACHINE(x)  
    #define M_DRV_USBHSV1_HOST_RESET_STATE_MACINE(x)  
#endif

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.4"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */
#endif

