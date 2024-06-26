/*******************************************************************************
  USB Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_local.h

  Summary:
    USB driver local declarations and definitions

  Description:
    This file contains the USB driver's local declarations and definitions.
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

#ifndef DRV_USBFSV1_LOCAL_H
#define DRV_USBFSV1_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "driver/usb/drv_usb_external_dependencies.h"
#include "driver/usb/usbfsv1/drv_usbfsv1.h"
#include "driver/usb/usbfsv1/src/drv_usbfsv1_variant_mapping.h"
#include "osal/osal.h"

/* MISRA C-2012 Rule 5.1, 5.2, 5.4 and 8.6 deviated below. Deviation record ID -  
    H3_USB_MISRAC_2012_R_5_2_DR_1, H3_USB_MISRAC_2012_R_5_4_DR_1 and H3_USB_MISRAC_2012_R_8_6_DR_1*/
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:4 "MISRA C-2012 Rule 5.1" "H3_USB_MISRAC_2012_R_5_1_DR_1" )\
(deviate:13 "MISRA C-2012 Rule 5.2" "H3_USB_MISRAC_2012_R_5_2_DR_1" )\
(deviate:1 "MISRA C-2012 Rule 5.4" "H3_USB_MISRAC_2012_R_5_4_DR_1" )\
(deviate:4 "MISRA C-2012 Rule 8.6" "H3_USB_MISRAC_2012_R_8_6_DR_1" )
</#if>


#define COMPILER_WORD_ALIGNED                               __attribute__((__aligned__(4)))

#define DRV_USBFSV1_HOST_MAXIMUM_ENDPOINTS_NUMBER           10
#define DRV_USBFSV1_HOST_IRP_PER_FRAME_NUMBER               10U
#define M_DRV_USBFSV1_SW_EP_NUMBER                           DRV_USBFSV1_HOST_IRP_PER_FRAME_NUMBER
#define DRV_USBFSV1_POST_DETACH_DELAY                       2000

#define DRV_USBFSV1_MAX_CONTROL_BANDWIDTH_FULL_SPEED      20
#define DRV_USBFSV1_MAX_CONTROL_BANDWIDTH_LOW_SPEED       30
#define DRV_USBFSV1_MAX_BANDWIDTH_PER_FRAME    70
/* Number of Endpoints used */
#define DRV_USBFSV1_ENDPOINT_NUMBER_MASK                    0x0FU
#define DRV_USBFSV1_ENDPOINT_DIRECTION_MASK                 0x80U

#define DRV_USBFSV1_AUTO_ZLP_ENABLE                         false

/* Macro to define number of USB Device descriptor banks */
#ifndef USB_DEVICE_DESC_BANK_NUMBER
#define USB_DEVICE_DESC_BANK_NUMBER                         DEVICE_DESC_BANK_NUMBER
#endif

/* Macro to define number of USB Host Pipe */
#ifndef USB_HOST_PIPE_NUMBER
#define USB_HOST_PIPE_NUMBER                                HOST_PIPE_NUMBER
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************
/***********************************************
 * Hardware Pipe Transfer Type Assignments
 ***********************************************/

#define DRV_USBFSV1_HOST_HW_PIPE_CONTROL 0
#define DRV_USBFSV1_HOST_HW_PIPE_ISOC_IN 1
#define DRV_USBFSV1_HOST_HW_PIPE_ISOC_OUT 2
#define DRV_USBFSV1_HOST_HW_PIPE_INT_IN 3
#define DRV_USBFSV1_HOST_HW_PIPE_INT_OUT 4
#define DRV_USBFSV1_HOST_HW_PIPE_BULK_IN 5
#define DRV_USBFSV1_HOST_HW_PIPE_BULK_OUT 6

/************************************************
 * Pipe Token Type
 ************************************************/
#define    DRV_USBFSV1_HOST_PIPE_TOKEN_SETUP  0x00
#define    DRV_USBFSV1_HOST_PIPE_TOKEN_IN     0x01
#define    DRV_USBFSV1_HOST_PIPE_TOKEN_OUT    0x02

