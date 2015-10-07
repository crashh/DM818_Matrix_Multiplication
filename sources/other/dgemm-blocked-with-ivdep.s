	.file	"dgemm-blocked.cpp"
	.text
	.p2align 4,,15
	.globl	_Z10simd_dgemmiiiiPKdPdS1_
	.type	_Z10simd_dgemmiiiiPKdPdS1_, @function
_Z10simd_dgemmiiiiPKdPdS1_:
.LFB608:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	.cfi_offset 15, -24
	movl	%ecx, %r15d
	pushq	%r14
	pushq	%r13
	pushq	%r12
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	movl	%edx, %r12d
	movl	%ecx, %edx
	shrl	$31, %edx
	leal	(%rcx,%rdx), %eax
	pushq	%rbx
	.cfi_offset 3, -56
	movq	%r9, %rbx
	andl	$1, %eax
	subq	$136, %rsp
	movl	%esi, -84(%rbp)
	subl	%edx, %eax
	movq	%r8, -168(%rbp)
	movl	%edi, -88(%rbp)
	leal	(%rax,%rcx), %r14d
	movl	%r12d, %ecx
	movq	%r9, -160(%rbp)
	shrl	$31, %ecx
	leal	(%r12,%rcx), %eax
	andl	$1, %eax
	subl	%ecx, %eax
	addl	%r12d, %eax
	movl	%eax, -64(%rbp)
	imull	%r14d, %eax
	cltq
	leaq	30(,%rax,8), %rsi
	andq	$-16, %rsi
	subq	%rsi, %rsp
	leaq	15(%rsp), %r8
	movq	%r8, -80(%rbp)
	andq	$-16, -80(%rbp)
	testl	%r12d, %r12d
	jle	.L2
	movq	-80(%rbp), %rdx
	movslq	%r15d, %r10
	movslq	%r14d, %r9
	movslq	%edi, %rdi
	xorpd	%xmm0, %xmm0
	movq	%r9, -128(%rbp)
	salq	$3, %rdi
	salq	$3, %r9
	leaq	(%rdx,%r10,8), %r11
	movl	%r14d, %r13d
	movq	%r9, -136(%rbp)
	movq	%r10, -144(%rbp)
	movq	%rdi, -152(%rbp)
	subl	%r15d, %r13d
	movq	%r11, -96(%rbp)
	movl	$0, -104(%rbp)
	xorl	%r11d, %r11d
	movq	$0, -56(%rbp)
	movl	$0, -72(%rbp)
	movq	%rdx, %rsi
.L3:
	testl	%r15d, %r15d
	jle	.L16
	movq	%rbx, %rcx
	salq	$60, %rcx
	shrq	$63, %rcx
	cmpl	%r15d, %ecx
	cmova	%r15d, %ecx
	cmpl	$1, %r15d
	jne	.L17
	movsd	(%rbx), %xmm4
	movslq	%r11d, %rcx
	movsd	%xmm4, (%rsi,%rcx,8)
.L16:
	cmpl	%r14d, %r15d
	jge	.L14
	movq	-96(%rbp), %r9
	salq	$60, %r9
	shrq	$63, %r9
	cmpl	%r13d, %r9d
	cmova	%r13d, %r9d
	cmpl	$1, %r13d
	cmovbe	%r13d, %r9d
	testl	%r9d, %r9d
	je	.L54
	leal	(%r15,%r11), %eax
	cmpl	%r9d, %r13d
	leal	1(%r15), %r8d
	movslq	%eax, %rdi
	movq	$0, (%rsi,%rdi,8)
	je	.L14
.L9:
	movl	%r13d, %edi
	movl	%r9d, %edx
	subl	%r9d, %edi
	movl	%edi, -108(%rbp)
	shrl	%edi
	movl	%edi, %r10d
	addl	%r10d, %r10d
	je	.L11
	movq	-144(%rbp), %rcx
	addq	-56(%rbp), %rcx
	addq	%rdx, %rcx
	movl	$1, %edx
	leaq	(%rsi,%rcx,8), %r9
	leal	-1(%rdi), %ecx
	andl	$7, %ecx
	cmpl	%edi, %edx
	leaq	16(%r9), %rax
	movapd	%xmm0, (%r9)
	jnb	.L278
	testl	%ecx, %ecx
	je	.L15
	cmpl	$1, %ecx
	je	.L221
	cmpl	$2, %ecx
	.p2align 4,,3
	je	.L222
	cmpl	$3, %ecx
	.p2align 4,,2
	je	.L223
	cmpl	$4, %ecx
	.p2align 4,,2
	je	.L224
	cmpl	$5, %ecx
	.p2align 4,,2
	je	.L225
	cmpl	$6, %ecx
	.p2align 4,,2
	je	.L226
	movapd	%xmm0, 16(%r9)
	leaq	32(%r9), %rax
	movl	$2, %edx
