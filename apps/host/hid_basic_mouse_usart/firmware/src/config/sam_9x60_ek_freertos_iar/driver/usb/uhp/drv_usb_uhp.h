/******************************************************************************
  USB Module Driver Interface Header File

  Company:
    Microchip Technology Inc.
    
  File Name:
    drv_usb_uhp.h
	
  Summary:
    USB Host Port Module Driver Interface File
	
  Description:
    The USB Host port Module driver provides a simple interface to manage
    the "USB" peripheral on the  microcontroller. This file defines the
    interface definitions and prototypes for the Hi-Speed USB Driver. The driver
    interface meets the requirements of the MPLAB Harmony USB Host and Device
    Layer.                                                  
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
//DOM-IGNORE-END

#ifndef _DRV_UHP_H
#define _DRV_UHP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files (continued at end of file)
// *****************************************************************************
// *****************************************************************************
/*  This section lists the other files that are included in this file.  Also,
    see the bottom of the file for additional implementation header files that
    are also included
*/

#include <stdint.h>
#include <stdbool.h>
#include "usb/usb_common.h"
#include "usb/usb_chapter_9.h"
#include "driver/driver_common.h"
#include "system/int/sys_int.h"
#include "driver/usb/drv_usb.h"
#include "system/system_module.h"
#include "usb/usb_hub.h"
#include "driver/usb/uhp/src/drv_usb_uhp_ohci_registers.h"
#include "driver/usb/uhp/src/drv_usb_uhp_variant_mapping.h"

// *****************************************************************************
// *****************************************************************************
// Section: Hi-Speed USB Driver Constants
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Hi-Speed USB Driver Host Mode Interface Functions.

  Summary:
    Hi-Speed USB Driver Host Mode Interface Functions.

  Description:
    The Host Controller Driver Interface member of the Host Layer Initialization
    data structure should be set to this value so that the Host Layer can access
    the Hi-Speed USB Driver Host Mode functions.

  Remarks:
    None.
*/

/*DOM-IGNORE-BEGIN*/extern DRV_USB_HOST_INTERFACE gDrvUSBUHPHostInterface;/*DOM-IGNORE-END */
#define DRV_USB_UHP_HOST_INTERFACE /*DOM-IGNORE-BEGIN*/&gDrvUSBUHPHostInterface/*DOM-IGNORE-END */

// *****************************************************************************
/* Hi-Speed USB Driver Module Index 0 Definition.

  Summary:
    Hi-Speed USB Driver Module Index 0 Definition.

  Description:
    This constant defines the value of Hi-Speed USB Driver Index 0.  The
    SYS_MODULE_INDEX parameter of the DRV_USB_UHP_Initialize and DRV_USB_UHP_Open
    functions should be set to this value to identify instance 0 of the driver. 

  Remarks:
    These constants should be used in place of hard-coded numeric literals
    and should be passed into the DRV_USB_UHP_Initialize and DRV_USB_UHP_Open
    functions to identify the driver instance in use. These are not
    indicative of the number of modules that are actually supported by the
    microcontroller.
*/

#define DRV_USB_UHP_INDEX_0         0

// *****************************************************************************
// *****************************************************************************
// Section: USB Device Driver Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Hi-Speed USB Driver Host Pipe Handle.

  Summary:
    Defines the Hi-Speed USB Driver Host Pipe Handle type.

  Description:
    This type definition defines the type of the Hi-Speed USB Driver Host Pipe
    Handle.

  Remarks:
    None.
*/

typedef uintptr_t DRV_USB_UHP_HOST_PIPE_HANDLE;

// *****************************************************************************
/* Hi-Speed USB Driver Invalid Host Pipe Handle.

  Summary:
    Value of an Invalid Host Pipe Handle.

  Description:
    This constant defines the value of an Invalid Host Pipe Handle.

  Remarks:
    None.
*/

#define DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID /*DOM-IGNORE-BEGIN*/((DRV_USB_UHP_HOST_PIPE_HANDLE)(-1))/*DOM-IGNORE-END*/

// *****************************************************************************
/* Hi-Speed USB Driver Events Enumeration.

  Summary:
    Identifies the different events that the Hi-Speed USB Driver provides.

  Description:
    This enumeration identifies the different events that are generated by the
    Hi-Speed USB Driver.

  Remarks:
    None.
*/

typedef enum
{
    /* Bus error occurred and was reported */
    DRV_USB_UHP_EVENT_ERROR = DRV_USB_EVENT_ERROR,

    /* Host has issued a device reset */
    DRV_USB_UHP_EVENT_RESET_DETECT = DRV_USB_EVENT_RESET_DETECT,

    /* Resume detected while USB in suspend mode */
    DRV_USB_UHP_EVENT_RESUME_DETECT = DRV_USB_EVENT_RESUME_DETECT,

    /* Idle detected */
    DRV_USB_UHP_EVENT_IDLE_DETECT = DRV_USB_EVENT_IDLE_DETECT,

    /* Stall handshake has occurred */
    DRV_USB_UHP_EVENT_STALL = DRV_USB_EVENT_STALL,

    /* Device received SOF operation */
    DRV_USB_UHP_EVENT_SOF_DETECT = DRV_USB_EVENT_SOF_DETECT,

    /* VBUS voltage had Session valid */
    DRV_USB_UHP_EVENT_DEVICE_SESSION_VALID = DRV_USB_EVENT_DEVICE_SESSION_VALID,

    /* Session Invalid */
    DRV_USB_UHP_EVENT_DEVICE_SESSION_INVALID = DRV_USB_EVENT_DEVICE_SESSION_INVALID,

} DRV_USB_UHP_EVENT;


// *****************************************************************************
/* Type of the Hi-Speed USB Driver Event Callback Function.

  Summary:
    Type of the Hi-Speed USB Driver event callback function.

  Description:
    Define the type of the Hi-Speed USB Driver event callback function. The
    client should register an event callback function of this type when it
    intends to receive events from the Hi-Speed USB Driver. The event callback
    function is registered using the DRV_USB_UHP_ClientEventCallBackSet function. 

  Parameters:
    hClient    - Handle to the driver client that registered this callback function.
    eventType  - This parameter identifies the event that caused the callback
                 function to be called.
    eventData  - Pointer to a data structure that is related to this event. 
                 This value will be NULL if the event has no related data.

  Returns:
    None.

  Remarks:
    None.

*/

typedef void (*DRV_USB_UHP_EVENT_CALLBACK) 
(
    uintptr_t hClient, 
    DRV_USB_UHP_EVENT  eventType,
    void * eventData   
);

