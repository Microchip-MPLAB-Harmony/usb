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
