/**
 * \brief Component description for UHP_EHCI
 *
 * Copyright (c) 2019 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software and any derivatives
 * exclusively with Microchip products. It is your responsibility to comply with third party license
 * terms applicable to your use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
 * MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT
 * EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/* file generated from device description version 2019-01-10T17:30:45Z */
#ifndef _DA78_UHP_EHCI_COMPONENT_H_
#define _DA78_UHP_EHCI_COMPONENT_H_
#define _DA78_UHP_EHCI_COMPONENT_         /**< \deprecated  Backward compatibility for ASF */

/** \addtogroup PIC32CZ_DA78 USB Host High Speed Port
 *  @{
 */
/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR UHP_EHCI */
/* ========================================================================== */
#ifndef COMPONENT_TYPEDEF_STYLE
  #define COMPONENT_TYPEDEF_STYLE 'R'  /**< Defines default style of typedefs for the component header files ('R' = RFO, 'N' = NTO)*/
#endif

#define UHP_EHCI_6354                       /**< (UHP_EHCI) Module ID */
#define REV_UHP_EHCI R                      /**< (UHP_EHCI) Module revision */

/* -------- UHP_EHCI_HCCAPBASE : (UHP_EHCI Offset: 0x00) (R/ 32) UHP_EHCI Host Controller Capability Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t CAPLENGTH:8;               /**< bit:   0..7  Capability Registers Length              */
    uint32_t :8;                        /**< bit:  8..15  Reserved */
    uint32_t HCIVERSION:16;             /**< bit: 16..31  Host Controller Interface Version Number */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_HCCAPBASE_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_HCCAPBASE_OFFSET           (0x00)                                        /**<  (UHP_EHCI_HCCAPBASE) UHP_EHCI Host Controller Capability Register  Offset */

#define UHP_EHCI_HCCAPBASE_CAPLENGTH_Pos    0                                              /**< (UHP_EHCI_HCCAPBASE) Capability Registers Length Position */
#define UHP_EHCI_HCCAPBASE_CAPLENGTH_Msk    (_U_(0xFF) << UHP_EHCI_HCCAPBASE_CAPLENGTH_Pos)  /**< (UHP_EHCI_HCCAPBASE) Capability Registers Length Mask */
#define UHP_EHCI_HCCAPBASE_CAPLENGTH(value) (UHP_EHCI_HCCAPBASE_CAPLENGTH_Msk & ((value) << UHP_EHCI_HCCAPBASE_CAPLENGTH_Pos))
#define UHP_EHCI_HCCAPBASE_HCIVERSION_Pos   16                                             /**< (UHP_EHCI_HCCAPBASE) Host Controller Interface Version Number Position */
#define UHP_EHCI_HCCAPBASE_HCIVERSION_Msk   (_U_(0xFFFF) << UHP_EHCI_HCCAPBASE_HCIVERSION_Pos)  /**< (UHP_EHCI_HCCAPBASE) Host Controller Interface Version Number Mask */
#define UHP_EHCI_HCCAPBASE_HCIVERSION(value) (UHP_EHCI_HCCAPBASE_HCIVERSION_Msk & ((value) << UHP_EHCI_HCCAPBASE_HCIVERSION_Pos))
#define UHP_EHCI_HCCAPBASE_MASK             _U_(0xFFFF00FF)                                /**< \deprecated (UHP_EHCI_HCCAPBASE) Register MASK  (Use UHP_EHCI_HCCAPBASE_Msk instead)  */
#define UHP_EHCI_HCCAPBASE_Msk              _U_(0xFFFF00FF)                                /**< (UHP_EHCI_HCCAPBASE) Register Mask  */


/* -------- UHP_EHCI_HCSPARAMS : (UHP_EHCI Offset: 0x04) (R/ 32) UHP_EHCI Host Controller Structural Parameters Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t N_PORTS:4;                 /**< bit:   0..3  Number of Ports                          */
    uint32_t PPC:1;                     /**< bit:      4  Port Power Control                       */
    uint32_t :3;                        /**< bit:   5..7  Reserved */
    uint32_t N_PCC:4;                   /**< bit:  8..11  Number of Ports per Companion Controller */
    uint32_t N_CC:4;                    /**< bit: 12..15  Number of Companion Controllers          */
    uint32_t P_INDICATOR:1;             /**< bit:     16  Port Indicators                          */
    uint32_t :3;                        /**< bit: 17..19  Reserved */
    uint32_t N_DP:4;                    /**< bit: 20..23  Debug Port Number                        */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_HCSPARAMS_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_HCSPARAMS_OFFSET           (0x04)                                        /**<  (UHP_EHCI_HCSPARAMS) UHP_EHCI Host Controller Structural Parameters Register  Offset */

#define UHP_EHCI_HCSPARAMS_N_PORTS_Pos      0                                              /**< (UHP_EHCI_HCSPARAMS) Number of Ports Position */
#define UHP_EHCI_HCSPARAMS_N_PORTS_Msk      (_U_(0xF) << UHP_EHCI_HCSPARAMS_N_PORTS_Pos)   /**< (UHP_EHCI_HCSPARAMS) Number of Ports Mask */
#define UHP_EHCI_HCSPARAMS_N_PORTS(value)   (UHP_EHCI_HCSPARAMS_N_PORTS_Msk & ((value) << UHP_EHCI_HCSPARAMS_N_PORTS_Pos))
#define UHP_EHCI_HCSPARAMS_PPC_Pos          4                                              /**< (UHP_EHCI_HCSPARAMS) Port Power Control Position */
#define UHP_EHCI_HCSPARAMS_PPC_Msk          (_U_(0x1) << UHP_EHCI_HCSPARAMS_PPC_Pos)       /**< (UHP_EHCI_HCSPARAMS) Port Power Control Mask */
#define UHP_EHCI_HCSPARAMS_PPC              UHP_EHCI_HCSPARAMS_PPC_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_HCSPARAMS_PPC_Msk instead */
#define UHP_EHCI_HCSPARAMS_N_PCC_Pos        8                                              /**< (UHP_EHCI_HCSPARAMS) Number of Ports per Companion Controller Position */
#define UHP_EHCI_HCSPARAMS_N_PCC_Msk        (_U_(0xF) << UHP_EHCI_HCSPARAMS_N_PCC_Pos)     /**< (UHP_EHCI_HCSPARAMS) Number of Ports per Companion Controller Mask */
#define UHP_EHCI_HCSPARAMS_N_PCC(value)     (UHP_EHCI_HCSPARAMS_N_PCC_Msk & ((value) << UHP_EHCI_HCSPARAMS_N_PCC_Pos))
#define UHP_EHCI_HCSPARAMS_N_CC_Pos         12                                             /**< (UHP_EHCI_HCSPARAMS) Number of Companion Controllers Position */
#define UHP_EHCI_HCSPARAMS_N_CC_Msk         (_U_(0xF) << UHP_EHCI_HCSPARAMS_N_CC_Pos)      /**< (UHP_EHCI_HCSPARAMS) Number of Companion Controllers Mask */
#define UHP_EHCI_HCSPARAMS_N_CC(value)      (UHP_EHCI_HCSPARAMS_N_CC_Msk & ((value) << UHP_EHCI_HCSPARAMS_N_CC_Pos))
#define UHP_EHCI_HCSPARAMS_P_INDICATOR_Pos  16                                             /**< (UHP_EHCI_HCSPARAMS) Port Indicators Position */
#define UHP_EHCI_HCSPARAMS_P_INDICATOR_Msk  (_U_(0x1) << UHP_EHCI_HCSPARAMS_P_INDICATOR_Pos)  /**< (UHP_EHCI_HCSPARAMS) Port Indicators Mask */
#define UHP_EHCI_HCSPARAMS_P_INDICATOR      UHP_EHCI_HCSPARAMS_P_INDICATOR_Msk             /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_HCSPARAMS_P_INDICATOR_Msk instead */
#define UHP_EHCI_HCSPARAMS_N_DP_Pos         20                                             /**< (UHP_EHCI_HCSPARAMS) Debug Port Number Position */
#define UHP_EHCI_HCSPARAMS_N_DP_Msk         (_U_(0xF) << UHP_EHCI_HCSPARAMS_N_DP_Pos)      /**< (UHP_EHCI_HCSPARAMS) Debug Port Number Mask */
#define UHP_EHCI_HCSPARAMS_N_DP(value)      (UHP_EHCI_HCSPARAMS_N_DP_Msk & ((value) << UHP_EHCI_HCSPARAMS_N_DP_Pos))
#define UHP_EHCI_HCSPARAMS_MASK             _U_(0xF1FF1F)                                  /**< \deprecated (UHP_EHCI_HCSPARAMS) Register MASK  (Use UHP_EHCI_HCSPARAMS_Msk instead)  */
#define UHP_EHCI_HCSPARAMS_Msk              _U_(0xF1FF1F)                                  /**< (UHP_EHCI_HCSPARAMS) Register Mask  */


