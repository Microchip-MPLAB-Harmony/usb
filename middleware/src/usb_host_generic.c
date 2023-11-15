/*******************************************************************************
  USB Host Generic Driver Framework Implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_generic.c

  Summary:
    This file contains implementations of both private and public functions
    of the USB Host Generic driver framework

  Description:
    This file contains implementations of both private and public functions
    of the USB Host Generic driver framework. This file should be
    included in the project if USB Host functionality is desired.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "usb/src/usb_external_dependencies.h"
#include "usb/usb_common.h"
#include "usb/usb_chapter_9.h"
#include "usb/usb_host_generic.h"
#include "usb/usb_host_client_driver.h"
#include "usb_host_generic_local.h"

/************************************************
 * USB Host Generic Driver Object
 ************************************************/
static USB_HOST_GENERIC_OBJ gUSBHostGenericDriverObj;

/************************************************
 * Generic Driver Interface to the host layer
 ************************************************/
USB_HOST_CLIENT_DRIVER gUSBHostGenericClientDriver =
{
    .initialize = F_USB_HOST_GENERIC_Initialize,
    .deinitialize = F_USB_HOST_GENERIC_Deinitialize,
    .reinitialize = F_USB_HOST_GENERIC_Reinitialize,
    .interfaceAssign = F_USB_HOST_GENERIC_InterfaceAssign,
    .interfaceRelease = F_USB_HOST_GENERIC_InterfaceRelease,
    .interfaceEventHandler = F_USB_HOST_GENERIC_InterfaceEventHandler,
    .interfaceTasks = F_USB_HOST_GENERIC_InterfaceTasks,
    .deviceEventHandler = F_USB_HOST_GENERIC_DeviceEventHandler,
    .deviceAssign = F_USB_HOST_GENERIC_DeviceAssign,
    .deviceRelease = F_USB_HOST_GENERIC_DeviceRelease,
    .deviceTasks = F_USB_HOST_GENERIC_DeviceTasks
};

// *****************************************************************************
// *****************************************************************************
// USB Host Generic Client Driver Local function
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void F_USB_HOST_GENERIC_Initialize(void * data)

  Summary:
    This function is called when the Host Layer is initializing.

  Description:
    This function is called when the Host Layer is initializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/


void F_USB_HOST_GENERIC_Initialize(void * data)
{
    /* The host layer will call this function when the USB_HOST_Initialize() 
     * funtion is called. We will let the application decide how the Generic
     * driver should be initialized. Hence we will leave this function as 
     * emtpy. */
}

// *****************************************************************************
/* Function:
    void F_USB_HOST_GENERIC_Deinitialize(void)

  Summary:
    This function is called when the Host Layer is deinitializing.

  Description:
    This function is called when the Host Layer is deinitializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/


void F_USB_HOST_GENERIC_Deinitialize(void)
{
    /* This host layer will call this funciton is the host laye itself is
     * being deinitialized. But this is an unlikely scenario. Additionally
     * we would let the application itself handle this in the application 
     * deinitialize. */
}

// *****************************************************************************
/* Function:
    void F_USB_HOST_GENERIC_Reinitialize(void * data)

  Summary:
    This function is called when the Host Layer is reinitializing.

  Description:
    This function is called when the Host Layer is reinitializing.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/


void F_USB_HOST_GENERIC_Reinitialize(void * init)
{
    /* This host layer will call this funciton is the host laye itself is
     * being reinitialized. But this is an unlikely scenario. Additionally
     * we would let the application itself handle this in the application 
     * reinitialize. */
}

// *****************************************************************************
/* Function:
    void F_USB_HOST_GENERIC_DeviceAssign 
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

void F_USB_HOST_GENERIC_DeviceAssign 
(
    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    USB_DEVICE_DESCRIPTOR * deviceDescriptor
)
{
    /* We will call the application provided device assign function here. */
    if(gUSBHostGenericDriverObj.deviceAssign != NULL)
    {
        gUSBHostGenericDriverObj.deviceAssign(deviceHandle, deviceObjHandle, deviceDescriptor);
    }
    else
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Host Generic Driver: Device Assign Function pointer is NULL");
    }
}

