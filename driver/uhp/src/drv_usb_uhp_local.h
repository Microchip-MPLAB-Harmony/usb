/*******************************************************************************
  USB Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_uhp_ehci_local.h

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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "definitions.h"
#include "driver/usb/drv_usb_external_dependencies.h"
#include "driver/usb/uhp/drv_usb_uhp.h"
#include "drv_usb_uhp_variant_mapping.h"
#include "osal/osal.h"

#define DRV_USBHS_HOST_IRP_PER_FRAME_NUMBER 	     5
#define DRV_USB_UHP_HOST_MAXIMUM_ENDPOINTS_NUMBER   USB_HOST_PIPES_NUMBER
#define DRV_USB_UHP_MAX_DMA_CHANNELS                7

#ifndef UHP_EHCI_RAM_ADDR
#define UHP_EHCI_RAM_ADDR                            0xA0100000u
#endif
#ifndef UHP_OHCI_RAM_ADDR
#define UHP_OHCI_RAM_ADDR                            0xA0100000u
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************


/***************************************************
 * This is an intermediate flag that is set by
 * the driver to indicate that a ZLP should be sent
 ***************************************************/
#define USB_DEVICE_IRP_FLAG_SEND_ZLP 0x80


/************************************************
 * Endpoint state enumeration.
 ************************************************/
typedef enum
{
    DRV_USB_UHP_DEVICE_ENDPOINT_STATE_ENABLED = 0x1,
    DRV_USB_UHP_DEVICE_ENDPOINT_STATE_STALLED = 0x2
}
DRV_USB_UHP_DEVICE_ENDPOINT_STATE;

/************************************************
 * Endpoint data structure. This data structure
 * holds the IRP queue and other flags associated
 * with functioning of the endpoint.
 ************************************************/

typedef struct
{
    /* Max packet size for the endpoint */
    uint16_t maxPacketSize;

    /* Endpoint type */
    USB_TRANSFER_TYPE endpointType;

    /* Endpoint state bitmap */
    DRV_USB_UHP_DEVICE_ENDPOINT_STATE endpointState;
}
DRV_USB_UHP_DEVICE_ENDPOINT_OBJ;

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


#define _CC_PRAGMA(x) _Pragma(#x)


#if defined(__ICCARM__)
	#define WEAK __weak
	#define USED __root
	#define CONSTRUCTOR
//	#define SECTION(a) _CC_PRAGMA(location = a)
	#define ALIGNED(a) _CC_PRAGMA(data_alignment = a)
#elif defined(__GNUC__)
	#define WEAK __attribute__((weak))
	#define USED __attribute__((used))
	#define CONSTRUCTOR __attribute__((constructor))
	#define SECTION(a) __attribute__((__section__(a)))
	#define ALIGNED(a) __attribute__((__aligned__(a)))
#else
	#error Unknown compiler!
#endif
/*********************************************
 * This is the local USB Host IRP object
 ********************************************/
typedef struct _USB_HOST_IRP_LOCAL
{
    /* Points to the 8 byte setup command
     * packet in case this is a IRP is 
     * scheduled on a CONTROL pipe. Should
     * be NULL otherwise */
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

    /* Pointer to function to be called
     * when IRP is terminated. Can be 
     * NULL, in which case the function
     * will not be called. */
    void (*callback)(struct _USB_HOST_IRP * irp);

    /****************************************
     * These members of the IRP should not be
     * modified by client
     ****************************************/
    DRV_USB_UHP_HOST_IRP_STATE tempState;
    struct _USB_HOST_IRP_LOCAL * next;
    struct _USB_HOST_IRP_LOCAL * previous;
    DRV_USB_UHP_HOST_PIPE_HANDLE  pipe;
    uint32_t completedBytes;
}
USB_HOST_IRP_LOCAL;

/************************************************
 * This is the Host Pipe Object.
 ************************************************/
typedef struct _DRV_USB_UHP_HOST_PIPE_OBJ
{
    /* This pipe object is in use */
    bool inUse;

    /* The device address */
    uint8_t deviceAddress;

    /* Interval in case this
     * is a Isochronous or
     * an interrupt pipe */
    uint8_t bInterval;

    uint8_t noOfSlots;
    unsigned int startingOffset;

    /* Client that owns this pipe */
    DRV_HANDLE hClient;
    
    /* USB endpoint and direction */
    USB_ENDPOINT endpointAndDirection;
    
    /* USB Endpoint type */
    USB_TRANSFER_TYPE pipeType;

    /* The IRP queue on this pipe */
    USB_HOST_IRP_LOCAL * irpQueueHead;

    /* The NAK counter for the IRP
     * being served on the pipe */
    
    uint32_t nakCounter;

    /* Pipe endpoint size*/

    unsigned int endpointSize;

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

    /* Host Pipe allocated*/
    uint8_t hostPipeN;
}
DRV_USB_UHP_HOST_PIPE_OBJ;

/*********************************************
 * Host Transfer Group. This data structures
 * contains all pipes belonging to one tranfer
 * type.
 *********************************************/

