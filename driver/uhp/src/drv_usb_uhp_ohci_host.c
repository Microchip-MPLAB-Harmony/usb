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
    .hostIRPSubmit     = DRV_USB_UHP_OHCI_HOST_IRPSubmit,
    .hostIRPCancel     = DRV_USB_UHP_IRPCancel,
    .hostPipeSetup     = DRV_USB_UHP_OHCI_PipeSetup,
    .hostPipeClose     = DRV_USB_UHP_OHCI_PipeClose,
    .hostEventsDisable = DRV_USB_UHP_EventsDisable,
    .endpointToggleClear = DRV_USB_UHP_EndpointToggleClear,
    .hostEventsEnable  = DRV_USB_UHP_EventsEnable,
    .rootHubInterface.rootHubPortInterface.hubPortReset           = DRV_USB_UHP_ROOT_HUB_PortReset,
    .rootHubInterface.rootHubPortInterface.hubPortSpeedGet        = DRV_USB_UHP_ROOT_HUB_PortSpeedGet,
    .rootHubInterface.rootHubPortInterface.hubPortResetIsComplete = DRV_USB_UHP_ROOT_HUB_PortResetIsComplete,
    .rootHubInterface.rootHubPortInterface.hubPortSuspend         = DRV_USB_UHP_ROOT_HUB_PortSuspend,
    .rootHubInterface.rootHubPortInterface.hubPortResume          = DRV_USB_UHP_ROOT_HUB_PortResume,
    .rootHubInterface.rootHubMaxCurrentGet      = DRV_USB_UHP_ROOT_HUB_MaximumCurrentGet,
    .rootHubInterface.rootHubPortNumbersGet     = DRV_USB_UHP_ROOT_HUB_PortNumbersGet,
    .rootHubInterface.rootHubSpeedGet           = DRV_USB_UHP_ROOT_HUB_BusSpeedGet,
    .rootHubInterface.rootHubInitialize         = DRV_USB_UHP_ROOT_HUB_Initialize,
    .rootHubInterface.rootHubOperationEnable    = DRV_USB_UHP_OHCI_HOST_ROOT_HUB_OperationEnable,
    .rootHubInterface.rootHubOperationIsEnabled = DRV_USB_UHP_ROOT_HUB_OperationIsEnabled
};

typedef union __attribute__ ((packed))
{
    struct
    {
        uint32_t FAddr :  7; /* FunctionAddress */
        uint32_t EptNum:  4; /* EndpointNumber */
        uint32_t Direc :  2; /* Direction */
        uint32_t Speed :  1; /* Speed */
        uint32_t sKip  :  1; /* sKip */
        uint32_t Format:  1; /* Format */
        uint32_t MPSize: 11; /* MaximumPacketSize */
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
        uint32_t buffR :  1; /* bufferRounding */
        uint32_t DirPid:  2; /* Direction/PID */
        uint32_t DelIt :  3; /* DelayInterrupt */
        uint32_t dataTo:  2; /* DataToggle */
        uint32_t ErrCo :  2; /* ErrorCount */
        uint32_t ConCod:  4; /* ConditionCode */
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

#define DIT   6    /* DelayInterrupt (7 == no interrupt) */

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
/* pQueueHeadCurrentOHCI is a pointer to a queue head that is already in the active list */
OHCIQueueHeadDescriptor* pQueueHeadCurrentOHCIBulk;
OHCIQueueHeadDescriptor* pQueueHeadCurrentOHCICtrl;


// ****************************************************************************
// ****************************************************************************
// Local Functions
// ****************************************************************************
// ****************************************************************************

// *****************************************************************************
/* Function:
    static uitn32_t _DRV_USB_UHP_OHCI_HOST_TestTD(void)

   Summary:
    Test OHCI transfer descriptor

   Description:


   Remarks:
    Refer to .h for usage information.
 */
static uint8_t _DRV_USB_UHP_OHCI_HOST_TestTD(void)
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
              /* SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rNoERROR"); */
              break;
            }
            else if( ConditionCode == 1 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rCRC\033[0m");
            }
            else if( ConditionCode == 2 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rBITSTUFFING\033[0m");
            }
            else if( ConditionCode == 3 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rDATATOGGLEMISMATCH\033[0m");
            }
            else if( ConditionCode == 4 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rSTALL\033[0m");
            }
            else if( ConditionCode == 5 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rDEVICENOTRESPONDING\033[0m");
            }
            else if( ConditionCode == 6 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rPIDCHECKFAILURE\033[0m");
            }
            else if( ConditionCode == 7 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rUNEXPECTEDPID\033[0m");
            }
            else if( ConditionCode == 8 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rDATAOVERRUN 0x%X 0x%X\033[0m", (int)i, (int)j);
            }
            else if( ConditionCode == 9 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rDATAUNDERRUN\033[0m");
            }
            else if( ConditionCode == 10 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rreserved10\033[0m");
            }
            else if( ConditionCode == 11 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rreserved11\033[0m");
            }
            else if( ConditionCode == 12 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rBUFFEROVERRUN\033[0m");
            }
            else if( ConditionCode == 13 )
            {
              SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\033[31m\n\rBUFFERUNDERRUN\033[0m");
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
} /* end _DRV_USB_UHP_OHCI_HOST_TestTD */


// *****************************************************************************
/* Function:
    static void _DRV_USB_UHP_OHCI_HOST_CreateQTD(OHCIQueueTDDescriptor * qTD_base_addr,
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
static void _DRV_USB_UHP_OHCI_HOST_CreateQTD(OHCIQueueTDDescriptor * qTD_base_addr,
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
} /* end _DRV_USB_UHP_OHCI_HOST_CreateQTD */


// ****************************************************************************
// ****************************************************************************
// External Functions
// ****************************************************************************
// ****************************************************************************


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_OHCI_HOST_Init(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    OHCI initialization

   Description:


   Remarks:
    Refer to .h for usage information.
 */
void DRV_USB_UHP_OHCI_HOST_Init(DRV_USB_UHP_OBJ *hDriver)
{
    volatile UhpOhci *usbIDOHCI = hDriver->usbIDOHCI;
    volatile uint32_t read_data;
    uint32_t Interval;

    for( uint8_t i=0; i<32; i++)
    {
        HCCA.UHP_HccaInterruptTable[i] = 0;
    }

    /* Set the HcHCCA to the physical address of the HCCA block. */
    usbIDOHCI->UHP_OHCI_HCHCCA = (uint32_t) &HCCA;
    HCCA.UHP_HccaDoneHead = 0;
    memset(OHCI_QueueHead, 0, sizeof(OHCI_QueueHead));
    memset(OHCI_QueueTD, 0, sizeof(OHCI_QueueTD));

    /* HcFmInterval register is used to control the length of USB frames */
    Interval = (usbIDOHCI->UHP_OHCI_HCFMINTERVAL & 0x00003FFF);
    Interval |= (((Interval - MAXIMUM_OVERHEAD) * 6) / 7) << 16;
    Interval |= 0x80000000 & (0x80000000 ^ (usbIDOHCI->UHP_OHCI_HCFMREMAINING));
    usbIDOHCI->UHP_OHCI_HCFMINTERVAL = Interval;

    /* Set HcPeriodicStart to a value that is 90% of the value in FrameInterval field of the HcFmInterval register. */
    usbIDOHCI->UHP_OHCI_HCPERIODICSTART = OHCI_PRDSTRT;

    usbIDOHCI->UHP_OHCI_HCCONTROL = (usbIDOHCI->UHP_OHCI_HCCONTROL & ~UHP_OHCI_HCCONTROL_HCFS_Msk) | UHP_OHCI_HCCONTROL_HCFS( DRV_USB_UHP_HOST_OHCI_STATE_USBRESUME );

    /* ClearPortEnable: clear the PortEnableStatus bit: 0 = port is disabled */
    *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk;

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
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rPerPort Power Switching is implemented");
            /* SetPortPower */
            *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) |= UHP_OHCI_HCRHPORTSTATUS0_PPS_Msk; /* PPS */
        }
        else
        {
            /* SetGlobalPower: turn on power to all ports */
            usbIDOHCI->UHP_OHCI_HCRHSTATUS |= UHP_OHCI_HCRHSTATUS_LPSC_Msk;
        }
    }
} /* end DRV_USB_UHP_OHCI_HOST_Init */

// *****************************************************************************
/* Function:
    void DRV_USB_UHP_OHCI_HOST_Init_simpl(DRV_USB_UHP_OBJ *hDriver)

   Summary:
    OHCI secind initialization

   Description:


   Remarks:
    Refer to .h for usage information.
 */
void DRV_USB_UHP_OHCI_HOST_Init_simpl(DRV_USB_UHP_OBJ *hDriver)
{
    volatile UhpOhci *usbIDOHCI = hDriver->usbIDOHCI;
    volatile uint32_t read_data;
    uint32_t Interval;

    for( uint8_t i=0; i<32; i++)
    {
        HCCA.UHP_HccaInterruptTable[i] = 0;
    }

    /* Set the HcHCCA to the physical address of the HCCA block. */
    usbIDOHCI->UHP_OHCI_HCHCCA = (uint32_t) &HCCA;

    /* HcFmInterval register is used to control the length of USB frames */
    Interval = (usbIDOHCI->UHP_OHCI_HCFMINTERVAL & 0x00003FFF);
    Interval |= (((Interval - MAXIMUM_OVERHEAD) * 6) / 7) << 16;
    Interval |= 0x80000000 & (0x80000000 ^ (usbIDOHCI->UHP_OHCI_HCFMREMAINING));
    usbIDOHCI->UHP_OHCI_HCFMINTERVAL = Interval;

    /* Set HcPeriodicStart to a value that is 90% of the value in FrameInterval field of the HcFmInterval register. */
    usbIDOHCI->UHP_OHCI_HCPERIODICSTART = OHCI_PRDSTRT;

    usbIDOHCI->UHP_OHCI_HCCONTROL = (usbIDOHCI->UHP_OHCI_HCCONTROL & ~UHP_OHCI_HCCONTROL_HCFS_Msk) | UHP_OHCI_HCCONTROL_HCFS( DRV_USB_UHP_HOST_OHCI_STATE_USBRESUME );

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
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rPerPort Power Switching is implemented");
            /* SetPortPower */
            *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) |= UHP_OHCI_HCRHPORTSTATUS0_PPS_Msk; /* PPS */
        }
        else
        {
            /* SetGlobalPower: turn on power to all ports */
            usbIDOHCI->UHP_OHCI_HCRHSTATUS |= UHP_OHCI_HCRHSTATUS_LPSC_Msk;
        }
    }
} /* DRV_USB_UHP_OHCI_HOST_Init_simpl */


// *****************************************************************************
/* Function:
    DRV_USB_UHP_HOST_PIPE_HANDLE DRV_USB_UHP_OHCI_PipeSetup
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
DRV_USB_UHP_HOST_PIPE_HANDLE DRV_USB_UHP_OHCI_PipeSetup
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
    volatile UhpOhci *usbIDOHCI;
    DRV_USB_UHP_OBJ * hDriver;
    DRV_USB_UHP_HOST_PIPE_OBJ * pipe = NULL;
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle = DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID;
    OHCIQueueHeadDescriptor* pQueueHeadNew;  /* pQueueHeadNew is a pointer to the queue head to be added */
    uint8_t pipeIter = 0;

    if((client == DRV_HANDLE_INVALID) || (((DRV_USB_UHP_OBJ *)client) == NULL))
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid client handle");
    }
    else if(((DRV_USB_UHP_OBJ *)client)->inUse)
    {
        if((speed == USB_SPEED_LOW) || (speed == USB_SPEED_FULL) || (speed == USB_SPEED_HIGH))
        {
            /* wMaxPacketSize can be less than 8 for Non control endpoints. E.g.
             * Interrupt IN endpoints can be 4 bytes for HID Mouse device. This
             * USB module requires minimum 2 to the power 3 i.e. 8 bytes as the
             * wMaxPacketSize.
             *
             * For Control endpoints the minimum has to be 8 bytes.  */
            if(pipeType != USB_TRANSFER_TYPE_CONTROL)
            {
                if(wMaxPacketSize < 8)
                {
                    wMaxPacketSize = 8;
                }
            }
            if((wMaxPacketSize < 8) || (wMaxPacketSize > DRV_USB_UHP_NO_CACHE_BUFFER_LENGTH))
            {
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid pipe endpoint size");
            }
            else
            {
                hDriver = (DRV_USB_UHP_OBJ *)client;
                /* There are two things that we need to check before we allocate
                 * a pipe. We check if have a free pipe object and check if we
                 * have a free endpoint that we can use */

                /* All control transfer pipe are grouped together as a linked
                 * list of pipes.  Non control transfer pipes are organized
                 * individually */

                /* We start by searching for a free pipe */
                if(OSAL_MUTEX_Lock(&hDriver->mutexID, OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex lock failed");
                }
                else
                {
                    for(pipeIter = 0; pipeIter < USB_HOST_PIPES_NUMBER; pipeIter ++)
                    {
                        pipe = &gDrvUSBHostPipeObj[pipeIter];

                        if(false == pipe->inUse)
                        {
                            /* This means we have found a free pipe object */

                            /* Setup the pipe object */
                            pipe->inUse = true;
                            pipe->deviceAddress = deviceAddress;
                            pipe->irpQueueHead = NULL;
                            pipe->bInterval = bInterval;
                            pipe->speed = speed;
                            pipe->hubAddress = hubAddress;
                            pipe->hubPort = hubPort;
                            pipe->pipeType = pipeType;
                            pipe->hClient = client;
                            pipe->endpointSize = wMaxPacketSize;
                            pipe->intervalCounter = bInterval;
                            pipe->hostEndpoint = pipeIter;
                            pipe->endpointAndDirection = endpointAndDirection;

                            usbIDOHCI = hDriver->usbIDOHCI;
                            SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\r\n\033[31mPSt = %d %d @%d \033[0m", pipe->hostEndpoint, pipeIter, pipe->deviceAddress);

                            pQueueHeadNew = &OHCI_QueueHead[pipeIter];
                            pQueueHeadNew->Control.sKip = 1;  /* SKIP */

                            /*
                             * Link endpoint descriptor into HCD tracking queue
                             */
                            if(pipe->pipeType != USB_TRANSFER_TYPE_ISOCHRONOUS)
                            {
                                /*
                                 * Link ED into head of ED list
                                 */
                                /* Dword 0 */
                                pQueueHeadNew->Control.ed_control = (pipe->endpointSize << 16) | /* MPS: MaximumPacketSize */
                                                                  (0 << 15) | /* F: Format: 1 Iso, 0 for others */
                                                                  (1 << 14) | /* K: sKip */
                                                                  (3 << 11) | /* D: Direction: 1 OUT, 2 IN, 0 or 3 => def in TD */
                                     ((pipe->endpointAndDirection&0xF) <<  7) | /* EN: EndpointNumber */
                                                (pipe->deviceAddress <<  0);  /* FA: FunctionAddress */

                                if(pipe->speed == USB_SPEED_LOW)
                                {
                                    pQueueHeadNew->Control.ed_control |= (1 << 13); /* S: Speed 0 FS, 1 LS */
                                }

                                pQueueHeadNew->TailP = (uint32_t)&OHCI_QueueTD[pipeIter][0];
                                pQueueHeadNew->HeadP = (uint32_t)&OHCI_QueueTD[pipeIter][0];

                                if(pipe->pipeType == USB_TRANSFER_TYPE_CONTROL)
                                {
                                    /* Dword 3, Next Endpoint Descriptor (NextED) */
                                    if(pQueueHeadCurrentOHCICtrl == NULL)
                                    {
                                        /* programming of HcControlHeadED */
                                        usbIDOHCI->UHP_OHCI_HCCONTROLHEADED = (uint32_t)pQueueHeadNew;
                                        /* ControlListEnable: Enable the processing of the Control list in the next Frame */
                                        usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_CLE_Msk;
                                        pQueueHeadCurrentOHCICtrl = &OHCI_QueueHead[0];
                                    }
                                    else
                                    {
                                        if( pQueueHeadCurrentOHCICtrl != pQueueHeadNew )
                                        {
                                            pQueueHeadCurrentOHCICtrl->NextEd = ((uint32_t)pQueueHeadNew) & 0xFFFFFFF0;      /* NextED */
                                        }
                                        pQueueHeadCurrentOHCICtrl = pQueueHeadNew;
                                        usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_CLE_Msk;
                                    }
                                }
                                else
                                {
                                    /* Dword 3, Next Endpoint Descriptor (NextED) */
                                    if(pQueueHeadCurrentOHCIBulk == NULL)
                                    {
                                        /* programming of HcBulkHeadED */
                                        usbIDOHCI->UHP_OHCI_HCBULKHEADED = (uint32_t)pQueueHeadNew;
                                        /* BulkListEnable: Enable the processing of the Bulk list in the next Frame */
                                        usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_BLE_Msk;
                                        pQueueHeadCurrentOHCIBulk = &OHCI_QueueHead[pipeIter];
                                    }
                                    else
                                    {
                                        if( pQueueHeadCurrentOHCIBulk != pQueueHeadNew )
                                        {
                                            pQueueHeadCurrentOHCIBulk->NextEd = ((uint32_t)pQueueHeadNew) & 0xFFFFFFF0;      /* NextED */
                                        }
                                        pQueueHeadCurrentOHCIBulk = pQueueHeadNew;
                                        usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_BLE_Msk;
                                    }
                                }

                            }
                            else
                            {
                                /* ISOCHRONOUS */
                                /*
                                 * Link ED into tail of ED list
                                 */
                                /* TailED = CONTAINING_RECORD( List->Head.Blink, HCD_ENDPOINT_DESCRIPTOR, Link);
                                 * InsertTailList (&List->Head, &Endpoint->Link);
                                 * ED->NextED = 0;
                                 * TailED->NextED = ED->PhysicalAddress;
                                 */

                                /* Dword 0 */
                                pQueueHeadNew->Control.ed_control = (pipe->endpointSize << 16) | /* MPS: MaximumPacketSize */
                                                                  (1 << 15) | /* F: Format: 1 Iso, 0 for others */
                                                                  (1 << 14) | /* K: sKip */
                                                                  (0 << 13) | /* S: Speed 0 FS, 1 LS */
                                                                  (3 << 11) | /* D: Direction: 1 OUT, 2 IN, 0 or 3 => def in TD */
                                                 (pipe->hostEndpoint <<  7) | /* EN: EndpointNumber */
                                                (pipe->deviceAddress <<  0);  /* FA: FunctionAddress */

                                /* TailED ? */
                            }

                            if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
                            {
                                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP Driver: Mutex unlock failed in DRV USB_UHP_HOST_PipeSetup()");
                            }
                            else
                            {
                                pipeHandle = (DRV_USB_UHP_HOST_PIPE_HANDLE)pipe;
                                break;
                            }
                        }
                    }
                    if(pipeHandle == DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID)
                    {
                        /* This means that we could not find a free pipe object */
                        if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
                        {
                            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP Driver: Mutex unlock failed in DRV USB_UHP_HOST_PipeSetup()");
                        }
                    }
                }
            }
        }
    }
    else
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP Driver: Invalid client in DRV USB_UHP_HOST_PipeSetup()");
    }
    return (pipeHandle);
}/* end of DRV_USB_UHP_OHCI_PipeSetup() */


// *****************************************************************************
/* Function:
     void DRV_USB_UHP_OHCI_PipeClose(DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle)

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
void DRV_USB_UHP_OHCI_PipeClose
(
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
)
{
    DRV_USB_UHP_OBJ * hDriver = NULL;
    USB_HOST_IRP_LOCAL  * irp = NULL;
    DRV_USB_UHP_HOST_PIPE_OBJ * pipe = NULL;
    OHCIQueueHeadDescriptor* pQueueHeadToUnlink; /* pQHeadToUnlink is a pointer to the queue head to be removed */
    OHCIQueueHeadDescriptor* pQueueHeadPrevious = NULL; /* pQHeadPrevious is a pointer to a queue head that references the queue head to remove */
    uint8_t pipeIter = 0;
    bool interruptWasEnabled = false;

    /* Make sure we have a valid pipe */
    if ((pipeHandle == 0) || (pipeHandle == DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID))
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Invalid pipe handle");
    }
    else
    {
        pipe = (DRV_USB_UHP_HOST_PIPE_OBJ *)pipeHandle;

        /* Make sure that we are working with a pipe in use */
        if(pipe->inUse != true)
        {
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Pipe is not in use");
        }
        else
        {
            hDriver = (DRV_USB_UHP_OBJ *)pipe->hClient;
            /* Disable the interrupt */
            if(!hDriver->isInInterruptContext)
            {
                if(OSAL_MUTEX_Lock(&hDriver->mutexID, OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex lock failed");
                }
                interruptWasEnabled = _DRV_USB_UHP_InterruptSourceDisable(hDriver->interruptSource);
            }

            pipe->inUse = false;

            /* Now we invoke the call back for each IRP in this pipe and say that it is
             * aborted.  If the IRP is in progress, then that IRP will be actually
             * aborted on the next SOF This will allow the USB module to complete any
             * transaction that was in progress. */

            irp = (USB_HOST_IRP_LOCAL *)pipe->irpQueueHead;
            while(irp != NULL)
            {
                irp->pipe = DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID;

                if(irp->status == USB_HOST_IRP_STATUS_IN_PROGRESS)
                {
                    /* If the IRP is in progress, then we set the temp IRP state. This
                     * will be caught in the DRV_USB_UHP_TransferProcess() function */

                    irp->status = USB_HOST_IRP_STATUS_ABORTED;

                    if(irp->callback != NULL)
                    {
                        irp->callback((USB_HOST_IRP*)irp);
                    }
                    irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_ABORTED;
                }
                else
                {
                    /* IRP is pending */
                    irp->status = USB_HOST_IRP_STATUS_ABORTED;

                    if(irp->callback != NULL)
                    {
                        irp->callback((USB_HOST_IRP*)irp);
                    }
                }
                irp = irp->next;
            }

            /* Now we return the pipe back to the driver */
            pipe->inUse = false;

            /* 5.2.7.1.2 Removing */
            SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\r\n\033[31mPCl = %d @%d\033[0m", pipe->hostEndpoint, pipe->deviceAddress);

            /*
             * Prevent Host Controller from processing this ED
             */
            pQueueHeadToUnlink = &OHCI_QueueHead[pipe->hostEndpoint];

            pQueueHeadToUnlink->Control.sKip = 1;  /* SKIP */
            /*
             * Remove ED
             */

            /* Previous ED go to next ED, let unlik unchanged */
            if( 0 != pipe->hostEndpoint )
            {
                if( (pQueueHeadToUnlink->NextEd & 0xFFFFFFF8) != (uint32_t)pQueueHeadToUnlink )
                {
                    /* Find Previous QueuHead */
                    for(pipeIter = 0; pipeIter < USB_HOST_PIPES_NUMBER; pipeIter++)
                    {
                        if( (OHCI_QueueHead[pipeIter].NextEd & 0xFFFFFFF8) == (uint32_t)pQueueHeadToUnlink )
                        {
                            pQueueHeadPrevious = &OHCI_QueueHead[pipeIter];
                            break;
                        }
                    }
                }
                else
                {
                    pQueueHeadPrevious = pQueueHeadToUnlink;
                }
                if( pipeIter == USB_HOST_PIPES_NUMBER)
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: error pipe->hostEndpoint = %d ", pipe->hostEndpoint);
                }
                pQueueHeadPrevious->NextEd = pQueueHeadToUnlink->NextEd;
                /* pQueueHeadToUnlink->Horizontal_Link_Pointer: Stay pointed to the next. */
                if( pipe->pipeType == USB_TRANSFER_TYPE_CONTROL )
                {
                    pQueueHeadCurrentOHCICtrl = pQueueHeadPrevious;
                }
                else
                {
                    pQueueHeadCurrentOHCIBulk = pQueueHeadPrevious;
                }
            }
            else
            {
                if( pipe->pipeType == USB_TRANSFER_TYPE_CONTROL )
                {
                    pQueueHeadCurrentOHCICtrl = pQueueHeadPrevious;
                }
                else
                {
                    pQueueHeadCurrentOHCIBulk = pQueueHeadPrevious;
                }
            }

            /* Enable interrupts */
            if(!hDriver->isInInterruptContext)
            {
                if(interruptWasEnabled)
                {
                    _DRV_USB_UHP_InterruptSourceEnable(hDriver->interruptSource);
                }

                if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex unlock failed");
                }
            }
        }
    }
}/* end of DRV_USB_UHP_OHCI_PipeClose() */


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_OHCI_ClearDataToggle(DRV_USB_UHP_HOST_PIPE_OBJ *pipe);

  Summary:
    Clear the datatoggle for a pipe.

  Description:
    This function clear the datatoggle for a pipe.

  Remarks:
    See .h for usage information.
*/
void DRV_USB_UHP_OHCI_ClearDataToggle(DRV_USB_UHP_HOST_PIPE_OBJ *pipe)
{
    OHCI_QueueHead[pipe->hostEndpoint].HeadP |= 1;   /* Set Halt */
    OHCI_QueueHead[pipe->hostEndpoint].HeadP &= ~2;  /* Remove Carry bit */
    OHCI_QueueHead[pipe->hostEndpoint].HeadP &= ~1;  /* Remove Halt */
} /* end of DRV_USB_UHP_OHCI_ClearDataToggle() */


// *****************************************************************************
/* Function:
    USB_ERROR DRV_USB_UHP_OHCI_HOST_IRPSubmit
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
USB_ERROR DRV_USB_UHP_OHCI_HOST_IRPSubmit
(
    DRV_USB_UHP_HOST_PIPE_HANDLE hPipe,
    USB_HOST_IRP * inputIRP
)
{
    USB_HOST_IRP_LOCAL * irpIterator;
    USB_HOST_IRP_LOCAL * irp = (USB_HOST_IRP_LOCAL *)inputIRP;
    DRV_USB_UHP_HOST_PIPE_OBJ *pipe = (DRV_USB_UHP_HOST_PIPE_OBJ *)(hPipe);
    DRV_USB_UHP_OBJ *hDriver;
    uint8_t *point;
    volatile uint8_t idx = 0;
    volatile uint8_t idx_plus = 0;
    volatile uint8_t idx_eot = 0;
    volatile uint8_t idx_end = 0;
    volatile UhpOhci *usbIDOHCI;
    volatile uint8_t DToggle = 0;
    USB_ERROR returnValue = USB_ERROR_PARAMETER_INVALID;
    volatile uint32_t i;

    if((pipe == NULL) || (hPipe == (DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID)))
    {
        /* This means an invalid pipe was specified.  Return with an error */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Pipe handle is not valid");
    }
    else
    {
        hDriver = (DRV_USB_UHP_OBJ *)(pipe->hClient);
        usbIDOHCI = hDriver->usbIDOHCI;

        /* Assign owner pipe */
        irp->pipe = hPipe;
        irp->status = USB_HOST_IRP_STATUS_PENDING;
        irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;

        /* We need to disable interrupts was the queue state
         * does not change asynchronously */

        if(!hDriver->isInInterruptContext)
        {
            if(OSAL_MUTEX_Lock(&(hDriver->mutexID), OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
            {
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex lock failed in DRV_USB_UHP_OHCI_HOST_IRPSubmit()");
                returnValue = USB_ERROR_OSAL_FUNCTION;
            }
            else
            {
                _DRV_USB_UHP_InterruptSourceDisable(hDriver->interruptSource);
            }
        }
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: IRPSubmit= %d @%d E%d", pipe->hostEndpoint, pipe->deviceAddress, pipe->endpointAndDirection&0xF );

        if(USB_ERROR_OSAL_FUNCTION != returnValue)
        {
            /* This needs to be done for all irp irrespective
             * of type or if there IRP is immediately processed */

            irp->next = NULL;
            irp->completedBytes = 0;
            irp->status = USB_HOST_IRP_STATUS_PENDING;

            if(pipe->irpQueueHead == NULL)
            {
                /* This means that there are no IRPs on this pipe. We can add
                 * this IRP directly */

                irp->previous = NULL;
                pipe->irpQueueHead = irp;

                OHCI_QueueHead[pipe->hostEndpoint].Control.sKip = 1;  /* SKIP */

                /* Find qTD End of Tranfer */
                for (idx_eot = 0; idx_eot < DRV_USB_UHP_MAX_TRANSACTION; idx_eot++)
                {
                    if( (uint32_t)OHCI_QueueHead[pipe->hostEndpoint].TailP == (uint32_t)(&OHCI_QueueTD[pipe->hostEndpoint][idx_eot]))
                    {
                        idx = idx_eot;
                        if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx = 0;
                        }
                        idx_plus = idx + 1;
                        if( idx_plus == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx_plus = 0;
                        }
                        break;
                    }
                }

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

                    /* Send the setup packet to device */

                    /* SETUP Transaction */
                    if (*((uint8_t *)(irp->setup)) & 0x80)
                    {
                        /* SETUP IN: (1<<7) Device to host */
                        idx++;
                        if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx = 0;
                        }
                        idx_plus = idx + 1;
                        if( idx_plus == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx_plus = 0;
                        }

                        /* The host sends an IN packet to allow the device to send the descriptor. */
                        /* IN transaction */
                        /* Setup DATA IN packet */
                        _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                        &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                        (++DToggle)|0x02,        /* T: DataToggle value is taken from the LSb of this field. */
                                        irp->size,               /* BE: BufferEnd: Total Bytes to transfer */
                                        2,                       /* DP: Direction/PID: IN=2 */
                                        1,                       /* R: bufferRounding: data packet may be smaller than the defined buffer. */
                                        (uint32_t *)(USBBufferNoCache[pipe->hostEndpoint] + irp->completedBytes)); /* CBP: CurrentBufferPointer */

                        irp->completedBytes += irp->size;

                        /* The host issues an OUT zero length packet (ZLP) to acknowledge reception of the descriptor. */
                        idx++;
                        if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx = 0;
                        }
                        idx_plus = idx + 1;
                        if( idx_plus == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx_plus = 0;
                        }

                        /* Setup STATUS OUT packet */
                        _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                        &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                        3,        /* T: DataToggle, STATUS is always DATA1 */
                                        0,        /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                        1,        /* DP: Direction/PID: OUT=1 */
                                        0,        /* R: bufferRounding: exactly fill the defined data buffer */
                                        NULL);    /* CBP: CurrentBufferPointer ZLP */
                    }
                    else
                    {
                        /* SETUP OUT: (1<<7) Host to Device */

                        if (irp->size != 0)
                        {
                            /* OUT Packet */
                            idx++;
                            if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                            {
                                idx = 0;
                            }
                            idx_plus = idx + 1;
                            if( idx_plus == DRV_USB_UHP_MAX_TRANSACTION)
                            {
                                idx_plus = 0;
                            }

                            point = (uint8_t *)irp->data;
                            for (i = 0; i < irp->size; i++)
                            {
                                USBBufferNoCache[pipe->hostEndpoint][i] = point[i];
                            }

                            /* Setup DATA OUT Packet */
                            _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                            &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                            3,          /* T: DataToggle value is taken from the LSb of this field. */
                                            irp->size,  /* BE: BufferEnd: Total Bytes to transfer */
                                            1,          /* DP: Direction/PID: OUT=1 */
                                            0,          /* R: bufferRounding: exactly fill the defined data buffer */
                                            (uint32_t *)USBBufferNoCache[pipe->hostEndpoint]); /* Setup DATA OUT Packet */
                            irp->completedBytes += irp->size;
                        }

                        idx++;
                        if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx = 0;
                        }
                        idx_plus = idx + 1;
                        if( idx_plus == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx_plus = 0;
                        }

                        /* Setup STATUS IN Packet (ZLP) */
                        _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                        &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                        3,     /* T: DataToggle value is taken from the LSb of this field. */
                                        0,     /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                        2,     /* DP: Direction/PID: IN=2 */
                                        0,     /* R: bufferRounding: exactly fill the defined data buffer */
                                        NULL); /* CBP: CurrentBufferPointer ZLP */
                    }

                    /* Create new dummy qTD */
                    idx++;
                    if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                    {
                        idx = 0;
                    }
                    _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                    NULL,               /* NextTD */
                                    0,                  /* T: DataToggle acquired from the toggleCarry field in the ED */
                                    0,                  /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                    0,                  /* DP: Direction/PID */
                                    0,                  /* R: bufferRounding: exactly fill the defined data buffer */
                                    NULL);              /* CBP: CurrentBufferPointer ZLP */
                    OHCI_QueueTD[pipe->hostEndpoint][idx].Control.td_control = 0;

                    idx_end = idx;
                    /* The size of the data buffer  should not be larger than MaximumPacketSize
                     * from the Endpoint Descriptor (this is not checked by the Host Controller and
                     * transmission problems occur if software violates this restriction). */

                    /* Then overwrite the old dummy QTD: */
                    idx = idx_eot;
                    if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                    {
                        idx = 0;
                    }
                    idx_plus = idx + 1;
                    if( idx_plus == DRV_USB_UHP_MAX_TRANSACTION)
                    {
                        idx_plus = 0;
                    }

                    point = (uint8_t *)irp->setup;
                    for (i = 0; i < 8; i++)
                    {
                        USBBufferNoCacheSetup[pipe->hostEndpoint][i] = point[i];
                    }

                    /* SETUP packet PID */
                    _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                    &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                    0x2,                     /* T: DataToggle value is taken from the LSb of this field. */
                                    8,                       /* BE: BufferEnd: Total Bytes to transfer */
                                    0,                       /* DP: Direction/PID: SETUP=0 */
                                    0,                       /* R: bufferRounding: exactly fill the defined data buffer */
                                    (uint32_t *)USBBufferNoCacheSetup[pipe->hostEndpoint]); /* CBP: CurrentBufferPointer */

                    /* 3. Advancing the TailP pointer to the new place holder */
                    OHCI_QueueHead[pipe->hostEndpoint].TailP = ((uint32_t)&OHCI_QueueTD[pipe->hostEndpoint][idx_end]);
                }
                else
                {
                    /* For non control transfers, if this is the first
                     * irp in the queue, then we can start the irp */

                    irp->tempState = DRV_USB_UHP_HOST_IRP_STATE_PROCESSING;

                    if( (pipe->endpointAndDirection & 0x80) == 0 )
                    {
                        /* Host to Device: OUT */
                        /* Find the last QTD to be filled */
                        idx = idx_eot+1;
                        if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx = 0;
                        }
                        idx_end = idx;

                       /* Create new dummy qTD */
                       _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                    NULL,               /* NextTD */
                                    0,                  /* T: DataToggle acquired from the toggleCarry field in the ED */
                                    0,                  /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                    0,                  /* DP: Direction/PID: OUT=1 */
                                    0,                  /* R: bufferRounding: exactly fill the defined data buffer */
                                    NULL);              /* CBP: CurrentBufferPointer ZLP */
                        OHCI_QueueTD[pipe->hostEndpoint][idx].Control.td_control = 0;

                        /* Then overwrite the old dummy QTD: */
                        idx = idx_eot;
                        if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx = 0;
                        }
                        idx_plus = idx + 1;
                        if( idx_plus == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx_plus = 0;
                        }

                        point = (uint8_t *)irp->data;
                        for (i = 0; i < irp->size; i++)
                        {
                            USBBufferNoCache[pipe->hostEndpoint][i] = point[i];
                        }

                        /* Host to Device: OUT */
                        _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                        &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                        0,                       /* data toggle */
                                        irp->size,               /* BE: BufferEnd: Total Bytes to transfer */
                                        1,                       /* DP: Direction/PID: OUT=1 */
                                        0,                       /* R: bufferRounding: exactly fill the defined data buffer */
                                    (uint32_t *)(USBBufferNoCache[pipe->hostEndpoint] + irp->completedBytes)); /* CBP: CurrentBufferPointer */
                        irp->completedBytes += irp->size;

                        /* 3. Advancing the TailP pointer to the new place holder */
                        OHCI_QueueHead[pipe->hostEndpoint].TailP = ((uint32_t)&OHCI_QueueTD[pipe->hostEndpoint][idx_end]);
                    }
                    else
                    {
                        /* Device to Host: IN */
                        idx = idx_eot+1;
                        if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx = 0;
                        }
                        idx_end = idx;

                        /* Create new dummy qTD */
                        _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                    NULL,               /* NextTD */
                                    0,                  /* T: DataToggle acquired from the toggleCarry field in the ED */
                                    0,                  /* BE: BufferEnd: Total Bytes to transfer: ZLP */
                                    0,                  /* DP: Direction/PID: OUT=1 */
                                    0,                  /* R: bufferRounding: exactly fill the defined data buffer */
                                    NULL);              /* CBP: CurrentBufferPointer ZLP */
                        OHCI_QueueTD[pipe->hostEndpoint][idx].Control.td_control = 0;

                        /* Then overwrite the old dummy QTD: */
                        idx = idx_eot;
                        if( idx == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx = 0;
                        }
                        idx_plus = idx + 1;
                        if( idx_plus == DRV_USB_UHP_MAX_TRANSACTION)
                        {
                            idx_plus = 0;
                        }

                        /* IN */
                        _DRV_USB_UHP_OHCI_HOST_CreateQTD(&OHCI_QueueTD[pipe->hostEndpoint][idx], /* Transfer Descriptor address */
                                        &OHCI_QueueTD[pipe->hostEndpoint][idx_plus], /* NextTD */
                                        0,                       /* T: DataToggle acquired from the toggleCarry field in the ED */
                                        irp->size,                  /* BE: BufferEnd: Total Bytes to transfer */
                                        2,                       /* DP: Direction/PID: IN=2 */
                                        0,                       /* R: bufferRounding: exactly fill the defined data buffer */
                                        (uint32_t *)(USBBufferNoCache[pipe->hostEndpoint] + irp->completedBytes)); /* CBP: CurrentBufferPointer */

                        irp->completedBytes += irp->size;

                        /* 3. Advancing the TailP pointer to the new place holder */
                        OHCI_QueueHead[pipe->hostEndpoint].TailP = ((uint32_t)&OHCI_QueueTD[pipe->hostEndpoint][idx_end]);
                    }
                }
                OHCI_QueueHead[pipe->hostEndpoint].Control.sKip = 0;  /* SKIP */
                OHCI_QueueHead[pipe->hostEndpoint].HeadP &= ~1;       /* remove Halt */
                __DSB();

                /*
                 * if( pipe->pipeType == USB_TRANSFER_TYPE_INTERRUPT )
                 * {
                 *     programming of HccaInterruptTable
                 *     An ED that is in only one list has a polling rate of once every 32 ms.
                 *     for( uint8_t i=0; i<32; i++)
                 *     {
                 *         HCCA.UHP_HccaInterruptTable[i] = (uint32_t)QHToProceed;
                 *     }
                 *     HCCA.UHP_HccaInterruptTable[31] = (uint32_t)QHToProceed;
                 *     HcPeriodicStart
                 *     PeriodCurrentED
                 *     HcPeriodicStart Register
                 *     PeriodicListEnable: enable the processing of the periodic list
                 *     usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_PLE_Msk;
                 * }
                 * else */
                if( pipe->pipeType == USB_TRANSFER_TYPE_ISOCHRONOUS )
                {
                    /* IsochronousEnable: enable/disable processing of isochronous EDs */
                    usbIDOHCI->UHP_OHCI_HCCONTROL |= UHP_OHCI_HCCONTROL_IE_Msk;

                    /* Change Format in Endpoint Descriptor
                     * IsochronousListEnable */
                }
                else if(( pipe->pipeType == USB_TRANSFER_TYPE_BULK )
                     || ( pipe->pipeType == USB_TRANSFER_TYPE_INTERRUPT ) )
                {
                    /* BulkListFilled: programming HcCommandStatus Register */
                    usbIDOHCI->UHP_OHCI_HCCOMMANDSTATUS |= UHP_OHCI_HCCOMMANDSTATUS_BLF_Msk;
                }
                else
                {
                    /* USB_TRANSFER_TYPE_CONTROL */
                    /* ControlListFilled: programming HcCommandStatus Register */
                    usbIDOHCI->UHP_OHCI_HCCOMMANDSTATUS |= UHP_OHCI_HCCOMMANDSTATUS_CLF_Msk;
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
                _DRV_USB_UHP_InterruptSourceEnable(hDriver->interruptSource);

                if(OSAL_MUTEX_Unlock(&hDriver->mutexID) != OSAL_RESULT_TRUE)
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Mutex unlock failed");
                }
            }
            returnValue = USB_ERROR_NONE;
        }
    }
    return (returnValue);
}/* end of DRV_USB_UHP_OHCI_HOST_IRPSubmit() */


// *****************************************************************************
/* Function:
    void DRV_USB_UHP_OHCI_HOST_Tasks_ISR(DRV_USB_UHP_OBJ *hDriver)

  Summary:
    Interrupt handler

  Description:
    Management of all OHCI interrupt

  Remarks:
    See drv_xxx.h for usage information.
*/
void DRV_USB_UHP_OHCI_HOST_Tasks_ISR(DRV_USB_UHP_OBJ *hDriver)
{
    volatile UhpOhci *usbIDOHCI = hDriver->usbIDOHCI;
    uint32_t read_data;
    uint32_t i, j;
    uint32_t old_it;
    uint32_t td;
    uint32_t ContextInfo;
    OHCIQueueTDDescriptor *done_td;

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
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\r\033[31mOHCI IRQ : Unrecoverable Error\033[0m");
    }

    if (ContextInfo & (UHP_OHCI_HCINTERRUPTSTATUS_SO_Msk | UHP_OHCI_HCINTERRUPTSTATUS_WDH | UHP_OHCI_HCINTERRUPTSTATUS_SF_Msk | UHP_OHCI_HCINTERRUPTSTATUS_FNO_Msk))
    {
        ContextInfo |= UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_MIE_Msk; /* flag for end of frame type interrupts */
    }

    /*
     * Check for Schedule Overrun
     */
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_SO_Msk)
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : Scheduling Overrun");
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
     * controller, but for simplicity this code won t.
     */
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_RD_Msk)
    {
        /*
         * Resume has been requested by a device on USB. HCD must wait 20ms then put controller in the
         * UsbOperational state.
         */
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ: Resume Detected");
        ContextInfo &= ~UHP_OHCI_HCINTERRUPTSTATUS_RD_Msk;
        usbIDOHCI->UHP_OHCI_HCINTERRUPTSTATUS = UHP_OHCI_HCINTERRUPTSTATUS_RD_Msk;
    }

    /*
     * Process the Done Queue
     */
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_WDH_Msk)
    {
//        if(hDriver->deviceSpeed == USB_SPEED_LOW)
//        {
//            /* Patch for low speed device. */
//            test=0;
//            while( test < 9000 )
//            {
//                test++;
//            }
//        }
        //td = ((uint32_t)HCCA.UHP_HccaDoneHead & 0xFFFFFFFC);
        td = (uint32_t)HCCA.UHP_HccaDoneHead & ~1;
        /* UHP_HccaDoneHead can be updated */
        usbIDOHCI->UHP_OHCI_HCINTERRUPTSTATUS = UHP_OHCI_HCINTERRUPTSTATUS_WDH_Msk;
        old_it = 0xFF;
        /* Process done queue (reversed order) */
        while (td)
        {
            done_td = (OHCIQueueTDDescriptor *)(td);
            for(i=0; i<DRV_USB_UHP_PIPES_NUMBER-1; i++)
            {
                if( td < (uint32_t)&OHCI_QueueTD[i+1])
                {
                    if( old_it != i )
                    {
                        old_it = i;
                        gDrvUSBHostPipeObj[i].intXfrQtdComplete = 1;
                        if( _DRV_USB_UHP_OHCI_HOST_TestTD() == 4)   /* Todo: Test LSB of UHP_HccaDoneHead */
                        {
                            /* Stall detected */
                            gDrvUSBHostPipeObj[i].intXfrQtdComplete = 0xFF;
                        }
                        DRV_USB_UHP_TransferProcess(hDriver, i);
                    }
                    break;
                }
            }
            /* Advance pointer to next TD. */
            td = done_td->NextTD;
        }

        /*
         * Done Queue processing complete, notify controller
         */
        HCCA.UHP_HccaDoneHead = 0;
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
                *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_CSC_Msk;
                hDriver->portNumber = i;

                /* CurrentConnectStatus */
                if( (read_data & UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk) == UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk)
                {
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : Device connected: %d", (int)i);
                    /* New connection */
                    hDriver->deviceAttached = true;
                    /* SetPortEnable */
                    *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_PES_Msk;
                    usbIDOHCI->UHP_OHCI_HCCONTROL = (usbIDOHCI->UHP_OHCI_HCCONTROL & ~UHP_OHCI_HCCONTROL_HCFS_Msk) | UHP_OHCI_HCCONTROL_HCFS( DRV_USB_UHP_HOST_OHCI_STATE_USBOPERATIONAL );
                }
                else
                {
                    /* Disconnect */
                    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : Device Disconnected: %d", (int)i);
                    /* ClearPortEnable */
                    *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0) + hDriver->portNumber) = UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk;
                    *((uint32_t *)&(usbIDOHCI->UHP_OHCI_HCRHPORTSTATUS0)) = UHP_OHCI_HCRHPORTSTATUS0_CCS_Msk;
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
                    for(j=0; j<DRV_USB_UHP_PIPES_NUMBER; j++)
                    {
                        gDrvUSBHostPipeObj[j].staticDToggleIn = 0;  /* data toggle */
                        gDrvUSBHostPipeObj[j].staticDToggleOut = 0;  /* data toggle */
                    }

                    hDriver->portNumber = 0;
                }
            }
            /* PortEnableStatusChange (bit 17) */
            if ( (read_data & UHP_OHCI_HCRHPORTSTATUS0_PESC_Msk) == UHP_OHCI_HCRHPORTSTATUS0_PESC_Msk )
            {
                /* change in PortEnableStatus */
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : PortEnableStatusChange");
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
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : PortSuspendStatusChange");
            }
            /* PortOwnerCurrentindicatorChange (bit 19) */
            if ( (read_data & UHP_OHCI_HCRHPORTSTATUS0_OCIC_Msk) == UHP_OHCI_HCRHPORTSTATUS0_OCIC_Msk )
            {
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : PortOwnerCurrentindicatorChange");
            }
            /* PortResetStatusChange (bit 20) */
            if ( (read_data & UHP_OHCI_HCRHPORTSTATUS0_PRSC_Msk) == UHP_OHCI_HCRHPORTSTATUS0_PRSC_Msk )
            {
                /* This bit is set at the end of the 10-ms port reset signal.
                 * port reset is complete */
                SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : PortResetStatusChanger");
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
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : Ownership Change");
    }

    /* SOF */
    if (ContextInfo & UHP_OHCI_HCINTERRUPTSTATUS_SF_Msk)
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\rOHCI IRQ : SOF");
        usbIDOHCI->UHP_0HCI_HCINTERRUPTENABLE = UHP_OHCI_HCINTERRUPTSTATUS_SF_Msk; /* clear interrupt */
    }

    /*
     * Any interrupts left should just be masked out. (This is normal processing for StartOfFrame and
     * RootHubStatusChange)
     */
    if (ContextInfo & ~UHP_OHCI_HCINTERRUPTDISABLE_MIE_Msk) /* any unprocessed interrupts? */
    {
        usbIDOHCI->UHP_OHCI_HCINTERRUPTDISABLE = ContextInfo; /* yes, mask them */
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

}/* end of DRV_USB_UHP_OHCI_HOST_Tasks_ISR() */


