/*******************************************************************************
  USB Device MSD function driver compile time options

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_msd_config_template.h

  Summary:
    USB Device MSD configuration template header file

  Description:
    This file contains USB device MSD function driver compile time 
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

#ifndef _USB_DEVICE_MSD_CONFIG_TEMPLATE_H_
#define _USB_DEVICE_MSD_CONFIG_TEMPLATE_H_

#error (" This is a template file and must not be included directly in the project" );

// *****************************************************************************
// *****************************************************************************
// Section: USB Device configuration Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* MSD Function Driver Instances Number
 
  Summary:
    Number of MSD function Driver instances required in the USB Device.

  Description:
    This configuration constant defines the number of MSD Function Driver
    instances in the USB Device. This value should be atleast 1 if the MSD
    function is required. In case where multiple MSD function drivers are
    required, it should be noted the MSD function driver supports multiple LUNs.
    This allows one MSD function driver instance to manage multiple media. Using
    multiple LUNs can be considered as an alternative to using multiple MSD
    function driver instances.
    
  Remarks:
    None.
*/

#define USB_DEVICE_MSD_INSTANCES_NUMBER 1

// *****************************************************************************
/* Number of LUNs 
 
  Summary:
    Defines the number of LUNs per MSD function driver instance.

  Description:
    This constant sets maximum possible number of Logical Unit (LUN) an instance
    of MSD can support. This value should be atleast 1. In cases where multiple
    MSD Function Driver instances are required, this constant should be set to
    the maximum number of LUNs required by any MSD Function Driver instanceThe
    following figure shows a pictorial representation of the MSD function driver
    initialization data structure.
    
  Remarks:
    None.
*/

#define USB_DEVICE_MSD_LUNS_NUMBER  1

#endif



