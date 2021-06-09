#include "gdt.h"
#include <Kernel/Boot/multiboot2_defs.h>

.section .multiboot

multiboot_header_start:

	.long  MULTIBOOT2_HEADER_MAGIC
	.long  MULTIBOOT_ARCHITECTURE_I386
	.long  (multiboot_header_end - multiboot_header_start)
	.long  -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header_start))

	/* START OF MULTIBOOT2 TAGS */

	/*
framebuffer_tag_start:
	.short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
	.short MULTIBOOT_HEADER_TAG_OPTIONAL
	.long  (framebuffer_tag_end - framebuffer_tag_start)
	.long  1024
	.long  768
	.long  32
framebuffer_tag_end:
	*/

	/*  END OF MULTIBOOT2 TAGS  */

	.short MULTIBOOT_HEADER_TAG_END
	.short 0
	.long  8

multiboot_header_end:

.code32

/* Common Symbols */
/* syntax: .comm {name}, {size}, {alignment} */
.global kernel_stack, kernel_stack_top
kernel_stack:
	.skip 0x4000
kernel_stack_top:
.align 0x1000

/* Global Descriptor Table */
.section .data
.align 0x1000
gdt_table:
	.quad GDT_FIRST_ENTRY
	.quad GDT_KERNEL_ENTRY

gdt_table_end:
	.skip (GDT_TABLE_SIZE - (gdt_table_end - gdt_table))

gdt_ptr:
	.short GDT_TABLE_SIZE - 1
	.long gdt_table

.section .text
.global _start
.type _start, @function
_start:
	movl $kernel_stack + 0x4000, %esp

	lgdt gdt_ptr

	ljmp $(KERNEL_GDT_ENTRY * GDT_ENTRY_SIZE), $_start64

	cli
	hlt

.type _start64, @function
_start64:
	//TODO: Get multiboot info ptr and magic value later!!
	movw 0  , %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	call k_init
	
	/* Should Never Reach */
	cli
	hlt
1:
	jmp 1b
