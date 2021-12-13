#define RCC_CR      (unsigned long *)0x40021000
#define RCC_CFGR    (unsigned long *)0x40021008
#define RCC_PLLCFGR (unsigned long *)0x4002100C
#define RCC_IOPENR  (unsigned long *)0x40021034

#define FLASH_ACR   (unsigned long *)0x40022000

#define GPIOA_MODER (unsigned long *) 0x50000000
#define GPIOA_ODR    (unsigned long *)0x50000014

#define STK_CSR    (unsigned long *)0xE000E010
#define STK_RVR    (unsigned long *)0xE000E014
#define STK_CVR    (unsigned long *)0xE000E018

extern void _print_ch(char *ptr);
extern void _print_str(char *str);

void print_ch(char ch)
{
#if !NDEBUG
    _print_ch(&ch);
#endif
}

void print_str(char *str)
{
#if !NDEBUG
    print_str(str);
#endif
}

void delay(int ms)
{
    int count = 0;
    while(count < ms){
        if(*STK_CSR & 0x00010000UL)
        {
            count++;
        }
    }
}

int main()
{
#if NDEBUG
    //change FLASH wait states
    *FLASH_ACR |= 0x00000002UL;

    //modify PLL to set R=2, M=1, N=8

    //step 1 disable the PLL
    *RCC_CR &= ~0x01000000UL;

    //step 2 wait for PLL to stop
    while(*RCC_CR & 0x02000000UL);

    //step 3 set new values
    unsigned long pll_cfg = *RCC_PLLCFGR; //backup reg into memory
    pll_cfg &= ~0x00000070UL; //set M to 1
    pll_cfg &= ~0x00007F00UL; //clear N to 0
    pll_cfg |= 0x00000800UL; //set N to 8
    pll_cfg &= ~0xE0000000UL; //clear R to 0
    pll_cfg |=  0x20000000UL; //set R to 2
    pll_cfg &= ~0x00000003UL; //clear CLK source
    pll_cfg |= 0x00000002UL; //set HSI16 as new source
    *RCC_PLLCFGR = pll_cfg;

    //step 4 enable the PLL
    *RCC_CR |= 0x01000000UL;

    //step 5 enable PLLR out
    *RCC_PLLCFGR |= 0x10000000UL; //enable PLLR out

    unsigned long rcc_cfgr = *RCC_CFGR;
    rcc_cfgr &= ~0x00000007UL;
    rcc_cfgr |=  0x00000002UL;
    *RCC_CFGR = rcc_cfgr;
#endif

    //set sysClock to 1ms
    unsigned long stk_rvr = *STK_RVR;
    stk_rvr &= ~0x00FFFFFFUL;
    stk_rvr |= 0x00001F40UL; //8000
    *STK_RVR = stk_rvr;

    //enable the systck timer
    *STK_CSR |= 0x00000001;

    //enable GPIOA registers
    // 1 for port A and 4 for port C
    *RCC_IOPENR |= 0x00000005UL;

    //set GPIOA 5 (LED) to an output
    //pages 241 for port mode register
    *GPIOA_MODER &= ~0x00000C00UL;
    *GPIOA_MODER |=  0x00000400UL;

    while(1)
    {
        //turn the user LED on
        *GPIOA_ODR |= 0x00000020UL;

        //wait 1 second
        delay(1000);

        //turn user LED off
        *GPIOA_ODR &= ~0x00000020UL;

        //wait 1 second
        delay(1000);
    }
}