// *****************************************************************************
/* USB Root hub Application Hooks (Port Overcurrent detection).

  Summary:
     USB Root hub Application Hooks (Port Overcurrent detection).

  Description:
    A function of the type defined here should be provided to the driver root
    hub to check for port over current condition.  This function will be called
    periodically by the root hub driver to check the Overcurrent status of the
    port. It should continue to return true while the Overcurrent condition
    exists on the port. It should return false when the Overcurrent condition
    does not exist. 

  Remarks:
    None.
*/

typedef bool (* DRV_USB_UHP_ROOT_HUB_PORT_OVER_CURRENT_DETECT)(uint8_t port);

// *****************************************************************************
/* USB Root hub Application Hooks (Port Power Enable/ Disable).

  Summary:
     USB Root hub Application Hooks (Port Power Enable/ Disable).

  Description:
    A function of the type defined here should be provided to the driver root to
    control port power.  The root hub driver will call this function when it
    needs to enable port power. If the application circuit contains a VBUS
    switch, the switch should be accessed and controlled by this function. If
    the enable parameter is true, the switch should be enabled and VBUS should
    be available on the port. If the enable parameter is false, the
    switch should be disabled and VBUS should not be available on the port.  

  Remarks:
    None.
*/

typedef void (* DRV_USB_UHP_ROOT_HUB_PORT_POWER_ENABLE)
(
    uint8_t port, 
    bool control
);

// *****************************************************************************
/* USB Root hub Application Hooks (Port Indication).

  Summary:
     USB Root hub Application Hooks (Port Indication).

  Description:
    A function of the type defined here should be provided to the driver root to
    implement Port Indication.  The root hub driver calls this function when it
    needs to update the state of the port indication LEDs. The application can
    choose to implement the Amber and Green colors as one LED or two different
    LEDs.  The root hub driver specifies the color and the indicator attribute
    (on, off or blinking) when it calls this function.

  Remarks:
    None.
*/

typedef void(* DRV_USB_UHP_ROOT_HUB_PORT_INDICATION)
(
    uint8_t port, 
    USB_HUB_PORT_INDICATOR_COLOR color, 
    USB_HUB_PORT_INDICATOR_STATE state
);

// *****************************************************************************
/* USB Device Driver Initialization Data.

  Summary:
    This type definition defines the Driver Initialization Data Structure.

  Description:
    This structure contains all the data necessary to initialize the Hi-Speed
    USB Driver.  A pointer to a structure of this type, containing the desired
    initialization data, must be passed into the DRV_USB_UHP_Initialize function.

  Remarks:
    None.
*/

typedef struct
{    
    /* Identifies the USB peripheral to be used. */   
    uhphs_registers_t * usbIDEHCI;
    UhpOhci * usbIDOHCI;

    /* Specify the interrupt source for the USB module. This should be the 
       interrupt source for the USB module instance specified in usbID. */
    INT_SOURCE interruptSource;

    /* Specify the operational speed of the USB module. This should always be
       set to USB_SPEED_FULL. */
    USB_SPEED operationSpeed; 

    /* Root hub available current in milliamperes. This specifies the amount of current
       that root hub can provide to the attached device. This should be
       specified in mA. This is required when the driver is required to operate
       in Host mode. */
    uint32_t rootHubAvailableCurrent;

    /* When operating in Host mode, the application can specify a Root hub port
       enable function. This parameter should point to Root hub port enable
       function. If this parameter is NULL, it implies that the port is always
       enabled. */
    DRV_USB_UHP_ROOT_HUB_PORT_POWER_ENABLE portPowerEnable;

    /* When operating in Host mode, the application can specify a Root Port
       Indication. This parameter should point to the Root Port Indication
       function. If this parameter is NULL, it implies that Root Port Indication
       is not supported. */
    DRV_USB_UHP_ROOT_HUB_PORT_INDICATION portIndication;

    /* When operating is Host mode, the application can specify a Root Port
       Overcurrent detection. This parameter should point to the Root Port
       Indication function. If this parameter is NULL, it implies that
       Overcurrent detection is not supported. */
    DRV_USB_UHP_ROOT_HUB_PORT_OVER_CURRENT_DETECT portOverCurrentDetect;

} DRV_USB_UHP_INIT;

// ****************************************************************************
/* Function:
    void DRV_USB_UHP_EndpointToggleClear
    (
        DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
    )

  Summary:
    Facilitates in resetting of endpoint data toggle to 0 for Non Control
    endpoints.

  Description:
    Facilitates in resetting of endpoint data toggle to 0 for Non Control
    endpoints.
	
  Precondition:
    None.

  Parameters:
    handle - Handle to the driver.
    pipeHandle - Handle to the pipe that connected to the device endpoint
    whose toggle needs to be cleared.

  Example:
    <code>

    // This code shows how the USB Host Layer calls the
    // DRV_USB_UHP_EndpointToggleClear function.

    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle;

    DRV_USB_UHP_EndpointToggleClear(pipeHandle);

    </code>

  Remarks:
    None.
*/

void DRV_USB_UHP_EndpointToggleClear
(
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
);

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - System Level
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_USB_UHP_Initialize
    ( 
        const SYS_MODULE_INDEX drvIndex,
        const SYS_MODULE_INIT * const init    
    )
    
  Summary:
    Initializes the Hi-Speed USB Driver.
	
  Description:
    This function initializes the Hi-Speed USB Driver, making it ready for
    clients to open. The driver initialization does not complete when this
    function returns. The DRV_USB_UHP_Tasks function must called periodically to
    complete the driver initialization. The DRV_USB_UHP_Open function will fail if
    the driver was not initialized or if initialization has not completed.
	
  Precondition:
    None.
	
  Parameters:
    drvIndex - Ordinal number of driver instance to be initialized. This should
    be set to DRV_USB_UHP_INDEX_0 if driver instance 0 needs to be initialized.

    init - Pointer to a data structure containing data necessary to
    initialize the driver. This should be a DRV_USB_UHP_INIT structure reference
    typecast to SYS_MODULE_INIT reference. 
				
  Returns:
    * SYS_MODULE_OBJ_INVALID - The driver initialization failed.
    * A valid System Module Object - The driver initialization was able to
      start. It may have not completed and requires the DRV_USB_UHP_Tasks function
      to be called periodically. This value will never be the same as
      SYS_MODULE_OBJ_INVALID. 
	
  Example:
    <code>
     // The following code shows an example initialization of the
     // driver. The USB module to be used is USB1.  The module should not
     // automatically suspend when the  microcontroller enters Sleep mode.  The
     // module should continue  operation when the module enters Idle mode.  The
     // power state is set to run at full clock speeds. Device Mode operation
     // should be at FULL speed. The size of the endpoint table is set for two
     // endpoints.
    
    DRV_USB_UHP_INIT moduleInit;

    usbInitData.usbID               = USB_UHP_ID_0;
    usbInitData.opMode              = DRV_USB_UHP_OPMODE_DEVICE;
    usbInitData.stopInIdle          = false;
    usbInitData.suspendInSleep      = false;
    usbInitData.operationSpeed      = USB_SPEED_FULL;
    usbInitData.interruptSource     = INT_SOURCE_USB;
    
    usbInitData.sysModuleInit.powerState = SYS_MODULE_POWER_RUN_FULL ;
    
    // This is how this data structure is passed to the initialize
    // function.
    
    DRV_USB_UHP_Initialize(DRV_USB_UHP_INDEX_0, (SYS_MODULE_INIT *) &usbInitData);
    
    </code>
	
  Remarks:
    This function must be called before any other Hi-Speed USB Driver function
    is called. This function should only be called once during system
    initialization unless DRV_USB_UHP_Deinitialize is called to deinitialize the
    driver instance.                                    
