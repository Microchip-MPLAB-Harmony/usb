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

/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"
#include <string.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/*** Application Instance 0 Configuration ***/

#define APP_READ_BUFFER_SIZE 512

/* Macro defines USB internal DMA Buffer criteria*/

#define APP_MAKE_BUFFER_DMA_READY  __attribute__((aligned(16)))
    
    
// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_STATE_INIT=0,

    /* Application waits for device configuration */
    APP_STATE_WAIT_FOR_CONFIGURATION,

    /* Check if we got data from CDC */
    APP_STATE_SERIAL_EMULATOR,

    /* Application error occurred */
    APP_STATE_ERROR

} APP_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
   /* Device layer handle returned by device layer open function */
    USB_DEVICE_HANDLE usbDevHandle;

    /* Application CDC Instance */
    USB_DEVICE_CDC_INDEX cdcInstance;

    /* Application USART Driver handle */
    DRV_HANDLE usartHandle;
    /* Application state*/
    APP_STATES state;

    /* Track device configuration */
    bool deviceIsConfigured;

    /* Read Data Buffer */
    uint8_t * cdcReadBuffer;

    /* Read Data Buffer */
    uint8_t * uartReadBuffer;

    /* Set Line Coding Data */
    USB_CDC_LINE_CODING setLineCodingData;

    /* Get Line Coding Data */
    USB_CDC_LINE_CODING getLineCodingData;

    /* Control Line State */
    USB_CDC_CONTROL_LINE_STATE controlLineStateData;

    /* Break data */
    uint16_t breakData;

    /* Read transfer handle */
    USB_DEVICE_CDC_TRANSFER_HANDLE readTransferHandle;

    /* Write transfer handle */
    USB_DEVICE_CDC_TRANSFER_HANDLE writeTransferHandle;

    /* True if a character was read */
    bool isCDCReadInProgress;

    /* True if a character was written*/
    bool isCDCWriteInProgress;

    /* True if a character was read */
    bool isUSARTReadInProgress;

    /* True if a character was written*/
    bool isUSARTWriteInProgress;
    
    /* To indicate the transfer completion status */
    volatile bool completeStatus;
    
    /* To indicate the error status */
    volatile bool errorStatus;
    
    /* USART Buffer Handler */
    DRV_USART_BUFFER_HANDLE readBufferHandler;
    
    /* USART Buffer Handler */
    DRV_USART_BUFFER_HANDLE writeBufferHandler;

    /* UART1 received data */
    uint8_t * uartReceivedData;

    /* Read Buffer Length*/
    size_t readLength;

    /* Current UART TX Count*/
    size_t uartTxCount;

    /* This flag is uses to check if Host has requested to change baudrate*/
    bool isBaudrateDataReceived;

    /* This flag used to check if the Set Line coding command is in progress */
    bool isSetLineCodingCommandInProgress;

    /* Configuration value */
    uint8_t configValue;

    /* speed */
    USB_SPEED speed;

    /* ep data sent */
    bool epDataWritePending;

    /* ep data received */
    bool epDataReadPending;

    /* Transfer handle */
    USB_DEVICE_TRANSFER_HANDLE writeTranferHandle;

    /* Transfer handle */
    USB_DEVICE_TRANSFER_HANDLE readTranferHandle;

    /* The transmit endpoint address */
    USB_ENDPOINT_ADDRESS endpointTx;

    /* The receive endpoint address */
    USB_ENDPOINT_ADDRESS endpointRx;

    /* Tracks the alternate setting */
    uint8_t altSetting;


} APP_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );



#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