/************************************************
 * Pipe Transfer Type
 ************************************************/
    
#define DRV_USBFSV1_HOST_PIPE_TYPE_DISABLED   0x00
#define DRV_USBFSV1_HOST_PIPE_TYPE_CONTROL    0x08
#define DRV_USBFSV1_HOST_PIPE_TYPE_ISOC       0x10
#define DRV_USBFSV1_HOST_PIPE_TYPE_BULK       0x18
#define DRV_USBFSV1_HOST_PIPE_TYPE_INTERRUPT  0x20

/************************************************
 * Bank Type
 ************************************************/

#define DRV_USBFSV1_HOST_PIPE_BANK_SINGLE 0x0
#define DRV_USBFSV1_HOST_PIPE_BANK_DUAL 0x4



/***************************************************
 * This is an intermediate flag that is set by
 * the driver to indicate that a ZLP should be sent
 ***************************************************/
#define USB_DEVICE_IRP_FLAG_SEND_ZLP 0x80U

#if DRV_USBFSV1_AUTO_ZLP_ENABLE == true
#define M_DRV_USBFSV1_DEVICE_AutoZlpControl(epNumber, bankNumber) hDriver->endpointDescriptorTable[epNumber].DEVICE_DESC_BANK[bankNumber].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_AUTO_ZLP_Msk;

#else 
#define M_DRV_USBFSV1_DEVICE_AutoZlpControl(epNumber, bankNumber) hDriver->endpointDescriptorTable[epNumber].DEVICE_DESC_BANK[bankNumber].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_AUTO_ZLP_Msk;
#endif 
/***************************************************
 * This object is used by the driver as IRP place
 * holder along with queuing feature.
 ***************************************************/
typedef struct S_DRV_USBFSV1_DEVICE_IRP_LOCAL
{
    /* Pointer to the data buffer */
    void * data;

    /* Size of the data buffer */
    unsigned int size;

    /* Status of the IRP */
    USB_DEVICE_IRP_STATUS status;

    /* IRP Callback. If this is NULL,
     * then there is no callback generated */
    void (*callback)(struct S_USB_DEVICE_IRP * irp);

    /* Request specific flags */
    USB_DEVICE_IRP_FLAG flags;

    /* User data */
    uintptr_t userData;

    /* This points to the next IRP in the queue */
    struct S_DRV_USBFSV1_DEVICE_IRP_LOCAL * next;

    /* This points to the previous IRP in the queue */
    struct S_DRV_USBFSV1_DEVICE_IRP_LOCAL * previous;

    /* Pending bytes in the IRP */
    uint32_t nPendingBytes;

}
DRV_USBFSV1_DEVICE_IRP_LOCAL;

/************************************************
 * Endpoint state enumeration.
 ************************************************/

typedef enum
{
    DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED = 0x1,
    DRV_USBFSV1_DEVICE_ENDPOINT_STATE_STALLED = 0x2
}
DRV_USBFSV1_DEVICE_ENDPOINT_STATE;

/************************************************
 * Endpoint data structure. This data structure
 * holds the IRP queue and other flags associated
 * with functioning of the endpoint.
 ************************************************/

typedef struct
{
    /* This is the IRP queue for
     * the endpoint */
    DRV_USBFSV1_DEVICE_IRP_LOCAL * irpQueue;

    /* Max packet size for the endpoint */
    uint16_t maxPacketSize;

    /* Endpoint type */
    USB_TRANSFER_TYPE endpointType;

    /* Endpoint state bitmap */
    DRV_USBFSV1_DEVICE_ENDPOINT_STATE endpointState;

}
DRV_USBFSV1_DEVICE_ENDPOINT_OBJ;

/*********************************************
 * These IRP states are used internally by the
 * HCD to track completion of a host IRP. This
 * is not the same as the public IRP status
 *********************************************/
