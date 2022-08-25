/*******************************************************************************
  USBHS Peripheral Library Register Defintions 

  File Name:
    usbhs_registers.h

  Summary:
    USBHS PLIB Register Defintions

  Description:
    This file contains the constants and defintions which are required by the
    the USBHS library.
*******************************************************************************/

#ifndef __USBHS_REGISTERS_H__
#define __USBHS_REGISTERS_H__

#include "device.h"
#include <stdint.h>



/*****************************************
 * Module Register Offsets.
 *****************************************/

#define USBHS_REG_FADDR         0x000
#define USBHS_REG_POWER         0x001
#define USBHS_REG_INTRTX        0x002
#define USBHS_REG_INTRRX        0x004
#define USBHS_REG_INTRTXE       0x006
#define USBHS_REG_INTRRXE       0x008
#define USBHS_REG_INTRUSB       0x00A 
#define USBHS_REG_INTRUSBE      0x00B 
#define USBHS_REG_FRAME         0x00C
#define USBHS_REG_INDEX         0x00E
#define USBHS_REG_TESTMODE      0x00F

/*******************************************************
 * Endpoint Control Status Registers (CSR). These values 
 * should be added to either the 0x10 to access the
 * register through Indexed CSR. To access the actual 
 * CSR, see ahead in this header file.
 ******************************************************/

#define USBHS_REG_EP_TXMAXP     0x000
#define USBHS_REG_EP_CSR0L      0x002
#define USBHS_REG_EP_CSR0H      0x003
#define USBHS_REG_EP_TXCSRL     0x002
#define USBHS_REG_EP_TXCSRH     0x003
#define USBHS_REG_EP_RXMAXP     0x004
#define USBHS_REG_EP_RXCSRL     0x006
#define USBHS_REG_EP_RXCSRH     0x007
#define USBHS_REG_EP_COUNT0     0x008
#define USBHS_REG_EP_RXCOUNT    0x008
#define USBHS_REG_EP_TYPE0      0x01A
#define USBHS_REG_EP_TXTYPE     0x01A
#define USBHS_REG_EP_NAKLIMIT0  0x01B
#define USBHS_REG_EP_TXINTERVAL 0x01B
#define USBHS_REG_EP_RXTYPE     0x01C
#define USBHS_REG_EP_RXINTERVAL 0x01D
#define USBHS_REG_EP_CONFIGDATA 0x01F
#define USBHS_REG_EP_FIFOSIZE   0x01F

#define USBHS_HOST_EP0_SETUPKT_SET 0x8
#define USBHS_HOST_EP0_TXPKTRDY_SET 0x2
#define USBHS_SOFT_RST_NRST_SET 0x1
#define USBHS_SOFT_RST_NRSTX_SET 0x2
#define USBHS_EP0_DEVICE_SERVICED_RXPKTRDY 0x40
#define USBHS_EP0_DEVICE_DATAEND 0x08
#define USBHS_EP0_DEVICE_TXPKTRDY 0x02
#define USBHS_EP0_HOST_STATUS_STAGE_START 0x40
#define USBHS_EP0_HOST_REQPKT 0x20
#define USBHS_EP0_HOST_TXPKTRDY 0x02
#define USBHS_EP0_HOST_RXPKTRDY 0x01
#define USBHS_EP_DEVICE_TX_SENT_STALL 0x20
#define USBHS_EP_DEVICE_TX_SEND_STALL 0x10
#define USBHS_EP_DEVICE_RX_SENT_STALL 0x40
#define USBHS_EP_DEVICE_RX_SEND_STALL 0x20

/* FADDR - Device Function Address */
typedef union 
{
    struct __attribute__((packed)) 
    {
        uint8_t FUNC:7;
        uint8_t :1;
    };

    uint8_t w;  

} __USBHS_FADDR_t;

/* POWER - Control Resume and Suspend signalling */
typedef union 
{
    struct __attribute__((packed))
    {
        uint8_t SUSPEN:1;
        uint8_t SUSPMODE:1;
        uint8_t RESUME:1;
        uint8_t RESET:1;
        uint8_t HSMODE:1;
        uint8_t HSEN:1;
        uint8_t SOFTCONN:1;
        uint8_t ISOUPD:1;
    };
    struct
    {   
        uint8_t w;
    };

} __USBHS_POWER_t;

