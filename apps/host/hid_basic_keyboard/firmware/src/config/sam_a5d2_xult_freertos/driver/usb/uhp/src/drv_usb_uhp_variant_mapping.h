/*******************************************************************************
  USB Driver Feature Variant Implementations

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_uhp_ehci_variant_mapping.h

  Summary:
    USB Driver Feature Variant Implementations

  Description:
    This file implements the functions which differ based on different parts
    and various implementations of the same feature.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _DRV_USB_UHP_VARIANT_MAPPING_H
#define _DRV_USB_UHP_VARIANT_MAPPING_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"

/**********************************************
 * Macro Mapping
 **********************************************/

/* With v1.04 the USB Driver implementation has been been split such
 * multiple USB Driver can be included in the same application. But to
 * continue support for application developed before v1.04, we should
 * map the DRV_USB configuration macros to DRV_USBHS macros */

#if (!defined(DRV_USB_UHP_INSTANCES_NUMBER))
	#error "DRV_USB_UHP_INSTANCES_NUMBER must be defined"
#endif

#if (!defined(DRV_USB_UHP_INTERRUPT_MODE))




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
    option DRV_USB_UHP_INTERRUPT_MODE is set to true.

  Remarks:
    These macros are mandatory.
*/

    #error "Interrupt mode must be defined and must be either true or false"
#endif

#if (DRV_USB_UHP_INTERRUPT_MODE == true)

    #define _DRV_USB_UHP_InterruptSourceEnable(source)            SYS_INT_SourceEnable( source )
    #define _DRV_USB_UHP_InterruptSourceDisable(source)           SYS_INT_SourceDisable( source )
    #define _DRV_USB_UHP_InterruptSourceClear(source)		   SYS_INT_SourceStatusClear( source )
    #define _DRV_USB_UHP_InterruptSourceStatusGet(source)               SYS_INT_SourceStatusGet( source )
    #define _DRV_USB_UHP_InterruptSourceStatusSet(source)               SYS_INT_SourceStatusSet( source )
    #define _DRV_USB_UHP_InterruptVectorPrioritySet(source, priority)   SYS_INT_VectorPrioritySet(source, priority)
    #define _DRV_USB_UHP_Tasks_ISR(object)                             
 
#endif

#if (DRV_USB_UHP_INTERRUPT_MODE == false)

    #define _DRV_USB_UHP_InterruptSourceEnable(source)      
    #define _DRV_USB_UHP_InterruptSourceDisable(source)     
    #define _DRV_USB_UHP_InterruptSourceClear(source)       
    #define _DRV_USB_UHP_InterruptSourceStatusGet(source)   
    #define _DRV_USB_UHP_Tasks_ISR(object)                   DRV_USB_UHP_Tasks_ISR(object) 

#endif

/**********************************************
 * Sets up driver mode-specific init routine
 * based on selected support.
 *********************************************/

#define _DRV_USB_UHP_HOST_INIT(x, y)                      _DRV_USB_UHP_HOST_Initialize(x , y)
#define _DRV_USB_UHP_HOST_RESET_STATE_MACHINE(x)          _DRV_USB_UHP_HOST_ResetStateMachine(x)

#endif
