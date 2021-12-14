
#define RCC_CR      (unsigned long *)0x40021000
#define RCC_CFGR    (unsigned long *)0x40021008
#define RCC_PLLCFGR (unsigned long *)0x4002100C
#define RCC_IOPENR  (unsigned long *)0x40021034

#define EXTI_RTSR1  (unsigned long *)0x40021800
#define EXTI_FTSR1  (unsigned long *)0x40021804
#define EXTI_RPR1   (unsigned long *)0x4002180C
#define EXTI_FPR1     (unsigned long *)0x40021810
#define EXTI_EXTICR1 (unsigned long *)0x40021860 //x=1
#define EXTI_EXTICR2 (unsigned long *)0x40021864 //x=2
#define EXTI_EXTICR3 (unsigned long *)0x40021868 //x=3
#define EXTI_EXTICR4 (unsigned long *)0x4002186C //x=4
#define EXTI_IMR1    (unsigned long *)0x40021880

#define FLASH_ACR   (unsigned long *)0x40022000

#define GPIOA_MODER (unsigned long *)0x50000000
#define GPIOA_ODR   (unsigned long *)0x50000014

#define GPIOC_MODER (unsigned long *)0x50000800
#define GPIOC_IDR   (unsigned long *)0x50000810

#define STK_CSR     (unsigned long *)0xE000E010
#define STK_RVR     (unsigned long *)0xE000E014
#define STK_CVR     (unsigned long *)0xE000E018

#define NVIC_ISER   (unsigned long *)0xE000E100

extern void _print_ch(char* ptr);

void print_ch(char ch) {
#if !NDEBUG
    _print_ch(&ch);
#endif
}

int tick;
void systick_handler() {
    tick++;
}

void exti_handler() {
    /* toggle user LED ON/OFF */
    *GPIOA_ODR ^= 0x00000020UL;

    //clear the pending interrupt
    *EXTI_FPR1 |= 0x00002000UL;
    *EXTI_RPR1 |= 0x00002000UL;
}

int millis() {
    return tick;
}

int main() {

#if NDEBUG
    /* configure main clock to 64MHz 8 */

    /* change flash wait state */
    *FLASH_ACR |= 0x00000002UL;

    /* modify PLL to set R=2, M=1, N=8 */
    /* Step 1: Disable the PLL */
    *RCC_CR &= ~0x01000000UL;

    /* Step 2: wait for PLL to stop */
    while (*RCC_CR & 0x02000000UL);

    /* Step 3: Set new values */
    unsigned long pll_cfg = *RCC_PLLCFGR;
    pll_cfg &= ~0x00000070UL; // set M to 1
    pll_cfg &= ~0x00007F00UL; // clear N to 0
    pll_cfg |= 0x00000800UL; // set N to 8
    pll_cfg &= ~0xE0000000UL; // clear R to 0
    pll_cfg |= 0x20000000UL; // set R to 2
    pll_cfg &= ~0x00000003UL; // clear clock source
    pll_cfg |= 0x00000002UL; // select HSI16 as new source
    *RCC_PLLCFGR = pll_cfg;  // write it back out

    /* Step 4: enable the PLL */
    *RCC_CR |= 0x01000000UL;

    /* Step 5: enable PLLR out */
    *RCC_PLLCFGR |= 0x10000000UL;

    /* select new 64MHz clock */
    unsigned long rcc_cfgr = *RCC_CFGR;
    rcc_cfgr &= ~0x00000007UL;
    rcc_cfgr |= 0x00000002UL;
    *RCC_CFGR = rcc_cfgr;
#endif

    // ocnfig SYSTCK or 1ms count
    unsigned long stk_rvr = *STK_RVR;
    stk_rvr &= ~0x00FFFFFFUL;
    stk_rvr |= 0x00001F40UL;
    *STK_RVR = stk_rvr;

    //enable the SYSTCK timer and generate an exception
    *STK_CSR |= 0x00000003UL;

    /* setup port registers */

    /* enable GPIOA and C registers */
    *RCC_IOPENR |= 0x00000005UL;

    /* set GPIOA 5  to an output */
    *GPIOA_MODER &= ~0x00000C00UL;
    *GPIOA_MODER |=  0x00000400UL;

    /* Configure User Button(PC13) as input */
    *GPIOC_MODER &= ~0x0C000000UL;

    //enable PC13 as interrupt


    //PC13 turns into EXTI13 via the multiplexer
    *EXTI_EXTICR4 &= ~0x0000FF00UL;
    *EXTI_EXTICR4 |= 0x00000200UL;

    //select rising or falling edge
    *EXTI_FTSR1 |= 0x00002000UL;

    *EXTI_RTSR1 |= 0x00002000UL; //Rising edge

    *EXTI_IMR1 |= 0x00002000UL;


    *NVIC_ISER |= 0x00000080UL;

    int last_millis = millis();
    while(1) {

        if ((millis() - last_millis) >= 1000) {
            //Configure LED as output


            last_millis = millis();

        }

    }

}
