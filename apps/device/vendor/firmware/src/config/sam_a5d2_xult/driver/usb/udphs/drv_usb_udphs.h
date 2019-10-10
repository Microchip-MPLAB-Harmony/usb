/******************************************************************************
  USB Module Driver Interface Header File.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_udphs.h

  Summary:
    USB Module Driver Interface File.

  Description:
    The Full speed USB Module driver provides a simple interface to
    manage the "USB" peripheral on microcontrollers. This file defines
    the interface definitions and prototypes for the USB driver. The driver
    interface meets the requirements of the MPLAB Harmony USB Host and Device
    Layer.
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

#ifndef _DRV_USB_UDPHS_H
#define _DRV_USB_UDPHS_H

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
#include "system/system_module.h"
#include "driver/driver_common.h"
#include "driver/usb/drv_usb.h"
//#include "system/int/sys_int.h"
//#include "usb/usb_common.h"
//#include "usb/usb_hub.h"
//#include "usb/usb_chapter_9.h"

// *****************************************************************************
// *****************************************************************************
// Section: Hi-Speed USB Driver Constants
// *****************************************************************************
// *****************************************************************************

/*DOM-IGNORE-BEGIN*/#define DRV_USB_UDPHS_ENDPOINT_ALL 16/*DOM-IGNORE-END*/

// *****************************************************************************
/* USB Driver Device Mode Interface Functions.

  Summary:
    USB Driver Device Mode Interface Functions.

  Description:
    The Device Driver interface in the Device Layer Initialization data
    structure should be set to this value so that Device Layer can access the
    USB Driver Device Mode functions.

  Remarks:
    None.
*/

/*DOM-IGNORE-BEGIN*/extern DRV_USB_DEVICE_INTERFACE gDrvUSBUDPHSDeviceInterface;/*DOM-IGNORE-END */
#define DRV_USB_UDPHS_DEVICE_INTERFACE /*DOM-IGNORE-BEGIN*/&gDrvUSBUDPHSDeviceInterface/*DOM-IGNORE-END */

// *****************************************************************************
/* USB Driver Module Index 0 Definition.

  Summary:
    USB Driver Module Index 0 Definition.

  Description:
    This constant defines the value of USB Driver Index 0.  The SYS_MODULE_INDEX
    parameter of the DRV_USB_UDPHS_Initialize and DRV_USB_UDPHS_Open functions should be
    set to this value to identify instance 0 of the driver.

  Remarks:
    These constants should be used in place of hard-coded numeric literals
    and should be passed into the DRV_USB_UDPHS_Initialize and DRV_USB_UDPHS_Open
    functions to identify the driver instance in use. These are not
    indicative of the number of modules that are actually supported by the
    microcontroller.
*/

#define DRV_USB_UDPHS_INDEX_0         0

// *****************************************************************************
/* USB Driver Module Index 1 Definition.

  Summary:
    USB Driver Module Index 1 Definition.

  Description:
    This constant defines the value of USB Driver Index 1.  The SYS_MODULE_INDEX
    parameter of the DRV_USB_UDPHS_Initialize and DRV_USB_UDPHS_Open functions should be
    set to this value to identify instance 1 of the driver.

  Remarks:
    These constants should be used in place of hard-coded numeric literals
    and should be passed into the DRV_USB_UDPHS_Initialize and DRV_USB_UDPHS_Open
    functions to identify the driver instance in use. These are not
    indicative of the number of modules that are actually supported by the
    microcontroller.
*/

#define DRV_USB_UDPHS_INDEX_1         1

// *****************************************************************************
// Section: USB Device Driver Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

/*DOM-IGNORE-BEGIN*/#define DRV_USB_UDPHS_DEVICE_ENDPOINT_ALL 16/*DOM-IGNORE-END*/

// *****************************************************************************

/* USB Driver VBUS Levels

  Summary:
    Possible USB VBUS levels.

  Description:
    This enumeration lists the possible USB VBUS levels. In USB modules that do
	not contain a VBUS comparator, the application should provide a VBUS
	monitoring 	function to the driver. The function should return a value of
	this type. The driver will call this function periodically to monitor the
	VBUS.

  Remarks:
    None.
*/

typedef enum
{
	/* VBUS is below Session End */
	DRV_USB_VBUS_LEVEL_INVALID = 0,

	/* VBUS is above session end but below A valid */
	DRV_USB_VBUS_LEVEL_VALID = 1

} DRV_USB_VBUS_LEVEL;

// *****************************************************************************
/* USB Device VBUS Comparator Application Hook Function Pointer Type.

  Summary:
     USB Device VBUS Comparator Application Hook Function Pointer Type.

  Description:
    A function of the type defined here should be provided to the driver to
	monitor the status of the VBUS. The application should provide this function
	through the driver initialization data structure. The function should
	return the current state of the external VBUS comparator. The function
	should be non-blocking and will be called from the DRV_USB_UDPHS_Tasks
	function.

  Remarks:
    None.
*/

typedef DRV_USB_VBUS_LEVEL(* DRV_USB_UDPHS_VBUS_COMPARATOR)(void);


// *****************************************************************************
/* USB Driver Events Enumeration.

  Summary:
    Identifies the different events that the USB Driver provides.

  Description:
    This enumeration identifies the different events that are generated by the
    USB Driver.

  Remarks:
    None.
*/

typedef enum
{
    /* Bus error occurred and was reported */
    DRV_USB_UDPHS_EVENT_ERROR = DRV_USB_EVENT_ERROR,

    /* Host has issued a device reset */
    DRV_USB_UDPHS_EVENT_RESET_DETECT = DRV_USB_EVENT_RESET_DETECT,

    /* Resume detected while USB in suspend mode */
    DRV_USB_UDPHS_EVENT_RESUME_DETECT = DRV_USB_EVENT_RESUME_DETECT,

    /* Idle detected */
    DRV_USB_UDPHS_EVENT_IDLE_DETECT = DRV_USB_EVENT_IDLE_DETECT,

    /* Stall handshake has occurred */
    DRV_USB_UDPHS_EVENT_STALL = DRV_USB_EVENT_STALL,

    /* Device received SOF */
    DRV_USB_UDPHS_EVENT_SOF_DETECT = DRV_USB_EVENT_SOF_DETECT,

    /* Session valid */
    DRV_USB_UDPHS_EVENT_DEVICE_SESSION_VALID = DRV_USB_EVENT_DEVICE_SESSION_VALID,

    /* Session Invalid */
    DRV_USB_UDPHS_EVENT_DEVICE_SESSION_INVALID = DRV_USB_EVENT_DEVICE_SESSION_INVALID,

} DRV_USB_UDPHS_EVENT;

