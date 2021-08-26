/*******************************************************************************
  USB Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_udphs_local.h

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

#ifndef _DRV_USB_UDPHS_LOCAL_H
#define _DRV_USB_UDPHS_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "definitions.h"
#include "driver/usb/udphs/drv_usb_udphs.h"
#include "driver/usb/udphs/src/drv_usb_udphs_variant_mapping.h"
#include "osal/osal.h"


#define COMPILER_WORD_ALIGNED                               __attribute__((__aligned__(4)))
/* Number of Endpoints used */
#define DRV_USB_UDPHS_ENDPOINT_NUMBER_MASK                    (0x0F)
#define DRV_USB_UDPHS_ENDPOINT_DIRECTION_MASK                 (0x80)
#define DRV_USB_UDPHS_MAX_ENDPOINT_NUMBER                     (UDPHS_EPT_NUMBER)
#define DRV_USB_UDPHS_NUMBER_OF_BANKS                         (DRV_USB_UDPHS_ENDPOINT_BANKS_ONE)

/** Max size of the DMA FIFO */
#define DMA_MAX_FIFO_SIZE                                   (65536)

/** FIFO space size in bytes */
#define EPT_VIRTUAL_SIZE                                    (65536)

#define ENDPOINT_FIFO_ADDRESS(endpoint)                     (((uint8_t*) UDPHS_RAM_ADDR) + EPT_VIRTUAL_SIZE * (endpoint))

#define DRV_USB_UDPHS_AUTO_ZLP_ENABLE                         false

#ifdef __CORE_CA_H_GENERIC
    #ifdef _SAMA7G54_H_
        #define _DRV_USB_UDPHS_InitUTMI() \
        {\
            uint32_t i;\
            uint32_t temp;\
        \
            /* Reset the USB port (by setting RSTC_GRSTR.USB_RSTx). */ \
            RSTC_REGS->RSTC_GRSTR |= RSTC_GRSTR_USB_RST(1); \
        \
            /* Clear the COMMONONN bit. */ \
            /* SFR_UTMI0Rx.COMMONONN */  \
            *(unsigned int *) (SFR_BASE_ADDRESS + 0x2040) &= ~0x00000008; \
            \
            /* HS Transmitter pre-emphasis circuit sources 1x pre-emphasis current. */ \
            *(unsigned int *) (SFR_BASE_ADDRESS + 0x2040) &= 0x01800000; \
            *(unsigned int *) (SFR_BASE_ADDRESS + 0x2040) |= 0x00800000; \
        \
            /* Release the USB port reset (by clearing USB_RSTx). */ \
            /* The PLL starts as soon as PHY reset is released in RSTC_GRSTR.USB_RSTx */ \
            /* Release PHY reset in RSTC_GRSTR for each PHY (A, B, C) to be used. */ \
            RSTC_REGS->RSTC_GRSTR &= ~RSTC_GRSTR_USB_RST(1); \
        \
            /* Wait for 45 us before any USB operation */ \
            temp = 45 * (CPU_CLOCK_FREQUENCY/1000000)/6;   /* comments*/  \
            for (i = 0; i < temp; i++) \
            { \
                asm("NOP"); \
            } \
        \
            /* SFR->SFR_UTMI0R[0] and SFR_UTMI0R_VBUS; */ \
            /* 1: The VBUS signal is valid, and the pull-up resistor on D+ is enabled. */ \
            *(unsigned int *) (SFR_BASE_ADDRESS + 0x2040) |= 0x02000000; \
        }
    #else
        #define _DRV_USB_UDPHS_InitUTMI() \
        {\
            uint32_t uckr;                   /* Shadow Register */  \
        \
            uckr = CKGR_UCKR_UPLLEN_Msk | CKGR_UCKR_UPLLCOUNT(0x3); \
            /* enable the 480MHz UTMI PLL  */ \
            PMC_REGS->CKGR_UCKR = uckr; \
        \
            /* wait until UPLL is locked */ \
            while (!(PMC_REGS->PMC_SR & PMC_SR_LOCKU_Msk)); \
        }
    #endif  /* _SAMA7G54_H_ */