/* ***************************************************************************** */
/* ***************************************************************************** */
/* Root Hub Driver Routines */
/* ***************************************************************************** */
/* ***************************************************************************** */

/* **************************************************************************** */
/* Function:
    void DRV_USB_UHP_OHCI_HOST_ROOT_HUB_OperationEnable(DRV_HANDLE handle, bool enable)

   Summary:
    Root hub enable

   Description:


   Remarks:
    Refer to .h for usage information.
 */
void DRV_USB_UHP_OHCI_HOST_ROOT_HUB_OperationEnable(DRV_HANDLE handle, bool enable)
{
    DRV_USB_UHP_OBJ *pUSBDrvObj = (DRV_USB_UHP_OBJ *)handle;
    volatile UhpOhci *usbIDOHCI = pUSBDrvObj->usbIDOHCI;


    /* Check if the handle is valid or opened */
    if((handle == DRV_HANDLE_INVALID) || (!(pUSBDrvObj->isOpened)))
    {
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\r\nDRV USB_UHP: Bad Client or client closed");
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
            usbIDOHCI->UHP_0HCI_HCINTERRUPTENABLE = UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_RD_Msk   | /* ResumeDetected */
                                                    UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_RHSC_Msk | /* RootHubStatusChange */
                                                    UHP_OHCI_UHP_0HCI_HCINTERRUPTENABLE_MIE_Msk;   /* MasterInterruptEnable */

            /* Enable VBUS */
            if(pUSBDrvObj->rootHubInfo.portPowerEnable != NULL)
            {
                /* This USB module has only one port. So we call this function
                 * once to enable the port power on port 0 */
                pUSBDrvObj->rootHubInfo.portPowerEnable(0 /* Port 0 */, true);
            }
        }
    }
} /* end of DRV_USB_UHP_OHCI_HOST_ROOT_HUB_OperationEnable() */

