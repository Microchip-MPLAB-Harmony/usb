/*******************************************************************************
   USB OHCI Host Controller Driver

   Company:
    Microchip Technology Inc.

   File Name:
    drv_uhp_ohci_host.c

   Summary:
    USB OHCI Host Driver Implementation.

   Description:
    This file implements the USB Host port OHCI driver implementation.
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
#include "driver/usb/uhp/src/drv_usb_uhp_ohci_host.h"

/**********************************************************
 * This structure is a set of pointer to the USB OHCI driver
 * functions. It is provided to the host and device layer
 * as the interface to the driver.
 * *******************************************************/

DRV_USB_HOST_INTERFACE gDrvUSBUHPHostInterfaceOhci =
{
    .open              = DRV_USB_UHP_Open,
    .close             = DRV_USB_UHP_Close,
    .eventHandlerSet   = DRV_USB_UHP_ClientEventCallBackSet,
    .hostIRPSubmit     = DRV_USB_UHP_HOST_IRPSubmitOhci,
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
    .rootHubInterface.rootHubOperationEnable    = DRV_USB_UHP_HOST_ROOT_HUB_OperationEnableOhci,
    .rootHubInterface.rootHubOperationIsEnabled = DRV_USB_UHP_HOST_ROOT_HUB_OperationIsEnabled
};

typedef union __attribute__ ((packed))
{
    struct
    {
        uint32_t FA    :  7; /* FunctionAddress */
        uint32_t EN    :  4; /* EndpointNumber */
        uint32_t D     :  2; /* Direction */
        uint32_t S     :  1; /* Speed */
        uint32_t K     :  1; /* sKip */
        uint32_t F     :  1; /* Format */
        uint32_t MPS   : 11; /* MaximumPacketSize */
        uint32_t dummy :  5;
    };
    uint32_t ed_control;
}
ED_Control;

/* Endpoint Descriptor Field Definitions */
/* 4.2.1 Endpoint Descriptor Format */
typedef struct 
{
    /* FunctionAddress | EndpointNumber | Direction | Speed | sKip | Format
     * MaximumPacketSize */
    volatile ED_Control Control;
    /* TailP: TDQueueTailPointer
     * If TailP and HeadP are the same, then the list contains no TD that the HC
     * can process. If TailP and HeadP are different, then the list contains a TD to be
     * processed. */
    volatile uint32_t TailP;
    /* HeadP: TDQueueHeadPointer Points to the next TD to be processed for this
     * endpoint. */
    volatile uint32_t HeadP;
    /* NextED: If nonzero, then this entry points to the next ED on the list */
    volatile uint32_t NextEd;

    /* 240 */
    volatile uint8_t dummy[0xF0];
    
}  OHCIQueueHeadDescriptor;


/* 4.3.1.2 General Transfer Descriptor Field Definitions */
typedef union __attribute__ ((packed))
{
    struct
    {
        uint32_t dummy : 18; 
        uint32_t R     :  1; /* bufferRounding */
        uint32_t DP    :  2; /* Direction/PID */
        uint32_t DI    :  3; /* DelayInterrupt */
        uint32_t T     :  2; /* DataToggle */
        uint32_t EC    :  2; /* ErrorCount */
        uint32_t CC    :  4; /* ConditionCode */
    };
    uint32_t td_control;
}
TD_Control;

/* 4.3.1.1 General Transfer Descriptor Format */
/* This General TD is a 16-byte, host memory structure that must be aligned to a 16-byte boundary. */
typedef struct 
{
    volatile TD_Control Control;
    volatile uint32_t CBP;
    volatile uint32_t NextTD;
    volatile uint32_t BE;
} OHCIQueueTDDescriptor;

/* OHCI: HcFmInterval Register:
 * FrameInterval specifies the interval between 2 consecutive SOFs in bit times
 *  Bit rate USB Full Speed: 12 MHz
 *  1/12 = 0,08333333
 *  1(ms) / 0,0833333 = 12
 *  FrameInterval = 12x1000 = 12000 */
#define FRAMEINTERVAL    12000

/* 5.4 FrameInterval Counter */
/*  7.3.1 HcFmInterval Register: FSLargestDataPacket
 *  This field specifies a value which is loaded into the Largest
 *  Data Packet Counter at the beginning of each frame. */
/*  FSLargestDataPacket initializes a counter within the Host Controller that is
 *  used to determine if a transaction on USB can be completed before EOF
 *  processing must start.
 *  It is a function of the new FrameInterval and is calculated by subtracting
 *  from FrameInterval the maximum number of bit times for transaction overhead
 *  on USB and the number of bit times needed for EOF processing, then 
 *  multiplying the result by 6/7 to account for the worst case bit stuffing  */
/*  The value of MAXIMUM_OVERHEAD below is 210 bit times. */
#define MAXIMUM_OVERHEAD 210
#define FSLARGESTDATAPACKET (((FRAMEINTERVAL-MAXIMUM_OVERHEAD) * 6) / 7)
#define OHCI_FMINTERVAL ((FSLARGESTDATAPACKET << UHP_OHCI_HCFMINTERVAL_FSMPS_Pos) | (FRAMEINTERVAL-1))

/* 7.3.4 HcPeriodicStart Register
 * The HcPeriodicStart register has a 14-bit programmable value which 
 * determines when is the earliest time HC should start processing the 
 * periodic list. */
/*  Set HcPeriodicStart to a value that is 90% of the value in FrameInterval 
 *  field of the HcFmInterval register. */
#define OHCI_PRDSTRT    (FRAMEINTERVAL*90/100)

#define DIT   2    /* DelayInterrupt (7 == no interrupt) */

/* 4.4 Host Controller Communications Area */
typedef struct 
{
    volatile uint32_t UHP_HccaInterruptTable[32]; /* 0x00: These 32 Dwords are pointers to interrupt EDs. (128 chars / 4 = 32 uint32_t) */
    volatile uint16_t UHP_HccaFrameNumber;        /* 0x80: Contains the current frame number. */
    volatile uint16_t UHP_HccaPad1;               /* 0x82: When the HC updates HccaFrameNumber, it sets this word to 0. */
    volatile uint32_t UHP_HccaDoneHead;           /* 0x84: When a TD is complete (with or without an error) it is unlinked from the queue that it is on and linked to the Done Queue */
    volatile uint8_t  reserved[116];              /* 0x88: Reserved for use by HC */
} OHCI_HCCA;