// *****************************************************************************
/* USB Device Operation Speed Configuration.

  Summary:
    Identifies the Speeds supported by USB_UDPHS USB Driver.

  Description:
    This enumeration identifies the different speed configurations that are
	supported by the USB_UDPHS USB Driver.

  Remarks:
    None.
*/

typedef enum
{
    DRV_USB_UDPHS_DEVICE_SPEED_CFG_NORMAL = 0x0,
    DRV_USB_UDPHS_DEVICE_SPEED_CFG_HIGH_SPEED = 0x2,
    DRV_USB_UDPHS_DEVICE_SPEED_CFG_FULL_SPEED = 0x3,
}
DRV_USB_UDPHS_DEVICE_SPEED_CFG;

// *****************************************************************************
/* USB Operating Modes Enumeration.

  Summary:
    Identifies the operating modes supported by the USB Driver.

  Description:
    This enumeration identifies the operating modes supported by the USB Driver.

  Remarks:
    None.
*/

typedef enum
{
    /* The driver supports device mode operation only */
    DRV_USB_UDPHS_OPMODE_DEVICE  = DRV_USB_OPMODE_DEVICE

} DRV_USB_UDPHS_OPMODES;

// *****************************************************************************
/* Type of the USB Driver Event Callback Function.

  Summary:
    Type of the USB Driver event callback function.

  Description:
    Define the type of the USB Driver event callback function. The
    client should register an event callback function of this type when it
    intends to receive events from the USB Driver. The event callback
    function is registered using the DRV_USB_UDPHS_ClientEventCallBackSet function.

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

typedef void (*DRV_USB_UDPHS_EVENT_CALLBACK)
(
    uintptr_t hClient,
    DRV_USB_UDPHS_EVENT  eventType,
    void * eventData
);


// *****************************************************************************
/* USB Device Driver Initialization Data.

  Summary:
    This type definition defines the Driver Initialization Data Structure.

  Description:
    This structure contains all the data necessary to initialize the USB Driver.
    A pointer to a structure of this type, containing the desired initialization
    data, must be passed into the DRV_USB_UDPHS_Initialize function.

  Remarks:
    None.
*/

typedef struct
{
    /* System Module Initialization */
    SYS_MODULE_INIT moduleInit;

    /* Identifies the USB peripheral to be used. This should be the USB PLIB
       module instance identifier. */
    udphs_registers_t * usbID;

    /* This should be set to true if the USB module must stop operation in Standby
       mode */
    bool runInStandby;

    /* This should be set to true if the USB module must suspend when the CPU
       enters sleep mode. */
    bool suspendInSleep;

    /* Specify the interrupt source for the USB module. This should be the
       interrupt source identifier for the USB module instance specified in
       usbID. */
    uint8_t interruptSource;

    /* Specify the operational speed of the USB module. This should always be
       set to USB_SPEED_FULL. */
    USB_SPEED operationSpeed;

    /* When operating in Device mode, the application can set this pointer to a VBUS
       monitoring function. The driver will call this function periodically
       from the DRV_USB_UDPHS_Tasks function. This function should return
       the present voltage level of the VBUS line. The driver will generate
       a DRV_USB_UDPHS_EVENT_DEVICE_SESSION_VALID event when the VBUS voltage
       is above A session valid and below VBUS valid. The driver will generate
       a DRV_USB_UDPHS_EVENT_DEVICE_SESSION_INVALID event when the VBUS
       voltage drops below A Session Valid. If this function pointer is NULL, the
       driver assumes that VBUS is above session valid. */
    DRV_USB_UDPHS_VBUS_COMPARATOR vbusComparator;

} DRV_USB_UDPHS_INIT;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - System Level
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ DRV_USB_UDPHS_Initialize
    (
        const SYS_MODULE_INDEX drvIndex,
        const SYS_MODULE_INIT * const init
    )

  Summary:
    Initializes the Hi-Speed USB Driver.

  Description:
    This function initializes the Hi-Speed USB Driver, making it ready for
    clients to open. The driver initialization does not complete when this
    function returns. The DRV_USB_UDPHS_Tasks function must called periodically to
    complete the driver initialization. The DRV_USB_UDPHS_Open function will fail if
    the driver was not initialized or if initialization has not completed.

  Precondition:
    None.

  Parameters:
    drvIndex - Ordinal number of driver instance to be initialized. This should
    be set to DRV_USB_UDPHS_INDEX_0 if driver instance 0 needs to be initialized.

    init - Pointer to a data structure containing data necessary to
    initialize the driver. This should be a DRV_USB_UDPHS_INIT structure reference
    typecast to SYS_MODULE_INIT reference.

  Returns:
    * SYS_MODULE_OBJ_INVALID - The driver initialization failed.
    * A valid System Module Object - The driver initialization was able to
      start. It may have not completed and requires the DRV_USB_UDPHS_Tasks function
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

    DRV_USB_UDPHS_INIT moduleInit;

    usbInitData.usbID               = USBHS_ID_0;
    usbInitData.opMode              = DRV_USB_UDPHS_OPMODE_DEVICE;
    usbInitData.stopInIdle          = false;
    usbInitData.suspendInSleep      = false;
    usbInitData.operationSpeed      = USB_SPEED_FULL;
    usbInitData.interruptSource     = INT_SOURCE_USB;

    usbInitData.sysModuleInit.powerState = SYS_MODULE_POWER_RUN_FULL ;

    // This is how this data structure is passed to the initialize
    // function.

    DRV_USB_UDPHS_Initialize(DRV_USB_UDPHS_INDEX_0, (SYS_MODULE_INIT *) &usbInitData);

    </code>

  Remarks:
    This function must be called before any other Hi-Speed USB Driver function
    is called. This function should only be called once during system
    initialization unless DRV_USB_UDPHS_Deinitialize is called to deinitialize the
    driver instance.
*/

