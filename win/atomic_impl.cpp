#include <zephyr/win/atomic_impl.h>
#include <atomic>

#ifdef __cplusplus
extern "C" {
#endif

long atomic_get_impl( const long* target )
{
    return std::atomic_ref<const long>( *target ).load( std::memory_order_seq_cst );
}

long atomic_and_impl( long* target, long value )
{
    return std::atomic_ref<long>( *target ).fetch_and( value, std::memory_order_seq_cst );
}

long atomic_add_impl( long* target, long value )
{
    return std::atomic_ref<long>( *target ).fetch_add( value, std::memory_order_seq_cst );
}

long atomic_or_impl( long* target, long value )
{
    return std::atomic_ref<long>( *target ).fetch_or( value, std::memory_order_seq_cst );
}

long atomic_set_impl( long* target, long value )
{
    return std::atomic_ref<long>( *target ).exchange( value, std::memory_order_seq_cst );
}

void* atomic_ptr_set_impl( void* target, void* value )
{
    void** actual_target = reinterpret_cast< void** >( target );
    return std::atomic_ref<void*>(*actual_target).exchange( value, std::memory_order_seq_cst );
}

void* atomic_ptr_clear_impl( void* target )
{
    return atomic_ptr_set_impl(target, NULL);
}

int atomic_cas_impl( long* target, long old_value, long new_value )
{
    auto atomic_target = std::atomic_ref<long>( *target );
    auto ret = atomic_target.compare_exchange_strong( old_value, new_value,
        std::memory_order_seq_cst,
        std::memory_order_seq_cst );
    return ret ? 1 : 0;
}

int atomic_ptr_cas_impl( void* target, void* old_value,
    void* new_value )
{
    void** actual_target = reinterpret_cast< void** >( target );
    auto atomic_target = std::atomic_ref<void*>( *actual_target );
    void* expected = old_value;
    auto ret = atomic_target.compare_exchange_strong( expected, new_value,
        std::memory_order_seq_cst,
        std::memory_order_seq_cst );
    return ret ? 1 : 0;
}

void* atomic_ptr_get_impl( const void** target )
{
    const void** actual_target = reinterpret_cast< const void** >( target );
    return const_cast< void* >( std::atomic_ref<const void*>( *actual_target ).load( std::memory_order_seq_cst ) );
}

#ifdef __cplusplus
} /* extern "C" */
#endif
