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
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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
    /**************** OSC16M IniTialization *************/
    OSCCTRL_REGS->OSCCTRL_OSC16MCTRL = OSCCTRL_OSC16MCTRL_FSEL(0x0) | OSCCTRL_OSC16MCTRL_ENABLE_Msk;
}

static void OSC32KCTRL_Initialize(void)
{
    /****************** XOSC32K initialization  ******************************/

    /* Configure 32K External Oscillator */
    OSC32KCTRL_REGS->OSC32KCTRL_XOSC32K = OSC32KCTRL_XOSC32K_STARTUP(0) | OSC32KCTRL_XOSC32K_ENABLE_Msk | OSC32KCTRL_XOSC32K_EN1K_Msk | OSC32KCTRL_XOSC32K_EN32K_Msk ;


    while(!((OSC32KCTRL_REGS->OSC32KCTRL_STATUS & OSC32KCTRL_STATUS_XOSC32KRDY_Msk) == OSC32KCTRL_STATUS_XOSC32KRDY_Msk))
    {
        /* Waiting for the XOSC32K Ready state */
    }
	OSC32KCTRL_REGS->OSC32KCTRL_RTCCTRL = OSC32KCTRL_RTCCTRL_RTCSEL(0);
}

static void DFLL48M_Initialize(void)
{
    /****************** DFLL Initialization  *********************************/
    OSCCTRL_REGS->OSCCTRL_DFLLCTRL = 0 ;

    while((OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_DFLLRDY_Msk) != OSCCTRL_STATUS_DFLLRDY_Msk)
    {
        /* Waiting for the Ready state */
    }

    /*Load Calibration Value*/
    uint8_t calibCoarse = (uint8_t)(((*(uint32_t*)0x806020) >> 25 ) & 0x3f);

    OSCCTRL_REGS->OSCCTRL_DFLLVAL = OSCCTRL_DFLLVAL_COARSE(calibCoarse) | OSCCTRL_DFLLVAL_FINE(512);

    OSCCTRL_REGS->OSCCTRL_DFLLCTRL = 0 ;

    while((OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_DFLLRDY_Msk) != OSCCTRL_STATUS_DFLLRDY_Msk)
    {
        /* Waiting for the Ready state */
    }

    /* Configure DFLL    */
    OSCCTRL_REGS->OSCCTRL_DFLLCTRL = OSCCTRL_DFLLCTRL_ENABLE_Msk ;

    while((OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_DFLLRDY_Msk) != OSCCTRL_STATUS_DFLLRDY_Msk)
    {
        /* Waiting for DFLL to be ready */
    }
}




static void GCLK0_Initialize(void)
{
    
    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_SRC(7) | GCLK_GENCTRL_GENEN_Msk;

    while((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL0_Msk) == GCLK_SYNCBUSY_GENCTRL0_Msk)
    {
        /* wait for the Generator 0 synchronization */
    }
}


static void GCLK1_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL[1] = GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_SRC(5) | GCLK_GENCTRL_GENEN_Msk;

    while((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL1_Msk) == GCLK_SYNCBUSY_GENCTRL1_Msk)
    {
        /* wait for the Generator 1 synchronization */
    }
}


static void GCLK2_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL[2] = GCLK_GENCTRL_DIV(1) | GCLK_GENCTRL_SRC(4) | GCLK_GENCTRL_GENEN_Msk;

    while((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL2_Msk) == GCLK_SYNCBUSY_GENCTRL2_Msk)
    {
        /* wait for the Generator 2 synchronization */
    }
}

void CLOCK_Initialize (void)
{
    /* Function to Initialize the Oscillators */
    OSCCTRL_Initialize();

    /* Function to Initialize the 32KHz Oscillators */
    OSC32KCTRL_Initialize();

	SUPC_REGS->SUPC_VREGPLL = SUPC_VREGPLL_ENABLE_Msk;
    DFLL48M_Initialize();
    GCLK1_Initialize();
    GCLK2_Initialize();
    GCLK0_Initialize();



	/* Selection of the Generator and write Lock for USB */
    GCLK_REGS->GCLK_PCHCTRL[7] = GCLK_PCHCTRL_GEN(0x0)  | GCLK_PCHCTRL_CHEN_Msk;

    while ((GCLK_REGS->GCLK_PCHCTRL[7] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk)
    {
        /* Wait for synchronization */
    }
	/* Selection of the Generator and write Lock for TC0 TC1 */
    GCLK_REGS->GCLK_PCHCTRL[23] = GCLK_PCHCTRL_GEN(0x0)  | GCLK_PCHCTRL_CHEN_Msk;

    while ((GCLK_REGS->GCLK_PCHCTRL[23] & GCLK_PCHCTRL_CHEN_Msk) != GCLK_PCHCTRL_CHEN_Msk)
    {
        /* Wait for synchronization */
    }


}