SYS_MODULE_OBJ DRV_USB_UDPHS_Initialize
(
    const SYS_MODULE_INDEX drvIndex,
    const SYS_MODULE_INIT * const init
);
// *****************************************************************************
/* Function:
    SYS_STATUS DRV_USB_UDPHS_Status ( SYS_MODULE_OBJ object )

  Summary:
    Provides the current status of the Hi-Speed USB Driver module.

  Description:
    This function provides the current status of the Hi-Speed USB Driver module.

  Precondition:
    The DRV_USB_UDPHS_Initialize function must have been called before calling this
    function.

  Parameters:
    object - Driver object handle, returned from the DRV_USB_UDPHS_Initialize function.

  Returns:
    * SYS_STATUS_READY - Indicates that the driver is ready.
    * SYS_STATUS_UNINITIALIZED - Indicates that the driver has never been
      initialized.

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_USB_UDPHS_Initialize
    SYS_STATUS          status;
    DRV_USB_UDPHS_INIT moduleInit;

    usbInitData.usbID               = USBHS_ID_0;
    usbInitData.opMode              = DRV_USB_UDPHS_OPMODE_DEVICE;
    usbInitData.stopInIdle          = false;
    usbInitData.suspendInSleep      = false;
    usbInitData.operationSpeed      = USB_SPEED_FULL;
    usbInitData.interruptSource     = INT_SOURCE_USB;

    usbInitData.sysModuleInit.powerState = SYS_MODULE_POWER_RUN_FULL ;

    // This is how this data structure is passed to the initialize
    // function.

    DRV_USB_UDPHS_Initialize(DRV_USB_UDPHS_INDEX_0, (SYS_MODULE_INIT *) &usbInitData);

    // The status of the driver can be checked.
    status = DRV_USB_UDPHS_Status(object);
    if(SYS_STATUS_READY == status)
    {
        // Driver is ready to be opened.
    }

    </code>

  Remarks:
    None.
*/

SYS_STATUS DRV_USB_UDPHS_Status ( SYS_MODULE_OBJ object );

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_Tasks( SYS_MODULE_OBJ object )

  Summary:
    Maintains the driver's state machine when the driver is configured for
    Polled mode.

  Description:
    Maintains the driver's Polled state machine. This function should be called
    from the SYS_Tasks function.

  Precondition:
    The DRV_USB_UDPHS_Initialize function must have been called for the specified
    Hi-Speed USB Driver instance.

  Parameters:
    object - Object handle for the specified driver instance (returned from
    DRV_USB_UDPHS_Initialize function).

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from DRV_USB_UDPHS_Initialize

    while (true)
    {
        DRV_USB_UDPHS_Tasks(object);

        // Do other tasks
    }
    </code>

  Remarks:
    This function is normally not called  directly  by  an  application.   It  is
    called by the system's Tasks function (SYS_Tasks). This function will never
    block.
*/

void DRV_USB_UDPHS_Tasks(SYS_MODULE_OBJ object);

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_Tasks_ISR( SYS_MODULE_OBJ object )

  Summary:
    Maintains the driver's Interrupt state machine and implements its ISR.

  Description:
    This function is used to maintain the driver's internal Interrupt state
    machine and implement its ISR for interrupt-driven implementations.

  Precondition:
    The DRV_USB_UDPHS_Initialize function must have been called for the specified
    Hi-Speed USB Driver instance.

  Parameters:
    object - Object handle for the specified driver instance (returned from
    DRV_USB_UDPHS_Initialize).

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ object;     // Returned from DRV_USB_UDPHS_Initialize

    while (true)
    {
        DRV_USB_UDPHS_Tasks_ISR (object);

        // Do other tasks
    }
    </code>

  Remarks:
    This function should be called from the USB ISR.  For multiple USB modules,
    it should be ensured that the correct Hi-Speed USB Driver system module
    object is passed to this function.
*/

void DRV_USB_UDPHS_Tasks_ISR( SYS_MODULE_OBJ object );

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - Client Level
// *****************************************************************************
// *****************************************************************************

// ****************************************************************************
/* Function:
    DRV_HANDLE DRV_USB_UDPHS_Open
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
    Function DRV_USB_UDPHS_Initialize must have been called before calling this
    function.

  Parameters:
    drvIndex - Identifies the driver instance to be opened. As an example, this
    value can be set to DRV_USB_UDPHS_INDEX_0 if instance 0 of the driver has to be
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
    handle = DRV_USB_UDPHS_Open(DRV_USB_UDPHS_INDEX_0, DRV_IO_INTENT_EXCLUSIVE| DRV_IO_INTENT_READWRITE| DRV_IO_INTENT_NON_BLOCKING);

    if(DRV_HANDLE_INVALID == handle)
    {
        // The application should try opening the driver again.
    }

    </code>

  Remarks:
    The handle returned is valid until the DRV_USB_UDPHS_Close function is called.
    The function will typically return DRV_HANDLE_INVALID if the driver was not
    initialized. In such a case the client should try to open the driver again.
*/

DRV_HANDLE DRV_USB_UDPHS_Open
(
    const SYS_MODULE_INDEX drvIndex,
    const DRV_IO_INTENT intent
);

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_Close( DRV_HANDLE handle )

  Summary:
    Closes an opened-instance of the Hi-Speed USB Driver.

  Description:
    This function closes an opened-instance of the Hi-Speed USB Driver,
    invalidating the handle.

  Precondition:
    The DRV_USB_UDPHS_Initialize function must have been called for the specified
    Hi-Speed USB Driver instance. DRV_USB_UDPHS_Open function must have been called
    to obtain a valid opened device handle.

  Parameters:
    handle  - Client's driver handle (returned from DRV_USB_UDPHS_Open function).

  Returns:
    None.

  Example:
    <code>
    DRV_HANDLE handle;  // Returned from DRV_USB_UDPHS_Open

    DRV_USB_UDPHS_Close(handle);
    </code>

  Remarks:
    After calling this function, the handle passed in handle parameter must not
    be  used with any of the other driver functions. A new handle must be
    obtained by calling DRV_USB_UDPHS_Open function before the caller may use the
    driver again.
*/

void DRV_USB_UDPHS_Close( DRV_HANDLE handle );

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_ClientEventCallBackSet
    (
        DRV_HANDLE handle,
        uintptr_t hReferenceData,
        DRV_USB_UDPHS_EVENT_CALLBACK myEventCallBack
    );

  Summary:
    This function sets up the event callback function that is invoked by the USB
    controller driver to notify the client of USB bus events.

  Description:
    This function sets up the event callback function that is invoked by the USB
    controller driver to notify the client of USB bus events. The callback is
    disabled by either not calling this function after the DRV_USB_UDPHS_Open
    function has been called or by setting the myEventCallBack argument as NULL.
    When the callback function is called, the hReferenceData argument is
    returned.

  Precondition:
    None.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).

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

    DRV_USB_UDPHS_ClientEventCallBackSet(myUSBDevice.usbDriverHandle, (uintptr_t)&myUSBDevice, USBDeviceLayerEventHandler);

    </code>

  Remarks:
    Typical usage of the Hi-Speed USB Driver requires a client to register a callback.
*/

