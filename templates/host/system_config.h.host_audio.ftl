<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.host_audio.ftl

  Summary:
    USB Device Freemarker Template File

  Description:
    This file contains configurations necessary to run the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
*******************************************************************************/

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
-->

/* Number of Audio v1.0 Client driver instances in the application */
#define USB_HOST_AUDIO_V1_INSTANCES_NUMBER         ${CONFIG_USB_HOST_AUDIO_NUMBER_OF_INSTANCES}


/* MISRA C-2012 Rule 5.4 deviated:1, Deviation record ID -  H3_MISRAC_2012_R_5_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:1 "MISRA C-2012 Rule 5.4" "H3_MISRAC_2012_R_5_4_DR_1" )   
</#if>
/* Maximum number of Streaming interfaces provides by any Device that will be
 be connected to this Audio Host */
#define USB_HOST_AUDIO_V1_STREAMING_INTERFACES_NUMBER ${USB_HOST_AUDIO_NUMBER_OF_STREAMING_INTERFACES}

/* Maximum number of Streaming interface alternate settings provided by any 
   Device that will be connected to this Audio Host. (This number includes 
   alternate setting 0) */
#define USB_HOST_AUDIO_V1_STREAMING_INTERFACE_ALTERNATE_SETTINGS_NUMBER ${USB_HOST_AUDIO_NUMBER_OF_STREAMING_INTERFACE_SETTINGS}

/* Maximum number of discrete Sampling frequencies supported by the Attached Audio Device */ 
#define USB_HOST_AUDIO_V1_SAMPLING_FREQUENCIES_NUMBER ${USB_HOST_AUDIO_NUMBER_OF_SAMPLING_FREQUENCIES}

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.4"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>    
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->

