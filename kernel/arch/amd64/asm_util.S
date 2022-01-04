.code64

.global load_cr3
.type load_cr3, @function
load_cr3:
	movq (%rdi), %cr3
	ret