/* Global variables */
#define NOT_CACHED __attribute__((__section__(".region_nocache")))
#define DRV_USB_UHP_MAX_TRANSACTION   20
__ALIGNED(32) NOT_CACHED OHCIQueueHeadDescriptor OHCI_QueueHead[DRV_USB_UHP_PIPES_NUMBER];    /* Queue EndpointDescriptor: 0x30=48 length */
__ALIGNED(32) NOT_CACHED OHCIQueueTDDescriptor   OHCI_QueueTD[DRV_USB_UHP_PIPES_NUMBER][DRV_USB_UHP_MAX_TRANSACTION];   /* Queue Element Transfer Descriptor: 1 qTD is 0x20=32. Size 9 to reach 512 byte (8x64) + 1 NULL Packet transfer */
__ALIGNED(256) NOT_CACHED OHCI_HCCA HCCA;
__ALIGNED(4096) NOT_CACHED uint8_t USBBufferAligned[USB_HOST_TRANSFERS_NUMBER*64]; /* 4K page aligned, see Table 3-17. qTD Buffer Pointer(s) (DWords 3-7) */
__ALIGNED(4096) NOT_CACHED volatile uint8_t setupPacket[8];


// ****************************************************************************
// ****************************************************************************
// Local Functions
// ****************************************************************************
// ****************************************************************************



/* Function:
    uitn32_t _DRV_USB_UHP_HOST_OHCITESTTD(void)

   Summary:
    Test OHCI transfer descriptor

   Description:
    

   Remarks:
    Refer to .h for usage information.
 */
