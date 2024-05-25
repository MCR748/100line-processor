.section .text
.globl _start

_start:
	li sp, 0x100f0000
	jal main
	j _start+0x8