typedef enum
{
    DRV_USBFSV1_HOST_IRP_STATE_SETUP_STAGE,
    DRV_USBFSV1_HOST_IRP_STATE_SETUP_TOKEN_SENT,
    DRV_USBFSV1_HOST_IRP_STATE_DATA_STAGE,
    DRV_USBFSV1_HOST_IRP_STATE_DATA_STAGE_SENT,
    DRV_USBFSV1_HOST_IRP_STATE_HANDSHAKE,
    DRV_USBFSV1_HOST_IRP_STATE_HANDSHAKE_SENT,
    DRV_USBFSV1_HOST_IRP_STATE_COMPLETE,
    DRV_USBFSV1_HOST_IRP_STATE_ABORTED,
    DRV_USBFSV1_HOST_IRP_STATE_PROCESSING
}
DRV_USBFSV1_HOST_IRP_STATE;

/*********************************************
 * This is the local USB Host IRP object
 ********************************************/
typedef struct S_DRV_USBFSV1_HOST_IRP_LOCAL
{
    /* Points to the 8 byte setup command
     * packet in case this is a IRP is
     * scheduled on a CONTROL pipe. Should
     * be NULL otherwise */
    void * setup;

    /* Pointer to data buffer */
    void * data;

    /* Size of the data buffer */
    unsigned int size;

    /* Status of the IRP */
    USB_HOST_IRP_STATUS status;

    /* Request specific flags */
    USB_HOST_IRP_FLAG flags;

    /* User data */
    uintptr_t userData;

    /* Pointer to function to be called
     * when IRP is terminated. Can be
     * NULL, in which case the function
     * will not be called. */
    void (*callback)(struct S_USB_HOST_IRP * irp);

    /****************************************
     * These members of the IRP should not be
     * modified by client
     ****************************************/

    uint32_t tempSize;
    DRV_USBFSV1_HOST_IRP_STATE tempState;
    uint32_t completedBytes;
    uint32_t completedBytesInThisFrame;
    struct S_DRV_USBFSV1_HOST_IRP_LOCAL * next;
    DRV_USB_HOST_PIPE_HANDLE  pipe;

}
DRV_USBFSV1_HOST_IRP_LOCAL;

/************************************************
 * This is the Host Pipe Object.
 ************************************************/
typedef struct S_DRV_USBFSV1_HOST_PIPE_OBJ
{
    /* This pipe object is in use */
    bool inUse;

    /* Client that owns this pipe */
    DRV_HANDLE hClient;

    /* USB endpoint and direction */
    USB_ENDPOINT endpointAndDirection;

    /* USB Endpoint type */
    USB_TRANSFER_TYPE pipeType;

    /* The IRP queue on this pipe */
    DRV_USBFSV1_HOST_IRP_LOCAL * irpQueueHead;

    /* The data toggle for this pipe*/
    bool dataToggle;

    /* The NAK counter for the IRP
     * being served on the pipe */

    uint32_t nakCounter;

    /* Pipe endpoint size*/

    unsigned int endpointSize;

    /* The next pipe */
    struct S_DRV_USBFSV1_HOST_PIPE_OBJ * next;
    struct S_DRV_USBFSV1_HOST_PIPE_OBJ * previous;
    /* Interval in case this
     * is a Isochronous or
     * an interrupt pipe */
    uint8_t bInterval;

    /* The device address */
    uint8_t deviceAddress;

    /* Interval counter */

    uint8_t intervalCounter;

    /* Bandwidth */
    uint8_t bwPerTransaction;

    /* Pipe Speed */
    USB_SPEED speed;

    /* Hub Address */
    uint8_t hubAddress;

    /* Hub Port*/
    uint8_t hubPort;

    /* Index of the pipe group to which this pipe belongs*/
    uint8_t pipeGroupIndex;
    
    /* Pipe PCKSIZE field*/
    uint8_t pckSize;

}
DRV_USBFSV1_HOST_PIPE_OBJ;

/************************************************
 * This is the Host SOF Frame IRP tracker.
 ************************************************/
