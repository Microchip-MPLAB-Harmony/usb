/*******************************************************************************
  USB Driver Local Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usbdp_local.h

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

#ifndef DRV_USBDP_LOCAL_H
#define DRV_USBDP_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "definitions.h"
#include "driver/usb/usbdp/drv_usbdp.h"
#include "driver/usb/usbdp/src/drv_usbdp_variant_mapping.h"
#include "osal/osal.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
 * This is an local macro used to give the endpoint FIFO from the virtual 
 * address
 ******************************************************************************/
#define EPT_VIRTUAL_SIZE                              (65536)

#define ENDPOINT_FIFO_ADDRESS(endpoint)               (((uint8_t*) UDP_RAM_ADDR) + EPT_VIRTUAL_SIZE * (endpoint))

/*******************************************************************************
 * This is an local macro used to mask the bit positions of direction of 
 * endpoints
 ******************************************************************************/
#define DRV_USBDP_ENDPOINT_DIRECTION_MASK             (0x80U)

/*******************************************************************************
 * This is an local macro used to mask the bit positions of available endpoint 
 * numbers
 ******************************************************************************/
#define DRV_USBDP_ENDPOINT_NUMBER_MASK                (0x0FU)

/*******************************************************************************
 * This is an intermediate flag that is set by the driver to indicate that a 
 * ZLP should be sent
 ******************************************************************************/
#define DRV_USBDP_AUTO_ZLP_ENABLE                     false

/*******************************************************************************
 * This is an intermediate flag that is set by the driver to indicate that a 
 * ZLP should be sent
 ******************************************************************************/
#define USB_DEVICE_IRP_FLAG_SEND_ZLP                  0x80U

/*******************************************************************************
 * This is a macro needed while accessing UDP_CSR register to ensure modifying 
 * certain bits of UDP_CSR register will not affect others. This is as per 
 * steps given in datasheet
 ******************************************************************************/
#define USBDP_REG_NO_EFFECT_1_ALL               ( UDP_CSR_RX_DATA_BK0_Msk |\
                                                  UDP_CSR_RX_DATA_BK1_Msk |\
                                                  UDP_CSR_STALLSENT_Msk   |\
                                                  UDP_CSR_RXSETUP_Msk     |\
                                                  UDP_CSR_TXCOMP_Msk)

/*******************************************************************************
 * This is a macro needed to set any bit(s) in CSR register. Due to 
 * synchronization between MCK and UDPCK, the software application must wait 
 * for the end of the write operation before executing another write by polling 
 * the bits which must be set/cleared. This is as per steps given in datasheet
 ******************************************************************************/
#define USBDP_CSR_SET_BITS(usbID, endpoint, bits)           \
{                                                           \
    volatile uint32_t regValue;                             \
    volatile uint8_t nopCount = 0;                          \
    regValue  = usbID->UDP_CSR[endpoint];                   \
    regValue |= (USBDP_REG_NO_EFFECT_1_ALL | (bits));       \
    usbID->UDP_CSR[endpoint] = regValue;                    \
    while(nopCount++ < 20U)                                  \
    {                                                       \
        __NOP();                                            \
    }                                                       \
}

/*******************************************************************************
 * This is a macro needed to clear any bit(s) in CSR register. Due to 
 * synchronization between MCK and UDPCK, the software application must wait 
 * for the end of the write operation before executing another write by polling 
 * the bits which must be set/cleared. This is as per steps given in datasheet
 ******************************************************************************/
