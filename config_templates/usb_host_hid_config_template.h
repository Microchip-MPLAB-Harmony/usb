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