typedef struct
{
    /* Tracks the HW pipe type to which this IRP belongs */
    int hardwarePipeType;

    /* The pointer to the IRP to be processed */
    DRV_USBFSV1_HOST_IRP_LOCAL * pIRP;

} DRV_USBFSV1_HOST_FRAME_IRP;

/*********************************************
 * Host Transfer Group. This data structures
 * contains all pipes belonging to one transfer
 * type.
 *********************************************/

typedef struct 
{
    /* The first pipe in this transfer
     * group. This will link to other pipes */
    DRV_USBFSV1_HOST_PIPE_OBJ * pipe;

    /* The current pipe being serviced
     * in this pipe group */
    DRV_USBFSV1_HOST_PIPE_OBJ * currentPipe;

    /* The current IRP being serviced
     * in this pipe group */
    void * currentIRP;

    /* Total number of pipes in this
     * pipe group */
    int nPipes;

} DRV_USBFSV1_HOST_PIPE_GROUP;

/********************************************
 * This enumeration list the possible status
 * value of a token once it has completed.
 ********************************************/

typedef enum
{
    USB_TRANSACTION_ACK             = 0x2,
    USB_TRANSACTION_NAK             = 0xA,
    USB_TRANSACTION_STALL           = 0xE,
    USB_TRANSACTION_DATA0           = 0x3,
    USB_TRANSACTION_DATA1           = 0xB,
    USB_TRANSACTION_BUS_TIME_OUT    = 0x1,
    USB_TRANSACTION_DATA_ERROR      = 0xF

}
DRV_USBFSV1_TRANSACTION_RESULT;

/***********************************************
 * USB Driver flags. Binary flags needed to
 * track different states of the USB driver.
 ***********************************************/
typedef enum
{

    /* Driver Host Mode operation has been enabled */
    DRV_USBFSV1_FLAG_HOST_MODE_ENABLED = /*DOM-IGNORE-BEGIN*/0x10/*DOM-IGNORE-END*/,


} DRV_USBFSV1_FLAGS;

/**************************************
 * Root Hub parameters
 **************************************/

typedef struct
{
    /* Pointer to the port over current detect function */
    DRV_USBFSV1_ROOT_HUB_PORT_OVER_CURRENT_DETECT portOverCurrentDetect;

    /* Pointer to the port indication function */
    DRV_USBFSV1_ROOT_HUB_PORT_INDICATION portIndication;

    /* Pointer to the port enable function */
    DRV_USBFSV1_ROOT_HUB_PORT_POWER_ENABLE portPowerEnable;

    /* Total current available */
    uint32_t rootHubAvailableCurrent;
}
DRV_USBFSV1_HOST_ROOT_HUB_INFO;

/**********************************************
 * Host Endpoint Object.
 *********************************************/

typedef struct
{
    /* Indicates this endpoint is in use */
    bool inUse;
    DRV_USBFSV1_HOST_PIPE_OBJ * pipe;

}S_DRV_USBFSV1_HOST_ENDPOINT;


typedef struct
{
    /* A combination of two structures for
     * each direction. */

    S_DRV_USBFSV1_HOST_ENDPOINT endpoint;

}DRV_USBFSV1_HOST_ENDPOINT_OBJ;


/*********************************************
 * Host Mode Device Attach Detach State
 ********************************************/
typedef enum
{
    /* No device is attached */
    DRV_USBFSV1_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH = 0,

    /* Start debounce delay */
    DRV_USBFSV1_HOST_ATTACH_STATE_DEBOUNCING,

    /* Wait for the debounce to complete */
    DRV_USBFSV1_HOST_ATTACH_STATE_DEBOUNCING_WAIT,

    /* Debouncing is complete. Device is attached */
    DRV_USBFSV1_HOST_ATTACH_STATE_IDLE,

} DRV_USBFSV1_HOST_ATTACH_STATE;

typedef enum
{
    DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST,
    DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_SETUP_IRP_FROM_CLIENT,

    DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST,
    DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_DATA_IRP_FROM_CLIENT,
    DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_IRP_FROM_CLIENT,
    DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE,

    DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT,
    DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT,
    DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE,
    DRV_USBFSV1_DEVICE_EP0_STATE_TX_DATA_STAGE_IN_PROGRESS
}
DRV_USBFSV1_DEVICE_EP0_STATE;

