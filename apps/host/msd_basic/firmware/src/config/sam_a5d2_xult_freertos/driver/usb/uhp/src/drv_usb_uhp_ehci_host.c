/*******************************************************************************
   USB EHCI Host Controller Driver

   Company:
    Microchip Technology Inc.

   File Name:
    drv_usb_uhp_ehci_host.c

   Summary:
    USB Host Port EHCI Driver Implementation.

   Description:
    This file implements the USB Host port EHCI driver implementation.
    This file should be included in the application if USB Host mode operation
    is desired.
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

/* #include "sam.h" */
#ifndef __inline__
    #define __inline__    inline
#endif
#include <stdint.h>
#include <string.h>
#include "configuration.h"
#include "definitions.h"
#include "driver/usb/drv_usb.h"
#include "driver/usb/uhp/src/drv_usb_uhp_local.h"

/**********************************************************
 * This structure is a set of pointer to the USB EHCI driver
 * functions. It is provided to the host and device layer
 * as the interface to the driver.
 * *******************************************************/

DRV_USB_HOST_INTERFACE gDrvUSBUHPHostInterface =
{
    .open              = DRV_USB_UHP_Open,
    .close             = DRV_USB_UHP_Close,
    .eventHandlerSet   = DRV_USB_UHP_ClientEventCallBackSet,
    .hostIRPSubmit     = DRV_USB_UHP_HOST_IRPSubmit,
    .hostIRPCancel     = DRV_USB_UHP_HOST_IRPCancel,
    .hostPipeSetup     = DRV_USB_UHP_HOST_PipeSetup,
    .hostPipeClose     = DRV_USB_UHP_HOST_PipeClose,
    .hostEventsDisable = DRV_USB_UHP_HOST_EventsDisable,
    .hostEventsEnable  = DRV_USB_UHP_HOST_EventsEnable,
    .rootHubInterface.rootHubPortInterface.hubPortReset           = DRV_USB_UHP_HOST_ROOT_HUB_PortReset,
    .rootHubInterface.rootHubPortInterface.hubPortSpeedGet        = DRV_USB_UHP_HOST_ROOT_HUB_PortSpeedGet,
    .rootHubInterface.rootHubPortInterface.hubPortResetIsComplete = DRV_USB_UHP_HOST_ROOT_HUB_PortResetIsComplete,
    .rootHubInterface.rootHubPortInterface.hubPortSuspend         = DRV_USB_UHP_HOST_ROOT_HUB_PortSuspend,
    .rootHubInterface.rootHubPortInterface.hubPortResume          = DRV_USB_UHP_HOST_ROOT_HUB_PortResume,
    .rootHubInterface.rootHubMaxCurrentGet      = DRV_USB_UHP_HOST_ROOT_HUB_MaximumCurrentGet,
    .rootHubInterface.rootHubPortNumbersGet     = DRV_USB_UHP_HOST_ROOT_HUB_PortNumbersGet,
    .rootHubInterface.rootHubSpeedGet           = DRV_USB_UHP_HOST_ROOT_HUB_BusSpeedGet,
    .rootHubInterface.rootHubInitialize         = DRV_USB_UHP_HOST_ROOT_HUB_Initialize,
    .rootHubInterface.rootHubOperationEnable    = DRV_USB_UHP_HOST_ROOT_HUB_OperationEnable,
    .rootHubInterface.rootHubOperationIsEnabled = DRV_USB_UHP_HOST_ROOT_HUB_OperationIsEnabled
};

#define NUMBER_OF_PORTS    2
/* #define HSIC */


typedef union __attribute__ ((packed))
{
    struct
    {
        uint32_t Status       : 8;
        uint32_t PIDCode      : 2;
        uint32_t ErrorCounter : 2;
        uint32_t C_Page       : 3;
        uint32_t IOC          : 1;
        uint32_t TotalBytesTF : 15;
        uint32_t DataToggle   : 1;
    };

    uint32_t qtdtoken;
}
QTDToken;


typedef struct
{
    volatile uint32_t Next_qTD_Pointer;             /* DWord 0 */
    volatile uint32_t Alternate_Next_qTD_Pointer;   /* DWord 1 */
    volatile QTDToken qTD_Token;                    /* DWord 2 */
    volatile uint32_t qTD_Buffer_Page_Pointer_List; /* DWord 3 */
    volatile uint32_t qTD_Buffer[4];                /* DWord 4 to 7 */
} EHCITransferDescriptor;

typedef struct
{
    EHCITransferDescriptor ElementNumberQTD;
} EHCIQueueTDDescriptor;

typedef struct
{
    volatile uint32_t Horizontal_Link_Pointer;  /* DWord 0 */
    volatile uint32_t Endpoint_Characteristics; /* DWord 1 */
    volatile uint32_t Endpoint_Capabilities;    /* DWord 2 */
    volatile uint32_t Current_qTD_Pointer;      /* DWord 3: processed by the host */
    volatile uint32_t Transfer_Overlay[8];      /* DWord 4 to 12 */
} EHCIQueueHeadDescriptor;

__ALIGNED(32) EHCIQueueTDDescriptor EHCI_QueueTD[5];                    /* Queue Element Transfer Descriptor: 1 qTD is 0x20=32 */
__ALIGNED(32) uint32_t EHCI_QueueHead[48];                              /* Queue Head: 0x30=48 length */
__ALIGNED(4096) uint8_t USBBufferAligned[USB_HOST_TRANSFERS_NUMBER*64]; /* 4K page aligned, see Table 3-17. qTD Buffer Pointer(s) (DWords 3-7) */
__ALIGNED(32) volatile uint8_t setupPacket[8];                          /* 32 bit aligned */
__ALIGNED(4096) uint8_t USBBufferCSWAligned[64];

/*****************************************
* Pool of pipe objects that is used by
* all driver instances.
*****************************************/
DRV_USB_UHP_HOST_PIPE_OBJ gDrvUSBHostPipeObj[DRV_USB_UHP_PIPES_NUMBER];

/****************************************
* The driver object
****************************************/
extern DRV_USB_UHP_OBJ gDrvUSBObj[DRV_USB_UHP_INSTANCES_NUMBER];

extern void DRV_USB_UHP_HOST_TransferProcess(DRV_USB_UHP_OBJ *hDriver);

// ****************************************************************************
// ****************************************************************************
// Local Functions
// ****************************************************************************
// ****************************************************************************


/* Function:
    static void _USB_UHP_ResetEnable(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    Initiate the USB reset on the current port number

   Description:
    Initiate the USB reset on the current port number

   Remarks:
    Refer to .h for usage information.
 */
static void _USB_UHP_ResetEnable(DRV_USB_UHP_OBJ *hDriver)
{
    volatile uhphs_registers_t *usbID = hDriver->usbID;
 
    /* Test if the Host Controller is halted */
    if ((usbID->UHPHS_USBSTS & UHPHS_USBSTS_HCHLT_Msk) != 0)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "ehci_send_reset error!!!!");
    }

    /* Set the reset bit */ 
    *((uint32_t *)&(usbID->UHPHS_PORTSC_0) + hDriver->portNumber)
        &= ~UHPHS_PORTSC_0_PED_Msk;                         /* Port Disabled */
    *((uint32_t *)&(usbID->UHPHS_PORTSC_0) + hDriver->portNumber)
        |= UHPHS_PORTSC_0_PR_Msk;                           /* Set Port Reset */
}

/* Function:
    void _DRV_USB_UHP_HOST_AttachDetachStateMachine(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    Initiate the USB reset on the current port number

   Description:
    Initiate the USB reset on the current port number

   Remarks:
    Refer to .h for usage information.
 */
void _DRV_USB_UHP_HOST_AttachDetachStateMachine(DRV_USB_UHP_OBJ *hDriver)
{
    /* In the host mode, we perform attach de-bouncing */
    bool interruptWasEnabled;

    switch (hDriver->attachState)
    {
        case DRV_USB_UHP_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH:

            /* If no device is attached, then there is nothing to do
             * If device is attached, then move to next state */
            if (hDriver->deviceAttached)
            {
                hDriver->attachState = DRV_USB_UHP_HOST_ATTACH_STATE_DETECTED;
            }

            break;

        case DRV_USB_UHP_HOST_ATTACH_STATE_DETECTED:

            /* Disable the driver interrupt as
             * we do not want this section to be interrupted. */
            interruptWasEnabled = _DRV_USB_UHP_InterruptSourceDisable(hDriver->interruptSource);

            if (hDriver->deviceAttached)
            {
                /* Yes the device is still attached. Enumerate
                 * this device. usbHostDeviceInfo is the ID of
                 * this root hub. */
                hDriver->attachedDeviceObjHandle = USB_HOST_DeviceEnumerate(hDriver->usbHostDeviceInfo, 0);
                hDriver->attachState             = DRV_USB_UHP_HOST_ATTACH_STATE_READY;
            }
            else
            {
                /* The device is not attached any more. This was a false attach
                 */
                hDriver->attachState = DRV_USB_UHP_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH;
            }

            if (interruptWasEnabled)
            {
                /* Re-enable the interrupt if it was originally enabled. */
                _DRV_USB_UHP_InterruptSourceEnable(hDriver->interruptSource);
            }
            break;

        case DRV_USB_UHP_HOST_ATTACH_STATE_READY:

            /* De-bouncing is done and device ready. We can check
             * here if the device is detached */
            if (!hDriver->deviceAttached)
            {
                /* Device is not attached */
                hDriver->attachState = DRV_USB_UHP_HOST_ATTACH_STATE_CHECK_FOR_DEVICE_ATTACH;
            }
            break;

        default:
            break;
    }
}


/* Function:
    void _DRV_USB_UHP_HOST_ResetStateMachine(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    Reset State Machine

   Description:
    Reset State Machine 

   Remarks:
    Refer to .h for usage information.
 */