/* -------- UHP_EHCI_HCCPARAMS : (UHP_EHCI Offset: 0x08) (R/ 32) UHP_EHCI Host Controller Capability Parameters Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t AC:1;                      /**< bit:      0  64-bit Addressing Capability             */
    uint32_t PFLF:1;                    /**< bit:      1  Programmable Frame List Flag             */
    uint32_t ASPC:1;                    /**< bit:      2  Asynchronous Schedule Park Capability    */
    uint32_t :1;                        /**< bit:      3  Reserved */
    uint32_t IST:4;                     /**< bit:   4..7  Isochronous Scheduling Threshold         */
    uint32_t EECP:8;                    /**< bit:  8..15  EHCI Extended Capabilities Pointer       */
    uint32_t :16;                       /**< bit: 16..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_HCCPARAMS_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_HCCPARAMS_OFFSET           (0x08)                                        /**<  (UHP_EHCI_HCCPARAMS) UHP_EHCI Host Controller Capability Parameters Register  Offset */

#define UHP_EHCI_HCCPARAMS_AC_Pos           0                                              /**< (UHP_EHCI_HCCPARAMS) 64-bit Addressing Capability Position */
#define UHP_EHCI_HCCPARAMS_AC_Msk           (_U_(0x1) << UHP_EHCI_HCCPARAMS_AC_Pos)        /**< (UHP_EHCI_HCCPARAMS) 64-bit Addressing Capability Mask */
#define UHP_EHCI_HCCPARAMS_AC               UHP_EHCI_HCCPARAMS_AC_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_HCCPARAMS_AC_Msk instead */
#define UHP_EHCI_HCCPARAMS_PFLF_Pos         1                                              /**< (UHP_EHCI_HCCPARAMS) Programmable Frame List Flag Position */
#define UHP_EHCI_HCCPARAMS_PFLF_Msk         (_U_(0x1) << UHP_EHCI_HCCPARAMS_PFLF_Pos)      /**< (UHP_EHCI_HCCPARAMS) Programmable Frame List Flag Mask */
#define UHP_EHCI_HCCPARAMS_PFLF             UHP_EHCI_HCCPARAMS_PFLF_Msk                    /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_HCCPARAMS_PFLF_Msk instead */
#define UHP_EHCI_HCCPARAMS_ASPC_Pos         2                                              /**< (UHP_EHCI_HCCPARAMS) Asynchronous Schedule Park Capability Position */
#define UHP_EHCI_HCCPARAMS_ASPC_Msk         (_U_(0x1) << UHP_EHCI_HCCPARAMS_ASPC_Pos)      /**< (UHP_EHCI_HCCPARAMS) Asynchronous Schedule Park Capability Mask */
#define UHP_EHCI_HCCPARAMS_ASPC             UHP_EHCI_HCCPARAMS_ASPC_Msk                    /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_HCCPARAMS_ASPC_Msk instead */
#define UHP_EHCI_HCCPARAMS_IST_Pos          4                                              /**< (UHP_EHCI_HCCPARAMS) Isochronous Scheduling Threshold Position */
#define UHP_EHCI_HCCPARAMS_IST_Msk          (_U_(0xF) << UHP_EHCI_HCCPARAMS_IST_Pos)       /**< (UHP_EHCI_HCCPARAMS) Isochronous Scheduling Threshold Mask */
#define UHP_EHCI_HCCPARAMS_IST(value)       (UHP_EHCI_HCCPARAMS_IST_Msk & ((value) << UHP_EHCI_HCCPARAMS_IST_Pos))
#define UHP_EHCI_HCCPARAMS_EECP_Pos         8                                              /**< (UHP_EHCI_HCCPARAMS) EHCI Extended Capabilities Pointer Position */
#define UHP_EHCI_HCCPARAMS_EECP_Msk         (_U_(0xFF) << UHP_EHCI_HCCPARAMS_EECP_Pos)     /**< (UHP_EHCI_HCCPARAMS) EHCI Extended Capabilities Pointer Mask */
#define UHP_EHCI_HCCPARAMS_EECP(value)      (UHP_EHCI_HCCPARAMS_EECP_Msk & ((value) << UHP_EHCI_HCCPARAMS_EECP_Pos))
#define UHP_EHCI_HCCPARAMS_MASK             _U_(0xFFF7)                                    /**< \deprecated (UHP_EHCI_HCCPARAMS) Register MASK  (Use UHP_EHCI_HCCPARAMS_Msk instead)  */
#define UHP_EHCI_HCCPARAMS_Msk              _U_(0xFFF7)                                    /**< (UHP_EHCI_HCCPARAMS) Register Mask  */


/* -------- UHP_EHCI_USBCMD : (UHP_EHCI Offset: 0x10) (R/W 32) UHP_EHCI USB Command Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t RS:1;                      /**< bit:      0  Run/Stop (read/write)                    */
    uint32_t HCRESET:1;                 /**< bit:      1  Host Controller Reset (read/write)       */
    uint32_t FLS:2;                     /**< bit:   2..3  Frame List Size (read/write or read-only) */
    uint32_t PSE:1;                     /**< bit:      4  Periodic Schedule Enable (read/write)    */
    uint32_t ASE:1;                     /**< bit:      5  Asynchronous Schedule Enable (read/write) */
    uint32_t IAAD:1;                    /**< bit:      6  Interrupt on Async Advance Doorbell (read/write) */
    uint32_t LHCR:1;                    /**< bit:      7  Light Host Controller Reset (optional) (read/write) */
    uint32_t ASPMC:2;                   /**< bit:   8..9  Asynchronous Schedule Park Mode Count (optional) (read/write or read-only) */
    uint32_t :1;                        /**< bit:     10  Reserved */
    uint32_t ASPME:1;                   /**< bit:     11  Asynchronous Schedule Park Mode Enable (optional) (read/write or read-only) */
    uint32_t :4;                        /**< bit: 12..15  Reserved */
    uint32_t ITC:8;                     /**< bit: 16..23  Interrupt Threshold Control (read/write) */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_USBCMD_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_USBCMD_OFFSET              (0x10)                                        /**<  (UHP_EHCI_USBCMD) UHP_EHCI USB Command Register  Offset */

