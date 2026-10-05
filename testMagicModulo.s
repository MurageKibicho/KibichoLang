	.file	"testMagicModulo.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"KibichoMagicModulo.h"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"toMulLength > 0 && (toMulLength & (toMulLength - 1)) == 0"
	.text
	.p2align 4
	.globl	InitProductTree
	.type	InitProductTree, @function
InitProductTree:
.LFB91:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	testq	%rdi, %rdi
	je	.L2
	movq	%rdi, %rbx
	blsr	%rdi, %rax
	jne	.L2
	movl	$32, %edi
	call	malloc@PLT
	leaq	-1(%rbx,%rbx), %rbp
	movq	%rbx, (%rax)
	movq	%rbp, 8(%rax)
	salq	$3, %rbp
	movq	%rbp, %rdi
	movl	$1, %esi
	movq	%rax, %r12
	call	calloc@PLT
	movq	%rbp, %rdi
	movl	$1, %esi
	movq	%rax, 16(%r12)
	call	calloc@PLT
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rax, 24(%r12)
	movq	%r12, %rax
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L2:
	.cfi_restore_state
	leaq	__PRETTY_FUNCTION__.6779(%rip), %rcx
	movl	$29, %edx
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	call	__assert_fail@PLT
	.cfi_endproc
.LFE91:
	.size	InitProductTree, .-InitProductTree
	.p2align 4
	.globl	SetRemainderTree_NaiveTreeTraversal
	.type	SetRemainderTree_NaiveTreeTraversal, @function
SetRemainderTree_NaiveTreeTraversal:
.LFB92:
	.cfi_startproc
	endbr64
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	24(%rdi), %rdi
	call	fmpz_set@PLT
	cmpq	$1, (%rbx)
	je	.L13
	xorl	%ebp, %ebp
	.p2align 4,,10
	.p2align 3
.L11:
	incq	%rbp
	movq	%rbp, %r13
	movq	24(%rbx), %rdi
	salq	$4, %r13
	movq	16(%rbx), %rdx
	leaq	-8(%r13), %rax
	leaq	-8(,%rbp,8), %r12
	addq	%rax, %rdx
	leaq	(%rdi,%r12), %rsi
	addq	%rax, %rdi
	call	fmpz_mod@PLT
	movq	24(%rbx), %rdi
	movq	16(%rbx), %rdx
	leaq	(%rdi,%r12), %rsi
	addq	%r13, %rdx
	addq	%r13, %rdi
	call	fmpz_mod@PLT
	movq	(%rbx), %rax
	decq	%rax
	cmpq	%rbp, %rax
	ja	.L11
.L13:
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE92:
	.size	SetRemainderTree_NaiveTreeTraversal, .-SetRemainderTree_NaiveTreeTraversal
	.p2align 4
	.globl	SetRemainderNaive
	.type	SetRemainderNaive, @function
SetRemainderNaive:
.LFB93:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rdi, %r12
	movq	(%r12), %rax
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rsi, %rdi
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	leaq	-1(%rax), %rbx
	call	fmpz_get_si@PLT
	cmpq	$0, (%r12)
	je	.L19
	salq	$3, %rbx
	xorl	%ebp, %ebp
	.p2align 4,,10
	.p2align 3
.L17:
	movq	16(%r12), %rdi
	incq	%rbp
	addq	%rbx, %rdi
	call	fmpz_get_si@PLT
	addq	$8, %rbx
	cmpq	%rbp, (%r12)
	ja	.L17
.L19:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE93:
	.size	SetRemainderNaive, .-SetRemainderNaive
	.p2align 4
	.globl	FreeProductTree
	.type	FreeProductTree, @function
FreeProductTree:
.LFB94:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	cmpq	$0, 8(%rdi)
	je	.L22
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L26:
	movq	16(%rbp), %rax
	leaq	0(,%rbx,8), %r12
	movq	(%rax,%rbx,8), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L28
.L23:
	movq	24(%rbp), %rax
	movq	(%rax,%r12), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L29
	incq	%rbx
	cmpq	%rbx, 8(%rbp)
	ja	.L26
.L22:
	movq	16(%rbp), %rdi
	call	free@PLT
	movq	24(%rbp), %rdi
	call	free@PLT
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rbp, %rdi
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.p2align 4,,10
	.p2align 3
.L29:
	.cfi_restore_state
	incq	%rbx
	call	_fmpz_clear_mpz@PLT
	cmpq	%rbx, 8(%rbp)
	ja	.L26
	jmp	.L22
	.p2align 4,,10
	.p2align 3