*/

extern SYS_MODULE_OBJ DRV_USB_UHP_Initialize 
(
    const SYS_MODULE_INDEX drvIndex,
    const SYS_MODULE_INIT * const init
);

/* Function:
    void DRV_USB_UHP_Deinitialize( const SYS_MODULE_OBJ object )

   Summary:
    Dynamic implementation of DRV_USB_UHP_Deinitialize system interface function.

   Description:
    This is the dynamic implementation of DRV_USB_UHP_Deinitialize
    system interface function.
 */
extern void DRV_USB_UHP_Deinitialize(const SYS_MODULE_INDEX object);

// *****************************************************************************
/* Function:
    SYS_STATUS DRV_USB_UHP_Status ( SYS_MODULE_OBJ object )

  Summary:
    Provides the current status of the Hi-Speed USB Driver module.

  Description:
    This function provides the current status of the Hi-Speed USB Driver module.

  Precondition:
    The DRV_USB_UHP_Initialize function must have been called before calling this
    function.

  Parameters:
    object - Driver object handle, returned from the DRV_USB_UHP_Initialize function.

  Returns:
    * SYS_STATUS_READY - Indicates that the driver is ready.
    * SYS_STATUS_UNINITIALIZED - Indicates that the driver has never been
      initialized.

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_USB_UHP_Initialize
    SYS_STATUS          status;
    DRV_USB_UHP_INIT moduleInit;
    
    usbInitData.usbID               = USB_UHP_ID_0;
    usbInitData.opMode              = DRV_USB_UHP_OPMODE_DEVICE;
    usbInitData.stopInIdle          = false;
    usbInitData.suspendInSleep      = false;
    usbInitData.operationSpeed      = USB_SPEED_FULL;
    usbInitData.interruptSource     = INT_SOURCE_USB;
    
    usbInitData.sysModuleInit.powerState = SYS_MODULE_POWER_RUN_FULL ;
    
    // This is how this data structure is passed to the initialize
    // function.
    
    DRV_USB_UHP_Initialize(DRV_USB_UHP_INDEX_0, (SYS_MODULE_INIT *) &usbInitData);
    
    // The status of the driver can be checked.
    status = DRV_USB_UHP_Status(object);
    if(SYS_STATUS_READY == status)
    {
        // Driver is ready to be opened.
    }

    </code>

  Remarks:
    None.
*/

extern SYS_STATUS DRV_USB_UHP_Status ( SYS_MODULE_OBJ object );

// *****************************************************************************
/* Function:
    void DRV_USB_UHP_Tasks( SYS_MODULE_OBJ object )

  Summary:
    Maintains the driver's state machine when the driver is configured for 
    Polled mode.

  Description:
    Maintains the driver's Polled state machine. This function should be called
    from the SYS_Tasks function.

  Precondition:
    The DRV_USB_UHP_Initialize function must have been called for the specified
    Hi-Speed USB Driver instance.

  Parameters:
    object - Object handle for the specified driver instance (returned from
    DRV_USB_UHP_Initialize function).

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_USB_UHP_Initialize

    while (true)
    {
        DRV_USB_UHP_Tasks(object);

        // Do other tasks
    }
    </code>

  Remarks:
    This function is normally not called  directly  by  an  application.   It  is
    called by the system's Tasks function (SYS_Tasks). This function will never
    block.  
*/

extern void DRV_USB_UHP_Tasks(SYS_MODULE_OBJ object);

// *****************************************************************************
/* Function:
    void DRV_USB_UHP_Tasks_ISR( SYS_MODULE_OBJ object )

  Summary:
    Maintains the driver's Interrupt state machine and implements its ISR.

  Description:
    This function is used to maintain the driver's internal Interrupt state
    machine and implement its ISR for interrupt-driven implementations.

  Precondition:
    The DRV_USB_UHP_Initialize function must have been called for the specified
    Hi-Speed USB Driver instance.

  Parameters:
    object - Object handle for the specified driver instance (returned from
    DRV_USB_UHP_Initialize).

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ object;     // Returned from DRV_USB_UHP_Initialize

    while (true)
    {
        DRV_USB_UHP_Tasks_ISR (object);

        // Do other tasks
    }
    </code>

  Remarks:
    This function should be called from the USB ISR.  For multiple USB modules,
    it should be ensured that the correct Hi-Speed USB Driver system module
    object is passed to this function.
*/

extern void DRV_USB_UHP_Tasks_ISR( SYS_MODULE_OBJ object );

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - Client Level
// *****************************************************************************
// *****************************************************************************