// *****************************************************************************
/* Function:
    void F_USB_HOST_GENERIC_DeviceRelease 
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

void F_USB_HOST_GENERIC_DeviceRelease
(
    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle
)
{
    /* We will call the application provided device release function here. */
    if(gUSBHostGenericDriverObj.deviceRelease != NULL)
    {
        gUSBHostGenericDriverObj.deviceRelease(deviceHandle);
    }
    else
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Host Generic Driver: Device Release Function pointer is NULL");
    }
}

// *****************************************************************************
/* Function:
    void F_USB_HOST_GENERIC_InterfaceAssign 
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

void F_USB_HOST_GENERIC_InterfaceAssign 
(
    USB_HOST_DEVICE_INTERFACE_HANDLE * interfaces,
    USB_HOST_DEVICE_OBJ_HANDLE deviceObjHandle,
    size_t nInterfaces,
    uint8_t * descriptor
)
{
    /* We will call the application provided interface assign function here */
    if(gUSBHostGenericDriverObj.interfaceAssign != NULL)
    {
        gUSBHostGenericDriverObj.interfaceAssign(interfaces, deviceObjHandle, nInterfaces, descriptor);
    }
    else
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Host Generic Driver: Interface Assign Function pointer is NULL");
    }
}

// *****************************************************************************
/* Function:
    void F_USB_HOST_GENERIC_InterfaceRelease
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

void F_USB_HOST_GENERIC_InterfaceRelease
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)
{
    /* We will call the application provided interface release function here */
    if(gUSBHostGenericDriverObj.interfaceRelease != NULL)
    {
        gUSBHostGenericDriverObj.interfaceRelease(interfaceHandle);
    }
    else
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nUSB Host Generic Driver: Interface Release Function pointer is NULL");
    }
}

// *****************************************************************************
/* Function:
    USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE F_USB_HOST_GENERIC_InterfaceEventHandler
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

USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE F_USB_HOST_GENERIC_InterfaceEventHandler
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle,
    USB_HOST_DEVICE_INTERFACE_EVENT event,
    void * eventData,
    uintptr_t context
)
{
    USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE returnVal = USB_HOST_DEVICE_INTERFACE_EVENT_RESPONSE_NONE;
    
    /* We will pass the on the event directly to the application registered 
     * interface event handler. */
    if(gUSBHostGenericDriverObj.interfaceEventHandler != NULL)
    {
        returnVal = gUSBHostGenericDriverObj.interfaceEventHandler(interfaceHandle, event, eventData, context);
    }
    
    return(returnVal);
}

// *****************************************************************************
/* Function:
    USB_HOST_DEVICE_EVENT_RESPONSE F_USB_HOST_GENERIC_DeviceEventHandler
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

USB_HOST_DEVICE_EVENT_RESPONSE F_USB_HOST_GENERIC_DeviceEventHandler
(
    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle,
    USB_HOST_DEVICE_EVENT event,
    void * eventData,
    uintptr_t context
)
{
    USB_HOST_DEVICE_EVENT_RESPONSE returnVal = USB_HOST_DEVICE_EVENT_RESPONSE_NONE;
    
    /* We will pass on the event directly to the application registered 
     * device event handler. */
    if(gUSBHostGenericDriverObj.deviceEventHandler != NULL)
    {
        returnVal = gUSBHostGenericDriverObj.deviceEventHandler(deviceHandle, event, eventData, context);
    }
    
    return(returnVal);
}

// *****************************************************************************
/* Function:
    void F_USB_HOST_GENERIC_DeviceTasks 
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

void F_USB_HOST_GENERIC_DeviceTasks
(
    USB_HOST_DEVICE_CLIENT_HANDLE deviceHandle
)
{
    /* The device tasks will be implemented by the application. This function is therefore
     * empty */
}

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

void F_USB_HOST_GENERIC_InterfaceTasks
(
    USB_HOST_DEVICE_INTERFACE_HANDLE interfaceHandle
)
{
}

// *****************************************************************************
// *****************************************************************************
// USB Host Generic Client Driver Public function
// *****************************************************************************
// *****************************************************************************

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
)
{
    gUSBHostGenericDriverObj.deviceAssign = deviceAssign;
    gUSBHostGenericDriverObj.deviceRelease = deviceRelease;
    gUSBHostGenericDriverObj.interfaceAssign = interfaceAssign;
    gUSBHostGenericDriverObj.interfaceRelease = interfaceRelease;
    gUSBHostGenericDriverObj.deviceEventHandler = deviceEventHandler;
    gUSBHostGenericDriverObj.interfaceEventHandler = interfaceEventHandler;
}