void _DRV_USB_UHP_HOST_ResetStateMachine(DRV_USB_UHP_OBJ *hDriver)
{
    volatile uhphs_registers_t *usbID = hDriver->usbID;

    /* Check if reset is needed */
    switch (hDriver->resetState)
    {
        case DRV_USB_UHP_HOST_RESET_STATE_NO_RESET:
            /* No reset signaling is request */
            break;

        case DRV_USB_UHP_HOST_RESET_STATE_START:
            /* Trigger USB Reset */
            _USB_UHP_ResetEnable(hDriver);
            if (SYS_TIME_DelayMS( DRV_USB_UHP_RESET_DURATION, &hDriver->timerHandle) != SYS_TIME_SUCCESS)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO,"\r\nDRV USBHSV1: Handle error ");
            }
            hDriver->resetState = DRV_USB_UHP_HOST_RESET_STATE_WAIT_FOR_COMPLETE;
            break;

        case DRV_USB_UHP_HOST_RESET_STATE_WAIT_FOR_COMPLETE:
            /* reset should last 50ms */
            if (SYS_TIME_DelayIsComplete(hDriver->timerHandle) == true)
            {
                hDriver->resetState = DRV_USB_UHP_HOST_RESET_STATE_COMPLETE;
            }
            break;

        case DRV_USB_UHP_HOST_RESET_STATE_COMPLETE:
            /* Check if the reset has completed */
            /* if( (usbID->UHPHS_PORTSC[hDriver->portNumber] & UHPHS_PORTSC_0_PR_Msk) == UHPHS_PORTSC_0_PR_Msk ) */
            if ((*((uint32_t *)&(usbID->UHPHS_PORTSC_0) + hDriver->portNumber) & UHPHS_PORTSC_0_PR_Msk) == UHPHS_PORTSC_0_PR_Msk)
            {
                hDriver->resetState = DRV_USB_UHP_HOST_RESET_STATE_COMPLETE2;
            }
            break;

        case DRV_USB_UHP_HOST_RESET_STATE_COMPLETE2:
            /* Reset has completed */
            /* usbID->UHPHS_PORTSC[hDriver->portNumber] &=  ~UHPHS_PORTSC_0_PR_Msk; */
            *((uint32_t *)&(usbID->UHPHS_PORTSC_0) + hDriver->portNumber)
                &= ~UHPHS_PORTSC_0_PR_Msk;              /* Clear Port Reset */

            if ((*((uint32_t *)&(usbID->UHPHS_PORTSC_0) + hDriver->portNumber) & UHPHS_PORTSC_0_PR_Msk) != UHPHS_PORTSC_0_PR_Msk)
            {
                hDriver->resetState = DRV_USB_UHP_HOST_RESET_STATE_NO_RESET;

                /* Clear the flag */
                hDriver->isResetting = false;

                /* Now that reset is complete, we can find out the
                 * speed of the attached device. */
                if ((*((uint32_t *)&(usbID->UHPHS_PORTSC_0) + hDriver->portNumber) & UHPHS_PORTSC_0_PED_Msk) == UHPHS_PORTSC_0_PED_Msk)
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rHS device connected");
                    /* This means the device attached at high speed */
                    hDriver->deviceSpeed    = USB_SPEED_HIGH;
                    hDriver->deviceAttached = true;
                }
                else
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rFS device connected");
                    hDriver->deviceSpeed = USB_SPEED_FULL;

                    /* Software writes a one to this bit when the attached device is not a high-speed device. A one in
                     * this bit means that a companion host controller owns and controls the port. */
                    *((uint32_t *)&(usbID->UHPHS_PORTSC_0) + hDriver->portNumber)
                        |= UHPHS_PORTSC_0_PO_Msk;      /* Port Owner bit to 1 */

                    /* Route all ports to OHCI in config flag
                     * Port routing control logic default-routes each port to an implementation-dependent classic host controller (default value). */
                    usbID->UHPHS_CONFIGFLAG &= ~UHPHS_CONFIGFLAG_CF_Msk;
                }
            }
            break;

        default:
            break;
    }
}

/* Function:
    void ehci_TestTd(void)

   Summary:
    Test EHCI tranfert descriptor

   Description:
    

   Remarks:
    Refer to .h for usage information.
 */
void ehci_TestTd(void)
{
    uint32_t               ConditionCode = 0xFF;
    uint32_t               i;
    EHCITransferDescriptor *qTD = (EHCITransferDescriptor *)&EHCI_QueueTD[0];

    for (i = 0; i < 5; i++)
    {
        qTD = (EHCITransferDescriptor *)&EHCI_QueueTD[i];

        /* 3.5.3 qTD Token
         * Table 3-16. qTD Token (DWord 2)
         * Error Counter (CERR) */
        if (((qTD->qTD_Token.qtdtoken>>10)&0x3) != 0)
        {
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rqTD Token: CERR = 0x%04X, EHCI_QueueTD[%d]", (unsigned int)((qTD->qTD_Token.qtdtoken>>10)&0x3), i);
        }

        /* Table 3-16. qTD Token (DWord 2)
         * This field contains the status of the last transaction
         * performed on this qTD */
        ConditionCode = qTD->qTD_Token.qtdtoken & 0x7E;

        if (ConditionCode != 0)
        {
            if (ConditionCode & (1<<6))
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rHalted");
            }
            else if (ConditionCode & (1<<5))
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rData Buffer Error");
            }
            else if (ConditionCode & (1<<4))
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rBabble Detected");
            }
            else if (ConditionCode & (1<<3))
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rTransaction Error (XactErr)");
            }
            else if (ConditionCode & (1<<2))
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rMissed Micro-Frame");
            }
            else if (ConditionCode & (1<<1))
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rSplit Transaction State (SplitXstate)");
            }
            /* else if( ConditionCode & (1<<0) )
             * {
             *     SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rPing State (P)/ERR");
             * } */
            else
            {
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rehci_TestTd ERR: ConditionCode = 0x%04X", (unsigned int)ConditionCode);
            }
        }
    }
}

/* Function:
    static void ehci_create_queue_head(uint32_t              *qh_base_addr,
                                   uint32_t              *qh_link_pointer,
                                   uint32_t              terminate,
                                   uint32_t              EP_number,
                                   uint32_t              DeviceAddr,
                                   EHCIQueueTDDescriptor *next_qTD_pointer,
                                   uint32_t              mul,
                                   uint32_t              smask,
                                   uint32_t              dtc,
                                   uint8_t               hubAddress,
                                   uint8_t               hubPortAddress)

   Summary:
    Create Queue Head

   Description:
    Fill Queue Head

   Remarks:
    Refer to .h for usage information.
 */
static void ehci_create_queue_head(uint32_t              *qh_base_addr,
                                   uint32_t              *qh_link_pointer,
                                   uint32_t              terminate,
                                   uint32_t              EP_number,
                                   uint32_t              DeviceAddr,
                                   EHCIQueueTDDescriptor *next_qTD_pointer,
                                   uint32_t              mul,
                                   uint32_t              smask,
                                   uint32_t              dtc,
                                   uint8_t               hubAddress,
                                   uint8_t               hubPortAddress)
{
    EHCIQueueHeadDescriptor *QueueHead = (EHCIQueueHeadDescriptor *)qh_base_addr;

    /* Queue Head Horizontal Link Pointer
     * see: 3.6 Queue Head
     * Table 3-18. Queue Head DWord 0 */
    QueueHead->Horizontal_Link_Pointer =
        (uint32_t)qh_link_pointer |        /* QHLP: Queue Head Horizontal Link Pointer: bit[31:5] */
        (1 << 1) |                         /* QH/(s)iTD/FSTN: Data structure type = 01b QH (queue head) */
        (terminate << 0);                  /* T: Terminate = 1 : last transfer */

    /* Table 3-19. Endpoint Characteristics: Queue Head DWord 1 */
    QueueHead->Endpoint_Characteristics =
        (0 << 28) |                      /* RL: Nak Count Reload */
        (0 << 27) |                      /* C: Control Endpoint Flag */
        (512 << 16) |                    /* Maximum Packet Length  // 1024 or 512 */
        (1 << 15) |                      /* H: Head of Reclamation List Flag */
        (dtc << 14) |                    /* DTC: Data Toggle Control: 1: from qTD, 0: from QH (automatic) */
        (2 << 12) |                      /* EPS: Endpoint Speed = 10b High-Speed (480 Mb/s) */
        (EP_number <<  8) |              /* Endpt: Endpoint Number */
        (DeviceAddr <<  0);              /* Device Address */

    /* Table 3-20. Endpoint Capabilities: Queue Head DWord 2 */
    QueueHead->Endpoint_Capabilities =
        (mul << 30) |                      /* Mult: High-Bandwidth Pipe Multiplier = 3 transactions to
                                            * be issued for this endpoint per micro-frame */
        (hubPortAddress << 23) |           /* Port Number */
        (hubAddress << 16) |               /* Hub Addr */
        (0 <<  8) |                        /* µFrame C-Mask: Split Completion Mask */
        (smask <<  0);                     /* µFrame S-mask: Interrupt Schedule Mask */

    /* 3.6.3 Transfer Overlay */
    QueueHead->Transfer_Overlay[0] = (uint32_t)next_qTD_pointer | /* Next qTD Pointer: DWord 4 */
                                     (0 <<  0);                   /* Terminate */
}

/* Function:
    static void ehci_create_qTD(EHCIQueueTDDescriptor *qTD_base_addr,
                            EHCIQueueTDDescriptor *next_qTD_base_addr,
                            uint32_t terminate, uint32_t PID,
                            uint32_t data_toggle,
                            uint32_t nb_bytes,
                            uint32_t int_on_complete,
                            uint32_t *buffer_base_addr)

   Summary:
    Create Transfer Descriptor

   Description:
    Fill Transfer Descriptor

   Remarks:
    Refer to .h for usage information.
 */
