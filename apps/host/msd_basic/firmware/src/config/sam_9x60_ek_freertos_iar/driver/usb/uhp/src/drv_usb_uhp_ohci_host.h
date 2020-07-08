/*******************************************************************************
  USB Driver OHCI Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_uhp_ohci_host.h

  Summary:
    USB driver OHCI declarations and definitions

  Description:
    This file contains the USB driver's OHCI declarations and definitions.
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

#ifndef _DRV_USB_UHP_OHCI_H
#define _DRV_USB_UHP_OHCI_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#ifndef UHP_OHCI_RAM_ADDR
#define UHP_OHCI_RAM_ADDR                            0xA0100000u
#endif

/*********************************************
 * 6.2 USB States
 *********************************************/
typedef enum
{
    DRV_USB_UHP_HOST_OHCI_STATE_USBRESET = 0,
    /* While the Host Controller is in the USBRESUME state, it is asserting resume signaling on the USB */
    DRV_USB_UHP_HOST_OHCI_STATE_USBRESUME = 1,
    /* Host Controller may process lists and will generate SOF Tokens. */
    DRV_USB_UHP_HOST_OHCI_STATE_USBOPERATIONAL = 2,
    /* Host Controller is not generating SOF tokens on the USB */
    DRV_USB_UHP_HOST_OHCI_STATE_USBSUSPEND = 3
} DRV_USB_UHP_HOST_OHCI_STATE;

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

extern uint32_t DRV_USB_UHP_OHCI_HOST_EDIsFinish( uint8_t hostPipeInUse );
extern void DRV_USB_UHP_OHCI_HOST_ReceivedSize( uint32_t * BuffSize );
extern void DRV_USB_UHP_OHCI_HOST_Init(DRV_USB_UHP_OBJ *drvObj);
extern void DRV_USB_UHP_OHCI_HOST_Init_simpl(DRV_USB_UHP_OBJ *drvObj);
extern void DRV_USB_UHP_OHCI_HOST_DisableControlList(DRV_USB_UHP_OBJ *hDriver);
extern void DRV_USB_UHP_OHCI_HOST_DisableBulkList(DRV_USB_UHP_OBJ *hDriver);
extern void DRV_USB_UHP_OHCI_HOST_Tasks_ISR(DRV_USB_UHP_OBJ *hDriver);
// ****************************************************************************
/* Function:
    DRV_USB_UHP_HOST_PIPE_HANDLE DRV_USB_UHP_OHCI_PipeSetup
    (
        DRV_HANDLE client,
        uint8_t deviceAddress,
        USB_ENDPOINT endpointAndDirection,
        uint8_t hubAddress,
        uint8_t hubPort,
        USB_TRANSFER_TYPE pipeType,
        uint8_t bInterval,
        uint16_t wMaxPacketSize,
        USB_SPEED speed
    );

  Summary:
    Open a pipe with the specified attributes.

  Description:
    This function opens a communication pipe between the Host and the device
    endpoint. The transfer type and other attributes are specified through the
    function parameters. The driver does not check for available bus bandwidth,
    which should be done by the application (the USB Host Layer in this case)

  Precondition:
    The driver handle should be valid.

  Parameters:
    client - Handle to the driver (returned from DRV_USB_UHP_Open function).

    deviceAddress - USB Address of the device to connect to.

    endpoint - Endpoint on the device to connect to.

    hubAddress - Address of the hub to which this device is connected. If not
    connected to a hub, this value should be set to 0.

    hubPort - Port number of the hub to which this device is connected.

    pipeType - Transfer type of the pipe to open.

    bInterval - Polling interval for periodic transfers. This should be
    specified as defined by the USB 2.0 Specification.

    wMaxPacketSize - This should be set to the endpoint size reported by the
    device in its configuration descriptors. This defines the maximum size of
    the transaction in a transfer on this pipe.

    speed - The speed of the pipe. This should match the speed at which the
    device connected to the Host.

  Returns:
    * DRV_USB_HOST_PIPE_HANDLE_INVALID - The pipe could not be created.
    * A valid Pipe Handle - The pipe was created successfully. This is an
      arbitrary value and will never be the same as
      DRV_USB_HOST_PIPE_HANDLE_INVALID.

  Example:
    <code>
    // This code shows how the DRV_USB_UHP_PipeSetup function is called for
    // create a communication pipe. In this example, Bulk pipe is created
    // between the Host and a device. The Device address is 2 and the target
    // endpoint on this device is 4 . The direction of the data transfer over
    // this pipe is from the Host to the device. The device is connected to Port
    // 1 of a Hub, whose USB address is 3. The maximum size of a transaction
    // on this pipe is 64 bytes. This is a Bulk Pipe and hence the bInterval
    // field is set to 0. The target device is operating at Full Speed.

    DRV_HANDLE driverHandle;
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle;

    pipeHandle = DRV_USB_UHP_PipeSetup(driverHandle, 0x02, 0x14, 0x03, 0x01, USB_TRANSFER_TYPE_BULK, 0, 64, USB_SPEED_FULL);

    if(pipeHandle != DRV_USB_UHP_HOST_PIPE_HANDLE_INVALID)
    {
        // The pipe was created successfully.
    }

    </code>

  Remarks:
    None.
*/

extern DRV_USB_UHP_HOST_PIPE_HANDLE DRV_USB_UHP_OHCI_PipeSetup
(
    DRV_HANDLE client,
    uint8_t deviceAddress,
    USB_ENDPOINT endpointAndDirection,
    uint8_t hubAddress,
    uint8_t hubPort,
    USB_TRANSFER_TYPE pipeType,
    uint8_t bInterval,
    uint16_t wMaxPacketSize,
    USB_SPEED speed
);

// ****************************************************************************
/* Function:
    void DRV_USB_UHP_OHCI_PipeClose
    (
        DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
    );

  Summary:
    Closes an open pipe.

  Description:
    This function closes an open pipe. Any IRPs scheduled on the pipe will be
    aborted and IRP callback functions will be called with the status as
    DRV_USB_HOST_IRP_STATE_ABORTED. The pipe handle will become invalid and the
    pipe and will not accept IRPs.

  Precondition:
    The pipe handle should be valid.

  Parameters:
    pipeHandle - Handle to the pipe to close.

  Returns:
    None.

  Example:
    <code>
    // This code shows how an open Host pipe can be closed.

    DRV_HANDLE driverHandle;
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle;

    // Close the pipe.
    DRV_USB_UHP_PipeClose(pipeHandle);
    </code>

  Remarks:
    None.
*/

extern void DRV_USB_UHP_OHCI_PipeClose
(
    DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle
);


#endif  /* _DRV_USB_UHP_OHCI_H */
