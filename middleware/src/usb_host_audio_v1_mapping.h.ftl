/*******************************************************************************
  USB Host Audio v1.0 Client Driver mapping

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_audio_v1_mapping.h

  Summary:
    USB Host Audio v1.0 Client Driver mapping

  Description:
    This file contain mapppings required for the USB Host Audio v1.0 Client driver
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

#ifndef USB_HOST_AUDIO_V1_MAPPING_H
#define USB_HOST_AUDIO_V1_MAPPING_H

#include "usb_external_dependencies.h"
#include "usb_host_audio_local.h"

/* MISRA C-2012 Rule 5.8 deviated:4 Deviation record ID -  H3_USB_MISRAC_2012_R_5_8_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate:4 "MISRA C-2012 Rule 5.8" "H3_USB_MISRAC_2012_R_5_8_DR_1" 
</#if>

#if defined (USB_HOST_AUDIO_V1_0_INSTANCES_NUMBER) && !defined (USB_HOST_AUDIO_V1_INSTANCES_NUMBER)
 #define USB_HOST_AUDIO_V1_INSTANCES_NUMBER USB_HOST_AUDIO_V1_0_INSTANCES_NUMBER
 #endif 

#if defined (USB_HOST_AUDIO_V1_0_STREAMING_INTERFACES_NUMBER) && !defined (USB_HOST_AUDIO_V1_STREAMING_INTERFACES_NUMBER)
 #define USB_HOST_AUDIO_V1_STREAMING_INTERFACES_NUMBER USB_HOST_AUDIO_V1_0_STREAMING_INTERFACES_NUMBER
 #endif 

#if defined (USB_HOST_AUDIO_V1_0_STREAMING_INTERFACE_ALTERNATE_SETTINGS_NUMBER) && !defined (USB_HOST_AUDIO_V1_STREAMING_INTERFACE_ALTERNATE_SETTINGS_NUMBER)
 #define USB_HOST_AUDIO_V1_STREAMING_INTERFACE_ALTERNATE_SETTINGS_NUMBER USB_HOST_AUDIO_V1_0_STREAMING_INTERFACE_ALTERNATE_SETTINGS_NUMBER + 1
 #endif 


#define USB_HOST_AUDIO_V1_0_INSTANCE  USB_HOST_AUDIO_V1_INSTANCE
#define USB_HOST_AUDIO_V1_0_ControlRequestCallback F_USB_HOST_AUDIO_V1_ControlRequestCallback

#define USB_HOST_AUDIO_V1_StreamWrite(handle, transferHandle, source, length)  F_USB_HOST_AUDIO_V1_StreamWrite(handle, transferHandle, source, length, USB_HOST_AUDIO_V1_API_VERSION_FLAG_V1)
#define USB_HOST_AUDIO_V1_0_StreamWrite(handle, transferHandle, source, length)  F_USB_HOST_AUDIO_V1_StreamWrite(handle, transferHandle, source, length, USB_HOST_AUDIO_V1_API_VERSION_FLAG_V1_0_DEPRECIATED)
#define USB_HOST_AUDIO_V1_StreamRead(handle, transferHandle, source, length)  F_USB_HOST_AUDIO_V1_StreamRead(handle, transferHandle, source, length, USB_HOST_AUDIO_V1_API_VERSION_FLAG_V1)
#define USB_HOST_AUDIO_V1_0_StreamRead(handle, transferHandle, source, length)  F_USB_HOST_AUDIO_V1_StreamRead(handle, transferHandle, source, length, USB_HOST_AUDIO_V1_API_VERSION_FLAG_V1_0_DEPRECIATED)

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.8"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */

#endif
