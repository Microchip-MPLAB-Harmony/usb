/*******************************************************************************
  USB Device HID Class Configuration Definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_hid_config_template.h

  Summary:
    USB device HID class configuration definitions template,

  Description:
    This file contains configurations macros needed to configure the HID
    Function Driver. This file is a template file only. It should not be
    included by the application. The configuration macros defined in the file
    should be defined in the configuration specific system_config.h.
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

#ifndef _USB_DEVICE_HID_CONFIG_TEMPLATE_H_
#define _USB_DEVICE_HID_CONFIG_TEMPLATE_H_

#error "This is configuration template file. Do not include it directly in the workspace."

// *****************************************************************************
// *****************************************************************************
// Section: USB Device HID Class Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Device HID Maximum Number of Instances

  Summary:
    Specifies the number of HID instances.

  Description:
    This macro defines the number of instances of the HID Function Driver. For
    example, if the application needs to implement two instances of the HID
    Function Driver (to create composite device) on one USB Device, the macro
    should be set to 2. Note that implementing a USB Device that features
    multiple HID interfaces requires appropriate USB configuration descriptors. 

  Remarks:
    None.

*/

#define USB_DEVICE_HID_INSTANCES_NUMBER       /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB device HID Combined Queue Size

  Summary:
    Specifies the combined queue size of all HID instances.

  Description:
    This macro defines the number of entries in all queues in all instances of
    the HID function driver. This value can be obtained by adding up the read
    and write queue sizes of each HID Function driver instance. In a simple
    single instance USB HID device application, that does not require buffer
    queuing and serial state notification, the
    USB_DEVICE_HID_QUEUE_DEPTH_COMBINED macro can be set to 2. Consider a case
    with two HID function driver instances, HID 1 has a read queue size of 2 and
    write queue size of 3, HID 2 has a read queue size of 4 and write queue size
    of 1, this macro should be set to 10 (2 +3 + 4 + 1). 

  Remarks:
    None.
*/
#define USB_DEVICE_HID_QUEUE_DEPTH_COMINED /*DOM-IGONORE-BEGIN*/ 2 /*DOM-IGNORE-END*/

#endif // #ifndef _USB_DEVICE_HID_CONFIG_TEMPLATE_H_

/*******************************************************************************
 End of File
*/