void DRV_USB_UDPHS_ClientEventCallBackSet
(
    DRV_HANDLE handle,
    uintptr_t  hReferenceData ,
    DRV_USB_EVENT_CALLBACK myEventCallBack
);

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines - Device Mode Operation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_DEVICE_AddressSet(DRV_HANDLE handle, uint8_t address);

  Summary:
    This function will set the USB module address that is obtained from the Host.

  Description:
    This function will set the USB module address  that  is  obtained  from  the
    Host in a setup transaction. The address is obtained from  the  SET_ADDRESS
    command issued by the Host. The  primary  (first)  client  of  the  driver
    uses this function to set the module's USB address after decoding the  setup
    transaction from the Host.

  Precondition:
    None.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).
    address - The address of this module on the USB bus.

  Returns:
    None.

  Example:
    <code>
    // This function should be called by the first client of the driver,
    // which is typically the Device Layer. The address to set is obtained
    // from the Host during enumeration.

    DRV_USB_UDPHS_DEVICE_AddressSet(deviceLayer, 4);
    </code>

  Remarks:
    None.
*/

void DRV_USB_UDPHS_DEVICE_AddressSet(DRV_HANDLE handle, uint8_t address);

// *****************************************************************************
/* Function:
    USB_SPEED DRV_USB_UDPHS_DEVICE_CurrentSpeedGet(DRV_HANDLE handle);

  Summary:
    This function will return the USB speed at which the device is operating.

  Description:
    This function will return the USB speed at which the device is operating.

  Precondition:
    Only valid after the device is attached to the Host and Host has completed
    reset signaling.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).

  Returns:
    Returns a member of the USB_SPEED type.

  Example:
    <code>
    // Get the current speed.

    USB_SPEED deviceSpeed;

    deviceSpeed = DRV_USB_UDPHS_DEVICE_CurrentSpeedGet(deviceLayer);

    if(deviceLayer == USB_SPEED_HIGH)
    {
        // Possibly adjust buffers for higher throughput.
    }

    </code>

  Remarks:
    None.
*/

USB_SPEED DRV_USB_UDPHS_DEVICE_CurrentSpeedGet(DRV_HANDLE handle);

// *****************************************************************************
/* Function:
    uint16_t DRV_USB_UDPHS_DEVICE_SOFNumberGet(DRV_HANDLE handle);

  Summary:
    This function will return the USB SOF packet number.

  Description:
    This function will return the USB SOF packet number..

  Precondition:
    This function will return a valid value only when the device is attached to
    the bus. The SOF packet count will not increment if the bus is suspended.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).

  Returns:
    The SOF packet number.

  Example:
    <code>
    // This code shows how the DRV_USB_UDPHS_DEVICE_SOFNumberGet function is called
    // to read the current SOF number.

    DRV_HANDLE handle;
    uint16_t sofNumber;

    sofNumber = DRV_USB_UDPHS_DEVICE_SOFNumberGet(handle);

    </code>

  Remarks:
    None.
*/

uint16_t DRV_USB_UDPHS_DEVICE_SOFNumberGet(DRV_HANDLE handle);

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_DEVICE_Attach(DRV_HANDLE handle);

  Summary:
    This function will enable the attach signaling resistors on the D+ and D-
    lines thus letting the USB Host know that a device has been attached on the
    bus.

  Description:
    This function enables the pull-up resistors on the D+ or D- lines thus
    letting the USB Host know that a device has been attached on the bus . This
    function should be called when the driver client is ready  to  receive
    communication  from  the  Host (typically after all initialization is
    complete). The USB 2.0 specification requires VBUS to be detected before the
    data line pull-ups are enabled. The application must ensure the same.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).

  Returns:
    None.

  Example:
    <code>

    // Open the device driver and attach the device to the USB.
    handle = DRV_USB_UDPHS_Open(DRV_USB_UDPHS_INDEX_0, DRV_IO_INTENT_EXCLUSIVE| DRV_IO_INTENT_READWRITE| DRV_IO_INTENT_NON_BLOCKING);

    // Register a callback
    DRV_USB_UDPHS_ClientEventCallBackSet(handle, (uintptr_t)&myDeviceLayer, MyDeviceLayerEventCallback);

    // The device can be attached when VBUS Session Valid event occurs
    void MyDeviceLayerEventCallback(uintptr_t handle, DRV_USB_UDPHS_EVENT event, void * hReferenceData)
    {
        switch(event)
        {
            case DRV_USB_UDPHS_EVENT_DEVICE_SESSION_VALID:
                // A valid VBUS was detected.
                DRV_USB_UDPHS_DEVICE_Attach(handle);
                break;

            case DRV_USB_UDPHS_EVENT_DEVICE_SESSION_INVALID:
                // VBUS is not valid anymore. The device can be disconnected.
                DRV_USB_UDPHS_DEVICE_Detach(handle);
                break;

            default:
                break;
            }
        }
    }

    </code>

  Remarks:
    None.
*/

void DRV_USB_UDPHS_DEVICE_Attach(DRV_HANDLE handle);

// *****************************************************************************
/* Function:
    void DRV_USB_UDPHS_DEVICE_Detach(DRV_HANDLE handle);

  Summary:
    This function will disable the attach signaling resistors on the D+ and D-
    lines thus letting the USB Host know that the device has detached from the
    bus.

  Description:
    This function disables the pull-up resistors on the D+ or D- lines. This
    function should be called when the application wants to disconnect the
    device  from  the bus (typically to implement a soft detach or switch  to
    Host  mode operation).  A self-powered device should be detached from the
    bus when the VBUS is not valid.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).

  Returns:
    None.

  Example:
    <code>

    // Open the device driver and attach the device to the USB.
    handle = DRV_USB_UDPHS_Open(DRV_USB_UDPHS_INDEX_0, DRV_IO_INTENT_EXCLUSIVE| DRV_IO_INTENT_READWRITE| DRV_IO_INTENT_NON_BLOCKING);

    // Register a callback
    DRV_USB_UDPHS_ClientEventCallBackSet(handle, (uintptr_t)&myDeviceLayer, MyDeviceLayerEventCallback);

    // The device can be detached when VBUS Session Invalid event occurs
    void MyDeviceLayerEventCallback(uintptr_t handle, DRV_USB_UDPHS_EVENT event, void * hReferenceData)
    {
        switch(event)
        {
            case DRV_USB_UDPHS_EVENT_DEVICE_SESSION_VALID:
                // A valid VBUS was detected.
                DRV_USB_UDPHS_DEVICE_Attach(handle);
                break;

            case DRV_USB_UDPHS_EVENT_DEVICE_SESSION_INVALID:
                // VBUS is not valid anymore. The device can be disconnected.
                DRV_USB_UDPHS_DEVICE_Detach(handle);
                break;

            default:
                break;
            }
        }
    }

    </code>

  Remarks:
    None.
*/

