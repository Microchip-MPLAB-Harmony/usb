/*******************************************************************************
  UART3 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_uart3.c

  Summary:
    UART3 PLIB Implementation File

  Description:
    None

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

#include "device.h"
#include "plib_uart3.h"

// *****************************************************************************
// *****************************************************************************
// Section: UART3 Implementation
// *****************************************************************************
// *****************************************************************************

UART_OBJECT uart3Obj;

void static UART3_ISR_RX_Handler( void )
{
    if(uart3Obj.rxBusyStatus == true)
    {
        while((UART_SR_RXRDY_Msk == (UART3_REGS->UART_SR& UART_SR_RXRDY_Msk)) && (uart3Obj.rxSize > uart3Obj.rxProcessedSize) )
        {
            uart3Obj.rxBuffer[uart3Obj.rxProcessedSize++] = (UART3_REGS->UART_RHR& UART_RHR_RXCHR_Msk);
        }

        /* Check if the buffer is done */
        if(uart3Obj.rxProcessedSize >= uart3Obj.rxSize)
        {
            uart3Obj.rxBusyStatus = false;

            /* Disable Read, Overrun, Parity and Framing error interrupts */
            UART3_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

            if(uart3Obj.rxCallback != NULL)
            {
                uart3Obj.rxCallback(uart3Obj.rxContext);
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

void static UART3_ISR_TX_Handler( void )
{
    if(uart3Obj.txBusyStatus == true)
    {
        while((UART_SR_TXEMPTY_Msk == (UART3_REGS->UART_SR& UART_SR_TXEMPTY_Msk)) && (uart3Obj.txSize > uart3Obj.txProcessedSize) )
        {
            UART3_REGS->UART_THR|= uart3Obj.txBuffer[uart3Obj.txProcessedSize++];
        }

        /* Check if the buffer is done */
        if(uart3Obj.txProcessedSize >= uart3Obj.txSize)
        {
            uart3Obj.txBusyStatus = false;
            UART3_REGS->UART_IDR = UART_IDR_TXEMPTY_Msk;

            if(uart3Obj.txCallback != NULL)
            {
                uart3Obj.txCallback(uart3Obj.txContext);
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

void UART3_InterruptHandler( void )
{
    /* Error status */
    uint32_t errorStatus = (UART3_REGS->UART_SR & (UART_SR_OVRE_Msk | UART_SR_FRAME_Msk | UART_SR_PARE_Msk));

    if(errorStatus != 0)
    {
        /* Client must call UARTx_ErrorGet() function to clear the errors */

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART3_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        uart3Obj.rxBusyStatus = false;

        /* UART errors are normally associated with the receiver, hence calling
         * receiver callback */
        if( uart3Obj.rxCallback != NULL )
        {
            uart3Obj.rxCallback(uart3Obj.rxContext);
        }
    }

    /* Receiver status */
    if(UART_SR_RXRDY_Msk == (UART3_REGS->UART_SR& UART_SR_RXRDY_Msk))
    {
        UART3_ISR_RX_Handler();
    }

    /* Transmitter status */
    if(UART_SR_TXEMPTY_Msk == (UART3_REGS->UART_SR& UART_SR_TXEMPTY_Msk))
    {
        UART3_ISR_TX_Handler();
    }

    return;
}


void static UART3_ErrorClear( void )
{
    uint8_t dummyData = 0u;

    UART3_REGS->UART_CR|= UART_CR_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( UART_SR_RXRDY_Msk == (UART3_REGS->UART_SR& UART_SR_RXRDY_Msk) )
    {
        dummyData = (UART3_REGS->UART_RHR& UART_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;

    return;
}

void UART3_Initialize( void )
{
    /* Reset UART3 */
    UART3_REGS->UART_CR = (UART_CR_RSTRX_Msk | UART_CR_RSTTX_Msk | UART_CR_RSTSTA_Msk);

    /* Enable UART3 */
    UART3_REGS->UART_CR = (UART_CR_TXEN_Msk | UART_CR_RXEN_Msk);

    /* Configure UART3 mode */
    UART3_REGS->UART_MR = ((UART_MR_BRSRCCK_PERIPH_CLK) | (UART_MR_PAR_NO) | (0 << UART_MR_FILTER_Pos));

    /* Configure UART3 Baud Rate */
    UART3_REGS->UART_BRGR = UART_BRGR_CD(976);

    /* Initialize instance object */
    uart3Obj.rxBuffer = NULL;
    uart3Obj.rxSize = 0;
    uart3Obj.rxProcessedSize = 0;
    uart3Obj.rxBusyStatus = false;
    uart3Obj.rxCallback = NULL;
    uart3Obj.txBuffer = NULL;
    uart3Obj.txSize = 0;
    uart3Obj.txProcessedSize = 0;
    uart3Obj.txBusyStatus = false;
    uart3Obj.txCallback = NULL;

    return;
}

UART_ERROR UART3_ErrorGet( void )
{
    UART_ERROR errors = UART_ERROR_NONE;
    uint32_t status = UART3_REGS->UART_SR;

    errors = (UART_ERROR)(status & (UART_SR_OVRE_Msk | UART_SR_PARE_Msk | UART_SR_FRAME_Msk));

    if(errors != UART_ERROR_NONE)
    {
        UART3_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool UART3_SerialSetup( UART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    bool status = false;
    uint32_t baud = setup->baudRate;
    uint32_t brgVal = 0;
    uint32_t uartMode;

    if((uart3Obj.rxBusyStatus == true) || (uart3Obj.txBusyStatus == true))
    {
        /* Transaction is in progress, so return without updating settings */
        return false;
    }
    if (setup != NULL)
    {
        if(srcClkFreq == 0)
        {
            srcClkFreq = UART3_FrequencyGet();
        }

        /* Calculate BRG value */
        brgVal = srcClkFreq / (16 * baud);

        /* Configure UART3 mode */
        uartMode = UART3_REGS->UART_MR;
        uartMode &= ~UART_MR_PAR_Msk;
        UART3_REGS->UART_MR = uartMode | setup->parity ;

        /* Configure UART3 Baud Rate */
        UART3_REGS->UART_BRGR = UART_BRGR_CD(brgVal);

        status = true;
    }

    return status;
}

bool UART3_Read( void *buffer, const size_t size )
{
    bool status = false;

    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Clear errors before submitting the request.
         * ErrorGet clears errors internally. */
        UART3_ErrorGet();

        /* Check if receive request is in progress */
        if(uart3Obj.rxBusyStatus == false)
        {
            uart3Obj.rxBuffer = lBuffer;
            uart3Obj.rxSize = size;
            uart3Obj.rxProcessedSize = 0;
            uart3Obj.rxBusyStatus = true;
            status = true;

            /* Enable Read, Overrun, Parity and Framing error interrupts */
            UART3_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);
        }
    }

    return status;
}

bool UART3_Write( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Check if transmit request is in progress */
        if(uart3Obj.txBusyStatus == false)
        {
            uart3Obj.txBuffer = lBuffer;
            uart3Obj.txSize = size;
            uart3Obj.txProcessedSize = 0;
            uart3Obj.txBusyStatus = true;
            status = true;

            /* Initiate the transfer by sending first byte */
            if(UART_SR_TXEMPTY_Msk == (UART3_REGS->UART_SR& UART_SR_TXEMPTY_Msk))
            {
                UART3_REGS->UART_THR = (UART_THR_TXCHR(*lBuffer) & UART_THR_TXCHR_Msk);
                uart3Obj.txProcessedSize++;
            }

            UART3_REGS->UART_IER = UART_IER_TXEMPTY_Msk;
        }
    }

    return status;
}

void UART3_WriteCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart3Obj.txCallback = callback;

    uart3Obj.txContext = context;
}

void UART3_ReadCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart3Obj.rxCallback = callback;

    uart3Obj.rxContext = context;
}

bool UART3_WriteIsBusy( void )
{
    return uart3Obj.txBusyStatus;
}

bool UART3_ReadIsBusy( void )
{
    return uart3Obj.rxBusyStatus;
}

size_t UART3_WriteCountGet( void )
{
    return uart3Obj.txProcessedSize;
}

size_t UART3_ReadCountGet( void )
{
    return uart3Obj.rxProcessedSize;
}

