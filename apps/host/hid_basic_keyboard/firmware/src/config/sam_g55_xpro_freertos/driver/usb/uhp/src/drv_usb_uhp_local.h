/*******************************************************************************
  USB Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_uhp_local.h

  Summary:
    USB driver local declarations and definitions

  Description:
    This file contains the USB driver's local declarations and definitions.
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

#ifndef _DRV_USB_UHP_LOCAL_H
#define _DRV_USB_UHP_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"
#include "driver/usb/drv_usb_external_dependencies.h"
#include "drv_usb_uhp_variant_mapping.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************


/*********************************************
 * These IRP states are used internally by the
 * HCD to track completion of a host IRP. This
 * is not the same as the public IRP status
 *********************************************/
typedef enum
{
    DRV_USB_UHP_HOST_IRP_STATE_ABORTED,
    DRV_USB_UHP_HOST_IRP_STATE_PROCESSING,
    DRV_USB_UHP_HOST_IRP_STATE_DEFAULT
}
DRV_USB_UHP_HOST_IRP_STATE;


/*********************************************
 * This is the local USB Host IRP object
 ********************************************/
typedef struct _USB_HOST_IRP_LOCAL
{
    /* Points to the 8 byte setup command packet in case this is a IRP is
     * scheduled on a CONTROL pipe. Should be NULL otherwise */
    void * setup;

    /* Pointer to data buffer */
    void * data;

    /* Size of the data buffer */
    uint32_t size;

    /* Status of the IRP */
    USB_HOST_IRP_STATUS status;

    /* Request specific flags */
    USB_HOST_IRP_FLAG flags;

    /* User data */
    uint32_t userData;

    /* Pointer to function to be called when IRP is terminated. Can be NULL, in
     * which case the function will not be called. */
    void (*callback)(struct _USB_HOST_IRP * irp);

    /****************************************
     * These members of the IRP should not be
     * modified by client
     ****************************************/
    DRV_USB_UHP_HOST_IRP_STATE tempState;
    uint32_t completedBytes;
    struct _USB_HOST_IRP_LOCAL * next;
    struct _USB_HOST_IRP_LOCAL * previous;
    DRV_USB_UHP_HOST_PIPE_HANDLE  pipe;

} USB_HOST_IRP_LOCAL;

/************************************************
 * This is the Host Pipe Object.
 ************************************************/
typedef struct _DRV_USB_UHP_HOST_PIPE_OBJ
{
    /* This pipe object is in use */
    bool inUse;

    /* The device address */
    uint8_t deviceAddress;

    /* Interval in case this is a Isochronous or an interrupt pipe */
    uint8_t bInterval;

    /* Client that owns this pipe */
    DRV_HANDLE hClient;

    /* USB endpoint and direction */
    USB_ENDPOINT endpointAndDirection;

    volatile uint8_t intXfrQtdComplete;

    volatile uint8_t staticDToggleIn;
    volatile uint8_t staticDToggleOut;

    /* USB Endpoint type */
    USB_TRANSFER_TYPE pipeType;

    /* The IRP queue on this pipe */
    USB_HOST_IRP_LOCAL * irpQueueHead;

    /* Pipe endpoint size*/
    uint32_t endpointSize;

    /* The next and previous pipe */
    struct _DRV_USB_UHP_HOST_PIPE_OBJ * next;
    struct _DRV_USB_UHP_HOST_PIPE_OBJ * previous;

    /* Interval counter */
    uint8_t intervalCounter;

    /* Pipe Speed */
    USB_SPEED speed;

    /* Hub Address */
    uint8_t hubAddress;

    /* Hub Port*/
    uint8_t hubPort;

    /* Host endpoint */
    uint8_t hostEndpoint;
}
DRV_USB_UHP_HOST_PIPE_OBJ;

/*********************************************
 * Driver NON ISR Tasks routine states.
 *********************************************/

typedef enum
{
    /* Driver checks if the UTMI clock is usable */
    DRV_USB_UHP_TASK_STATE_WAIT_FOR_CLOCK_USABLE = 1,

    /* Driver initializes the required operation mode  */
    DRV_USB_UHP_TASK_STATE_INITIALIZE_OPERATION_MODE,

    /* Driver has complete initialization and can be opened */
    DRV_USB_UHP_TASK_STATE_RUNNING

} DRV_USB_UHP_TASK_STATE;


/*********************************************
 * Host Mode Device Attach Detach State
 ********************************************/
typedef enum
{
    /* No device is attached */
    DRV_USB_UHP_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH = 0,

    /* Waiting for debounce delay */
    DRV_USB_UHP_HOST_ATTACH_STATE_DETECTED,
    DRV_USB_UHP_HOST_ATTACH_STATE_DETECTED_DEBOUNCE,

    /* Debouncing is complete. Device is attached */
    DRV_USB_UHP_HOST_ATTACH_STATE_READY,

} DRV_USB_UHP_HOST_ATTACH_STATE;

