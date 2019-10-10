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



