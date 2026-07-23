#pragma once
#include <stdint.h>

#define ACTIONS_PLATFORM 0
#define ZEPHYR_ORIGINAL  1

#define PLATFORM_TYPE ZEPHYR_ORIGINAL

/**
 * this file cannot include any header file from zephyr and any c++ headers.
 */
#ifdef __cplusplus
extern "C" {
#endif

enum
{
    tracked_task_queued,
    tracked_task_running,
};

typedef void (*function_type)( void* p1 );

struct k_thread;

struct k_thread* get_current_thread_info();

struct k_thread* create_k_thread( const char* a_name, uint64_t* id );

int post_task_to_thread
    (
    uint64_t a_thread_id,
    function_type a_task,
    void* a_parameters,
    uint32_t a_delay_time_in_milliseconds
    );

int post_task_to_thread_tackable
    (
    uint64_t a_thread_id,
    function_type a_task,
    void* a_parameters,
    uint32_t a_delay_time_in_milliseconds,
    uint64_t* a_track_id
    );

int cancel_tacked_task( uint64_t a_track_id );

int change_tacked_task_delay( uint64_t a_track_id, uint32_t a_delay );

void require_global_lock();

void release_global_lock();

int64_t get_system_up_time();

uint64_t common_timer_create_in_thread( uint64_t a_thread_id, function_type a_callback, void* a_user_data );

void set_timer_duration_in_thread( uint64_t timer_id, int duration, int _peroid_milliseconds );

void stop_timer_in_thread( uint64_t timer_id);

uint16_t allocate_one_mutex();
int require_mutex( uint16_t a_id, uint32_t timeout );
int release_mutex( uint16_t a_id );
int free_mutex( uint16_t a_id );
int free_condition_variable( uint16_t a_id );
uint16_t allocate_one_condition_variable();
int condition_variable_wait_for( uint16_t a_cond_id, uint16_t a_mutex_id, uint32_t a_timeout );
int condition_var_notify_all( uint16_t a_id );
int semaphone_require( uint16_t a_id, uint32_t a_timeout );
uint16_t allocate_one_semaphone();
int seamaphone_free( uint16_t a_id );
int seamaphone_release( uint16_t a_id, uint32_t count );
int semaphone_clear_count( uint16_t a_id );
int semaphone_get_count( uint16_t a_id );
int set_thread_name(const char* a_name, uint64_t a_thread_id);
void sleep_current_thread(uint64_t duration_in_us);
void init_zephyr_runtime();

int associate_signal_exist( uint64_t id );

uint64_t allocate_underlying_associate_signal( const char* a_name );

void trigger_associate_signal( uint64_t id );

void reset_associate_signal( uint64_t id );

void poll_event_init
    (
    uint64_t a_chain_up_signal_id,
    uint64_t a_chain_down_signal_id,
    uint32_t type,
    int mode
    );

int poll_event_
    (
    uint64_t a_signal_id,
    uint32_t* a_type
    );

void initialize_hci();
void initialize_net_pool();

struct k_thread* allocate_one_k_thread();
uint64_t get_underlying_id_for_thread( struct k_thread* );
void set_underlying_id_for_thread( struct k_thread*, uint64_t);
void free_resource( void* );
void final_log_sinker( int level, const char* file, uint32_t line, const char* log);

#ifdef __cplusplus
} /* extern "C" */
#endif

