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



