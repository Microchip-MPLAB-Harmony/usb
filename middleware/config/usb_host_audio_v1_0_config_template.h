/*******************************************************************************
  USB Host Audio v1.0 Class Configuration Definitions 

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_cdc_config_template.h

  Summary:
    USB host Audio v1.0 Class configuration definitions template

  Description:
    This file contains configurations macros needed to configure the Audio v1.0 host
    Driver. This file is a template file only. It should not be included by the
    application. The configuration macros defined in the file should be defined
    in the configuration specific system_config.h.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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

#ifndef _USB_HOST_AUDIO_V1_CONFIG_TEMPLATE_H_
#define _USB_HOST_AUDIO_V1_CONFIG_TEMPLATE_H_

#error "This is configuration template file. Do not include it directly."

// *****************************************************************************
// *****************************************************************************
// Section: USB Host Audio v1.0 Class Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Host Audio v1.0 Maximum Number of Instances

  Summary:
    Specifies the number of Audio v1.0 instances.

  Description:
    This macro defines the number of instances of the Audio v1.0 host Driver. For
    example, if the application needs to implement two instances of the Audio v1.0
    host Driver should be set to 2. 

  Remarks:
    None.
*/

#define USB_HOST_AUDIO_V1_INSTANCES_NUMBER /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Audio v1.0 Attach Listeners Number 
 
  Summary: 
    Defines the number of attach event listeners that can be registered with Audio v1.0
    Host Client Driver. 

  Description:
    The USB Audio v1.0 Host Client Driver provides attach notification to listeners who
    have registered with the client driver via the
    USB_HOST_AUDIO_V1_0_AttachEventHandlerSet() function. The
    USB_HOST_AUDIO_V1_0_ATTACH_LISTENERS_NUMBER configuration constant defines the
    maximum number of event handlers that can be set. This number should be set
    to equal the number of entities that interested in knowing when a Audio v1.0 device
    is attached.

  Remarks:
    None.
*/

#define USB_HOST_AUDIO_V1_ATTACH_LISTENERS_NUMBER  /*DOM-IGNORE-BEGIN*/1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Audio v1.0 Streaming Interfaces Number 
 
  Summary: 
    Defines the maximum number of streaming interfaces could be present in an 
    Audio v1.0 device that this Audio v1.0 Host Client Driver can support. 

  Description:
    This configuration constant defines maximum number of streaming interfaces
    could be present in an Audio v1.0 device that this Audio v1.0 Host Client 
    Driver can support. The value of this constant should be atleast 1.
    
    Example 1 - If the USB Audio v1.0 Host application must support a USB 
    Headset, this constant should be set 2 as an Audio Headset will have 
    atleast 2 Audio Streaming interfaces, one for Host to Device streaming
    and one for Device to Host streaming. 
    
    Example 2 - If the USB Audio v1.0 Hos t application must support a USB 
    Speaker, this constant should be set 1 as an Audio Speaker will have 
    atleast 1 Audio Streaming interface. 

  Remarks:
    Supporting multiple streaming interfaces requires more data memory and 
    processing time.
*/
#define USB_HOST_AUDIO_V1_STREAMING_INTERFACES_NUMBER  /*DOM-IGNORE-BEGIN*/2 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Audio v1.0 Streaming interface alternate setting number 
 
  Summary: 
    Defines maximum number of alternate settings per Streaming interface  
    provided by any Device that will be connected to this Audio Host.

  Description:
    This configuration constant defines maximum number of Streaming interface
    alternate settings provided by any Device that will be connected to this 
    Audio Host. The value of this constant should be at-least 1.
    
    Example  - If the USB Audio v1.0 Host application must support a USB 
    Audio Device with 2 alternate settings including Alternate Setting 0 
	then this constant should be defined to 3.  

  Remarks:
    Supporting multiple alternate settings per streaming interfaces requires 
    more data memory and processing time.
*/
#define USB_HOST_AUDIO_V1_STREAMING_INTERFACE_ALTERNATE_SETTINGS_NUMBER /*DOM-IGNORE-BEGIN*/1 /*DOM-IGNORE-END*/


#endif // #ifndef _USB_HOST_AUDIO_V1_CONFIG_TEMPLATE_H_

/*******************************************************************************
 End of File
*/

