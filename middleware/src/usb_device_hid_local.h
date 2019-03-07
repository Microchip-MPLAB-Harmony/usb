/*******************************************************************************
  USB HID Function Driver Local Header file.

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_hid_local.h

  Summary:
    USB HID function driver local header file.
  
  Description:
    This file contains the type, definitions and function prototypes that are
    local to USB Device HID function Driver.
*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef _USB_DEVICE_HID_LOCAL_H_
#define _USB_DEVICE_HID_LOCAL_H_
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "osal/osal.h"


// *****************************************************************************
// *****************************************************************************
// Section: Local data types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* HID flags.

  Summary:
    Flags for tracking internal status.

  Description:
    Flags for tracking internal status.

  Remarks:
    This structure is internal to the HID function driver.
*/

typedef union _USB_DEVICE_HID_FLAGS
{
    struct
    {
        uint8_t interfaceReady:1;
        uint8_t interruptEpTxReady:1;
        uint8_t interruptEpRxReady:1;
    };
    uint8_t allFlags;

} USB_DEVICE_HID_FLAGS;

// *****************************************************************************
/* HID Instance structure.

  Summary:
    Identifies the HID instance.

  Description:
    This type identifies the HID instance.

  Remarks:
    This structure is internal to the HID function driver.
*/

typedef struct 
{

    USB_DEVICE_HID_FLAGS flags;
    USB_DEVICE_HANDLE devLayerHandle;
    USB_DEVICE_HID_INIT * hidFuncInit;
    USB_DEVICE_HID_EVENT_HANDLER appCallBack;
    uintptr_t userData;
    bool ignoreControlEvents;
    USB_ENDPOINT endpointTx;
    uint16_t endpointTxSize;
    USB_ENDPOINT endpointRx;
    uint16_t endpointRxSize;
    size_t currentTxQueueSize;
    size_t currentRxQueueSize;
    uint8_t *hidDescriptor;

} USB_DEVICE_HID_INSTANCE;

// *****************************************************************************
/* HID Common data object

  Summary:
    Object used to keep track of data that is common to all instances of the
    HID function driver.

  Description:
    This object is used to keep track of any data that is common to all
    instances of the HID function driver.

  Remarks:
    None.
*/
typedef struct
{
    /* Set to true if all members of this structure
       have been initialized once */
    bool isMutexHidIrpInitialized;

    /* Mutex to protect client object pool */
    OSAL_MUTEX_DECLARE(mutexHIDIRP);

} USB_DEVICE_HID_COMMON_DATA_OBJ;
// *****************************************************************************
// *****************************************************************************
// Section: Local function protoypes.
// *****************************************************************************
// *****************************************************************************

void _USB_DEVICE_HID_InitializeByDescriptorType
(
    SYS_MODULE_INDEX iHID,
    USB_DEVICE_HANDLE usbDeviceHandle,
    void* funcDriverInit,
    uint8_t intfNumber,
    uint8_t altSetting,
    uint8_t descriptorType,
    uint8_t * pDescriptor
);

void _USB_DEVICE_HID_ReportSendCallBack(USB_DEVICE_IRP * irpTx);

void _USB_DEVICE_HID_ReportReceiveCallBack(USB_DEVICE_IRP * irpRx);

void _USB_DEVICE_HID_ControlTransferHandler
(
    SYS_MODULE_INDEX iHID,
    USB_DEVICE_EVENT controlEvent,
    USB_SETUP_PACKET * setupPkt
);

void _USB_DEVICE_HID_DeInitialize(SYS_MODULE_INDEX iHID);

void _USB_DEVICE_HID_GlobalInitialize (void); 

#endif
