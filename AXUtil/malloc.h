#pragma once

#ifdef KERNEL

#include <Kernel/Memory/Heap/k_malloc.h>
#define malloc k_malloc
#define free k_free

#else

#include <new>
#include <stdlib.h>

#endif
