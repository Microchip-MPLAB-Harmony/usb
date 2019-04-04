/*******************************************************************************
  USB Printer class definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_Printer.h

  Summary:
    USB Printer class definitions

  Description:
    This file describes the Printer class specific definitions. This file is
    included by usb_device_Printer.h and usb_host_Printer.h header files. The
    application can include this file if it needs to use any USB Printer Class
    definitions.
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
// DOM-IGNORE-END

#ifndef _USB_PRINTER_H
#define _USB_PRINTER_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Printer Interface Class Subclass and Protocol constants.

  Summary:
    Identifies the Printer Interface Class, Subclass and protocol constants.

  Description:
    These constants identify the Printer Interface Class, Subclass and protocol
    constants.

  Remarks:
    None.
*/

/* Base class for printers */
#define USB_PRINTER_INTERFACE_CLASS_CODE            0x07
/* Subclass codes for Printer devices */        
#define USB_PRINTER_INTERFACE_SUBCLASS_CODE         0x01
/* Printer Interface Type */       
#define USB_PRINTER_INTERFACE_PROTOCOL              0x01
        
#define USB_PRINTER_REQUEST_CLASS_SPECIFIC          0x21

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB Printer Command.

  Summary:
    Identified the USB Printer Commands used by the stack.

  Description:
    Identified the USB Printer Commands used by the stack.

  Remarks:
    None.
*/

typedef enum
{
    USB_PRINTER_GET_DEVICE_ID   = 0x0,
    USB_PRINTER_GET_PORT_STATUS = 0x1,
    USB_PRINTER_SOFT_RESET      = 0x2
} USB_PRINTER_COMMAND;

// *****************************************************************************

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif
