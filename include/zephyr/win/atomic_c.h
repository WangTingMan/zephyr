#ifndef ZEPHYR_INCLUDE_WINDOWS_ATOMIC_C_H_
#define ZEPHYR_INCLUDE_WINDOWS_ATOMIC_C_H_

#include <zephyr/sys/atomic_types.h>

#ifdef __cplusplus
extern "C" {
#endif

atomic_val_t atomic_get( const atomic_t* target );

atomic_val_t atomic_and( atomic_t* target, atomic_val_t value );

atomic_val_t atomic_or( atomic_t* target, atomic_val_t value );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

