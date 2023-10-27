/********************************************************************************
  USB Host Hub Client Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hub.h

  Summary:
    USB Host Hub Client Driver Interface Header

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the USB HOST Hub
    Client Driver.
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
#ifndef USB_HOST_HUB_H_
#define USB_HOST_HUB_H_
//DOM-IGNORE-END

#include "usb_hub.h"
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// ****************************************************************************
// ****************************************************************************
// Section: Data Types and Constants
// ****************************************************************************
// ****************************************************************************

// ****************************************************************************
/* USB Hub Host Client Driver Interface Pointer
 
  Summary: 
    USB Hub Host Client Driver Interface Pointer.

  Description:
    This constant is a pointer to a table of function pointers that define the
    interface between the Hub Host Client Driver and the USB Host Layer. This
    constant should be used while adding support for the Hub Driver in TPL
    table.

  Remarks:
    None.
*/

/*DOM-IGNORE-BEGIN*/extern USB_HOST_CLIENT_DRIVER gUSBHostHUBClientDriver; /*DOM-IGNORE-END*/
#define USB_HOST_HUB_INTERFACE  /*DOM-IGNORE-BEGIN*/&gUSBHostHUBClientDriver /*DOM-IGNORE-END*/

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

// ****************************************************************************
// ****************************************************************************
// Section: Hub Host Client Driver Interface
// ****************************************************************************
// ****************************************************************************

/* The Hub Host Client Driver does not contain any application callable API. */

#endif
