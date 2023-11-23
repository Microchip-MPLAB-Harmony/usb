/*******************************************************************************
  USB Host Generic Client Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_generic.h

  Summary:
    USB Host Generic Client Driver Interface Header

  Description:
    This header file contains the function prototypes and definitions of the
    data types and constants that make up the interface to the USB Host Generic
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
#ifndef USB_HOST_GENERIC_H_
#define USB_HOST_GENERIC_H_

#include "usb/usb_host_client_driver.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

// *****************************************************************************
/* USB HOST Generic Client Driver Interface
 
  Summary:
    USB HOST Generic Client Driver Interface

  Description:
    This macro should be used by the application in TPL table while adding
    support for the USB Generic Host Client Driver.

  Remarks:
    None.
*/

/*DOM-IGNORE-BEGIN*/extern USB_HOST_CLIENT_DRIVER gUSBHostGenericClientDriver; /*DOM-IGNORE-END*/
#define USB_HOST_GENERIC_INTERFACE  /*DOM-IGNORE-BEGIN*/&gUSBHostGenericClientDriver /*DOM-IGNORE-END*/

// *****************************************************************************
/* Function:
    void USB_HOST_GENERIC_Register
    (
        USB_HOST_DEVICE_ASSIGN deviceAssign,
        USB_HOST_DEVICE_RELEASE deviceRelease,
        USB_HOST_INTERFACE_ASSIGN interfaceAssign,
        USB_HOST_INTERFACE_RELEASE interfaceRelease,
        USB_HOST_DEVICE_EVENT_HANDLER deviceEventHandler,
        USB_HOST_DEVICE_INTERFACE_EVENT_HANDLER interfaceEventHandler
    );

  Summary:
    Register required functions with the USB Host Generic driver.

  Description:
    This function registers the required function with the USB Host Generic
    driver. The USB Host Layer invokes these functions during the lifetime and
    the operation of the attached device. 
    
  Precondition:
    The TPL Table should contain an entry that directs the USB Host to assign
    the device to the Generic Driver.

  Parameters:
    deviceAssign - A pointer to the Device Assign Function.
    deviceRelease - A pointer to the Device Release Function.
    interfaceAssign - A pointer to the Interface Assign Function.
    interfaceRelease - A pointer to the Interaface Release Function.
    deviceEventHandler - A pointer to the Device Event Handling Function.
    interfaceEventHandler - A pointer to the Interface Event Handling Function.

  Returns:
    None.

  Example:
  <code>
  </code>

  Remarks:
    Setting any of the parameters to NULL will affect the application's
    capability to successfully operate the device. The functions should be
    registered before the bus is enabled.
*/

void USB_HOST_GENERIC_Register
(
    USB_HOST_DEVICE_ASSIGN deviceAssign,
    USB_HOST_DEVICE_RELEASE deviceRelease,
    USB_HOST_INTERFACE_ASSIGN interfaceAssign,
    USB_HOST_INTERFACE_RELEASE interfaceRelease,
    USB_HOST_DEVICE_EVENT_HANDLER deviceEventHandler,
    USB_HOST_DEVICE_INTERFACE_EVENT_HANDLER interfaceEventHandler
);




#ifdef __cplusplus
}
#endif
#endif