void DRV_USB_UDPHS_DEVICE_Detach(DRV_HANDLE handle);

// ****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointEnable
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection,
        USB_TRANSFER_TYPE transferType,
        uint16_t endpointSize
    );

  Summary:
    This function enables an endpoint for the specified direction and endpoint
    size.

  Description:
    This function enables an endpoint for the specified direction and endpoint
    size. The function will enable the endpoint for communication in one
    direction at a time. It must be called twice if the endpoint is required to
    communicate in both the directions, with the exception of control endpoints.
    If the endpoint type is a control endpoint, the endpoint is always
    bidirectional and the function needs to be called only once.

    The size of the endpoint must match the wMaxPacketSize reported in the
    endpoint descriptor for this endpoint. A transfer that is scheduled over
    this endpoint will be scheduled in wMaxPacketSize transactions. The function
    does not check if the endpoint is already in use. It is the client's
    responsibility to make sure that a endpoint is not accidentally reused.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).

    endpointAndDirection - Specifies the endpoint and direction.

    transferType - Should be USB_TRANSFER_TYPE_CONTROL for control endpoint,
    USB_TRANSFER_TYPE_BULK for bulk endpoint, USB_TRANSFER_TYPE_INTERRUPT for
    interrupt endpoint and USB_TRANSFER_TYPE_ISOCHRONOUS for isochronous
    endpoint.

    endpointSize - Maximum size (in bytes) of the endpoint as reported in the
    endpoint descriptor.

  Returns:
    * USB_ERROR_NONE - The endpoint was successfully enabled.
    * USB_ERROR_DEVICE_ENDPOINT_INVALID - If the endpoint that is being accessed
      is not a valid endpoint defined for this driver instance.  The value of
      DRV_USB_UDPHS_ENDPOINTS_NUMBER configuration constant should be adjusted.
    * USB_ERROR_PARAMETER_INVALID - The driver handle is invalid.

  Example:
    <code>
    // This code shows an example of how to enable Endpoint
    // 0 for control transfers. Note that for a control endpoint, the
    // direction parameter is ignored. A control endpoint is always
    // bidirectional. Endpoint size is 64 bytes.

    uint8_t ep;

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 0);

    DRV_USB_UDPHS_DEVICE_EndpointEnable(handle, ep, USB_TRANSFER_TYPE_CONTROL, 64);

    // This code shows an example of how to set up a endpoint
    // for BULK IN transfer. For an IN transfer, data moves from device
    // to Host. In this example, Endpoint 1 is enabled. The maximum
    // packet size is 64.

    uint8_t ep;

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 1);

    DRV_USB_UDPHS_DEVICE_EndpointEnable(handle, ep, USB_TRANSFER_TYPE_BULK, 64);

    // If Endpoint 1 must also be set up for BULK OUT, the
    // DRV_USB_UDPHS_DEVICE_EndpointEnable function must be called again, as shown
    // here.

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_HOST_TO_DEVICE, 1);

    DRV_USB_UDPHS_DEVICE_EndpointEnable(handle, ep, USB_TRANSFER_TYPE_BULK, 64);
    </code>

  Remarks:
    None.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointEnable
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection,
    USB_TRANSFER_TYPE transferType,
    uint16_t endpointSize
);

// ***************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointDisable
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function disables an endpoint.

  Description:
    This function disables an endpoint. If the endpoint type is a control
    endpoint type, both directions are disabled. For non-control endpoints, the
    function disables the specified direction only. The direction to be disabled
    is specified by the Most Significant Bit (MSB) of the endpointAndDirection
    parameter.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).
    endpointAndDirection - Specifies the endpoint and direction.

  Returns:
    * USB_ERROR_NONE - The endpoint was successfully enabled.
    * USB_ERROR_DEVICE_ENDPOINT_INVALID - The endpoint that is being accessed
      is not a valid endpoint (endpoint was not provisioned through the
      DRV_USB_UDPHS_ENDPOINTS_NUMBER configuration constant) defined for this driver
      instance.

  Example:
    <code>
    // This code shows an example of how to disable
    // a control endpoint. Note that the direction parameter is ignored.
    // For a control endpoint, both the directions are disabled.

    USB_ENDPOINT ep;

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 0);

    DRV_USB_UDPHS_DEVICE_EndpointDisable(handle, ep );

    // This code shows an example of how to disable a BULK IN
    // endpoint

    USB_ENDPOINT ep;

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 1);

    DRV_USB_UDPHS_DEVICE_EndpointDisable(handle, ep );

    </code>

  Remarks:
    None.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointDisable
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
);

// *****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointDisableAll(DRV_HANDLE handle)

  Summary:
    This function disables all provisioned endpoints.

  Description:
    This function disables all provisioned endpoints in both directions.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).

  Returns:
    * USB_ERROR_NONE - The function exited successfully.
    * USB_ERROR_PARAMETER_INVALID - The driver handle is invalid.

  Example:
    <code>
    // This code shows an example of how to disable all endpoints.

    DRV_USB_UDPHS_DEVICE_EndpointDisableAll(handle);

    </code>

  Remarks:
    This function is typically called by the USB Device Layer to disable
    all endpoints upon detecting a bus reset.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointDisableAll(DRV_HANDLE handle);

// ****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointStall
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function stalls an endpoint in the specified direction.

  Description:
    This function stalls an endpoint in the specified direction.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).
    endpointAndDirection -  Specifies the endpoint and direction.

  Returns:
    * USB_ERROR_NONE - The endpoint was successfully enabled.
    * USB_ERROR_PARAMETER_INVALID - The driver handle is not valid.
    * USB_ERROR_DEVICE_ENDPOINT_INVALID - If the endpoint that is being
      accessed is out of the valid endpoint defined for this driver instance.
    * USB_ERROR_OSAL_FUNCTION - An error with an OSAL function called in this
      function.

  Example:
    <code>
    // This code shows an example of how to stall an endpoint. In
    // this example, Endpoint 1 IN direction is stalled.

    USB_ENDPOINT ep;

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 1);

    DRV_USB_UDPHS_DEVICE_EndpointStall(handle, ep);

    </code>

  Remarks:
    None.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointStall
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
);