static uint8_t _DRV_USB_UHP_HOST_OHCITESTTD(void)
{
    uint32_t ConditionCode = 0xFF;
    uint32_t i=0;
    uint32_t j=0;
    OHCIQueueTDDescriptor *qTD;

    for (i = 0; i < DRV_USB_UHP_PIPES_NUMBER; i++)
    {
        for (j = 0; j < DRV_USB_UHP_MAX_TRANSACTION; j++)
        {
            /* ConditionCode : This field contains the status of the last attempted transaction.
             * 4.3.3 Completion Codes
             * Table 4-7: Completion Codes */
            qTD = (OHCIQueueTDDescriptor *)&OHCI_QueueTD[i][j];

            /* CC: Condition Code */
            ConditionCode = (qTD->Control.td_control&((uint32_t)(0xF << 28)))>>28;
            if( ConditionCode == 0 )
            {
              /* SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rNoERROR"); */
              break;
            }
            else if( ConditionCode == 1 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rCRC\033[0m");
            }
            else if( ConditionCode == 2 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rBITSTUFFING\033[0m");
            }
            else if( ConditionCode == 3 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rDATATOGGLEMISMATCH\033[0m");
            }
            else if( ConditionCode == 4 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rSTALL\033[0m");
            }
            else if( ConditionCode == 5 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rDEVICENOTRESPONDING\033[0m");
            }
            else if( ConditionCode == 6 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rPIDCHECKFAILURE\033[0m");
            }
            else if( ConditionCode == 7 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rUNEXPECTEDPID\033[0m");
            }
            else if( ConditionCode == 8 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rDATAOVERRUN\033[0m");
            }
            else if( ConditionCode == 9 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rDATAUNDERRUN\033[0m");
            }
            else if( ConditionCode == 10 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rreserved10\033[0m");
            }
            else if( ConditionCode == 11 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rreserved11\033[0m");
            }
            else if( ConditionCode == 12 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rBUFFEROVERRUN\033[0m");
            }
            else if( ConditionCode == 13 )
            {
              SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\033[31m\n\rBUFFERUNDERRUN\033[0m");
            }
            else
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rNOT ACCESSED = %d \033[0m", (int)ConditionCode);
            }
            break;
        }
        if(ConditionCode != 0)
        {
            break;
        }
    }
    return(ConditionCode);
}



/* Function:
    static void ohci_create_queue_head( )

   Summary:
     4.2 Endpoint Descriptor
     An Endpoint Descriptor (ED) is a 16-byte, memory resident structure that must be aligned to a 16-byte boundary.

   Description:
    Fill Queue Head

   Remarks:
    Refer to .h for usage information.
 */
static void ohci_create_queue_head(OHCIQueueHeadDescriptor * EDAddr,
                                   OHCIQueueHeadDescriptor * NextED,
                                   uint32_t Direction,
                                   uint32_t EndPt,
                                   uint32_t MaxPacket,
                                   uint32_t TDFormat,
                                   uint32_t Skip,
                                   uint32_t Speed,
                                   uint32_t FuncAddress,
                                   OHCIQueueTDDescriptor * TDQHeadPntr,
                                   OHCIQueueTDDescriptor * TDQTailPntr )
{
    OHCIQueueHeadDescriptor *pED = (OHCIQueueHeadDescriptor*) EDAddr;

    /* Dword 0 */
    pED->Control.ed_control = (MaxPacket << 16) | /* MPS: MaximumPacketSize */
                              ( TDFormat << 15) | /* F: Format */
                                   (Skip << 14) | /* K: sKip */
                                  (Speed << 13) | /* S: Speed 0 FS, 1 LS */
                              (Direction << 11) | /* D: Direction */
                                  (EndPt <<  7) | /* EN: EndpointNumber */
                            (FuncAddress <<  0);  /* FA: FunctionAddress */

    /* Dword 1, TailP: TDQueueTailPointer */
    pED->TailP = ((uint32_t)TDQTailPntr) & 0xFFFFFFF0;

    /* Dword 2, TD Queue Head Pointer (HeadP) */
    pED->HeadP &= ~0xFFFFFFFD; /* clear all but not C */
    pED->HeadP |= (((uint32_t)TDQHeadPntr) & 0xFFFFFFF2); /* HeadP: TDQueueHeadPointer */
                                                          /* C: toggleCarry: This bit is the data toggle carry bit. */

    /* Dword 3, Next Endpoint Descriptor (NextED) */
    pED->NextEd = ((uint32_t)NextED) & 0xFFFFFFF0;      /* NextED */
}

/* Function:
    void ohci_received_size( uint32_t * BuffSize )

   Summary:
    Change the received size if needed

   Description:
    Change the received size if the requesting data number have not been sent

   Remarks:
    Refer to .h for usage information.
 */
void ohci_received_size( uint32_t * BuffSize )
{
    /* If CurrentBufferPointer == 0, all data has been receive */
    if( OHCI_QueueTD[0][1].CBP != 0 )
    {
        *BuffSize = OHCI_QueueTD[0][1].BE - OHCI_QueueTD[0][1].CBP;
    }
}

/* Function:
    static void ohci_create_qTD(OHCIQueueTDDescriptor * qTD_base_addr,
                            OHCIQueueTDDescriptor * NextTD,
                            uint32_t DataToggle,
                            uint32_t BuffLen,
                            uint32_t Direction,
                            uint32_t BufRnding,
                            uint32_t * CurBufPtr)

   Summary:
    Create Transfer Descriptor

   Description:
    Fill Transfer Descriptor

   Remarks:
    Refer to .h for usage information.
 */
static void ohci_create_qTD(OHCIQueueTDDescriptor * qTD_base_addr,
                            OHCIQueueTDDescriptor * NextTD,
                            uint32_t DataToggle,
                            uint32_t BuffLen,
                            uint32_t Direction,
                            uint32_t BufRnding,
                            uint32_t * CurBufPtr)
{
    OHCIQueueTDDescriptor *pTD = (OHCIQueueTDDescriptor*) qTD_base_addr;

    /* 4.3.1.1 General Transfer Descriptor Format
     * Table 4-2: Field Definitions for General TD
     * Dword 0: CC, EC, T, DI, DP, R
     * The unused portion of this Dword must either not be written by Host controller, or must be read and then written back unmodified.
     * T: Manual: MSB=1, 2=DATA0, 3=DATA1. ToggleCarry: MSB=0, Tvalue 0 or 1
     * T: (0=automatic) */
    pTD->Control.td_control &= 0x0007FFFF;          /* Clear all permit bits. */
    pTD->Control.td_control |= (DataToggle << 24) | /* T: DataToggle */
                                      (DIT << 21) | /* DI: DelayInterrupt 7=no interrupt */
                                (Direction << 19) | /* DP: Direction/PID: 0=setup, 1=out, 2=in */
                                (BufRnding << 18);  /* R: bufferRounding */
    /* EC: ErrorCount: For each transmission error, this value is incremented. */
    /* CC: ConditionCode: This field contains the status of the last attempted transaction. */

    /* Dword 1: Current Buffer Pointer (CBP) */
    pTD->CBP = (uint32_t)CurBufPtr;  /* CBP: CurrentBufferPointer, 0 indicates a ZLP */

    /* Dword 2: Next TD (NextTD) */
    pTD->NextTD = (uint32_t)NextTD & 0xFFFFFFF0;  /* NextTD */

    /* Dword 3: Buffer End (BE): Contains physical address of the last byte in the buffer for this TD */
    pTD->BE = (BuffLen) ? ((uint32_t)CurBufPtr + BuffLen - 1) : (uint32_t)CurBufPtr;  /* BE: BufferEnd */
}

/* Function:
    void _DRV_USB_UHP_HOST_OhciInit(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    OHCI initialization

   Description:
    

   Remarks:
    Refer to .h for usage information.
 */
void _DRV_USB_UHP_HOST_OhciInit(DRV_USB_UHP_OBJ *hDriver)
{
    volatile UhpOhci *usbIDOHCI = hDriver->usbIDOHCI;
    volatile uint32_t read_data;
    uint32_t Interval;

    /* HostControllerReset: initiate a software reset of Host Controller */
//    usbIDOHCI->UHP_OHCI_HCCOMMANDSTATUS |= UHP_OHCI_HCCOMMANDSTATUS_HCR_Msk;
 
    /* 10 µs max */
//    while( (usbIDOHCI->UHP_OHCI_HCCOMMANDSTATUS & UHP_OHCI_HCCOMMANDSTATUS_HCR_Msk) == UHP_OHCI_HCCOMMANDSTATUS_HCR_Msk )
    
    for( uint8_t i=0; i<32; i++)
    {
        HCCA.UHP_HccaInterruptTable[i] = 0; 
    }

    /* Set the HcHCCA to the physical address of the HCCA block. */
    usbIDOHCI->UHP_OHCI_HCHCCA = (uint32_t) &HCCA; 

    memset(OHCI_QueueHead, 0, sizeof(OHCI_QueueHead));
    memset(OHCI_QueueTD, 0, sizeof(OHCI_QueueTD));

    /* HcFmInterval register is used to control the length of USB frames */
    Interval = (usbIDOHCI->UHP_OHCI_HCFMINTERVAL & 0x00003FFF);
    Interval |= (((Interval - MAXIMUM_OVERHEAD) * 6) / 7) << 16;
    Interval |= 0x80000000 & (0x80000000 ^ (usbIDOHCI->UHP_OHCI_HCFMREMAINING));
    usbIDOHCI->UHP_OHCI_HCFMINTERVAL = Interval;
    
    /* Set HcPeriodicStart to a value that is 90% of the value in FrameInterval field of the HcFmInterval register. */
    usbIDOHCI->UHP_OHCI_HCPERIODICSTART = OHCI_PRDSTRT;

    /* ClearPortEnable: clear the PortEnableStatus bit: 0 = port is disabled */
    *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk;
    
    /* USB is in suspend state, set it to operational */
    usbIDOHCI->UHP_OHCI_HCCONTROL = (usbIDOHCI->UHP_OHCI_HCCONTROL & ~UHP_OHCI_HCCONTROL_HCFS_Msk) | UHP_OHCI_HCCONTROL_HCFS( DRV_USB_UHP_HOST_OHCI_STATE_USBOPERATIONAL );

    /* check for port power switching
     * Put power is MANDATORY is order to detect connection */
    read_data = usbIDOHCI->UHP_OHCI_HCRHDESCRIPTORA;

    /* Only if ports are power switched (NoPowerSwitching) */
    if ((read_data & UHP_OHCI_HCRHDESCRIPTORA_NPS) == 0)  /* NPS bit */
    { 
        /* PortPowerControlMask */
        if((read_data & UHP_OHCI_HCRHDESCRIPTORA_PSM_Msk) == UHP_OHCI_HCRHDESCRIPTORA_PSM_Msk) /* PSM */
        {
            /* PortPowerControlMask
             * Ganged-power mask on Port #x */
            usbIDOHCI->UHP_OHCI_HCRHDESCRIPTORB |= UHP_OHCI_HCRHDESCRIPTORB_PPCM_Msk;
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rPerPort Power Switching is implemented");
            /* SetPortPower */
            *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) |= UHP_OHCI_HCRHPORTSTATUS0_PPS_Msk; /* PPS */
        }
        else
        {
            /* SetGlobalPower: turn on power to all ports */
            usbIDOHCI->UHP_OHCI_HCRHSTATUS |= UHP_OHCI_HCRHSTATUS_LPSC_Msk;
        }
    }
}



// *****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UHP_HOST_IRPSubmitOhci
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
USB_ERROR DRV_USB_UHP_HOST_IRPSubmitOhci
(
    DRV_USB_UHP_HOST_PIPE_HANDLE hPipe,
    USB_HOST_IRP * inputIRP
)
{
    USB_HOST_IRP_LOCAL * irpIterator;
    DRV_USB_UHP_HOST_TRANSFER_GROUP *controlTransferGroup;
    USB_HOST_IRP_LOCAL * irp = (USB_HOST_IRP_LOCAL *)inputIRP;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe = (DRV_USB_UHP_HOST_PIPE_OBJ *)(hPipe);
    DRV_USB_UHP_OBJ *hDriver;
    uint32_t tosend;
    uint32_t nbBytes;
    uint8_t *point;
    uint8_t idx = 0;
    uint8_t idx_plus;
    volatile UhpOhci *usbIDOHCI;
    uint8_t stop = 0;
    uint8_t DToggle = 0;
    uint8_t low_speed = 0;
    USB_ERROR returnValue = USB_ERROR_PARAMETER_INVALID;
    uint32_t i;
    uint32_t *QHToProceed = NULL;

    if((pipe == NULL) || (hPipe == (DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID)))
    {
        /* This means an invalid pipe was specified.  Return with an error */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Pipe handle is not valid");
    }
    else
    {
        hDriver = (DRV_USB_UHP_OBJ *)(pipe->hClient);
        usbIDOHCI = hDriver->usbIDOHCI;
        controlTransferGroup = &hDriver->controlTransferGroup;

        /* Low speed is too slow, HID keyboard not working */
        if(hDriver->deviceSpeed == USB_SPEED_LOW)
        {
            while(hDriver->blockPipe == 1)
            {
            }
        }        
        hDriver->blockPipe = 1;

        /* Assign owner pipe */
        irp->pipe      = hPipe;
        irp->status    = USB_HOST_IRP_STATUS_PENDING;
        irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;
        hDriver->hostPipeInUse = pipe->hostEndpoint;

        /* We need to disable interrupts was the queue state
         * does not change asynchronously */

        if(!hDriver->isInInterruptContext)
        {
            if(OSAL_MUTEX_Lock(&(hDriver->mutexID), OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex lock failed in DRV_USB_UHP_HOST_IRPSubmitOhci()");
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

            irp->next = NULL;
            irp->completedBytes = 0;
            irp->status = USB_HOST_IRP_STATUS_PENDING;

            if(hDriver->deviceSpeed == USB_SPEED_LOW)
            {
                low_speed = 1;
            }
            if(pipe->irpQueueHead == NULL)
            {
                /* This means that there are no IRPs on this pipe. We can add
                 * this IRP directly */

                irp->previous = NULL;
                pipe->irpQueueHead = irp;

                if(pipe->pipeType == USB_TRANSFER_TYPE_CONTROL)
                {
                    /* We need to update the flags parameter of the IRP
                     * to indicate the direction of the control transfer. */
               
                    /* SETUP */
                    /* Set the initial stage of the IRP */
                    irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;

                    /* We need to update the flags parameter of the IRP to
                     * indicate the direction of the control transfer. */

                    if(*((uint8_t*)(irp->setup)) & 0x80)
                    {
                        /* Device to Host: IN */
                        /* This means the data stage moves from device to host.
                         * Set bit 15 of the flags parameter */

                        irp->flags |= 0x80;
                    }
                    else
                    {
                        /* Host to Device: OUT */
                        /* This means the data stage moves from host to device.
                         * Clear bit 15 of the flags parameter. */

                        irp->flags &= 0x7F;
                    }

                    /* We need to check if the endpoint 0 is free and if so
                     * then start processing the IRP */

                    if (controlTransferGroup->currentIRP == NULL)
                    {
                        /* This means that no IRPs are being processed
                         * So we should start the IRP processing. Else
                         * the IRP processing will start in interrupt.
                         * We start by copying the setup command */
                        controlTransferGroup->currentIRP  = irp;
                        controlTransferGroup->currentPipe = pipe;
                        irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
                        /* Send the setup packet to device */

                        /* SETUP Transaction */
                        point = (uint8_t *)irp->setup;
                        for (i = 0; i < 8; i++)
                        {
                            setupPacket[i] = point[i];
                        }

                        idx = 0;
                        idx_plus = idx + 1;

                        /* SETUP packet PID */
                        ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx],      /* Transfer Descriptor address */
                                        &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                        DToggle | 0x2,           /* T: DataToggle value is taken from the LSb of this field. */
                                        8,                       /* BE: BufferEnd: Total Bytes to transfer */
                                        0,                       /* DP: Direction/PID: SETUP=0 */
                                        0,                       /* R: bufferRounding: exactly fill the defined data buffer */
                                        (uint32_t *)setupPacket);/* CBP: CurrentBufferPointer (must be aligned) */

                        if (*((uint8_t *)(irp->setup)) & 0x80)
                        {
                            /* SETUP IN: (1<<7) Device to host */
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
                                /* IN transaction */
                                /* Setup DATA IN packet */
                                ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx],      /* Transfer Descriptor address */
                                                &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                                (++DToggle)|0x02,        /* T: DataToggle value is taken from the LSb of this field. */
                                                tosend,                  /* BE: BufferEnd: Total Bytes to transfer */
                                                2,                       /* DP: Direction/PID: IN=2 */
                                                1,                       /* R: bufferRounding: data packet may be smaller than the defined buffer. */
                                                (uint32_t *)(USBBufferAligned + irp->completedBytes)); /* CBP: CurrentBufferPointer */

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
                            ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx],      /* Transfer Descriptor address */
                                            &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                            3,        /* T: DataToggle, STATUS is always DATA1 */
                                            0,        /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                            1,        /* DP: Direction/PID: OUT=1 */
                                            0,        /* R: bufferRounding: exactly fill the defined data buffer */
                                            NULL);    /* CBP: CurrentBufferPointer ZLP */
                        }
                        else
                        {
                                /* For non control transfers, if this is the first irp in
                                 * the queue, then we can start the irp */

                                irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;

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
                                ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx],      /* Transfer Descriptor address */
                                                &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                                3,          /* T: DataToggle value is taken from the LSb of this field. */
                                                irp->size,  /* BE: BufferEnd: Total Bytes to transfer */
                                                1,          /* DP: Direction/PID: OUT=1 */
                                                0,          /* R: bufferRounding: exactly fill the defined data buffer */
                                                (uint32_t *)USBBufferAligned); /* CBP: CurrentBufferPointer */
                                DCACHE_CLEAN_BY_ADDR((uint32_t *)irp->data, sizeof(irp->data)); 
                            }

                            idx++;
                            idx_plus = idx + 1;
                            /* Setup STATUS IN Packet (ZLP) */
                            ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx],      /* Transfer Descriptor address */
                                            &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                            3,     /* T: DataToggle value is taken from the LSb of this field. */
                                            0,     /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                            2,     /* DP: Direction/PID: IN=2 */
                                            0,     /* R: bufferRounding: exactly fill the defined data buffer */
                                            NULL); /* CBP: CurrentBufferPointer ZLP */
                        }

                        idx++;
                        /* NULL Packet */
                        ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                        NULL,               /* NextTD */
                                        0,                  /* T: DataToggle acquired from the toggleCarry field in the ED */
                                        0,                  /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                        1,                  /* DP: Direction/PID: OUT=1 */
                                        0,                  /* R: bufferRounding: exactly fill the defined data buffer */
                                        NULL);              /* CBP: CurrentBufferPointer ZLP */

                         /* Create Queue Head for the command: */
                        ohci_create_queue_head(&OHCI_QueueHead[pipe->hostEndpoint],        /* Endpoint Descriptor Addr */
                                               NULL,                   /* NextED */
                                               3,                      /* D: Direction: 1 OUT, 2 IN, 0 or 3 => def in TD */
                                       pipe->endpointAndDirection&0xF, /* EN: EndpointNumber */
                                               pipe->endpointSize,     /* MPS: MaximumPacketSize */
                                               0,                      /* F: Format: 1 Iso, 0 for others */
                                               0,                      /* K: sKip */
                                               low_speed,              /* S: Speed: 0 FS, 1 LS */
                                               pipe->deviceAddress,    /* FA: FunctionAddress */
                                               &OHCI_QueueTD[pipe->hostEndpoint][0],       /* HeadP: TDQueueHeadPointer */
                                               &OHCI_QueueTD[pipe->hostEndpoint][idx]);    /* TailP: TDQueueTailPointer */

                        /* The size of the data buffer  should not be larger than MaximumPacketSize
                         * from the Endpoint Descriptor (this is not checked by the Host Controller and 
                         * transmission problems occur if software violates this restriction). */
                        QHToProceed = (uint32_t *)&OHCI_QueueHead[pipe->hostEndpoint];

                    }
                }
                else
                {
                    /* For non control transfers, if this is the first
                     * irp in the queue, then we can start the irp */
                    idx = 0;
                    idx_plus = idx + 1;

                    irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
                    irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;

                    if( (pipe->endpointAndDirection & 0x80) == 0 )
                    {
                        /* Host to Device: OUT */

                        /* Data is moving from device to host
                         * We need to set the Rx Packet Request bit */
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

                            /* Host to Device: OUT */
                            ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx],      /* Transfer Descriptor address */
                                            &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                            hDriver->staticDToggleOut|0x2,  /* T: DataToggle value is taken from the LSb of this field. */
                                            tosend,                  /* BE: BufferEnd: Total Bytes to transfer */
                                            1,                       /* DP: Direction/PID: OUT=1 */
                                            0,                       /* R: bufferRounding: exactly fill the defined data buffer */
                                            (uint32_t *)((uint32_t)irp->data + irp->completedBytes)); /* data buffer address base, 32-Byte align */

                            DCACHE_CLEAN_BY_ADDR((uint32_t *)irp->data, irp->size);
                            hDriver->staticDToggleOut++;
                            idx++;
                            idx_plus = idx + 1;
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

                        /* If TailP and HeadP are the same, then the list contains no TD that the HC can process. */
                        /* NULL Packet */
                        ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                        NULL,               /* NextTD */
                                        0,                  /* T: DataToggle acquired from the toggleCarry field in the ED */
                                        0,                  /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                        1,                  /* DP: Direction/PID: OUT=1 */
                                        0,                  /* R: bufferRounding: exactly fill the defined data buffer */
                                        NULL);              /* CBP: CurrentBufferPointer ZLP */

                        /* Create Queue Head for the command: */
                        ohci_create_queue_head(&OHCI_QueueHead[pipe->hostEndpoint],     /* Endpoint Descriptor Addr */
                                               NULL,                   /* NextED */
                                               0,                      /* D: Direction: 1 OUT, 2 IN, 0 or 3 => def in TD */
                                       pipe->endpointAndDirection&0xF, /* EN: EndpointNumber */
                                               pipe->endpointSize,     /* MPS: MaximumPacketSize */
                                               0,                      /* F: Format: 1 Iso, 0 for others */
                                               0,                      /* K: sKip */
                                               low_speed,              /* S: Speed: 0 FS, 1 LS */
                                               pipe->deviceAddress,    /* FA: FunctionAddress */
                                               &OHCI_QueueTD[pipe->hostEndpoint][0],       /* HeadP: TDQueueHeadPointer */
                                               &OHCI_QueueTD[pipe->hostEndpoint][idx]);    /* TailP: TDQueueTailPointer */
                        QHToProceed = (uint32_t *)&OHCI_QueueHead[pipe->hostEndpoint];
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
                            }
                            else
                            {
                                tosend = nbBytes;
                            }
                            /* IN */
                            ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx],      /* Transfer Descriptor address */
                                            &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                            hDriver->staticDToggleIn|0x2, /* T: DataToggle value is taken from the LSb of this field. */
                                            tosend,                  /* BE: BufferEnd: Total Bytes to transfer */
                                            2,                       /* DP: Direction/PID: IN=2 */
                                            0,                       /* R: bufferRounding: exactly fill the defined data buffer */
                                            (uint32_t *)((uint32_t)irp->data + irp->completedBytes)); /* CBP: CurrentBufferPointer */

                            DCACHE_CLEAN_BY_ADDR((uint32_t *)irp->data, irp->size);

                            hDriver->staticDToggleIn++;
                            idx++;
                            idx_plus = idx + 1;
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

                        /* If TailP and HeadP are the same, then the list contains no TD that the HC can process. */
                        /* NULL Packet */
                        ohci_create_qTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                        NULL,               /* NextTD */
                                        0,                  /* T: DataToggle acquired from the toggleCarry field in the ED */
                                        0,                  /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                        2,                  /* DP: Direction/PID: IN=2 */
                                        0,                  /* R: bufferRounding: exactly fill the defined data buffer */
                                        NULL);              /* CBP: CurrentBufferPointer ZLP */

                        /* Create Queue Head for the command: */
                        ohci_create_queue_head(&OHCI_QueueHead[pipe->hostEndpoint],      /* Endpoint Descriptor Addr */
                                               NULL,                   /* NextED */
                                               0,                      /* D: Direction: 1 OUT, 2 IN, 0 or 3 => def in TD */
                                       pipe->endpointAndDirection&0xF, /* EN: EndpointNumber */
                                               pipe->endpointSize,     /* MPS: MaximumPacketSize */
                                               0,                      /* F: Format: 1 Iso, 0 for others */
                                               0,                      /* K: sKip */
                                               low_speed,              /* S: Speed: 0 FS, 1 LS */
                                               pipe->deviceAddress,    /* FA: FunctionAddress */
                                               &OHCI_QueueTD[pipe->hostEndpoint][0],       /* HeadP: TDQueueHeadPointer: Points to the next TD to be processed for this endpoint. */
                                               &OHCI_QueueTD[pipe->hostEndpoint][idx]);    /* TailP: TDQueueTailPointer */
                        QHToProceed = (uint32_t *)&OHCI_QueueHead[pipe->hostEndpoint];
                    }
                }

                __DSB();
                if( QHToProceed != NULL )
                {
//                if( pipe->pipeType == USB_TRANSFER_TYPE_INTERRUPT ) 
//                {
//                    /* programming of HccaInterruptTable */
//                    /* An ED that is in only one list has a polling rate of once every 32 ms. */
////                    for( uint8_t i=0; i<32; i++)
////                    {
////                        HCCA.UHP_HccaInterruptTable[i] = (uint32_t)QHToProceed; 
////                    }
//                    HCCA.UHP_HccaInterruptTable[31] = (uint32_t)QHToProceed; 
//                    // HcPeriodicStart
//                    // PeriodCurrentED
//                    // HcPeriodicStart Register
//                    /* PeriodicListEnable: enable the processing of the periodic list */
//                    usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_PLE_Msk;
//                }
//                else 
                    if( pipe->pipeType == USB_TRANSFER_TYPE_ISOCHRONOUS )
                    {
                        /* IsochronousEnable: enable/disable processing of isochronous EDs */
                        usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_IE_Msk;

                        // Change Format in Endpoint Descriptor
                        // IsochronousListEnable
                    }
                    else if(( pipe->pipeType == USB_TRANSFER_TYPE_BULK )
                         || ( pipe->pipeType == USB_TRANSFER_TYPE_INTERRUPT ) )

                    {
                        /* BulkListFilled: programming HcCommandStatus Register */
                        usbIDOHCI->UHP_OHCI_HCCOMMANDSTATUS |= UHP_OHCI_HCCOMMANDSTATUS_BLF_Msk;

                        /* programming of BulkCurrentED */
                        usbIDOHCI->UHP_OHCI_HCBULKCURRENTED = (uint32_t)QHToProceed;

                        /* programming of HcBulkHeadED */
                        usbIDOHCI->UHP_OHCI_HCBULKHEADED = (uint32_t)QHToProceed;

                        /* BulkListEnable: Enable the processing of the Bulk list in the next Frame */
                        usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_BLE_Msk;
                    }
                    else 
                    {
                        /* USB_TRANSFER_TYPE_CONTROL */

                        /* ControlListFilled: programming HcCommandStatus Register */
                        usbIDOHCI->UHP_OHCI_HCCOMMANDSTATUS |= UHP_OHCI_HCCOMMANDSTATUS_CLF_Msk;

                        /* programming of HcControlCurrentED */
                        usbIDOHCI->UHP_OHCI_HCCONTROLCURRENTED = (uint32_t)QHToProceed;

                        /* programming of HcControlHeadED */
                        usbIDOHCI->UHP_OHCI_HCCONTROLHEADED = (uint32_t)QHToProceed;

                        /* ControlListEnable: Enable the processing of the Control list in the next Frame */
                        usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_CLE_Msk;
                    }
                    __DMB();
                    __ISB();
                    /* Enable interrupts */
                    usbIDOHCI->UHP_0HCI_HCINTERRUPTENABLE = UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_SO   |   /* SchedulingOverrun */
                                                            UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_WDH  |   /* WritebackDoneHead */
                                                          /*UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_SF   |*/ /* StartofFrame      */
                                                            UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_RD   |   /* ResumeDetected    */
                                                            UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_UE   |   /* UnrecoverableError  */
                                                            UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_FNO  |   /* FrameNumberOverflow */
                                                            UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_RHSC |   /* RootHubStatusChange */
                                                            UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_OC   |   /* OwnershipChange     */
                                                            UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_MIE;     /* MasterInterruptEnable */

                    irp->status = USB_HOST_IRP_STATUS_IN_PROGRESS;
                    returnValue = USB_ERROR_NONE;
                }
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

                if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex unlock failed");
                }
            }
            returnValue = USB_ERROR_NONE;
        }
    }
    return (returnValue);
}/* end of DRV_USB_UHP_IRPSubmit() */

