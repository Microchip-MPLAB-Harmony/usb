/*******************************************************************************
  USB Device CDC Class Configuration Definitions 

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_cdc_config_template.h

  Summary:
    USB device CDC Class configuration definitions template

  Description:
    This file contains configurations macros needed to configure the CDC
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

#ifndef _USB_DEVICE_CDC_CONFIG_TEMPLATE_H_
#define _USB_DEVICE_CDC_CONFIG_TEMPLATE_H_

#error "This is configuration template file. Do not include it directly."

// *****************************************************************************
// *****************************************************************************
// Section: USB Device CDC Class Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB device CDC Maximum Number of instances

  Summary:
    Specifies the number of CDC instances.

  Description:
    This macro defines the number of instances of the CDC Function Driver. For
    example, if the application needs to implement two instances of the CDC
    Function Driver (to create two COM ports) on one USB Device, the macro
    should be set to 2. Note that implementing a USB Device that features
    multiple CDC interfaces requires appropriate USB configuration descriptors. 

  Remarks:
    None.
*/

#define USB_DEVICE_CDC_INSTANCES_NUMBER       /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB device CDC Combined Queue Size

  Summary:
    Specifies the combined queue size of all CDC instances.

  Description:
    This macro defines the number of entries in all queues in all instances of
    the CDC function driver. This value can be obtained by adding up the read
    and write queue sizes of each CDC Function driver instance. In a simple
    single instance USB CDC device application, that does not require buffer
    queuing and serial state notification, the
    USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED macro can be set to 2. Consider a case
    with two CDC function driver instances, CDC 1 has a read queue size of 2 and
    write queue size of 3, CDC 2 has a read queue size of 4 and write queue size
    of 1, this macro should be set to 10 (2 +3 + 4 + 1). 

  Remarks:
    None.
*/

#define USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED /*DOM-IGNORE-BEGIN*/ 2 /*DOM-IGNORE-END*/

#endif // #ifndef _USB_DEVICE_CDC_CONFIG_TEMPLATE_H_

/*******************************************************************************
 End of File
*/

