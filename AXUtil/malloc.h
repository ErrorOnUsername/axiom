#pragma once

#ifdef KERNEL

#include <Kernel/Memory/Heap/k_malloc.h>
#define malloc kmalloc
#define free kfree

#else

#include <new>
#include <stdlib.h>

#endif
