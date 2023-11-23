        
/*******************************************************************************
  USB Billboard class definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_billboard.h

  Summary:
    USB Billboard class definitions

  Description:
    This file describes the Billboard class specific definitions. File needs to
    be included by the application for USB Billboard host/device functionality.
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
// DOM-IGNORE-END

#ifndef _USB_BILLBOARD_H
#define _USB_BILLBOARD_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define USB_BILLBOARD_CLASS 0x11
#define USB_BILLBOARD_SUBCLASS 0x00
#define USB_BILLBOARD_PROTOCOL 0x00
#define USB_BILLBOARD_CAPABILITY_DESCRIPTOR_TYPE 0x0D

        

    
// *****************************************************************************
/* USB Billboard Capability Descriptor

  Summary:
    Identifies USB Billboard Capability Descriptor

  Description:
    This type identifies USB Billboard Capability Descriptor. This structure is
    as per Table 3-6: Billboard Capability Descriptor of USB Device Class Definition 
    for Billboard Devices protocol Revision 1.0 .

  Remarks:
    Needs to packed always.
*/
typedef struct __attribute__ ((packed))
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDevCapabilityType;
    uint8_t iAdditionalInfoURL;
    uint8_t bNumberOfAlternateModes;
    uint8_t bPreferredAlternateMode;
    uint16_t vconnPower; 
    uint8_t bmConfigured[32]; 
    uint32_t bReserved;
} USB_BILLBOARD_CAPABILITY_DESCRIPTOR_HEADER;
 
 
// *****************************************************************************
/* USB Billboard Alternate mode

  Summary:
    Identifies USB Billboard Alternate mode

  Description:
    This type identifies USB Billboard Alternate mode. This structure is
    as per Table 3-6: Billboard Capability Descriptor of USB Device Class Definition 
    for Billboard Devices protocol Revision 1.0 .

  Remarks:
    Needs to packed always.
*/ 
typedef struct __attribute__ ((packed))
{
    uint16_t wSVID;
    uint8_t bAlternateMode; 
    uint8_t iAlternateModeString;
} USB_BILLBOARD_ALTERNATE_MODE;
       
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END
 
        
#endif
       