// ****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointStallClear
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function clears the stall on an endpoint in the specified direction.

  Description:
    This function clears the stall on an endpoint in the specified direction.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).
    endpointAndDirection -  Specifies the endpoint and direction.

  Returns:
    * USB_ERROR_NONE - The endpoint was successfully enabled.
    * USB_ERROR_PARAMETER_INVALID - The driver handle is not valid.
    * USB_ERROR_DEVICE_ENDPOINT_INVALID - If the endpoint that is being
      accessed is out of the valid endpoint defined for this driver instance.

  Example:
    <code>
    // This code shows an example of how to clear a stall. In this
    // example, the stall condition on Endpoint 1 IN direction is cleared.

    USB_ENDPOINT ep;

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 1);

    DRV_USB_UDPHS_DEVICE_EndpointStallClear(handle, ep);

    </code>

  Remarks:
    None.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_EndpointStallClear
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
);

// *****************************************************************************
/* Function:
    bool DRV_USB_UDPHS_DEVICE_EndpointIsEnabled
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function returns the enable/disable status of the specified endpoint
    and direction.

  Description:
    This function returns the enable/disable status of the specified endpoint
    and direction.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).
    endpointAndDirection - Specifies the endpoint and direction.

  Returns:
    * true - The endpoint is enabled.
    * false - The endpoint is disabled.

  Example:
    <code>
    // This code shows an example of how the
    // DRV_USB_UDPHS_DEVICE_EndpointIsEnabled function can be used to obtain the
    // status of Endpoint 1 and IN direction.

    USB_ENDPOINT ep;

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 1);

    if(DRV_USB_UDPHS_ENDPOINT_STATE_DISABLED ==
                DRV_USB_UDPHS_DEVICE_EndpointIsEnabled(handle, ep))
    {
        // Endpoint is disabled. Enable endpoint.

        DRV_USB_UDPHS_DEVICE_EndpointEnable(handle, ep, USB_ENDPOINT_TYPE_BULK, 64);

    }

    </code>

  Remarks:
    None.
*/

bool DRV_USB_UDPHS_DEVICE_EndpointIsEnabled
(
    DRV_HANDLE client,
    USB_ENDPOINT endpointAndDirection
);

// *****************************************************************************
/* Function:
    bool DRV_USB_UDPHS_DEVICE_EndpointIsStalled
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function returns the stall status of the specified endpoint and
    direction.

  Description:
    This function returns the stall status of the specified endpoint and
    direction.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).

    endpointAndDirection - Specifies the endpoint and direction.

  Returns:
    * true - The endpoint is stalled.
    * false - The endpoint is not stalled.

  Example:
    <code>
    // This code shows an example of how the
    // DRV_USB_UDPHS_DEVICE_EndpointIsStalled function can be used to obtain the
    // stall status of Endpoint 1 and IN direction.

    USB_ENDPOINT ep;

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 1);

    if(true == DRV_USB_UDPHS_DEVICE_EndpointIsStalled (handle, ep))
    {
        // Endpoint stall is enabled. Clear the stall.

        DRV_USB_UDPHS_DEVICE_EndpointStallClear(handle, ep);

    }

    </code>

  Remarks:
    None.
*/

bool DRV_USB_UDPHS_DEVICE_EndpointIsStalled
(
    DRV_HANDLE client,
    USB_ENDPOINT endpoint
);

// ***********************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_IRPSubmit
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection,
        USB_DEVICE_IRP * irp
    );

  Summary:
    This function submits an I/O Request Packet (IRP) for processing to the
    Hi-Speed USB Driver.

  Description:
    This function submits an I/O Request Packet (IRP) for processing to the USB
    Driver. The IRP allows a client to send and receive data from the USB Host.
    The data will be sent or received through the specified endpoint. The direction
    of the data transfer is indicated by the direction flag in the
    endpointAndDirection parameter. Submitting an IRP arms the endpoint to
    either send data to or receive data from the Host.  If an IRP is already
    being processed on the endpoint, the subsequent IRP submit operation
    will be queued. The contents of the IRP (including the application buffers)
    should not be changed until the IRP has been processed.

    Particular attention should be paid to the size parameter of IRP. The
    following should be noted:

      * The size parameter while sending data to the Host can be less than,
        greater than, equal to, or be an exact multiple of the maximum packet size
        for the endpoint. The maximum packet size for the endpoint determines
        the number of transactions required to process the IRP.
      * If the size parameter, while sending data to the Host is less than the
        maximum packet size, the transfer will complete in one transaction.
      * If the size parameter, while sending data to the Host is greater
        than the maximum packet size, the IRP will be processed in multiple
        transactions.
      * If the size parameter, while sending data to the Host is equal to or
        an exact multiple of the maximum packet size, the client can optionally
        ask the driver to send a Zero Length Packet(ZLP) by specifying the
        USB_DEVICE_IRP_FLAG_DATA_COMPLETE flag as the flag parameter.
      * The size parameter, while receiving data from the Host must be an
        exact multiple of the maximum packet size of the endpoint. If this is
        not the case, the driver will return a USB_ERROR_IRP_SIZE_INVALID
        result. If while processing the IRP, the driver receives less than
        maximum packet size or a ZLP from the Host, the driver considers the
        IRP as processed. The size parameter at this point contains the actual
        amount of data received from the Host. The IRP status is returned as
        USB_DEVICE_IRP_STATUS_COMPLETED_SHORT.
      * If a ZLP needs to be sent to Host, the IRP size should be specified
        as 0 and the flag parameter should be set as
        USB_DEVICE_IRP_FLAG_DATA_COMPLETE.
      * If the IRP size is an exact multiple of the endpoint size, the client
        can request the driver to not send a ZLP by setting the flag parameter
        to USB_DEVICE_IRP_FLAG_DATA_PENDING. This flag indicates that there is
        more data pending in this transfer.
      * Specifying a size less than the endpoint size along with the
        USB_DEVICE_IRP_FLAG_DATA_PENDING flag will cause the driver to return a
        USB_ERROR_IRP_SIZE_INVALID.
      * If the size is greater than but not a multiple of the endpoint size, and
        the flag is specified as USB_DEVICE_IRP_FLAG_DATA_PENDING, the driver
        will send multiple of endpoint size number of bytes. For example, if the
        IRP size is 130 and the endpoint size if 64, the number of bytes sent
        will 128.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).
    endpointAndDirection -  Specifies the endpoint and direction.
    irp - Pointer to the IRP to be added to the queue for processing.

  Returns:
    * USB_ERROR_NONE - if the IRP was submitted successful.
    * USB_ERROR_IRP_SIZE_INVALID - if the size parameter of the IRP is not
      correct.
    * USB_ERROR_PARAMETER_INVALID - If the client handle is not valid.
    * USB_ERROR_ENDPOINT_NOT_CONFIGURED - If the endpoint is not enabled.
    * USB_ERROR_DEVICE_ENDPOINT_INVALID - The specified endpoint is not valid.
    * USB_ERROR_OSAL_FUNCTION - An OSAL call in the function did not complete
      successfully.

  Example:
    <code>
    // The following code shows an example of how to schedule a IRP to send data
    // from a device to the Host. Assume that the max packet size is 64 and
    // and this data needs to sent over Endpoint 1. In this example, the
    // transfer is processed as three transactions of 64, 64 and 2 bytes.

    USB_ENDPOINT ep;
    USB_DEVICE_IRP irp;

    ep.direction = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 1);

    irp.data = myDataBufferToSend;
    irp.size = 130;
    irp.flags = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;
    irp.callback = MyIRPCompletionCallback;
    irp.referenceData = (uintptr_t)&myDeviceLayerObj;

    if (DRV_USB_UDPHS_DEVICE_IRPSubmit(handle, ep, &irp) != USB_ERROR_NONE)
    {
        // This means there was an error.
    }
    else
    {
        // The status of the IRP can be checked.
        while(irp.status != USB_DEVICE_IRP_STATUS_COMPLETED)
        {
            // Wait or run a task function.
        }
    }

    // The following code shows how the client can request
    // the driver to send a ZLP when the size is an exact multiple of
    // endpoint size.

    irp.data = myDataBufferToSend;
    irp.size = 128;
    irp.flags = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;
    irp.callback = MyIRPCompletionCallback;
    irp.referenceData = (uintptr_t)&myDeviceLayerObj;

    // Note that while  receiving data from the Host, the size should be an
    // exact multiple of the maximum packet size of the endpoint. In the
    // following example, the DRV_USB_UDPHS_DEVICE_IRPSubmit function will return a
    // USB_DEVICE_IRP_SIZE_INVALID value.

    ep = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_HOST_TO_DEVICE, 1);

    irp.data = myDataBufferToSend;
    irp.size = 60; // THIS SIZE IS NOT CORRECT
    irp.flags = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;
    irp.callback = MyIRPCompletionCallback;
    irp.referenceData = (uintptr_t)&myDeviceLayerObj;

    </code>

  Remarks:
    This function can be called from the ISR of the USB module to associated
    with the client.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_IRPSubmit
