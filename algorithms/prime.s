	.file	"prime.c"
	.option nopic
	.attribute arch, "rv32i2p1_m2p0_a2p1_f2p2_d2p2_zicsr2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	sieve_of_eratosthenes
	.type	sieve_of_eratosthenes, @function
sieve_of_eratosthenes:
.LFB0:
	.cfi_startproc
	addi	sp,sp,-464
	.cfi_def_cfa_offset 464
	sw	s0,460(sp)
	.cfi_offset 8, -4
	addi	s0,sp,464
	.cfi_def_cfa 8, 0
	sw	a0,-452(s0)
	sw	zero,-20(s0)
	sw	zero,-24(s0)
	j	.L2
.L3:
	lw	a5,-24(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	li	a4,1
	sw	a4,-424(a5)
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L2:
	lw	a4,-24(s0)
	lw	a5,-452(s0)
	ble	a4,a5,.L3
	li	a5,2
	sw	a5,-28(s0)
	j	.L4
.L8:
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-424(a5)
	li	a5,1
	bne	a4,a5,.L5
	lw	a5,-28(s0)
	mul	a5,a5,a5
	sw	a5,-32(s0)
	j	.L6
.L7:
	lw	a5,-32(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	sw	zero,-424(a5)
	lw	a4,-32(s0)
	lw	a5,-28(s0)
	add	a5,a4,a5
	sw	a5,-32(s0)
.L6:
	lw	a4,-32(s0)
	lw	a5,-452(s0)
	ble	a4,a5,.L7
.L5:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
.L4:
	lw	a5,-28(s0)
	mul	a5,a5,a5
	lw	a4,-452(s0)
	bge	a4,a5,.L8
	li	a5,2
	sw	a5,-36(s0)
	j	.L9
.L11:
	lw	a5,-36(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-424(a5)
	li	a5,1
	bne	a4,a5,.L10
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L10:
	lw	a5,-36(s0)
	addi	a5,a5,1
	sw	a5,-36(s0)
.L9:
	lw	a4,-36(s0)
	lw	a5,-452(s0)
	ble	a4,a5,.L11
	lw	a5,-20(s0)
	mv	a0,a5
	lw	s0,460(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 464
	addi	sp,sp,464
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE0:
	.size	sieve_of_eratosthenes, .-sieve_of_eratosthenes
	.align	2
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sw	ra,28(sp)
	sw	s0,24(sp)
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	addi	s0,sp,32
	.cfi_def_cfa 8, 0
	li	a5,50
	sw	a5,-20(s0)
	lw	a0,-20(s0)
	call	sieve_of_eratosthenes
	sw	a0,-24(s0)
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
.LFE1:
	.size	main, .-main
	.ident	"GCC: () 13.2.0"
	.section	.note.GNU-stack,"",@progbits
