#include "malloc.h"

void* operator new(size_t);
void* operator new(size_t, std::nothrow_t const &) noexcept;
void* operator new(size_t, std::align_val_t);
void* operator new(size_t, std::align_val_t, std::nothrow_t const&) noexcept;
void* operator new[](size_t size);
void* operator new[](size_t size, std::nothrow_t const &) noexcept;

void operator delete(void*, size_t) noexcept;
void operator delete(void*, size_t, std::align_val_t) noexcept;
void operator delete[](void*, size_t) noexcept;