static void ehci_create_qTD(EHCIQueueTDDescriptor *qTD_base_addr,
                            EHCIQueueTDDescriptor *next_qTD_base_addr,
                            uint32_t terminate, uint32_t PID,
                            uint32_t data_toggle,
                            uint32_t nb_bytes,
                            uint32_t int_on_complete,
                            uint32_t *buffer_base_addr)
{
    EHCITransferDescriptor *qTD = (EHCITransferDescriptor *)qTD_base_addr;

    /* 3.5.1 Next qTD Pointer */
    /* Table 3-14. qTD Next Element Transfer Pointer (DWord 0) */
    qTD->Next_qTD_Pointer = (uint32_t)next_qTD_base_addr |  /* Next qTD Pointer */
                            terminate;                      /* Terminate */

    /* 3.5.2 Alternate Next qTD Pointer */
    /* Table 3-15. qTD Alternate Next Element Transfer Pointer (DWord 1) */
    qTD->Alternate_Next_qTD_Pointer = (uint32_t)next_qTD_base_addr | /* Alternate Next Transfer Element Pointer. The host controller will always use */
                                                                     /* this pointer when the current qTD is retired due to short packet. */
                                      terminate;                     /* Terminate */
    /* 3.5.3 qTD Token */
    /* Table 3-16. qTD Token (DWord 2) */
    qTD->qTD_Token.qtdtoken = (data_toggle << 31) |     /* Data Toggle */
                              (nb_bytes << 16) |        /* Total Bytes to Transfer */
                              (int_on_complete << 15) | /* Interrupt On Complete (IOC) */
                              (0 << 12) |               /* Current Page (C_Page) */
                              (0 << 10) |               /* Error Counter (CERR)   Allow 3 retry */
                              (PID <<  8) |             /* PID Code (0: OUT, 1: IN 2: SETUP) */
                              (0x80 <<  0);             /* Status Field Description: Active. */

    /* 3.5.4 qTD Buffer Page Pointer List */
    /* Table 3-17. qTD Buffer Pointer(s) (DWords 3-7) */
    qTD->qTD_Buffer_Page_Pointer_List = (uint32_t)buffer_base_addr;    /* Buffer Pointer List : 4k page aligned (Bit 31:12) */
}

/* Function:
    void ehci_init(volatile uhphs_registers_t *usbID)

   Summary:
    EHCI init

   Description:
    

   Remarks:
    Refer to .h for usage information.
 */
void ehci_init(volatile uhphs_registers_t *usbID)
{
    uint32_t loop1 = 0;

    /* Initialize portsc registers */
    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rUHPHS Ports ");
    /* PPC: Port Power Control */
    if (((usbID->UHPHS_HCCPARAMS & UHPHS_HCSPARAMS_PPC_Msk) == UHPHS_HCSPARAMS_PPC_Msk))
    {
        /* Put Port Power on (bit 12) : Host controller has port power control switches
         * put Wake-up on (bit 22, 21, 20) */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "have ");
        for (loop1 = 0; loop1 < (usbID->UHPHS_HCSPARAMS&0xF); loop1++)
        {
            *((uint32_t *)&(usbID->UHPHS_PORTSC_0) + loop1)
                = UHPHS_PORTSC_0_PP_Msk |              /* Host controller has port power control switches. */
                  UHPHS_PORTSC_0_WKOC_E_Msk |          /* enables the port to be sensitive to over-current conditions as wake-up events. */
                  UHPHS_PORTSC_0_WKDSCNNT_E_Msk |      /* enables the port to be sensitive to device disconnects as wake-up events */
                  UHPHS_PORTSC_0_WKCNNT_E_Msk;         /* enables the port to be sensitive to device connects as wake-up events */
        }
    }
    else
    {
        /* Host controller does not have port power control switches. Each port is hard-wired to power. */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "do not have ");
    }
    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "port power switches");

#ifdef HSIC
    /* Enable HSIC on PortC
     * (UHPHS_HSIC instead PORTSC.PP because the controller does not include Port Power Control) */
    usbID->UHPHS_INSNREG08 = UHPHS_INSNREG08_HSIC_EN_Msk;
#endif

    if ((usbID->UHPHS_INSNREG08 & UHPHS_INSNREG08_HSIC_EN_Msk) == UHPHS_INSNREG08_HSIC_EN_Msk)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rHSIC is enabled");
    }
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rHSIC is disabled");
    }

    /* Enable interrupts */
    usbID->UHPHS_USBINTR = UHPHS_USBINTR_USBIE_Msk    /* (UHPHS_USBINTR) USB Interrupt Enable */
                           | UHPHS_USBINTR_USBEIE_Msk /* (UHPHS_USBINTR) USB Error Interrupt Enable */
                           | UHPHS_USBINTR_PCIE_Msk   /* (UHPHS_USBINTR) Port Change Interrupt Enable */
                                                      /* | UHPHS_USBINTR_FLRE_Msk     // (UHPHS_USBINTR) Frame List Rollover Enable */
                           | UHPHS_USBINTR_HSEE_Msk   /* (UHPHS_USBINTR) Host System Error Enable */
                           | UHPHS_USBINTR_IAAE_Msk;  /* (UHPHS_USBINTR) Interrupt on Async Advance Enable */

    /* Write the USBCMD register to set the desired interrupt threshold, frame list size (if applicable) */
    usbID->UHPHS_USBCMD &= ~UHPHS_USBCMD_ITC_Msk;
    usbID->UHPHS_USBCMD |= UHPHS_USBCMD_ITC(1);

    /* and turn the host controller ON via setting the Run/Stop bit. */
    usbID->UHPHS_USBCMD |= UHPHS_USBCMD_RS_Msk; /* Stop = 1 => RUN */

    /* Write a 1 to CONFIGFLAG register to route all ports to the EHCI controller */
    usbID->UHPHS_CONFIGFLAG = UHPHS_CONFIGFLAG_CF_Msk;

    /*  At this point, the host controller is up and running and the port registers will begin reporting device
     *  connects, etc. System software can enumerate a port through the reset process (where the port is in the
     *  enabled state). At this point, the port is active with SOFs occurring down the enabled port enabled Highspeed
     *  ports, but the schedules have not yet been enabled. The EHCI Host controller will not transmit SOFs
     *  to enabled Full- or Low-speed ports. */
}

// *****************************************************************************
/* Function:
    void _DRV_USB_UHP_HOST_Initialize
    (
        DRV_USB_UHP_OBJ * const pUSBDrvObj,
        const SYS_MODULE_INDEX index
    )

  Summary:
    This function initializes the driver for host mode operation.

  Description:
    Function performs the following tasks:
      - Informs the USB module with SOF threshold in bit times
      - Enables VBUS power and initializes the module in HOST mode
      - Resets the BDT table data structure with init value
      - Configures EP0 register for the specific USB module
      - Enables the USB attach interrupt

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/
void _DRV_USB_UHP_HOST_Initialize
(
    DRV_USB_UHP_OBJ *drvObj,
    SYS_MODULE_INDEX  index
)
{
    volatile uhphs_registers_t *usbID = drvObj->usbID;

    /* No device attached */
    drvObj->deviceAttached = false;
    /* Initialize the device handle */
    drvObj->attachedDeviceObjHandle = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;

    drvObj->int_xfr_qtd_complete = 0;
    drvObj->staticDToggleIn      = 0;
    drvObj->staticDToggleOut     = 0;
    drvObj->portNumber = 0xFF;

    ehci_init(usbID);

    /* Disable all interrupts. Interrupts will be enabled when the root hub is
     * enabled */
    usbID->UHPHS_USBINTR &= ~UHPHS_USBINTR_USBIE_Msk;

    /* Initialize the host specific members in the driver object */
    drvObj->isResetting       = false;
    drvObj->usbHostDeviceInfo = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;
    drvObj->operationEnabled  = false;
}/* end of _DRV_USB_UHP_HOST_Initialize() */

// *****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UHP_HOST_IRPSubmit
    (
        DRV_USB_UHP_HOST_PIPE_HANDLE  hPipe,
        USB_HOST_IRP * pinputIRP
    )

  Summary:
    Submits an IRP on a pipe.
    
  Description:
    This function submits an IRP on the specified pipe. The IRP will be added to
    the queue and will be processed in turn. The data will be transferred on the
    bus based on the USB bus scheduling rules. When the IRP has been processed,
    the callback function specified in the IRP will be called. The IRP status
    will be updated to reflect the completion status of the IRP. 

  Remarks:
    See .h for usage information.
