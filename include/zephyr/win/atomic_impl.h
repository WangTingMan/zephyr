#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

long atomic_get_impl( const long* target );

long atomic_and_impl( long* target, long value );

long atomic_add_impl( long* target, long value );

long atomic_or_impl( long* target, long value );

long atomic_set_impl( long* target, long value );

void* atomic_ptr_set_impl( void* target, void* value );

void* atomic_ptr_clear_impl( void* target );

int atomic_cas_impl( long* target, long old_value, long new_value );

int atomic_ptr_cas_impl( void* target, void* old_value,
    void* new_value );

void* atomic_ptr_get_impl( const void** target );

#ifdef __cplusplus
} /* extern "C" */
#endif