/* INTRTXE - Transmit endpoint interrupt enable */
typedef union 
{
    struct __attribute__((packed))
    {
        uint8_t EP0IE:1;
        uint8_t EP1TXIE:1;
        uint8_t EP2TXIE:1;
        uint8_t EP3TXIE:1;
        uint8_t EP4TXIE:1;
        uint8_t EP5TXIE:1;
        uint8_t EP6TXIE:1;
        uint8_t EP7TXIE:1;
        uint8_t :8;
    };
    struct
    {
        uint16_t    w;
    };

} __USBHS_INTRTXE_t;

/* INTRRXE - Receive endpoint interrupt enable */
typedef union 
{
    struct __attribute__((packed))
    {
        uint8_t :1;
        uint8_t EP1RXIE:1;
        uint8_t EP2RXIE:1;
        uint8_t EP3RXIE:1;
        uint8_t EP4RXIE:1;
        uint8_t EP5RXIE:1;
        uint8_t EP6RXIE:1;
        uint8_t EP7RXIE:1;
 	uint8_t :8;
    };
    struct
    {
        uint16_t    w;
    };

} __USBHS_INTRRXE_t;

/* INTRUSBE - General USB Interrupt enable */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t SUSPIE:1;
        uint8_t RESUMEIE:1;
        uint8_t RESETIE:1;
        uint8_t SOFIE:1;
        uint8_t CONNIE:1;
        uint8_t DISCONIE:1;
        uint8_t SESSRQIE:1;
        uint8_t VBUSERRIE:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_INTRUSBE_t;

/* FRAME - Frame number */
typedef union 
{
    struct __attribute__((packed))
    {
        uint16_t RFRMNUM:11;
        uint8_t :5;
    };
    struct
    {
        uint16_t w;
    };

} __USBHS_FRAME_t;