#define UHP_EHCI_USBCMD_RS_Pos              0                                              /**< (UHP_EHCI_USBCMD) Run/Stop (read/write) Position */
#define UHP_EHCI_USBCMD_RS_Msk              (_U_(0x1) << UHP_EHCI_USBCMD_RS_Pos)           /**< (UHP_EHCI_USBCMD) Run/Stop (read/write) Mask */
#define UHP_EHCI_USBCMD_RS                  UHP_EHCI_USBCMD_RS_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBCMD_RS_Msk instead */
#define UHP_EHCI_USBCMD_HCRESET_Pos         1                                              /**< (UHP_EHCI_USBCMD) Host Controller Reset (read/write) Position */
#define UHP_EHCI_USBCMD_HCRESET_Msk         (_U_(0x1) << UHP_EHCI_USBCMD_HCRESET_Pos)      /**< (UHP_EHCI_USBCMD) Host Controller Reset (read/write) Mask */
#define UHP_EHCI_USBCMD_HCRESET             UHP_EHCI_USBCMD_HCRESET_Msk                    /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBCMD_HCRESET_Msk instead */
#define UHP_EHCI_USBCMD_FLS_Pos             2                                              /**< (UHP_EHCI_USBCMD) Frame List Size (read/write or read-only) Position */
#define UHP_EHCI_USBCMD_FLS_Msk             (_U_(0x3) << UHP_EHCI_USBCMD_FLS_Pos)          /**< (UHP_EHCI_USBCMD) Frame List Size (read/write or read-only) Mask */
#define UHP_EHCI_USBCMD_FLS(value)          (UHP_EHCI_USBCMD_FLS_Msk & ((value) << UHP_EHCI_USBCMD_FLS_Pos))
#define UHP_EHCI_USBCMD_PSE_Pos             4                                              /**< (UHP_EHCI_USBCMD) Periodic Schedule Enable (read/write) Position */
#define UHP_EHCI_USBCMD_PSE_Msk             (_U_(0x1) << UHP_EHCI_USBCMD_PSE_Pos)          /**< (UHP_EHCI_USBCMD) Periodic Schedule Enable (read/write) Mask */
#define UHP_EHCI_USBCMD_PSE                 UHP_EHCI_USBCMD_PSE_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBCMD_PSE_Msk instead */
#define UHP_EHCI_USBCMD_ASE_Pos             5                                              /**< (UHP_EHCI_USBCMD) Asynchronous Schedule Enable (read/write) Position */
#define UHP_EHCI_USBCMD_ASE_Msk             (_U_(0x1) << UHP_EHCI_USBCMD_ASE_Pos)          /**< (UHP_EHCI_USBCMD) Asynchronous Schedule Enable (read/write) Mask */
#define UHP_EHCI_USBCMD_ASE                 UHP_EHCI_USBCMD_ASE_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBCMD_ASE_Msk instead */
#define UHP_EHCI_USBCMD_IAAD_Pos            6                                              /**< (UHP_EHCI_USBCMD) Interrupt on Async Advance Doorbell (read/write) Position */
#define UHP_EHCI_USBCMD_IAAD_Msk            (_U_(0x1) << UHP_EHCI_USBCMD_IAAD_Pos)         /**< (UHP_EHCI_USBCMD) Interrupt on Async Advance Doorbell (read/write) Mask */
#define UHP_EHCI_USBCMD_IAAD                UHP_EHCI_USBCMD_IAAD_Msk                       /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBCMD_IAAD_Msk instead */
#define UHP_EHCI_USBCMD_LHCR_Pos            7                                              /**< (UHP_EHCI_USBCMD) Light Host Controller Reset (optional) (read/write) Position */
#define UHP_EHCI_USBCMD_LHCR_Msk            (_U_(0x1) << UHP_EHCI_USBCMD_LHCR_Pos)         /**< (UHP_EHCI_USBCMD) Light Host Controller Reset (optional) (read/write) Mask */
#define UHP_EHCI_USBCMD_LHCR                UHP_EHCI_USBCMD_LHCR_Msk                       /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBCMD_LHCR_Msk instead */
#define UHP_EHCI_USBCMD_ASPMC_Pos           8                                              /**< (UHP_EHCI_USBCMD) Asynchronous Schedule Park Mode Count (optional) (read/write or read-only) Position */
#define UHP_EHCI_USBCMD_ASPMC_Msk           (_U_(0x3) << UHP_EHCI_USBCMD_ASPMC_Pos)        /**< (UHP_EHCI_USBCMD) Asynchronous Schedule Park Mode Count (optional) (read/write or read-only) Mask */
#define UHP_EHCI_USBCMD_ASPMC(value)        (UHP_EHCI_USBCMD_ASPMC_Msk & ((value) << UHP_EHCI_USBCMD_ASPMC_Pos))
#define UHP_EHCI_USBCMD_ASPME_Pos           11                                             /**< (UHP_EHCI_USBCMD) Asynchronous Schedule Park Mode Enable (optional) (read/write or read-only) Position */
#define UHP_EHCI_USBCMD_ASPME_Msk           (_U_(0x1) << UHP_EHCI_USBCMD_ASPME_Pos)        /**< (UHP_EHCI_USBCMD) Asynchronous Schedule Park Mode Enable (optional) (read/write or read-only) Mask */
#define UHP_EHCI_USBCMD_ASPME               UHP_EHCI_USBCMD_ASPME_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBCMD_ASPME_Msk instead */
#define UHP_EHCI_USBCMD_ITC_Pos             16                                             /**< (UHP_EHCI_USBCMD) Interrupt Threshold Control (read/write) Position */
#define UHP_EHCI_USBCMD_ITC_Msk             (_U_(0xFF) << UHP_EHCI_USBCMD_ITC_Pos)         /**< (UHP_EHCI_USBCMD) Interrupt Threshold Control (read/write) Mask */
#define UHP_EHCI_USBCMD_ITC(value)          (UHP_EHCI_USBCMD_ITC_Msk & ((value) << UHP_EHCI_USBCMD_ITC_Pos))
#define UHP_EHCI_USBCMD_MASK                _U_(0xFF0BFF)                                  /**< \deprecated (UHP_EHCI_USBCMD) Register MASK  (Use UHP_EHCI_USBCMD_Msk instead)  */
#define UHP_EHCI_USBCMD_Msk                 _U_(0xFF0BFF)                                  /**< (UHP_EHCI_USBCMD) Register Mask  */


/* -------- UHP_EHCI_USBSTS : (UHP_EHCI Offset: 0x14) (R/W 32) UHP_EHCI USB Status Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t USBINT:1;                  /**< bit:      0  USB Interrupt (read/write clear)         */
    uint32_t USBERRINT:1;               /**< bit:      1  USB Error Interrupt (read/write clear)   */
    uint32_t PCD:1;                     /**< bit:      2  Port Change Detect (read/write clear)    */
    uint32_t FLR:1;                     /**< bit:      3  Frame List Rollover (read/write clear)   */
    uint32_t HSE:1;                     /**< bit:      4  Host System Error (read/write clear)     */
    uint32_t IAA:1;                     /**< bit:      5  Interrupt on Async Advance (read/write clear) */
    uint32_t :6;                        /**< bit:  6..11  Reserved */
    uint32_t HCHLT:1;                   /**< bit:     12  HCHalted (read-only)                     */
    uint32_t RCM:1;                     /**< bit:     13  Reclamation (read-only)                  */
    uint32_t PSS:1;                     /**< bit:     14  Periodic Schedule Status (read-only)     */
    uint32_t ASS:1;                     /**< bit:     15  Asynchronous Schedule Status (read-only) */
    uint32_t :16;                       /**< bit: 16..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_USBSTS_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_USBSTS_OFFSET              (0x14)                                        /**<  (UHP_EHCI_USBSTS) UHP_EHCI USB Status Register  Offset */