.L226:
	movapd	%xmm0, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L225:
	movapd	%xmm0, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L224:
	movapd	%xmm0, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L223:
	movapd	%xmm0, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L222:
	movapd	%xmm0, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L221:
	addl	$1, %edx
	addq	$16, %rax
	movapd	%xmm0, -16(%rax)
	cmpl	%edi, %edx
	jnb	.L278
.L15:
	addl	$8, %edx
	subq	$-128, %rax
	movapd	%xmm0, -128(%rax)
	movapd	%xmm0, -112(%rax)
	movapd	%xmm0, -96(%rax)
	movapd	%xmm0, -80(%rax)
	movapd	%xmm0, -64(%rax)
	movapd	%xmm0, -48(%rax)
	movapd	%xmm0, -32(%rax)
	movapd	%xmm0, -16(%rax)
	cmpl	%edi, %edx
	jb	.L15
.L278:
	addl	%r10d, %r8d
	cmpl	%r10d, -108(%rbp)
	je	.L14
.L11:
	leal	(%r11,%r8), %eax
	cltq
	movq	$0, (%rsi,%rax,8)
.L14:
	addl	$1, -72(%rbp)
	addl	%r14d, %r11d
	movq	-136(%rbp), %rdi
	movq	-128(%rbp), %r9
	addq	%rdi, -96(%rbp)
	addq	%r9, -56(%rbp)
	addq	-152(%rbp), %rbx
	movl	-88(%rbp), %ecx
	addl	%ecx, -104(%rbp)
	cmpl	%r12d, -72(%rbp)
	jne	.L3
.L2:
	cmpl	-64(%rbp), %r12d
	jge	.L22
	movl	%r14d, %esi
	movslq	%r14d, %r11
	movq	-80(%rbp), %rbx
	imull	%r12d, %esi
	leaq	0(,%r11,8), %rdx
	movl	%r12d, %r13d
	xorpd	%xmm5, %xmm5
	movq	%rdx, -96(%rbp)
	movslq	%esi, %r8
	leaq	(%rbx,%r8,8), %rbx
	movq	%r8, %r10
	jmp	.L23
	.p2align 4,,10
	.p2align 3
.L299:
	movq	-80(%rbp), %rdx
	movslq	%esi, %rcx
	movq	$0, (%rdx,%rcx,8)
.L26:
	addl	$1, %r13d
	addl	%r14d, %esi
	addq	-96(%rbp), %rbx
	addq	%r11, %r10
	cmpl	-64(%rbp), %r13d
	je	.L22
.L23:
	testl	%r14d, %r14d
	jle	.L26
	movq	%rbx, %r9
	salq	$60, %r9
	shrq	$63, %r9
	cmpl	%r9d, %r14d
	cmovbe	%r14d, %r9d
	cmpl	$1, %r14d
	je	.L299
	testl	%r9d, %r9d
	jne	.L29
	xorl	%r8d, %r8d
.L52:
	movl	%r14d, %edi
	movl	%r9d, %edx
	subl	%r9d, %edi
	movl	%edi, -72(%rbp)
	shrl	%edi
	movl	%edi, %ecx
	addl	%ecx, %ecx
	movl	%ecx, -56(%rbp)
	je	.L30
	movq	-80(%rbp), %r9
	addq	%r10, %rdx
	leal	-1(%rdi), %ecx
	andl	$7, %ecx
	cmpl	$1, %edi
	leaq	(%r9,%rdx,8), %r9
	movl	$1, %edx
	movapd	%xmm5, (%r9)
	leaq	16(%r9), %rax
	jbe	.L280
	testl	%ecx, %ecx
	je	.L31
	cmpl	$1, %ecx
	je	.L215
	cmpl	$2, %ecx
	.p2align 4,,3
	je	.L216
	cmpl	$3, %ecx
	.p2align 4,,2
	je	.L217
	cmpl	$4, %ecx
	.p2align 4,,2
	je	.L218
	cmpl	$5, %ecx
	.p2align 4,,2
	je	.L219
	cmpl	$6, %ecx
	.p2align 4,,2
	je	.L220
	movapd	%xmm5, 16(%r9)
	leaq	32(%r9), %rax
	movl	$2, %edx
