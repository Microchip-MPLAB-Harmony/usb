/*******************************************************************************
  UART0 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_uart0.c

  Summary:
    UART0 PLIB Implementation File

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
#include "plib_uart0.h"

// *****************************************************************************
// *****************************************************************************
// Section: UART0 Implementation
// *****************************************************************************
// *****************************************************************************

UART_OBJECT uart0Obj;

void static UART0_ISR_RX_Handler( void )
{
    if(uart0Obj.rxBusyStatus == true)
    {
        while((UART_SR_RXRDY_Msk == (UART0_REGS->UART_SR& UART_SR_RXRDY_Msk)) && (uart0Obj.rxSize > uart0Obj.rxProcessedSize) )
        {
            uart0Obj.rxBuffer[uart0Obj.rxProcessedSize++] = (UART0_REGS->UART_RHR& UART_RHR_RXCHR_Msk);
        }

        /* Check if the buffer is done */
        if(uart0Obj.rxProcessedSize >= uart0Obj.rxSize)
        {
            uart0Obj.rxBusyStatus = false;

            /* Disable Read, Overrun, Parity and Framing error interrupts */
            UART0_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

            if(uart0Obj.rxCallback != NULL)
            {
                uart0Obj.rxCallback(uart0Obj.rxContext);
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

void static UART0_ISR_TX_Handler( void )
{
    if(uart0Obj.txBusyStatus == true)
    {
        while((UART_SR_TXEMPTY_Msk == (UART0_REGS->UART_SR& UART_SR_TXEMPTY_Msk)) && (uart0Obj.txSize > uart0Obj.txProcessedSize) )
        {
            UART0_REGS->UART_THR|= uart0Obj.txBuffer[uart0Obj.txProcessedSize++];
        }

        /* Check if the buffer is done */
        if(uart0Obj.txProcessedSize >= uart0Obj.txSize)
        {
            uart0Obj.txBusyStatus = false;
            UART0_REGS->UART_IDR = UART_IDR_TXEMPTY_Msk;

            if(uart0Obj.txCallback != NULL)
            {
                uart0Obj.txCallback(uart0Obj.txContext);
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

void UART0_InterruptHandler( void )
{
    /* Error status */
    uint32_t errorStatus = (UART0_REGS->UART_SR & (UART_SR_OVRE_Msk | UART_SR_FRAME_Msk | UART_SR_PARE_Msk));

    if(errorStatus != 0)
    {
        /* Client must call UARTx_ErrorGet() function to clear the errors */

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART0_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        uart0Obj.rxBusyStatus = false;

        /* UART errors are normally associated with the receiver, hence calling
         * receiver callback */
        if( uart0Obj.rxCallback != NULL )
        {
            uart0Obj.rxCallback(uart0Obj.rxContext);
        }
    }

    /* Receiver status */
    if(UART_SR_RXRDY_Msk == (UART0_REGS->UART_SR& UART_SR_RXRDY_Msk))
    {
        UART0_ISR_RX_Handler();
    }

    /* Transmitter status */
    if(UART_SR_TXEMPTY_Msk == (UART0_REGS->UART_SR& UART_SR_TXEMPTY_Msk))
    {
        UART0_ISR_TX_Handler();
    }

    return;
}


void static UART0_ErrorClear( void )
{
    uint8_t dummyData = 0u;

    UART0_REGS->UART_CR|= UART_CR_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( UART_SR_RXRDY_Msk == (UART0_REGS->UART_SR& UART_SR_RXRDY_Msk) )
    {
        dummyData = (UART0_REGS->UART_RHR& UART_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;

    return;
}

void UART0_Initialize( void )
{
    /* Reset UART0 */
    UART0_REGS->UART_CR = (UART_CR_RSTRX_Msk | UART_CR_RSTTX_Msk | UART_CR_RSTSTA_Msk);

    /* Enable UART0 */
    UART0_REGS->UART_CR = (UART_CR_TXEN_Msk | UART_CR_RXEN_Msk);

    /* Configure UART0 mode */
    UART0_REGS->UART_MR = ((UART_MR_BRSRCCK_PERIPH_CLK) | (UART_MR_PAR_NO) | (0 << UART_MR_FILTER_Pos));

    /* Configure UART0 Baud Rate */
    UART0_REGS->UART_BRGR = UART_BRGR_CD(81);

    /* Initialize instance object */
    uart0Obj.rxBuffer = NULL;
    uart0Obj.rxSize = 0;
    uart0Obj.rxProcessedSize = 0;
    uart0Obj.rxBusyStatus = false;
    uart0Obj.rxCallback = NULL;
    uart0Obj.txBuffer = NULL;
    uart0Obj.txSize = 0;
    uart0Obj.txProcessedSize = 0;
    uart0Obj.txBusyStatus = false;
    uart0Obj.txCallback = NULL;

    return;
}

UART_ERROR UART0_ErrorGet( void )
{
    UART_ERROR errors = UART_ERROR_NONE;
    uint32_t status = UART0_REGS->UART_SR;

    errors = (UART_ERROR)(status & (UART_SR_OVRE_Msk | UART_SR_PARE_Msk | UART_SR_FRAME_Msk));

    if(errors != UART_ERROR_NONE)
    {
        UART0_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool UART0_SerialSetup( UART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    bool status = false;
    uint32_t baud = setup->baudRate;
    uint32_t brgVal = 0;
    uint32_t uartMode;

    if((uart0Obj.rxBusyStatus == true) || (uart0Obj.txBusyStatus == true))
    {
        /* Transaction is in progress, so return without updating settings */
        return false;
    }
    if (setup != NULL)
    {
        if(srcClkFreq == 0)
        {
            srcClkFreq = UART0_FrequencyGet();
        }

        /* Calculate BRG value */
        brgVal = srcClkFreq / (16 * baud);

        /* Configure UART0 mode */
        uartMode = UART0_REGS->UART_MR;
        uartMode &= ~UART_MR_PAR_Msk;
        UART0_REGS->UART_MR = uartMode | setup->parity ;

        /* Configure UART0 Baud Rate */
        UART0_REGS->UART_BRGR = UART_BRGR_CD(brgVal);

        status = true;
    }

    return status;
}

bool UART0_Read( void *buffer, const size_t size )
{
    bool status = false;

    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Clear errors before submitting the request.
         * ErrorGet clears errors internally. */
        UART0_ErrorGet();

        /* Check if receive request is in progress */
        if(uart0Obj.rxBusyStatus == false)
        {
            uart0Obj.rxBuffer = lBuffer;
            uart0Obj.rxSize = size;
            uart0Obj.rxProcessedSize = 0;
            uart0Obj.rxBusyStatus = true;
            status = true;

            /* Enable Read, Overrun, Parity and Framing error interrupts */
            UART0_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);
        }
    }

    return status;
}

bool UART0_Write( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Check if transmit request is in progress */
        if(uart0Obj.txBusyStatus == false)
        {
            uart0Obj.txBuffer = lBuffer;
            uart0Obj.txSize = size;
            uart0Obj.txProcessedSize = 0;
            uart0Obj.txBusyStatus = true;
            status = true;

            /* Initiate the transfer by sending first byte */
            if(UART_SR_TXEMPTY_Msk == (UART0_REGS->UART_SR& UART_SR_TXEMPTY_Msk))
            {
                UART0_REGS->UART_THR = (UART_THR_TXCHR(*lBuffer) & UART_THR_TXCHR_Msk);
                uart0Obj.txProcessedSize++;
            }

            UART0_REGS->UART_IER = UART_IER_TXEMPTY_Msk;
        }
    }

    return status;
}

void UART0_WriteCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart0Obj.txCallback = callback;

    uart0Obj.txContext = context;
}

void UART0_ReadCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart0Obj.rxCallback = callback;

    uart0Obj.rxContext = context;
}

bool UART0_WriteIsBusy( void )
{
    return uart0Obj.txBusyStatus;
}

bool UART0_ReadIsBusy( void )
{
    return uart0Obj.rxBusyStatus;
}

size_t UART0_WriteCountGet( void )
{
    return uart0Obj.txProcessedSize;
}

size_t UART0_ReadCountGet( void )
{
    return uart0Obj.rxProcessedSize;
}

