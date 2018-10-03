
#include "device.h"
#include "plib_clk.h"





/*********************************************************************************
Initialize Main Clock (MAINCK)
*********************************************************************************/
static void CLK_MainClockInitialize(void)
{
    /* Enable the RC Oscillator */
    PMC_REGS->CKGR_MOR|= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN_Msk;

    /* Wait until the RC oscillator clock is ready. */
    while( (PMC_REGS->PMC_SR & PMC_SR_MOSCRCS_Msk) != PMC_SR_MOSCRCS_Msk);

    /* Configure the RC Oscillator frequency */
    PMC_REGS->CKGR_MOR = (PMC_REGS->CKGR_MOR & ~CKGR_MOR_MOSCRCF_Msk) | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCF_12_MHz;

    /* Wait until the RC oscillator clock is ready */
    while( (PMC_REGS->PMC_SR& PMC_SR_MOSCRCS_Msk) != PMC_SR_MOSCRCS_Msk);


    /* Enable Main Crystal Oscillator */
    PMC_REGS->CKGR_MOR = (PMC_REGS->CKGR_MOR & ~CKGR_MOR_MOSCXTST_Msk) | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(255) | CKGR_MOR_MOSCXTEN_Msk;

    /* Wait until the main oscillator clock is ready */
    while ( (PMC_REGS->PMC_SR & PMC_SR_MOSCXTS_Msk) != PMC_SR_MOSCXTS_Msk);

    /* Main Crystal Oscillator is selected as the Main Clock (MAINCK) source.
    Switch Main Clock (MAINCK) to Main Crystal Oscillator clock */
    PMC_REGS->CKGR_MOR|= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL_Msk;

    /* Wait until MAINCK is switched to Main Crystal Oscillator */
    while ( (PMC_REGS->PMC_SR & PMC_SR_MOSCSELS_Msk) != PMC_SR_MOSCSELS_Msk);

}

/*********************************************************************************
Initialize PLLA (PLLACK)
*********************************************************************************/

static void CLK_PLLAInitialize(void)
{
    /* Configure and Enable PLLA */
    PMC_REGS->CKGR_PLLAR = CKGR_PLLAR_ONE_Msk | CKGR_PLLAR_PLLACOUNT(0x3f) |
                              CKGR_PLLAR_MULA(25 - 1) |
                              CKGR_PLLAR_DIVA(1);

    while ( (PMC_REGS->PMC_SR & PMC_SR_LOCKA_Msk) != PMC_SR_LOCKA_Msk);
}



/*********************************************************************************
Initialize UTMI PLL  (UPLLCK)
*********************************************************************************/

static void CLK_UTMIPLLInitialize(void)
{

    /* Configure Crystal Clock Frequency (12MHz or 16MHz) to select USB PLL (UPLL) multiplication factor
       UPLL multiplication factor is x40 to generate 480MHz from 12MHz
       UPLL multiplication factor is x30 to generate 480MHz from 16MHz  */
    UTMI_REGS->UTMI_CKTRIM= UTMI_CKTRIM_FREQ_XTAL12;

    /* Enable UPLL and configure UPLL lock time */
    PMC_REGS->CKGR_UCKR = CKGR_UCKR_UPLLEN_Msk | CKGR_UCKR_UPLLCOUNT(0x3F);

    /* Wait until PLL Lock occurs */
    while ((PMC_REGS->PMC_SR & PMC_SR_LOCKU_Msk) != PMC_SR_LOCKU_Msk);

    /* UPLL clock frequency is 480MHz (Divider=1) */
    PMC_REGS->PMC_MCKR &= (~PMC_MCKR_UPLLDIV2_Msk);

    /* Wait until clock is ready */
    while ( (PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);
}

/*********************************************************************************
Initialize Master clock (MCK)
*********************************************************************************/

static void CLK_MasterClockInitialize(void)
{
    /* Program PMC_MCKR.PRES and wait for PMC_SR.MCKRDY to be set   */
    PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES_CLK_1;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);

    /* Program PMC_MCKR.MDIV and Wait for PMC_SR.MCKRDY to be set   */
    PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_MDIV_Msk) | PMC_MCKR_MDIV_PCK_DIV2;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);

    /* Program PMC_MCKR.CSS and Wait for PMC_SR.MCKRDY to be set    */
    PMC_REGS->PMC_MCKR = (PMC_REGS->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_PLLA_CLK;
    while ((PMC_REGS->PMC_SR & PMC_SR_MCKRDY_Msk) != PMC_SR_MCKRDY_Msk);

}

/*********************************************************************************
Initialize USB FS clock
*********************************************************************************/

static void CLK_USBClockInitialize ( void )
{
    /* Configure Full-Speed USB Clock source and Clock Divider */
    PMC_REGS->PMC_USB = PMC_USB_USBDIV(9)  | PMC_USB_USBS_Msk;


    /* Enable Full-Speed USB Clock Output */
    PMC_REGS->PMC_SCER = PMC_SCER_USBCLK_Msk;
}






/*********************************************************************************
Initialize Peripheral clock
*********************************************************************************/

static void CLK_PeripheralClockInitialize(void)
{
    /* Enable clock for the selected peripherals */
    PMC_REGS->PMC_PCER0=0x31c00;
    PMC_REGS->PMC_PCER1=0x4;
}

/*********************************************************************************
Clock Initialize
*********************************************************************************/

void CLK_Initialize( void )
{
    /* Set Flash Wait States and  Enable Code Loop Optimization */
    EFC_REGS->EEFC_FMR = EEFC_FMR_FWS(6) | EEFC_FMR_CLOE_Msk;


    /* Initialize Main Clock */
    CLK_MainClockInitialize();

    /* Initialize PLLA */
    CLK_PLLAInitialize();

    /* Initialize UTMI PLL */
    CLK_UTMIPLLInitialize();

    /* Initialize Master Clock */
    CLK_MasterClockInitialize();

    /* Initialize USB Clock */
    CLK_USBClockInitialize();



    /* Initialize Peripheral Clock */
    CLK_PeripheralClockInitialize();
}
