.file	"sub_140550FF0_v2.c"
	.text
	.globl	inner_method_40
	.type	inner_method_40, @function
inner_method_40:
.LFB6449:
	.cfi_startproc
	endbr64
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE6449:
	.size	inner_method_40, .-inner_method_40
	.globl	main_method_88
	.type	main_method_88, @function
main_method_88:
.LFB6450:
	.cfi_startproc
	endbr64
	movl	$66, %eax
	ret
	.cfi_endproc
.LFE6450:
	.size	main_method_88, .-main_method_88
	.globl	Subsumption__Task__InitializeTask
	.type	Subsumption__Task__InitializeTask, @function
Subsumption__Task__InitializeTask:
.LFB6448:
	.cfi_startproc
	endbr64
	ret
	.cfi_endproc
.LFE6448:
	.size	Subsumption__Task__InitializeTask, .-Subsumption__Task__InitializeTask
	.globl	sub_140550FF0
	.type	sub_140550FF0, @function
sub_140550FF0:
.LFB6447:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rdi
	pushq	%rsi
	subq	$160, %rsp
	andq	$-32, %rsp
	subq	$448, %rsp
	vmovaps	%xmm6, 448(%rsp)
	.cfi_escape 0x10,0x17,0x3,0x77,0xc0,0x3
	.cfi_offset 5, -24
	.cfi_offset 4, -32
	vmovaps	%xmm7, 464(%rsp)
	.cfi_escape 0x10,0x18,0x3,0x77,0xd0,0x3
	vmovaps	%xmm8, 480(%rsp)
	.cfi_escape 0x10,0x19,0x3,0x77,0xe0,0x3
	vmovaps	%xmm9, 496(%rsp)
	.cfi_escape 0x10,0x1a,0x3,0x77,0xf0,0x3
	vmovaps	%xmm10, 512(%rsp)
	.cfi_escape 0x10,0x1b,0x3,0x77,0x80,0x4
	vmovaps	%xmm11, 528(%rsp)
	.cfi_escape 0x10,0x1c,0x3,0x77,0x90,0x4
	vmovaps	%xmm12, 544(%rsp)
	.cfi_escape 0x10,0x1d,0x3,0x77,0xa0,0x4
	vmovaps	%xmm13, 560(%rsp)
	.cfi_escape 0x10,0x1e,0x3,0x77,0xb0,0x4
	vmovaps	%xmm14, 576(%rsp)
	.cfi_escape 0x10,0x1f,0x3,0x77,0xc0,0x4
	vmovaps	%xmm15, 592(%rsp)
	.cfi_escape 0x10,0x20,0x3,0x77,0xd0,0x4
	movq	%fs:40, %rax
	movq	%rax, 440(%rsp)
	xorl	%eax, %eax
	movq	%rcx, 16(%rsp)
	movq	%r8, 24(%rsp)
	vmovups	(%rdx), %ymm2
	vmovaps	%ymm2, 192(%rsp)
	vmovups	32(%rdx), %ymm3
	vmovaps	%ymm3, 224(%rsp)
	vmovups	64(%rdx), %ymm4
	vmovaps	%ymm4, 256(%rsp)
	vmovups	96(%rdx), %xmm5
	vmovaps	%xmm5, 288(%rsp)
	vmovups	112(%rdx), %xmm6
	vmovaps	%xmm6, 304(%rsp)
	vmovups	128(%rdx), %ymm7
	vmovaps	%ymm7, 320(%rsp)
	vmovups	160(%rdx), %ymm2
	vmovaps	%ymm2, 352(%rsp)
	vmovups	192(%rdx), %ymm3
	vmovaps	%ymm3, 384(%rsp)
	vmovdqu	56(%rcx), %xmm1
	vmovdqu	296(%rsp), %xmm0
	movl	312(%rsp), %edx
	movq	192(%rsp), %rcx
	movq	224(%rsp), %rax
	movl	%edx, 104(%rsp)
	movq	%rcx, 64(%rsp)
	vpextrq	$1, %xmm0, %rax
	movq	16(%rsp), %r8
	movq	64(%r8), %r8
	xorq	%rcx, %rax
	movl	%edx, %edx
	xorq	%rdx, %rax
	xorq	%r8, %rax
	vmovq	%xmm0, %rdx
	xorq	%rdx, %rax
	movq	16(%rsp), %rdx
	movq	136(%rdx), %rdi
	vmovq	%xmm1, %rdx
	xorq	%rdx, %rax
	vmovdqu	%xmm0, 88(%rsp)
	vmovdqu	%xmm1, 72(%rsp)
	movabsq	$1311768467425612850, %rdx
	xorq	%rdx, %rax
	movq	(%rdi), %rdx
	movq	%rax, 112(%rsp)
	vmovaps	64(%rsp), %ymm4
	vmovaps	%ymm4, 128(%rsp)
	vmovsd	112(%rsp), %xmm0
	vmovsd	%xmm0, 176(%rsp)
	vmovaps	96(%rsp), %xmm5
	vmovaps	%xmm5, 160(%rsp)
	vzeroupper
	leaq	128(%rsp), %rsi
	call	*64(%rdx)
	movq	%rax, 280(%rsp)
	testq	%rax, %rax
	je	.L8