.L220:
	movapd	%xmm5, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L219:
	movapd	%xmm5, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L218:
	movapd	%xmm5, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L217:
	movapd	%xmm5, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L216:
	movapd	%xmm5, (%rax)
	addl	$1, %edx
	addq	$16, %rax
.L215:
	addl	$1, %edx
	addq	$16, %rax
	movapd	%xmm5, -16(%rax)
	cmpl	%edx, %edi
	jbe	.L280
.L31:
	addl	$8, %edx
	subq	$-128, %rax
	movapd	%xmm5, -128(%rax)
	movapd	%xmm5, -112(%rax)
	movapd	%xmm5, -96(%rax)
	movapd	%xmm5, -80(%rax)
	movapd	%xmm5, -64(%rax)
	movapd	%xmm5, -48(%rax)
	movapd	%xmm5, -32(%rax)
	movapd	%xmm5, -16(%rax)
	cmpl	%edx, %edi
	ja	.L31
.L280:
	movl	-56(%rbp), %r9d
	movl	-72(%rbp), %edi
	addl	%r9d, %r8d
	cmpl	%edi, %r9d
	je	.L26
.L30:
	leal	(%r8,%rsi), %eax
	movq	-80(%rbp), %r8
	cltq
	movq	$0, (%r8,%rax,8)
	jmp	.L26
.L17:
	testl	%ecx, %ecx
	jne	.L19
	xorl	%r10d, %r10d
.L51:
	movl	%r15d, %edx
	movl	%ecx, %r8d
	subl	%ecx, %edx
	movl	%edx, -108(%rbp)
	shrl	%edx
	movl	%edx, %r9d
	addl	%edx, %edx
	movl	%edx, -120(%rbp)
	je	.L20
	leaq	(%rbx,%r8,8), %rdi
	addq	-56(%rbp), %r8
	movl	$1, %ecx
	movl	$16, %eax
	movapd	(%rdi), %xmm2
	leaq	(%rsi,%r8,8), %rdx
	leal	-1(%r9), %r8d
	andl	$7, %r8d
	cmpl	%r9d, %ecx
	movlpd	%xmm2, (%rdx)
	movhpd	%xmm2, 8(%rdx)
	jnb	.L279
	testl	%r8d, %r8d
	je	.L21
	cmpl	$1, %r8d
	je	.L227
	cmpl	$2, %r8d
	je	.L228
	cmpl	$3, %r8d
	je	.L229
	cmpl	$4, %r8d
	je	.L230
	cmpl	$5, %r8d
	je	.L231
	cmpl	$6, %r8d
	je	.L232
	movapd	16(%rdi), %xmm3
	movl	$2, %ecx
	movl	$32, %eax
	movlpd	%xmm3, 16(%rdx)
	movhpd	%xmm3, 24(%rdx)
.L232:
	addl	$1, %ecx
	movapd	(%rdi,%rax), %xmm4
	movlpd	%xmm4, (%rdx,%rax)
	movhpd	%xmm4, 8(%rdx,%rax)
	addq	$16, %rax
.L231:
	addl	$1, %ecx
	movapd	(%rdi,%rax), %xmm5
	movlpd	%xmm5, (%rdx,%rax)
	movhpd	%xmm5, 8(%rdx,%rax)
	addq	$16, %rax
.L230:
	addl	$1, %ecx
	movapd	(%rdi,%rax), %xmm6
	movlpd	%xmm6, (%rdx,%rax)
	movhpd	%xmm6, 8(%rdx,%rax)
	addq	$16, %rax
.L229:
	addl	$1, %ecx
	movapd	(%rdi,%rax), %xmm7
	movlpd	%xmm7, (%rdx,%rax)
	movhpd	%xmm7, 8(%rdx,%rax)
	addq	$16, %rax
.L228:
	addl	$1, %ecx
	movapd	(%rdi,%rax), %xmm8
	movlpd	%xmm8, (%rdx,%rax)
	movhpd	%xmm8, 8(%rdx,%rax)
	addq	$16, %rax
.L227:
	addl	$1, %ecx
	movapd	(%rdi,%rax), %xmm9
	movlpd	%xmm9, (%rdx,%rax)
	movhpd	%xmm9, 8(%rdx,%rax)
	addq	$16, %rax
	cmpl	%r9d, %ecx
	jnb	.L279