// *****************************************************************************
/* Function:
    void _DRV_USB_UHP_HOST_DisableControlList_OHCI(DRV_USB_UHP_OBJ *hDriver)

  Summary:
    Disable the processing of the Control list
	
  Description:
    Disable the processing of the Control list

  Remarks:
    See drv_xxx.h for usage information.
*/
void _DRV_USB_UHP_HOST_DisableControlList_OHCI(DRV_USB_UHP_OBJ *hDriver)
{
    volatile UhpOhci *usbIDOHCI;

    usbIDOHCI = hDriver->usbIDOHCI;

    /* Disable Asynchronou list */
    usbIDOHCI->UHP_OHCI_HCCONTROL &= ~UHP_OHCI_HCCONTROL_CLE_Msk;
}

// *****************************************************************************
/* Function:
    void _DRV_USB_UHP_HOST_Tasks_ISR_OHCI(DRV_USB_UHP_OBJ *hDriver)

  Summary:
    Interrupt handler
	
  Description:
    Management of all OHCI interrupt

  Remarks:
    See drv_xxx.h for usage information.
*/
void _DRV_USB_UHP_HOST_Tasks_ISR_OHCI(DRV_USB_UHP_OBJ *hDriver)
{
    volatile UhpOhci *usbIDOHCI = hDriver->usbIDOHCI;
    uint32_t read_data;
    uint32_t i;
    uint32_t td;
    uint32_t endpoint = 0xFF;
    uint32_t ContextInfo;

    if ((uint32_t)HCCA.UHP_HccaDoneHead != 0 )
    {
        ContextInfo = UHP_OHCI_HCINTERRUPTSTATUS_WDH; /* note interrupt processing required */
        if ((uint32_t)HCCA.UHP_HccaDoneHead & 1) 
        {
            ContextInfo |= usbIDOHCI->UHP_OHCI_HCINTERRUPTSTATUS & usbIDOHCI->UHP_0HCI_HCINTERRUPTENABLE;
        }
    }
    else
    {
        ContextInfo = usbIDOHCI->UHP_OHCI_HCINTERRUPTSTATUS & usbIDOHCI->UHP_0HCI_HCINTERRUPTENABLE;
        if(ContextInfo == 0)
        {
            return;  /* not my interrupt */
        }
    }

    /*
     * It is our interrupt, prevent HC from doing it to us again until we are finished
     */
    usbIDOHCI->UHP_OHCI_HCINTERRUPTDISABLE = UHP_OHCI_HCINTERRUPTDISABLE_MIE_Msk;

    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_UE_Msk)
    {
        /*
         * The controller is hung, maybe resetting it can get it going again.
         */
        /* Unrecoverable Error
         * The Host Controller sets the UnrecoverableError bit when it detects a system error
         * not related to USB or an error that cannot be reported in any other way. */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\r\033[31mOHCI IRQ : Unrecoverable Error\033[0m");
    }

    if (ContextInfo & (UHP_OHCI_HCINTERRUPTSTATUS_SO_Msk | UHP_OHCI_HCINTERRUPTSTATUS_WDH | UHP_OHCI_HCINTERRUPTSTATUS_SF_Msk | UHP_OHCI_HCINTERRUPTSTATUS_FNO_Msk))
    {
        ContextInfo |= UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_MIE_Msk; // flag for end of frame type interrupts
    }

    /*
     * Check for Schedule Overrun
     */
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_SO_Msk) 
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rOHCI IRQ : Scheduling Overrun");
        usbIDOHCI->UHP_OHCI_HCINTERRUPTSTATUS = UHP_OHCI_HCINTERRUPTSTATUS_SO_Msk; /* acknowledge interrupt */
        ContextInfo &= ~UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_SO_Msk;
    } 

    /*
     * Check for Frame Number Overflow
     * Note: the formula below prevents a debugger break from making the 32-bit frame number run backward.
     */
    if (ContextInfo & UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_FNO_Msk) 
    {
        usbIDOHCI->UHP_OHCI_HCINTERRUPTSTATUS = UHP_OHCI_HCINTERRUPTSTATUS_FNO_Msk; /* acknowledge interrupt */
        ContextInfo &= ~UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_FNO_Msk;
    }
    /*
     * Processor interrupts could be enabled here and the interrupt dismissed at the interrupt
     * controller, but for simplicity this code won?t.
     */
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_RD_Msk)
    {
        /*
         * Resume has been requested by a device on USB. HCD must wait 20ms then put controller in the
         * UsbOperational state. This code is left as an exercise to the reader.
         */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rOHCI IRQ: Resume Detected");
        ContextInfo &= ~UHP_OHCI_HCINTERRUPTSTATUS_RD_Msk;
        usbIDOHCI->UHP_OHCI_HCINTERRUPTSTATUS = UHP_OHCI_HCINTERRUPTSTATUS_RD_Msk;
    }
    
    /*
     * Process the Done Queue
     */
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_WDH_Msk) 
    {
        td = ((uint32_t)HCCA.UHP_HccaDoneHead & 0xFFFFFFFC);
        if( td != 0)
        {
            for(i=0; i<DRV_USB_UHP_PIPES_NUMBER; i++)
            {
                if( (uint32_t)td < OHCI_QueueHead[i].TailP)
                {
                    endpoint = i;
                    break;
                }
            }
            hDriver->hostPipeInUse = endpoint;
            hDriver->intXfrQtdComplete = 1;
            hDriver->blockPipe = 0;
            if( _DRV_USB_UHP_HOST_OHCITESTTD() == 4)
            {
                /* Stall detected */
                hDriver->intXfrQtdComplete = 0xFF;
            }
        }
        /*
         * Done Queue processing complete, notify controller
         */
        HCCA.UHP_HccaDoneHead = 0;
        usbIDOHCI->UHP_OHCI_HCINTERRUPTSTATUS = UHP_OHCI_HCINTERRUPTSTATUS_WDH_Msk;
        ContextInfo &= ~UHP_OHCI_HCINTERRUPTSTATUS_WDH_Msk;
    }
    /*
     * Process Root Hub changes
     */
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_RHSC_Msk) 
    {
        usbIDOHCI->UHP_OHCI_HCINTERRUPTSTATUS = UHP_OHCI_HCINTERRUPTSTATUS_RHSC_Msk; /* clear interrupt */
        /*
         * EmulateRootHubInterruptXfer will complete a HCD_TRANSFER_DESCRIPTOR which
         * we then pass to ProcessDoneQueue to emulate an HC completion
         */
        /*
         * leave RootHubStatusChange set in ContextInfo so that it will be masked off (it won't be unmasked
         * again until another TD is queued for the emulated endpoint)
         */

        /* NDP: NumberDownstreamPorts */
        for (i=0; i<(usbIDOHCI->UHP_OHCI_HCRHDESCRIPTORA & UHP_OHCI_HCRHDESCRIPTORA_NDP_Msk); i++)
        {
            read_data = *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + i);

            /* PortPowerStatus (bit 8) */
            if ( (read_data & UHP_OHCI_HCRHPORTSTATUS0_PPS_Msk) == UHP_OHCI_HCRHPORTSTATUS0_PPS_Msk )
            {
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : Port Power is on: %d", (int)i);
            }
            /* ConnectStatusChange && PortResetStatus */
            if ( ((read_data & UHP_OHCI_HCRHPORTSTATUS0_CSC_Msk) == UHP_OHCI_HCRHPORTSTATUS0_CSC_Msk)
              && ((read_data & UHP_OHCI_HCRHPORTSTATUS0_PRS_Msk) != UHP_OHCI_HCRHPORTSTATUS0_PRS_Msk) )
            {
                /*  Change in CurrentConnectStatus */
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : ConnectStatusChange, port: %d", (int)i);
                memset(OHCI_QueueHead, 0, sizeof(OHCI_QueueHead));

                /* CurrentConnectStatus */
                if( (read_data & UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk) == UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk)
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : Device connected: %d", (int)i);
                    /* New connection */
                    hDriver->deviceAttached = true;
                    hDriver->portNumber = i;
                    /* SetPortEnable */
                    *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_PES_Msk;
                }
                else
                {
                    /* Disconnect */
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : Device Disconnected: %d", (int)i);

                    /* ClearPortEnable */
                    *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk;
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
                    hDriver->portNumber = 0;
                }
                *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_CSC_Msk;
            }
            /* PortEnableStatusChange (bit 17) */
            if ( (read_data & UHP_OHCI_HCRHPORTSTATUS0_PESC_Msk) == UHP_OHCI_HCRHPORTSTATUS0_PESC_Msk )
            {
                /* change in PortEnableStatus */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rOHCI IRQ : PortEnableStatusChange");
                if ( (read_data & UHP_OHCI_HCRHPORTSTATUS0_PES_Msk) == UHP_OHCI_HCRHPORTSTATUS0_PES_Msk )
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : Port is enabled: %d", (int)i);
                }
                else
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : Port is disabled: %d", (int)i);
                }
            }
            /* PortSuspendStatusChange (bit 18) */
            if ( (read_data & UHP_OHCI_HCRHPORTSTATUS0_PSSC_Msk) == UHP_OHCI_HCRHPORTSTATUS0_PSSC_Msk )
            {
                /* This bit is set when the full resume sequence has been completed */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rOHCI IRQ : PortSuspendStatusChange");
            }
            /* PortOwnerCurrentindicatorChange (bit 19) */
            if ( (read_data & UHP_OHCI_HCRHPORTSTATUS0_OCIC_Msk) == UHP_OHCI_HCRHPORTSTATUS0_OCIC_Msk )
            {
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rOHCI IRQ : PortOwnerCurrentindicatorChange");
            }
            /* PortResetStatusChange (bit 20) */
            if ( (read_data & UHP_OHCI_HCRHPORTSTATUS0_PRSC_Msk) == UHP_OHCI_HCRHPORTSTATUS0_PRSC_Msk )
            {
                /* This bit is set at the end of the 10-ms port reset signal.
                 * port reset is complete */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rOHCI IRQ : PortResetStatusChanger");
            }
        }
        ContextInfo &= ~UHP_OHCI_HCINTERRUPTSTATUS_RHSC_Msk;
    }
    
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_OC_Msk)
    {
        /*
         * Only SMM drivers need implement this. See Sections 5.1.1.3.3 and 5.1.1.3.6 for descriptions of what
         * the code here must do.
         */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rOHCI IRQ : Ownership Change");          
    }

    /* SOF */
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_SF_Msk)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\n\rOHCI IRQ : SOF"); 
        usbIDOHCI->UHP_0HCI_HCINTERRUPTENABLE = UHP_OHCI_HCINTERRUPTSTATUS_SF_Msk; /* clear interrupt */
    }

    /*
     * Any interrupts left should just be masked out. (This is normal processing for StartOfFrame and
     * RootHubStatusChange)
     */
    if (ContextInfo & ~UHP_OHCI_HCINTERRUPTDISABLE_MIE_Msk) // any unprocessed interrupts?
    {
        usbIDOHCI->UHP_OHCI_HCINTERRUPTDISABLE = ContextInfo; // yes, mask them    
    }   
    /*
     * We've completed the actual service of the HC interrupts, now we must deal with the effects
     */
    /*
     * Check for Scheduling Overrun limit
     */

    /*
     * If processor interrupts were enabled earlier then they must be disabled here before we re-enable
     * the interrupts at the controller.
     */
    usbIDOHCI->UHP_0HCI_HCINTERRUPTENABLE = UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_MIE_Msk;

}/* end of _DRV_USB_UHP_HOST_Tasks_ISR_OHCI() */


