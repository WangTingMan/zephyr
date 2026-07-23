/*
 * Copyright (c) 2021 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_TOOLCHAIN_LLVM_H_
#define ZEPHYR_INCLUDE_TOOLCHAIN_LLVM_H_

#include <global_configs.h>

#ifndef ZEPHYR_INCLUDE_TOOLCHAIN_H_
#error Please do not include toolchain-specific headers directly, use <zephyr/toolchain.h> instead
#endif

#ifndef _LINKER

#ifndef __ORDER_BIG_ENDIAN__
#define __ORDER_BIG_ENDIAN__            (1)
#endif

#ifndef __ORDER_LITTLE_ENDIAN__
#define __ORDER_LITTLE_ENDIAN__         (2)
#endif

#ifndef __BYTE_ORDER__
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif

#endif

#ifndef CONFIG_LITTLE_ENDIAN
#define CONFIG_LITTLE_ENDIAN
#endif

#ifndef CONFIG_SYS_CLOCK_EXISTS
#define CONFIG_SYS_CLOCK_EXISTS
#endif

#define _DO_CONCAT_DETAIL_FOR_LINK(y) ptr_ ## y
#ifndef _CONCAT_PTR_PRE
#define _CONCAT_PTR_PRE(y) _DO_CONCAT_DETAIL_FOR_LINK(y)
#endif

#pragma section(".CRT$XCU", read)
#define REGISTER_PRE_MAIN(func_name) \
    __declspec(allocate(".CRT$XCU")) void (*_CONCAT_PTR_PRE(func_name))(void) = func_name;

#define __no_optimization __attribute__((optnone))

#define CONFIG_SETTINGS_ENCODE_LEN

#ifndef __CHAR_BIT__
#define __CHAR_BIT__ 8
#endif

#ifndef __SIZEOF_LONG__
#define __SIZEOF_LONG__ sizeof(long)
#endif

#ifndef __SIZEOF_LONG_LONG__
#define __SIZEOF_LONG_LONG__ sizeof(long long)
#endif

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE __forceinline
#endif

#ifndef ARG_UNUSED
#define ARG_UNUSED(...)
#endif

#ifndef ssize_t
#define ssize_t int
#endif

#ifndef __syscall
#define __syscall
#endif

#define __builtin_clzl(x) \
    ((x) == 0 ? 64 : \
    ((x) & 0xFFFFFFFF00000000ULL) ? \
        (((x) & 0xFFFF000000000000ULL) ? \
            (((x) & 0xFF00000000000000ULL) ? \
                (((x) & 0xF000000000000000ULL) ? (((x) & 0xC000000000000000ULL) ? (((x) & 0x8000000000000000ULL) ? 0 : 1) : (((x) & 0x2000000000000000ULL) ? 2 : 3)) : (((x) & 0x0C00000000000000ULL) ? (((x) & 0x0800000000000000ULL) ? 4 : 5) : (((x) & 0x0200000000000000ULL) ? 6 : 7))) : \
            (((x) & 0x00F0000000000000ULL) ? (((x) & 0x00C0000000000000ULL) ? (((x) & 0x0080000000000000ULL) ? 8 : 9) : (((x) & 0x0020000000000000ULL) ? 10 : 11)) : (((x) & 0x000C000000000000ULL) ? (((x) & 0x0008000000000000ULL) ? 12 : 13) : (((x) & 0x0002000000000000ULL) ? 14 : 15)))) : \
        (((x) & 0x0000FF0000000000ULL) ? \
            (((x) & 0x0000F00000000000ULL) ? (((x) & 0x0000C00000000000ULL) ? (((x) & 0x0000800000000000ULL) ? 16 : 17) : (((x) & 0x0000200000000000ULL) ? 18 : 19)) : (((x) & 0x00000C0000000000ULL) ? (((x) & 0x0000080000000000ULL) ? 20 : 21) : (((x) & 0x0000020000000000ULL) ? 22 : 23))) : \
        (((x) & 0x000000F000000000ULL) ? (((x) & 0x000000C000000000ULL) ? (((x) & 0x0000008000000000ULL) ? 24 : 25) : (((x) & 0x0000002000000000ULL) ? 26 : 27)) : (((x) & 0x0000000C00000000ULL) ? (((x) & 0x0000000800000000ULL) ? 28 : 29) : (((x) & 0x0000000200000000ULL) ? 30 : 31))))) : \
    ((x) & 0x00000000FFFF0000ULL) ? \
        (((x) & 0x00000000FF000000ULL) ? \
            (((x) & 0x00000000F0000000ULL) ? (((x) & 0x00000000C0000000ULL) ? (((x) & 0x0000000080000000ULL) ? 32 : 33) : (((x) & 0x0000000020000000ULL) ? 34 : 35)) : (((x) & 0x000000000C000000ULL) ? (((x) & 0x0000000008000000ULL) ? 36 : 37) : (((x) & 0x0000000002000000ULL) ? 38 : 39))) : \
            (((x) & 0x0000000000F00000ULL) ? (((x) & 0x0000000000C00000ULL) ? (((x) & 0x0000000000800000ULL) ? 40 : 41) : (((x) & 0x0000000000200000ULL) ? 42 : 43)) : (((x) & 0x00000000000C0000ULL) ? (((x) & 0x0000000000080000ULL) ? 44 : 45) : (((x) & 0x0000000000020000ULL) ? 46 : 47)))) : \
    (((x) & 0x000000000000FF00ULL) ? \
        (((x) & 0x000000000000F000ULL) ? (((x) & 0x000000000000C000ULL) ? (((x) & 0x0000000000008000ULL) ? 48 : 49) : (((x) & 0x0000000000002000ULL) ? 50 : 51)) : (((x) & 0x0000000000000C00ULL) ? (((x) & 0x0000000000000800ULL) ? 52 : 53) : (((x) & 0x0000000000000200ULL) ? 54 : 55))) : \
    (((x) & 0x00000000000000F0ULL) ? \
        (((x) & 0x00000000000000C0ULL) ? (((x) & 0x0000000000000080ULL) ? 56 : 57) : (((x) & 0x0000000000000020ULL) ? 58 : 59)) : \
    (((x) & 0x000000000000000CULL) ? (((x) & 0x0000000000000008ULL) ? 60 : 61) : (((x) & 0x0000000000000002ULL) ? 62 : 63)))))

#define Z_IS_POW2(x) (((x) != 0) && (((x) & ((x)-1)) == 0))
#define Z_POW2_CEIL(x) ((x) <= 2UL ? (x) : (1UL << (8 * sizeof(long) - __builtin_clzl((x) - 1))))

#ifndef __fallthrough
#if __clang_major__ >= 10
#define __fallthrough __attribute__((fallthrough))
#endif
#endif

#define TOOLCHAIN_CLANG_VERSION \
	((__clang_major__ * 10000) + (__clang_minor__ * 100) + \
	  __clang_patchlevel__)

#define TOOLCHAIN_HAS_PRAGMA_DIAG 1

#if TOOLCHAIN_CLANG_VERSION >= 30800
#define TOOLCHAIN_HAS_C_GENERIC 1
#define TOOLCHAIN_HAS_C_AUTO_TYPE 1
#endif

/* clear out common version. The build assert assert from gcc.h is defined to be empty */
#undef BUILD_ASSERT