#define UHP_EHCI_USBSTS_USBINT_Pos          0                                              /**< (UHP_EHCI_USBSTS) USB Interrupt (read/write clear) Position */
#define UHP_EHCI_USBSTS_USBINT_Msk          (_U_(0x1) << UHP_EHCI_USBSTS_USBINT_Pos)       /**< (UHP_EHCI_USBSTS) USB Interrupt (read/write clear) Mask */
#define UHP_EHCI_USBSTS_USBINT              UHP_EHCI_USBSTS_USBINT_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_USBINT_Msk instead */
#define UHP_EHCI_USBSTS_USBERRINT_Pos       1                                              /**< (UHP_EHCI_USBSTS) USB Error Interrupt (read/write clear) Position */
#define UHP_EHCI_USBSTS_USBERRINT_Msk       (_U_(0x1) << UHP_EHCI_USBSTS_USBERRINT_Pos)    /**< (UHP_EHCI_USBSTS) USB Error Interrupt (read/write clear) Mask */
#define UHP_EHCI_USBSTS_USBERRINT           UHP_EHCI_USBSTS_USBERRINT_Msk                  /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_USBERRINT_Msk instead */
#define UHP_EHCI_USBSTS_PCD_Pos             2                                              /**< (UHP_EHCI_USBSTS) Port Change Detect (read/write clear) Position */
#define UHP_EHCI_USBSTS_PCD_Msk             (_U_(0x1) << UHP_EHCI_USBSTS_PCD_Pos)          /**< (UHP_EHCI_USBSTS) Port Change Detect (read/write clear) Mask */
#define UHP_EHCI_USBSTS_PCD                 UHP_EHCI_USBSTS_PCD_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_PCD_Msk instead */
#define UHP_EHCI_USBSTS_FLR_Pos             3                                              /**< (UHP_EHCI_USBSTS) Frame List Rollover (read/write clear) Position */
#define UHP_EHCI_USBSTS_FLR_Msk             (_U_(0x1) << UHP_EHCI_USBSTS_FLR_Pos)          /**< (UHP_EHCI_USBSTS) Frame List Rollover (read/write clear) Mask */
#define UHP_EHCI_USBSTS_FLR                 UHP_EHCI_USBSTS_FLR_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_FLR_Msk instead */
#define UHP_EHCI_USBSTS_HSE_Pos             4                                              /**< (UHP_EHCI_USBSTS) Host System Error (read/write clear) Position */
#define UHP_EHCI_USBSTS_HSE_Msk             (_U_(0x1) << UHP_EHCI_USBSTS_HSE_Pos)          /**< (UHP_EHCI_USBSTS) Host System Error (read/write clear) Mask */
#define UHP_EHCI_USBSTS_HSE                 UHP_EHCI_USBSTS_HSE_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_HSE_Msk instead */
#define UHP_EHCI_USBSTS_IAA_Pos             5                                              /**< (UHP_EHCI_USBSTS) Interrupt on Async Advance (read/write clear) Position */
#define UHP_EHCI_USBSTS_IAA_Msk             (_U_(0x1) << UHP_EHCI_USBSTS_IAA_Pos)          /**< (UHP_EHCI_USBSTS) Interrupt on Async Advance (read/write clear) Mask */
#define UHP_EHCI_USBSTS_IAA                 UHP_EHCI_USBSTS_IAA_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_IAA_Msk instead */
#define UHP_EHCI_USBSTS_HCHLT_Pos           12                                             /**< (UHP_EHCI_USBSTS) HCHalted (read-only) Position */
#define UHP_EHCI_USBSTS_HCHLT_Msk           (_U_(0x1) << UHP_EHCI_USBSTS_HCHLT_Pos)        /**< (UHP_EHCI_USBSTS) HCHalted (read-only) Mask */
#define UHP_EHCI_USBSTS_HCHLT               UHP_EHCI_USBSTS_HCHLT_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_HCHLT_Msk instead */
#define UHP_EHCI_USBSTS_RCM_Pos             13                                             /**< (UHP_EHCI_USBSTS) Reclamation (read-only) Position */
#define UHP_EHCI_USBSTS_RCM_Msk             (_U_(0x1) << UHP_EHCI_USBSTS_RCM_Pos)          /**< (UHP_EHCI_USBSTS) Reclamation (read-only) Mask */
#define UHP_EHCI_USBSTS_RCM                 UHP_EHCI_USBSTS_RCM_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_RCM_Msk instead */
#define UHP_EHCI_USBSTS_PSS_Pos             14                                             /**< (UHP_EHCI_USBSTS) Periodic Schedule Status (read-only) Position */
#define UHP_EHCI_USBSTS_PSS_Msk             (_U_(0x1) << UHP_EHCI_USBSTS_PSS_Pos)          /**< (UHP_EHCI_USBSTS) Periodic Schedule Status (read-only) Mask */
#define UHP_EHCI_USBSTS_PSS                 UHP_EHCI_USBSTS_PSS_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_PSS_Msk instead */
#define UHP_EHCI_USBSTS_ASS_Pos             15                                             /**< (UHP_EHCI_USBSTS) Asynchronous Schedule Status (read-only) Position */
#define UHP_EHCI_USBSTS_ASS_Msk             (_U_(0x1) << UHP_EHCI_USBSTS_ASS_Pos)          /**< (UHP_EHCI_USBSTS) Asynchronous Schedule Status (read-only) Mask */
#define UHP_EHCI_USBSTS_ASS                 UHP_EHCI_USBSTS_ASS_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBSTS_ASS_Msk instead */
#define UHP_EHCI_USBSTS_MASK                _U_(0xF03F)                                    /**< \deprecated (UHP_EHCI_USBSTS) Register MASK  (Use UHP_EHCI_USBSTS_Msk instead)  */
#define UHP_EHCI_USBSTS_Msk                 _U_(0xF03F)                                    /**< (UHP_EHCI_USBSTS) Register Mask  */


/* -------- UHP_EHCI_USBINTR : (UHP_EHCI Offset: 0x18) (R/W 32) UHP_EHCI USB Interrupt Enable Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t USBIE:1;                   /**< bit:      0  USB Interrupt Enable                     */
    uint32_t USBEIE:1;                  /**< bit:      1  USB Error Interrupt Enable               */
    uint32_t PCIE:1;                    /**< bit:      2  Port Change Interrupt Enable             */
    uint32_t FLRE:1;                    /**< bit:      3  Frame List Rollover Enable               */
    uint32_t HSEE:1;                    /**< bit:      4  Host System Error Enable                 */
    uint32_t IAAE:1;                    /**< bit:      5  Interrupt on Async Advance Enable        */
    uint32_t :26;                       /**< bit:  6..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_USBINTR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_USBINTR_OFFSET             (0x18)                                        /**<  (UHP_EHCI_USBINTR) UHP_EHCI USB Interrupt Enable Register  Offset */