typedef struct _DRV_USB_UHP_HOST_TRANSFER_GROUP
{
    /* The current pipe being serviced
     * in this transfer group */
    DRV_USB_UHP_HOST_PIPE_OBJ * currentPipe;
    
    /* The current IRP being serviced
     * in the pipe */
    void * currentIRP;

    /* Total number of pipes in this
     * transfer group */
    uint32_t nPipes;

    /* Transfer management */
    uint32_t int_on_async_advance;
    
    /* Are the interrupt enable or not during transfer ? */
    uint32_t interruptWasEnabled;
}
DRV_USB_UHP_HOST_TRANSFER_GROUP;


/**********************************************
 * Host Endpoint Object.
 *********************************************/

typedef struct
{
    /* Indicates this endpoint is in use */
    bool inUse;
    DRV_USB_UHP_HOST_PIPE_OBJ * pipe;

}_DRV_USB_UHP_HOST_ENDPOINT;


typedef struct
{
    /* A combination of two structures for
     * each direction. */

    _DRV_USB_UHP_HOST_ENDPOINT endpoint;

}DRV_USB_UHP_HOST_ENDPOINT_OBJ;


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

typedef struct _DRV_USB_UHP_DMA_POOL_STRUCT
{
    bool inUse;
    bool endpointDir;
    uint8_t iEndpoint;
    unsigned int count;

} DRV_USB_UHP_DMA_POOL;

/*********************************************
 * Host Mode Device Attach Detach State
 ********************************************/
typedef enum
{
    /* No device is attached */
    DRV_USB_UHP_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH = 0,

    /* Waiting for debounce delay */
    DRV_USB_UHP_HOST_ATTACH_STATE_DETECTED,

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

    /* Start the reset signalling */
    DRV_USB_UHP_HOST_RESET_STATE_START,

    /* Check if reset duration is done and stop reset */
    DRV_USB_UHP_HOST_RESET_STATE_WAIT_FOR_COMPLETE,

    DRV_USB_UHP_HOST_RESET_STATE_COMPLETE,
    DRV_USB_UHP_HOST_RESET_STATE_COMPLETE2

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

    DRV_USB_UHP_DMA_POOL gDrvUSBDMAPool[DRV_USB_UHP_MAX_DMA_CHANNELS + 1];

    /* Status of this driver instance */
    SYS_STATUS status;     

    /* Contains the consumed FIFO size */
    unsigned int consumedFIFOSize;

    /* The USB peripheral associated with the object */
    volatile uhphs_registers_t * usbID;

    /* Interrupt source for USB module */
    INT_SOURCE interruptSource;

    /* Mutex create function place holder*/
    OSAL_MUTEX_DECLARE (mutexID);

    /* Pointer to the endpoint table */
    DRV_USB_UHP_DEVICE_ENDPOINT_OBJ *endpointTable;

    /* Pointer to the endpoint table */
    DRV_USB_UHP_HOST_ENDPOINT_OBJ hostEndpointTable[DRV_USB_UHP_HOST_MAXIMUM_ENDPOINTS_NUMBER];

    /* The object is current in an interrupt context */
    bool isInInterruptContext;

    /* Maintains the timer count value for host */
    uint32_t timerCount;

    /* Root Hub Port 0 attached device speed in host mode
     * In device mode, the speed at which the device attached */
    USB_SPEED deviceSpeed;

    /* Transfer Groups */
    DRV_USB_UHP_HOST_TRANSFER_GROUP controlTransferGroup;

    /* Tracks if the current control transfer is a zero length */
    bool isZeroLengthControlTransfer;

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

    /* True if the Host Controller sets this bit to 1 on the completion of a USB transaction */
    volatile uint8_t int_xfr_qtd_complete;
    volatile uint8_t hostPipeInUse;
    
    uint8_t staticDToggleIn;
    uint8_t staticDToggleOut;

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

extern void _DRV_USB_UHP_HOST_AttachDetachStateMachine (DRV_USB_UHP_OBJ * hDriver);
extern void _DRV_USB_UHP_HOST_ResetStateMachine(DRV_USB_UHP_OBJ * hDriver);
extern void _DRV_USB_UHP_DEVICE_Initialize(DRV_USB_UHP_OBJ * drvObj, SYS_MODULE_INDEX index);
extern void _DRV_USB_UHP_DEVICE_Tasks_ISR(DRV_USB_UHP_OBJ * hDriver);
extern void _DRV_USB_UHP_DEVICE_Tasks_ISR_USBDMA(DRV_USB_UHP_OBJ * hDriver);
extern void _DRV_USB_UHP_HOST_Initialize(DRV_USB_UHP_OBJ * drvObj, SYS_MODULE_INDEX index);
extern void _DRV_USB_UHP_HOST_Tasks_ISR(DRV_USB_UHP_OBJ * hDriver);
extern void _DRV_USB_UHP_HOST_Tasks_ISR_USBDMA(DRV_USB_UHP_OBJ * hDriver);
extern uint8_t _DRV_USB_UHP_DEVICE_Get_FreeDMAChannel
(
    DRV_USB_UHP_OBJ * hDriver,
    bool endpointDir,
    uint8_t iEndpoint
);

#endif  // _DRV_USB_UHP_LOCAL_H
