#include <zephyr/win/atomic_impl.h>
#include <zephyr/sys/atomic_types.h>
#include <zephyr/sys/atomic.h>

atomic_val_t atomic_get( const atomic_t* target )
{
    return atomic_get_impl(target);
}

atomic_val_t atomic_and( atomic_t* target, atomic_val_t value )
{
    return atomic_and_impl(target, value);
}

atomic_val_t atomic_or( atomic_t* target, atomic_val_t value )
{
    return atomic_or_impl(target, value);
}

atomic_val_t atomic_set( atomic_t* target, atomic_val_t value )
{
    return atomic_set_impl( target, value );
}

atomic_ptr_val_t atomic_ptr_set( atomic_ptr_t* target, atomic_ptr_val_t value )
{
    return atomic_ptr_set_impl( target, value );
}

atomic_ptr_val_t atomic_ptr_clear( atomic_ptr_t* target )
{
    return atomic_ptr_clear_impl( target );
}

atomic_val_t atomic_clear( atomic_t* target )
{
    return atomic_set( target, 0 );
}

bool atomic_cas( atomic_t* target, atomic_val_t old_value, atomic_val_t new_value )
{
    return atomic_cas_impl( target, old_value, new_value );
}

atomic_val_t atomic_add( atomic_t* target, atomic_val_t value )
{
    return atomic_add_impl( target, value );
}

atomic_val_t atomic_inc( atomic_t* target )
{
    return atomic_add(target, 1);
}

atomic_val_t atomic_dec( atomic_t* target )
{
    return atomic_add( target, -1 );
}

bool atomic_ptr_cas( atomic_ptr_t* target, atomic_ptr_val_t old_value,
    atomic_ptr_val_t new_value )
{
    int ret = atomic_ptr_cas_impl(target, old_value, new_value);
    return ret == 1 ? 1 : 0;
}

atomic_ptr_val_t atomic_ptr_get( const atomic_ptr_t* target )
{
    return atomic_ptr_get_impl(target);
}