.L28:
	call	_fmpz_clear_mpz@PLT
	jmp	.L23
	.cfi_endproc
.LFE94:
	.size	FreeProductTree, .-FreeProductTree
	.section	.rodata.str1.1
.LC2:
	.string	"Sizes: %lu,%lu\n"
	.section	.rodata.str1.8
	.align 8
.LC5:
	.string	"SetRemainderTree %lu: %.3f ms\n"
	.text
	.p2align 4
	.globl	Kibicho_CreateBatchMagicList
	.type	Kibicho_CreateBatchMagicList, @function
Kibicho_CreateBatchMagicList:
.LFB95:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movq	%rdi, %r15
	movl	$24, %edi
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movq	%rsi, %r14
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rdx, %rbx
	subq	$168, %rsp
	.cfi_def_cfa_offset 224
	movq	%rdx, 32(%rsp)
	leaq	96(%rsp), %r13
	leaq	112(%rsp), %rbp
	movq	%fs:40, %rax
	movq	%rax, 152(%rsp)
	xorl	%eax, %eax
	call	malloc@PLT
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	InitProductTree
	movq	%rbx, %rdi
	movq	%rax, (%r12)
	call	InitProductTree
	movq	%r13, %rdi
	movq	%rax, 8(%r12)
	movq	%rbx, 16(%r12)
	call	__gmpz_init@PLT
	movq	%rbp, %rdi
	call	__gmpz_init@PLT
	movl	$1, %esi
	movq	%r13, %rdi
	call	__gmpz_set_ui@PLT
	movq	%r15, %rdx
	movq	%r13, %rsi
	movq	%r13, %rdi
	call	__gmpz_mul_2exp@PLT
	movq	%r13, %rsi
	movq	%r13, %rdi
	call	__gmpz_nextprime@PLT
	movl	$1, %esi
	movq	%rbp, %rdi
	call	__gmpz_set_ui@PLT
	movq	%r14, %rdx
	movq	%rbp, %rsi
	movq	%rbp, %rdi
	call	__gmpz_mul_2exp@PLT
	movq	%rbp, %rsi
	movq	%rbp, %rdi
	call	__gmpz_nextprime@PLT
	movq	16(%r12), %rax
	leaq	-1(%rbx), %rcx
	movq	%rcx, 24(%rsp)
	movq	%rax, (%rsp)
	testq	%rax, %rax
	je	.L50
	movq	$0, 8(%rsp)
	movq	$0, (%rsp)
	leaq	0(,%rcx,8), %rbx
	xorl	%r14d, %r14d
	jmp	.L36
	.p2align 4,,10
	.p2align 3
.L71:
	movq	%rax, 16(%rsp)
	testl	%edx, %edx
	je	.L33
	movq	104(%rsp), %rdx
	movq	%r13, %rsi
	movq	(%rdx), %r15
	call	fmpz_set_mpz@PLT
	movq	8(%r12), %rcx
	movq	%rbp, %rsi
	movq	16(%rcx), %rdi
	addq	%rbx, %rdi
	call	fmpz_set_mpz@PLT
	testq	%r15, %r15
	movq	16(%rsp), %rax
	je	.L34
.L48:
	xorl	%edx, %edx
	movl	$64, %ecx
	lzcntq	%r15, %rdx
	subl	%edx, %ecx
	movslq	%ecx, %rdx
	addq	%rdx, (%rsp)
.L34:
	testq	%rax, %rax
	je	.L35
	movl	$64, %ecx
	lzcntq	%rax, %rax
	subl	%eax, %ecx
	movslq	%ecx, %rax
	addq	%rax, 8(%rsp)
.L35:
	movq	%rbp, %rsi
	movq	%rbp, %rdi
	call	__gmpz_nextprime@PLT
	incq	%r14
	movq	%r13, %rsi
	movq	%r13, %rdi
	call	__gmpz_nextprime@PLT
	addq	$8, %rbx
	cmpq	%r14, 16(%r12)
	jbe	.L31
.L36:
	movq	(%r12), %rcx
	movq	120(%rsp), %rax
	movq	16(%rcx), %rdi
	movl	100(%rsp), %ecx
	movl	116(%rsp), %edx
	movq	(%rax), %rax
	addq	%rbx, %rdi
	testl	%ecx, %ecx
	jne	.L71
	testl	%edx, %edx
	jne	.L47
	movq	%r13, %rsi
	call	fmpz_set_mpz@PLT
	movq	8(%r12), %rax
	movq	%rbp, %rsi
	movq	16(%rax), %rdi
	addq	%rbx, %rdi
	call	fmpz_set_mpz@PLT
	jmp	.L35
	.p2align 4,,10
	.p2align 3