#if defined(__cplusplus) && (__cplusplus >= 201103L)

/* C++11 has static_assert built in */
#define BUILD_ASSERT(EXPR, ...) static_assert((EXPR), #__VA_ARGS__)

#elif !defined(__cplusplus) && ((__STDC_VERSION__) >= 201100)

/* C11 has static_assert built in */
#define BUILD_ASSERT(EXPR, ...) _Static_assert((EXPR), #__VA_ARGS__)

#else

/* Rely on that the C-library provides a static assertion function */
#define BUILD_ASSERT(EXPR, MSG...) _Static_assert((EXPR), "" MSG)

#endif

/** @cond INTERNAL_HIDDEN */
#define TOOLCHAIN_WARNING_SIZEOF_ARRAY_DECAY            "-Wsizeof-array-decay"
#define TOOLCHAIN_WARNING_UNNEEDED_INTERNAL_DECLARATION "-Wunneeded-internal-declaration"
#define TOOLCHAIN_WARNING_USED_BUT_MARKED_UNUSED        "-Wused-but-marked-unused"
#define TOOLCHAIN_WARNING_UNALIGNED_ACCESS              "-Wunaligned-access"
#define TOOLCHAIN_WARNING_ARM_INTERRUPT_VFP_CLOBBER     "-Warm-interrupt-vfp-clobber"
/** @endcond */

#define TOOLCHAIN_DISABLE_CLANG_WARNING(warning) _TOOLCHAIN_DISABLE_WARNING(clang, warning)
#define TOOLCHAIN_ENABLE_CLANG_WARNING(warning)  _TOOLCHAIN_ENABLE_WARNING(clang, warning)

/*
 * Provide these definitions only when minimal libc is used.
 * Avoid collision with defines from include/zephyr/toolchain/zephyr_stdint.h
 */
#ifdef CONFIG_MINIMAL_LIBC

/*
 * Predefined __INTN_C/__UINTN_C macros are provided by clang starting in version 20.1.
 * Avoid redefining these macros if a sufficiently modern clang is being used.
 */
