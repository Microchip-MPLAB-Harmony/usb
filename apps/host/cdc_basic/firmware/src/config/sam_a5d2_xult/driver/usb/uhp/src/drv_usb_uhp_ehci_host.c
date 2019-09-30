/*******************************************************************************
   USB EHCI Host Controller Driver

   Company:
    Microchip Technology Inc.

   File Name:
    drv_usb_uhp_ehci_host.c

   Summary:
    USB EHCI Host Driver Implementation.

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

#include <stdint.h>
#include <string.h>
#include "configuration.h"
#include "definitions.h"
#include "driver/usb/drv_usb.h"
#include "driver/usb/uhp/src/drv_usb_uhp_local.h"
#include "driver/usb/uhp/src/drv_usb_uhp_ehci_host.h"

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
    .hostIRPSubmit     = DRV_USB_UHP_HOST_IRPSubmitEhci,
    .hostIRPCancel     = DRV_USB_UHP_HOST_IRPCancel,
    .hostPipeSetup     = DRV_USB_UHP_HOST_PipeSetup,
    .hostPipeClose     = DRV_USB_UHP_HOST_PipeClose,
    .hostEventsDisable = DRV_USB_UHP_HOST_EventsDisable,
    .endpointToggleClear = DRV_USB_UHP_HOST_EndpointToggleClear,
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
    .rootHubInterface.rootHubOperationEnable    = DRV_USB_UHP_HOST_ROOT_HUB_OperationEnableEhci,
    .rootHubInterface.rootHubOperationIsEnabled = DRV_USB_UHP_HOST_ROOT_HUB_OperationIsEnabled
};


typedef union __attribute__ ((packed))
{
    struct
    {
        volatile uint32_t Status       : 8;
        volatile uint32_t PIDCode      : 2;
        volatile uint32_t ErrorCounter : 2;
        volatile uint32_t C_Page       : 3;
        volatile uint32_t IOC          : 1;
        volatile uint32_t TotalBytesTF : 15;
        volatile uint32_t DataToggle   : 1;
    };

    volatile uint32_t qtdtoken;
}
QTDToken;

typedef struct
{
    volatile uint32_t Next_qTD_Pointer;             /* DWord 0 */
    volatile uint32_t Alternate_Next_qTD_Pointer;   /* DWord 1 */
    volatile QTDToken qTD_Token;                    /* DWord 2 */
    volatile uint32_t qTD_Buffer_Page_Pointer_List; /* DWord 3 */
    volatile uint32_t qTD_Buffer[4];                /* DWord 4 to 7 */
} EHCIQueueTDDescriptor;

typedef struct
{
    volatile uint32_t Horizontal_Link_Pointer;  /* DWord 0 */
    volatile uint32_t Endpoint_Characteristics; /* DWord 1 */
    volatile uint32_t Endpoint_Capabilities;    /* DWord 2 */
    volatile uint32_t Current_qTD_Pointer;      /* DWord 3: processed by the host */
    volatile uint32_t Transfer_Overlay[8];      /* DWord 4 to 12 */
    volatile uint8_t dummy[0xD0];               /* Aligned 208 */
} EHCIQueueHeadDescriptor;

#define DRV_USB_UHP_MAX_TRANSACTION   10
#define NOT_CACHED __attribute__((__section__(".region_nocache")))
__ALIGNED(32) NOT_CACHED EHCIQueueHeadDescriptor EHCI_QueueHead[DRV_USB_UHP_PIPES_NUMBER]; /* Queue Head: 0x30=48 length */
__ALIGNED(32) NOT_CACHED EHCIQueueTDDescriptor EHCI_QueueTD[DRV_USB_UHP_PIPES_NUMBER][DRV_USB_UHP_MAX_TRANSACTION];  /* Queue Element Transfer Descriptor: 1 qTD is 0x20=32 */
__ALIGNED(4096) NOT_CACHED uint32_t PeriodicFrameList[1024];
extern __ALIGNED(4096) NOT_CACHED uint8_t USBBufferAligned[USB_HOST_TRANSFERS_NUMBER*64]; /* 4K page aligned */
extern __ALIGNED(4096) NOT_CACHED volatile uint8_t setupPacket[8];

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
   void USB_UHP_ResetEnableEhci(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    Reset the current port number

   Description:
    Reset the current port number to default value

   Remarks:
    Refer to .h for usage information.
 */
void USB_UHP_ResetEnableEhci(DRV_USB_UHP_OBJ *hDriver)
{
    volatile uhphs_registers_t *usbIDEHCI = hDriver->usbIDEHCI;
 
    /* Test if the Host Controller is halted */
    if ((usbIDEHCI->UHPHS_USBSTS & UHPHS_USBSTS_HCHLT_Msk) != 0)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rehci_send_reset error\033[0m");
    }

    /* Port Disabled */
    *((uint32_t *)&(usbIDEHCI->UHPHS_PORTSC) + hDriver->portNumber) &= ~UHPHS_PORTSC_PED_Msk;

    /* Set HcInterruptEnable to have all interrupt enabled except SOF detect.*/
    hDriver->usbIDOHCI->UHP_OHCI_HCINTERRUPTDISABLE = UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_SO   | /*     SchedulingOverrun */
                                                      UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_WDH  | /*     WritebackDoneHead */
                                                      UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_SF   | /*          StartofFrame */
                                                      UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_RD   | /*        ResumeDetected */
                                                      UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_UE   | /*    UnrecoverableError */
                                                      UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_FNO  | /*   FrameNumberOverflow */
                                                      UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_RHSC | /*     otHubStatusChange */
                                                      UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_OC   | /*       OwnershipChange */
                                                      UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_MIE;   /* MasterInterruptEnable */

    /* Port routing control logic default-routes all ports to this host controller. */
    usbIDEHCI->UHPHS_CONFIGFLAG = UHPHS_CONFIGFLAG_CF_Msk;
}


/* Function:
    void _DRV_USB_UHP_HOST_EHCITESTTD(void)

   Summary:
    Test EHCI tranfert descriptor

   Description:
    

   Remarks:
    Refer to .h for usage information.
 */
static void _DRV_USB_UHP_HOST_EHCITESTTD(void)
{
    uint32_t               ConditionCode = 0xFF;
    uint32_t               i;
    uint32_t j;
    uint32_t dummy;
    EHCIQueueTDDescriptor *qTD = (EHCIQueueTDDescriptor *)&EHCI_QueueTD[0];

    for (i = 0; i < DRV_USB_UHP_PIPES_NUMBER; i++)    // Host Pipe Number: queue head
    {
        for (j = 0; j < DRV_USB_UHP_MAX_TRANSACTION; j++)   // Transaction number
        {
            qTD = (EHCIQueueTDDescriptor *)&EHCI_QueueTD[i][j];

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
                if ((ConditionCode & (1<<7)) == 0 )
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rNot Active\033[0m");
                }
                if (ConditionCode & (1<<6))
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rHalted\033[0m");
                    // 1. Set the Halted bit to a one, then
                    qTD->qTD_Token.qtdtoken |= (1<<6);
                    // 2. Set the I-bit to a zero, then
                    EHCI_QueueHead[i].Endpoint_Characteristics &= ~(1<<7);

                    EHCI_QueueHead[i].Transfer_Overlay[1] = 0;
                    EHCI_QueueHead[i].Transfer_Overlay[2] = 0;
                    EHCI_QueueHead[i].Transfer_Overlay[3] = 0;

                    // 3. Set the Active bit to a one and the Halted bit to a zero in the same write.
                    dummy = qTD->qTD_Token.qtdtoken;
                    dummy &= ~(1<<6); // Halted bit 
                    dummy |= (1<<7);  // Active bit
                    qTD->qTD_Token.qtdtoken = dummy;
                }
                else if (ConditionCode & (1<<5))
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rData Buffer Error\033[0m");
                }
                else if (ConditionCode & (1<<4))
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rBabble Detected\033[0m");
                }
                else if (ConditionCode & (1<<3))
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rTransaction Error (XactErr)\033[0m");
                }
                else if (ConditionCode & (1<<2))
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rMissed Micro-Frame\033[0m");
                }
                else if (ConditionCode & (1<<1))
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rSplit Transaction State (SplitXstate)\033[0m");
                }
                /* else if( ConditionCode & (1<<0) )
                 * {
                 *     SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rPing State (P)/ERR");
                 * } */
                else
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\r_DRV_USB_UHP_HOST_EHCITESTTD ERR: ConditionCode = 0x%04X\033[0m", (unsigned int)ConditionCode);
                }
            }
        }
    }
}