.L33:
	movq	104(%rsp), %rax
	movq	%r13, %rsi
	movq	(%rax), %r15
	call	fmpz_set_mpz@PLT
	movq	8(%r12), %rax
	movq	%rbp, %rsi
	movq	16(%rax), %rdi
	addq	%rbx, %rdi
	call	fmpz_set_mpz@PLT
	testq	%r15, %r15
	je	.L35
	xorl	%eax, %eax
	jmp	.L48
	.p2align 4,,10
	.p2align 3
.L47:
	movq	%r13, %rsi
	movq	%rax, 16(%rsp)
	call	fmpz_set_mpz@PLT
	movq	8(%r12), %rdx
	movq	%rbp, %rsi
	movq	16(%rdx), %rdi
	addq	%rbx, %rdi
	call	fmpz_set_mpz@PLT
	movq	16(%rsp), %rax
	jmp	.L34
	.p2align 4,,10
	.p2align 3
.L50:
	movq	$0, 8(%rsp)
	.p2align 4,,10
	.p2align 3
.L31:
	movq	32(%rsp), %r14
	subq	$2, %r14
	cmpq	$0, 24(%rsp)
	je	.L40
	movq	%rbp, 16(%rsp)
	.p2align 4,,10
	.p2align 3
.L37:
	movq	(%r12), %rax
	leaq	1(%r14), %rbx
	movq	16(%rax), %rdi
	salq	$4, %rbx
	leaq	0(,%r14,8), %rbp
	leaq	-8(%rbx), %r15
	leaq	(%rdi,%rbx), %rdx
	leaq	(%rdi,%r15), %rsi
	addq	%rbp, %rdi
	call	fmpz_mul@PLT
	movq	8(%r12), %rdx
	decq	%r14
	movq	16(%rdx), %rdi
	leaq	(%rdi,%rbx), %rdx
	leaq	(%rdi,%r15), %rsi
	addq	%rbp, %rdi
	call	fmpz_mul@PLT
	cmpq	$-1, %r14
	jne	.L37
	movq	16(%rsp), %rbp
.L40:
	leaq	128(%rsp), %r15
	leaq	80(%rsp), %rbx
	movq	(%rsp), %rdx
	movabsq	$-4601097622831300607, %rax
	movq	%rbx, %rdi
	movq	%rax, 136(%rsp)
	movq	%r15, %rsi
	movabsq	$-5304373996139296842, %rax
	movq	%rax, 144(%rsp)
	movq	$0, 128(%rsp)
	movq	$0, 80(%rsp)
	movq	$0, 88(%rsp)
	movq	%r15, 40(%rsp)
	call	fmpz_randbits@PLT
	movq	8(%rsp), %r14
	leaq	88(%rsp), %rax
	movq	%r14, %rdx
	movq	%rax, %rdi
	movq	%r15, %rsi
	movq	%rax, 32(%rsp)
	call	fmpz_randbits@PLT
	movq	(%rsp), %rdx
	movq	%r14, %rcx
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	leaq	48(%rsp), %rsi
	movl	$1, %edi
	movq	%rsi, 16(%rsp)
	call	clock_gettime@PLT
	movq	(%r12), %r14
	movq	%rbx, %rsi
	movq	24(%r14), %rdi
	xorl	%ebx, %ebx
	call	fmpz_set@PLT
	cmpq	$1, (%r14)
	je	.L39
	movq	%rbp, 24(%rsp)
	.p2align 4,,10
	.p2align 3
.L38:
	incq	%rbx
	movq	%rbx, %rbp
	salq	$4, %rbp
	movq	16(%r14), %r8
	movq	24(%r14), %rdi
	leaq	-8(%rbp), %rdx
	addq	%rdx, %r8
	leaq	-8(,%rbx,8), %r15
	leaq	(%rdi,%r15), %rsi
	addq	%rdx, %rdi
	movq	%r8, %rdx
	call	fmpz_mod@PLT
	movq	24(%r14), %rdi
	movq	16(%r14), %rdx
	leaq	(%rdi,%r15), %rsi
	addq	%rbp, %rdx
	addq	%rbp, %rdi
	call	fmpz_mod@PLT
	movq	(%r14), %rax
	decq	%rax
	cmpq	%rax, %rbx
	jb	.L38
	movq	24(%rsp), %rbp
