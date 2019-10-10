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