.L21:
	addl	$8, %ecx
	movapd	(%rdi,%rax), %xmm10
	movapd	16(%rdi,%rax), %xmm11
	movapd	32(%rdi,%rax), %xmm12
	movapd	48(%rdi,%rax), %xmm13
	movapd	64(%rdi,%rax), %xmm14
	movapd	80(%rdi,%rax), %xmm15
	movapd	96(%rdi,%rax), %xmm1
	movapd	112(%rdi,%rax), %xmm2
	movlpd	%xmm10, (%rdx,%rax)
	movhpd	%xmm10, 8(%rdx,%rax)
	movlpd	%xmm11, 16(%rdx,%rax)
	movhpd	%xmm11, 24(%rdx,%rax)
	movlpd	%xmm12, 32(%rdx,%rax)
	movhpd	%xmm12, 40(%rdx,%rax)
	movlpd	%xmm13, 48(%rdx,%rax)
	movhpd	%xmm13, 56(%rdx,%rax)
	movlpd	%xmm14, 64(%rdx,%rax)
	movhpd	%xmm14, 72(%rdx,%rax)
	movlpd	%xmm15, 80(%rdx,%rax)
	movhpd	%xmm15, 88(%rdx,%rax)
	movlpd	%xmm1, 96(%rdx,%rax)
	movhpd	%xmm1, 104(%rdx,%rax)
	movlpd	%xmm2, 112(%rdx,%rax)
	movhpd	%xmm2, 120(%rdx,%rax)
	subq	$-128, %rax
	cmpl	%r9d, %ecx
	jb	.L21
.L279:
	movl	-120(%rbp), %r9d
	addl	%r9d, %r10d
	cmpl	%r9d, -108(%rbp)
	je	.L16
.L20:
	movl	-104(%rbp), %edi
	leal	(%r11,%r10), %eax
	movq	-160(%rbp), %r8
	cltq
	leal	(%rdi,%r10), %r10d
	movslq	%r10d, %rdx
	movsd	(%r8,%rdx,8), %xmm3
	movsd	%xmm3, (%rsi,%rax,8)
	jmp	.L16
.L54:
	movl	%r15d, %r8d
	jmp	.L9
.L22:
	movl	-84(%rbp), %edi
	movl	%edi, %eax
	imull	%r14d, %eax
	cltq
	leaq	30(,%rax,8), %rbx
	andq	$-16, %rbx
	subq	%rbx, %rsp
	leaq	15(%rsp), %rbx
	andq	$-16, %rbx
	cmpl	%r14d, %r15d
	jge	.L32
	movl	%r15d, %esi
.L33:
	testl	%edi, %edi
	jle	.L35
	leal	-1(%rdi), %r10d
	movl	$1, %r8d
	movslq	%esi, %r11
	movq	$0, (%rbx,%r11,8)
	leal	(%rsi,%r14), %r13d
	andl	$7, %r10d
	cmpl	%edi, %r8d
	je	.L35
	testl	%r10d, %r10d
	je	.L36
	cmpl	$1, %r10d
	je	.L209
	cmpl	$2, %r10d
	je	.L210
	cmpl	$3, %r10d
	je	.L211
	cmpl	$4, %r10d
	je	.L212
	cmpl	$5, %r10d
	je	.L213
	cmpl	$6, %r10d
	je	.L214
	movslq	%r13d, %r9
	movl	$2, %r8d
	addl	%r14d, %r13d
	movq	$0, (%rbx,%r9,8)
.L214:
	movslq	%r13d, %rcx
	addl	$1, %r8d
	addl	%r14d, %r13d
	movq	$0, (%rbx,%rcx,8)
.L213:
	movslq	%r13d, %rdx
	addl	$1, %r8d
	addl	%r14d, %r13d
	movq	$0, (%rbx,%rdx,8)
.L212:
	movslq	%r13d, %rax
	addl	$1, %r8d
	addl	%r14d, %r13d
	movq	$0, (%rbx,%rax,8)
.L211:
	movslq	%r13d, %r10
	addl	$1, %r8d
	addl	%r14d, %r13d
	movq	$0, (%rbx,%r10,8)
.L210:
	movslq	%r13d, %r11
	addl	$1, %r8d
	addl	%r14d, %r13d
	movq	$0, (%rbx,%r11,8)
.L209:
	addl	$1, %r8d
	movslq	%r13d, %r9
	addl	%r14d, %r13d
	cmpl	%edi, %r8d
	movq	$0, (%rbx,%r9,8)
	je	.L35