#else
    #define _DRV_USB_UDPHS_InitUTMI()
#endif /* __CORE_CA_H_GENERIC */

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

/*********************************************
 * USB Device Endpoint Banks count. This data
 * structures contains all possibilities of
 * bank counts that can be configured type.
 *********************************************/

typedef enum
{
    /* Zero bank, the endpoint is not mapped in memory  */
    DRV_USB_UDPHS_ENDPOINT_BANKS_ZERO = 0,

    /* One bank (bank 0) */
    DRV_USB_UDPHS_ENDPOINT_BANKS_ONE = 1,

    /* Double bank (Ping-Pong: bank0/bank1) */
    DRV_USB_UDPHS_ENDPOINT_BANKS_TWO = 2,

    /* Triple bank (bank0/bank1/bank2) */
    DRV_USB_UDPHS_ENDPOINT_BANKS_THREE = 3
}
DRV_USB_UDPHS_ENDPOINT_BANKS;

/***************************************************
 * This is an intermediate flag that is set by
 * the driver to indicate that a ZLP should be sent
 ***************************************************/
#define USB_DEVICE_IRP_FLAG_SEND_ZLP 0x80

/***************************************************
 * This object is used by the driver as IRP place
 * holder along with queuing feature.
 ***************************************************/
typedef struct _USB_DEVICE_IRP_LOCAL
{
    /* Pointer to the data buffer */
    void * data;

    /* Size of the data buffer */
    unsigned int size;

    /* Status of the IRP */
    USB_DEVICE_IRP_STATUS status;

    /* IRP Callback. If this is NULL,
     * then there is no callback generated */
    void (*callback)(struct _USB_DEVICE_IRP * irp);

    /* Request specific flags */
    USB_DEVICE_IRP_FLAG flags;

    /* User data */
    uintptr_t userData;

    /* This points to the next IRP in the queue */
    struct _USB_DEVICE_IRP_LOCAL * next;

    /* This points to the previous IRP in the queue */
    struct _USB_DEVICE_IRP_LOCAL * previous;

    /* Pending bytes in the IRP */
    uint32_t nPendingBytes;

}
USB_DEVICE_IRP_LOCAL;

/************************************************
 * Endpoint state enumeration.
 ************************************************/
typedef enum
{
    DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_ENABLED = 0x1,
    DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE_STALLED = 0x2
}
DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE;

/************************************************
 * Endpoint data structure. This data structure
 * holds the IRP queue and other flags associated
 * with functioning of the endpoint.
 ************************************************/
typedef struct
{
    /* This is the IRP queue for
     * the endpoint */
    USB_DEVICE_IRP_LOCAL * irpQueue;

    /* Max packet size for the endpoint */
    uint16_t maxPacketSize;

    /* Endpoint type */
    USB_TRANSFER_TYPE endpointType;

    /* Endpoint state bitmap */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_STATE endpointState;

	/* This gives the Endpoint Direction */
	USB_DATA_DIRECTION endpointDirection;

}
DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ;

/********************************************
 * This enumeration list the possible status
 * valus of a token once it has completed.
 ********************************************/

typedef enum
{
    USB_TRANSACTION_ACK             = 0x2,
    USB_TRANSACTION_NAK             = 0xA,
    USB_TRANSACTION_STALL           = 0xE,
    USB_TRANSACTION_DATA0           = 0x3,
    USB_TRANSACTION_DATA1           = 0xB,
    USB_TRANSACTION_BUS_TIME_OUT    = 0x0,
    USB_TRANSACTION_DATA_ERROR      = 0xF

}
DRV_USB_UDPHS_TRANSACTION_RESULT;

