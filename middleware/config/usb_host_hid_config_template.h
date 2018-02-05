/*******************************************************************************
  USB Host HID Class Configuration Definitions 

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hid_config_template.h

  Summary:
    USB host HID Class configuration definitions template

  Description:
    This file contains configurations macros needed to configure the HID
    host Driver. This file is a template file only. It should not be
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

#ifndef _USB_HOST_HID_CONFIG_TEMPLATE_H_
#define _USB_HOST_HID_CONFIG_TEMPLATE_H_

#error "This is configuration template file. Do not include it directly."

// *****************************************************************************
// *****************************************************************************
// Section: USB Host HID Class Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB host HID Maximum Number of instances

  Summary:
    Specifies the number of HID instances.

  Description:
    This macro defines the number of instances of the HID host Driver. For
    example, if the application needs to implement two instances of the HID
    host Driver, value should be set to 2. 

  Remarks:
    None.
*/

#define USB_HOST_HID_INSTANCES_NUMBER       /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/


// *****************************************************************************
/* USB Host HID Maximum Number of INTERRUPT IN endpoints supported

  Summary:
    Specifies the maximum number of INTERRUPT IN endpoints supported.

  Description:
    This macro defines the number of INTERRUPT IN endpoints supported by
	USB Host HID driver. If the application needs to work with HID device
	which has 2 INTERRUPT IN endpoints, the value should be set to 2.

  Remarks:
    None.
*/

#define USB_HOST_HID_INTERRUPT_IN_ENDPOINTS_NUMBER       /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host HID number of Usage driver registered

  Summary:
    Specifies the number of Usage driver registered
  Description:
    This macro defines the number of Usage driver registered with USB Host
	Hid driver. If the application wants HID driver to support 2 HID device
	having different usage, then the value should be set to 2.

  Remarks:
    None.
*/

#define USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER      /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host HID Global PUSH POP stack size supported

  Summary:
    Specifies the Global PUSH POP stack size supported.

  Description:
    This macro defines the size of the Global PUSH POP stack per HID driver
	instance. If the application wants to support HID device
	having 2 continuous PUSH item or 2 continuous POP item in the Report Descriptor,
	then the value should be set to 2.

  Remarks:
    None.
*/

#define USB_HID_GLOBAL_PUSH_POP_STACK_SIZE       /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host HID number of Mouse buttons supported

  Summary:
    Specifies the number of Mouse buttons supported.

  Description:
    This macro defines the number of Mouse buttons supported. If the application
	wants to support HID Mouse device having 5 buttons, then the value should
	be set to 5.

  Remarks:
    None.
*/

#define USB_HOST_HID_MOUSE_BUTTONS_NUMBER       /*DOM-IGNORE-BEGIN*/ 5 /*DOM-IGNORE-END*/



#endif // #ifndef _USB_HOST_HID_CONFIG_TEMPLATE_H_

/*******************************************************************************
 End of File
*/

