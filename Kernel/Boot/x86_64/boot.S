#define ASM_FILE

#include <Kernel/Boot/multiboot2.h>

#include "control_register.h"
#include "gdt.h"
#include "memory.h"
#include "msr.h"

//#########################################################
//
// This is the multiboot2 bootstrap.
//
// We begin by defining the multiboot2 header, as the
// specification defines. The spec can be found here:
// https://gnu.org/software/grub/manual/multiboot2/multiboot2.html
// 
// We then go on to set up paging and other preparations
// for the jump into long mode.
//
// Finally, once we have entered long mode, we jump to
// our C++ code by calling k_init, which is defined in
// Kernel/k_init.cpp.
//
//#########################################################

.section .multiboot
multiboot_header_start:

	.long  MULTIBOOT2_HEADER_MAGIC
	.long  MULTIBOOT_ARCHITECTURE_I386
	.long  (multiboot_header_end - multiboot_header_start)
	.long  -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header_start))

	.align 8

/*
	// START OF MULTIBOOT2 TAGS
framebuffer_tag_start:
	.short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
	.short MULTIBOOT_HEADER_TAG_OPTIONAL
	.long  framebuffer_tag_end - framebuffer_tag_start
	.long  1024
	.long  768
	.long  32
framebuffer_tag_end:
	.align 8

	// END OF MULTIBOOT2 TAGS
*/

	.short MULTIBOOT_HEADER_TAG_END
	.short 0
	.long  8

multiboot_header_end:

.code32

// HUGE thank you to missimer to his great tutorial on
// ringzeroandlower for much of this code.
//
// Link: http://ringzeroandlower.com/2017/08/08/x86-64-kernel-boot.html
//
// Also, the "Setting Up Long Mode" article on osdev
// wiki was super helpful as well.
//
// Link: https://wiki.osdev.org/Setting_Up_Long_Mode

.section .bss
.comm kernel_stack, 0x4000, 0x1000
.comm pml4, PML4_SIZE, 0x1000
.comm pdpt_l, PDPT_SIZE, 0x1000
.comm pdpt_h, PDPT_SIZE, 0x1000
.comm pdt_l, PDT_SIZE, 0x1000
.comm pdt_h, PDT_SIZE, 0x1000

// Global Descriptor Table
.data
.align 0x1000
gdt_table:
	.quad GDT_FIRST_ENTRY
	.quad GDT_KERNEL_ENTRY

gdt_table_end:
	.skip (GDT_TABLE_SIZE - (gdt_table_end - gdt_table))

gdt_ptr:
	.short GDT_TABLE_SIZE - 1
	.long gdt_table

// ELF Entry Point
//
// This is where execution begins.
//
// All we're doing is setting up the initial stack and
// loading the GDT. After that, we make the jump into
// long mode.
//
// TODO: We need to check if the cpu actually supports
// long mode before we make the jump.

.section .text
.global _start
.type _start, @function
_start:
	movl $kernel_stack + 0x4000, %esp

	// Initialize the low Page Directory Pointer Table
	movl $pdpt_l, %eax
	or $( FLAG_PRESENT | FLAG_WRITABLE ), %eax
	movl %eax, pml4 + (PML4_ADDR_TO_IDX(KERNEL_PHYSICAL_START) * PML4_ENTRY_SIZE)

	// Initialize the high Page Directory Pointer Table
	movl $pdpt_h, %eax
	or $( FLAG_PRESENT | FLAG_WRITABLE ), %eax
	movl %eax, pml4 + (PML4_ADDR_TO_IDX(KERNEL_VIRTUAL_START) * PML4_ENTRY_SIZE)

	// Initialize the low Page Directory Table
	movl $pdt_l, %eax
	or $( FLAG_PRESENT | FLAG_WRITABLE ), %eax
	movl %eax, pdpt_l + (PDPT_ADDR_TO_IDX(KERNEL_PHYSICAL_START) * PDPT_ENTRY_SIZE)

	// Initialize the high Page Directory Table
	movl $pdt_h, %eax
	or $( FLAG_PRESENT | FLAG_WRITABLE ), %eax
	movl %eax, pdpt_h + (PDPT_ADDR_TO_IDX(KERNEL_VIRTUAL_START) * PDPT_ENTRY_SIZE)

	movl $0, %ecx

	movl $_kernel_physical_end, %esi
	shrl $21, %esi
	addl $1, %esi

	// Initialize the Page Tables
pdt_loop:
	movl $( 1 << 21 ), %eax
	mul %ecx
	or $( FLAG_PRESENT | FLAG_WRITABLE | FLAG_PDE_TWO_MB ), %eax
	movl %eax, pdt_l(, %ecx, PDT_ENTRY_SIZE)
	movl %eax, pdt_h(, %ecx, PDT_ENTRY_SIZE)

	inc %ecx
	cmp %esi, %ecx
	jne pdt_loop

	movl $pml4, %eax
	movl %eax, %cr3

	// Set the Phisical Address Extention bit
	movl $KERNEL_CR4_FLAGS, %eax
	movl %eax, %cr4

	// Set the long mode bit in the EFER MSR and we're pretty
	// much set to make the jump into long mode.
	//
	// Note:
	//   EFER: Extended Feature Enable Register
	//   MSR: Model-Specific Register
	movl $MSR_EFER, %ecx
	rdmsr
	or $MSR_EFER_LONG_MODE_ENABLE, %eax
	wrmsr

	movl $KERNEL_CR0_FLAGS, %eax
	movl %eax, %cr0

	// Load out Global Descriptor Table
	lgdt gdt_ptr

	// Long mode time :)
	ljmp $(KERNEL_GDT_ENTRY * GDT_ENTRY_SIZE), $_start64

	cli
	hlt

.code64

.global _start64
.type _start64, @function
_start64:
	movw $0, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	//TODO: Get multiboot_info ptr and magic value later!!

	// Time for a HIGH LEVEL LANGUAGE... C++
	call k_init

	cli
	hlt
1:
	jmp 1b
