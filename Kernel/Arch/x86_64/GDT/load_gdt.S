.code64

.global load_gdt
.type load_gdt, @function
load_gdt:
	lgdt (%rdi)

	sub $16, %rsp
	movq $0x08, 8(%rsp)
	movabsq $exit, %rax
	mov %rax, (%rsp)
	lretq

exit:
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	ret
