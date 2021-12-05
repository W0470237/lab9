#define RCC_IOPENR  (unsigned long *)0x40021034
#define GPIOA_MODER (unsigned long *)0x50000000
#define GPIOA_ODR   (unsigned long *)0x50000014
#define GPIOC_MODER (unsigned long *)0x50000800
#define GPIOC_IDR   (unsigned long *)0x50000810


extern void _print_ch(char* ptr);

void print_ch(char ch) {
    _print_ch(&ch);
}

int main() {

    /* setup port registers */

    /* configure GPIOA and GPIOC registers */

    *RCC_IOPENR |= 0x00000005UL;

    /* configure LED port as output */

    *GPIOA_MODER &= ~0x00000C00UL;
    *GPIOA_MODER |=  0x00000400UL;

    /* configure user button as input */

    *GPIOC_MODER  &= ~0x0C000000UL;


    int x = 0;
    while(1) {

        /* turn user LED on */


        /* turn Arduino D0 HIGH */

        // for (i = 0; i < 1000000; i++);



        /* turn user LED OFF */

        /* turn Arduino D0 LOW */
        char N = 'N';
        char Y = 'Y';

        if((*GPIOC_IDR & 0x2000UL) == 0 && x == 0) {
            *GPIOA_ODR |= 0x00000020UL;
            print_ch(Y);
            x = 1;

        }
        if((*GPIOC_IDR & 0x2000UL) != 0 && x == 1)
        {
            print_ch(N);
            *GPIOA_ODR &= ~0x00000020UL;
            x = 0;

        }
    }
}