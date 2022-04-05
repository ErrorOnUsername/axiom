.macro push_all
	pushq %r15
	pushq %r14
	pushq %r13
	pushq %r12
	pushq %r11
	pushq %r10
	pushq %r9
	pushq %r8

	pushq %rdx
	pushq %rcx
	pushq %rbx
	pushq %rax

	pushq %rdi
	pushq %rsi
	pushq %rbp
	pushq %rsp
	lea   (%rip), %rax
	pushq %rax
.endm

.macro pop_all
	addq $16, %rsp
	popq %rbp
	popq %rsi
	popq %rdi

	popq %rax
	popq %rbx
	popq %rcx
	popq %rdx

	popq %r8
	popq %r9
	popq %r10
	popq %r11
	popq %r12
	popq %r13
	popq %r14
	popq %r15
.endm

.macro isr_err_stub vec
isr_stub_\vec:
	pushq $\vec
	push_all
	movq  %rsp, %rdi

	call common_interrupt_handler

	pop_all
	addq  $16,  %rsp

	iretq
.endm

.macro isr_no_err_stub vec
isr_stub_\vec:
	pushq $0
	pushq $\vec
	push_all

	movq  %rsp, %rdi
	call common_interrupt_handler

	pop_all
	addq  $16,  %rsp

	iretq
.endm

.global common_interrupt_handler
.type common_interrupt_handler, @function

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

isr_no_err_stub 32
isr_no_err_stub 33
isr_no_err_stub 34
isr_no_err_stub 35
isr_no_err_stub 36
isr_no_err_stub 37
isr_no_err_stub 38
isr_no_err_stub 39
isr_no_err_stub 40
isr_no_err_stub 41
isr_no_err_stub 42
isr_no_err_stub 43
isr_no_err_stub 44
isr_no_err_stub 45
isr_no_err_stub 46
isr_no_err_stub 47

.global isr_table
isr_table:
	.quad isr_stub_0
	.quad isr_stub_1
	.quad isr_stub_2
	.quad isr_stub_3
	.quad isr_stub_4
	.quad isr_stub_5
	.quad isr_stub_6
	.quad isr_stub_7
	.quad isr_stub_8
	.quad isr_stub_9
	.quad isr_stub_10
	.quad isr_stub_11
	.quad isr_stub_12
	.quad isr_stub_13
	.quad isr_stub_14
	.quad isr_stub_15
	.quad isr_stub_16
	.quad isr_stub_17
	.quad isr_stub_18
	.quad isr_stub_19
	.quad isr_stub_20
	.quad isr_stub_21
	.quad isr_stub_22
	.quad isr_stub_23
	.quad isr_stub_24
	.quad isr_stub_25
	.quad isr_stub_26
	.quad isr_stub_27
	.quad isr_stub_28
	.quad isr_stub_29
	.quad isr_stub_30
	.quad isr_stub_31

	.quad isr_stub_32
	.quad isr_stub_33
	.quad isr_stub_34
	.quad isr_stub_35
	.quad isr_stub_36
	.quad isr_stub_37
	.quad isr_stub_38
	.quad isr_stub_39
	.quad isr_stub_40
	.quad isr_stub_41
	.quad isr_stub_42
	.quad isr_stub_43
	.quad isr_stub_44
	.quad isr_stub_45
	.quad isr_stub_46
	.quad isr_stub_47
