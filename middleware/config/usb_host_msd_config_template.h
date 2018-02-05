/*******************************************************************************
  USB Host MSD function driver compile time options

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_msd_config_template.h

  Summary:
    USB Host MSD configuration template header file

  Description:
    This file contains USB host MSD function driver compile time 
    options(macros) that has to be configured by the user. This file is a 
    template file and must be used as an example only. This file must not 
    be directly included in the project.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _USB_HOST_MSD_CONFIG_TEMPLATE_H_
#define _USB_HOST_MSD_CONFIG_TEMPLATE_H_

#error (" This is a template file and must not be included directly in the project" );

// *****************************************************************************
// *****************************************************************************
// Section: USB Host MSD Client Driver Configuration Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Host MSD Client Driver Instances Number.
  
  Summary:
    Defines the maximum number of MSD devices to be supported by this host
    application. 

  Description:
    This constant defines the maximum number of MSD devices to be supported by
    this host application. For example, if 3 USB Pen Drives need to be
    supported, then this value should be 3. This value cannot be greater than
    USB_HOST_DEVICES_NUMBER, which defines the maximum number of devices to be
    supported in the application. If this constant is less than
    USB_HOST_DEVICES_NUMBER, then only USB_HOST_MSD_INSTANCES_NUMBER of MSD
    devices will be enumerated.
    
  Remarks:
    None
*/

#define USB_HOST_MSD_INSTANCES_NUMBER  /*DOM-IGNORE-BEGIN*/1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Host MSD Client Driver LUNs Number.
  
  Summary:
    Defines the maximum number of MSD Device LUNs to be supported in the
    application.

  Description:
    An MSD device may have mutliple storage units, each addressable through a
    LUN number. An example is a USB Card reader with multiple card slots. Each
    card slot has a LUN number. The USB_HOST_MSD_LUNS_NUMBER constant defines
    the maximum number of such logical units that can be managed by the USB Host
    application. This number should atleast be equal to
    USB_HOST_MSD_INSTANCES_NUMBER. To configure this value, consider an example
    of an application that will support a maximum of 2 USB Storage device. These
    2 storage devices are expected to have at the most 3 LUNs each. Then the the
    USB_HOST_MSD_LUNS_NUMBER constant should be set to 6 (2 devices and 3 LUNs
    per device)/ 
    
  Remarks:
    None
*/

#define USB_HOST_MSD_LUNS_NUMBER  /*DOM-IGNORE-BEGIN*/1 /*DOM-IGNORE-END*/

#endif