/* ***************************************************************************** */
/* ***************************************************************************** */
/* Root Hub Driver Routines */
/* ***************************************************************************** */
/* ***************************************************************************** */

/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_HOST_ROOT_HUB_OperationEnableOhci(DRV_HANDLE handle, bool enable)

   Summary:
    Root hub enable

   Description:
    

   Remarks:
    Refer to .h for usage information.
 */
void DRV_USB_UHP_HOST_ROOT_HUB_OperationEnableOhci(DRV_HANDLE handle, bool enable)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    volatile UhpOhci *usbIDOHCI = pUSBDrvObj->usbIDOHCI;

    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV_USB_UHP_HOST_ROOT_HUB_OperationEnableOhci");

    /* Check if the handle is valid or opened */
    if((handle == DRV_HANDLE_INVALID) || (!(pUSBDrvObj->isOpened)))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
    }
    else
    {
        if(false == enable)
        {
             /* If the root hub operation is disable, we disable detach and
             * attached event and switch off the port power. */
            /* Disable interrupt generation */
            usbIDOHCI->UHP_0HCI_HCINTERRUPTENABLE &= ~UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_MIE;
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
            usbIDOHCI->UHP_0HCI_HCINTERRUPTENABLE = UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_RD_Msk   |   /* ResumeDetected    */
                                                    UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_RHSC_Msk |   /* RootHubStatusChange */
                                                    UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_MIE_Msk;     /* MasterInterruptEnable */

            /* Enable VBUS */
            if(pUSBDrvObj->rootHubInfo.portPowerEnable != NULL)
            {
                /* This USB module has only one port. So we call this function
                 * once to enable the port power on port 0 */
                pUSBDrvObj->rootHubInfo.portPowerEnable(0 /* Port 0 */, true);
            }
        }
    }
} /* end of DRV_USB_UHP_HOST_ROOT_HUB_OperationEnableOhci() */


