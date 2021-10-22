#pragma once

namespace AX::Extra {

template<typename T>
auto declval() -> T;

template<typename...>
using void_type = void;

template<typename T, typename U>
inline constexpr bool is_same = false;
template<typename T>
inline constexpr bool is_same<T, T> = true;

template<typename From, typename To>
inline constexpr bool is_convertable = requires { declval<void (*)(To)>()(declval<From>()); };

// ==================
// | const          |
// ==================

template<class T>
using add_const = const T;

template<class T>
struct __remove_const {
	using Type = T;
};
template<class T>
struct __remove_const<const T> {
	using Type = T;
};
template<class T>
using remove_const = typename __remove_const<T>::Type;

// ==================
// | volatile       |
// ==================

template<class T>
struct __remove_volatile {
	using Type = T;
};
template<class T>
struct __remove_volatile<volatile T> {
	using Type = T;
};
template<typename T>
using remove_volatile = typename __remove_volatile<T>::Type;

// ==================
// | pointer        |
// ==================
template<class T>
struct __remove_pointer {
	using Type = T;
};
template<class T>
struct __remove_pointer<T*> {
	using Type = T;
};
template<class T>
struct __remove_pointer<T const*> {
	using Type = T;
};
template<class T>
struct __remove_pointer<T volatile*> {
	using Type = T;
};
template<class T>
struct __remove_pointer<T const volatile*> {
	using Type = T;
};
template<typename T>
using remove_pointer = typename __remove_pointer<T>::Type;


// ==================
// | reference      |
// ==================

template<typename T, typename = void>
struct __add_reference {
	using lvalue_type = T;
	using rvalue_type = T;
};
template<typename T>
struct __add_reference<T, void_type<T&>> {
	using lvalue_type = T&;
	using rvalue_type = T&&;
};
template<typename T>
using add_lvalue_reference = typename __add_reference<T>::lvalue_type;
template<typename T>
using add_rvalue_reference = typename __add_reference<T>::rvalue_type;

template<typename T>
struct __remove_reference {
	using Type = T;
};
template<class T>
struct __remove_reference<T&> {
	using Type = T;
};
template<class T>
struct __remove_reference<T&&> {
	using Type = T;
};
template<typename T>
using remove_reference = typename __remove_reference<T>::Type;

template<typename T>
inline constexpr bool is_lvalue_reference = false;
template<typename T>
inline constexpr bool is_lvalue_reference<T&> = true;

// ==================
// | const volatile |
// ==================

template<class T>
using remove_cv = remove_volatile<remove_const<T>>;
template<typename T>
using remove_cv_reference = remove_cv<remove_reference<T>>;

// ==================
// | constructors   |
// ==================

template<typename T, typename... Args>
inline constexpr bool is_constructible = requires { ::new T(declval<Args>()...); };
template<typename T, typename... Args>
inline constexpr bool is_trivially_constructible = __is_trivially_constructible(T, Args...);

template<typename T>
inline constexpr bool is_copy_constructible = is_constructible<T, add_lvalue_reference<add_const<T>>>;
template<typename T>
inline constexpr bool is_trivially_copy_constructible = is_trivially_constructible<T, add_lvalue_reference<add_const<T>>>;

template<typename T>
inline constexpr bool is_move_constructible = is_constructible<T, add_rvalue_reference<T>>;
template<typename T>
inline constexpr bool is_trivially_move_constructible = is_trivially_constructible<T, add_rvalue_reference<T>>;

// ==================
// | destructors    |
// ==================

template<typename T>
inline constexpr bool is_destructible = requires { declval<T>().~T(); };
template<typename T>
inline constexpr bool is_trivially_destructible = __has_trivial_destructor(T) && is_destructible<T>;

}
