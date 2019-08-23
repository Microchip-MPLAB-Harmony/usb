/*******************************************************************************
  FLEXCOM USART PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_flexcom_usart_local.h

  Summary
    Data Type definition of the USART Peripheral Interface Plib.

  Description
    This file defines the Data Types for the USART Plib.

  Remarks:
    None.
*******************************************************************************/

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

#ifndef PLIB_FLEXCOM_USART_LOCAL_H // Guards against multiple inclusion
#define PLIB_FLEXCOM_USART_LOCAL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

/* FLEXCOM USART Errors */
typedef enum
{
    FLEXCOM_USART_ERROR_NONE = 0,
    FLEXCOM_USART_ERROR_OVERRUN = FLEX_US_CSR_OVRE_Msk,
    FLEXCOM_USART_ERROR_PARITY = FLEX_US_CSR_PARE_Msk,
    FLEXCOM_USART_ERROR_FRAMING = FLEX_US_CSR_FRAME_Msk,

    /* Force the compiler to reserve 32-bit memory for each enum */
    FLEXCOM_USART_ERROR_INVALID = 0xFFFFFFFF

} FLEXCOM_USART_ERROR;

/* FLEXCOM USART Data Width */
typedef enum
{
    FLEXCOM_USART_DATA_5_BIT = FLEX_US_MR_CHRL_5_BIT,
    FLEXCOM_USART_DATA_6_BIT = FLEX_US_MR_CHRL_6_BIT,
    FLEXCOM_USART_DATA_7_BIT = FLEX_US_MR_CHRL_7_BIT,
    FLEXCOM_USART_DATA_8_BIT = FLEX_US_MR_CHRL_8_BIT,
    FLEXCOM_USART_DATA_9_BIT = FLEX_US_MR_MODE9_Msk,

    /* Force the compiler to reserve 32-bit memory for each enum */
    FLEXCOM_USART_DATA_INVALID = 0xFFFFFFFF

} FLEXCOM_USART_DATA;

/* FLEXCOM USART Parity */
typedef enum
{
    FLEXCOM_USART_PARITY_NONE = FLEX_US_MR_PAR_NO,
    FLEXCOM_USART_PARITY_ODD = FLEX_US_MR_PAR_ODD,
    FLEXCOM_USART_PARITY_EVEN = FLEX_US_MR_PAR_EVEN,
    FLEXCOM_USART_PARITY_MARK = FLEX_US_MR_PAR_MARK,
    FLEXCOM_USART_PARITY_SPACE = FLEX_US_MR_PAR_SPACE,
    FLEXCOM_USART_PARITY_MULTIDROP = FLEX_US_MR_PAR_MULTIDROP,

    /* Force the compiler to reserve 32-bit memory for each enum */
    FLEXCOM_USART_PARITY_INVALID = 0xFFFFFFFF

} FLEXCOM_USART_PARITY;

/* FLEXCOM USART Stop bits */
typedef enum
{
    FLEXCOM_USART_STOP_1_BIT = FLEX_US_MR_NBSTOP_1_BIT,
    FLEXCOM_USART_STOP_1_5_BIT = FLEX_US_MR_NBSTOP_1_5_BIT,
    FLEXCOM_USART_STOP_2_BIT = FLEX_US_MR_NBSTOP_2_BIT,

    /* Force the compiler to reserve 32-bit memory for each enum */
    FLEXCOM_USART_STOP_INVALID = 0xFFFFFFFF

} FLEXCOM_USART_STOP;

/* FLEXCOM USART Serial Setup */
typedef struct
{
    uint32_t baudRate;
    FLEXCOM_USART_DATA dataWidth;
    FLEXCOM_USART_PARITY parity;
    FLEXCOM_USART_STOP stopBits;

} FLEXCOM_USART_SERIAL_SETUP;

/* Callback Function Pointer */
typedef void (*FLEXCOM_USART_CALLBACK)( uintptr_t context );

/* FLEXCOM USART Object */
typedef struct
{
    uint8_t *               txBuffer;
    size_t                  txSize;
    size_t                  txProcessedSize;
    FLEXCOM_USART_CALLBACK  txCallback;
    uintptr_t               txContext;
    bool                    txBusyStatus;

    uint8_t *               rxBuffer;
    size_t                  rxSize;
    size_t                  rxProcessedSize;
    FLEXCOM_USART_CALLBACK  rxCallback;
    uintptr_t               rxContext;
    bool                    rxBusyStatus;

} FLEXCOM_USART_OBJECT ;

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
// DOM-IGNORE-END

#endif //PLIB_FLEXCOM_USART_LOCAL_H