/* INDEX - Endpoint index */
typedef union 
{
    struct __attribute__((packed))
    {
        uint8_t ENDPOINT:4;
        uint8_t :4;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_INDEX_t;

/* TESTMODE - Test mode register */
typedef union 
{
    struct __attribute__((packed))
    {
        uint8_t NAK:1;
        uint8_t TESTJ:1;
        uint8_t TESTK:1;
        uint8_t PACKET:1;
        uint8_t FORCEHS:1;
        uint8_t FORCEFS:1;
        uint8_t FIFOACC:1;
        uint8_t FORCEHST:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_TESTMODE_t;

/* COUNT0 - Indicates the amount of data received in endpoint 0 */ 
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t RXCNT:7;
        uint8_t :1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_COUNT0_t;

/* TYPE0 - Operating speed of target device */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t :6;
        uint8_t SPEED:2;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_TYPE0_t;

/* DEVCTL - Module control register */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t SESSION:1;
        uint8_t HOSTREQ:1;
        uint8_t HOSTMODE:1;
        uint8_t VBUS:2;
        uint8_t LSDEV:1;
        uint8_t FSDEV:1;
        uint8_t BDEV:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_DEVCTL_t;

/* CSR0L Device - Endpoint Device Mode Control Status Register */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t RXPKTRDY:1;
        uint8_t TXPKTRDY:1;
        uint8_t SENTSTALL:1;
        uint8_t DATAEND:1;
        uint8_t SETUPEND:1;
        uint8_t SENDSTALL:1;
        uint8_t SVCRPR:1;
        uint8_t SVSSETEND:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_CSR0L_DEVICE_t;

/* CSR0L Host - Endpoint Host Mode Control Status Register */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t RXPKTRDY:1;
        uint8_t TXPKTRDY:1;
        uint8_t RXSTALL:1;
        uint8_t SETUPPKT:1;
        uint8_t ERROR:1;
        uint8_t REQPKT:1;
        uint8_t STATPKT:1;
        uint8_t NAKTMOUT:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_CSR0L_HOST_t;

/* TXCSRL Device - Endpoint Transmit Control Status Register Low */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t TXPKTRDY:1;
        uint8_t FIFOONE:1;
        uint8_t UNDERRUN:1;
        uint8_t FLUSH:1;
        uint8_t SENDSTALL:1;
        uint8_t SENTSTALL:1;
        uint8_t CLRDT:1;
        uint8_t INCOMPTX:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_TXCSRL_DEVICE_t;

/* TXCSRL Host - Endpoint Transmit Control Status Register Low */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t TXPKTRDY:1;
        uint8_t FIFONE:1;
        uint8_t ERROR:1;
        uint8_t FLUSH:1;
        uint8_t SETUPPKT:1;
        uint8_t RXSTALL:1;
        uint8_t CLRDT:1;
        uint8_t INCOMPTX:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_TXCSRL_HOST_t;

/* TXCSRH Device - Endpoint Transmit Control Status Register High */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t :2;
        uint8_t DMAREQMD:1;
        uint8_t FRCDATTG:1;
        uint8_t DMAREQENL:1;
        uint8_t MODE:1;
        uint8_t ISO:1;
        uint8_t AUTOSET:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_TXCSRH_DEVICE_t;

/* TXCSRH Host - Endpoint Transmit Control Status Register High */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t DATATGGL:1;
        uint8_t DTWREN:1;
        uint8_t DMAREQMD:1;
        uint8_t FRCDATTG:1;
        uint8_t DMAREQEN:1;
        uint8_t MODE:1;
        uint8_t :1;
        uint8_t AUOTSET:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_TXCSRH_HOST_t;

/* CSR0H Device - Endpoint 0 Control Status Register High */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t FLSHFIFO:1;
        uint8_t :7;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_CSR0H_DEVICE_t;

/* CSR0H Host - Endpoint 0 Control Status Register High */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t FLSHFIFO:1;
        uint8_t DATATGGL:1;
        uint8_t DTWREN:1;
        uint8_t DISPING:1;
        uint8_t :4;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_CSR0H_HOST_t;

/* RXMAXP - Receive Endpoint Max packet size. */
typedef union
{
    struct __attribute__((packed))
    {
        uint16_t RXMAXP:11;
        uint8_t MULT:5;
    };
    struct
    {
        uint16_t w;
    };

} __USBHS_RXMAXP_t;

/* RXCSRL Device - Receive endpoint Control Status Register */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t RXPKTRDY:1;
        uint8_t FIFOFULL:1;
        uint8_t OVERRUN:1;
        uint8_t DATAERR:1;
        uint8_t FLUSH:1;
        uint8_t SENDSTALL:1;
        uint8_t SENTSTALL:1;
        uint8_t CLRDT:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_RXCSRL_DEVICE_t;

/* RXCSRL Host - Receive endpoint Control Status Register */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t RXPKTRDY:1;
        uint8_t FIFOFULL:1;
        uint8_t ERROR:1;
        uint8_t DERRNAKT:1;
        uint8_t FLUSH:1;
        uint8_t REQPKT:1;
        uint8_t RXSTALL:1;
        uint8_t CLRDT:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_RXCSRL_HOST_t;

/* RXCSRH Device - Receive endpoint Control Status Register */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t INCOMPRX:1;
        uint8_t :2;
        uint8_t DMAREQMODE:1;
        uint8_t DISNYET:1;
        uint8_t DMAREQEN:1;
        uint8_t ISO:1;
        uint8_t AUTOCLR:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_RXCSRH_DEVICE_t;

/* RXCSRH Host - Receive endpoint Control Status Register */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t INCOMPRX:1;
        uint8_t DATATGGL:1;
        uint8_t DATATWEN:1;
        uint8_t DMAREQMD:1;
        uint8_t PIDERR:1;
        uint8_t DMAREQEN:1;
        uint8_t AUTORQ:1;
        uint8_t AUOTCLR:1;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_RXCSRH_HOST_t;

/* RXCOUNT - Amount of data pending in RX FIFO */
typedef union
{
    struct __attribute__((packed))
    {
        uint16_t RXCNT:14;
        uint8_t :2;
    };
    struct
    {
        uint16_t w;
    };

} __USBHS_RXCOUNT_t;

/* TXTYPE - Specifies the target transmit endpoint */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t TEP:4;
        uint8_t PROTOCOL:2;
        uint8_t SPEED:2;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_TXTYPE_t;

/* RXTYPE - Specifies the target receive endpoint */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t TEP:4;
        uint8_t PROTOCOL:2;
        uint8_t SPEED:2;
    };
    struct
    {
        uint8_t w;
    };

} __USBHS_RXTYPE_t;

/* TXINTERVAL - Defines the polling interval */
typedef struct
{
    uint8_t TXINTERV;

} __USBHS_TXINTERVAL_t;

/* RXINTERVAL - Defines the polling interval */
typedef struct
{
    uint8_t RXINTERV;

} __USBHS_RXINTERVAL_t;

/* TXMAXP - Maximum amount of data that can be transferred through a TX endpoint
 * */

typedef union
{
    struct __attribute__((packed))
    {
        uint16_t TXMAXP:11;
        uint8_t MULT:5;
    };
    uint16_t w;

} __USBHS_TXMAXP_t;  

/* TXFIFOSZ - Size of the transmit endpoint FIFO */
typedef struct __attribute__((packed))
{
    uint8_t TXFIFOSZ:4;
    uint8_t TXDPB:1;
    uint8_t :3;

} __USBHS_TXFIFOSZ_t;

/* RXFIFOSZ - Size of the receive endpoint FIFO */
typedef struct __attribute__((packed))
{
    uint8_t RXFIFOSZ:4;
    uint8_t RXDPB:1;
    uint8_t :3;

} __USBHS_RXFIFOSZ_t;

/* TXFIFOADD - Start address of the transmit endpoint FIFO */
typedef union
{
    struct __attribute__((packed))
    {
        uint16_t TXFIFOAD:13;
        uint8_t :3;
    };
    uint16_t w;

} __USBHS_TXFIFOADD_t;

/* RXFIFOADD - Start address of the receive endpoint FIFO */
typedef union
{
    struct __attribute__((packed))
    {
        uint16_t RXFIFOAD:13;
        uint8_t :3;
    };
    uint16_t w;

} __USBHS_RXFIFOADD_t;

/* SOFTRST - Asserts NRSTO and NRSTOX */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t NRST:1;
        uint8_t NRSTX:1;
        uint8_t :6;
    };
    uint8_t w;

} __USBHS_SOFTRST_t;

