/*******************************************************************************
  USB Device Audio Class Configuration Definitions 

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_audio_v1_0_config_template.h

  Summary:
    USB device Audio Class configuration definitions template

  Description:
    This file contains configurations macros needed to configure the Audio
    Function Driver. This file is a template file only. It should not be
    included by the application. The configuration macros defined in the file
    should be defined in the configuration specific system_config.h.
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

#ifndef _USB_DEVICE_AUDIO_V1_0_CONFIG_TEMPLATE_H_
#define _USB_DEVICE_AUDIO_V1_0_CONFIG_TEMPLATE_H_

#error "This is configuration template file. Do not include it directly."

// *****************************************************************************
// *****************************************************************************
// Section: USB Device Audio Class Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB device Audio Maximum Number of instances

  Summary:
    Specifies the number of Audio Function Driver instances.

  Description:
    This macro defines the number of instances of the Audio Function Driver. For
    example, if the application needs to implement two instances of the Audio
    Function Driver (to create two composite Audio Device) on one USB Device,
    the macro should be set to 2. Note that implementing a USB Device that
    features multiple Audio interfaces requires appropriate USB configuration
    descriptors. 

  Remarks:
    None.
*/

#define USB_DEVICE_AUDIO_INSTANCES_NUMBER       /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB device Audio Combined Queue Size

  Summary:
    Specifies the combined queue size of all Audio function driver instances.

  Description:
    This macro defines the number of entries in all queues in all instances of
    the Audio function driver. This value can be obtained by adding up the read
    and write queue sizes of each Audio Function driver instance. In a simple
    single instance USB Audio device application, that does not require buffer
    queuing, the USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED macro can be set to 2.
    Consider a case of a Audio function driver instances, with has a read queue
    size of 2 and write queue size of 3, this macro should be set to 5 (2 + 3). 

  Remarks:
    None.
*/

#define USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED /*DOM-IGNORE-BEGIN*/ 2 /*DOM-IGNORE-END*/

// *****************************************************************************
/* Maximum Audio Streaming Interfaces

  Summary:
    Specifies the maximum number of Audio Streaming interfaces in an Audio
    Function Driver instance.

  Description:
    This macro defines the maximum number of streaming interfaces in an Audio
    Function Driver instance. In case of multiple Audio Function Driver
    instances, this constant should be equal to the maximum number of interfaces
    amongst the Audio Function Driver instances.

  Remarks:
    None.
*/

#define USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/ 

// *****************************************************************************
/* Maximum number of Alternate Settings

  Summary:
    Specifies the maximum number of Alternate Settings per streaming interface.

  Description:
    This macro defines the maximum number of Alternate Settings per streaming
    interface. If the Audio Device features multiple streaming interfaces, this
    configuration constant should be equal to the the maximum number of
    alternate required amongst the streaming interfaces.

  Remarks:
    None.
*/

#define USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING /*DOM-IGNORE-BEGIN*/ 2 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Audio Transfer abort notification

  Summary:
    USB Audio Transfer abort notification enable

  Description:
    This macro enabled USB Audio Transfer abort notifications. Whenever a scheduled 
	transfer request is aborted due to Device Unplug or Host resets the device, the
	transfer complete event with status as aborted would be send to application's 
	event handler. 

  Remarks:
    None.
*/

#define USB_DEVICE_AUDIO_TRANSFER_ABORT_NOTIFY 

#endif