#define UHP_EHCI_USBINTR_USBIE_Pos          0                                              /**< (UHP_EHCI_USBINTR) USB Interrupt Enable Position */
#define UHP_EHCI_USBINTR_USBIE_Msk          (_U_(0x1) << UHP_EHCI_USBINTR_USBIE_Pos)       /**< (UHP_EHCI_USBINTR) USB Interrupt Enable Mask */
#define UHP_EHCI_USBINTR_USBIE              UHP_EHCI_USBINTR_USBIE_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBINTR_USBIE_Msk instead */
#define UHP_EHCI_USBINTR_USBEIE_Pos         1                                              /**< (UHP_EHCI_USBINTR) USB Error Interrupt Enable Position */
#define UHP_EHCI_USBINTR_USBEIE_Msk         (_U_(0x1) << UHP_EHCI_USBINTR_USBEIE_Pos)      /**< (UHP_EHCI_USBINTR) USB Error Interrupt Enable Mask */
#define UHP_EHCI_USBINTR_USBEIE             UHP_EHCI_USBINTR_USBEIE_Msk                    /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBINTR_USBEIE_Msk instead */
#define UHP_EHCI_USBINTR_PCIE_Pos           2                                              /**< (UHP_EHCI_USBINTR) Port Change Interrupt Enable Position */
#define UHP_EHCI_USBINTR_PCIE_Msk           (_U_(0x1) << UHP_EHCI_USBINTR_PCIE_Pos)        /**< (UHP_EHCI_USBINTR) Port Change Interrupt Enable Mask */
#define UHP_EHCI_USBINTR_PCIE               UHP_EHCI_USBINTR_PCIE_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBINTR_PCIE_Msk instead */
#define UHP_EHCI_USBINTR_FLRE_Pos           3                                              /**< (UHP_EHCI_USBINTR) Frame List Rollover Enable Position */
#define UHP_EHCI_USBINTR_FLRE_Msk           (_U_(0x1) << UHP_EHCI_USBINTR_FLRE_Pos)        /**< (UHP_EHCI_USBINTR) Frame List Rollover Enable Mask */
#define UHP_EHCI_USBINTR_FLRE               UHP_EHCI_USBINTR_FLRE_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBINTR_FLRE_Msk instead */
#define UHP_EHCI_USBINTR_HSEE_Pos           4                                              /**< (UHP_EHCI_USBINTR) Host System Error Enable Position */
#define UHP_EHCI_USBINTR_HSEE_Msk           (_U_(0x1) << UHP_EHCI_USBINTR_HSEE_Pos)        /**< (UHP_EHCI_USBINTR) Host System Error Enable Mask */
#define UHP_EHCI_USBINTR_HSEE               UHP_EHCI_USBINTR_HSEE_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBINTR_HSEE_Msk instead */
#define UHP_EHCI_USBINTR_IAAE_Pos           5                                              /**< (UHP_EHCI_USBINTR) Interrupt on Async Advance Enable Position */
#define UHP_EHCI_USBINTR_IAAE_Msk           (_U_(0x1) << UHP_EHCI_USBINTR_IAAE_Pos)        /**< (UHP_EHCI_USBINTR) Interrupt on Async Advance Enable Mask */
#define UHP_EHCI_USBINTR_IAAE               UHP_EHCI_USBINTR_IAAE_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_USBINTR_IAAE_Msk instead */
#define UHP_EHCI_USBINTR_MASK               _U_(0x3F)                                      /**< \deprecated (UHP_EHCI_USBINTR) Register MASK  (Use UHP_EHCI_USBINTR_Msk instead)  */
#define UHP_EHCI_USBINTR_Msk                _U_(0x3F)                                      /**< (UHP_EHCI_USBINTR) Register Mask  */


/* -------- UHP_EHCI_FRINDEX : (UHP_EHCI Offset: 0x1c) (R/W 32) UHP_EHCI USB Frame Index Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t FI:14;                     /**< bit:  0..13  Frame Index                              */
    uint32_t :18;                       /**< bit: 14..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_FRINDEX_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_FRINDEX_OFFSET             (0x1C)                                        /**<  (UHP_EHCI_FRINDEX) UHP_EHCI USB Frame Index Register  Offset */

#define UHP_EHCI_FRINDEX_FI_Pos             0                                              /**< (UHP_EHCI_FRINDEX) Frame Index Position */
#define UHP_EHCI_FRINDEX_FI_Msk             (_U_(0x3FFF) << UHP_EHCI_FRINDEX_FI_Pos)       /**< (UHP_EHCI_FRINDEX) Frame Index Mask */
#define UHP_EHCI_FRINDEX_FI(value)          (UHP_EHCI_FRINDEX_FI_Msk & ((value) << UHP_EHCI_FRINDEX_FI_Pos))
#define UHP_EHCI_FRINDEX_MASK               _U_(0x3FFF)                                    /**< \deprecated (UHP_EHCI_FRINDEX) Register MASK  (Use UHP_EHCI_FRINDEX_Msk instead)  */
#define UHP_EHCI_FRINDEX_Msk                _U_(0x3FFF)                                    /**< (UHP_EHCI_FRINDEX) Register Mask  */


/* -------- UHP_EHCI_CTRLDSSEGMENT : (UHP_EHCI Offset: 0x20) (R/W 32) UHP_EHCI Control Data Structure Segment Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_CTRLDSSEGMENT_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_CTRLDSSEGMENT_OFFSET       (0x20)                                        /**<  (UHP_EHCI_CTRLDSSEGMENT) UHP_EHCI Control Data Structure Segment Register  Offset */

#define UHP_EHCI_CTRLDSSEGMENT_MASK         _U_(0x00)                                      /**< \deprecated (UHP_EHCI_CTRLDSSEGMENT) Register MASK  (Use UHP_EHCI_CTRLDSSEGMENT_Msk instead)  */
#define UHP_EHCI_CTRLDSSEGMENT_Msk          _U_(0x00)                                      /**< (UHP_EHCI_CTRLDSSEGMENT) Register Mask  */


/* -------- UHP_EHCI_PERIODICLISTBASE : (UHP_EHCI Offset: 0x24) (R/W 32) UHP_EHCI Periodic Frame List Base Address Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t :12;                       /**< bit:  0..11  Reserved */
    uint32_t BA:20;                     /**< bit: 12..31  Base Address (Low)                       */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_PERIODICLISTBASE_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_PERIODICLISTBASE_OFFSET    (0x24)                                        /**<  (UHP_EHCI_PERIODICLISTBASE) UHP_EHCI Periodic Frame List Base Address Register  Offset */

#define UHP_EHCI_PERIODICLISTBASE_BA_Pos    12                                             /**< (UHP_EHCI_PERIODICLISTBASE) Base Address (Low) Position */
#define UHP_EHCI_PERIODICLISTBASE_BA_Msk    (_U_(0xFFFFF) << UHP_EHCI_PERIODICLISTBASE_BA_Pos)  /**< (UHP_EHCI_PERIODICLISTBASE) Base Address (Low) Mask */
#define UHP_EHCI_PERIODICLISTBASE_BA(value) (UHP_EHCI_PERIODICLISTBASE_BA_Msk & ((value) << UHP_EHCI_PERIODICLISTBASE_BA_Pos))
#define UHP_EHCI_PERIODICLISTBASE_MASK      _U_(0xFFFFF000)                                /**< \deprecated (UHP_EHCI_PERIODICLISTBASE) Register MASK  (Use UHP_EHCI_PERIODICLISTBASE_Msk instead)  */
#define UHP_EHCI_PERIODICLISTBASE_Msk       _U_(0xFFFFF000)                                /**< (UHP_EHCI_PERIODICLISTBASE) Register Mask  */