/*********************************************
 * Host Mode Device Reset state
 *********************************************/
typedef enum
{
    /* No Reset in progress */
    DRV_USB_UHP_HOST_RESET_STATE_NO_RESET = 0,

    /* Start the reset signaling */
    DRV_USB_UHP_HOST_RESET_STATE_START,
    DRV_USB_UHP_HOST_RESET_STATE_START_DELAYED,

    /* Check if reset duration is done and stop reset */
    DRV_USB_UHP_HOST_RESET_STATE_WAIT_FOR_COMPLETE,

    DRV_USB_UHP_HOST_RESET_STATE_COMPLETE,
    DRV_USB_UHP_HOST_RESET_STATE_OHCI_RESET_START,
    DRV_USB_UHP_HOST_RESET_STATE_OHCI_WAIT_FOR_COMPLETE

} DRV_USB_UHP_HOST_RESET_STATE;

/**************************************
 * Root Hub parameters
 **************************************/

typedef struct
{
    /* Pointer to the port over current detect function */
    DRV_USB_UHP_ROOT_HUB_PORT_OVER_CURRENT_DETECT portOverCurrentDetect;

    /* Pointer to the port indication function */
    DRV_USB_UHP_ROOT_HUB_PORT_INDICATION portIndication;

    /* Pointer to the port enable function */
    DRV_USB_UHP_ROOT_HUB_PORT_POWER_ENABLE portPowerEnable;

    /* Total current available */
    uint32_t rootHubAvailableCurrent;

} DRV_USB_UHP_HOST_ROOT_HUB_INFO;


/***********************************************
 * Driver object structure. One object per
 * hardware instance
 **********************************************/

typedef struct _DRV_USB_UHP_OBJ_STRUCT
{
    /* Indicates this object is in use */
    bool inUse;

    /* Indicates that the client has opened the instance */
    bool isOpened;

    /* Status of this driver instance */
    SYS_STATUS status;

    /* The USB peripheral associated with the object */
    volatile UhpOhci * usbIDOHCI;

    /* Interrupt source for USB module */
    INT_SOURCE interruptSource;

    /* Mutex create function place holder*/
    OSAL_MUTEX_DECLARE (mutexID);

    /* The object is current in an interrupt context */
    bool isInInterruptContext;

    /* Root Hub Port 0 attached device speed in host mode
     * In device mode, the speed at which the device attached */
    USB_SPEED deviceSpeed;

    /* Non ISR Task Routine state */
    DRV_USB_UHP_TASK_STATE state;

    /* Client data that will be returned at callback */
    uintptr_t  hClientArg;

    /* Call back function for this client */
    DRV_USB_EVENT_CALLBACK  pEventCallBack;

    /* Sent session invalid event to the client */
    bool sessionInvalidEventSent;

    /* Attach state of the device */
    DRV_USB_UHP_HOST_ATTACH_STATE attachState;

    /* True if device is attached */
    volatile bool deviceAttached;

    uint8_t portNumber;

    volatile uint8_t blockPipe;

    /* Device Object handle assigned by the host */
    USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo;

     /* Device object handle of the current attached device. */
    USB_HOST_DEVICE_OBJ_HANDLE attachedDeviceObjHandle;

    /* This is needed to track if the host is generating reset signal */
    bool isResetting;

    /* This counts the reset signal duration */
    DRV_USB_UHP_HOST_RESET_STATE resetState;

    /* Timer handle for reset */
    SYS_TIME_HANDLE timerHandle;

    /* This counts the reset signal duration */
    DRV_USB_UHP_HOST_ROOT_HUB_INFO rootHubInfo;

    /* True if Root Hub Operation is enabled */
    bool operationEnabled;

} DRV_USB_UHP_OBJ;

extern void DRV_USB_UHP_AttachDetachStateMachine (DRV_USB_UHP_OBJ * hDriver);
extern void DRV_USB_UHP_ResetStateMachine(DRV_USB_UHP_OBJ * hDriver);
extern void DRV_USB_UHP_HostInitialize(DRV_USB_UHP_OBJ * drvObj, SYS_MODULE_INDEX index);
extern void DRV_USB_UHP_TransferProcess(DRV_USB_UHP_OBJ *hDriver, uint8_t hostPipe);
extern USB_SPEED DRV_USB_UHP_DeviceCurrentSpeedGet(DRV_HANDLE client);

extern DRV_USB_UHP_HOST_PIPE_OBJ gDrvUSBHostPipeObj[DRV_USB_UHP_PIPES_NUMBER];
extern uint8_t USBBufferNoCache[DRV_USB_UHP_PIPES_NUMBER][DRV_USB_UHP_NO_CACHE_BUFFER_LENGTH];
extern uint8_t USBBufferNoCacheSetup[DRV_USB_UHP_PIPES_NUMBER][8];

#endif  // _DRV_USB_UHP_LOCAL_
