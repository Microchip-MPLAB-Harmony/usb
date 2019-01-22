 /*******************************************************************************
  USB HOST HID Mouse local definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hid_mouse_local.h

  Summary:
    USB HOST HID Mouse local definitions

  Description:
    This file describes the local HID Mouse usage definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef _USB_HOST_HID_MOUSE_LOCAL_H
#define _USB_HOST_HID_MOUSE_LOCAL_H




// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "usb/usb_host_hid_mouse.h"

// *****************************************************************************
/* USB HOST HID Mouse Driver State

  Summary:
    USB HOST HID Mouse Driver State.

  Description:
    This enumeration defines the possible task state of USB HOST HID Mouse
    driver.

  Remarks:
    None.
*/

typedef enum
{
    USB_HOST_HID_MOUSE_DETACHED = 0,
    USB_HOST_HID_MOUSE_ATTACHED,
    USB_HOST_HID_MOUSE_REPORT_PROCESS
    
}USB_HOST_HID_MOUSE_STATE;

// *****************************************************************************
/* USB HOST HID Mouse Driver data structure

  Summary:
    USB HOST HID Mouse Driver information

  Description:
    This structure holds all information on per HID Mouse driver instance level.
    Contains information like driver state, buffer information, ping pong states.
    Structure is used as part of parsing once report is received from mouse.

  Remarks:
    None.
 */

typedef struct
{
    bool inUse;
    bool isPongReportProcessing;
    bool isPingReportProcessing;
    bool nextPingPong;
    bool taskPingPong;
    int8_t dataPing[64];
    int8_t dataPong[64];
    USB_HOST_HID_MOUSE_STATE state;
    USB_HOST_HID_OBJ_HANDLE handle;
    USB_HOST_HID_MOUSE_DATA appData;
} USB_HOST_HID_MOUSE_DATA_OBJ;

#endif