/* Function:
    static void ehci_create_queue_head(EHCIQueueHeadDescriptor *qh_base_addr,
                                       EHCIQueueHeadDescriptor *qh_link_pointer,
                                       uint32_t              terminate,
                                       uint32_t              EP_number,
                                       uint32_t              DeviceAddr,
                                       EHCIQueueTDDescriptor *next_qTD_pointer,
                                       uint32_t              mul,
                                       uint32_t              smask,
                                       uint32_t              dtc,
                                       uint32_t              typ,
                                       uint32_t              hubAddress,
                                       uint32_t              hubPortAddress)
   Summary:
    Create Queue Head

   Description:
    Fill Queue Head

   Remarks:
    Refer to .h for usage information.
 */
static void ehci_create_queue_head(EHCIQueueHeadDescriptor *qh_base_addr,
                                   EHCIQueueHeadDescriptor *qh_link_pointer,
                                   uint32_t              terminate,
                                   uint32_t              EP_number,
                                   uint32_t              DeviceAddr,
                                   EHCIQueueTDDescriptor *next_qTD_pointer,
                                   uint32_t              mult,
                                   uint32_t              smask,
                                   uint32_t              dtc,
                                   uint32_t              typ,
                                   uint32_t              hubAddress,
                                   uint32_t              hubPortAddress)
{
    EHCIQueueHeadDescriptor *QueueHead = (EHCIQueueHeadDescriptor *)qh_base_addr;

    /* Queue Head Horizontal Link Pointer
     * see: 3.6 Queue Head
     * Table 3-18. Queue Head DWord 0 */
    QueueHead->Horizontal_Link_Pointer =
        (uint32_t)qh_link_pointer |        /* QHLP: Queue Head Horizontal Link Pointer: bit[31:5] */
        (typ << 1) |                       /* Typ: QH/(s)iTD/FSTN: Data structure type = 01b QH (queue head) */
        (terminate << 0);                  /* T: Terminate = 1 : last transfer */

    /* Table 3-19. Endpoint Characteristics: Queue Head DWord 1 */
    QueueHead->Endpoint_Characteristics =
        (0 << 28) |                      /* RL: Nak Count Reload */
        (0 << 27) |                      /* C: Control Endpoint Flag */
        (512 << 16) |                    /* Maximum Packet Length   1024 or 512 */
        (1 << 15) |                      /* H: Head of Reclamation List Flag */  // This bit is set to mark the QH as the head of the asynchronous schedule.
         //   This bit is cleared since this bit is not used for the periodic schedule.
        (dtc << 14) |                    /* DTC: Data Toggle Control: 1: from qTD, 0: from QH (automatic) */
        (2 << 12) |                      /* EPS: Endpoint Speed = 10b High-Speed (480 Mb/s) */
        (EP_number <<  8) |              /* Endpt: Endpoint Number */
        (0 << 7) |                       /* I: Inactivate on Next Transaction (Periodic Schedule Full or Low-speed)*/ 
        (DeviceAddr <<  0);              /* Device Address */

    /* Table 3-20. Endpoint Capabilities: Queue Head DWord 2 */
    QueueHead->Endpoint_Capabilities =
        (mult << 30) |                     /* Mult: High-Bandwidth Pipe Multiplier = 3 transactions to
                                            * be issued for this endpoint per micro-frame */
        (hubPortAddress << 23) |           /* Port Number */
        (hubAddress << 16) |               /* Hub Addr (full or low-speed device) */
        (0 <<  8) |                        /* µFrame C-Mask: Split Completion Mask (low- or full-speed device) */
        (smask <<  0);                     /* µFrame S-mask: Interrupt Schedule Mask */

    /* 3.6.3 Transfer Overlay */
    QueueHead->Transfer_Overlay[0] = (uint32_t)next_qTD_pointer; /* Next qTD Pointer: DWord 4 */
    // nothing   QueueHead->Transfer_Overlay[1] = 0x01; 
}

