    .section .text
    .global _start
    .global main

_start:
    j _start_continue

_trap:
    j _trap          # infinite loop trap

_start_continue:
    /* set stack pointer (adjust according to your RAM size) */
    li sp, 2048

    /* jump to main C code */
    jal ra, main

    /* main returned: loop forever */
halt:
    j halt           # infinite loop
