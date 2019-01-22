/*******************************************************************************
  USB Host Hub Configuration Definitions 

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hub_config_template.h

  Summary:
    USB host CDC Class configuration definitions template

  Description:
    This file contains configurations macros needed to configure the Hub Driver.
    This file is a template file only. It should not be included by the
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

#ifndef _USB_HOST_HUB_CONFIG_TEMPLATE_H_
#define _USB_HOST_HUB_CONFIG_TEMPLATE_H_

#error "This is configuration template file. Do not include it directly."

// *****************************************************************************
/* USB Host Hub Instances Number 
 
  Summary: 
    Specifies the number of Hub to be supported in the system.

  Description:
    This configuration constant defines the total number of hubs to be supported
    in the application. This includes hubs connected across multiple USBs. If
    the hub connected to the host exceed this number, then the additional hubs
    will not be enumerated.

  Remarks:
    Increasing number of Hubs to be supported will also increase memory
    consumption.
*/

#define USB_HOST_HUB_INSTANCES_NUMBER  /*DOM-IGNORE-BEGIN*/1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host Hub Number of Ports per Hub 
 
  Summary: 
    Specifies the number of ports per Hub.

  Description:
    This configuration macros specifies the number of Ports per Hub. If any Hub
    connected to host will a have a maximum of 4 ports, then this number should
    be set to 4. A hub with more ports than the value defined by this constant
    will not be supported.

  Remarks:
    Supporting a hub with more ports increases the memory requirement. 
*/

#define USB_HOST_HUB_PORTS_NUMBER  /*DOM-IGNORE-BEGIN*/4 /*DOM-IGNORE-END*/


#endif