// ****************************************************************************
/* Function:
    DRV_HANDLE DRV_USB_UHP_Open
    ( 
        const SYS_MODULE_INDEX drvIndex,
        const DRV_IO_INTENT intent
    )
    
  Summary:
    Opens the specified Hi-Speed USB Driver instance and returns a handle to it.
	
  Description:
    This function opens the specified Hi-Speed USB Driver instance and provides a
    handle that must be provided to all other client-level operations to
    identify the caller and the instance of the driver. The intent flag
    should always be
    DRV_IO_INTENT_EXCLUSIVE|DRV_IO_INTENT_READWRITE|DRV_IO_INTENT_NON_BLOCKING.
    Any other setting of the intent flag will return a invalid driver
    handle. A driver instance can only support one client. Trying to open a
    driver that has an existing client will result in an unsuccessful
    function call.
	
  Precondition:
    Function DRV_USB_UHP_Initialize must have been called before calling this
    function.
	
  Parameters:
    drvIndex - Identifies the driver instance to be opened. As an example, this
    value can be set to DRV_USB_UHP_INDEX_0 if instance 0 of the driver has to be
    opened.
    
    intent - Should always be 
    (DRV_IO_INTENT_EXCLUSIVE|DRV_IO_INTENT_READWRITE| DRV_IO_INTENT_NON_BLOCKING).
				
  Returns:
    * DRV_HANDLE_INVALID - The driver could not be opened successfully.This can
     happen if the driver initialization was not complete or if an internal
     error has occurred.  
    * A Valid Driver Handle - This is an arbitrary value and is returned if the
      function was successful. This value will never be the same as
      DRV_HANDLE_INVALID. 
	
  Example:
    <code>

    DRV_HANDLE handle;

    // This code assumes that the driver has been initialized.
    handle = DRV_USB_UHP_Open(DRV_USB_UHP_INDEX_0, DRV_IO_INTENT_EXCLUSIVE| DRV_IO_INTENT_READWRITE| DRV_IO_INTENT_NON_BLOCKING);

    if(DRV_HANDLE_INVALID == handle)
    {
        // The application should try opening the driver again.
    }
    
    </code>
	
  Remarks:
    The handle returned is valid until the DRV_USB_UHP_Close function is called.
    The function will typically return DRV_HANDLE_INVALID if the driver was not
    initialized. In such a case the client should try to open the driver again.
*/

extern DRV_HANDLE DRV_USB_UHP_Open
(
    const SYS_MODULE_INDEX drvIndex,
    const DRV_IO_INTENT intent  
);

// *****************************************************************************
/* Function:
    void DRV_USB_UHP_Close( DRV_HANDLE handle )

  Summary:
    Closes an opened-instance of the Hi-Speed USB Driver.

  Description:
    This function closes an opened-instance of the Hi-Speed USB Driver,
    invalidating the handle.

  Precondition:
    The DRV_USB_UHP_Initialize function must have been called for the specified
    Hi-Speed USB Driver instance. DRV_USB_UHP_Open function must have been called
    to obtain a valid opened device handle.

  Parameters:
    handle  - Client's driver handle (returned from DRV_USB_UHP_Open function).

  Returns:
    None.

  Example:
    <code>
    DRV_HANDLE handle;  // Returned from DRV_USB_UHP_Open

    DRV_USB_UHP_Close(handle);
    </code>

  Remarks:
    After calling this function, the handle passed in handle parameter must not
    be  used with any of the other driver functions. A new handle must be
    obtained by calling DRV_USB_UHP_Open function before the caller may use the
    driver again.
*/

extern void DRV_USB_UHP_Close( DRV_HANDLE handle );

// *****************************************************************************
/* Function:
    void DRV_USB_UHP_ClientEventCallBackSet
    ( 
        DRV_HANDLE handle,
        uintptr_t hReferenceData,
        DRV_USB_UHP_EVENT_CALLBACK myEventCallBack 
    );

  Summary:
    This function sets up the event callback function that is invoked by the USB
    controller driver to notify the client of USB bus events.
	
  Description:
    This function sets up the event callback function that is invoked by the USB
    controller driver to notify the client of USB bus events. The callback is
    disabled by either not calling this function after the DRV_USB_UHP_Open
    function has been called or by setting the myEventCallBack argument as NULL.
    When the callback function is called, the hReferenceData argument is
    returned.
	
  Precondition:
    None.
	
  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UHP_Open function).

    hReferenceData - Object (could be a pointer) that is returned with the
    callback.  
    
    myEventCallBack -  Callback function for all USB events.
	
  Returns:
    None.
	
  Example:
    <code>

     // Set the client event callback for the Device Layer.  The
     // USBDeviceLayerEventHandler function is the event handler. When this
     // event handler is invoked by the driver, the driver returns back the
     // second argument specified in the following function (which in this case
     // is the Device Layer data structure). This allows the application
     // firmware to identify, as an example, the Device Layer object associated
     // with this callback.
    
    DRV_USB_UHP_ClientEventCallBackSet(myUSBDevice.usbDriverHandle, (uintptr_t)&myUSBDevice, USBDeviceLayerEventHandler);
    
    </code>
	
  Remarks:
    Typical usage of the Hi-Speed USB Driver requires a client to register a callback.                                                                         
*/

extern void DRV_USB_UHP_ClientEventCallBackSet
( 
    DRV_HANDLE handle,
    uintptr_t  hReferenceData ,
    DRV_USB_EVENT_CALLBACK myEventCallBack
);

// ****************************************************************************
/* Function:
    bool DRV_USB_UHP_EventsDisable
    (
        DRV_HANDLE handle
    );
    
  Summary:
    Disables Host mode events.
	
  Description:
    This function disables the Host mode events. This function is called by the
    Host Layer when it wants to execute code atomically. 
	
  Precondition:
    The handle should be valid.
	
  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UHP_Open function).
	
  Returns:
    * true - Driver event generation was enabled when this function was called.
    * false - Driver event generation was not enabled when this function was
      called. 
	
  Example:
    <code>
    // This code shows how the DRV_USB_UHP_EventsDisable and
    // DRV_USB_UHP_EventsEnable function can be called to disable and enable
    // events.

    DRV_HANDLE driverHandle;
    bool eventsWereEnabled;

    // Disable the driver events.
    eventsWereEnabled = DRV_USB_UHP_EventsDisable(driverHandle);

    // Code in this region will not be interrupted by driver events.

    // Enable the driver events.
    DRV_USB_UHP_EventsEnable(driverHandle, eventsWereEnabled);

    </code>
	
  Remarks:
    None.
*/

extern bool DRV_USB_UHP_EventsDisable
(
    DRV_HANDLE handle
);