/***********************************************
 * Driver object structure. One object per
 * hardware instance
 **********************************************/

typedef struct S_DRV_USBFSV1_OBJ_STRUCT
{
    M_DRV_USBFSV1_FOR_HOST(uint32_t, hostTransactionBuffer[16]);
    
    /* Indicates this object is in use */
    bool inUse;

    /* Set if the driver is opened */
    bool isOpened;

    /* Set if device if D+ pull up is enabled. */
    bool deviceAttached;

    /* The object is current in an interrupt context */
    bool isInInterruptContext;

    /* Sent session invalid event to the client */
    bool sessionInvalidEventSent;

    /* Set if valid VBUS was detected in device mode */
    bool vbusIsValid;

    /* Set if device if D+ pull up is enabled. */
    bool isAttached;

    /* Set if the device is suspended */
    bool isSuspended;

    /* Set if device if D+ pull up is enabled. */
    bool operationEnabled;

    /* Current operating mode of the driver */
    DRV_USBFSV1_OPMODES operationMode;

    /* Interrupt source for USB module */
    INT_SOURCE interruptSource;

    /* Interrupt source for USB module */
    INT_SOURCE interruptSource1;

    /* Interrupt source for USB module */
    INT_SOURCE interruptSource2;

    /* Interrupt source for USB module */
    INT_SOURCE interruptSource3;

    /* Mutex create function */
    OSAL_MUTEX_DECLARE(mutexID);

    /* Pointer to the endpoint 0 Buffers */
    uint8_t * endpoint0BufferPtr[USB_DEVICE_DESC_BANK_NUMBER];

    /* Next Ping Pong state */
    uint32_t rxEndpointsNextPingPong;
    uint32_t txEndpointsNextPingPong;

    /* Status of this driver instance */
    SYS_STATUS status;

    /* Contains the EPO state */
    DRV_USBFSV1_DEVICE_EP0_STATE endpoint0State;

    /* The USB peripheral associated with
     * the object */
    usb_registers_t * usbID;

    /* Attach state of the device */
    DRV_USBFSV1_HOST_ATTACH_STATE attachState;
    
    /* Pointer to the endpoint table */
    usb_descriptor_host_registers_t  * hostPipeDescTable;

    /* Root Hub Port 0 attached device speed in host mode
     * In device mode, the speed at which the device attached */
    USB_SPEED deviceSpeed;

    /* Client data that will be returned at callback */
    uintptr_t  hClientArg;

    /* Call back function for this client */
    DRV_USB_EVENT_CALLBACK  pEventCallBack;

    /* Current VBUS level when running in device mode */
    DRV_USB_VBUS_LEVEL vbusLevel;

    /* Callback to determine the Vbus level */
    DRV_USBFSV1_VBUS_COMPARATOR vbusComparator;

    /* This is array of device endpoint objects pointers */
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * deviceEndpointObj[DRV_USBFSV1_ENDPOINTS_NUMBER];

    /* Pointer to the endpoint table */
    DRV_USBFSV1_ENDPOINT_DESC_TABLE endpointDescriptorTable[DRV_USBFSV1_ENDPOINTS_NUMBER];

    /* Driver flags to indicate different things */
    DRV_USBFSV1_FLAGS driverFlags;

    /* Pipe Groups */
    DRV_USBFSV1_HOST_PIPE_GROUP pipeGroup[7];

    /* This counts the reset signal duration */
    DRV_USBFSV1_HOST_ROOT_HUB_INFO rootHubInfo;

    /* The SWEPBuffer index */
    M_DRV_USBFSV1_FOR_HOST(uint8_t, currentFrameIRPIndex);

    /* Placeholder for bandwidth consumed in frame */
    M_DRV_USBFSV1_FOR_HOST(uint8_t, globalBWConsumed);

    /* Variable used SW Endpoint objects that is used by this HW instances for
     * USB transfer scheduling */
    M_DRV_USBFSV1_FOR_HOST(DRV_USBFSV1_HOST_FRAME_IRP, frameIRPList[DRV_USBFSV1_HOST_IRP_PER_FRAME_NUMBER]);

    /* This flag is true if an detach event has come and device de-enumeration
     * operation is in progress  */
    M_DRV_USBFSV1_FOR_HOST(bool, isDeviceDeenumerating);

    /* This is the pointer to host Pipe descriptor table */
    M_DRV_USBFSV1_FOR_HOST(usb_descriptor_host_registers_t *, hostEndpointTablePtr);

    /* The parent UHD assigned by the host */
    M_DRV_USBFSV1_FOR_HOST(USB_HOST_DEVICE_OBJ_HANDLE, usbHostDeviceInfo);

    /* The UHD of the device attached to port assigned by the host */
    M_DRV_USBFSV1_FOR_HOST(USB_HOST_DEVICE_OBJ_HANDLE, attachedDeviceObjHandle);

    M_DRV_USBFSV1_FOR_HOST(SYS_TIME_HANDLE, timerHandle);
    
    M_DRV_USBFSV1_FOR_HOST(bool, isResetting);

} DRV_USBFSV1_OBJ;