#if TOOLCHAIN_CLANG_VERSION < 200100

#define __int_c(v, suffix) v ## suffix
#define int_c(v, suffix) __int_c(v, suffix)
#define uint_c(v, suffix) __int_c(v ## U, suffix)

#ifndef CONFIG_ENFORCE_ZEPHYR_STDINT

#ifdef __INT64_TYPE__
#undef __int_least64_c_suffix__
#undef __int_least32_c_suffix__
#undef __int_least16_c_suffix__
#undef __int_least8_c_suffix__
#ifdef __INT64_C_SUFFIX__
#define __int_least64_c_suffix__ __INT64_C_SUFFIX__
#define __int_least32_c_suffix__ __INT64_C_SUFFIX__
#define __int_least16_c_suffix__ __INT64_C_SUFFIX__
#define __int_least8_c_suffix__ __INT64_C_SUFFIX__
#endif /* __INT64_C_SUFFIX__ */
#endif /* __INT64_TYPE__ */

#ifdef __INT_LEAST64_TYPE__
#ifdef __int_least64_c_suffix__
#define __INT64_C(x)	int_c(x, __int_least64_c_suffix__)
#define __UINT64_C(x)	uint_c(x, __int_least64_c_suffix__)
#else
#define __INT64_C(x)	x
#define __UINT64_C(x)	x ## U
#endif /* __int_least64_c_suffix__ */
#endif /* __INT_LEAST64_TYPE__ */

#ifdef __INT32_TYPE__
#undef __int_least32_c_suffix__
#undef __int_least16_c_suffix__
#undef __int_least8_c_suffix__
#ifdef __INT32_C_SUFFIX__
#define __int_least32_c_suffix__ __INT32_C_SUFFIX__
#define __int_least16_c_suffix__ __INT32_C_SUFFIX__
#define __int_least8_c_suffix__ __INT32_C_SUFFIX__
#endif /* __INT32_C_SUFFIX__ */
#endif /* __INT32_TYPE__ */

#ifdef __INT_LEAST32_TYPE__
#ifdef __int_least32_c_suffix__
#define __INT32_C(x)	int_c(x, __int_least32_c_suffix__)
#define __UINT32_C(x)	uint_c(x, __int_least32_c_suffix__)
#else
#define __INT32_C(x)	x
#define __UINT32_C(x)	x ## U
#endif /* __int_least32_c_suffix__ */
#endif /* __INT_LEAST32_TYPE__ */

#endif /* !CONFIG_ENFORCE_ZEPHYR_STDINT */

#ifdef __INT16_TYPE__
#undef __int_least16_c_suffix__
#undef __int_least8_c_suffix__
#ifdef __INT16_C_SUFFIX__
#define __int_least16_c_suffix__ __INT16_C_SUFFIX__
#define __int_least8_c_suffix__ __INT16_C_SUFFIX__
#endif /* __INT16_C_SUFFIX__ */
#endif /* __INT16_TYPE__ */

#ifdef __INT_LEAST16_TYPE__
#ifdef __int_least16_c_suffix__
#define __INT16_C(x)	int_c(x, __int_least16_c_suffix__)
#define __UINT16_C(x)	uint_c(x, __int_least16_c_suffix__)
#else
#define __INT16_C(x)	x
#define __UINT16_C(x)	x ## U
#endif /* __int_least16_c_suffix__ */
#endif /* __INT_LEAST16_TYPE__ */

#ifdef __INT8_TYPE__
#undef __int_least8_c_suffix__
#ifdef __INT8_C_SUFFIX__
#define __int_least8_c_suffix__ __INT8_C_SUFFIX__
#endif /* __INT8_C_SUFFIX__ */
#endif /* __INT8_TYPE__ */

#ifdef __INT_LEAST8_TYPE__
#ifdef __int_least8_c_suffix__
#define __INT8_C(x)	int_c(x, __int_least8_c_suffix__)
#define __UINT8_C(x)	uint_c(x, __int_least8_c_suffix__)
#else
#define __INT8_C(x)	x
#define __UINT8_C(x)	x ## U
#endif /* __int_least8_c_suffix__ */
#endif /* __INT_LEAST8_TYPE__ */

#define __INTMAX_C(x)	int_c(x, __INTMAX_C_SUFFIX__)
#define __UINTMAX_C(x)	int_c(x, __UINTMAX_C_SUFFIX__)

#endif /* TOOLCHAIN_CLANG_VERSION < 200100 */

#endif /* CONFIG_MINIMAL_LIBC */

#endif /* ZEPHYR_INCLUDE_TOOLCHAIN_LLVM_H_ */