*/
USB_ERROR DRV_USB_UHP_HOST_IRPSubmit
(
    DRV_USB_UHP_HOST_PIPE_HANDLE hPipe,
    USB_HOST_IRP                   *inputIRP
)
{
    USB_HOST_IRP_LOCAL * irpIterator;
    DRV_USB_UHP_HOST_TRANSFER_GROUP *controlTransferGroup;
    USB_HOST_IRP_LOCAL          *irp  = (USB_HOST_IRP_LOCAL *)inputIRP;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe = (DRV_USB_UHP_HOST_PIPE_OBJ *)(hPipe);
    DRV_USB_UHP_OBJ           *hDriver;
    uint32_t tosend;
    uint32_t nbBytes;
    uint8_t *point;
    uint8_t idx;
    uint8_t idx_plus;
    uint8_t stop = 0;
    uint8_t DToggle = 0;
    uint8_t OnComplete;
    USB_ERROR returnValue = USB_ERROR_PARAMETER_INVALID;
    uint32_t i;
    volatile uhphs_registers_t *usbID;

    if ((pipe == NULL) || (hPipe == (DRV_USB_HOST_PIPE_HANDLE_INVALID)))
    {
        /* This means an invalid pipe was specified.
         * Return with an error */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: PPipe handle is not valid");
    }
    else
    {
        hDriver = (DRV_USB_UHP_OBJ *)(pipe->hClient);
        controlTransferGroup = &hDriver->controlTransferGroup;

        /* Assign owner pipe */
        irp->pipe      = hPipe;
        irp->status    = USB_HOST_IRP_STATUS_PENDING;
        irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;
        hDriver->hostPipeInUse = pipe->hostPipeN;

        /* We need to disable interrupts was the queue state
         * does not change asynchronously */
        if (!hDriver->isInInterruptContext)
        {
            /* OSAL: Get Mutex */
            if (OSAL_MUTEX_Lock(&(hDriver->mutexID), OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex lock failed");
                returnValue = USB_ERROR_OSAL_FUNCTION;
            }
            else
            {
                controlTransferGroup->interruptWasEnabled = _DRV_USB_UHP_InterruptSourceDisable(hDriver->interruptSource);
            }
        }

        if(USB_ERROR_OSAL_FUNCTION != returnValue)
        {
            /* This needs to be done for all irp irrespective
             * of type or if there IRP is immediately processed */
            irp->next           = NULL;
            irp->completedBytes = 0;
            irp->status = USB_HOST_IRP_STATUS_PENDING;

            if (pipe->irpQueueHead == NULL)
            {
                /* This means that there are no IRPs on this pipe. We can add
                 * this IRP directly */

                irp->previous      = NULL;
                pipe->irpQueueHead = irp;

                if(pipe->pipeType == USB_TRANSFER_TYPE_CONTROL)
                {
                    /* We need to update the flags parameter of the IRP
                     * to indicate the direction of the control transfer. */
               
                    /* SETUP */
                    /* Control transfer */
                    if (*((uint8_t *)(irp->setup)) & 0x80)
                    {
                        /* Device to Host: IN */
                        /* This means the data stage moves from device to 
                         * host. Set bit 15 of the flags parameter */
                        OnComplete = 0;
                        irp->flags |= 0x80;
                    }
                    else
                    {
                        /* Host to Device: OUT */
                        /* This means the data stage moves from host to
                         * device. Clear bit 15 of the flags parameter. */
                        OnComplete = 1;
                        irp->flags &= 0x7F;
                    }

                    /* We need to check if the endpoint 0 is free and if so
                     * then start processing the IRP */
                }
                if (controlTransferGroup->currentIRP == NULL)
                {
                    /* This means that no IRPs are being processed
                     * So we should start the IRP processing. Else
                     * the IRP processing will start in interrupt.
                     * We start by copying the setup command */
                    controlTransferGroup->currentIRP  = irp;
                    controlTransferGroup->currentPipe = pipe;
                    irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
                }

                /* Data is moving from host to device. We
                 * need to copy data into the FIFO and
                 * then and set the TX request bit. If the
                 * IRP size is greater than endpoint size then
                 * we must cut the packet */

                memset(EHCI_QueueHead, 0, sizeof(EHCI_QueueHead));
                memset(EHCI_QueueTD, 0, sizeof(EHCI_QueueTD));
                DCACHE_INVALIDATE_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* INVALIDATE should be called before reading */
                memset(USBBufferAligned, 0, sizeof(USBBufferAligned));


                if (irp->setup != NULL)
                {
                    /* Create Queue Head for the command: */
                    ehci_create_queue_head(&EHCI_QueueHead[0],                /* Queue Head base address */
                                           &EHCI_QueueHead[0],                /* Queue Head Link Pointer */
                                           1,                                 /* Terminate */
                                           pipe->endpointAndDirection & 0xF,
                                           pipe->deviceAddress,               /* Device Address */
                                           &EHCI_QueueTD[0],                  /* Next qTD Pointer */
                                           1,                                 /* High-Bandwidth Pipe Multiplier (Mul) */
                                           0,                                 /* Interrupt Schedule Mask (S-Mask) */
                                           1,                                 /* Initial data toggle comes from incoming qTD DT bit. */
                                           pipe->hubAddress,
                                           pipe->hubPort);
                    DCACHE_CLEAN_BY_ADDR(EHCI_QueueHead, sizeof(EHCI_QueueHead));  /* CLEAN should be called before writing */

                    DCACHE_CLEAN_BY_ADDR((uint32_t *)irp->setup, sizeof(irp->setup));        /* CLEAN should be called before writing */
                    DCACHE_CLEAN_BY_ADDR((uint32_t *)setupPacket, sizeof(setupPacket));      /* CLEAN should be called before writing */
                    DCACHE_INVALIDATE_BY_ADDR((uint32_t *)(irp->setup), sizeof(irp->setup)); /* INVALIDATE should be called before reading */

                    point = (uint8_t *)irp->setup;
                    for (i = 0; i < 8; i++)
                    {
                        setupPacket[i] = point[i];
                    }

                    /* SETUP Transaction */
                    idx      = 0;
                    idx_plus = idx + 1;
                    DCACHE_CLEAN_BY_ADDR((uint32_t *)setupPacket, sizeof(setupPacket)); /* CLEAN should be called before writing */
                    /* Fill EHCI_QueueTD[0] With SETUP data */
                    ehci_create_qTD(&EHCI_QueueTD[idx],       /* qTD address base */
                                    &EHCI_QueueTD[idx_plus],  /* next qTD address base, 32-Byte align */
                                    0,                        /* Terminate */
                                    2,                        /* PID: Setup=2 */
                                    DToggle,                  /* data toggle */
                                    8,                        /* Total Bytes to transfer */
                                    OnComplete,               /* Interrupt on Complete */
                                    (uint32_t *)setupPacket); /* data buffer address base, 32-Byte align */

                    if (*((uint8_t *)(irp->setup)) & 0x80)
                    {
                        /* (1<<7) Device to host */

                        /* This function will recover the count of the received data/
                         * and then unload the pipe FIFO. */

                        /* Copy the data from the FIFO0 to the application
                         * buffer and then update the complete byte count
                         * and clear the RX packet ready bit */
                        nbBytes = irp->size;
                        stop = 0;
                        do
                        {
                            if (nbBytes > pipe->endpointSize)
                            {
                                tosend = pipe->endpointSize;
                            }
                            else
                            {
                                tosend = nbBytes;
                            }

                            idx++;
                            idx_plus = idx + 1;

                            /* The host sends an IN packet to allow the device to send the descriptor. */
                            /* Fill EHCI_QueueTD[1] With received data */
                            /* IN transaction */
                            DCACHE_CLEAN_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* CLEAN should be called before writing */
                            ehci_create_qTD(&EHCI_QueueTD[idx],                                           /* qTD address base */
                                            &EHCI_QueueTD[idx_plus],                                      /* next qTD address base, 32-Byte align */
                                            0,                                                            /* Terminate */
                                            1,                                                            /* PID: IN = 1 */
                                            (++DToggle)&0x01,                                             /* data toggle */
                                            tosend,                                                       /* Total Bytes to transfer */
                                            0,                                                            /* Interrupt on Complete */
                                            (uint32_t *)(USBBufferAligned + irp->completedBytes));        /* data buffer address base, 32-Byte align */

                            if (nbBytes > pipe->endpointSize)
                            {
                                nbBytes             -= pipe->endpointSize;
                                irp->completedBytes += pipe->endpointSize;
                            }
                            else
                            {
                                stop = 1;
                                irp->completedBytes += irp->size;
                            }
                        } while (stop == 0);

                        /* The host issues a zero-length OUT packet to acknowledge reception of the descriptor. */
                        idx++;
                        ehci_create_qTD(&EHCI_QueueTD[idx], /* qTD address base */
                                        NULL,               /* next qTD address base, 32-Byte align */
                                        1,                  /* Terminate */
                                        0,                  /* PID: OUT = 0 */
                                        1,                  /* data toggle, STATUS is always DATA1 */
                                        0,                  /* Total Bytes to transfer: ZLP */
                                        1,                  /* Interrupt on Complete */
                                        NULL);              /* data buffer address base, 32-Byte align */
                    }
                    else
                    {
                        /* (1<<7) Host to Device */
                        if (irp->size != 0)
                        {
                            /* OUT Packet */
                            idx++;
                            idx_plus = idx + 1;

                            point = (uint8_t *)irp->data;
                            DCACHE_CLEAN_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* CLEAN should be called before writing */
                            for (i = 0; i < irp->size; i++)
                            {
                                USBBufferAligned[i] = point[i];
                            }

                            DCACHE_CLEAN_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* CLEAN should be called before writing */
                            ehci_create_qTD(&EHCI_QueueTD[idx],                                           /* qTD address base */
                                            &EHCI_QueueTD[idx_plus],                                      /* next qTD address base, 32-Byte align */
                                            0,                                                            /* Terminate */
                                            0,                                                            /* PID: out=0 */
                                            1,                                                            /* data toggle */
                                            irp->size,                                                    /* Total Bytes to transfer */
                                            0,                                                            /* Interrupt on Complete */
                                            (uint32_t *)USBBufferAligned);
                        }
                        /* IN: ZLP */
                        /* The host sends an IN packet to allow the device to send the descriptor. */
                        idx++;
                        ehci_create_qTD(&EHCI_QueueTD[idx], /* qTD address base */
                                        NULL,               /* next qTD address base, 32-Byte align */
                                        1,                  /* Terminate */
                                        1,                  /* PID: IN = 1 */
                                        1,                  /* data toggle */
                                        0,                  /* Total Bytes to transfer */
                                        0,                  /* Interrupt on Complete */
                                        NULL);
                    }
                    DCACHE_CLEAN_BY_ADDR((uint32_t *)EHCI_QueueTD, sizeof(EHCI_QueueTD));              /* CLEAN should be called before writing */
                    DCACHE_INVALIDATE_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* INVALIDATE should be called before reading */
                }
                else
                {
                    /* For non control transfers, if this is the first
                     * irp in the queue, then we can start the irp */
                    memset(EHCI_QueueHead, 0, sizeof(EHCI_QueueHead));

                    /* Create Queue Head for the command: */
                    ehci_create_queue_head(&EHCI_QueueHead[0],     /* Queue Head base address */
                                           &EHCI_QueueHead[0],     /* Queue Head Link Pointer */
                                           1,                      /* Terminate */
                                   pipe->endpointAndDirection&0xF, /* Endpoint number */
                                           pipe->deviceAddress,     /* Device Address */
                                           &EHCI_QueueTD[0],        /* Next qTD Pointer */
                                           1,                       /* High-Bandwidth Pipe Multiplier (Mul) */
                                           0,                       /* Interrupt Schedule Mask (S-Mask) */
                                           1,                       /* Initial data toggle comes from incoming qTD DT bit. */
                                           pipe->hubAddress,
                                           pipe->hubPort);
                    DCACHE_CLEAN_BY_ADDR(EHCI_QueueHead, sizeof(EHCI_QueueHead));  /* CLEAN should be called before writing */
 
                    /* BULK Transfer */
                    idx = 0;
                    idx_plus = idx + 1;

                    if( (pipe->endpointAndDirection & 0x80) == 0 )
                    {
                        /* Host to Device: OUT */
                        /* Data is moving from device to host
                         * We need to set the Rx Packet Request bit */
                        memset(EHCI_QueueTD, 0, sizeof(EHCI_QueueTD));
                        DCACHE_INVALIDATE_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* INVALIDATE should be called before reading */
                        memset(USBBufferAligned, 0, sizeof(USBBufferAligned));
                        DCACHE_INVALIDATE_BY_ADDR((uint32_t *)USBBufferCSWAligned, sizeof(USBBufferCSWAligned)); /* INVALIDATE should be called before reading */
                        memset(USBBufferCSWAligned, 0, sizeof(USBBufferCSWAligned));

                        nbBytes = irp->size;
                        stop = 0;
                        do
                        {
                            if (nbBytes > pipe->endpointSize)
                            {
                                tosend = pipe->endpointSize;
                                OnComplete = 0;
                            }
                            else
                            {
                                tosend = nbBytes;
                                OnComplete = 1;
                            }
                            point = (uint8_t *)irp->data + irp->completedBytes;
                            DCACHE_CLEAN_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* CLEAN should be called before writing */
                            for (i = 0; i < tosend; i++)
                            {
                                USBBufferAligned[i] = point[i];
                            }
                            DCACHE_CLEAN_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned));      /* CLEAN should be called before writing */
                            ehci_create_qTD(&EHCI_QueueTD[idx],                                                /* qTD address base */
                                            &EHCI_QueueTD[idx_plus],                                           /* next qTD address base, 32-Byte align */
                                            OnComplete,                                                                 /* Terminate */
                                            0,                                                                 /* PID: OUT = 0 */
                                            hDriver->staticDToggleOut&0x1,                                     /* data toggle */
                                            tosend,                                                            /* Total Bytes to transfer */
                                            OnComplete,                                                        /* Interrupt on Complete */
                                            (uint32_t *)USBBufferAligned);                                     /* data buffer address base, 32-Byte align */
                            DCACHE_CLEAN_BY_ADDR((uint32_t *)EHCI_QueueTD, sizeof(EHCI_QueueTD));              /* CLEAN should be called before writing */
                            DCACHE_INVALIDATE_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* INVALIDATE should be called before reading */
                            hDriver->staticDToggleOut++;

                            idx++;
                            idx_plus = idx + 1;
                            if (nbBytes > pipe->endpointSize)
                            {
                                nbBytes             -= pipe->endpointSize;
                                irp->completedBytes += pipe->endpointSize;
                            }
                            else
                            {
                                stop = 1;
                                irp->completedBytes += irp->size;
                            }
                        } while (stop == 0);
                    }
                    else
                    {
                        /* Device to Host: IN */
                        /* Data is moving from host to device. We
                         * need to copy data into the FIFO and
                         * then and set the TX request bit. If the
                         * IRP size is greater than endpoint size then
                         * we must packetize. */
                        memset(EHCI_QueueTD, 0, sizeof(EHCI_QueueTD));
                        DCACHE_INVALIDATE_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* INVALIDATE should be called before reading */
                        memset(USBBufferAligned, 0, sizeof(USBBufferAligned));
                        DCACHE_INVALIDATE_BY_ADDR((uint32_t *)USBBufferCSWAligned, sizeof(USBBufferCSWAligned)); /* INVALIDATE should be called before reading */
                        memset(USBBufferCSWAligned, 0, sizeof(USBBufferCSWAligned));

                        nbBytes = irp->size;
                        stop = 0;
                        do
                        {
                            if (nbBytes > pipe->endpointSize)
                            {
                                tosend = pipe->endpointSize;
                                OnComplete = 0;
                            }
                            else
                            {
                                tosend = nbBytes;
                                OnComplete = 1;
                            }

                            DCACHE_INVALIDATE_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* INVALIDATE should be called before reading */
                            ehci_create_qTD(&EHCI_QueueTD[idx],             /* qTD address base */
                                            &EHCI_QueueTD[idx_plus],        /* next qTD address base, 32-Byte align */
                                            OnComplete,                     /* Terminate */
                                            1,                              /* PID: IN = 1 */
                                            hDriver->staticDToggleIn&0x1,   /* data toggle */
                                            tosend,                         /* Total Bytes to transfer */
                                            OnComplete,                     /* Interrupt on Complete */
                                            (uint32_t *)USBBufferAligned + irp->completedBytes);  /* data buffer address base, 32-Byte align */
                            DCACHE_CLEAN_BY_ADDR((uint32_t *)EHCI_QueueTD, sizeof(EHCI_QueueTD));              /* CLEAN should be called before writing */
                            DCACHE_INVALIDATE_BY_ADDR((uint32_t *)USBBufferAligned, sizeof(USBBufferAligned)); /* INVALIDATE should be called before reading */

                            hDriver->staticDToggleIn++;
                            idx++;
                            idx_plus = idx + 1;
                            if (nbBytes > pipe->endpointSize)
                            {
                                nbBytes             -= pipe->endpointSize;
                                irp->completedBytes += pipe->endpointSize;
                            }
                            else
                            {
                                stop = 1;
                                irp->completedBytes += irp->size;
                            }
                        } while (stop == 0);
                    }                                            
                }
                irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
                returnValue = USB_ERROR_NONE;
            }
            else
            {
                /* We need to add the irp to the last irp
                 * in the pipe queue (which is a linked list) */
                irpIterator = pipe->irpQueueHead;

                /* Find the last IRP in the linked list*/
                while(irpIterator->next != 0)
                {
                    irpIterator = irpIterator->next;
                }

                /* Add the item to the last irp in the linked list */
                irpIterator->next = irp;
                irp->previous = irpIterator;
            }
            usbID = hDriver->usbID;

            /* async list addr */
            usbID->UHPHS_ASYNCLISTADDR = (uint32_t)&EHCI_QueueHead[0];

            /* USB Command is send here
             * In order to communicate with devices via the asynchronous schedule, system software must write the
             * ASYNDLISTADDR register with the address of a control or bulk queue head. Software must then enable
             * the asynchronous schedule by writing a one to the Asynchronous Schedule Enable bit in the USBCMD register. */
            usbID->UHPHS_USBCMD |= UHPHS_USBCMD_ASE_Msk; /* | UHPHS_USBCMD_IAAD;  // async enable = 1 */
            /* ASE: Asynchronous Schedule Enable: Use the UHPHS_ASYNCLISTADDR register to access the Asynchronous Schedule. */

            if(!hDriver->isInInterruptContext)
            {
                if(controlTransferGroup->interruptWasEnabled)
                {
                    _DRV_USB_UHP_InterruptSourceEnable(hDriver->interruptSource);
                }
                /* OSAL: Return Mutex */
                if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex unlock failed");
                }
                usbID->UHPHS_USBINTR = UHPHS_USBINTR_USBIE_Msk  /* (UHPHS_USBINTR) USB Interrupt Enable */
                                     | UHPHS_USBINTR_USBEIE_Msk /* (UHPHS_USBINTR) USB Error Interrupt Enable */
                                     | UHPHS_USBINTR_PCIE_Msk   /* (UHPHS_USBINTR) Port Change Interrupt Enable */
                                  /* | UHPHS_USBINTR_FLRE          (UHPHS_USBINTR) Frame List Rollover Enable */
                                     | UHPHS_USBINTR_HSEE_Msk   /* (UHPHS_USBINTR) Host System Error Enable */
                                     | UHPHS_USBINTR_IAAE_Msk;  /* (UHPHS_USBINTR) Interrupt on Async Advance Enable */
            }
            irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;
        }
    }
    return (returnValue);
}/* end of DRV_USB_UHP_IRPSubmit() */

