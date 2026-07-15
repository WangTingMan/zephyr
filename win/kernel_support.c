#include <zephyr/kernel.h>
#include <zephyr/logging/log_ctrl.h>
#include <zephyr/win/kernel_support_impl.h>
#include <zephyr/win/queue_support_impl.h>
#include <windows.h>

#include "vadefs.h"
#include "stdarg.h"

extern uint16_t s_irq_mutex_id;
logger_sink_type s_log_sinker = NULL;
struct k_work_q k_sys_work_q;

struct k_thread* allocate_one_k_thread()
{
    struct k_thread* ptr = malloc(sizeof( struct k_thread ) );
    if( ptr )
    {
        ptr->has_onwership = 1;
        memset(ptr, 0x00, sizeof( struct k_thread ) );
    }
    return ptr;
}

void free_resource( void* ptr)
{
    free(ptr);
}

k_tid_t k_sched_current_thread_query( void )
{
    k_tid_t tid = get_current_thread_info();
    if( tid->thread_id == 0x00 )
    {
        tid->thread_id = GetCurrentThreadId();
    }
    return tid;
}

void k_thread_abort( k_tid_t thread )
{
    // it is not a good idea to kill a thread..
}

void k_sched_lock( void )
{
    require_global_lock();
}

void k_sched_unlock( void )
{
    release_global_lock();
}

int k_thread_name_set( k_tid_t thread, const char* str )
{
    if( thread->thread_id )
    {
        set_thread_name( str, thread->thread_id );
    }
    return 0;
}

bool device_is_ready( const struct device* dev )
{
    /* we always keep ready anytime! */
    return true;
}

int32_t k_sleep( k_timeout_t timeout )
{
    uint64_t ms = timeout.ticks;
    k_usleep(ms * 1000);
    // TODO need confirm that this is milliseconds.
    return 0;
}

int32_t k_usleep( int32_t us )
{
    sleep_current_thread(us);
    return 0;
}

int k_mutex_lock( struct k_mutex* mutex, k_timeout_t timeout )
{
    if( mutex->underlying_id == 0x00 )
    {
        mutex->underlying_id = allocate_one_mutex();
    }
    int status = require_mutex(mutex->underlying_id, timeout.ticks);
    return status == 1 ? 0 : -9;
}

int k_mutex_unlock( struct k_mutex* mutex )
{
    int status = release_mutex(mutex->underlying_id);
    return status == 1 ? 0 : -9;
}

int k_mutex_init( struct k_mutex* mutex )
{
    int status = 0;
    status = free_mutex(mutex->underlying_id);
    mutex->underlying_id = allocate_one_mutex();
    return 0;
}

uint32_t arch_k_cycle_get_32( void )
{
    return ( uint32_t )arch_k_cycle_get_64();
}

void k_yield( void )
{
    SwitchToThread();
}

void* k_heap_alloc( struct k_heap* h, size_t bytes,
    k_timeout_t timeout )
{
    return malloc(bytes);
}

void k_heap_free( struct k_heap* h, void* mem )
{
    free(mem);
}

void* k_heap_aligned_alloc( struct k_heap* h, size_t align, size_t bytes,
    k_timeout_t timeout )
{
    return malloc( bytes );
}

int k_mem_slab_alloc( struct k_mem_slab* slab, void** mem,
    k_timeout_t timeout )
{
    if( slab == NULL || mem == NULL ) {
        return -EINVAL;
    }

    uint32_t size = slab->info.block_size < 2048 ? slab->info.block_size : 2048;
    *mem = malloc( slab->info.block_size );

    if( *mem == NULL ) {
        return -ENOMEM;
    }
    return 0;
}

void k_mem_slab_free( struct k_mem_slab* slab, void* mem )
{
    free(mem);
}

int k_condvar_init( struct k_condvar* condvar )
{
    free_condition_variable(condvar->underlying_id);
    condvar->underlying_id = allocate_one_condition_variable();
    return 0;
}

