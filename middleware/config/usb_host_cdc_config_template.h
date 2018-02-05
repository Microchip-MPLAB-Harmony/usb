/*******************************************************************************
  USB Host CDC Class Configuration Definitions 

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_cdc_config_template.h

  Summary:
    USB host CDC Class configuration definitions template

  Description:
    This file contains configurations macros needed to configure the CDC host
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

#ifndef _USB_HOST_CDC_CONFIG_TEMPLATE_H_
#define _USB_HOST_CDC_CONFIG_TEMPLATE_H_

#error "This is configuration template file. Do not include it directly."

// *****************************************************************************
// *****************************************************************************
// Section: USB Host CDC Class Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Host CDC Maximum Number of Instances

  Summary:
    Specifies the number of CDC instances.

  Description:
    This macro defines the number of instances of the CDC host Driver. For
    example, if the application needs to implement two instances of the CDC
    host Driver should be set to 2. 

  Remarks:
    None.
*/

#define USB_HOST_CDC_INSTANCES_NUMBER /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host CDC Attach Listeners Number 
 
  Summary: 
    Defines the number of attach event listeners that can be registered with CDC
    Host Client Driver. 

  Description:
    The USB CDC Host Client Driver provides attach notification to listeners who
    have registered with the client driver via the
    USB_HOST_CDC_AttachEventHandlerSet() function. The
    USB_HOST_CDC_ATTACH_LISTENERS_NUMBER configuration constant defines the
    maximum number of event handlers that can be set. This number should be set
    to equal the number of entities that interested in knowing when a CDC device
    is attached.

  Remarks:
    None.
*/

#define USB_HOST_CDC_ATTACH_LISTENERS_NUMBER  /*DOM-IGNORE-BEGIN*/1 /*DOM-IGNORE-END*/

#endif // #ifndef _USB_HOST_CDC_CONFIG_TEMPLATE_H_

/*******************************************************************************
 End of File
*/