// *****************************************************************************
/* Function:
    void DRV_USB_UHP_HOST_IRPCancel(USB_HOST_IRP * pinputIRP)

  Summary:
    Cancels the specified IRP.
    
  Description:
    This function attempts to cancel the specified IRP. If the IRP is queued and
    its processing has not started, it will be cancelled successfully. If the
    IRP in progress, the ongoing transaction will be allowed to complete. 

  Remarks:
    See .h for usage information.
*/
void DRV_USB_UHP_HOST_IRPCancel(USB_HOST_IRP *inputIRP)
{
    /* This function cancels an IRP */

    USB_HOST_IRP_LOCAL          *irp = (USB_HOST_IRP_LOCAL *)inputIRP;
    DRV_USB_UHP_OBJ           *hDriver;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe;
    bool interruptWasEnabled = false;

    if (irp->pipe == DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid pipe");
        return;
    }

    if (irp->status <= USB_HOST_IRP_STATUS_COMPLETED_SHORT)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: IRP is not pending or in progress");
        return;
    }

    pipe    = (DRV_USB_UHP_HOST_PIPE_OBJ *)irp->pipe;
    hDriver = (DRV_USB_UHP_OBJ *)pipe->hClient;

    if (!hDriver->isInInterruptContext)
    {
        /* OSAL: Get Mutex */
        if (OSAL_MUTEX_Lock(&(hDriver->mutexID), OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex lock failed");
        }
        interruptWasEnabled = _DRV_USB_UHP_InterruptSourceDisable(hDriver->interruptSource);
    }

    if (irp->previous == NULL)
    {
        /* This means this was the first
         * irp in the queue. Update the pipe
         * queue head directly */

        pipe->irpQueueHead = irp->next;
        if (irp->next != NULL)
        {
            irp->next->previous = NULL;
        }
    }
    else
    {
        /* Remove the IRP from the linked
         * list */
        irp->previous->next = irp->next;

        if (irp->next != NULL)
        {
            /* This applies if this is not the last
             * irp in the linked list */
            irp->next->previous = irp->previous;
        }
    }

    if (irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)
    {
        /* If the irp is already in progress then
         * we set the temporary state. This will get
         * caught in _DRV_USB_UHP_HOST_ControlXferProcess()
         * and _DRV_USB_UHP_HOST_NonControlIRPProcess()
         * functions. */

        irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_ABORTED;
    }
    else
    {
        irp->status = USB_HOST_IRP_STATUS_ABORTED;
        if (irp->callback != NULL)
        {
            irp->callback((USB_HOST_IRP *)irp);
        }
    }

    if (!hDriver->isInInterruptContext)
    {
        if (interruptWasEnabled)
        {
            _DRV_USB_UHP_InterruptSourceEnable(hDriver->interruptSource);
        }
        /* OSAL: Release Mutex */
        if (OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex unlock failed");
        }
    }
}/* end of DRV_USB_UHP_IRPCancel() */