#define USBDP_CSR_CLR_BITS(usbID, endpoint, bits)           \
{                                                           \
    volatile uint32_t regValue;                             \
    volatile uint8_t nopCount = 0;                          \
    regValue  = usbID->UDP_CSR[endpoint];                   \
    regValue |= USBDP_REG_NO_EFFECT_1_ALL;                  \
    regValue &= ~(bits);                                    \
    usbID->UDP_CSR[endpoint] = regValue;                    \
    while(nopCount++ < 20U)                                  \
    {                                                       \
        __NOP();                                            \
    }                                                       \
}

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************
/* MISRA C-2012 Rule 5.2 deviate:6, and Rule 8.6 deviate:1. 
   Deviation record ID - H3_USB_MISRAC_2012_R_5_2_DR_1, H3_USB_MISRAC_2012_R_8_6_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:6 "MISRA C-2012 Rule 5.2" "H3_USB_MISRAC_2012_R_5_2_DR_1" )\
(deviate:1 "MISRA C-2012 Rule 8.6" "H3_USB_MISRAC_2012_R_8_6_DR_1" )
</#if>

/*******************************************************************************
 * USB Device Endpoint Banks count. This data structures contains all 
 * possibilities of bank counts that can be configured type.
 ******************************************************************************/

typedef enum
{
    /* Zero bank, the endpoint is not mapped in memory  */
    DRV_USBDP_ENDPOINT_BANKS_ZERO = 0,

    /* One bank (bank 0) */
    DRV_USBDP_ENDPOINT_BANKS_ONE = 1,

    /* Double bank (Ping-Pong: bank0/bank1) */
    DRV_USBDP_ENDPOINT_BANKS_TWO = 2,

    /* Triple bank (bank0/bank1/bank2) */
    DRV_USBDP_ENDPOINT_BANKS_THREE = 3
}
DRV_USBDP_ENDPOINT_BANKS;

/*******************************************************************************
 * This object is used by the driver as IRP place holder along with queuing 
 * feature.
 ******************************************************************************/
typedef struct S_USB_DEVICE_IRP_LOCAL
{
    /* Pointer to the data buffer */
    void * data;

    /* Size of the data buffer */
    unsigned int size;

    /* Status of the IRP */
    USB_DEVICE_IRP_STATUS status;

    /* IRP Callback. If this is NULL, then there is no callback generated */
    void (*callback)(struct S_USB_DEVICE_IRP * irp);

    /* Request specific flags */
    USB_DEVICE_IRP_FLAG flags;

    /* User data */
    uintptr_t userData;

    /* This points to the next IRP in the queue */
    struct S_USB_DEVICE_IRP_LOCAL * next;

    /* This points to the previous IRP in the queue */
    struct S_USB_DEVICE_IRP_LOCAL * previous;

    /* Pending bytes in the IRP */
    uint32_t nPendingBytes;

}
USB_DEVICE_IRP_LOCAL;

/*******************************************************************************
 * Endpoint state enumeration.
 ******************************************************************************/
typedef enum
{
    DRV_USBDP_ENDPOINT_STATE_ENABLED = 0x1,
    DRV_USBDP_ENDPOINT_STATE_STALLED = 0x2
}
DRV_USBDP_ENDPOINT_STATE;

/*******************************************************************************
 * Endpoint data structure. This data structure holds the IRP queue and other 
 * flags associated with functioning of the endpoint.
 ******************************************************************************/
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
    DRV_USBDP_ENDPOINT_STATE endpointState;

    /* This gives the Endpoint Direction */
    USB_DATA_DIRECTION endpointDirection;

}
DRV_USBDP_ENDPOINT_OBJ;


/*******************************************************************************
 * Endpoint 0 State Enum. This is to maintain the Endpoint 0 State. 
 ******************************************************************************/

typedef enum
{
    DRV_USBDP_EP0_STATE_EXPECTING_SETUP_FROM_HOST,
    DRV_USBDP_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST,
    DRV_USBDP_EP0_STATE_EXPECTING_TX_STATUS_STAGE_FROM_HOST,
    DRV_USBDP_EP0_STATE_WAITING_FOR_SETUP_IRP_FROM_CLIENT,
    DRV_USBDP_EP0_STATE_WAITING_FOR_RX_DATA_IRP_FROM_CLIENT,
    DRV_USBDP_EP0_STATE_WAITING_FOR_RX_STATUS_IRP_FROM_CLIENT,
    DRV_USBDP_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT,
    DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT,
    DRV_USBDP_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE,
    DRV_USBDP_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE,
    DRV_USBDP_EP0_STATE_TX_DATA_STAGE_IN_PROGRESS,
    DRV_USBDP_EP0_STATE_RX_DATA_STAGE_IN_PROGRESS,
}
DRV_USBDP_EP0_STATE;

