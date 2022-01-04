#pragma once

#include <ax_util/types.hh>
#include <ax_util/helpers.hh>

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
