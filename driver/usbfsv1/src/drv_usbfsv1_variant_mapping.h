/*******************************************************************************
  USB Driver Feature Variant Implementations

  Company:
    Microchip Technology Inc.

  File Name:
    drv_USBFSV1_variant_mapping.h

  Summary:
    USB Driver Feature Variant Implementations

  Description:
    This file implements the functions which differ based on different parts
    and various implementations of the same feature.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip  Technology  Inc.   All  rights  reserved.

Microchip licenses to  you  the  right  to  use,  modify,  copy  and  distribute
Software only when embedded on a Microchip  microcontroller  or  digital  signal
controller  that  is  integrated  into  your  product  or  third  party  product
(pursuant to the  sublicense  terms  in  the  accompanying  license  agreement).

You should refer  to  the  license  agreement  accompanying  this  Software  for
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
//DOM-IGNORE-END

#ifndef _DRV_USBFSV1_VARIANT_MAPPING_H
#define _DRV_USBFSV1_VARIANT_MAPPING_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************


#include "configuration.h"

/**********************************************
 * Macro Mapping
 **********************************************/

/* With v1.04 the USB Driver implementation has been been split such
 * multiple USB Driver can be included in the same application. But to
 * continue support for application developed before v1.04, we should
 * map the DRV_USB configuration macros to DRV_USBFSV1 macros */

#if defined(DRV_USB_INSTANCES_NUMBER)
    #define DRV_USBFSV1_INSTANCES_NUMBER  DRV_USB_INSTANCES_NUMBER
#endif

#if defined(DRV_USB_DEVICE_SUPPORT)
    #define DRV_USBFSV1_DEVICE_SUPPORT  DRV_USB_DEVICE_SUPPORT
#endif

#if defined(DRV_USB_HOST_SUPPORT)
    #define DRV_USBFSV1_HOST_SUPPORT  DRV_USB_HOST_SUPPORT
#endif

#if defined(DRV_USB_ENDPOINTS_NUMBER)
    #define DRV_USBFSV1_ENDPOINTS_NUMBER  DRV_USB_ENDPOINTS_NUMBER
#endif

/**********************************************
 * Sets up driver mode-specific init routine
 * based on selected support.
 *********************************************/

#ifndef DRV_USBFSV1_DEVICE_SUPPORT
    #error "DRV_USBFSV1_DEVICE_SUPPORT must be defined and be either true or false"
#endif

#ifndef DRV_USBFSV1_HOST_SUPPORT
    #error "DRV_USBFSV1_HOST_SUPPORT must be defined and be either true or false"
#endif

#if (DRV_USBFSV1_DEVICE_SUPPORT == true)
    #define _DRV_USBFSV1_DEVICE_INIT(x, y)      _DRV_USBFSV1_DEVICE_Initialize(x , y)
    #define _DRV_USBFSV1_DEVICE_TASKS_ISR(x)    _DRV_USBFSV1_DEVICE_Tasks_ISR(x)
    #define _DRV_USBFSV1_FOR_DEVICE(x, y)       x y
#elif (DRV_USBFSV1_DEVICE_SUPPORT == false)
    #define _DRV_USBFSV1_DEVICE_INIT(x, y)  
    #define _DRV_USBFSV1_DEVICE_TASKS_ISR(x) 
    #define _DRV_USBFSV1_FOR_DEVICE(x, y)
#endif
 
#if (DRV_USBFSV1_HOST_SUPPORT == true)
    #define _DRV_USBFSV1_HOST_INIT(x, y)    _DRV_USBFSV1_HOST_Initialize(x , y)
    #define _DRV_USBFSV1_HOST_TASKS_ISR(x)  _DRV_USBFSV1_HOST_Tasks_ISR(x)
    #define _DRV_USBFSV1_FOR_HOST(x, y)     x y
#elif (DRV_USBFSV1_HOST_SUPPORT == false)
    #define _DRV_USBFSV1_HOST_INIT(x, y)
    #define _DRV_USBFSV1_HOST_TASKS_ISR(x) 
    #define _DRV_USBFSV1_FOR_HOST(x, y)
#endif

#endif