/*******************************************************************************
 * Driver NON ISR Tasks routine states.
 ******************************************************************************/

typedef enum
{
    /* Driver checks if the UTMI clock is usable */
    DRV_USBDP_TASK_STATE_WAIT_FOR_CLOCK_USABLE = 1,

    /* Driver initializes the required operation mode  */
    DRV_USBDP_TASK_STATE_INITIALIZE_OPERATION_MODE,

    /* Driver has complete initialization and can be opened */
    DRV_USBDP_TASK_STATE_RUNNING

} DRV_USBDP_TASK_STATE;

/*******************************************************************************
 * Driver object structure. One object per hardware instance
 ******************************************************************************/

typedef struct S_DRV_USBDP_OBJ_STRUCT
{
    /* Indicates this object is in use */
    bool inUse;

    /* Indicates that the client has opened the instance */
    bool isOpened;

    /* Status of this driver instance */
    SYS_STATUS status;

    /* Contains the desired operation speed */
    USB_SPEED operationSpeed;

    /* Contains the EPO state */
    DRV_USBDP_EP0_STATE endpoint0State;

    /* The USB peripheral associated with the object */
    udp_registers_t * usbID;

    /* Interrupt source for USB module */
    INT_SOURCE interruptSource;

    /* Mutex create function place holder*/
    OSAL_MUTEX_DECLARE (mutexID);

    /* Pointer to the endpoint table */
    DRV_USBDP_ENDPOINT_OBJ *endpointTable;

    /* The object is current in an interrupt context */
    bool isInInterruptContext;

    /* The speed at which the device attached */
    USB_SPEED deviceSpeed;

    /* Client data that will be returned at callback */
    uintptr_t  hClientArg;

    /* Call back function for this client */
    DRV_USB_EVENT_CALLBACK  pEventCallBack;

    /* Current VBUS level when running in device mode */
    DRV_USB_VBUS_LEVEL vbusLevel;

    /* Callback to determine the Vbus level */
    DRV_USBDP_VBUS_COMPARATOR vbusComparator;

    /* Sent session invalid event to the client */
    bool sessionInvalidEventSent;

    /* True if device is attached */
    volatile bool isAttached;

    /* This is array of device endpoint objects pointers */
    DRV_USBDP_ENDPOINT_OBJ * deviceEndpointObj[DRV_USBDP_ENDPOINTS_NUMBER];

} DRV_USBDP_OBJ;


#ifndef SYS_DEBUG_PRINT
    #define SYS_DEBUG_PRINT(level, format, ...)
#endif

#ifndef SYS_DEBUG_MESSAGE
    #define SYS_DEBUG_MESSAGE(a,b, ...)
#endif

#ifndef SYS_DEBUG
    #define SYS_DEBUG(a,b)
#endif

void F_DRV_USBDP_Initialize(DRV_USBDP_OBJ * drvObj, SYS_MODULE_INDEX index);
void F_DRV_USBDP_Tasks_ISR(DRV_USBDP_OBJ * hDriver);
void DRV_USBDP_Deinitialize
(
    const SYS_MODULE_INDEX  drvIndex
);
void F_DRV_USBDP_IRPQueueFlush
(
    DRV_USBDP_ENDPOINT_OBJ * endpointObj,
    USB_DEVICE_IRP_STATUS status
);
void F_DRV_USBDP_EndpointObjectEnable
(
    DRV_USBDP_ENDPOINT_OBJ * endpointObj,
    uint16_t endpointSize,
    USB_TRANSFER_TYPE endpointType
);

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.2"
#pragma coverity compliance end_block "MISRA C-2012 Rule 8.6"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */

#endif