// ****************************************************************************
/* Function:
    void DRV_USB_UHP_EventsEnable
    (
        DRV_HANDLE handle
        bool eventRestoreContext
    );
    
  Summary:
    Restores the events to the specified the original value.
	
  Description:
    This function will restore the enable disable state of the events.  The
    eventRestoreContext parameter should be equal to the value returned by the
    DRV_USB_UHP_EventsDisable function.
	
  Precondition:
    The handle should be valid.
	
  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).
    eventRestoreContext - Value returned by the DRV_USB_UHP_EventsDisable
    function.
	
  Returns:
    None.
	
  Example:
    <code>
    // This code shows how the DRV_USB_UHP_EventsDisable and
    // DRV_USB_UHP_EventsEnable function can be called to disable and enable
    // events.

    DRV_HANDLE driverHandle;
    bool eventsWereEnabled;

    // Disable the driver events.
    eventsWereEnabled = DRV_USB_UHP_EventsDisable(driverHandle);

    // Code in this region will not be interrupted by driver events.

    // Enable the driver events.
    DRV_USB_UHP_EventsEnable(driverHandle, eventsWereEnabled);

    </code>
	
  Remarks:
    None.
*/

extern void DRV_USB_UHP_EventsEnable
(
    DRV_HANDLE handle, 
    bool eventContext
);

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - Host Mode Operation
// *****************************************************************************
// *****************************************************************************

// ****************************************************************************
/* Function:
    void DRV_USB_UHP_IRPCancel(USB_HOST_IRP * inputIRP);
    
  Summary:
    Cancels the specified IRP.
	
  Description:
    This function attempts to cancel the specified IRP. If the IRP is queued and
    its processing has not started, it will be canceled successfully. If the
    IRP in progress, the ongoing transaction will be allowed to complete. 
	
  Precondition:
    None.
	
  Parameters:
    inputIRP - Pointer to the IRP to cancel.
	
  Returns:
    None.
	
  Example:
    <code>

    // This code shows how a submitted IRP can be canceled.

    USB_HOST_IRP irp;
    USB_ERROR result;
    USB_HOST_PIPE_HANDLE controlPipe;
    USB_SETUP_PACKET setup;
    uint8_t controlTransferData[32];

    irp.setup = setup;
    irp.data = controlTransferData;
    irp.size = 32;
    irp.flags = USB_HOST_IRP_FLAG_NONE ;
    irp.userData = &someApplicationObject;
    irp.callback = IRP_Callback;

    DRV_USB_UHP_HOST_IRPSubmit(controlPipeHandle, &irp);

    // Additional application logic may come here. This logic may decide to
    // cancel the submitted IRP.
    
    DRV_USB_UHP_IRPCancel(&irp);

    </code>
	
  Remarks:
    None.                                                                  
*/

extern void DRV_USB_UHP_IRPCancel(USB_HOST_IRP * inputIRP);

// ****************************************************************************
/* Function:
    void DRV_USB_UHP_PipeClose
    (
        DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
    );
    
  Summary:
    Closes an open pipe.
	
  Description:
    This function closes an open pipe. Any IRPs scheduled on the pipe will be
    aborted and IRP callback functions will be called with the status as
    DRV_USB_HOST_IRP_STATE_ABORTED. The pipe handle will become invalid and the
    pipe and will not accept IRPs.
	
  Precondition:
    The pipe handle should be valid.
	
  Parameters:
    pipeHandle - Handle to the pipe to close.
	
  Returns:
    None.
	
  Example:
    <code>
    // This code shows how an open Host pipe can be closed.
    
    DRV_HANDLE driverHandle;
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle;

    // Close the pipe.
    DRV_USB_UHP_PipeClose(pipeHandle);
    </code>
	
  Remarks:
    None.                                                                  
*/

extern void DRV_USB_UHP_PipeClose
(
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
);

// ****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UHP_HOST_IRPSubmit
    (
        DRV_USB_UHP_HOST_PIPE_HANDLE  hPipe,
        USB_HOST_IRP * pInputIRP
    );
    
  Summary:
    Submits an IRP on a pipe.
	
  Description:
    This function submits an IRP on the specified pipe. The IRP will be added to
    the queue and will be processed in turn. The data will be transferred on the
    bus based on the USB bus scheduling rules. When the IRP has been processed,
    the callback function specified in the IRP will be called. The IRP status
    will be updated to reflect the completion status of the IRP. 
	
  Precondition:
    The pipe handle should be valid.
	
  Parameters:
    hPipe - Handle to the pipe to which the IRP has to be submitted.

    pInputIRP - Pointer to the IRP.
	
  Returns:
    * USB_ERROR_NONE - The IRP was submitted successfully.
    * USB_ERROR_PARAMETER_INVALID - The pipe handle is not valid.
    * USB_ERROR_OSAL_FUNCTION - An error occurred in an OSAL function called in
      this function.
	
  Example:
    <code>
    // The following code shows an example of how the host layer populates
    // the IRP object and then submits it. IRP_Callback function is called when an
    // IRP has completed processing. The status of the IRP at completion can be
    // checked in the status flag. The size field of the irp will contain the amount
    // of data transferred.  

    void IRP_Callback(USB_HOST_IRP * irp)
    {
        // irp is pointing to the IRP for which the callback has occurred. In most
        // cases this function will execute in an interrupt context. The application
        // should not perform any hardware access or interrupt un-safe operations in
        // this function. 

        switch(irp->status)
        {
            case USB_HOST_IRP_STATUS_ERROR_UNKNOWN:
                // IRP was terminated due to an unknown error 
                break;

            case USB_HOST_IRP_STATUS_ABORTED:
                // IRP was terminated by the application 
                break;

            case USB_HOST_IRP_STATUS_ERROR_BUS:
                // IRP was terminated due to a bus error 
                break;

            case USB_HOST_IRP_STATUS_ERROR_DATA:
                // IRP was terminated due to data error 
                break;

            case USB_HOST_IRP_STATUS_ERROR_NAK_TIMEOUT:
                // IRP was terminated because of a NAK timeout 
                break;

            case USB_HOST_IRP_STATUS_ERROR_STALL:
                // IRP was terminated because of a device sent a STALL 
                break;

            case USB_HOST_IRP_STATUS_COMPLETED:
                // IRP has been completed 
                break;

            case USB_HOST_IRP_STATUS_COMPLETED_SHORT:
                // IRP has been completed but the amount of data processed was less
                // than requested. 
                break;

            default:
                break;
        }
    }

    // In the following code snippet the a control transfer IRP is submitted to a
    // control pipe. The setup parameter of the IRP points to the Setup command of
    // the control transfer. The direction of the data stage is specified by the
    // Setup packet. 

    USB_HOST_IRP irp;
    USB_ERROR result;
    USB_HOST_PIPE_HANDLE controlPipe;
    USB_SETUP_PACKET setup;
    uint8_t controlTransferData[32];

    irp.setup = setup;
    irp.data = controlTransferData;
    irp.size = 32;
    irp.flags = USB_HOST_IRP_FLAG_NONE ;
    irp.userData = &someApplicationObject;
    irp.callback = IRP_Callback;

    result = DRV_USB_UHP_HOST_IRPSubmit(controlPipeHandle, &irp);

    </code>
	
  Remarks:
    An IRP can also be submitted in an IRP callback function.                                                                  
