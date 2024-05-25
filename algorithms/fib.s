	.file	"fib.c"
	.option nopic
	.attribute arch, "rv32i2p1_m2p0_a2p1_f2p2_d2p2_zicsr2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sw	ra,28(sp)
	sw	s0,24(sp)
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	addi	s0,sp,32
	.cfi_def_cfa 8, 0
	li	a0,10
	call	fibonacci
	sw	a0,-20(s0)
	li	a5,0
	mv	a0,a5
	lw	ra,28(sp)
	.cfi_restore 1
	lw	s0,24(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 32
	addi	sp,sp,32
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.align	2
	.globl	fibonacci
	.type	fibonacci, @function
fibonacci:
.LFB1:
	.cfi_startproc
	addi	sp,sp,-48
	.cfi_def_cfa_offset 48
	sw	s0,44(sp)
	.cfi_offset 8, -4
	addi	s0,sp,48
	.cfi_def_cfa 8, 0
	sw	a0,-36(s0)
	lw	a4,-36(s0)
	li	a5,1
	bgt	a4,a5,.L4
	lw	a5,-36(s0)
	j	.L5
.L4:
	sw	zero,-20(s0)
	li	a5,1
	sw	a5,-24(s0)
	li	a5,2
	sw	a5,-28(s0)
	j	.L6
.L7:
	lw	a5,-20(s0)
	sw	a5,-32(s0)
	lw	a5,-24(s0)
	sw	a5,-20(s0)
	lw	a4,-24(s0)
	lw	a5,-32(s0)
	add	a5,a4,a5
	sw	a5,-24(s0)
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
.L6:
	lw	a4,-28(s0)
	lw	a5,-36(s0)
	ble	a4,a5,.L7
	lw	a5,-24(s0)
.L5:
	mv	a0,a5
	lw	s0,44(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 48
	addi	sp,sp,48
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE1:
	.size	fibonacci, .-fibonacci
	.ident	"GCC: () 13.2.0"
	.section	.note.GNU-stack,"",@progbits
