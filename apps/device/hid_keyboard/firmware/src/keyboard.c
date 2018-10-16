/*******************************************************************************
   Keyboard Driver

  Company:
    Microchip Technology Inc.

  FileName:
    keyboard.c

  Summary:
    This file contains keyboard routines 

  Description:
    This file contains keyboard routines.
*******************************************************************************/

/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/

#include "keyboard.h"
#include "usb/usb_device_hid.h"
#include "usb/usb_device.h"
#include "configuration.h"

void KEYBOARD_InputReportCreate
(
    KEYBOARD_KEYCODE_ARRAY * keyboardKeycodeArray,
    KEYBOARD_MODIFIER_KEYS * keyboardModifierKeys,
    KEYBOARD_INPUT_REPORT * keyboardInputReport
)
{
    int index;

    for (index = 0; index < 6 ; index ++)
    {
        /* Create the keyboard button bit map */
        keyboardInputReport->data[index + 2] = keyboardKeycodeArray->keyCode[index];
    }

    /* Update the modifier key */
    keyboardInputReport->data[0] = keyboardModifierKeys->modifierkeys;

    return;

}

