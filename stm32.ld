
MEMORY
{
    rom (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    ram (!rx) : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
    .text :
    {
        *(.text)
    } > rom

    .data :
    {
        *(.data)
    } > ram AT> rom

    .bss :
    {
        *(.bss)
    } > ram
}