(
    DRV_HANDLE client,
    USB_ENDPOINT endpointAndDirection,
    USB_DEVICE_IRP * irp
);

// **************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_IRPCancelAll
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
    );

  Summary:
    This function cancels all IRPs that are queued and in progress at the
    specified endpoint.

  Description:
    This function cancels all IRPs that are queued and in progress at the
    specified endpoint.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).
    endpointAndDirection - Specifies the endpoint and direction.

  Returns:
    * USB_ERROR_NONE - The endpoint was successfully enabled.
    * USB_ERROR_DEVICE_ENDPOINT_INVALID - If the endpoint that is being
      accessed is out of the valid endpoint defined for this driver instance.
    * USB_ERROR_PARAMETER_INVALID - The driver handle is not valid.
    * USB_ERROR_OSAL_FUNCTION - An OSAL function called in this function did not
      execute successfully.

  Example:
    <code>
    // This code shows an example of how to cancel all IRPs.

    void MyIRPCallback(USB_DEVICE_IRP * irp)
    {
        // Check if this is setup command

        if(irp->status == USB_DEVICE_IRP_STATUS_SETUP)
        {
            if(IsSetupCommandSupported(irp->data) == false)
            {
                // This means that this setup command is not
                // supported. Stall the some related endpoint and cancel all
                // queue IRPs.

                DRV_USB_UDPHS_DEVICE_EndpointStall(handle, ep);
                DRV_USB_UDPHS_DEVICE_IRPCancelAll(handle, ep);
            }
         }
     }
    </code>

  Remarks:
    None.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_IRPCancelAll
(
    DRV_HANDLE client,
    USB_ENDPOINT endpointAndDirection
);

// **************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_IRPCancel
	(
		DRV_HANDLE client,
		USB_DEVICE_IRP * irp
	)

  Summary:
    This function cancels the specific IRP that are queued and in progress at the
    specified endpoint.

  Description:
    This function attempts to cancel the processing of a queued IRP. An IRP that
    was in the queue but yet to be processed will be canceled successfully and
    the IRP callback function will be called from this function with the
    USB_DEVICE_IRP_STATUS_ABORTED status. The application can release the data
    buffer memory used by the IRP when this callback occurs.  If the IRP was in
    progress (a transaction in on the bus) when the cancel function was called,
    the IRP will be canceled only when an ongoing or the next transaction has
    completed. The IRP callback function will then be called in an interrupt
    context. The application should not release the related data buffer unless
    the IRP callback has occurred.

  Precondition:
    The Client handle should be valid.

  Parameters:
    handle - Client's driver handle (returned from DRV_USB_UDPHS_Open function).
    irp - Pointer to the IRP to cancel.

  Returns:
    * USB_ERROR_NONE - The IRP have been canceled successfully.
    * USB_ERROR_PARAMETER_INVALID - Invalid parameter or the IRP already has
      been aborted or completed
    * USB_ERROR_OSAL_FUNCTION - An OSAL function called in this function did
      not execute successfully.

  Example:
    <code>
    // This code shows an example of how to cancel IRP.  In this example the IRP
    // has been scheduled from a device to the Host.

    USB_ENDPOINT ep;
    USB_DEVICE_IRP irp;

    ep.direction = USB_ENDPOINT_AND_DIRECTION(USB_DATA_DIRECTION_DEVICE_TO_HOST, 1);

    irp.data = myDataBufferToSend;
    irp.size = 130;
    irp.flags = USB_DEVICE_IRP_FLAG_DATA_COMPLETE;
    irp.callback = MyIRPCompletionCallback;
    irp.referenceData = (uintptr_t)&myDeviceLayerObj;

    if (DRV_USB_UDPHS_DEVICE_IRPSubmit(handle, ep, &irp) != USB_ERROR_NONE)
    {
        // This means there was an error.
    }
    else
    {
        // Check the status of the IRP.
        if(irp.status != USB_DEVICE_IRP_STATUS_COMPLETED)
        {
            // Cancel the submitted IRP.
            if (DRV_USB_UDPHS_DEVICE_IRPCancel(handle, &irp) != USB_ERROR_NONE)
            {
                // The IRP Cancel request submission was successful.
                // IRP cancel status will be notified through the callback
                // function.
            }
            else
            {
                // The IRP may have been completed before IRP cancel operation.
                // could start. No callback notification will be generated.
            }
        }
        else
        {
            // The IRP processing must have been completed before IRP cancel was
            // submitted.
        }
    }

    void MyIRPCallback(USB_DEVICE_IRP * irp)
    {
        // Check if the IRP callback is for a Cancel request
        if(irp->status == USB_DEVICE_IRP_STATUS_ABORTED)
        {
            // IRP cancel completed
        }
     }

    </code>

  Remarks:
    The size returned after the ABORT callback will be always 0 regardless of
    the amount of data that has been sent or received. The client should not
    assume any data transaction has happened for an canceled IRP.  If the last
    transaction of the IRP was in progress, the IRP cancel does not have
    any effect. The first transaction of any ongoing IRP cannot be canceled.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_IRPCancel
