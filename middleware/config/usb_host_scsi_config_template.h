/*******************************************************************************
  USB Host SCSI Block Device Driver Compile time options

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_scsi_config_template.h

  Summary:
    USB Host SCSI Block Device Driver Compile time options

  Description:
    This file contains USB Host SCSI Block Device Driver compile time
    options(macros) that has to be configured by the user. This file is a
    template file and must be used as an example only. This file must not be
    directly included in the project.
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

#ifndef _USB_HOST_SCSI_CONFIG_TEMPLATE_H_
#define _USB_HOST_SCSI_CONFIG_TEMPLATE_H_

#error (" This is a template file and must not be included directly in the project" );

// *****************************************************************************
// *****************************************************************************
// Section: USB Host SCSI Block Driver Configuration Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Host SCSI Block Driver Client Numbers
  
  Summary:
    Defines the maximum number of SCSI Block Driver clients in the application.

  Description:
    This constant defines the maximum SCSI Block Driver clients in the USB Host
    application. The SCSI Block Driver is initialized by the MSD Host Client
    Driver when a mass storage device is attached. Any application that needs to
    access this mass storage device must open the associated SCSI block driver
    instance. An example of such application is the MPLAB Harmony File System.
    The value of the USB_HOST_SCSI_CLIENTS_NUMBER should atleast equal to
    USB_HOST_MSD_LUNS_NUMBER.

    If multiple application entities will need to access the storage media, then
    this number should be match the maximum the number of driver will be opened.
    For example, if the application supports 2 USB pen drives, then the value of
    this constant should be 2 (for the file system to access two drives). If an
    additional application will open the SCSI device, then this value should be
    3.
    
  Remarks:
    None
*/

#define USB_HOST_SCSI_CLIENTS_NUMBER  /*DOM-IGNORE-BEGIN*/1 /*DOM-IGNORE-END*/

#endif