/* -------- UHP_EHCI_ASYNCLISTADDR : (UHP_EHCI Offset: 0x28) (R/W 32) UHP_EHCI Asynchronous List Address Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t :5;                        /**< bit:   0..4  Reserved */
    uint32_t LPL:27;                    /**< bit:  5..31  Link Pointer Low                         */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_ASYNCLISTADDR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_ASYNCLISTADDR_OFFSET       (0x28)                                        /**<  (UHP_EHCI_ASYNCLISTADDR) UHP_EHCI Asynchronous List Address Register  Offset */

#define UHP_EHCI_ASYNCLISTADDR_LPL_Pos      5                                              /**< (UHP_EHCI_ASYNCLISTADDR) Link Pointer Low Position */
#define UHP_EHCI_ASYNCLISTADDR_LPL_Msk      (_U_(0x7FFFFFF) << UHP_EHCI_ASYNCLISTADDR_LPL_Pos)  /**< (UHP_EHCI_ASYNCLISTADDR) Link Pointer Low Mask */
#define UHP_EHCI_ASYNCLISTADDR_LPL(value)   (UHP_EHCI_ASYNCLISTADDR_LPL_Msk & ((value) << UHP_EHCI_ASYNCLISTADDR_LPL_Pos))
#define UHP_EHCI_ASYNCLISTADDR_MASK         _U_(0xFFFFFFE0)                                /**< \deprecated (UHP_EHCI_ASYNCLISTADDR) Register MASK  (Use UHP_EHCI_ASYNCLISTADDR_Msk instead)  */
#define UHP_EHCI_ASYNCLISTADDR_Msk          _U_(0xFFFFFFE0)                                /**< (UHP_EHCI_ASYNCLISTADDR) Register Mask  */


/* -------- UHP_EHCI_CONFIGFLAG : (UHP_EHCI Offset: 0x50) (R/W 32) UHP_EHCI Configured Flag Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t CF:1;                      /**< bit:      0  Configure Flag (read/write)              */
    uint32_t :31;                       /**< bit:  1..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_CONFIGFLAG_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_CONFIGFLAG_OFFSET          (0x50)                                        /**<  (UHP_EHCI_CONFIGFLAG) UHP_EHCI Configured Flag Register  Offset */

#define UHP_EHCI_CONFIGFLAG_CF_Pos          0                                              /**< (UHP_EHCI_CONFIGFLAG) Configure Flag (read/write) Position */
#define UHP_EHCI_CONFIGFLAG_CF_Msk          (_U_(0x1) << UHP_EHCI_CONFIGFLAG_CF_Pos)       /**< (UHP_EHCI_CONFIGFLAG) Configure Flag (read/write) Mask */
#define UHP_EHCI_CONFIGFLAG_CF              UHP_EHCI_CONFIGFLAG_CF_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_CONFIGFLAG_CF_Msk instead */
#define UHP_EHCI_CONFIGFLAG_MASK            _U_(0x01)                                      /**< \deprecated (UHP_EHCI_CONFIGFLAG) Register MASK  (Use UHP_EHCI_CONFIGFLAG_Msk instead)  */
#define UHP_EHCI_CONFIGFLAG_Msk             _U_(0x01)                                      /**< (UHP_EHCI_CONFIGFLAG) Register Mask  */


/* -------- UHP_EHCI_PORTSC : (UHP_EHCI Offset: 0x54) (R/W 32) UHP_EHCI Port Status and Control Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t CCS:1;                     /**< bit:      0  Current Connect Status (read-only)       */
    uint32_t CSC:1;                     /**< bit:      1  Connect Status Change (read/write clear) */
    uint32_t PED:1;                     /**< bit:      2  Port Enabled/Disabled (read/write)       */
    uint32_t PEDC:1;                    /**< bit:      3  Port Enable/Disable Change (read/write clear) */
    uint32_t OCA:1;                     /**< bit:      4  Over-current Active (read-only)          */
    uint32_t OCC:1;                     /**< bit:      5  Over-current Change (read/write clear)   */
    uint32_t FPR:1;                     /**< bit:      6  Force Port Resume (read/write)           */
    uint32_t SUS:1;                     /**< bit:      7  Suspend (read/write)                     */
    uint32_t PR:1;                      /**< bit:      8  Port Reset (read/write)                  */
    uint32_t :1;                        /**< bit:      9  Reserved */
    uint32_t LS:2;                      /**< bit: 10..11  Line Status (read-only)                  */
    uint32_t PP:1;                      /**< bit:     12  Port Power (read/write or read-only)     */
    uint32_t PO:1;                      /**< bit:     13  Port Owner (read/write)                  */
    uint32_t PIC:2;                     /**< bit: 14..15  Port Indicator Control (read/write)      */
    uint32_t PTC:4;                     /**< bit: 16..19  Port Test Control (read/write)           */
    uint32_t WKCNNT_E:1;                /**< bit:     20  Wake on Connect Enable (read/write)      */
    uint32_t WKDSCNNT_E:1;              /**< bit:     21  Wake on Disconnect Enable (read/write)   */
    uint32_t WKOC_E:1;                  /**< bit:     22  Wake on Over-current Enable (read/write) */
    uint32_t :9;                        /**< bit: 23..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_PORTSC_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_PORTSC_OFFSET              (0x54)                                        /**<  (UHP_EHCI_PORTSC) UHP_EHCI Port Status and Control Register  Offset */