.L36:
	movslq	%r13d, %rcx
	addl	%r14d, %r13d
	addl	$8, %r8d
	movslq	%r13d, %rdx
	addl	%r14d, %r13d
	movq	$0, (%rbx,%rcx,8)
	movslq	%r13d, %rax
	addl	%r14d, %r13d
	movq	$0, (%rbx,%rdx,8)
	movslq	%r13d, %r10
	addl	%r14d, %r13d
	movq	$0, (%rbx,%rax,8)
	movslq	%r13d, %r11
	addl	%r14d, %r13d
	movq	$0, (%rbx,%r10,8)
	movslq	%r13d, %r9
	addl	%r14d, %r13d
	movq	$0, (%rbx,%r11,8)
	movslq	%r13d, %rcx
	addl	%r14d, %r13d
	movq	$0, (%rbx,%r9,8)
	movslq	%r13d, %rdx
	addl	%r14d, %r13d
	cmpl	%edi, %r8d
	movq	$0, (%rbx,%rcx,8)
	movq	$0, (%rbx,%rdx,8)
	jne	.L36
.L35:
	addl	$1, %esi
	cmpl	%r14d, %esi
	jne	.L33
.L32:
	movl	-84(%rbp), %r13d
	testl	%r13d, %r13d
	jle	.L1
	movl	-88(%rbp), %eax
	leal	-1(%r15), %edi
	leal	(%r14,%r14,8), %r8d
	movslq	%r14d, %r10
	movl	$0, -128(%rbp)
	movl	$0, -136(%rbp)
	shrl	%edi
	addl	%r8d, %r8d
	leaq	0(,%r10,8), %r13
	movslq	%eax, %r11
	addl	%eax, %eax
	leaq	1(%rdi), %r9
	cltq
	leaq	0(,%r11,8), %rsi
	movl	%r8d, -112(%rbp)
	salq	$3, %rax
	salq	$4, %r9
	movq	$0, -120(%rbp)
	movq	%rax, -152(%rbp)
	leal	(%r14,%r14), %eax
	movq	%rsi, -144(%rbp)
	movl	$0, -108(%rbp)
	cltq
	salq	$3, %rax
	movq	%rax, -160(%rbp)
.L38:
	testl	%r15d, %r15d
	jle	.L49
	movl	-128(%rbp), %ecx
	movq	-168(%rbp), %rdx
	movq	-120(%rbp), %rdi
	movq	%r11, -56(%rbp)
	movq	%r13, -64(%rbp)
	movq	%r9, -72(%rbp)
	movl	-108(%rbp), %r13d
	movq	-144(%rbp), %r9
	leal	(%rcx,%r15), %r10d
	movl	-84(%rbp), %r11d
	movl	%ecx, %r8d
	addq	%rdx, %rdi
	.p2align 4,,10
	.p2align 3
.L50:
	movl	%r13d, %esi
	leal	1(%r13), %ecx
	movslq	%r8d, %rax
	notl	%esi
	movsd	(%rdi), %xmm6
	addl	%r11d, %esi
	leal	(%r8,%r14), %edx
	andl	$7, %esi
	cmpl	%ecx, %r11d
	movsd	%xmm6, (%rbx,%rax,8)
	leaq	8(%rdi), %rax
	jle	.L281
	testl	%esi, %esi
	je	.L43
	cmpl	$1, %esi
	je	.L203
	cmpl	$2, %esi
	.p2align 4,,3
	je	.L204
	cmpl	$3, %esi
	.p2align 4,,2
	je	.L205
	cmpl	$4, %esi
	.p2align 4,,2
	je	.L206
	cmpl	$5, %esi
	.p2align 4,,2
	je	.L207
	cmpl	$6, %esi
	.p2align 4,,2
	je	.L208
	movsd	(%rax), %xmm7
	movslq	%edx, %rcx
	leaq	16(%rdi), %rax
	addl	%r14d, %edx
	movsd	%xmm7, (%rbx,%rcx,8)
	leal	2(%r13), %ecx
.L208:
	movsd	(%rax), %xmm8
	movslq	%edx, %rsi
	addl	$1, %ecx
	addq	$8, %rax
	addl	%r14d, %edx
	movsd	%xmm8, (%rbx,%rsi,8)
.L207:
	movsd	(%rax), %xmm9
	movslq	%edx, %rsi
	addl	$1, %ecx
	addq	$8, %rax
	addl	%r14d, %edx
	movsd	%xmm9, (%rbx,%rsi,8)
.L206:
	movsd	(%rax), %xmm10
	movslq	%edx, %rsi
	addl	$1, %ecx
	addq	$8, %rax
	addl	%r14d, %edx
	movsd	%xmm10, (%rbx,%rsi,8)
.L205:
	movsd	(%rax), %xmm11
	movslq	%edx, %rsi
	addl	$1, %ecx
	addq	$8, %rax
	addl	%r14d, %edx
	movsd	%xmm11, (%rbx,%rsi,8)