/* TXFUNCADDR - Target address of transmit endpoint */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t TXFADDR:7;
        uint8_t :1;
    };
    uint8_t w;

} __USBHS_TXFUNCADDR_t;

/* RXFUNCADDR - Target address of receive endpoint */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t RXFADDR:7;
        uint8_t :1;
    };
    uint8_t w;

} __USBHS_RXFUNCADDR_t;

/* TXHUBADDR - Address of the hub to which the target transmit device endpoint
 * is connected */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t TXHUBADDR:7;
        uint8_t MULTTRAN:1;
    };
    uint8_t w;

} __USBHS_TXHUBADDR_t;

/* RXHUBADDR - Address of the hub to which the target receive device endpoint is
 * connected */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t RXHUBADDR:7;
        uint8_t MULTTRAN:1;
    };
    uint8_t w;

} __USBHS_RXHUBADDR_t;

/* TXHUBPORT - Address of the hub to which the target transmit device endpoint
 * is connected. */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t TXHUBPRT:7;
        uint8_t :1;
    };

    uint8_t w;

} __USBHS_TXHUBPORT_t;

/* RXHUBPORT - Address of the hub to which the target receive device endpoint
 * is connected. */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t RXHUBPRT:7;
        uint8_t :1;
    };

    uint8_t w;

} __USBHS_RXHUBPORT_t;

/* DMACONTROL - Configures a DMA channel */
typedef union
{
    struct __attribute__((packed))
    {
        uint8_t DMAEN:1;
        uint8_t DMADIR:1;
        uint8_t DMAMODE:1;
        uint8_t DMAIE:1;
        uint8_t DMAEP:4;
        uint8_t DMAERR:1;
        uint8_t DMABRSTM:2;
        uint32_t:21;
    };

    uint32_t w;

} __USBHS_DMACNTL_t;

/* Endpoint Control and Status Register Set */    
typedef struct __attribute__((packed))
{
    volatile __USBHS_TXMAXP_t TXMAXPbits;
    union
    {
        struct
        {
            union
            {
                volatile __USBHS_CSR0L_DEVICE_t CSR0L_DEVICEbits;
                volatile __USBHS_CSR0L_HOST_t CSR0L_HOSTbits;
            };
            union
            {
                volatile __USBHS_CSR0H_DEVICE_t CSR0H_DEVICEbits;
                volatile __USBHS_CSR0H_HOST_t CSR0H_HOSTbits;
            };
        };

        struct
        {
            union
            {
                volatile __USBHS_TXCSRL_DEVICE_t TXCSRL_DEVICEbits;
                volatile __USBHS_TXCSRL_HOST_t TXCSRL_HOSTbits;
            };

            union
            {
                volatile __USBHS_TXCSRH_DEVICE_t TXCSRH_DEVICEbits;
                volatile __USBHS_TXCSRH_HOST_t TXCSRH_HOSTbits;
            };
        };
    };

    volatile __USBHS_RXMAXP_t RXMAXPbits;

    union
    {
        volatile __USBHS_RXCSRL_DEVICE_t RXCSRL_DEVICEbits;
        volatile __USBHS_RXCSRL_HOST_t RXCSRL_HOSTbits;
    };

    union
    {
        volatile __USBHS_RXCSRH_DEVICE_t RXCSRH_DEVICEbits;
        volatile __USBHS_RXCSRH_HOST_t RXCSRH_HOSTbits;
    };

    union
    {
        volatile __USBHS_COUNT0_t COUNT0bits;
        volatile __USBHS_RXCOUNT_t RXCOUNTbits;
    };

    union
    {
        volatile __USBHS_TYPE0_t TYPE0bits;
        volatile __USBHS_TXTYPE_t TXTYPEbits;
    };

    union
    {
        volatile uint8_t NAKLIMIT0;
        volatile __USBHS_TXINTERVAL_t TXINTERVALbits;
    };

    volatile __USBHS_RXTYPE_t RXTYPEbits;
    volatile __USBHS_RXINTERVAL_t RXINTERVALbits;
    uint8_t :8;
    union
    {
        volatile uint8_t CONFIGDATA;
        volatile uint8_t FIFOSIZE;
    };

} __USBHS_EPCSR_t;