int k_condvar_wait( struct k_condvar* condvar, struct k_mutex* mutex,
    k_timeout_t timeout )
{
    int status = 0;

    if( condvar->underlying_id == 0x00 )
    {
        condvar->underlying_id = allocate_one_condition_variable();
    }

    if( mutex->underlying_id == 0x00 )
    {
        mutex->underlying_id = allocate_one_condition_variable();
    }

    status = condition_variable_wait_for(condvar->underlying_id, mutex->underlying_id, timeout.ticks);

    return status;
}

int k_condvar_broadcast( struct k_condvar* condvar )
{
    if( condvar->underlying_id == 0x00 )
    {
        condvar->underlying_id = allocate_one_condition_variable();
    }
    return condition_var_notify_all(condvar->underlying_id);
}

int k_sem_take( struct k_sem* sem, k_timeout_t timeout )
{
    int status = 0;
    if( sem->underlying_id == 0x00 )
    {
        sem->underlying_id = allocate_one_semaphone();
    }
    status = semaphone_require(sem->underlying_id, timeout.ticks);
    return status;
}

void k_sem_give( struct k_sem* sem )
{
    if( sem->underlying_id == 0x00 )
    {
        sem->underlying_id = allocate_one_semaphone();
    }
    seamaphone_release(sem->underlying_id, 1);
}

void k_sem_reset( struct k_sem* sem )
{
    if( sem->underlying_id == 0x00 )
    {
        sem->underlying_id = allocate_one_semaphone();
    }
    semaphone_clear_count(sem->underlying_id);
}

unsigned int k_sem_count_get( struct k_sem* sem )
{
    if( sem->underlying_id == 0x00 )
    {
        sem->underlying_id = allocate_one_semaphone();
    }
    return semaphone_get_count(sem->underlying_id);
}

int k_sem_init( struct k_sem* sem, unsigned int initial_count,
    unsigned int limit )
{
    seamaphone_free(sem->underlying_id);
    sem->underlying_id = allocate_one_semaphone();
    seamaphone_release( sem->underlying_id, initial_count );
    return 0;
}

void z_fatal_error( unsigned int reason, const struct arch_esf* esf )
{
    // TODO: log fata stack.
}

bool k_is_in_isr( void )
{
    /* we do not have interrupt on windows.*/
    return false;
}

void arch_irq_unlock( unsigned int key )
{
    release_mutex(s_irq_mutex_id);
}

unsigned int arch_irq_lock( void )
{
    return require_mutex(s_irq_mutex_id, 0xFFFFFFFF);
}

int k_work_cancel( struct k_work* work )
{
    return 0;
}

bool k_work_cancel_sync( struct k_work* work, struct k_work_sync* sync )
{
    return true;
}

int k_work_schedule_for_queue( struct k_work_q* queue,
    struct k_work_delayable* dwork,
    k_timeout_t delay )
{
    return 0;
}

void k_work_init( struct k_work* work, k_work_handler_t handler )
{
    free_mutex(work->underlying_mutex_id);
    work->underlying_mutex_id = allocate_one_mutex();
    int status = require_mutex( work->underlying_mutex_id, 0xFFFFFFFF );
    work->handler = handler;
    release_mutex(work->underlying_mutex_id);
}

int k_work_schedule( struct k_work_delayable* dwork,
    k_timeout_t delay )
{
    auto id = dwork->queue->thread_id->thread_id;

    return 0;
}

void k_work_init_delayable( struct k_work_delayable* dwork,
    k_work_handler_t handler )
{

}

int k_work_cancel_delayable( struct k_work_delayable* dwork )
{
    return 0;
}

int k_work_reschedule( struct k_work_delayable* dwork,
    k_timeout_t delay )
{
    return 0;
}

int k_work_submit( struct k_work* work )
{
    int r = 0;
    r = k_work_submit_to_queue( &k_sys_work_q, work);
    return 0;
}

void k_queue_prepend( struct k_queue* queue, void* data )
{
    queue_prepend(queue->underlying_id, data);
}

int k_queue_is_empty( struct k_queue* queue )
{
    return queue_is_empty(queue->underlying_id);
}