.L204:
	movsd	(%rax), %xmm12
	movslq	%edx, %rsi
	addl	$1, %ecx
	addq	$8, %rax
	addl	%r14d, %edx
	movsd	%xmm12, (%rbx,%rsi,8)
.L203:
	movsd	(%rax), %xmm13
	addl	$1, %ecx
	movslq	%edx, %rsi
	addq	$8, %rax
	addl	%r14d, %edx
	cmpl	%ecx, %r11d
	movsd	%xmm13, (%rbx,%rsi,8)
	jle	.L281
.L43:
	movsd	(%rax), %xmm14
	movslq	%edx, %rsi
	addl	%r14d, %edx
	movsd	8(%rax), %xmm15
	addl	$8, %ecx
	movsd	%xmm14, (%rbx,%rsi,8)
	movslq	%edx, %rsi
	addl	%r14d, %edx
	movsd	16(%rax), %xmm1
	addq	$64, %rax
	movsd	%xmm15, (%rbx,%rsi,8)
	movslq	%edx, %rsi
	addl	%r14d, %edx
	movsd	-40(%rax), %xmm2
	movsd	%xmm1, (%rbx,%rsi,8)
	movslq	%edx, %rsi
	addl	%r14d, %edx
	movsd	-32(%rax), %xmm3
	movsd	%xmm2, (%rbx,%rsi,8)
	movslq	%edx, %rsi
	addl	%r14d, %edx
	movsd	-24(%rax), %xmm4
	movsd	%xmm3, (%rbx,%rsi,8)
	movslq	%edx, %rsi
	addl	%r14d, %edx
	movsd	-16(%rax), %xmm0
	movsd	%xmm4, (%rbx,%rsi,8)
	movslq	%edx, %rsi
	addl	%r14d, %edx
	movsd	-8(%rax), %xmm5
	movsd	%xmm0, (%rbx,%rsi,8)
	movslq	%edx, %rsi
	addl	%r14d, %edx
	cmpl	%ecx, %r11d
	movsd	%xmm5, (%rbx,%rsi,8)
	jg	.L43
.L281:
	addl	$1, %r8d
	addq	%r9, %rdi
	cmpl	%r8d, %r10d
	jne	.L50
	movq	-56(%rbp), %r11
	movq	-64(%rbp), %r13
	movq	-72(%rbp), %r9
.L49:
	movl	-108(%rbp), %r10d
	cmpl	%r10d, -84(%rbp)
	leal	17(%r10), %r8d
	movl	%r8d, -88(%rbp)
	jle	.L41
	movq	16(%rbp), %rdx
	movq	-120(%rbp), %rcx
	movl	-136(%rbp), %edi
	movl	%r14d, -96(%rbp)
	movq	%rbx, -104(%rbp)
	movq	-160(%rbp), %r14
	movq	-152(%rbp), %rbx
	addq	%rcx, %rdx
	movl	%r10d, -72(%rbp)
	movl	%edi, -56(%rbp)
	movq	%rdx, -64(%rbp)
	.p2align 4,,10
	.p2align 3
.L42:
	testl	%r12d, %r12d
	jle	.L47
	movslq	-56(%rbp), %rax
	movq	-104(%rbp), %rsi
	xorl	%r10d, %r10d
	movq	-80(%rbp), %rdx
	movq	-64(%rbp), %r8
	leaq	(%rsi,%rax,8), %rsi
	.p2align 4,,10
	.p2align 3
.L48:
	testl	%r15d, %r15d
	movsd	(%r8), %xmm0
	movsd	(%r8,%r11,8), %xmm1
	jle	.L44
	movapd	(%rsi), %xmm6
	leaq	(%rdx,%r13), %rcx
	leaq	-16(%r9), %rdi
	movl	$16, %eax
	movapd	(%rdx), %xmm7
	shrq	$4, %rdi
	mulpd	%xmm6, %xmm7
	andl	$7, %edi
	cmpq	%r9, %rax
	mulpd	(%rcx), %xmm6
	addpd	%xmm7, %xmm0
	addpd	%xmm6, %xmm1
	je	.L44
	testq	%rdi, %rdi
	je	.L45
	cmpq	$1, %rdi
	je	.L197
	cmpq	$2, %rdi
	je	.L198
	cmpq	$3, %rdi
	je	.L199
	cmpq	$4, %rdi
	je	.L200
	cmpq	$5, %rdi
	je	.L201
	cmpq	$6, %rdi
	je	.L202
	movapd	16(%rsi), %xmm8
	movl	$32, %eax
	movapd	16(%rdx), %xmm9
	mulpd	%xmm8, %xmm9
	mulpd	16(%rcx), %xmm8
	addpd	%xmm9, %xmm0
	addpd	%xmm8, %xmm1
