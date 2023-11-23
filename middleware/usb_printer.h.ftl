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
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef USB_PRINTER_H
#define USB_PRINTER_H

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
/* Do not modify this value */
#define USB_PRINTER_INTERFACE_CLASS_CODE            0x07U
        
/* Subclass codes for Printer devices */        
/* Do not modify this value */
#define USB_PRINTER_INTERFACE_SUBCLASS_CODE         0x01U
        
/* Printer Interface Type */
/* 
 * 0x01 - Unidirectional interface
 * 0x02 - Bi-directional interface
 * 0x03 - 1284.4 compatible bi-directional interface
 */
#define USB_PRINTER_INTERFACE_PROTOCOL              0x01

/* bmRequestType for Printer class specific request */
#define USB_PRINTER_REQUEST_CLASS_SPECIFIC          0x21U

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
    /* class-specific request returns a device ID string that is compatible with
     * IEEE 1284 */
    USB_PRINTER_GET_DEVICE_ID   = 0x0,
    /* class-specific request returns the printer?s current status */
    USB_PRINTER_GET_PORT_STATUS = 0x1,
    /* class-specific request flushes all buffers and resets the Bulk endpoints 
     * to their default states */
    USB_PRINTER_SOFT_RESET      = 0x2
} USB_PRINTER_COMMAND;

// *****************************************************************************
/* USB Printer Port Status.

  Summary:
    Printer status required by class-specific request.

  Description:
    This structure defines the Printer status.

  Remarks:
    None.
 */
typedef struct 
{
    /* 1 = No Error, 0 = Error */
    uint8_t errorStatus;
    
    /* 1 = Selected, 0 = Not Selected */
    uint8_t selectStatus;
    
    /* 1 = Paper Empty, 0 = Paper Not Empty */
    uint8_t paperEmptyStatus;

} USB_PRINTER_PORT_STATUS;

// *****************************************************************************

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif
