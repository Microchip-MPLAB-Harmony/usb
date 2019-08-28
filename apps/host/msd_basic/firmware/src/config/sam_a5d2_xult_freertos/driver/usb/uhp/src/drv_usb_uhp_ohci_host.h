/*******************************************************************************
  USB Driver OHCI Data Structures

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usb_uhp_ehci_host.h

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

extern void _DRV_USB_UHP_HOST_Tasks_ISR_OHCI(DRV_USB_UHP_OBJ *hDriver);
extern void _DRV_USB_UHP_HOST_OhciInit(DRV_USB_UHP_OBJ *drvObj);
extern void _DRV_USB_UHP_HOST_DisableControlList_OHCI(DRV_USB_UHP_OBJ *hDriver);
extern void ohci_received_size( uint32_t * BuffSize );

#endif  /* _DRV_USB_UHP_OHCI_H */