// *****************************************************************************
/* Function:
     void DRV_USB_UHP_HOST_PipeClose(DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle)

  Summary:
    Closes an open pipe.
	
  Description:
    This function closes an open pipe. Any IRPs scheduled on the pipe will be
    aborted and IRP callback functions will be called with the status as
    DRV_USB_HOST_IRP_STATE_ABORTED. The pipe handle will become invalid and the
    pipe will not accept IRPs.

  Remarks:
    See .h for usage information.
*/
void DRV_USB_UHP_HOST_PipeClose
(
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
)
{
    /* This function closes an open pipe */
    bool interruptWasEnabled = false;

    DRV_USB_UHP_OBJ           *hDriver;
    USB_HOST_IRP_LOCAL          *irp;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe;
    DRV_USB_UHP_HOST_ENDPOINT_OBJ *endpointObj;

    /* Make sure we have a valid pipe */
    if ((pipeHandle == 0) || (pipeHandle == DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid pipe handle");
        return;
    }

    pipe = (DRV_USB_UHP_HOST_PIPE_OBJ *)pipeHandle;

    /* Make sure tha we are working with a pipe in use */
    if (pipe->inUse != true)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Pipe is not in use");
        return;
    }

    hDriver = (DRV_USB_UHP_OBJ *)pipe->hClient;

    /* Disable the interrupt */
    if (!hDriver->isInInterruptContext)
    {
        /* OSAL: Get Mutex */
        if (OSAL_MUTEX_Lock(&hDriver->mutexID, OSAL_WAIT_FOREVER) !=
            OSAL_RESULT_TRUE)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex lock failed");
        }
        interruptWasEnabled = _DRV_USB_UHP_InterruptSourceDisable(hDriver->interruptSource);
    }

    /* Non control transfer pipes are not stored as groups.  We deallocate
     * the endpoint object that this pipe used */

    endpointObj = &hDriver->hostEndpointTable[pipe->hostPipeN];
    endpointObj->endpoint.inUse = false;
    endpointObj->endpoint.pipe  = NULL;

    /* Now we invoke the call back for each IRP in this pipe and say that it is
     * aborted.  If the IRP is in progress, then that IRP will be actually
     * aborted on the next SOF This will allow the USB module to complete any
     * transaction that was in progress. */

    irp = (USB_HOST_IRP_LOCAL *)pipe->irpQueueHead;
    while (irp != NULL)
    {
        irp->pipe = DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID;

        if (irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)
        {
            /* If the IRP is in progress, then we set the temp IRP state. This
             * will be caught in the DRV_USB_UHP_HOST_NonControlTransferProcess() and
             * _DRV_USB_UHP_HOST_ControlXferProcess() functions */
            irp->status = USB_HOST_IRP_STATUS_ABORTED;
            if (irp->callback != NULL)
            {
                irp->callback((USB_HOST_IRP *)irp);
            }
            irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_ABORTED;
        }
        else
        {
            /* IRP is pending */
            irp->status = USB_HOST_IRP_STATUS_ABORTED;
            if (irp->callback != NULL)
            {
                irp->callback((USB_HOST_IRP *)irp);
            }
        }
        irp = irp->next;
    }

    /* Now we return the pipe back to the driver */
    pipe->inUse = false;

    /* Enable the interrupts */
    if (!hDriver->isInInterruptContext)
    {
        if (interruptWasEnabled)
        {
            _DRV_USB_UHP_InterruptSourceEnable(hDriver->interruptSource);
        }
        /* OSAL: Return Mutex */
        if (OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex unlock failed");
        }
    }
}/* end of DRV_USB_UHP_HOST_PipeClose() */


// *****************************************************************************
/* Function:
    DRV_USB_UHP_HOST_PIPE_HANDLE DRV_USB_UHP_HOST_PipeSetup
    (
        DRV_HANDLE handle,
        uint8_t deviceAddress,
        USB_ENDPOINT endpointAndDirection,
        uint8_t hubAddress,
        uint8_t hubPort,
        USB_TRANSFER_TYPE pipeType,
        uint8_t bInterval,
        uint16_t wMaxPacketSize,
        USB_SPEED speed
    )

  Summary:
    Open a pipe with the specified attributes.
	
  Description:
    This function opens a communication pipe between the Host and the device
    endpoint. The transfer type and other attributes are specified through the
    function parameters. The driver does not check for available bus bandwidth,
    which should be done by the application (the USB Host Layer in this case)

  Remarks:
    See drv_xxx.h for usage information.
*/
DRV_USB_UHP_HOST_PIPE_HANDLE DRV_USB_UHP_HOST_PipeSetup
(
    DRV_HANDLE        client,
    uint8_t           deviceAddress,
    USB_ENDPOINT      endpointAndDirection,
    uint8_t           hubAddress,
    uint8_t           hubPort,
    USB_TRANSFER_TYPE pipeType,
    uint8_t           bInterval,
    uint16_t          wMaxPacketSize,
    USB_SPEED         speed
)
{
    int pipeIter;
    DRV_USB_UHP_OBJ           *hDriver;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe;
    bool epFound;

    if (client == DRV_HANDLE_INVALID)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid client handle");
        return DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID;
    }

    if ((speed == USB_SPEED_LOW) || (speed == USB_SPEED_FULL) || (speed == USB_SPEED_HIGH))
    {
        if (pipeType != USB_TRANSFER_TYPE_CONTROL)
        {
            if (wMaxPacketSize < 8)
            {
                wMaxPacketSize = 8;
            }
        }
    }
    else
    {
        return DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID;
    }

    if ((wMaxPacketSize < 8) || (wMaxPacketSize > 4096))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid pipe endpoint size");
        return DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID;
    }

    hDriver = (DRV_USB_UHP_OBJ *)client;

    /* OSAL: Mutex Lock */
    if (OSAL_MUTEX_Lock(&hDriver->mutexID, OSAL_WAIT_FOREVER) !=
        OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex lock failed");
        /* return USB_ERROR_OSAL_FUNCTION; */
        return DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID;
    }

    if (pipeType == USB_TRANSFER_TYPE_CONTROL)
    {
        /* Set pipeIter to zero to indicate that we must use pipe 0
         * for control transfers. We also add the control transfer pipe
         * to the control transfer group. */
        pipeIter = 0;

        if (wMaxPacketSize < 8)
        {
            return DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID;
        }

        /* Enable Pipe */
        epFound = true;
        pipe    = &gDrvUSBHostPipeObj[pipeIter];
    }
    else
    {
        /* Pipe allocation for non-control transfer */
        for (pipeIter = 1; pipeIter < DRV_USB_UHP_PIPES_NUMBER; pipeIter++)
        {
            if (false == gDrvUSBHostPipeObj[pipeIter].inUse)
            {
                /* This means we have found a free pipe object */
                epFound = true;
                pipe    = &gDrvUSBHostPipeObj[pipeIter];
                hDriver->hostEndpointTable[pipeIter].endpoint.inUse = true;
                hDriver->hostEndpointTable[pipeIter].endpoint.pipe  = pipe;
                break;
            }
        }
    }

    if (!epFound)
    {
        /* This means we could not find a spare endpoint for this
         * non control transfer. */

        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Could not allocate endpoint");
        /* OSAL: Mutex Unlock */
        if (OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex unlock failed");
        }
        return DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID;
    }

    /* Setup the pipe object */
    pipe->inUse                = true;
    pipe->deviceAddress        = deviceAddress;
    pipe->irpQueueHead         = NULL;
    pipe->bInterval            = bInterval;
    pipe->speed                = speed;
    pipe->hubAddress           = hubAddress;
    pipe->hubPort              = hubPort;
    pipe->pipeType             = pipeType;
    pipe->hClient              = client;
    pipe->endpointSize         = wMaxPacketSize;
    pipe->intervalCounter      = bInterval;
    pipe->hostPipeN            = pipeIter;
    pipe->endpointAndDirection = endpointAndDirection;

    /* OSAL: Release Mutex */
    if (OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP Mutex unlock failed");
    }
    return((DRV_USB_UHP_HOST_PIPE_HANDLE)pipe);
}/* end of DRV_USB_UHP_HOST_PipeSetup() */

