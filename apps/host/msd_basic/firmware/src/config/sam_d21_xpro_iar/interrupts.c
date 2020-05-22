/*******************************************************************************
 System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt.c

  Summary:
    Interrupt vectors mapping

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************

#pragma section="CSTACK"

void Dummy_Handler( void )
{
    while(1)
    {

    }
}
/* Device vectors list dummy definition*/
void Reset_Handler ( void );
#pragma weak Reset_Handler=Dummy_Handler
void NonMaskableInt_Handler ( void );
#pragma weak NonMaskableInt_Handler=Dummy_Handler
void HardFault_Handler ( void );
#pragma weak HardFault_Handler=Dummy_Handler
void SVCall_Handler ( void );
#pragma weak SVCall_Handler=Dummy_Handler
void PendSV_Handler ( void );
#pragma weak PendSV_Handler=Dummy_Handler
void SysTick_Handler ( void );
#pragma weak SysTick_Handler=Dummy_Handler
void PM_Handler ( void );
#pragma weak PM_Handler=Dummy_Handler
void SYSCTRL_Handler ( void );
#pragma weak SYSCTRL_Handler=Dummy_Handler
void WDT_Handler ( void );
#pragma weak WDT_Handler=Dummy_Handler
void RTC_Handler ( void );
#pragma weak RTC_Handler=Dummy_Handler
void EIC_Handler ( void );
#pragma weak EIC_Handler=Dummy_Handler
void NVMCTRL_Handler ( void );
#pragma weak NVMCTRL_Handler=Dummy_Handler
void DMAC_Handler ( void );
#pragma weak DMAC_Handler=Dummy_Handler
void DRV_USBFSV1_USB_Handler ( void );
#pragma weak DRV_USBFSV1_USB_Handler=Dummy_Handler
void EVSYS_Handler ( void );
#pragma weak EVSYS_Handler=Dummy_Handler
void SERCOM0_Handler ( void );
#pragma weak SERCOM0_Handler=Dummy_Handler
void SERCOM1_Handler ( void );
#pragma weak SERCOM1_Handler=Dummy_Handler
void SERCOM2_Handler ( void );
#pragma weak SERCOM2_Handler=Dummy_Handler
void SERCOM3_Handler ( void );
#pragma weak SERCOM3_Handler=Dummy_Handler
void SERCOM4_Handler ( void );
#pragma weak SERCOM4_Handler=Dummy_Handler
void SERCOM5_Handler ( void );
#pragma weak SERCOM5_Handler=Dummy_Handler
void TCC0_Handler ( void );
#pragma weak TCC0_Handler=Dummy_Handler
void TCC1_Handler ( void );
#pragma weak TCC1_Handler=Dummy_Handler
void TCC2_Handler ( void );
#pragma weak TCC2_Handler=Dummy_Handler
void TC3_TimerInterruptHandler ( void );
#pragma weak TC3_TimerInterruptHandler=Dummy_Handler
void TC4_Handler ( void );
#pragma weak TC4_Handler=Dummy_Handler
void TC5_Handler ( void );
#pragma weak TC5_Handler=Dummy_Handler
void TC6_Handler ( void );
#pragma weak TC6_Handler=Dummy_Handler
void TC7_Handler ( void );
#pragma weak TC7_Handler=Dummy_Handler
void ADC_Handler ( void );
#pragma weak ADC_Handler=Dummy_Handler
void AC_Handler ( void );
#pragma weak AC_Handler=Dummy_Handler
void DAC_Handler ( void );
#pragma weak DAC_Handler=Dummy_Handler
void PTC_Handler ( void );
#pragma weak PTC_Handler=Dummy_Handler
void I2S_Handler ( void );
#pragma weak I2S_Handler=Dummy_Handler



/* Mutiple handlers for vector */



#pragma location = ".intvec"
__root const DeviceVectors __vector_table=
{
    /* Configure Initial Stack Pointer, using linker-generated symbols */
    .pvStack = __sfe( "CSTACK" ),

    .pfnReset_Handler              = ( void * ) Reset_Handler,
    .pfnNonMaskableInt_Handler     = ( void * ) NonMaskableInt_Handler,
    .pfnHardFault_Handler          = ( void * ) HardFault_Handler,
    .pfnSVCall_Handler             = ( void * ) SVCall_Handler,
    .pfnPendSV_Handler             = ( void * ) PendSV_Handler,
    .pfnSysTick_Handler            = ( void * ) SysTick_Handler,
    .pfnPM_Handler                 = ( void * ) PM_Handler,
    .pfnSYSCTRL_Handler            = ( void * ) SYSCTRL_Handler,
    .pfnWDT_Handler                = ( void * ) WDT_Handler,
    .pfnRTC_Handler                = ( void * ) RTC_Handler,
    .pfnEIC_Handler                = ( void * ) EIC_Handler,
    .pfnNVMCTRL_Handler            = ( void * ) NVMCTRL_Handler,
    .pfnDMAC_Handler               = ( void * ) DMAC_Handler,
    .pfnUSB_Handler                = ( void * ) DRV_USBFSV1_USB_Handler,
    .pfnEVSYS_Handler              = ( void * ) EVSYS_Handler,
    .pfnSERCOM0_Handler            = ( void * ) SERCOM0_Handler,
    .pfnSERCOM1_Handler            = ( void * ) SERCOM1_Handler,
    .pfnSERCOM2_Handler            = ( void * ) SERCOM2_Handler,
    .pfnSERCOM3_Handler            = ( void * ) SERCOM3_Handler,
    .pfnSERCOM4_Handler            = ( void * ) SERCOM4_Handler,
    .pfnSERCOM5_Handler            = ( void * ) SERCOM5_Handler,
    .pfnTCC0_Handler               = ( void * ) TCC0_Handler,
    .pfnTCC1_Handler               = ( void * ) TCC1_Handler,
    .pfnTCC2_Handler               = ( void * ) TCC2_Handler,
    .pfnTC3_Handler                = ( void * ) TC3_TimerInterruptHandler,
    .pfnTC4_Handler                = ( void * ) TC4_Handler,
    .pfnTC5_Handler                = ( void * ) TC5_Handler,
    .pfnTC6_Handler                = ( void * ) TC6_Handler,
    .pfnTC7_Handler                = ( void * ) TC7_Handler,
    .pfnADC_Handler                = ( void * ) ADC_Handler,
    .pfnAC_Handler                 = ( void * ) AC_Handler,
    .pfnDAC_Handler                = ( void * ) DAC_Handler,
    .pfnPTC_Handler                = ( void * ) PTC_Handler,
    .pfnI2S_Handler                = ( void * ) I2S_Handler,



};

/*******************************************************************************
 End of File
*/