#define UHP_EHCI_PORTSC_CCS_Pos             0                                              /**< (UHP_EHCI_PORTSC) Current Connect Status (read-only) Position */
#define UHP_EHCI_PORTSC_CCS_Msk             (_U_(0x1) << UHP_EHCI_PORTSC_CCS_Pos)          /**< (UHP_EHCI_PORTSC) Current Connect Status (read-only) Mask */
#define UHP_EHCI_PORTSC_CCS                 UHP_EHCI_PORTSC_CCS_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_CCS_Msk instead */
#define UHP_EHCI_PORTSC_CSC_Pos             1                                              /**< (UHP_EHCI_PORTSC) Connect Status Change (read/write clear) Position */
#define UHP_EHCI_PORTSC_CSC_Msk             (_U_(0x1) << UHP_EHCI_PORTSC_CSC_Pos)          /**< (UHP_EHCI_PORTSC) Connect Status Change (read/write clear) Mask */
#define UHP_EHCI_PORTSC_CSC                 UHP_EHCI_PORTSC_CSC_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_CSC_Msk instead */
#define UHP_EHCI_PORTSC_PED_Pos             2                                              /**< (UHP_EHCI_PORTSC) Port Enabled/Disabled (read/write) Position */
#define UHP_EHCI_PORTSC_PED_Msk             (_U_(0x1) << UHP_EHCI_PORTSC_PED_Pos)          /**< (UHP_EHCI_PORTSC) Port Enabled/Disabled (read/write) Mask */
#define UHP_EHCI_PORTSC_PED                 UHP_EHCI_PORTSC_PED_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_PED_Msk instead */
#define UHP_EHCI_PORTSC_PEDC_Pos            3                                              /**< (UHP_EHCI_PORTSC) Port Enable/Disable Change (read/write clear) Position */
#define UHP_EHCI_PORTSC_PEDC_Msk            (_U_(0x1) << UHP_EHCI_PORTSC_PEDC_Pos)         /**< (UHP_EHCI_PORTSC) Port Enable/Disable Change (read/write clear) Mask */
#define UHP_EHCI_PORTSC_PEDC                UHP_EHCI_PORTSC_PEDC_Msk                       /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_PEDC_Msk instead */
#define UHP_EHCI_PORTSC_OCA_Pos             4                                              /**< (UHP_EHCI_PORTSC) Over-current Active (read-only) Position */
#define UHP_EHCI_PORTSC_OCA_Msk             (_U_(0x1) << UHP_EHCI_PORTSC_OCA_Pos)          /**< (UHP_EHCI_PORTSC) Over-current Active (read-only) Mask */
#define UHP_EHCI_PORTSC_OCA                 UHP_EHCI_PORTSC_OCA_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_OCA_Msk instead */
#define UHP_EHCI_PORTSC_OCC_Pos             5                                              /**< (UHP_EHCI_PORTSC) Over-current Change (read/write clear) Position */
#define UHP_EHCI_PORTSC_OCC_Msk             (_U_(0x1) << UHP_EHCI_PORTSC_OCC_Pos)          /**< (UHP_EHCI_PORTSC) Over-current Change (read/write clear) Mask */
#define UHP_EHCI_PORTSC_OCC                 UHP_EHCI_PORTSC_OCC_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_OCC_Msk instead */
#define UHP_EHCI_PORTSC_FPR_Pos             6                                              /**< (UHP_EHCI_PORTSC) Force Port Resume (read/write) Position */
#define UHP_EHCI_PORTSC_FPR_Msk             (_U_(0x1) << UHP_EHCI_PORTSC_FPR_Pos)          /**< (UHP_EHCI_PORTSC) Force Port Resume (read/write) Mask */
#define UHP_EHCI_PORTSC_FPR                 UHP_EHCI_PORTSC_FPR_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_FPR_Msk instead */
#define UHP_EHCI_PORTSC_SUS_Pos             7                                              /**< (UHP_EHCI_PORTSC) Suspend (read/write) Position */
#define UHP_EHCI_PORTSC_SUS_Msk             (_U_(0x1) << UHP_EHCI_PORTSC_SUS_Pos)          /**< (UHP_EHCI_PORTSC) Suspend (read/write) Mask */
#define UHP_EHCI_PORTSC_SUS                 UHP_EHCI_PORTSC_SUS_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_SUS_Msk instead */
#define UHP_EHCI_PORTSC_PR_Pos              8                                              /**< (UHP_EHCI_PORTSC) Port Reset (read/write) Position */
#define UHP_EHCI_PORTSC_PR_Msk              (_U_(0x1) << UHP_EHCI_PORTSC_PR_Pos)           /**< (UHP_EHCI_PORTSC) Port Reset (read/write) Mask */
#define UHP_EHCI_PORTSC_PR                  UHP_EHCI_PORTSC_PR_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_PR_Msk instead */
#define UHP_EHCI_PORTSC_LS_Pos              10                                             /**< (UHP_EHCI_PORTSC) Line Status (read-only) Position */
#define UHP_EHCI_PORTSC_LS_Msk              (_U_(0x3) << UHP_EHCI_PORTSC_LS_Pos)           /**< (UHP_EHCI_PORTSC) Line Status (read-only) Mask */
#define UHP_EHCI_PORTSC_LS(value)           (UHP_EHCI_PORTSC_LS_Msk & ((value) << UHP_EHCI_PORTSC_LS_Pos))
#define UHP_EHCI_PORTSC_PP_Pos              12                                             /**< (UHP_EHCI_PORTSC) Port Power (read/write or read-only) Position */
#define UHP_EHCI_PORTSC_PP_Msk              (_U_(0x1) << UHP_EHCI_PORTSC_PP_Pos)           /**< (UHP_EHCI_PORTSC) Port Power (read/write or read-only) Mask */
#define UHP_EHCI_PORTSC_PP                  UHP_EHCI_PORTSC_PP_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_PP_Msk instead */
#define UHP_EHCI_PORTSC_PO_Pos              13                                             /**< (UHP_EHCI_PORTSC) Port Owner (read/write) Position */
#define UHP_EHCI_PORTSC_PO_Msk              (_U_(0x1) << UHP_EHCI_PORTSC_PO_Pos)           /**< (UHP_EHCI_PORTSC) Port Owner (read/write) Mask */
#define UHP_EHCI_PORTSC_PO                  UHP_EHCI_PORTSC_PO_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_PO_Msk instead */
#define UHP_EHCI_PORTSC_PIC_Pos             14                                             /**< (UHP_EHCI_PORTSC) Port Indicator Control (read/write) Position */
#define UHP_EHCI_PORTSC_PIC_Msk             (_U_(0x3) << UHP_EHCI_PORTSC_PIC_Pos)          /**< (UHP_EHCI_PORTSC) Port Indicator Control (read/write) Mask */
#define UHP_EHCI_PORTSC_PIC(value)          (UHP_EHCI_PORTSC_PIC_Msk & ((value) << UHP_EHCI_PORTSC_PIC_Pos))
#define UHP_EHCI_PORTSC_PTC_Pos             16                                             /**< (UHP_EHCI_PORTSC) Port Test Control (read/write) Position */
#define UHP_EHCI_PORTSC_PTC_Msk             (_U_(0xF) << UHP_EHCI_PORTSC_PTC_Pos)          /**< (UHP_EHCI_PORTSC) Port Test Control (read/write) Mask */
#define UHP_EHCI_PORTSC_PTC(value)          (UHP_EHCI_PORTSC_PTC_Msk & ((value) << UHP_EHCI_PORTSC_PTC_Pos))
#define UHP_EHCI_PORTSC_WKCNNT_E_Pos        20                                             /**< (UHP_EHCI_PORTSC) Wake on Connect Enable (read/write) Position */
#define UHP_EHCI_PORTSC_WKCNNT_E_Msk        (_U_(0x1) << UHP_EHCI_PORTSC_WKCNNT_E_Pos)     /**< (UHP_EHCI_PORTSC) Wake on Connect Enable (read/write) Mask */
#define UHP_EHCI_PORTSC_WKCNNT_E            UHP_EHCI_PORTSC_WKCNNT_E_Msk                   /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_WKCNNT_E_Msk instead */
#define UHP_EHCI_PORTSC_WKDSCNNT_E_Pos      21                                             /**< (UHP_EHCI_PORTSC) Wake on Disconnect Enable (read/write) Position */
#define UHP_EHCI_PORTSC_WKDSCNNT_E_Msk      (_U_(0x1) << UHP_EHCI_PORTSC_WKDSCNNT_E_Pos)   /**< (UHP_EHCI_PORTSC) Wake on Disconnect Enable (read/write) Mask */
#define UHP_EHCI_PORTSC_WKDSCNNT_E          UHP_EHCI_PORTSC_WKDSCNNT_E_Msk                 /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_WKDSCNNT_E_Msk instead */
#define UHP_EHCI_PORTSC_WKOC_E_Pos          22                                             /**< (UHP_EHCI_PORTSC) Wake on Over-current Enable (read/write) Position */
#define UHP_EHCI_PORTSC_WKOC_E_Msk          (_U_(0x1) << UHP_EHCI_PORTSC_WKOC_E_Pos)       /**< (UHP_EHCI_PORTSC) Wake on Over-current Enable (read/write) Mask */
#define UHP_EHCI_PORTSC_WKOC_E              UHP_EHCI_PORTSC_WKOC_E_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_PORTSC_WKOC_E_Msk instead */
#define UHP_EHCI_PORTSC_MASK                _U_(0x7FFDFF)                                  /**< \deprecated (UHP_EHCI_PORTSC) Register MASK  (Use UHP_EHCI_PORTSC_Msk instead)  */
#define UHP_EHCI_PORTSC_Msk                 _U_(0x7FFDFF)                                  /**< (UHP_EHCI_PORTSC) Register Mask  */