typedef enum
{
    DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST,
    DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST,
    DRV_USB_UDPHS_DEVICE_EP0_STATE_EXPECTING_TX_STATUS_STAGE_FROM_HOST,
    DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_SETUP_IRP_FROM_CLIENT,
    DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_DATA_IRP_FROM_CLIENT,
    DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_IRP_FROM_CLIENT,
    DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT,
    DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT,
	DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE,
	DRV_USB_UDPHS_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE,
    DRV_USB_UDPHS_DEVICE_EP0_STATE_TX_DATA_STAGE_IN_PROGRESS,
    DRV_USB_UDPHS_DEVICE_EP0_STATE_RX_DATA_STAGE_IN_PROGRESS,
}
DRV_USB_UDPHS_DEVICE_EP0_STATE;

/*********************************************
 * Driver NON ISR Tasks routine states.
 *********************************************/

typedef enum
{
    /* Driver checks if the UTMI clock is usable */
    DRV_USB_UDPHS_TASK_STATE_WAIT_FOR_CLOCK_USABLE = 1,

    /* Driver initializes the required operation mode  */
    DRV_USB_UDPHS_TASK_STATE_INITIALIZE_OPERATION_MODE,

    /* Driver has complete initialization and can be opened */
    DRV_USB_UDPHS_TASK_STATE_RUNNING

} DRV_USB_UDPHS_TASK_STATE;

/***********************************************
 * Driver object structure. One object per
 * hardware instance
 **********************************************/

typedef struct _DRV_USB_UDPHS_OBJ_STRUCT
{
    /* Indicates this object is in use */
    bool inUse;

    /* Indicates that the client has opened the instance */
    bool isOpened;

    /* Status of this driver instance */
    SYS_STATUS status;

    /* Contains the consumed FIFO size */
    unsigned int consumedFIFOSize;

    /* Contains the desired operation speed */
    USB_SPEED operationSpeed;

    /* Contains the EPO state */
    DRV_USB_UDPHS_DEVICE_EP0_STATE endpoint0State;

    /* The USB peripheral associated with the object */
    udphs_registers_t * usbID;

    /* Interrupt source for USB module */
    INT_SOURCE interruptSource;

    /* Mutex create function place holder*/
    OSAL_MUTEX_DECLARE (mutexID);

    /* Pointer to the endpoint table */
    DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ *endpointTable;

    /* The object is current in an interrupt context */
    bool isInInterruptContext;

    /* The speed at which the device attached */
    USB_SPEED deviceSpeed;

    /* Tracks if the current control transfer is a zero length */
    bool isZeroLengthControlTransfer;

    /* Non ISR Task Routine state */
    DRV_USB_UDPHS_TASK_STATE state;

    /* Client data that will be returned at callback */
    uintptr_t  hClientArg;

    /* Call back function for this client */
    DRV_USB_EVENT_CALLBACK  pEventCallBack;

    /* Current VBUS level when running in device mode */
    DRV_USB_VBUS_LEVEL vbusLevel;

	/* Callback to determine the Vbus level */
	DRV_USB_UDPHS_VBUS_COMPARATOR vbusComparator;

    /* Sent session invalid event to the client */
    bool sessionInvalidEventSent;

    /* True if device is attached */
    volatile bool deviceAttached;

    /* Set if device if D+ pull up is enabled. */
    bool isAttached;

	/* This is array of device endpoint objects pointers */
	DRV_USB_UDPHS_DEVICE_ENDPOINT_OBJ * deviceEndpointObj[DRV_USB_UDPHS_ENDPOINTS_NUMBER];

    /* True if Root Hub Operation is enabled */
    bool operationEnabled;

} DRV_USB_UDPHS_OBJ;


#ifndef SYS_DEBUG_PRINT
	#define SYS_DEBUG_PRINT(level, format, ...)
#endif

#ifndef SYS_DEBUG_MESSAGE
	#define SYS_DEBUG_MESSAGE(a,b, ...)
#endif

#ifndef SYS_DEBUG
	#define SYS_DEBUG(a,b)
#endif

void _DRV_USB_UDPHS_DEVICE_Initialize(DRV_USB_UDPHS_OBJ * drvObj, SYS_MODULE_INDEX index);
void _DRV_USB_UDPHS_DEVICE_Tasks_ISR(DRV_USB_UDPHS_OBJ * hDriver);

#endif
