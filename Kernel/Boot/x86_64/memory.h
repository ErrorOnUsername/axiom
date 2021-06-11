#pragma once

#define KERNEL_PHYSICAL_START 0x0000000000400000
#define KERNEL_VIRTUAL_START  0xffffffff80400000

#define PML4_SIZE 0x1000
#define PML4_ENTRY_SIZE 8
#define PML4_ADDR_TO_IDX(addr) ( ((addr) >> 39) & 0x1ff )

#define PDPT_SIZE 0x1000
#define PDPT_ENTRY_SIZE 8
#define PDPT_ADDR_TO_IDX(addr) ( ((addr) >> 30) & 0x1ff )

#define PDT_SIZE 0x1000
#define PDT_ENTRY_SIZE 8

#define PT_SIZE 0x1000
#define PT_ENTRY_SIZE 8

#define FLAG_PRESENT ( 1 << 0 )
#define FLAG_WRITABLE ( 1 << 1 )
#define FLAG_USER_MEMORY ( 1 << 2 )
#define FLAG_PDE_TWO_MB ( 1 << 7 )