/* -------- UHP_EHCI_HSIC : (UHP_EHCI Offset: 0xb0) (R/W 32) UHP_EHCI HSIC -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t :1;                        /**< bit:      0  Reserved */
    uint32_t HSIC_EN:1;                 /**< bit:      1  UHP_EHCI HSIC Enable/Disable             */
    uint32_t :30;                       /**< bit:  2..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UHP_EHCI_HSIC_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UHP_EHCI_HSIC_OFFSET                (0xB0)                                        /**<  (UHP_EHCI_HSIC) UHP_EHCI HSIC  Offset */

#define UHP_EHCI_HSIC_HSIC_EN_Pos           1                                              /**< (UHP_EHCI_HSIC) UHP_EHCI HSIC Enable/Disable Position */
#define UHP_EHCI_HSIC_HSIC_EN_Msk           (_U_(0x1) << UHP_EHCI_HSIC_HSIC_EN_Pos)        /**< (UHP_EHCI_HSIC) UHP_EHCI HSIC Enable/Disable Mask */
#define UHP_EHCI_HSIC_HSIC_EN               UHP_EHCI_HSIC_HSIC_EN_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UHP_EHCI_HSIC_HSIC_EN_Msk instead */
#define UHP_EHCI_HSIC_MASK                  _U_(0x02)                                      /**< \deprecated (UHP_EHCI_HSIC) Register MASK  (Use UHP_EHCI_HSIC_Msk instead)  */
#define UHP_EHCI_HSIC_Msk                   _U_(0x02)                                      /**< (UHP_EHCI_HSIC) Register Mask  */


#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'R'
/** \brief UHP_EHCI hardware registers */
typedef struct {  
  __I  uint32_t UHP_EHCI_HCCAPBASE; /**< (UHP_EHCI Offset: 0x00) UHP_EHCI Host Controller Capability Register */
  __I  uint32_t UHP_EHCI_HCSPARAMS; /**< (UHP_EHCI Offset: 0x04) UHP_EHCI Host Controller Structural Parameters Register */
  __I  uint32_t UHP_EHCI_HCCPARAMS; /**< (UHP_EHCI Offset: 0x08) UHP_EHCI Host Controller Capability Parameters Register */
  __I  uint8_t                        Reserved1[4];
  __IO uint32_t UHP_EHCI_USBCMD; /**< (UHP_EHCI Offset: 0x10) UHP_EHCI USB Command Register */
  __IO uint32_t UHP_EHCI_USBSTS; /**< (UHP_EHCI Offset: 0x14) UHP_EHCI USB Status Register */
  __IO uint32_t UHP_EHCI_USBINTR; /**< (UHP_EHCI Offset: 0x18) UHP_EHCI USB Interrupt Enable Register */
  __IO uint32_t UHP_EHCI_FRINDEX; /**< (UHP_EHCI Offset: 0x1C) UHP_EHCI USB Frame Index Register */
  __IO uint32_t UHP_EHCI_CTRLDSSEGMENT; /**< (UHP_EHCI Offset: 0x20) UHP_EHCI Control Data Structure Segment Register */
  __IO uint32_t UHP_EHCI_PERIODICLISTBASE; /**< (UHP_EHCI Offset: 0x24) UHP_EHCI Periodic Frame List Base Address Register */
  __IO uint32_t UHP_EHCI_ASYNCLISTADDR; /**< (UHP_EHCI Offset: 0x28) UHP_EHCI Asynchronous List Address Register */
  __I  uint8_t                        Reserved2[36];
  __IO uint32_t UHP_EHCI_CONFIGFLAG; /**< (UHP_EHCI Offset: 0x50) UHP_EHCI Configured Flag Register */
  __IO uint32_t UHP_EHCI_PORTSC[2]; /**< (UHP_EHCI Offset: 0x54) UHP_EHCI Port Status and Control Register */
  __I  uint8_t                        Reserved3[84];
  __IO uint32_t UHP_EHCI_HSIC;  /**< (UHP_EHCI Offset: 0xB0) UHP_EHCI HSIC */
} UhpEhci;

#elif COMPONENT_TYPEDEF_STYLE == 'N'
/** \brief UHP_EHCI hardware registers */
typedef struct {  
  __I  UHP_EHCI_HCCAPBASE_Type        UHP_EHCI_HCCAPBASE; /**< Offset: 0x00 (R/   32) UHP_EHCI Host Controller Capability Register */
  __I  UHP_EHCI_HCSPARAMS_Type        UHP_EHCI_HCSPARAMS; /**< Offset: 0x04 (R/   32) UHP_EHCI Host Controller Structural Parameters Register */
  __I  UHP_EHCI_HCCPARAMS_Type        UHP_EHCI_HCCPARAMS; /**< Offset: 0x08 (R/   32) UHP_EHCI Host Controller Capability Parameters Register */
  __I  uint8_t                        Reserved1[4];
  __IO UHP_EHCI_USBCMD_Type           UHP_EHCI_USBCMD; /**< Offset: 0x10 (R/W  32) UHP_EHCI USB Command Register */
  __IO UHP_EHCI_USBSTS_Type           UHP_EHCI_USBSTS; /**< Offset: 0x14 (R/W  32) UHP_EHCI USB Status Register */
  __IO UHP_EHCI_USBINTR_Type          UHP_EHCI_USBINTR; /**< Offset: 0x18 (R/W  32) UHP_EHCI USB Interrupt Enable Register */
  __IO UHP_EHCI_FRINDEX_Type          UHP_EHCI_FRINDEX; /**< Offset: 0x1C (R/W  32) UHP_EHCI USB Frame Index Register */
  __IO UHP_EHCI_CTRLDSSEGMENT_Type    UHP_EHCI_CTRLDSSEGMENT; /**< Offset: 0x20 (R/W  32) UHP_EHCI Control Data Structure Segment Register */
  __IO UHP_EHCI_PERIODICLISTBASE_Type UHP_EHCI_PERIODICLISTBASE; /**< Offset: 0x24 (R/W  32) UHP_EHCI Periodic Frame List Base Address Register */
  __IO UHP_EHCI_ASYNCLISTADDR_Type    UHP_EHCI_ASYNCLISTADDR; /**< Offset: 0x28 (R/W  32) UHP_EHCI Asynchronous List Address Register */
  __I  uint8_t                        Reserved2[36];
  __IO UHP_EHCI_CONFIGFLAG_Type       UHP_EHCI_CONFIGFLAG; /**< Offset: 0x50 (R/W  32) UHP_EHCI Configured Flag Register */
  __IO UHP_EHCI_PORTSC_Type           UHP_EHCI_PORTSC[2]; /**< Offset: 0x54 (R/W  32) UHP_EHCI Port Status and Control Register */
  __I  uint8_t                        Reserved3[84];
  __IO UHP_EHCI_HSIC_Type             UHP_EHCI_HSIC;  /**< Offset: 0xB0 (R/W  32) UHP_EHCI HSIC */
} UhpEhci;

#else /* COMPONENT_TYPEDEF_STYLE */
#error Unknown component typedef style
#endif /* COMPONENT_TYPEDEF_STYLE */

#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */
/** @}  end of USB Host High Speed Port */

#endif /* _DA78_UHP_EHCI_COMPONENT_H_ */