.L202:
	movapd	(%rsi,%rax), %xmm10
	movapd	(%rdx,%rax), %xmm11
	mulpd	%xmm10, %xmm11
	mulpd	(%rcx,%rax), %xmm10
	addq	$16, %rax
	addpd	%xmm11, %xmm0
	addpd	%xmm10, %xmm1
.L201:
	movapd	(%rsi,%rax), %xmm12
	movapd	(%rdx,%rax), %xmm13
	mulpd	%xmm12, %xmm13
	mulpd	(%rcx,%rax), %xmm12
	addq	$16, %rax
	addpd	%xmm13, %xmm0
	addpd	%xmm12, %xmm1
.L200:
	movapd	(%rsi,%rax), %xmm14
	movapd	(%rdx,%rax), %xmm15
	mulpd	%xmm14, %xmm15
	mulpd	(%rcx,%rax), %xmm14
	addq	$16, %rax
	addpd	%xmm15, %xmm0
	addpd	%xmm14, %xmm1
.L199:
	movapd	(%rsi,%rax), %xmm2
	movapd	(%rdx,%rax), %xmm3
	mulpd	%xmm2, %xmm3
	mulpd	(%rcx,%rax), %xmm2
	addq	$16, %rax
	addpd	%xmm3, %xmm0
	addpd	%xmm2, %xmm1
.L198:
	movapd	(%rsi,%rax), %xmm4
	movapd	(%rdx,%rax), %xmm5
	mulpd	%xmm4, %xmm5
	mulpd	(%rcx,%rax), %xmm4
	addq	$16, %rax
	addpd	%xmm5, %xmm0
	addpd	%xmm4, %xmm1
.L197:
	movapd	(%rsi,%rax), %xmm6
	movapd	(%rdx,%rax), %xmm7
	mulpd	%xmm6, %xmm7
	mulpd	(%rcx,%rax), %xmm6
	addq	$16, %rax
	cmpq	%r9, %rax
	addpd	%xmm7, %xmm0
	addpd	%xmm6, %xmm1
	je	.L44
.L45:
	movapd	(%rsi,%rax), %xmm8
	movapd	(%rdx,%rax), %xmm9
	mulpd	%xmm8, %xmm9
	movapd	16(%rsi,%rax), %xmm10
	mulpd	(%rcx,%rax), %xmm8
	movapd	16(%rdx,%rax), %xmm11
	mulpd	%xmm10, %xmm11
	addpd	%xmm9, %xmm0
	movapd	32(%rsi,%rax), %xmm12
	mulpd	16(%rcx,%rax), %xmm10
	movapd	32(%rdx,%rax), %xmm13
	addpd	%xmm8, %xmm1
	mulpd	%xmm12, %xmm13
	movapd	48(%rsi,%rax), %xmm14
	addpd	%xmm11, %xmm0
	mulpd	32(%rcx,%rax), %xmm12
	movapd	48(%rdx,%rax), %xmm15
	addpd	%xmm10, %xmm1
	mulpd	%xmm14, %xmm15
	movapd	64(%rsi,%rax), %xmm2
	addpd	%xmm13, %xmm0
	mulpd	48(%rcx,%rax), %xmm14
	movapd	64(%rdx,%rax), %xmm3
	addpd	%xmm12, %xmm1
	mulpd	%xmm2, %xmm3
	movapd	80(%rsi,%rax), %xmm4
	addpd	%xmm15, %xmm0
	mulpd	64(%rcx,%rax), %xmm2
	movapd	80(%rdx,%rax), %xmm5
	addpd	%xmm14, %xmm1
	mulpd	%xmm4, %xmm5
	movapd	96(%rsi,%rax), %xmm6
	addpd	%xmm3, %xmm0
	mulpd	80(%rcx,%rax), %xmm4
	movapd	96(%rdx,%rax), %xmm7
	addpd	%xmm2, %xmm1
	mulpd	%xmm6, %xmm7
	movapd	112(%rsi,%rax), %xmm8
	addpd	%xmm5, %xmm0
	mulpd	96(%rcx,%rax), %xmm6
	movapd	112(%rdx,%rax), %xmm9
	addpd	%xmm4, %xmm1
	mulpd	%xmm8, %xmm9
	addpd	%xmm7, %xmm0
	mulpd	112(%rcx,%rax), %xmm8
	subq	$-128, %rax
	cmpq	%r9, %rax
	addpd	%xmm6, %xmm1
	addpd	%xmm9, %xmm0
	addpd	%xmm8, %xmm1
	jne	.L45
