#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log_ctrl.h>
#include <zephyr/win/kernel_support_impl.h>
#include <zephyr/win/queue_support_impl.h>
#include <windows.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/logging/log.h>

#include "vadefs.h"
#include "stdarg.h"

extern uint16_t s_irq_mutex_id;
logger_sink_type s_log_sinker = NULL;
struct k_work_q k_sys_work_q;

void work_detail_handler( struct k_work* work );

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
    /*we do not need this on windows to require os do not schedule other thread to run*/
}

void k_sched_unlock( void )
{
    /*we do not need this on windows to require os do not schedule other thread to run*/
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
    final_log_sinker(10, __FILE__, __LINE__, "fatal meet");
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

void k_work_init( struct k_work* work, k_work_handler_t handler )
{
    free_mutex(work->underlying_mutex_id);
    work->underlying_mutex_id = allocate_one_mutex();
    int status = require_mutex( work->underlying_mutex_id, 0xFFFFFFFF );
    work->handler = handler;
    release_mutex(work->underlying_mutex_id);
}

int k_work_schedule_for_queue
    (
    struct k_work_q* queue,
    struct k_work_delayable* dwork,
    k_timeout_t delay
    )
{
    struct k_work* work = &dwork->work;

    uint64_t id = 0x00;
    int status = post_task_to_thread_tackable
        (
        queue->thread_id->thread_id,
        work->handler,
        work,
        delay.ticks,
        &id
        );

    dwork->tracked_id = id;
    return status;
}

int k_work_schedule
    (
    struct k_work_delayable* dwork,
    k_timeout_t delay
    )
{
    int ret = k_work_schedule_for_queue( &k_sys_work_q, dwork, delay );
    return ret;
}

void k_work_init_delayable
    (
    struct k_work_delayable* dwork,
    k_work_handler_t handler
    )
{
    memset(dwork, 0x00, sizeof( struct k_work_delayable ));
    dwork->work.handler = handler;
    dwork->work.flags = K_WORK_DELAYABLE;
    dwork->tracked_id = 0x00;
}

int k_work_cancel_delayable( struct k_work_delayable* dwork )
{
    cancel_tacked_task( dwork->tracked_id );
    return 0;
}

int k_work_reschedule
    (
    struct k_work_delayable* dwork,
    k_timeout_t delay
    )
{
    return change_tacked_task_delay(dwork->tracked_id, delay.ticks);
}

int k_work_delayable_busy_get( const struct k_work_delayable* dwork )
{
    // TODO
    return 0;
}

bool k_work_cancel_delayable_sync
    (
    struct k_work_delayable* dwork,
    struct k_work_sync* sync
    )
{
    dwork->tracked_id;
    cancel_tacked_task( dwork->tracked_id );
    return true;
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
    trigger_associate_signal( queue->kobj.associate_signal_id );
}

int k_queue_is_empty( struct k_queue* queue )
{
    return queue_is_empty(queue->underlying_id);
}

void* k_queue_get( struct k_queue* queue, k_timeout_t timeout )
{
    void* data = queue_get( queue->underlying_id, timeout.ticks );
    if( queue_is_empty( queue->underlying_id ) )
    {
        /* since we do not reset the signal in queue_get, then there is a condition race rsik*/
        LOG_DBG( "reset associate sigal with id %d", queue->kobj.associate_signal_id );
        reset_associate_signal( queue->kobj.associate_signal_id );
    }

    if( data == NULL )
    {
        return data;
    }
    return data;
}

void k_queue_init( struct k_queue* queue )
{
    queue->underlying_id = queue_init(queue->underlying_id);
    queue->kobj.associate_signal_id = allocate_underlying_associate_signal( NULL );
    LOG_DBG( "allocate associate sigal with id %d", queue->kobj.associate_signal_id );
}

void k_queue_append( struct k_queue* queue, void* data )
{
    queue_append(queue->underlying_id,data);
    trigger_associate_signal( queue->kobj.associate_signal_id );
}

void* k_queue_peek_head( struct k_queue* queue )
{
    return queue_peek_head(queue->underlying_id);
}

int k_queue_append_list( struct k_queue* queue, void* a_head, void* a_tail )
{
    if( a_head == NULL || a_tail == NULL )
    {
        return -EINVAL;
    }

    if( associate_signal_exist( queue->kobj.associate_signal_id ) != 0 )
    {
        queue->kobj.associate_signal_id = allocate_underlying_associate_signal( NULL );
    }

    sys_dnode_t* head = ( sys_dnode_t* )a_head;
    sys_dnode_t* tail = ( sys_dnode_t* )a_tail;
    do
    {
        if( head == NULL )
        {
            break;
        }

        k_queue_append( queue, head );
        if( head == tail )
        {
            break;
        }

        head = head->next;
    } while( true );
    return 0;
}

int k_work_submit_to_queue
    (
    struct k_work_q* queue,
    struct k_work* work
    )
{
    int status = -90;
    k_work_handler_t detail_handler = NULL;
    detail_handler = work->handler;
    if( !detail_handler )
    {
        status = -90;
        return status;
    }

    if( associate_signal_exist( work->associstate_signal_id ) )
    {
        work->associstate_signal_id = allocate_underlying_associate_signal(NULL);
    }
    reset_associate_signal( work->associstate_signal_id );
    status = post_task_to_thread(queue->thread_id->thread_id, work_detail_handler, work, 0);
    return status;
}

bool k_work_flush( struct k_work* work,
    struct k_work_sync* sync )
{
    if( associate_signal_exist( work->associstate_signal_id ) )
    {
        uint32_t type = 0x00;
        poll_event_( work->associstate_signal_id,&type );
        return true;
    }
    return false;
}

void k_work_queue_init( struct k_work_q* queue )
{
    if( NULL == queue->thread_id )
    {
        uint64_t detail_id = 0x00;
        struct k_thread* kt = create_k_thread( "no_name", &detail_id );
        kt->thread_id = detail_id;
        queue->thread_id = kt;
    }
}

int k_work_reschedule_for_queue
    (
    struct k_work_q* queue,
    struct k_work_delayable* dwork,
    k_timeout_t delay
    )
{
    return NULL;
}

void k_work_queue_start( struct k_work_q* queue,
    k_thread_stack_t* stack, size_t stack_size,
    int prio, const struct k_work_queue_config* cfg )
{
    if( queue->thread_id == NULL )
    {
        uint64_t detail_id = 0x00;
        k_tid_t kt = create_k_thread( NULL, &detail_id );
        kt->thread_id = detail_id;
        queue->thread_id = kt;
        if( cfg && cfg->name )
        {
            set_thread_name( cfg->name, detail_id );
        }
    }
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

typedef struct __thread_entry_parameters
{
    k_thread_entry_t entry;
    void* p1;
    void* p2;
    void* p3;
} thread_entry_parameters_t;

void thread_entry_detail( thread_entry_parameters_t* a_paramter )
{
    k_thread_entry_t detail_entry = a_paramter->entry;
    void* p1 = a_paramter->p1;
    void* p2 = a_paramter->p2;
    void* p3 = a_paramter->p3;
    free( a_paramter );
    detail_entry( p1, p2, p3 );
}

k_tid_t k_thread_create
    (
    struct k_thread* new_thread,
    k_thread_stack_t* stack,
    size_t stack_size,
    k_thread_entry_t entry,
    void* p1, void* p2, void* p3,
    int prio, uint32_t options, k_timeout_t delay
    )
{
    uint64_t detail_id = 0x00;
    struct k_thread* kt = create_k_thread( "unknown", &detail_id );
    kt->thread_id = detail_id;

    thread_entry_parameters_t* paramter = malloc( sizeof( thread_entry_parameters_t ) );
    if( paramter )
    {
        paramter->entry = entry;
        paramter->p1 = p1;
        paramter->p2 = p2;
        paramter->p3 = p3;
        post_task_to_thread( detail_id, thread_entry_detail, paramter, 0 );
    }
    return kt;
}

void k_kernel_init()
{
    // 1. create system work queue.
    if( k_sys_work_q.thread_id == NULL )
    {
        uint64_t detail_id = 0x00;
        struct k_thread* kt = create_k_thread( "sysworkq", &detail_id );
        kt->thread_id = detail_id;
        k_sys_work_q.thread_id = kt;
    }

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

void work_detail_handler( struct k_work* work )
{
    k_work_handler_t hdr = work->handler;
    int status = require_mutex( work->underlying_mutex_id, 0xFFFFFF );
    work->flags = K_WORK_RUNNING;
    release_mutex(work->underlying_mutex_id);


    if( hdr )
    {
        reset_associate_signal( work->associstate_signal_id );
        hdr(work);
        trigger_associate_signal( work->associstate_signal_id );
    }
}

void k_timer_init
    (
    struct k_timer* timer,
    k_timer_expiry_t expiry_fn,
    k_timer_stop_t stop_fn
    )
{
    memset(timer, 0x00, sizeof( struct k_timer ));
    timer->expiry_fn = expiry_fn;
    timer->stop_fn = stop_fn;
}

void k_timer_start
    (
    struct k_timer* timer,
    k_timeout_t duration,
    k_timeout_t period
    )
{
    uint64_t id = common_timer_create_in_thread( k_sys_work_q.thread_id, timer->expiry_fn, timer );
    timer->timer_id = id;
    set_timer_duration_in_thread(id, duration.ticks, period.ticks );
}

void k_timer_stop( struct k_timer* timer )
{
    stop_timer_in_thread(timer->timer_id);
    if( timer->stop_fn )
    {
        timer->stop_fn(timer);
    }
}

void k_msgq_init( struct k_msgq* msgq, char* buffer, size_t msg_size,
    uint32_t max_msgs )
{
    msgq->underlying_id = queue_init( msgq->underlying_id );
    queue_set_element_size( msgq->underlying_id, msg_size );
}

int k_msgq_cleanup( struct k_msgq* msgq )
{
    // TODO
    return 0;
}

int k_msgq_put( struct k_msgq* msgq, const void* data, k_timeout_t timeout )
{
    uint32_t size = queue_get_element_size( msgq->underlying_id );
    char* msg = malloc( size );
    if( msg )
    {
        memcpy( msg, data, size );
        queue_append( msgq->underlying_id, msg );
    }
    return 0;
}

int k_msgq_get( struct k_msgq* msgq, void* data, k_timeout_t timeout )
{
    uint32_t size = queue_get_element_size( msgq->underlying_id );
    void* msg_data = queue_get( msgq->underlying_id, timeout.ticks );
    if( msg_data )
    {
        memcpy( data, msg_data, size );
        free( msg_data );
        return 0;
    }
    return -90;
}

void k_poll_event_init
    (
    struct k_poll_event* event,
    uint32_t type,
	int mode,
    void* obj
    )
{
    kernel_object* ko = (kernel_object*)( obj );
    uint64_t down_signal_id = event->kobj.associate_signal_id;
    if( associate_signal_exist( down_signal_id ) != 0 )
    {
        down_signal_id = allocate_underlying_associate_signal( NULL );
        event->kobj.associate_signal_id = down_signal_id;
    }

    uint64_t up_signal_id = ko->associate_signal_id;
    if( up_signal_id == 0x00 )
    {
        int sleep_ms = 50;
        k_usleep( 50 * 1000);
        up_signal_id = ko->associate_signal_id;
    }
    poll_event_init(up_signal_id, down_signal_id, type, mode );
}

int64_t k_uptime_ticks( void )
{
    return get_system_up_time();
}

int k_poll( struct k_poll_event* events, int num_events,
    k_timeout_t timeout )
{
    uint32_t type;
    int st = 0;
    st = poll_event_( events->kobj.associate_signal_id, &type );

    if( Z_POLL_TYPE_BIT( _POLL_TYPE_DATA_AVAILABLE ) == type )
    {
        events->poll_event_state = K_POLL_STATE_FIFO_DATA_AVAILABLE;
    }
    return st;
}

void k_poll_signal_init( struct k_poll_signal* sig )
{
    sig->kobj.associate_signal_id = allocate_underlying_associate_signal( NULL );
}

void k_poll_signal_reset( struct k_poll_signal* sig )
{
    trigger_associate_signal( sig->kobj.associate_signal_id );
}

int k_poll_signal_raise( struct k_poll_signal* sig, int result )
{
    // TODO
    return 0;
}

int k_thread_join( struct k_thread* thread, k_timeout_t timeout )
{
    // TODO
    return 0;
}

int k_thread_priority_get( k_tid_t thread )
{
    // TODO
    return 0;
}

const char* k_thread_name_get( k_tid_t thread )
{
    // TODO
    return 0;
}

int64_t sys_clock_tick_get( void )
{
    return get_system_up_time();
}

uint32_t sys_clock_tick_get_32( void )
{
    return get_system_up_time();
}

int wdt_feed( const struct device* dev, int channel_id )
{
    // TODO
    return 0;
}

void k_thread_priority_set( k_tid_t thread, int prio )
{
    // TODO
}

void k_mbox_async_put( struct k_mbox* mbox, struct k_mbox_msg* tx_msg,
    struct k_sem* sem )
{
    // TODO
}

int k_mbox_get( struct k_mbox* mbox, struct k_mbox_msg* rx_msg,
    void* buffer, k_timeout_t timeout )
{
    // TODO
    return 0;
}

k_timepoint_t sys_timepoint_calc( k_timeout_t timeout )
{
    k_timepoint_t ret;
    uint64_t now_ = get_system_up_time();
    now_ += timeout.ticks;
    ret.tick = now_;
    return ret;
}

k_timeout_t sys_timepoint_timeout( k_timepoint_t timepoint )
{
    k_timeout_t time;
    uint64_t now_ = get_system_up_time();
    time.ticks = timepoint.tick - now_;
    return time;
}

