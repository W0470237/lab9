#define RCC_IOPENR  (unsigned long *)0x40021034
#define GPIOA_MODER (unsigned long *)0x50000000
#define GPIOA_ODR   (unsigned long *)0x50000014
#define GPIOC_MODER (unsigned long *)0x50000800
#define GPIOC_IDR   (unsigned long *)0x50000810

int i;

int main() {

    /* setup port registers */

    /* configure GPIOA registers */

    *RCC_IOPENR |= 0x00000005UL;

    /* configure LED port as output */

    *GPIOA_MODER &= ~0x00000C00UL;
    *GPIOA_MODER |=  0x00000400UL;

    *GPIOC_MODER &= ~0x0C000000UL;

    while(1) {

        /* turn user LED on */

         *GPIOA_ODR |= 0x00000020UL;

        /* turn Arduino D0 HIGH */

        for (i = 0; i < 1000000; i++);

        /* turn user LED OFF */

        *GPIOA_ODR &= ~0x00000020UL;

        /* turn Arduino D0 LOW */

        for (i = 0; i < 1000000; i++);

    }
}