*/
extern USB_ERROR DRV_USB_UHP_EHCI_HOST_IRPSubmit
(
    DRV_USB_UHP_HOST_PIPE_HANDLE  hPipe,
    USB_HOST_IRP * pinputIRP
);
extern USB_ERROR DRV_USB_UHP_OHCI_HOST_IRPSubmit
(
    DRV_USB_UHP_HOST_PIPE_HANDLE  hPipe,
    USB_HOST_IRP * pinputIRP
);

// ****************************************************************************
/* Function:
    DRV_USB_UHP_HOST_PIPE_HANDLE DRV_USB_UHP_PipeSetup 
    (
        DRV_HANDLE client,
        uint8_t deviceAddress, 
        USB_ENDPOINT endpointAndDirection,
        uint8_t hubAddress,
        uint8_t hubPort,
        USB_TRANSFER_TYPE pipeType, 
        uint8_t bInterval, 
        uint16_t wMaxPacketSize,
        USB_SPEED speed
    );
    
  Summary:
    Open a pipe with the specified attributes.
	
  Description:
    This function opens a communication pipe between the Host and the device
    endpoint. The transfer type and other attributes are specified through the
    function parameters. The driver does not check for available bus bandwidth,
    which should be done by the application (the USB Host Layer in this case)
	
  Precondition:
    The driver handle should be valid.
	
  Parameters:
    client - Handle to the driver (returned from DRV_USB_UHP_Open function).
    
    deviceAddress - USB Address of the device to connect to.
    
    endpoint - Endpoint on the device to connect to.
    
    hubAddress - Address of the hub to which this device is connected. If not
    connected to a hub, this value should be set to 0. 

    hubPort - Port number of the hub to which this device is connected.

    pipeType - Transfer type of the pipe to open.
    
    bInterval - Polling interval for periodic transfers. This should be
    specified as defined by the USB 2.0 Specification.

    wMaxPacketSize - This should be set to the endpoint size reported by the
    device in its configuration descriptors. This defines the maximum size of
    the transaction in a transfer on this pipe.

    speed - The speed of the pipe. This should match the speed at which the
    device connected to the Host.

  Returns:
    * DRV_USB_HOST_PIPE_HANDLE_INVALID - The pipe could not be created.
    * A valid Pipe Handle - The pipe was created successfully. This is an
      arbitrary value and will never be the same as
      DRV_USB_HOST_PIPE_HANDLE_INVALID.
	
  Example:
    <code>
    // This code shows how the DRV_USB_UHP_PipeSetup function is called for
    // create a communication pipe. In this example, Bulk pipe is created
    // between the Host and a device. The Device address is 2 and the target
    // endpoint on this device is 4 . The direction of the data transfer over
    // this pipe is from the Host to the device. The device is connected to Port
    // 1 of a Hub, whose USB address is 3. The maximum size of a transaction
    // on this pipe is 64 bytes. This is a Bulk Pipe and hence the bInterval
    // field is set to 0. The target device is operating at Full Speed.

    DRV_HANDLE driverHandle;
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle;

    pipeHandle = DRV_USB_UHP_PipeSetup(driverHandle, 0x02, 0x14, 0x03, 0x01, USB_TRANSFER_TYPE_BULK, 0, 64, USB_SPEED_FULL); 

    if(pipeHandle != DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID)
    {
        // The pipe was created successfully.
    }

    </code>
	
  Remarks:
    None.                                                                  
*/

extern DRV_USB_UHP_HOST_PIPE_HANDLE DRV_USB_UHP_PipeSetup 
(
    DRV_HANDLE client,
    uint8_t deviceAddress, 
    USB_ENDPOINT endpointAndDirection,
    uint8_t hubAddress,
    uint8_t hubPort,
    USB_TRANSFER_TYPE pipeType, 
    uint8_t bInterval, 
    uint16_t wMaxPacketSize,
    USB_SPEED speed
);


// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - Root hub
// *****************************************************************************
// *****************************************************************************

// ****************************************************************************
/* Function:
    void DRV_USB_UHP_ROOT_HUB_PortReset(DRV_HANDLE handle, uint8_t port );
    
  Summary:
    Resets the specified root hub port.
	
  Description:
    This function resets the root hub port. The reset duration is defined by
    DRV_USB_UHP_ROOT_HUB_RESET_DURATION. The status of the reset signaling can be
    checked using the DRV_USB_UHP_ROOT_HUB_PortResetIsComplete function.
	
  Precondition:
    None.
	
  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

    port - Port to reset.
	
  Returns:
    None.
	
  Example:
    <code>
    // This code shows how the DRV_USB_HOST_ROOT_HUB_PortReset and the
    // DRV_USB_UHP_ROOT_HUB_PortResetIsComplete functions are called to complete a
    // port reset sequence.

    DRV_HANDLE driverHandle;

    // Reset Port 0.
    DRV_USB_HOST_ROOT_HUB_PortReset(driverHandle, 0);

    // Check if the Reset operation has completed.
    if(DRV_USB_UHP_ROOT_HUB_PortResetIsComplete(driverHandle, 0) == false)
    {
        // This means that the Port Reset operation has not completed yet. The
        // DRV_USB_UHP_ROOT_HUB_PortResetIsComplete function should be called
        // again after some time to check the status.
    }

    </code>
	
  Remarks:
    The root hub on the PIC32MZ USB controller contains only one port - port 0.                                                                  
*/

extern USB_ERROR DRV_USB_UHP_ROOT_HUB_PortReset(DRV_HANDLE handle, uint8_t port );

