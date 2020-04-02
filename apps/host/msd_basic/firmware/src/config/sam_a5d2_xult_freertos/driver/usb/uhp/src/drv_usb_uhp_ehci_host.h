/*******************************************************************************
  USB Driver EHCI Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_uhp_ehci_host.h

  Summary:
    USB driver EHCI declarations and definitions

  Description:
    This file contains the USB driver's EHCI declarations and definitions.
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

#ifndef _DRV_USB_UHP_EHCI_H
#define _DRV_USB_UHP_EHCI_H


// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#ifndef UHP_EHCI_RAM_ADDR
#define UHP_EHCI_RAM_ADDR                            0xA0100000u
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************

extern void DRV_USB_UHP_EHCI_HOST_ResetEnable(DRV_USB_UHP_OBJ *hDriver);
extern void DRV_USB_UHP_EHCI_HOST_ReceivedSize( uint32_t * BuffSize );
extern void DRV_USB_UHP_EHCI_HOST_Init(DRV_USB_UHP_OBJ *drvObj);
extern void DRV_USB_UHP_EHCI_HOST_DisableAsynchronousList(DRV_USB_UHP_OBJ *hDriver);
extern void DRV_USB_UHP_EHCI_HOST_Tasks_ISR(DRV_USB_UHP_OBJ *hDriver);
extern DRV_USB_UHP_HOST_PIPE_HANDLE DRV_USB_UHP_EHCI_PipeSetup
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
);
extern void DRV_USB_UHP_EHCI_PipeClose(DRV_USB_UHP_HOST_PIPE_HANDLE pipeHandle);

#endif  // _DRV_USB_UHP_EHCI_H