/****************************************
 * The driver object
 ****************************************/
extern DRV_USBFSV1_OBJ gDrvUSBObj[];

/**************************************
 * Local functions.
 *************************************/

void F_DRV_USBFSV1_DEVICE_Initialize(DRV_USBFSV1_OBJ * drvObj, SYS_MODULE_INDEX index);

void F_DRV_USBFSV1_DEVICE_Tasks_ISR(DRV_USBFSV1_OBJ * hDriver);

void F_DRV_USBFSV1_HOST_Initialize
(
    DRV_USBFSV1_OBJ * drvObj,
    SYS_MODULE_INDEX index,
    DRV_USBFSV1_INIT * usbInit
);

void F_DRV_USBFSV1_HOST_Tasks_ISR(DRV_USBFSV1_OBJ * hDriver);

void F_DRV_USBFSV1_HOST_AttachDetachStateMachine (DRV_USBFSV1_OBJ * hDriver);

void F_DRV_USBFSV1_HOST_CreateFrameIRPList(DRV_USBFSV1_OBJ * hDriver);
void F_DRV_USBFSV1_HOST_ControlTransferDataStageSend(DRV_USBFSV1_OBJ * hDriver);

void F_DRV_USBFSV1_DEVICE_IRPQueueFlush
(
  DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObject,
  USB_DEVICE_IRP_STATUS status
);

void F_DRV_USBFSV1_DEVICE_EndpointObjectEnable
(
  DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObject,
  uint16_t endpointSize,
  USB_TRANSFER_TYPE endpointType
);

bool F_DRV_USBFSV1_HOST_ControlTransferProcess(DRV_USBFSV1_OBJ * hDriver);

void F_DRV_USBFSV1_HOST_NonControlTransferDataSend(DRV_USBFSV1_OBJ * hDriver);

void F_DRV_USBFSV1_HOST_ControlTransferBW(DRV_USBFSV1_OBJ * hDriver);

DRV_USBFSV1_HOST_IRP_LOCAL * F_DRV_USBFSV1_HOST_PipeGroupGetNextIRP(DRV_USBFSV1_HOST_PIPE_GROUP * pipeGroup);

bool F_DRV_USBFSV1_HOST_NonControlTransferBW(DRV_USBFSV1_OBJ * hDriver, DRV_USBFSV1_HOST_IRP_LOCAL * irp);

USB_SPEED DRV_USBFSV1_HOST_DeviceCurrentSpeedGet(DRV_HANDLE client);

void DRV_USBFSV1_HOST_StartOfFrameControl(DRV_HANDLE client, bool control);


<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.1"
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.2"
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.4"
#pragma coverity compliance end_block "MISRA C-2012 Rule 8.6"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */

#endif