// ****************************************************************************
/* Function:
    bool DRV_USB_UHP_ROOT_HUB_PortResetIsComplete
    (
        DRV_HANDLE handle,
        uint8_t port
    );

  Summary:
    Returns true if the root hub has completed the port reset operation.

  Description:
    This function returns true if the port reset operation has completed. It
    should be called after the DRV_USB_HOST_ROOT_HUB_PortReset function to
    check if the reset operation has completed.

  Precondition:
    None.

  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

    port - Port to check

  Returns:
    * true - The reset signaling has completed.
    * false - The reset signaling has not completed.

  Example:
    <code>
    // This code shows how the DRV_USB_HOST_ROOT_HUB_PortReset and the
    // DRV_USB_UHP_ROOT_HUB_PortResetIsComplete functions are called to complete a
    // port reset sequence.

    DRV_HANDLE driverHandle;

    // Reset Port 0.
    DRV_USB_HOST_ROOT_HUB_PortReset(driverHandle, 0);

    // Check if the Reset operation has completed.
    if(DRV_USB_UHP_ROOT_HUB_PortResetIsComplete(driverHandle, 0) == false)
    {
        // This means that the Port Reset operation has not completed yet. The
        // DRV_USB_UHP_ROOT_HUB_PortResetIsComplete function should be called
        // again after some time to check the status.
    }

    </code>

  Remarks:
    The root hub on this particular hardware only contains one port - port 0.
*/

extern bool DRV_USB_UHP_ROOT_HUB_PortResetIsComplete(DRV_HANDLE handle, uint8_t port );

// ****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UHP_HOST_ROOT_HUB_PortResume
    (
        DRV_HANDLE handle, 
        uint8_t port
    );
    
  Summary:
    Resumes the specified root hub port.
	
  Description:
    This function resumes the root hub. The resume duration is defined by
    DRV_USB_UHP_ROOT_HUB_RESUME_DURATION. The status of the resume signaling can
    be checked using the DRV_USB_UHP_ROOT_HUB_PortResumeIsComplete function.
	
  Precondition:
    None.
	
  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

    port - Port to resume.
	
  Returns:
    * USB_ERROR_NONE - The function executed successfully.
    * USB_ERROR_PARAMETER_INVALID - The driver handle is not valid or the port
      number does not exist.
	
  Example:
    <code>
    // This code shows how the DRV_USB_UHP_ROOT_HUB_PortResume function is
    // called to resume the specified port.

    DRV_HANDLE driverHandle;

    // Resume Port 0.
    DRV_USB_UHP_ROOT_HUB_PortResume(driverHandle, 0);

    </code>
	
  Remarks:
    The root hub on this particular hardware only contains one port - port 0.                                                                  
*/

extern USB_ERROR DRV_USB_UHP_ROOT_HUB_PortResume(DRV_HANDLE handle, uint8_t port);

// ****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UHP_ROOT_HUB_PortSuspend(DRV_HANDLE handle, uint8_t port);
    
  Summary:
    Suspends the specified root hub port.
	
  Description:
    This function suspends the root hub port. 
	
  Precondition:
    None.
	
  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

    port - Port to suspend.
	
  Returns:
    * USB_ERROR_NONE - The function executed successfully.
    * USB_ERROR_PARAMETER_INVALID - The driver handle is not valid or the port
      number does not exist.
	
  Example:
    <code>
    // This code shows how the DRV_USB_UHP_ROOT_HUB_PortSuspend function is
    // called to suspend the specified port.

    DRV_HANDLE driverHandle;

    // Suspend Port 0.
    DRV_USB_UHP_ROOT_HUB_PortSuspend(driverHandle, 0);

    </code>
	
  Remarks:
    The root hub on this particular hardware only contains one port - port 0.                                                                  
*/

extern USB_ERROR DRV_USB_UHP_ROOT_HUB_PortSuspend(DRV_HANDLE handle, uint8_t port);

// ****************************************************************************
/* Function:
    USB_SPEED DRV_USB_UHP_ROOT_HUB_PortSpeedGet
    (
        DRV_HANDLE handle, 
        uint8_t port
    );
    
  Summary:
    Returns the speed of at which the port is operating.
	
  Description:
    This function returns the speed at which the port is operating.
	
  Precondition:
    None.
	
  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

    port - Port number of the port to be analyzed..
	
  Returns:
    * USB_SPEED_ERROR - This value is returned  if the driver handle is not
      or if the speed information is not available or if the specified port is
      not valid.
    * USB_SPEED_HIGH - A High Speed device has been connected to the port.
    * USB_SPEED_FULL - A Full Speed device has been connected to the port.
    * USB_SPEED_LOW - A Low Speed device has been connected to the port.
	
  Example:
    <code>
    // This code shows how the DRV_USB_UHP_ROOT_HUB_PortSpeedGet function is
    // called to know the operating speed of the port. This also indicates the
    // operating speed of the device connected to this port.

    DRV_HANDLE driverHandle;
    USB_SPEED speed;

    speed = DRV_USB_UHP_ROOT_HUB_PortSpeedGet(driverHandle, 0);

    </code>
	
  Remarks:
    The root hub on this particular hardware only contains one port - port 0.                                                                  
*/

extern USB_SPEED DRV_USB_UHP_ROOT_HUB_PortSpeedGet(DRV_HANDLE handle, uint8_t port);

// ****************************************************************************
/* Function:
    USB_SPEED DRV_USB_UHP_ROOT_HUB_BusSpeedGet(DRV_HANDLE handle);
    
  Summary:
    This function returns the operating speed of the bus to which this root hub
    is connected.
	
  Description:
    This function returns the operating speed of the bus to which this root hub
    is connected.
 
  Precondition:
    None.
	
  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

  Returns:
    * USB_SPEED_HIGH - The Root hub is connected to a bus that is operating at
      High Speed.
    * USB_SPEED_FULL - The Root hub is connected to a bus that is operating at
      Full Speed.
	
  Example:
    <code>
    // This code shows how the DRV_USB_UHP_ROOT_HUB_BusSpeedGet function is
    // called to know the operating speed of the bus to which this Root hub is
    // connected.

    DRV_HANDLE driverHandle;
    USB_SPEED speed;

    speed = DRV_USB_UHP_ROOT_HUB_BusSpeedGet(driverHandle);
    </code>
	
  Remarks:
    None.
*/

extern USB_SPEED DRV_USB_UHP_ROOT_HUB_BusSpeedGet(DRV_HANDLE handle);

// ****************************************************************************
/* Function:
    uint32_t DRV_USB_UHP_ROOT_HUB_MaximumCurrentGet(DRV_HANDLE);
    
  Summary:
    Returns the maximum amount of current that this root hub can provide on the
    bus.
	
  Description:
    This function returns the maximum amount of current that this root hub can
    provide on the bus.
	
  Precondition:
    None.
	
  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

  Returns:
    Returns the maximum current (in milliamperes) that the root hub can supply. 
	
  Example:
    <code>
    // This code shows how the DRV_USB_UHP_ROOT_HUB_MaximumCurrentGet
    // function is called to obtain the maximum VBUS current that the Root hub
    // can supply.

    DRV_HANDLE driverHandle;
    uint32_t currentMilliAmperes;

    currentMilliAmperes = DRV_USB_UHP_ROOT_HUB_MaximumCurrentGet(driverHandle);
    </code>
	
  Remarks:
    None.
*/

