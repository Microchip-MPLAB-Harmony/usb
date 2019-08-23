/*******************************************************************************
  FLEXCOM7 USART PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_flexcom7_usart.c

  Summary:
    FLEXCOM7 USART PLIB Implementation File

  Description
    This file defines the interface to the FLEXCOM7 USART peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include "plib_flexcom7_usart.h"

// *****************************************************************************
// *****************************************************************************
// Section: FLEXCOM7 USART Implementation
// *****************************************************************************
// *****************************************************************************

FLEXCOM_USART_OBJECT flexcom7UsartObj;

void static FLEXCOM7_USART_ISR_RX_Handler( void )
{
    if(flexcom7UsartObj.rxBusyStatus == true)
    {
        while((FLEX_US_CSR_RXRDY_Msk == (FLEXCOM7_REGS->FLEX_US_CSR & FLEX_US_CSR_RXRDY_Msk)) && (flexcom7UsartObj.rxSize > flexcom7UsartObj.rxProcessedSize) )
        {
            flexcom7UsartObj.rxBuffer[flexcom7UsartObj.rxProcessedSize++] = (FLEXCOM7_REGS->FLEX_US_RHR & FLEX_US_RHR_RXCHR_Msk);
        }

        /* Check if the buffer is done */
        if(flexcom7UsartObj.rxProcessedSize >= flexcom7UsartObj.rxSize)
        {
            flexcom7UsartObj.rxBusyStatus = false;

            /* Disable Read, Overrun, Parity and Framing error interrupts */
            FLEXCOM7_REGS->FLEX_US_IDR = (FLEX_US_IDR_RXRDY_Msk | FLEX_US_IDR_FRAME_Msk | FLEX_US_IDR_PARE_Msk | FLEX_US_IDR_OVRE_Msk);

            if(flexcom7UsartObj.rxCallback != NULL)
            {
                flexcom7UsartObj.rxCallback(flexcom7UsartObj.rxContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
        ;
    }

    return;
}

void static FLEXCOM7_USART_ISR_TX_Handler( void )
{
    if(flexcom7UsartObj.txBusyStatus == true)
    {
        while((FLEX_US_CSR_TXEMPTY_Msk == (FLEXCOM7_REGS->FLEX_US_CSR & FLEX_US_CSR_TXEMPTY_Msk)) && (flexcom7UsartObj.txSize > flexcom7UsartObj.txProcessedSize) )
        {
            FLEXCOM7_REGS->FLEX_US_THR = flexcom7UsartObj.txBuffer[flexcom7UsartObj.txProcessedSize++];
        }

        /* Check if the buffer is done */
        if(flexcom7UsartObj.txProcessedSize >= flexcom7UsartObj.txSize)
        {
            flexcom7UsartObj.txBusyStatus = false;
            FLEXCOM7_REGS->FLEX_US_IDR = FLEX_US_IDR_TXEMPTY_Msk;

            if(flexcom7UsartObj.txCallback != NULL)
            {
                flexcom7UsartObj.txCallback(flexcom7UsartObj.txContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
        ;
    }

    return;
}

void FLEXCOM7_InterruptHandler( void )
{
    /* Channel status */
    uint32_t channelStatus = FLEXCOM7_REGS->FLEX_US_CSR;

    /* Error status */
    uint32_t errorStatus = (channelStatus & (FLEX_US_CSR_OVRE_Msk | FLEX_US_CSR_FRAME_Msk | FLEX_US_CSR_PARE_Msk));

    if(errorStatus != 0)
    {
        /* Client must call FLEXCOM7_USART_ErrorGet() function to clear the errors */

        /* USART errors are normally associated with the receiver, hence calling
         * receiver context */
        if( flexcom7UsartObj.rxCallback != NULL )
        {
            flexcom7UsartObj.rxCallback(flexcom7UsartObj.rxContext);
        }

        flexcom7UsartObj.rxBusyStatus = false;

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        FLEXCOM7_REGS->FLEX_US_IDR = (FLEX_US_IDR_RXRDY_Msk | FLEX_US_IDR_FRAME_Msk | FLEX_US_IDR_PARE_Msk | FLEX_US_IDR_OVRE_Msk);
    }

    /* Receiver status */
    if(FLEX_US_CSR_RXRDY_Msk == (channelStatus & FLEX_US_CSR_RXRDY_Msk))
    {
        FLEXCOM7_USART_ISR_RX_Handler();
    }

    /* Transmitter status */
    if(FLEX_US_CSR_TXEMPTY_Msk == (channelStatus & FLEX_US_CSR_TXEMPTY_Msk))
    {
        FLEXCOM7_USART_ISR_TX_Handler();
    }

    return;
}


void static FLEXCOM7_USART_ErrorClear( void )
{
    uint8_t dummyData = 0u;

    FLEXCOM7_REGS->FLEX_US_CR = FLEX_US_CR_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( FLEX_US_CSR_RXRDY_Msk == (FLEXCOM7_REGS->FLEX_US_CSR& FLEX_US_CSR_RXRDY_Msk) )
    {
        dummyData = (FLEXCOM7_REGS->FLEX_US_RHR& FLEX_US_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;

    return;
}

void FLEXCOM7_USART_Initialize( void )
{
    /* Set FLEXCOM USART operating mode */
    FLEXCOM7_REGS->FLEX_MR = FLEX_MR_OPMODE_USART;

    /* Reset FLEXCOM7 USART */
    FLEXCOM7_REGS->FLEX_US_CR = (FLEX_US_CR_RSTRX_Msk | FLEX_US_CR_RSTTX_Msk | FLEX_US_CR_RSTSTA_Msk);

    /* Enable FLEXCOM7 USART */
    FLEXCOM7_REGS->FLEX_US_CR = (FLEX_US_CR_TXEN_Msk | FLEX_US_CR_RXEN_Msk);

    /* Configure FLEXCOM7 USART mode */
    FLEXCOM7_REGS->FLEX_US_MR = ((FLEX_US_MR_USCLKS_MCK) | FLEX_US_MR_CHRL_8_BIT | FLEX_US_MR_PAR_NO | FLEX_US_MR_NBSTOP_1_BIT | (0 << FLEX_US_MR_OVER_Pos));

    /* Configure FLEXCOM7 USART Baud Rate */
    FLEXCOM7_REGS->FLEX_US_BRGR = FLEX_US_BRGR_CD(108);

    /* Initialize instance object */
    flexcom7UsartObj.rxBuffer = NULL;
    flexcom7UsartObj.rxSize = 0;
    flexcom7UsartObj.rxProcessedSize = 0;
    flexcom7UsartObj.rxBusyStatus = false;
    flexcom7UsartObj.rxCallback = NULL;
    flexcom7UsartObj.txBuffer = NULL;
    flexcom7UsartObj.txSize = 0;
    flexcom7UsartObj.txProcessedSize = 0;
    flexcom7UsartObj.txBusyStatus = false;
    flexcom7UsartObj.txCallback = NULL;

    return;
}

FLEXCOM_USART_ERROR FLEXCOM7_USART_ErrorGet( void )
{
    FLEXCOM_USART_ERROR errors = FLEXCOM_USART_ERROR_NONE;
    uint32_t status = FLEXCOM7_REGS->FLEX_US_CSR;

    /* Collect all errors */
    if(status & FLEX_US_CSR_OVRE_Msk)
    {
        errors = FLEXCOM_USART_ERROR_OVERRUN;
    }
    if(status & FLEX_US_CSR_PARE_Msk)
    {
        errors |= FLEXCOM_USART_ERROR_PARITY;
    }
    if(status & FLEX_US_CSR_FRAME_Msk)
    {
        errors |= FLEXCOM_USART_ERROR_FRAMING;
    }

    if(errors != FLEXCOM_USART_ERROR_NONE)
    {
        FLEXCOM7_USART_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool FLEXCOM7_USART_SerialSetup( FLEXCOM_USART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    uint32_t baud = 0;
    uint32_t brgVal = 0;
    uint32_t overSampVal = 0;
    uint32_t usartMode;
    bool status = false;

    if((flexcom7UsartObj.rxBusyStatus == true) || (flexcom7UsartObj.txBusyStatus == true))
    {
        /* Transaction is in progress, so return without updating settings */
        return false;
    }

    if (setup != NULL)
    {
        baud = setup->baudRate;
        if(srcClkFreq == 0)
        {
            srcClkFreq = FLEXCOM7_USART_FrequencyGet();
        }

        /* Calculate BRG value */
        if (srcClkFreq >= (16 * baud))
        {
            brgVal = (srcClkFreq / (16 * baud));
        }
        else
        {
            brgVal = (srcClkFreq / (8 * baud));
            overSampVal = (1 << FLEX_US_MR_OVER_Pos) & FLEX_US_MR_OVER_Msk;
        }

        /* Configure FLEXCOM7 USART mode */
        usartMode = FLEXCOM7_REGS->FLEX_US_MR;
        usartMode &= ~(FLEX_US_MR_CHRL_Msk | FLEX_US_MR_MODE9_Msk | FLEX_US_MR_PAR_Msk | FLEX_US_MR_NBSTOP_Msk | FLEX_US_MR_OVER_Msk);
        FLEXCOM7_REGS->FLEX_US_MR = usartMode | ((uint32_t)setup->dataWidth | (uint32_t)setup->parity | (uint32_t)setup->stopBits | overSampVal);

        /* Configure FLEXCOM7 USART Baud Rate */
        FLEXCOM7_REGS->FLEX_US_BRGR = FLEX_US_BRGR_CD(brgVal);
        status = true;
    }

    return status;
}

bool FLEXCOM7_USART_Read( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Clear errors before submitting the request.
         * ErrorGet clears errors internally. */
        FLEXCOM7_USART_ErrorGet();

        /* Check if receive request is in progress */
        if(flexcom7UsartObj.rxBusyStatus == false)
        {
            flexcom7UsartObj.rxBuffer = lBuffer;
            flexcom7UsartObj.rxSize = size;
            flexcom7UsartObj.rxProcessedSize = 0;
            flexcom7UsartObj.rxBusyStatus = true;
            status = true;

            /* Enable Read, Overrun, Parity and Framing error interrupts */
            FLEXCOM7_REGS->FLEX_US_IER = (FLEX_US_IER_RXRDY_Msk | FLEX_US_IER_FRAME_Msk | FLEX_US_IER_PARE_Msk | FLEX_US_IER_OVRE_Msk);
        }
    }

    return status;
}

bool FLEXCOM7_USART_Write( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Check if transmit request is in progress */
        if(flexcom7UsartObj.txBusyStatus == false)
        {
            flexcom7UsartObj.txBuffer = lBuffer;
            flexcom7UsartObj.txSize = size;
            flexcom7UsartObj.txProcessedSize = 0;
            flexcom7UsartObj.txBusyStatus = true;
            status = true;

            /* Initiate the transfer by sending first byte */
            if(FLEX_US_CSR_TXEMPTY_Msk == (FLEXCOM7_REGS->FLEX_US_CSR& FLEX_US_CSR_TXEMPTY_Msk))
            {
                FLEXCOM7_REGS->FLEX_US_THR = (FLEX_US_THR_TXCHR(*lBuffer) & FLEX_US_THR_TXCHR_Msk);
                flexcom7UsartObj.txProcessedSize++;
            }

            FLEXCOM7_REGS->FLEX_US_IER = FLEX_US_IER_TXEMPTY_Msk;
        }
    }

    return status;
}

void FLEXCOM7_USART_WriteCallbackRegister( FLEXCOM_USART_CALLBACK callback, uintptr_t context )
{
    flexcom7UsartObj.txCallback = callback;
    flexcom7UsartObj.txContext = context;
}

void FLEXCOM7_USART_ReadCallbackRegister( FLEXCOM_USART_CALLBACK callback, uintptr_t context )
{
    flexcom7UsartObj.rxCallback = callback;
    flexcom7UsartObj.rxContext = context;
}

bool FLEXCOM7_USART_WriteIsBusy( void )
{
    return flexcom7UsartObj.txBusyStatus;
}

bool FLEXCOM7_USART_ReadIsBusy( void )
{
    return flexcom7UsartObj.rxBusyStatus;
}

size_t FLEXCOM7_USART_WriteCountGet( void )
{
    return flexcom7UsartObj.txProcessedSize;
}

size_t FLEXCOM7_USART_ReadCountGet( void )
{
    return flexcom7UsartObj.rxProcessedSize;
}