/* Function:
    void ehci_received_size( uint32_t * BuffSize )

   Summary:
    Change the received size if needed

   Description:
    Change the received size if the requesting data number have not been sent

   Remarks:
    Refer to .h for usage information.
 */
void ehci_received_size( uint32_t * BuffSize )
{
    /* If Total Bytes to Transfer == 0, all data has been receive */
    if( EHCI_QueueTD[0][1].qTD_Token.TotalBytesTF != 0 )
    {
        *BuffSize -= EHCI_QueueTD[0][1].qTD_Token.TotalBytesTF;
    }
}

/* Function:
    static void ehci_create_qTD(EHCIQueueTDDescriptor *qTD_base_addr,
                                EHCIQueueTDDescriptor *next_qTD_base_addr,
                                uint32_t terminate, 
                                uint32_t PID,
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
                            uint32_t terminate, 
                            uint32_t PID,
                            uint32_t data_toggle,
                            uint32_t nb_bytes,
                            uint32_t int_on_complete,
                            uint32_t *buffer_base_addr)
{
    EHCIQueueTDDescriptor *qTD = (EHCIQueueTDDescriptor *)qTD_base_addr;

    /* 3.5.1 Next qTD Pointer */
    /* Table 3-14. qTD Next Element Transfer Pointer (DWord 0) */
    qTD->Next_qTD_Pointer = (uint32_t)next_qTD_base_addr |  /* Next qTD Pointer */
                            terminate;                      /* Terminate */

    /* 3.5.2 Alternate Next qTD Pointer */
    /* Table 3-15. qTD Alternate Next Element Transfer Pointer (DWord 1) */
    qTD->Alternate_Next_qTD_Pointer = 0x1; /* No Alternate, Terminate */

    /* 3.5.3 qTD Token */
    /* Table 3-16. qTD Token (DWord 2) */
    qTD->qTD_Token.qtdtoken = (data_toggle << 31) |     /* Data Toggle */
                              (nb_bytes << 16) |        /* Total Bytes to Transfer */
                              (int_on_complete << 15) | /* Interrupt On Complete (IOC) */
                              (0 << 12) |               /* Current Page (C_Page) */
                              (3 << 10) |               /* Error Counter (CERR)   Allow 3 retry */
                              (PID <<  8) |             /* PID Code (0: OUT, 1: IN 2: SETUP) */
                              (1 << 7);                 /* Status Field Description: Active. */

    /* 3.5.4 qTD Buffer Page Pointer List */
    /* Table 3-17. qTD Buffer Pointer(s) (DWords 3-7) */
    qTD->qTD_Buffer_Page_Pointer_List = (uint32_t)buffer_base_addr;    /* Buffer Pointer List : 4k page aligned (Bit 31:12) */
}

/* Function:
    void _DRV_USB_UHP_HOST_EhciInit(DRV_USB_UHP_OBJ *drvObj)

   Summary:
    EHCI init

   Description:
    

   Remarks:
    Refer to .h for usage information.
 */
void _DRV_USB_UHP_HOST_EhciInit(DRV_USB_UHP_OBJ *drvObj)
{
    volatile uhphs_registers_t *usbIDEHCI = drvObj->usbIDEHCI;
    uint32_t loop1 = 0;
    uint32_t i;

    /* Initialize FrameList with a dummy queueHead */
    /* Create Queue Head for the command: */
    for( i=0; i<(sizeof(PeriodicFrameList)/4); i++ )
    {
        PeriodicFrameList[i] = 0x01;  /* T-Bit = 1: host controller will never use the value of the frame list pointer */
    }
    
    memset(EHCI_QueueHead, 0, sizeof(EHCI_QueueHead));
    memset(EHCI_QueueTD, 0, sizeof(EHCI_QueueTD));

    /* Host Controller Reset (HCRESET) */
    /* When software writes a one to this bit, the Host Controller resets its internal pipelines,
     * timers, counters, state machines, etc. to their initial value. Any transaction currently in
     * progress on USB is immediately terminated. A USB reset is not driven on downstream ports. */
    usbIDEHCI->UHPHS_USBCMD |= UHPHS_USBCMD_HCRESET_Msk;

    /* Initialize portsc registers */
    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rUHPHS Ports ");
    /* PPC: Port Power Control */
    if (((usbIDEHCI->UHPHS_HCCPARAMS & UHPHS_HCSPARAMS_PPC_Msk) == UHPHS_HCSPARAMS_PPC_Msk))
    {
        /* Put Port Power on (bit 12) : Host controller has port power control switches
         * put Wake-up on (bit 22, 21, 20) */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "have ");
        for (loop1 = 0; loop1 < (usbIDEHCI->UHPHS_HCSPARAMS&0xF); loop1++)
        {
            *((uint32_t *)&(usbIDEHCI->UHPHS_PORTSC) + loop1)
                = UHPHS_PORTSC_PP_Msk |          /* Host controller has port power control switches. */
                  UHPHS_PORTSC_WKOC_E_Msk |      /* enables the port to be sensitive to over-current conditions as wake-up events. */
                  UHPHS_PORTSC_WKDSCNNT_E_Msk |  /* enables the port to be sensitive to device disconnects as wake-up events */
                  UHPHS_PORTSC_WKCNNT_E_Msk;     /* enables the port to be sensitive to device connects as wake-up events */
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
    usbIDEHCI->UHPHS_INSNREG08 = UHPHS_INSNREG08_HSIC_EN_Msk;

    if ((usbIDEHCI->UHPHS_INSNREG08 & UHPHS_INSNREG08_HSIC_EN_Msk) == UHPHS_INSNREG08_HSIC_EN_Msk)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rHSIC is enabled");
    }
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rHSIC is disabled");
    }