// *****************************************************************************
/* Function:
    void _DRV_USB_UHP_HOST_Tasks_ISR(DRV_USB_UHP_OBJ *hDriver)

  Summary:
    Interrupt handler
	
  Description:
    Management of all EHCI interrupt

  Remarks:
    See drv_xxx.h for usage information.
*/
void _DRV_USB_UHP_HOST_Tasks_ISR(DRV_USB_UHP_OBJ *hDriver)
{
    uint32_t isr_read_data;
    uint32_t read_data;
    uint32_t i;
    DRV_USB_UHP_HOST_TRANSFER_GROUP *transferGroup;
    volatile uhphs_registers_t        *usbID = hDriver->usbID;

    transferGroup = &hDriver->controlTransferGroup;

    /* interruptStatus = PLIB_USB_UHP_GenInterruptFlagsGet(usbID); */
    /* OHCI suspend interrupt from SFR */
    isr_read_data = SFR_REGS->SFR_OHCIISR;

    if ((isr_read_data & ((1<<NUMBER_OF_PORTS) - 1)) != 0x00)
    {
        /* Disable interrupt */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rIT Disable interrupt");
        /* OHCI Asynchronous Resume Interrupt Disable */
        SFR_REGS->SFR_OHCIICR &= ~SFR_OHCIICR_ARIE_Msk;
    }
    else
    {
        /* EHCI interrupts */
        isr_read_data = usbID->UHPHS_USBINTR;
        isr_read_data &= usbID->UHPHS_USBSTS;

        if (isr_read_data != 0)
        {
            /* Interrupt on Async Advance */
            if ((isr_read_data & UHPHS_USBSTS_IAA_Msk) == UHPHS_USBSTS_IAA_Msk)
            {
                transferGroup->int_on_async_advance = 1;
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rEHCI interrupt on async advance");
                usbID->UHPHS_USBSTS = UHPHS_USBSTS_IAA_Msk;
            }

            /* Host system error */
            if ((isr_read_data & UHPHS_USBSTS_HSE_Msk) == UHPHS_USBSTS_HSE_Msk)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rEHCI Host system error interrupt\033[0m");
                usbID->UHPHS_USBSTS = UHPHS_USBSTS_HSE_Msk;
            }

            /* Frame list Rollover */
            if ((isr_read_data & UHPHS_USBSTS_FLR_Msk) == UHPHS_USBSTS_FLR_Msk)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rEHCI Frame list Rollover interrupt\033[0m");
                usbID->UHPHS_USBSTS = UHPHS_USBSTS_FLR_Msk;
            }

            /* Port Change Detect */
            if ((isr_read_data & UHPHS_USBSTS_PCD_Msk) == UHPHS_USBSTS_PCD_Msk)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rEHCI port change interrupt");
                for (i = 0; i < NUMBER_OF_PORTS; i++)
                {
                    /* read_data = usbID->UHPHS_PORTSC_0; */
                    read_data = *((uint32_t *)&(usbID->UHPHS_PORTSC_0) + i);
                    /* Over-current Change */
                    if ((read_data & UHPHS_PORTSC_0_OCC_Msk) == UHPHS_PORTSC_0_OCC_Msk)
                    {
                        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOver-current Change on port %d", (int)i);
                    }
                    /* Port Enable/Disable Change */
                    if ((read_data & UHPHS_PORTSC_0_PEDC_Msk) == UHPHS_PORTSC_0_PEDC_Msk)
                    {
                        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rPort Enable/Disable Change on port %d", (int)i);
                    }
                    /* Connect Status Change */
                    if ((read_data & UHPHS_PORTSC_0_CSC_Msk) == UHPHS_PORTSC_0_CSC_Msk)
                    {
                        /* 1=Device is present on port. */
                        if (((*((uint32_t *)&(usbID->UHPHS_PORTSC_0) + i)) & UHPHS_PORTSC_0_CCS_Msk) == UHPHS_PORTSC_0_CCS_Msk)
                        {
                            hDriver->deviceAttached = true;
                            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rConnect port %d", (int)i);
                            hDriver->portNumber = i;
                        }
                        else
                        {
                            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rDisconnect occurred on port %d", (int)i);

                            /* We go a detach interrupt. The detach interrupt could have occurred
                             * while the attach de-bouncing is in progress. We just set a flag saying
                             * the device is detached; */

                            hDriver->deviceAttached = false;

                            if (hDriver->attachedDeviceObjHandle != USB_HOST_DEVICE_OBJ_HANDLE_INVALID)
                            {
                                /* Ask the host layer to de-enumerate this device. The de-enumeration
                                 * must be done in the interrupt context. */
                                USB_HOST_DeviceDenumerate(hDriver->attachedDeviceObjHandle);
                            }

                            hDriver->attachedDeviceObjHandle = USB_HOST_DEVICE_OBJ_HANDLE_INVALID;

                            hDriver->controlTransferGroup.currentIRP = NULL;
                            hDriver->controlTransferGroup.currentPipe = NULL;
                            hDriver->hostPipeInUse = 0;
                            hDriver->staticDToggleOut = 0;
                            hDriver->staticDToggleIn = 0;
                        }
                    }
                }
                usbID->UHPHS_USBSTS = UHPHS_USBSTS_PCD_Msk; /* clear by wrting "1" = Port Change Detect */
            }

            /* USB error */
            if ((isr_read_data & UHPHS_USBSTS_USBERRINT_Msk) == UHPHS_USBSTS_USBERRINT_Msk)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rEHCI IRQ : USB error interrupt\033[0m");
                /* Clear It */
                usbID->UHPHS_USBSTS = UHPHS_USBSTS_USBERRINT_Msk;
                ehci_TestTd();
            }

            /* USB Interrupt (USBINT) R/WC */
            /* The Host Controller sets this bit to 1 on the completion of a USB transaction, which results in the retirement of a Transfer Descriptor */
            /* that had its IOC bit set. */
            /* The Host Controller also sets this bit to 1 when a short packet is detected (actual */
            /* number of bytes received was less than the expected number of bytes). */
            if ((isr_read_data & UHPHS_USBSTS_USBINT_Msk) == UHPHS_USBSTS_USBINT_Msk)
            {
                usbID->UHPHS_USBSTS           = UHPHS_USBSTS_USBINT_Msk; /* clear by wrting "1" */
                hDriver->int_xfr_qtd_complete = 1;
            }
        }
        else
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "Int_PB ");
        }
    }

    DRV_USB_UHP_HOST_TransferProcess(hDriver);

}/* end of _DRV_USB_UHP_HOST_Tasks_ISR() */


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_HOST_StartOfFrameControl(DRV_HANDLE client, bool control)

  Summary:
    SOF
	
  Description:
    Management of SOF: nothing to do

  Remarks:
    See drv_xxx.h for usage information.
*/
void DRV_USB_UHP_HOST_StartOfFrameControl(DRV_HANDLE client, bool control)
{
    /* At the point this function does not do any thing.
     * The Start of frame signaling in this HCD is controlled
     * automatically by the module. */
}/* end of DRV_USB_UHP_HOST_StartOfFrameControl() */

// *****************************************************************************
/* Function:
    USB_SPEED DRV_USB_UHP_HOST_DeviceCurrentSpeedGet(DRV_HANDLE client)

  Summary:
    Current speed
	
  Description:
    Get current usb speed of the connected device

  Remarks:
    See drv_xxx.h for usage information.
*/
USB_SPEED DRV_USB_UHP_HOST_DeviceCurrentSpeedGet(DRV_HANDLE client)
{
    /* This function returns the current device speed */
    DRV_USB_UHP_OBJ *hDriver;

    if ((client == DRV_HANDLE_INVALID) || (((DRV_USB_UHP_OBJ *)client) == NULL))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid client");
    }

    hDriver = (DRV_USB_UHP_OBJ *)client;
    return(hDriver->deviceSpeed);
}/* end of DRV_USB_UHP_HOST_DeviceCurrentSpeedGet() */

/* **************************************************************************** */
/* Function:
    bool DRV_USB_UHP_HOST_EventDisable
    (
        DRV_HANDLE handle
    );

   Summary:
    Disables host mode events.

   Description:
    This function disables the host mode events. This function is called by the
    Host Layer when it wants to execute code atomically.

   Remarks:
    Refer to .h for usage information.
 */
bool DRV_USB_UHP_HOST_EventsDisable
(
    DRV_HANDLE handle
)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj;
    bool              result = false;

    if ((DRV_HANDLE_INVALID != handle) && (0 != handle))
    {
        pUSBDrvObj = (DRV_USB_UHP_OBJ *)(handle);
        result     = _DRV_USB_UHP_InterruptSourceDisable(pUSBDrvObj->interruptSource);
    }
    return(result);
}

/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_HOST_EventsEnable
    (
        DRV_HANDLE handle
        bool eventRestoreContext
    );

   Summary:
    Restores the events to the specified the original value.

   Description:
    This function will restore the enable disable state of the events.
    eventRestoreContext should be equal to the value returned by the
    DRV_USB_UHP_HOST_EventsDisable() function.

   Remarks:
    Refer to .h for usage information.
 */
void DRV_USB_UHP_HOST_EventsEnable
(
    DRV_HANDLE handle,
    bool       eventContext
)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj;

    if ((DRV_HANDLE_INVALID != handle) && (0 != handle))
    {
        pUSBDrvObj = (DRV_USB_UHP_OBJ *)(handle);
        if (false == eventContext)
        {
            _DRV_USB_UHP_InterruptSourceDisable(pUSBDrvObj->interruptSource);
        }
        else
        {
            _DRV_USB_UHP_InterruptSourceEnable(pUSBDrvObj->interruptSource);
        }
    }
}

/* ***************************************************************************** */
/* ***************************************************************************** */
/* Root Hub Driver Routines */
/* ***************************************************************************** */
/* ***************************************************************************** */

/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_HOST_ROOT_HUB_OperationEnable(DRV_HANDLE handle, bool enable)

   Summary:
    Root hub enable

   Description:
    

   Remarks:
    Refer to .h for usage information.
 */