void* k_queue_get( struct k_queue* queue, k_timeout_t timeout )
{
    return queue_get(queue->underlying_id, timeout.ticks);
}

void k_queue_init( struct k_queue* queue )
{
    queue->underlying_id = queue_init(queue->underlying_id);
}

void k_queue_append( struct k_queue* queue, void* data )
{
    queue_append(queue->underlying_id,data);
}

void* k_queue_peek_head( struct k_queue* queue )
{
    return queue_peek_head(queue->underlying_id);
}

int k_work_submit_to_queue( struct k_work_q* queue,
    struct k_work* work )
{
    int status = -90;
    k_work_handler_t detail_handler = NULL;
    detail_handler = work->handler;
    if( !detail_handler )
    {
        status = -90;
        return status;
    }

    status = post_task_to_thread(queue->thread_id->thread_id, detail_handler, work, 0);
    return status;
}

void k_work_queue_init( struct k_work_q* queue )
{

}

int k_work_delayable_busy_get( const struct k_work_delayable* dwork )
{
    return 0;
}

bool k_work_cancel_delayable_sync( struct k_work_delayable* dwork,
    struct k_work_sync* sync )
{
    return true;
}

int k_work_reschedule_for_queue( struct k_work_q* queue,
    struct k_work_delayable* dwork,
    k_timeout_t delay )
{
    return NULL;
}

void k_work_queue_start( struct k_work_q* queue,
    k_thread_stack_t* stack, size_t stack_size,
    int prio, const struct k_work_queue_config* cfg )
{
    uint64_t detail_id = 0x00;
    k_tid_t kt = create_k_thread( NULL, &detail_id );
    kt->thread_id = detail_id;
    queue->thread_id = kt;
}

void log_output_msg_process( const struct log_output* log_output,
    struct log_msg* msg, uint32_t flags )
{

}

int log_set_timestamp_func( log_timestamp_get_t timestamp_getter,
    uint32_t freq )
{
    return 0;
}

k_tid_t k_thread_create( struct k_thread* new_thread,
    k_thread_stack_t* stack,
    size_t stack_size,
    k_thread_entry_t entry,
    void* p1, void* p2, void* p3,
    int prio, uint32_t options, k_timeout_t delay )
{
    new_thread->has_onwership = 0;
    return new_thread;
}

void k_kernel_init()
{
    // 1. create system work queue.
    uint64_t detail_id = 0x00;
    struct k_thread* kt = create_k_thread( "sysworkq", &detail_id);
    kt->thread_id = detail_id;
    k_sys_work_q.thread_id = kt;

    // 2. 
}

uint64_t get_underlying_id_for_thread( struct k_thread* th )
{
    if( th )
    {
        return th->thread_id;
    }
    return 0;
}

void set_underlying_id_for_thread( struct k_thread* th, uint64_t a_id)
{
    th->thread_id = a_id;
}

#define LOG_BUF_SIZE 1024
int k__log_print
    (
    int level,
    const char* file,
    unsigned int line,
    const char* fmt,
    ...
    )
{
    char buf[LOG_BUF_SIZE] = { 0x00 };
    if( fmt )
    {
        va_list vaList;
        va_start( vaList, fmt );
        vsnprintf( buf, LOG_BUF_SIZE - 1, fmt, vaList );
        va_end( vaList );
    }

    if( s_log_sinker )
    {
        struct kk__log_message log;
        log.file = file;
        log.line = line;
        log.message = buf;
        log.priority = level;
        s_log_sinker( &log );
        return 0;
    }
    final_log_sinker(level, file, line, buf);

    return 0;
}

void k_set_log_sinker( logger_sink_type a_sinker )
{
    s_log_sinker = a_sinker;
}

void printk( const char* fmt, ... )
{
    char buf[LOG_BUF_SIZE] = { 0x00 };
    if( fmt )
    {
        va_list vaList;
        va_start( vaList, fmt );
        vsnprintf( buf, LOG_BUF_SIZE - 1, fmt, vaList );
        va_end( vaList );
    }
    printf(buf);
}