#endif

    /* Enable interrupts */
    usbIDEHCI->UHPHS_USBINTR = UHPHS_USBINTR_USBIE_Msk  /* (UHPHS_USBINTR) USB Interrupt Enable */
                             | UHPHS_USBINTR_USBEIE_Msk /* (UHPHS_USBINTR) USB Error Interrupt Enable */
                             | UHPHS_USBINTR_PCIE_Msk   /* (UHPHS_USBINTR) Port Change Interrupt Enable */
                          /* | UHPHS_USBINTR_FLRE_Msk      (UHPHS_USBINTR) Frame List Rollover Enable */
                             | UHPHS_USBINTR_HSEE_Msk   /* (UHPHS_USBINTR) Host System Error Enable */
                             | UHPHS_USBINTR_IAAE_Msk;  /* (UHPHS_USBINTR) Interrupt on Async Advance Enable */

    /* Write the USBCMD register to set the desired interrupt threshold, frame list size (if applicable) */
    usbIDEHCI->UHPHS_USBCMD &= ~UHPHS_USBCMD_ITC_Msk;
    usbIDEHCI->UHPHS_USBCMD |= UHPHS_USBCMD_ITC(1);

    /* Frame List Size: 00b = 1024 elements (4096 bytes) */
    usbIDEHCI->UHPHS_USBCMD |= (((0x00) & UHPHS_USBCMD_FLS_Msk)<<UHPHS_USBCMD_FLS_Pos);

    /* and turn the host controller ON via setting the Run/Stop bit. */
    usbIDEHCI->UHPHS_USBCMD |= UHPHS_USBCMD_RS_Msk; /* Stop = 1 => RUN */

    /* Port routing control logic default-routes all ports to this host controller. */
    usbIDEHCI->UHPHS_CONFIGFLAG = UHPHS_CONFIGFLAG_CF_Msk;

    /*  At this point, the host controller is up and running and the port registers will begin reporting device
     *  connects, etc. System software can enumerate a port through the reset process (where the port is in the
     *  enabled state). At this point, the port is active with SOFs occurring down the enabled port enabled Highspeed
     *  ports, but the schedules have not yet been enabled. The EHCI Host controller will not transmit SOFs
     *  to enabled Full- or Low-speed ports. */

    /* Disable all interrupts. Interrupts will be enabled when the root hub is
     * enabled */
    usbIDEHCI->UHPHS_USBINTR &= ~UHPHS_USBINTR_USBIE_Msk;
}