.L5:
	movq	16(%rsp), %rax
	movq	24(%rsp), %rdx
	movq	16(%rsp), %rdi
	leaq	192(%rsp), %rsi
	movq	(%rax), %rax
	call	*136(%rax)
	movq	440(%rsp), %rdx
	subq	%fs:40, %rdx
	jne	.L9
	vmovaps	448(%rsp), %xmm6
	vmovaps	464(%rsp), %xmm7
	vmovaps	480(%rsp), %xmm8
	vmovaps	496(%rsp), %xmm9
	vmovaps	512(%rsp), %xmm10
	vmovaps	528(%rsp), %xmm11
	vmovaps	544(%rsp), %xmm12
	vmovaps	560(%rsp), %xmm13
	vmovaps	576(%rsp), %xmm14
	vmovaps	592(%rsp), %xmm15
	leaq	-16(%rbp), %rsp
	.cfi_remember_state
	.cfi_restore 32
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 28
	.cfi_restore 27
	.cfi_restore 26
	.cfi_restore 25
	.cfi_restore 24
	.cfi_restore 23
	popq	%rsi
	.cfi_restore 4
	popq	%rdi
	.cfi_restore 5
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
.L8:
	.cfi_restore_state
	movq	16(%rsp), %rdi
	leaq	192(%rsp), %rdx
	leaq	64(%rsp), %rsi
	call	Subsumption__Task__InitializeTask
	jmp	.L5
.L9:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6447:
	.size	sub_140550FF0, .-sub_140550FF0
	.globl	main
	.type	main, @function
main:
.LFB6451:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	andq	$-32, %rsp
	subq	$640, %rsp
	movq	%fs:40, %rax
	movq	%rax, 632(%rsp)
	xorl	%eax, %eax
	vpxor	%xmm0, %xmm0, %xmm0
	vmovdqu	%ymm0, 16(%rsp)
	vmovdqu	%ymm0, 48(%rsp)
	leaq	inner_method_40(%rip), %rax
	movq	%rax, 80(%rsp)
	vmovdqa	%ymm0, 96(%rsp)
	vmovdqa	%ymm0, 128(%rsp)
	vmovdqa	%ymm0, 160(%rsp)
	vmovdqa	%ymm0, 192(%rsp)
	movq	$0, 224(%rsp)
	leaq	main_method_88(%rip), %rax
	movq	%rax, 232(%rsp)
	leaq	16(%rsp), %rax
	movq	%rax, 8(%rsp)
	vpxor	%xmm0, %xmm0, %xmm0
	vmovdqu	%ymm0, 240(%rsp)
	vmovdqu	%ymm0, 272(%rsp)
	vmovdqu	%ymm0, 304(%rsp)
	vmovdqu	%ymm0, 336(%rsp)
	movq	$0, 368(%rsp)
	leaq	96(%rsp), %rax
	movq	%rax, 240(%rsp)
	leaq	8(%rsp), %rax
	movq	%rax, 376(%rsp)
	movabsq	$3689348815028241476, %rax
	movq	%rax, 296(%rsp)
	movabsq	$1229782938533634594, %rax
	movq	%rax, 304(%rsp)
	leaq	384(%rsp), %rdx
	movl	$0, %eax
.L11:
	movb	%al, (%rdx)
	addl	$7, %eax
	addq	$1, %rdx
	cmpb	$32, %al
	jne	.L11
	leaq	384(%rsp), %rdx
	leaq	240(%rsp), %rcx
	subq	$32, %rsp
	movl	$2882343476, %r8d
	call	sub_140550FF0
	addq	$32, %rsp
	movq	632(%rsp), %rdx
	subq	%fs:40, %rdx
	jne	.L15
	leave
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L15:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6451:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
