#pragma once

#ifdef KERNEL

#include <kernel/memory/heap/k_malloc.hh>
#define malloc k_malloc
#define free k_free

#else

#include <new>
#include <stdlib.h>

#endif