extern uint32_t DRV_USB_UHP_ROOT_HUB_MaximumCurrentGet(DRV_HANDLE handle);

// ****************************************************************************
/* Function:
    uint8_t DRV_USB_UHP_ROOT_HUB_PortNumbersGet(DRV_HANDLE handle);

  Summary:
    Returns the number of ports this root hub contains.

  Description:
    This function returns the number of ports that this root hub contains.

  Precondition:
    None.

  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

  Returns:
    This function will always return 1.

  Example:
    <code>
    // This code shows how DRV_USB_UHP_ROOT_HUB_PortNumbersGet function can
    // be called to obtain the number of Root hub ports.

    DRV_HANDLE driverHandle;
    uint8_t nPorts;

    nPorts = DRV_USB_UHP_ROOT_HUB_PortNumbersGet(driverHandle);

    </code>

  Remarks:
    None.
*/

extern uint8_t DRV_USB_UHP_ROOT_HUB_PortNumbersGet(DRV_HANDLE handle);

// ****************************************************************************
/* Function:
    void DRV_USB_UHP_HOST_ROOT_HUB_OperationEnable
    (
        DRV_HANDLE handle, 
        bool enable
    );

  Summary:
    This function enables or disables root hub operation.

  Description:
    This function enables or disables root hub operation. When enabled, the root
    hub will detect devices attached to the port and will request the Host Layer
    to enumerate the device. This function is called by the Host Layer when it
    is ready to receive enumeration requests from the Host. If the operation is
    disabled, the root hub will not detect attached devices.

  Precondition:
    None.

  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

    enable - If this is set to true, root hub operation is enabled. If this is
    set to false, root hub operation is disabled.

  Returns:
    None.

  Example:
    <code>
    // This code shows how the DRV_USB_UHP_HOST_ROOT_HUB_OperationEnable and the
    // DRV_USB_UHP_ROOT_HUB_OperationIsEnabled functions are called to enable
    // the Root hub operation.

    DRV_HANDLE driverHandle;

    // Enable Root hub operation.
    DRV_USB_UHP_HOST_ROOT_HUB_OperationEnable(driverHandle);

    // Wait till the Root hub operation is enabled.  
    if(DRV_USB_UHP_ROOT_HUB_OperationIsEnabled(driverHandle) == false)
    {
        // The operation has not completed. Call the
        // DRV_USB_UHP_ROOT_HUB_OperationIsEnabled function again to check if
        // the operation has completed. Note that the DRV_USB_UHP_Tasks function
        // must be allowed to run at periodic intervals to allow the enable
        // operation to completed.
    }
    </code>

  Remarks:
    None.
*/

extern void DRV_USB_UHP_EHCI_HOST_ROOT_HUB_OperationEnable(DRV_HANDLE handle, bool enable);
extern void DRV_USB_UHP_OHCI_HOST_ROOT_HUB_OperationEnable(DRV_HANDLE handle, bool enable);

// ****************************************************************************
/* Function:
    bool DRV_USB_UHP_ROOT_HUB_OperationIsEnabled(DRV_HANDLE handle);

  Summary:
    Returns the operation enabled status of the root hub.

  Description:
    This function returns true if the DRV_USB_UHP_HOST_ROOT_HUB_OperationEnable
    function has completed enabling the Host.

  Precondition:
    None.

  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UHP_Open function).

  Returns:
    * true - Root hub operation is enabled.
    * false - Root hub operation is not enabled.

  Example:
    <code>
    // This code shows how the DRV_USB_UHP_HOST_ROOT_HUB_OperationEnable and the
    // DRV_USB_UHP_ROOT_HUB_OperationIsEnabled functions are called to enable
    // the Root hub operation.

    DRV_HANDLE driverHandle;

    // Enable Root hub operation.
    DRV_USB_UHP_HOST_ROOT_HUB_OperationEnable(driverHandle);

    // Wait till the Root hub operation is enabled.  
    if(DRV_USB_UHP_ROOT_HUB_OperationIsEnabled(driverHandle) == false)
    {
        // The operation has not completed. Call the
        // DRV_USB_UHP_ROOT_HUB_OperationIsEnabled function again to check if
        // the operation has completed. Note that the DRV_USB_UHP_Tasks function
        // must be allowed to run at periodic intervals to allow the enable
        // operation to completed.
    }

    </code>

  Remarks:
    None.
*/

extern bool DRV_USB_UHP_ROOT_HUB_OperationIsEnabled(DRV_HANDLE handle);

// ****************************************************************************
/* Function:
    void DRV_USB_UHP_ROOT_HUB_Initialize
    (
        DRV_HANDLE handle,
        USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo,
    )

  Summary:
    This function initializes the root hub driver.

  Description:
    This function initializes the root hub driver. It is called by the Host
    Layer at the time of processing the root hub devices. The Host Layer assigns a
    USB_HOST_DEVICE_INFO reference to this root hub driver. This identifies the
    relationship between the root hub and the Host Layer.

  Precondition:
    None.

  Parameters:
    handle - Handle to the driver.
    usbHostDeviceInfo - Reference provided by the Host.

  Returns:
    None.

  Example:
    <code>

    // This code shows how the USB Host Layer calls the
    // DRV_USB_UHP_ROOT_HUB_Initialize function. The usbHostDeviceInfo
    // parameter is an arbitrary identifier assigned by the USB Host Layer. Its
    // interpretation is opaque to the Root hub Driver.

    DRV_HANDLE drvHandle;
    USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo = 0x10003000;

    DRV_USB_UHP_ROOT_HUB_Initialize(drvHandle, usbHostDeviceInfo);

    </code>

  Remarks:
    None.
*/

extern void DRV_USB_UHP_ROOT_HUB_Initialize
(
    DRV_HANDLE handle,
    USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo
);


// *****************************************************************************
// *****************************************************************************
// Section: Included Files (continued)
// *****************************************************************************
// *****************************************************************************
/*  The file included below maps the interface definitions above to appropriate
    Static implementations, depending on build mode.
*/


#endif  // _DRV_UHP_H
