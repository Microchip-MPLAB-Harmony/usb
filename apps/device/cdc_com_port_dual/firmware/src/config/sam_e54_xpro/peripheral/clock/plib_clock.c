/*******************************************************************************
 CLOCK PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_clock.c

  Summary:
    CLOCK PLIB Implementation File.

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

#include "plib_clock.h"
#include "device.h"

static void OSCCTRL_Initialize(void)
{

}

static void OSC32KCTRL_Initialize(void)
{

    OSC32KCTRL_REGS->OSC32KCTRL_RTCCTRL = OSC32KCTRL_RTCCTRL_RTCSEL(0);
}



static void DFLL_Initialize(void)
{
}


static void GCLK0_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_SRC(6) | GCLK_GENCTRL_GENEN_Msk;

    while((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK0) == GCLK_SYNCBUSY_GENCTRL_GCLK0)
    {
        /* wait for the Generator 0 synchronization */
    }
}

void CLOCK_Initialize (void)
{
    /* NVM Wait States */
    NVMCTRL_REGS->NVMCTRL_CTRLA |= NVMCTRL_CTRLA_RWS(1);

    /* Function to Initialize the Oscillators */
    OSCCTRL_Initialize();

    /* Function to Initialize the 32KHz Oscillators */
    OSC32KCTRL_Initialize();

   GCLK0_Initialize();
   DFLL_Initialize();

    /* selection of the CPU clock Division */
    MCLK_REGS->MCLK_CPUDIV = MCLK_CPUDIV_DIV(0x01);

    while((MCLK_REGS->MCLK_INTFLAG & MCLK_INTFLAG_CKRDY_Msk) != MCLK_INTFLAG_CKRDY_Msk)
    {
        /* Wait for the Main Clock to be Ready */
    }

    /* Selection of the Generator and write Lock for USB */
    GCLK_REGS->GCLK_PCHCTRL[10] = GCLK_PCHCTRL_GEN(0x0)  | GCLK_PCHCTRL_CHEN_Msk;

    while ((GCLK_REGS->GCLK_PCHCTRL[10] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk)
    {
        /* Wait for synchronization */
    }

    /* Configure the AHB Bridge Clocks */
    MCLK_REGS->MCLK_AHBMASK = 0xffffff;

    /* Configure the APBA Bridge Clocks */
    MCLK_REGS->MCLK_APBAMASK = 0x7ff;

    /* Configure the APBB Bridge Clocks */
    MCLK_REGS->MCLK_APBBMASK = 0x18057;

}
