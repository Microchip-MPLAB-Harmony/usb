/*******************************************************************************
  USB Host Generic Client Driver Local Header File Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_generic_local.h

  Summary:
    USB Host Generic Client Driver Local Header File

  Description:
    This file contains constants, definitions and prototypes which are private
    to the USB Host Generic Client Driver. These should not be accessed directly
    by the application.
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
#ifndef _USB_HOST_GENERIC_LOCAL_H_
#define _USB_HOST_GENERIC_LOCAL_H_

#include "usb/usb_host_client_driver.h"
/*******************************************************************
 * Definition of the Generic Host Driver object.
 *******************************************************************/

typedef struct
{
    USB_HOST_DEVICE_ASSIGN deviceAssign;
    USB_HOST_DEVICE_RELEASE deviceRelease;
    USB_HOST_INTERFACE_ASSIGN interfaceAssign;
    USB_HOST_INTERFACE_RELEASE interfaceRelease;
    USB_HOST_DEVICE_EVENT_HANDLER deviceEventHandler;
    USB_HOST_DEVICE_INTERFACE_EVENT_HANDLER interfaceEventHandler;

} USB_HOST_GENERIC_OBJ;

// *****************************************************************************
/* Function:
    void _USB_HOST_GENERIC_Initialize(void * data)

  Summary:
    This function is called when the Host Layer is initializing.

  Description:
    This function is called when the Host Layer is initializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_GENERIC_Initialize(void * data);

// *****************************************************************************
/* Function:
    void _USB_HOST_GENERIC_Deinitialize(void)

  Summary:
    This function is called when the Host Layer is deinitializing.

  Description:
    This function is called when the Host Layer is deinitializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_GENERIC_Deinitialize(void);

// *****************************************************************************
/* Function:
    void _USB_HOST_GENERIC_Reinitialize(void * data)

  Summary:
    This function is called when the Host Layer is reinitializing.

  Description:
    This function is called when the Host Layer is reinitializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_GENERIC_Reinitialize(void * init);

// *****************************************************************************
/* Function:
    void _USB_HOST_GENERIC_DeviceAssign 
    (
        USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
        USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
        USB_DEVICE_DESCRIPTOR * deviceDescriptor
    )
 
  Summary: 
    This function is called when the host layer finds device level class
    subclass protocol match or 

  Description:
    This function is called when the host layer finds device level class
    subclass protocol match.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_GENERIC_DeviceAssign 
(
    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    USB_DEVICE_DESCRIPTOR * deviceDescriptor
);

// *****************************************************************************
/* Function:
    void _USB_HOST_GENERIC_DeviceRelease 
    (
        USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle
    )
 
  Summary: 
    This function is called when the device is detached. 

  Description:
    This function is called when the device is detached. 

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_GENERIC_DeviceRelease
(
    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle
);

// *****************************************************************************
/* Function:
    void _USB_HOST_GENERIC_InterfaceAssign 
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE * interfaces,
        USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
        size_t nInterfaces,
        uint8_t * descriptor
    )

  Summary:
    This function is called when the Host Layer attaches this driver to an
    interface.

  Description:
    This function is called when the Host Layer attaches this driver to an
    interface.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_GENERIC_InterfaceAssign 
(
    USB_HOST_DEVICE_INTERFACE_HANDLE * interfaces,
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    size_t nInterfaces,
    uint8_t * descriptor
);

// *****************************************************************************
/* Function:
    void _USB_HOST_GENERIC_InterfaceRelease
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
    )

  Summary:
    This function is called when the Host Layer detaches this driver from an
    interface.

  Description:
    This function is called when the Host Layer detaches this driver from an
    interface.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_GENERIC_InterfaceRelease
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
);


// *****************************************************************************
/* Function:
    USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE _USB_HOST_GENERIC_InterfaceEventHandler
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle,
        USB_HOST_DEVICE_INTERFACE_EVENT event,
        void * eventData,
        uintptr_t context
    )

  Summary:
    This function is called when the Host Layer generates interface level
    events. 

  Description:
    This function is called when the Host Layer generates interface level
    events. 

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE _USB_HOST_GENERIC_InterfaceEventHandler
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle,
    USB_HOST_DEVICE_INTERFACE_EVENT event,
    void * eventData,
    uintptr_t context
);

// *****************************************************************************
/* Function:
    USB_HOST_DEVICE_EVENT_RESPONSE _USB_HOST_GENERIC_DeviceEventHandler
    (
        USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
        USB_HOST_DEVICE_EVENT event,
        void * eventData,
        uintptr_t context
    )

  Summary:
    This function is called when the Host Layer generates device level
    events. 

  Description:
    This function is called when the Host Layer generates device level
    events. 

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

USB_HOST_DEVICE_EVENT_RESPONSE _USB_HOST_GENERIC_DeviceEventHandler
(
    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
    USB_HOST_DEVICE_EVENT event,
    void * eventData,
    uintptr_t context
);

// *****************************************************************************
/* Function:
    void _USB_HOST_GENERIC_DeviceTasks 
    (
        USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle
    )
 
  Summary: 
    This function is called when the host layer want the device to update its
    state. 

  Description:
    This function is called when the host layer want the device to update its
    state. 

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_GENERIC_DeviceTasks
(
    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle
);

// *****************************************************************************
/* Function:
    void USB_HOST_GENERIC_InterfaceTasks
    (
        USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
    )

  Summary:
    This function is called by the Host Layer to update the state of this
    driver.

  Description:
    This function is called by the Host Layer to update the state of this
    driver.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _USB_HOST_GENERIC_InterfaceTasks
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
);

#endif


