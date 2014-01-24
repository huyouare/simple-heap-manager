	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:
Leh_func_begin1:
	pushq	%rbp
Ltmp0:
	movq	%rsp, %rbp
Ltmp1:
	movl	_g_isd_int(%rip), %eax
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	movq	_g_int@GOTPCREL(%rip), %rcx
	movl	%eax, (%rcx)
	movl	-4(%rbp), %eax
	popq	%rbp
	ret
Leh_func_end1:

	.section	__DATA,__data
	.globl	_g_isd_int
	.align	2
_g_isd_int:
	.long	27

	.comm	_g_int,4,2