// *****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UHP_HOST_IRPSubmitEhci
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
USB_ERROR DRV_USB_UHP_HOST_IRPSubmitEhci
(
    DRV_USB_UHP_HOST_PIPE_HANDLE hPipe,
    USB_HOST_IRP *inputIRP
)
{
    USB_HOST_IRP_LOCAL * irpIterator;
    DRV_USB_UHP_HOST_TRANSFER_GROUP *controlTransferGroup;
    USB_HOST_IRP_LOCAL *irp  = (USB_HOST_IRP_LOCAL *)inputIRP;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe = (DRV_USB_UHP_HOST_PIPE_OBJ *)(hPipe);
    DRV_USB_UHP_OBJ *hDriver;
    uint32_t tosend;
    uint32_t nbBytes;
    uint8_t *point;
    uint8_t idx;
    uint8_t idx_plus;
    uint8_t terminate;
    uint8_t InitialDataToggle;
    uint8_t stop = 0;
    uint8_t endpoint_int = 0;
    uint8_t DToggle = 0;
    uint8_t OnComplete;
    uint8_t IntOnComplete;
    USB_ERROR returnValue = USB_ERROR_PARAMETER_INVALID;
    uint32_t i;
    volatile uhphs_registers_t *usbIDEHCI;

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
                
                if (irp->setup != NULL)
                {
                    /* SETUP Transaction */
                    point = (uint8_t *)irp->setup;
                    for (i = 0; i < 8; i++)
                    {
                        setupPacket[i] = point[i];
                    }

                    idx = 0;
                    idx_plus = idx + 1;

                    /* Fill EHCI_QueueTD[0] With SETUP data */
                    /* SETUP packet PID */
                    ehci_create_qTD(&EHCI_QueueTD[pipe->hostEndpoint][idx],       /* qTD address base */
                                    &EHCI_QueueTD[pipe->hostEndpoint][idx_plus],  /* next qTD address base */
                                    0,                        /* Terminate */
                                    2,                        /* PID: SETUP = 2 */
                                    DToggle,                  /* data toggle */
                                    8,                        /* Total Bytes to transfer */
                                    0,                        /* Interrupt on Complete */
                                    (uint32_t *)setupPacket); /* data buffer address base, 32-Byte align */

                    if (*((uint8_t *)(irp->setup)) & 0x80)
                    {
                        /* SETUP IN: (1<<7) Device to host */

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
                            /* Setup DATA IN packet */
                            ehci_create_qTD(&EHCI_QueueTD[pipe->hostEndpoint][idx],      /* qTD address base */
                                            &EHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* next qTD address base */
                                            0,                       /* Terminate */
                                            1,                       /* PID: IN = 1 */
                                            (++DToggle)&0x01,        /* data toggle */
                                            tosend,                  /* Total Bytes to transfer */
                                            0,                       /* Interrupt on Complete */
                                            (uint32_t *)(USBBufferAligned + irp->completedBytes)); /* data buffer address base, 32-Byte align */

                            if (nbBytes > pipe->endpointSize)
                            {
                                nbBytes -= pipe->endpointSize;
                                irp->completedBytes += pipe->endpointSize;
                            }
                            else
                            {
                                stop = 1;
                                irp->completedBytes += irp->size;
                            }
                        } while (stop == 0);

                        /* The host issues an OUT zero length packet (ZLP) to acknowledge reception of the descriptor. */
                        idx++;
                        idx_plus = idx + 1;
                        /* Setup STATUS OUT packet */
                        ehci_create_qTD(&EHCI_QueueTD[pipe->hostEndpoint][idx], /* qTD address base */
                                        NULL,               /* next qTD address base */
                                        1,                  /* Terminate */
                                        0,                  /* PID: OUT = 0 */
                                        1,                  /* data toggle, STATUS is always DATA1 */
                                        0,                  /* Total Bytes to transfer: ZLP */
                                        1,                  /* Interrupt on Complete */
                                        NULL);              /* data buffer address base, 32-Byte align */
                    }
                    else
                    {
                        /* SETUP OUT: (1<<7) Host to Device */
                        if (irp->size != 0)
                        {
                            /* OUT Packet */
                            idx++;
                            idx_plus = idx + 1;

                            point = (uint8_t *)irp->data;

                            for (i = 0; i < irp->size; i++)
                            {
                                USBBufferAligned[i] = point[i];
                            }

                            /* Setup DATA OUT Packet */
                            ehci_create_qTD(&EHCI_QueueTD[pipe->hostEndpoint][idx],      /* qTD address base */
                                            &EHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* next qTD address base */
                                            0,                       /* Terminate */
                                            0,                       /* PID: OUT = 0 */
                                            1,                       /* data toggle */
                                            irp->size,               /* Total Bytes to transfer */
                                            0,                       /* Interrupt on Complete */
                                            (uint32_t *)USBBufferAligned);
                            DCACHE_CLEAN_BY_ADDR((uint32_t *)irp->data, irp->size);               /* CLEAN should be called before writing */
                        }
                        idx++;
                        idx_plus = idx + 1;
                        /* Setup STATUS IN Packet (ZLP) */
                        ehci_create_qTD(&EHCI_QueueTD[pipe->hostEndpoint][idx], /* qTD address base */
                                        NULL,               /* next qTD address base */
                                        1,                  /* Terminate */
                                        1,                  /* PID: IN = 1 */
                                        1,                  /* data toggle */
                                        0,                  /* Total Bytes to transfer: ZLP */
                                        1,                  /* Interrupt on Complete */
                                        NULL);
                    }

                    /* Create Queue Head for the command: */
                    ehci_create_queue_head(&EHCI_QueueHead[pipe->hostEndpoint], /* Queue Head base address */
                                           &EHCI_QueueHead[pipe->hostEndpoint], /* Queue Head Link Pointer */
                                           0,                   /* Terminate */
                              pipe->endpointAndDirection & 0xF, /* EndPt: Endpoint number */
                                           pipe->deviceAddress, /* Device Address */
                                           &EHCI_QueueTD[pipe->hostEndpoint][0],    /* Next qTD Pointer */
                                           1,                   /* Mult: High-Bandwidth Pipe Multiplier */
                                           0,                   /* µFrame S-mask: Interrupt Schedule Mask, A non-zero value in this field indicates an interrupt endpoint. */
                                           1,                   /* DTC: Initial data toggle comes from incoming qTD DT bit */
                                           1,                   /* Typ: 01b QH (queue head) */
                                           pipe->hubAddress,    /* Hub Addr */
                                           pipe->hubPort);      /* Port Number */

                } /* End SETUP Transaction */
                else
                {
                    /* For non control transfers, if this is the first
                     * irp in the queue, then we can start the irp */
                    idx = 0;
                    idx_plus = idx + 1;

                    if( (pipe->endpointAndDirection & 0x80) == 0 )
                    {
                        /* Host to Device: OUT */

                        /* Data is moving from device to host
                         * We need to set the Rx Packet Request bit */
                        nbBytes = irp->size;
                        stop = 0;
                        do
                        {
                            if (nbBytes > sizeof(USBBufferAligned))
                            {
                                tosend = sizeof(USBBufferAligned);
                                OnComplete = 0;
                            }
                            else
                            {
                                tosend = nbBytes;
                                OnComplete = 1;
                            }
                            /* Host to Device: OUT */
                            ehci_create_qTD(&EHCI_QueueTD[pipe->hostEndpoint][idx],      /* qTD address base */
                                            &EHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* next qTD address base */
                                            OnComplete,                    /* Terminate */
                                            0,                             /* PID: OUT = 0 */
                                            hDriver->staticDToggleOut&0x1, /* data toggle */
                                            tosend,                        /* Total Bytes to transfer */
                                            OnComplete,                    /* Interrupt on Complete */
                                            (uint32_t *)irp->data+irp->completedBytes); /* data buffer address base, 32-Byte align */

                            DCACHE_CLEAN_BY_ADDR((uint32_t *)irp->data, irp->size); /* CLEAN should be called before writing */
                            hDriver->staticDToggleOut++;

                            idx++;
                            idx_plus = idx + 1;
                            if (nbBytes > sizeof(USBBufferAligned))
                            {
                                nbBytes -= sizeof(USBBufferAligned);
                                irp->completedBytes += sizeof(USBBufferAligned);
                            }
                            else
                            {
                                stop = 1;
                                irp->completedBytes += irp->size;
                            }
                        } while (stop == 0);

                        if( pipe->pipeType == USB_TRANSFER_TYPE_INTERRUPT )
                        {
                            endpoint_int = 1;
                        }
                        else
                        {
                            endpoint_int = 0;
                        }
                        /* Create Queue Head for the command: */
                        ehci_create_queue_head(&EHCI_QueueHead[pipe->hostEndpoint],     /* Queue Head base address */
                                               &EHCI_QueueHead[pipe->hostEndpoint],     /* Queue Head Link Pointer */
                                               0,                      /* Terminate */
                                       pipe->endpointAndDirection&0xF, /* EndPt: Endpoint number */
                                               pipe->deviceAddress,    /* Device Address */
                                               &EHCI_QueueTD[pipe->hostEndpoint][0], /* Next qTD Pointer */
                                               1,                      /* Mult: High-Bandwidth Pipe Multiplier */
                                               endpoint_int,           /* µFrame S-mask: Interrupt Schedule Mask, A non-zero value in this field indicates an interrupt endpoint. */
                                               1,                      /* DTC: Initial data toggle comes from incoming qTD DT bit */
                                               1,                      /* Typ: 01b QH (queue head) */
                                               pipe->hubAddress,       /* Hub Addr */
                                               pipe->hubPort);         /* Port Number */
                    }
                    else
                    {
                        /* Device to Host: IN */

                        /* Data is moving from host to device. We
                         * need to copy data into the FIFO and
                         * then and set the TX request bit. If the
                         * IRP size is greater than endpoint size then
                         * we must packetize. */
                        nbBytes = irp->size;
                        stop = 0;
                        do
                        {
                            if (nbBytes > pipe->endpointSize)
                            {
                                tosend = pipe->endpointSize;
                                OnComplete = 0;
                                IntOnComplete = 0;
                            }
                            else
                            {
                                tosend = nbBytes;
                                OnComplete = 1;
                                IntOnComplete = 1;
                                if( pipe->pipeType == USB_TRANSFER_TYPE_INTERRUPT )
                                {
                                    OnComplete = 0;
                                    hDriver->staticDToggleIn = 1;
                                } 
                            }
                            /* IN */
                            ehci_create_qTD(&EHCI_QueueTD[pipe->hostEndpoint][idx],      /* qTD address base */
                                            &EHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* next qTD address base */
                                            OnComplete,                   /* Terminate */
                                            1,                            /* PID: IN = 1 */
                                            hDriver->staticDToggleIn&0x1, /* data toggle */
                                            tosend,                       /* Total Bytes to transfer */
                                            IntOnComplete,                /* Interrupt on Complete */
                                            (uint32_t *)irp->data + irp->completedBytes); /* data buffer address base, 32-Byte align */
                            DCACHE_CLEAN_BY_ADDR((uint32_t *)irp->data, irp->size);               /* CLEAN should be called before writing */

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

                        if( pipe->pipeType == USB_TRANSFER_TYPE_INTERRUPT )
                        {
                            //endpoint_int = 1;
                            endpoint_int = 0x1;    // A value of 0b00010001 gives you an interrupt on every fourth microframe (every 500us).
 /* JCB: should be 1 */     terminate = 0;    /* No Interrupt transfer if set to 1. */
                            // This bit is set indicating that the horizontal link pointer value is invalid. Since this QH goes into the periodic
                            // schedule, it is not used in a circular linked list like QHs for the asynchronous schedule.
                            
                            InitialDataToggle = 0; /* Ignore DT bit from incoming qTD. Host controller preserves DT bit in the queue head. */
                        }
                        else
                        {
                            endpoint_int = 0;
                            terminate = 1;  // JCB true ??? // This bit is ignored by the host controller when the queue head is in the Asynchronous schedule.
                            InitialDataToggle = 1; /* DTC: Initial data toggle comes from incoming qTD DT bit */
                        }

                        /* Create Queue Head for the command: */
                        ehci_create_queue_head(&EHCI_QueueHead[pipe->hostEndpoint],     /* Queue Head base address */
                                               &EHCI_QueueHead[pipe->hostEndpoint],     /* Queue Head Link Pointer */
                                               terminate,                      /* Terminate */
                                       pipe->endpointAndDirection&0xF, /* EndPt: Endpoint number */
                                               pipe->deviceAddress,    /* Device Address */
                                             &EHCI_QueueTD[pipe->hostEndpoint][0],       /* Next qTD Pointer */
                                               1,                      /* Mult: High-Bandwidth Pipe Multiplier */
                                               endpoint_int,           /* µFrame S-mask: Interrupt Schedule Mask, A non-zero value in this field indicates an interrupt endpoint. */
                                               InitialDataToggle,      /* DTC: Data Toggle Control: 1: from qTD, 0: from QH (automatic) */
                                               1,                      /* Typ: 01b QH (queue head) */
                                               pipe->hubAddress,       /* Hub Addr */
                                               pipe->hubPort);         /* Port Number */
                    }                                            
                }

                usbIDEHCI = hDriver->usbIDEHCI;

                if( pipe->pipeType == USB_TRANSFER_TYPE_INTERRUPT )
                {
                    hDriver->hostPipeInterrupt = pipe->hostEndpoint;

//                    printf("\n\r SUB hostPipeInterrupt = %d", hDriver->hostPipeInterrupt);
                    
//                    printf("\n\r sizeof(PeriodicFrameList) = %d", sizeof(PeriodicFrameList));
//                    printf("\n\r HCCPARAMS = 0x%X", usbIDEHCI->UHPHS_HCCPARAMS);
//                    for( uint32_t i=0; i<sizeof(PeriodicFrameList)/4; i++ )  //  4096 bytes, 1024 elements
//                    for( uint32_t i=0; i<1024; i++ )
//                    {
//                        /* Frame list size is 1024 */
//                        if( (pipe->endpointAndDirection & 0x80) == 0 )
//                        {
//                            /* Host to Device: OUT */
//                            PeriodicFrameList[i] = (uint32_t)&EHCI_QueueHead[pipe->hostEndpoint];
//                        }
//                        else
//                        {
//                            /* Host to Device: IN */
                            PeriodicFrameList[0] = (uint32_t)EHCI_QueueHead[pipe->hostEndpoint].Horizontal_Link_Pointer;
//                        }
//                    }
                    /* Periodic Frame List Base Address */
                    usbIDEHCI->UHPHS_PERIODICLISTBASE = (uint32_t)&PeriodicFrameList[0];

                    /* Periodic Schedule Enable: Use the PERIODICLISTBASE register to access the Periodic Schedule. */
                    usbIDEHCI->UHPHS_USBCMD |= UHPHS_USBCMD_PSE_Msk;
                }
                else if( pipe->pipeType == USB_TRANSFER_TYPE_ISOCHRONOUS )
                {
//                    /* IsochronousEnable: enable/disable processing of isochronous EDs */
//                    usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_IE_Msk;
//                    
//                    // Change Format in Endpoint Descriptor
//                    // IsochronousListEnable
                }
                else if( pipe->pipeType == USB_TRANSFER_TYPE_CONTROL )
                {
                    /* USB_TRANSFER_TYPE_CONTROL */
                    hDriver->hostPipeInUse = pipe->hostEndpoint;

                    /* Current Asynchronous List Address */
                    usbIDEHCI->UHPHS_ASYNCLISTADDR = (uint32_t)&EHCI_QueueHead[pipe->hostEndpoint];

                    /* USB Command is send here
                     * In order to communicate with devices via the asynchronous schedule, system software must write the
                     * ASYNDLISTADDR register with the address of a control or bulk queue head. Software must then enable
                     * the asynchronous schedule by writing a one to the Asynchronous Schedule Enable bit in the USBCMD register. */
                    /* ASE: Asynchronous Schedule Enable: Use the UHPHS_ASYNCLISTADDR register to access the Asynchronous Schedule. */
                    usbIDEHCI->UHPHS_USBCMD |= UHPHS_USBCMD_ASE_Msk;
                }
                else
                {
                    /* USB_TRANSFER_TYPE_BULK */
                    hDriver->hostPipeInUse = pipe->hostEndpoint;

                    /* Current Asynchronous List Address */
                    usbIDEHCI->UHPHS_ASYNCLISTADDR = (uint32_t)&EHCI_QueueHead[pipe->hostEndpoint];

                    /* USB Command is send here
                     * In order to communicate with devices via the asynchronous schedule, system software must write the
                     * ASYNDLISTADDR register with the address of a control or bulk queue head. Software must then enable
                     * the asynchronous schedule by writing a one to the Asynchronous Schedule Enable bit in the USBCMD register. */
                    /* ASE: Asynchronous Schedule Enable: Use the UHPHS_ASYNCLISTADDR register to access the Asynchronous Schedule. */
                    usbIDEHCI->UHPHS_USBCMD |= UHPHS_USBCMD_ASE_Msk;
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
                /* Enable interrupts */
                usbIDEHCI->UHPHS_USBINTR = UHPHS_USBINTR_USBIE_Msk  /* (UHPHS_USBINTR) USB Interrupt Enable */
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
    void _DRV_USB_UHP_HOST_DisableControlList_EHCI(DRV_USB_UHP_OBJ *hDriver)

  Summary:
    Disable the processing of the Control list
	
  Description:
    Disable the processing of the Control list

  Remarks:
    See drv_xxx.h for usage information.
*/
void _DRV_USB_UHP_HOST_DisableControlList_EHCI(DRV_USB_UHP_OBJ *hDriver)
{
    volatile uhphs_registers_t *usbIDEHCI;

    usbIDEHCI = hDriver->usbIDEHCI;

    /* Asynchronous Schedule Disable */
    usbIDEHCI->UHPHS_USBCMD &= ~UHPHS_USBCMD_ASE_Msk;

}


// *****************************************************************************
/* Function:
    void _DRV_USB_UHP_HOST_Tasks_ISR_EHCI(DRV_USB_UHP_OBJ *hDriver)

  Summary:
    Interrupt handler
	
  Description:
    Management of all EHCI interrupt

  Remarks:
    See drv_xxx.h for usage information.
*/
void _DRV_USB_UHP_HOST_Tasks_ISR_EHCI(DRV_USB_UHP_OBJ *hDriver)
{
    volatile uhphs_registers_t *usbIDEHCI = hDriver->usbIDEHCI;
    uint32_t isr_read_data;
    uint32_t read_data;
    uint32_t i;
    DRV_USB_UHP_HOST_TRANSFER_GROUP *transferGroup;

    transferGroup = &hDriver->controlTransferGroup;

    /* EHCI interrupts */
    isr_read_data = (usbIDEHCI->UHPHS_USBINTR & usbIDEHCI->UHPHS_USBSTS & 0x3F);
    isr_read_data |= (usbIDEHCI->UHPHS_USBSTS & 0xC000);
//    isr_read_data = usbIDEHCI->UHPHS_USBINTR;
//    isr_read_data &= usbIDEHCI->UHPHS_USBSTS;

    if (isr_read_data != 0)
    {
        /* Interrupt on Async Advance */
        if ((isr_read_data & UHPHS_USBSTS_IAA_Msk) == UHPHS_USBSTS_IAA_Msk)
        {
            transferGroup->int_on_async_advance = 1;
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rEHCI interrupt on async advance");
            usbIDEHCI->UHPHS_USBSTS = UHPHS_USBSTS_IAA_Msk;
        }

        /* Host system error */
        if ((isr_read_data & UHPHS_USBSTS_HSE_Msk) == UHPHS_USBSTS_HSE_Msk)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rEHCI Host system error interrupt\033[0m");
            usbIDEHCI->UHPHS_USBSTS = UHPHS_USBSTS_HSE_Msk;
            /* - The Run/Stop bit in the USBCMD register is set to a zero.
             * - The following bits in the USBSTS register are set:
             *    - Host System Error bit is to a one.
             *    - HCHalted bit is set to a one. */
            /* After a Host System Error, Software must reset the host controller via 
             * HCReset in the USBCMD register before re-initializing and restarting the host controller. */
        }

        /* Frame list Rollover */
        if ((isr_read_data & UHPHS_USBSTS_FLR_Msk) == UHPHS_USBSTS_FLR_Msk)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rEHCI Frame list Rollover interrupt\033[0m");
            usbIDEHCI->UHPHS_USBSTS = UHPHS_USBSTS_FLR_Msk;
            /* After a Host System Error, Software must reset the host controller via HCReset 
             * in the USBCMD register before re-initializing and restarting the host controller. */
        }

        /* Port Change Detect */
        if ((isr_read_data & UHPHS_USBSTS_PCD_Msk) == UHPHS_USBSTS_PCD_Msk)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rEHCI port change interrupt");
            for (i = 0; i < NUMBER_OF_PORTS; i++)
            {
                /* read_data = usbIDEHCI->UHPHS_PORTSC; */
                read_data = *((uint32_t *)&(usbIDEHCI->UHPHS_PORTSC) + i);
                /* Over-current Change */
                if ((read_data & UHPHS_PORTSC_OCC_Msk) == UHPHS_PORTSC_OCC_Msk)
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOver-current Change on port %d", (int)i);
                }
                /* Port Enable/Disable Change */
                if ((read_data & UHPHS_PORTSC_PEDC_Msk) == UHPHS_PORTSC_PEDC_Msk)
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rPort Enable/Disable Change on port %d", (int)i);
                }
                /* Connect Status Change */
                if ((read_data & UHPHS_PORTSC_CSC_Msk) == UHPHS_PORTSC_CSC_Msk)
                {
                    /* 1=Device is present on port. */
                    usbIDEHCI->UHPHS_USBCMD &= ~UHPHS_USBCMD_ASE_Msk;
                    if (((*((uint32_t *)&(usbIDEHCI->UHPHS_PORTSC) + i)) & UHPHS_PORTSC_CCS_Msk) == UHPHS_PORTSC_CCS_Msk)
                    {
                        /* New connection */
                        hDriver->deviceAttached = true;
                        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rConnect port %d", (int)i);
                        hDriver->portNumber = i;
                        break;
                    }
                    else
                    {
                        /* Disconnect */
                        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rDisconnect occurred on port %d", (int)i);

                        hDriver->portNumber = 0;
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
            usbIDEHCI->UHPHS_USBSTS = UHPHS_USBSTS_PCD_Msk; /* clear by writing "1" = Port Change Detect */
        }

        /* USB Interrupt (USBINT) R/WC */
        /* The Host Controller sets this bit to 1 on the completion of a USB transaction, which results in the retirement of a Transfer Descriptor */
        /* that had its IOC bit set. */
        /* The Host Controller also sets this bit to 1 when a short packet is detected (actual */
        /* number of bytes received was less than the expected number of bytes). */
        if ((isr_read_data & UHPHS_USBSTS_USBINT_Msk) == UHPHS_USBSTS_USBINT_Msk)
        {
            usbIDEHCI->UHPHS_USBSTS = UHPHS_USBSTS_USBINT_Msk; /* clear by writing "1" */
            hDriver->intXfrQtdComplete = 1;
        }
        /* USB error */
        if ((isr_read_data & UHPHS_USBSTS_USBERRINT_Msk) == UHPHS_USBSTS_USBERRINT_Msk)
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rEHCI IRQ : USB error interrupt\033[0m");
            /* Clear It */
            usbIDEHCI->UHPHS_USBSTS = UHPHS_USBSTS_USBERRINT_Msk;
            _DRV_USB_UHP_HOST_EHCITESTTD();
            usbIDEHCI->UHPHS_USBCMD &= ~UHPHS_USBCMD_ASE_Msk;
            hDriver->intXfrQtdComplete = 0xFF;
        }
    }
}/* end of _DRV_USB_UHP_HOST_Tasks_ISR_EHCI() */


/* ***************************************************************************** */
/* ***************************************************************************** */
/* Root Hub Driver Routines */
/* ***************************************************************************** */
/* ***************************************************************************** */

/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_HOST_ROOT_HUB_OperationEnableEhci(DRV_HANDLE handle, bool enable)

   Summary:
    Root hub enable

   Description:
    

   Remarks:
    Refer to .h for usage information.
 */
void DRV_USB_UHP_HOST_ROOT_HUB_OperationEnableEhci(DRV_HANDLE handle, bool enable)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    volatile uhphs_registers_t *usbIDEHCI = pUSBDrvObj->usbIDEHCI;

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
            /* Disable interrupt generation */
            usbIDEHCI->UHPHS_USBINTR &= ~UHPHS_USBINTR_USBIE_Msk;
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
            usbIDEHCI->UHPHS_USBINTR = UHPHS_USBINTR_USBIE_Msk    /* (UHPHS_USBINTR) USB Interrupt Enable */
                                   | UHPHS_USBINTR_USBEIE_Msk /* (UHPHS_USBINTR) USB Error Interrupt Enable */
                                   | UHPHS_USBINTR_PCIE_Msk   /* (UHPHS_USBINTR) Port Change Interrupt Enable */
                                /* | UHPHS_USBINTR_FLRE_Msk      (UHPHS_USBINTR) Frame List Rollover Enable */
                                   | UHPHS_USBINTR_HSEE_Msk   /* (UHPHS_USBINTR) Host System Error Enable */
                                   | UHPHS_USBINTR_IAAE_Msk;  /* (UHPHS_USBINTR) Interrupt on Async Advance Enable */

            /* Enable VBUS */
            if(pUSBDrvObj->rootHubInfo.portPowerEnable != NULL)
            {
                /* This USB module has only one port. So we call this function
                 * once to enable the port power on port 0 */
                pUSBDrvObj->rootHubInfo.portPowerEnable(0 /* Port 0 */, true);
            }
        }
    }
} /* end of DRV_USB_UHP_ROOT_HUB_OperationEnable() */

