.global invalidate_tlb
.type invalidate_tlb, @function
invalidate_tlb:
	movq %cr3, %rax
	movq %rax, %cr3
	ret

.global load_cr3
.type load_cr3, @function
load_cr3:
	movq %rdi, %cr3
	ret