void DRV_USB_UHP_HOST_ROOT_HUB_OperationEnable(DRV_HANDLE handle, bool enable)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    volatile uhphs_registers_t *usbID = pUSBDrvObj->usbID;

    /* Check if the handle is valid or opened */
    if ((handle == DRV_HANDLE_INVALID) || (!(pUSBDrvObj->isOpened)))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        if (false == enable)
        {
            /* If the root hub operation is disable, we disable detach and
             * attached event and switch off the port power. */
            usbID->UHPHS_USBINTR &= ~UHPHS_USBINTR_USBIE_Msk;
            pUSBDrvObj->operationEnabled = false;
        }
        else
        {
            /* The USB Global interrupt and USB module is already enabled at
             * this point. We enable the attach interrupt to detect attach
             */

            pUSBDrvObj->operationEnabled = true;
            /* Enable Device Connection Interrupt */
            /* Enable the attach and detach interrupt and EP0 interrupt. */
            usbID->UHPHS_USBINTR = UHPHS_USBINTR_USBIE_Msk    /* (UHPHS_USBINTR) USB Interrupt Enable */
                                   | UHPHS_USBINTR_USBEIE_Msk /* (UHPHS_USBINTR) USB Error Interrupt Enable */
                                   | UHPHS_USBINTR_PCIE_Msk   /* (UHPHS_USBINTR) Port Change Interrupt Enable */
                                                              /* | UHPHS_USBINTR_FLRE_Msk      (UHPHS_USBINTR) Frame List Rollover Enable */
                                   | UHPHS_USBINTR_HSEE_Msk   /* (UHPHS_USBINTR) Host System Error Enable */
                                   | UHPHS_USBINTR_IAAE_Msk;  /* (UHPHS_USBINTR) Interrupt on Async Advance Enable */

            /* Enable VBUS */
            if(pUSBDrvObj->rootHubInfo.portPowerEnable != NULL)
            {
                /* This USB module has only one port. So we call this function
                 * once to enable the port power on port 0*/
                pUSBDrvObj->rootHubInfo.portPowerEnable(0 /* Port 0 */, true);
            }
        }
    }
} /* end of DRV_USB_UHP_ROOT_HUB_OperationEnable() */

/* **************************************************************************** */
/* Function:
    bool DRV_USB_UHP_HOST_ROOT_HUB_OperationIsEnabled(DRV_HANDLE hClient)

   Summary:
    Root hub enable

   Description:
    return true if the HUB is operational and enabled.

   Remarks:
    Refer to .h for usage information.
 */
bool DRV_USB_UHP_HOST_ROOT_HUB_OperationIsEnabled(DRV_HANDLE hClient)
{
    DRV_USB_UHP_OBJ *hDriver;

    if ((hClient == DRV_HANDLE_INVALID) || (((DRV_USB_UHP_OBJ *)hClient) == NULL))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid client");
    }

    hDriver = (DRV_USB_UHP_OBJ *)hClient;
    return(hDriver->operationEnabled);
}/* end of DRV_USB_UHP_ROOT_HUB_OperationIsEnabled() */

/* **************************************************************************** */

/* Function:
    void DRV_USB_UHP_HOST_ROOT_HUB_Initialize
    (
        DRV_HANDLE handle,
        USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo,
    )

   Summary:
    This function instantiates the root hub driver.

   Description:
    This function initializes the root hub driver. It is called by the host
    layer at the time of processing root hub devices. The host layer assigns a
    USB_HOST_DEVICE_OBJ_HANDLE reference to this root hub driver. This
    identifies the relationship between the root hub and the host layer.

   Remarks:
    None.
 */
void DRV_USB_UHP_HOST_ROOT_HUB_Initialize
(
    DRV_HANDLE                 handle,
    USB_HOST_DEVICE_OBJ_HANDLE usbHostDeviceInfo
)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;

    if (DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if (!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        pUSBDrvObj->usbHostDeviceInfo = usbHostDeviceInfo;
    }
}

/* **************************************************************************** */

/* Function:
    uint8_t DRV_USB_UHP_ROOT_HUB_PortNumbersGet(DRV_HANDLE handle);

   Summary:
    Returns the number of ports this root hub contains.

   Description:
    This function returns the number of ports that this root hub contains.

   Remarks:
    None.
 */
uint8_t DRV_USB_UHP_HOST_ROOT_HUB_PortNumbersGet(DRV_HANDLE handle)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    uint8_t           result      = 0;

    if (DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if (!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        result = 1;
    }

    return(result);
}

/* **************************************************************************** */

/* Function:
    uint32_t DRV_USB_UHP_ROOT_HUB_MaximumCurrentGet(DRV_HANDLE);

   Summary:
    Returns the maximum amount of current that this root hub can provide on the
    bus.

   Description:
    This function returns the maximum amount of current that this root hubn can
    provide on the bus.

   Remarks:
    Refer to .h for usage information.
 */
uint32_t DRV_USB_UHP_HOST_ROOT_HUB_MaximumCurrentGet(DRV_HANDLE handle)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    uint32_t          result      = 0;

    if (DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if (!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        result = pUSBDrvObj->rootHubInfo.rootHubAvailableCurrent;
    }

    return(result);
}

/* **************************************************************************** */

/* Function:
    USB_SPEED DRV_USB_UHP_ROOT_HUB_BusSpeedGet(DRV_HANDLE handle);

   Summary:
    Returns the speed at which the bus to which this root hub is connected is
    operating.

   Description:
    This function returns the speed at which the bus to which this root hub is
    connected is operating.

   Remarks:
    None.
 */
USB_SPEED DRV_USB_UHP_HOST_ROOT_HUB_BusSpeedGet(DRV_HANDLE handle)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    USB_SPEED         speed       = USB_SPEED_ERROR;
    DRV_USB_UHP_OBJ *hDriver;

    if (DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if (!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        /* Return the bus speed. This is speed at which the root hub is
         * operating. */
        hDriver = (DRV_USB_UHP_OBJ *)handle;
        speed = hDriver->deviceSpeed;
    }

    return(speed);
}

/* **************************************************************************** */

/* Function:
    void DRV_USB_UHP_ROOT_HUB_PortResume(DRV_HANDLE handle, uint8_t port );

   Summary:
    Resumes the specified root hub port.

   Description:
    This function resumes the root hub. The resume duration is defined by
    DRV_USB_UHP_ROOT_HUB_RESUME_DURATION. The status of the resume signalling can
    be checked using the DRV_USB_UHP_ROOT_HUB_PortResumeIsComplete() function.

   Remarks:
    The root hub on this particular hardware only contains one port - port 0.
 */
USB_ERROR DRV_USB_UHP_HOST_ROOT_HUB_PortResume(DRV_HANDLE handle, uint8_t port)
{
    /* The functionality is yet to be implemented. */
    return(USB_ERROR_NONE);
}

/* **************************************************************************** */

/* Function:
    void DRV_USB_UHP_ROOT_HUB_PortSuspend(DRV_HANDLE handle, uint8_t port );

   Summary:
    Suspends the specified root hub port.

   Description:
    This function suspends the root hub port.

   Remarks:
    The root hub on this particular hardware only contains one port - port 0.
 */
USB_ERROR DRV_USB_UHP_HOST_ROOT_HUB_PortSuspend(DRV_HANDLE handle, uint8_t port)
{
    /* The functionality is yet to be implemented. */
    return(USB_ERROR_NONE);
}

/* **************************************************************************** */

/* Function:
    void DRV_USB_UHP_ROOT_HUB_PortResetIsComplete
    (
        DRV_HANDLE handle,
        uint8_t port
    );

   Summary:
    Returns true if the root hub has completed the port reset operation.

   Description:
    This function returns true if the port reset operation has completed. It
    should be called after the DRV_USB_HOST_ROOT_HUB_PortReset() function to
    check if the reset operation has completed.

   Remarks:
    Refer to .h for usage information.
 */
bool DRV_USB_UHP_HOST_ROOT_HUB_PortResetIsComplete
(
    DRV_HANDLE handle,
    uint8_t    port
)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    bool              result      = true;

    if (DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if (!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        _DRV_USB_UHP_HOST_ResetStateMachine((DRV_USB_UHP_OBJ *)handle);
        /* Return false if the driver is still resetting*/
        result = (pUSBDrvObj->isResetting) ? false : true;
    }

    return(result);
}

/* **************************************************************************** */

/* Function:
    void DRV_USB_UHP_ROOT_HUB_PortReset(DRV_HANDLE handle, uint8_t port );

   Summary:
    Resets the specified root hub port.

   Description:
    This function resets the root hub port. The reset duration is defined by
    DRV_USB_UHP_ROOT_HUB_RESET_DURATION. The status of the reset signalling can be
    checked using the DRV_USB_UHP_ROOT_HUB_PortResetIsComplete() function.

   Remarks:
    Refer to .h for usage information.
 */
USB_ERROR DRV_USB_UHP_HOST_ROOT_HUB_PortReset(DRV_HANDLE handle, uint8_t port)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    USB_ERROR         result      = USB_ERROR_NONE;

    if (DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
        result = USB_ERROR_PARAMETER_INVALID;
    }
    else if (!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
        result = USB_ERROR_PARAMETER_INVALID;
    }
    else if (pUSBDrvObj->isResetting)
    {
        /* This means a reset is already in progress. Lets not do anything. */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Reset already in progress");
    }
    else
    {
        /* Start the reset signal. Set the driver flag to indicate the reset
         * signal is in progress. Start generating the reset signal.
         */

        pUSBDrvObj->isResetting = true;
        pUSBDrvObj->resetState  = DRV_USB_UHP_HOST_RESET_STATE_START;
        /* Enable Reset sent interrupt */
        /* Start Reset */
        _USB_UHP_ResetEnable(pUSBDrvObj);
    }
    return(result);
}

/* **************************************************************************** */

/* Function:
    USB_SPEED DRV_USB_UHP_HOST_ROOT_HUB_PortSpeedGet
    (
        DRV_HANDLE handle,
        uint8_t port
    );

   Summary:
    Returns the speed of at which the port is operating.

   Description:
    This function returns the speed at which the port is operating.

   Remarks:
    Refer to drv_usb_uhp_ehci.h for usage information.
 */
USB_SPEED DRV_USB_UHP_HOST_ROOT_HUB_PortSpeedGet(DRV_HANDLE handle, uint8_t port)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    USB_SPEED         speed       = USB_SPEED_ERROR;

    if (DRV_HANDLE_INVALID == handle)
    {
        /* Driver handle is not valid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else if (!(pUSBDrvObj->isOpened))
    {
        /* Driver has not been opened. Handle could be stale */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        /* The driver will not check if a device is connected. It is assumed
         * that the client has issued a port reset before calling this function
         */
        speed = pUSBDrvObj->deviceSpeed;
    }
    return(speed);
}
