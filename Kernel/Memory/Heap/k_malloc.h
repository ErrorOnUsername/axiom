#pragma once

#include <AXUtil/Types.h>
#include <AXUtil/Helpers.h>

namespace std {

struct nothrow_t {
	explicit nothrow_t() = default;
};

extern const nothrow_t nothrow;

enum class align_val_t : size_t { };

}

void k_malloc_init();

[[gnu::malloc, gnu::alloc_size(1)]]void* k_malloc(size_t);
void k_free(void*);
void k_free_sized(void*, size_t);

inline void* operator new(size_t, void* ptr) { return ptr; }
inline void* operator new[](size_t, void* ptr) { return ptr; }

void* operator new(size_t);
void* operator new(size_t, std::nothrow_t const &) noexcept;
void* operator new(size_t, std::align_val_t);
void* operator new(size_t, std::align_val_t, std::nothrow_t const&) noexcept;
void* operator new[](size_t size);
void* operator new[](size_t size, std::nothrow_t const &) noexcept;

void operator delete(void*) noexcept ERROR("All calls to k_free must have a size!");
void operator delete(void*, size_t) noexcept;
void operator delete(void*, std::align_val_t) noexcept ERROR("All calls to k_free must have a size!");
void operator delete(void*, size_t, std::align_val_t) noexcept;
void operator delete[](void*) noexcept ERROR("All calls to k_free must have a size!");
void operator delete[](void*, size_t) noexcept;
