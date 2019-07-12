/*******************************************************************************
  USB Driver Feature Variant Implementations

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usbdp_variant_mapping.h

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

#ifndef _DRV_USBDP_VARIANT_MAPPING_H
#define _DRV_USBDP_VARIANT_MAPPING_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************


#include "configuration.h"

/**********************************************
 * Macro Mapping
 **********************************************/


#if defined(DRV_USB_INSTANCES_NUMBER)
    #define DRV_USBDP_INSTANCES_NUMBER  DRV_USB_INSTANCES_NUMBER
#endif

#if defined(DRV_USB_ENDPOINTS_NUMBER)
    #define DRV_USBDP_ENDPOINTS_NUMBER  DRV_USB_ENDPOINTS_NUMBER
#endif

/**********************************************
 * Sets up driver mode-specific init routine
 * based on selected support.
 *********************************************/
#define _DRV_USBDP_ISR(x)                 DRV_USBDP_Tasks_ISR(x)
#define _DRV_USBDP_INIT(x, y)             _DRV_USBDP_Initialize(x , y)
#define _DRV_USBDP_TASKS_ISR(x)           _DRV_USBDP_Tasks_ISR(x)
#define _DRV_USBDP_FOR_DEVICE(x, y)       x y

#endif
