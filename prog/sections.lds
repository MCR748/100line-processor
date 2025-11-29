
MEMORY
{
    ram     : ORIGIN = 0x00000000, LENGTH = 0x3E00
    din     : ORIGIN = 0x00003E00, LENGTH = 0x100
    dout    : ORIGIN = 0x00003F00, LENGTH = 0x100
}


SECTIONS 
{
    .text : 
    {
        start.o(.text*)
        *(.text*)
    }  > ram

    .rodata :
    {
        *(.rodata*)
    }  > ram

    .data :
    {
        *(.data*)
    } > ram

    .bss :
    {
        *(.bss*)
    } > ram

    .din :
    {
        KEEP(*(.din)) 
    } > din

    .dout :
    {
        KEEP(*(.dout)) 
    } > dout

    __din_start = ADDR(.din);
    __dout_start = ADDR(.dout);
}