(
    DRV_HANDLE client,
    USB_DEVICE_IRP * irp
);

// ****************************************************************************
/* Function:
    void DRV_USB_UDPHS_DEVICE_RemoteWakeupStart(DRV_HANDLE handle);

  Summary:
    This function causes the device to start Remote Wakeup Signalling on the
    bus.

  Description:
    This function causes the device to start Remote Wakeup Signalling on the
    bus. This function should be called when the device, presently placed in
    suspend mode by the Host, wants to be wakeup. Note that the device can do
    this only when the Host has enabled the device's Remote Wakeup capability.

  Precondition:
    The handle should be valid.

  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UDPHS_Open function).

  Returns:
    None.

  Example:
    <code>
    DRV_HANDLE handle;

    // If the Host has enabled the Remote Wakeup capability, and if the device
    // is in suspend mode, then start Remote Wakeup signaling.

    if(deviceIsSuspended && deviceRemoteWakeupEnabled)
    {
        DRV_USB_UDPHS_DEVICE_RemoteWakeupStart(handle);
    }
    </code>

  Remarks:
    None.
*/

void DRV_USB_UDPHS_DEVICE_RemoteWakeupStart(DRV_HANDLE handle);

// ****************************************************************************
/* Function:
    void DRV_USB_UDPHS_DEVICE_RemoteWakeupStop(DRV_HANDLE handle);

  Summary:
    This function causes the device to stop the Remote Wakeup Signalling on the
    bus.

  Description:
    This function causes the device to stop Remote Wakeup Signalling on the bus.
    This function should be called after the DRV_USB_UDPHS_DEVICE_RemoteWakeupStart
    function was called to start the Remote Wakeup signaling on the bus.

  Precondition:
    The handle should be valid. The DRV_USB_UDPHS_DEVICE_RemoteWakeupStart function was
    called to start the Remote Wakeup signaling on the bus.

  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UDPHS_Open function).

  Returns:
    None.

  Example:
    <code>
    DRV_HANDLE handle;

    // If the Host has enabled the Remote Wakeup capability, and if the device
    // is in suspend mode, then start Remote Wakeup signaling. Wait for 10
    // milliseconds and then stop the Remote Wakeup signaling

    if(deviceIsSuspended && deviceRemoteWakeupEnabled)
    {
        DRV_USB_UDPHS_DEVICE_RemoteWakeupStart(handle);
        DelayMilliSeconds(10);
        DRV_USB_UDPHS_DEVICE_RemoteWakeupStop(handle);
    }
    </code>

  Remarks:
    This function should be 1 to 15 milliseconds after the
    DRV_USB_UDPHS_DEVICE_RemoteWakeupStart function was called.
*/

void DRV_USB_UDPHS_DEVICE_RemoteWakeupStop(DRV_HANDLE handle);

// ****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_TestModeEnter
    (
        DRV_HANDLE handle,
        USB_TEST_MODE_SELECTORS testMode
    );

  Summary:
    This function enables the specified USB 2.0 Test Mode.

  Description:
    This function causes the device to enter the specified USB 2.0 defined test
    mode. It is called in response to Set Feature command from the host. The
    wValue field of this command specifies the Test Mode to enter. The USB
    module will perform the action identified by the testMode parameter.

  Precondition:
    The handle should be valid.

  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UDPHS_Open function).

    testMode - This parameter identifies the USB 2.0 specification test mode
    (see table 9-7 of the USB 2.0 specification).

  Returns:
    * USB_ERROR_NONE - The function executed successfully.
    * USB_ERROR_PARAMETER_INVALID - The handle or the value of testMode
      parameter is not valid.

  Example:
    <code>
    DRV_HANDLE handle;

    // This code shows how the DRV_USB_UDPHS_DEVICE_TestModeEnter function is
    // called to make the USB device enter the Test_J test mode.

    DRV_USB_UDPHS_DEVICE_TestModeEnter(handle, USB_TEST_MODE_SELECTOR_TEST_J);

    </code>

  Remarks:
    This function should be called only when the USB device has attached to the
    Host at High speed and only in response to the Set Feature command from the
    Host.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_TestModeEnter
(
    DRV_HANDLE handle,
    USB_TEST_MODE_SELECTORS testMode
);

// ****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UDPHS_DEVICE_TestModeExit
    (
        DRV_HANDLE handle,
        USB_TEST_MODE_SELECTORS testMode
    );

  Summary:
    This function disables the specified USB 2.0 Test Mode.

  Description:
    This function causes the device to stop the specified USB 2.0 defined test
    mode. This function can be called after calling the
    DRV_USB_UDPHS_DEVICE_TestModeEnter function to stop the test mode execution.

  Precondition:
    The handle should be valid.

  Parameters:
    handle - Handle to the driver (returned from DRV_USB_UDPHS_Open function).

    testMode - This parameter identifies the USB 2.0 specification test mode
    (see table 9-7 of the USB 2.0 specification).

  Returns:
    * USB_ERROR_NONE - The function executed successfully.
    * USB_ERROR_PARAMETER_INVALID - The handle or the value of testMode
      parameter is not valid.

  Example:
    <code>
    DRV_HANDLE handle;

    // This code shows how the DRV_USB_UDPHS_DEVICE_TestModeEnter function is
    // called to make the USB device enter the Test_J test mode.

    DRV_USB_UDPHS_DEVICE_TestModeEnter(handle, USB_TEST_MODE_SELECTOR_TEST_J);

    // Now the DRV_USB_UDPHS_DEVICE_TestModeExit function is called to stop the
    // Test_J test mode.

    DRV_USB_UDPHS_DEVICE_TestModeExit(handle, USB_TEST_MODE_SELECTOR_TEST_J);

    </code>

  Remarks:
    None.
*/

USB_ERROR DRV_USB_UDPHS_DEVICE_TestModeExit
(
    DRV_HANDLE handle,
    USB_TEST_MODE_SELECTORS testMode
);


void USBHS_Handler(void);
// *****************************************************************************
// *****************************************************************************
// Section: Included Files (continued)
// *****************************************************************************
// *****************************************************************************
/*  The file included below maps the interface definitions above to appropriate
    static implementations, depending on build mode.
*/


#endif