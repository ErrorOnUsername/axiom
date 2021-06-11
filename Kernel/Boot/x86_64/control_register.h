#pragma once

#define CR0_PROT_MODE ( 1 << 0 )
#define CR0_EXTENSION_TYPE ( 1 << 4 )
#define CR0_PAGING ( 1 << 31 )

#define CR4_PA_EXT ( 1 << 5 )

#define KERNEL_CR0_FLAGS \
(                        \
	CR0_PAGING |         \
	CR0_PROT_MODE |      \
	CR0_EXTENSION_TYPE   \
)

#define KERNEL_CR4_FLAGS ( CR4_PA_EXT )