.L39:
	leaq	64(%rsp), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	movq	%rax, 24(%rsp)
	call	clock_gettime@PLT
	movq	64(%rsp), %rax
	vxorpd	%xmm2, %xmm2, %xmm2
	subq	48(%rsp), %rax
	vcvtsi2sdq	%rax, %xmm2, %xmm0
	movq	72(%rsp), %rax
	movq	(%rsp), %rdx
	subq	56(%rsp), %rax
	vcvtsi2sdq	%rax, %xmm2, %xmm1
	leaq	.LC5(%rip), %rsi
	movl	$1, %edi
	movl	$1, %eax
	vdivsd	.LC3(%rip), %xmm1, %xmm1
	vfmadd132sd	.LC4(%rip), %xmm1, %xmm0
	xorl	%ebx, %ebx
	call	__printf_chk@PLT
	movq	16(%rsp), %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movq	8(%r12), %r14
	movq	32(%rsp), %rsi
	movq	24(%r14), %rdi
	call	fmpz_set@PLT
	cmpq	$1, (%r14)
	je	.L42
	movq	%rbp, (%rsp)
	.p2align 4,,10
	.p2align 3
.L41:
	incq	%rbx
	movq	%rbx, %rbp
	salq	$4, %rbp
	movq	16(%r14), %r8
	movq	24(%r14), %rdi
	leaq	-8(%rbp), %rdx
	addq	%rdx, %r8
	leaq	-8(,%rbx,8), %r15
	leaq	(%rdi,%r15), %rsi
	addq	%rdx, %rdi
	movq	%r8, %rdx
	call	fmpz_mod@PLT
	movq	24(%r14), %rdi
	movq	16(%r14), %rdx
	leaq	(%rdi,%r15), %rsi
	addq	%rbp, %rdx
	addq	%rbp, %rdi
	call	fmpz_mod@PLT
	movq	(%r14), %rax
	decq	%rax
	cmpq	%rax, %rbx
	jb	.L41
	movq	(%rsp), %rbp
.L42:
	movq	24(%rsp), %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movq	64(%rsp), %rax
	vxorpd	%xmm3, %xmm3, %xmm3
	subq	48(%rsp), %rax
	vcvtsi2sdq	%rax, %xmm3, %xmm0
	movq	72(%rsp), %rax
	movq	8(%rsp), %rdx
	subq	56(%rsp), %rax
	vcvtsi2sdq	%rax, %xmm3, %xmm1
	leaq	.LC5(%rip), %rsi
	movl	$1, %edi
	movl	$1, %eax
	vdivsd	.LC3(%rip), %xmm1, %xmm1
	vfmadd132sd	.LC4(%rip), %xmm1, %xmm0
	call	__printf_chk@PLT
	movq	%r13, %rdi
	call	__gmpz_clear@PLT
	movq	%rbp, %rdi
	call	__gmpz_clear@PLT
	cmpq	$0, 128(%rsp)
	je	.L44
	movq	40(%rsp), %rdi
	call	_flint_rand_clear_gmp_state@PLT
.L44:
	movq	80(%rsp), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L72
.L45:
	movq	88(%rsp), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L73