/* Set of registers that configure the multi-point option */
typedef struct __attribute__((packed))
{
    volatile __USBHS_TXFUNCADDR_t TXFUNCADDRbits;
    uint8_t :8;
    volatile __USBHS_TXHUBADDR_t TXHUBADDRbits;
    volatile __USBHS_TXHUBPORT_t TXHUBPORTbits;
    volatile __USBHS_RXFUNCADDR_t RXFUNCADDRbits;
    uint8_t :8;
    volatile __USBHS_RXHUBADDR_t RXHUBADDRbits;
    volatile __USBHS_RXHUBPORT_t RXHUBPORTbits;

} __USBHS_TARGET_ADDR_t;

/* Set of registers that configure the DMA channel */
typedef struct __attribute__((packed))
{
    volatile __USBHS_DMACNTL_t DMACNTLbits;
    volatile uint32_t DMAADDR;
    volatile uint32_t DMACOUNT;
    volatile uint32_t pad;
} __USBHS_DMA_CHANNEL_t;

/* USBHS module register set */
typedef struct __attribute__((aligned(4),packed))
{
    volatile __USBHS_FADDR_t    FADDRbits;
    volatile __USBHS_POWER_t    POWERbits;
    volatile uint16_t           INTRTX;
    volatile uint16_t           INTRRX;
    volatile __USBHS_INTRTXE_t  INTRTXEbits;
    volatile __USBHS_INTRRXE_t  INTRRXEbits;
    volatile uint8_t            INTRUSB;
    volatile __USBHS_INTRUSBE_t INTRUSBEbits;
    volatile __USBHS_FRAME_t    FRAMEbits;
    volatile __USBHS_INDEX_t    INDEXbits;
    volatile __USBHS_TESTMODE_t TESTMODEbits;
    volatile __USBHS_EPCSR_t    INDEXED_EPCSR;
    volatile uint32_t           FIFO[8];
    volatile uint32_t           reserved0[8];
    volatile __USBHS_DEVCTL_t   DEVCTLbits;
    volatile uint8_t            MISC;
    volatile __USBHS_TXFIFOSZ_t TXFIFOSZbits;
    volatile __USBHS_RXFIFOSZ_t RXFIFOSZbits;

    volatile __USBHS_TXFIFOADD_t   TXFIFOADDbits;
    volatile __USBHS_RXFIFOADD_t   RXFIFOADDbits;
    
    volatile uint32_t   VCONTROL;
    volatile uint16_t   HWVERS;
    volatile uint8_t    padding1[10];
    volatile uint8_t    EPINFO;
    volatile uint8_t    RAMINFO;
    volatile uint8_t    LINKINFO;
    volatile uint8_t    VPLEN;
    volatile uint8_t    HS_EOF1;
    volatile uint8_t    FS_EOF1;
    volatile uint8_t    LS_EOF1;

    volatile __USBHS_SOFTRST_t    SOFTRSTbits;

    volatile __USBHS_TARGET_ADDR_t  TADDR[8];
    volatile __USBHS_TARGET_ADDR_t  TADDR_reserved[8];

    volatile __USBHS_EPCSR_t        EPCSR[8];
    volatile __USBHS_EPCSR_t        EPCSR_reserved[8];

    volatile uint32_t               DMA_INTR;

    volatile __USBHS_DMA_CHANNEL_t  DMA_CHANNEL[8]; 
    volatile uint32_t               RQPKTXOUNT[7];
    volatile uint32_t               RQPKTXOUNT_reserved[9];

} usbhs_registers_sw_t;

#endif