.L44:
	haddpd	%xmm0, %xmm0
	addl	$2, %r10d
	addq	%r14, %rdx
	haddpd	%xmm1, %xmm1
	movlpd	%xmm0, (%r8)
	movlpd	%xmm1, (%r8,%r11,8)
	addq	%rbx, %r8
	cmpl	%r10d, %r12d
	jg	.L48
.L47:
	addl	$1, -72(%rbp)
	movl	-72(%rbp), %edx
	cmpl	%edx, -88(%rbp)
	jl	.L286
	movl	-84(%rbp), %r8d
	addq	$8, -64(%rbp)
	movl	-96(%rbp), %esi
	addl	%esi, -56(%rbp)
	cmpl	%r8d, %edx
	jne	.L42
.L286:
	movl	-96(%rbp), %r14d
	movq	-104(%rbp), %rbx
.L41:
	addl	$18, -108(%rbp)
	movl	-112(%rbp), %r10d
	addq	$144, -120(%rbp)
	movl	-108(%rbp), %ecx
	addl	%r10d, -136(%rbp)
	addl	%r10d, -128(%rbp)
	cmpl	%ecx, -84(%rbp)
	jg	.L38
.L1:
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L19:
	.cfi_restore_state
	movsd	(%rbx), %xmm1
	movslq	%r11d, %rax
	movl	$1, %r10d
	movsd	%xmm1, (%rsi,%rax,8)
	jmp	.L51
.L29:
	movq	-80(%rbp), %rdi
	movslq	%esi, %rax
	movl	$1, %r8d
	movq	$0, (%rdi,%rax,8)
	jmp	.L52
	.cfi_endproc
.LFE608:
	.size	_Z10simd_dgemmiiiiPKdPdS1_, .-_Z10simd_dgemmiiiiPKdPdS1_
	.p2align 4,,15
	.globl	_Z8do_blockiPdS_S_iii
	.type	_Z8do_blockiPdS_S_iii, @function
_Z8do_blockiPdS_S_iii:
.LFB609:
	.cfi_startproc
	movl	%r9d, %r10d
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movl	%edi, %r11d
	imull	%edi, %r10d
	subl	%r9d, %r11d
	movslq	%r8d, %rax
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	cmpl	$48, %r11d
	movq	%rdx, %rbp
	movl	$48, %r9d
	movl	%r11d, %edx
	movslq	%r10d, %r10
	cmovg	%r9d, %edx
	movl	%r9d, %r11d
	leaq	(%r10,%rax), %r9
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	%edi, %ebx
	subl	%r8d, %ebx
	movl	32(%rsp), %r12d
	leaq	(%rsi,%rax,8), %r8
	leaq	(%rcx,%r9,8), %rcx
	cmpl	$48, %ebx
	leaq	0(%rbp,%r10,8), %r9
	cmovle	%ebx, %r11d
	movq	%rcx, 32(%rsp)
	movl	%r12d, %ecx
	movl	%r11d, %esi
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	jmp	_Z10simd_dgemmiiiiPKdPdS1_
	.cfi_endproc
.LFE609:
	.size	_Z8do_blockiPdS_S_iii, .-_Z8do_blockiPdS_S_iii
	.p2align 4,,15
	.globl	_Z12square_dgemmiPdS_S_
	.type	_Z12square_dgemmiPdS_S_, @function
_Z12square_dgemmiPdS_S_:
.LFB610:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movl	%edi, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	testl	%edi, %edi
	jle	.L302
	movq	%rsi, %r13
	movq	%rdx, %r14
	movq	%rcx, %r15
	xorl	%r12d, %r12d
	.p2align 4,,10
	.p2align 3
.L304:
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L306:
	movl	%ebx, %r9d
	movl	%ebp, (%rsp)
	movl	%r12d, %r8d
	movq	%r15, %rcx
	movq	%r14, %rdx
	movq	%r13, %rsi
	movl	%ebp, %edi
	addl	$48, %ebx
	call	_Z8do_blockiPdS_S_iii
	cmpl	%ebx, %ebp
	jg	.L306
	addl	$48, %r12d
	cmpl	%r12d, %ebp
	jg	.L304
.L302:
	addq	$24, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE610:
	.size	_Z12square_dgemmiPdS_S_, .-_Z12square_dgemmiPdS_S_
	.globl	dgemm_desc
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Simple blocked dgemm."
	.data
	.align 8
	.type	dgemm_desc, @object
	.size	dgemm_desc, 8
dgemm_desc:
	.quad	.LC1
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