.L30:
	movq	152(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L74
	addq	$168, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	movq	%r12, %rax
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L72:
	.cfi_restore_state
	call	_fmpz_clear_mpz@PLT
	jmp	.L45
	.p2align 4,,10
	.p2align 3
.L73:
	call	_fmpz_clear_mpz@PLT
	jmp	.L30
.L74:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE95:
	.size	Kibicho_CreateBatchMagicList, .-Kibicho_CreateBatchMagicList
	.p2align 4
	.globl	Kibicho_DestroyBatchMagicList
	.type	Kibicho_DestroyBatchMagicList, @function
Kibicho_DestroyBatchMagicList:
.LFB96:
	.cfi_startproc
	endbr64
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	(%rdi), %rbp
	cmpq	$0, 8(%rbp)
	je	.L76
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L80:
	movq	16(%rbp), %rax
	leaq	0(,%rbx,8), %r13
	movq	(%rax,%rbx,8), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L87
.L77:
	movq	24(%rbp), %rax
	movq	(%rax,%r13), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L88
	incq	%rbx
	cmpq	8(%rbp), %rbx
	jb	.L80
.L76:
	movq	16(%rbp), %rdi
	call	free@PLT
	movq	24(%rbp), %rdi
	call	free@PLT
	movq	%rbp, %rdi
	call	free@PLT
	movq	8(%r12), %rbp
	cmpq	$0, 8(%rbp)
	je	.L81
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L85:
	movq	16(%rbp), %rax
	leaq	0(,%rbx,8), %r13
	movq	(%rax,%rbx,8), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L89
.L82:
	movq	24(%rbp), %rax
	movq	(%rax,%r13), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L90
	incq	%rbx
	cmpq	8(%rbp), %rbx
	jb	.L85
.L81:
	movq	16(%rbp), %rdi
	call	free@PLT
	movq	24(%rbp), %rdi
	call	free@PLT
	movq	%rbp, %rdi
	call	free@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	movq	%r12, %rdi
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.p2align 4,,10
	.p2align 3
.L88:
	.cfi_restore_state
	incq	%rbx
	call	_fmpz_clear_mpz@PLT
	cmpq	%rbx, 8(%rbp)
	ja	.L80
	jmp	.L76
	.p2align 4,,10
	.p2align 3
.L87:
	call	_fmpz_clear_mpz@PLT
	jmp	.L77
	.p2align 4,,10
	.p2align 3
.L90:
	incq	%rbx
	call	_fmpz_clear_mpz@PLT
	cmpq	%rbx, 8(%rbp)
	ja	.L85
	jmp	.L81
	.p2align 4,,10
	.p2align 3
.L89:
	call	_fmpz_clear_mpz@PLT
	jmp	.L82
	.cfi_endproc
.LFE96:
	.size	Kibicho_DestroyBatchMagicList, .-Kibicho_DestroyBatchMagicList
	.p2align 4
	.globl	TestMagicModulo
	.type	TestMagicModulo, @function
TestMagicModulo:
.LFB97:
	.cfi_startproc
	endbr64
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movl	$1024, %edx
	movl	$62, %esi
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movl	$10, %edi
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	call	Kibicho_CreateBatchMagicList
	movq	(%rax), %rbp
	movq	%rax, %r12
	cmpq	$0, 8(%rbp)
	je	.L92
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L96:
	movq	16(%rbp), %rax
	leaq	0(,%rbx,8), %r13
	movq	(%rax,%rbx,8), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L103
.L93:
	movq	24(%rbp), %rax
	movq	(%rax,%r13), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L104
	incq	%rbx
	cmpq	8(%rbp), %rbx
	jb	.L96
.L92:
	movq	16(%rbp), %rdi
	call	free@PLT
	movq	24(%rbp), %rdi
	call	free@PLT
	movq	%rbp, %rdi
	call	free@PLT
	movq	8(%r12), %rbp
	cmpq	$0, 8(%rbp)
	je	.L97
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L101:
	movq	16(%rbp), %rax
	leaq	0(,%rbx,8), %r13
	movq	(%rax,%rbx,8), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L105
.L98:
	movq	24(%rbp), %rax
	movq	(%rax,%r13), %rdi
	movq	%rdi, %rax
	sarq	$62, %rax
	cmpq	$1, %rax
	je	.L106
	incq	%rbx
	cmpq	8(%rbp), %rbx
	jb	.L101
.L97:
	movq	16(%rbp), %rdi
	call	free@PLT
	movq	24(%rbp), %rdi
	call	free@PLT
	movq	%rbp, %rdi
	call	free@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	movq	%r12, %rdi
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.p2align 4,,10
	.p2align 3
.L104:
	.cfi_restore_state
	incq	%rbx
	call	_fmpz_clear_mpz@PLT
	cmpq	%rbx, 8(%rbp)
	ja	.L96
	jmp	.L92
	.p2align 4,,10
	.p2align 3
.L103:
	call	_fmpz_clear_mpz@PLT
	jmp	.L93
	.p2align 4,,10
	.p2align 3
.L106:
	incq	%rbx
	call	_fmpz_clear_mpz@PLT
	cmpq	%rbx, 8(%rbp)
	ja	.L101
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L105:
	call	_fmpz_clear_mpz@PLT
	jmp	.L98
	.cfi_endproc
.LFE97:
	.size	TestMagicModulo, .-TestMagicModulo
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB98:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	call	TestMagicModulo
	call	flint_cleanup@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE98:
	.size	main, .-main
	.section	.rodata
	.align 16
	.type	__PRETTY_FUNCTION__.6779, @object
	.size	__PRETTY_FUNCTION__.6779, 16
__PRETTY_FUNCTION__.6779:
	.string	"InitProductTree"
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC3:
	.long	0
	.long	1093567616
	.align 8
.LC4:
	.long	0
	.long	1083129856
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
