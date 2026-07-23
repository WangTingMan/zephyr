#include <zephyr/win/kernel_support_impl.h>
#include <base/strings/sys_string_conversions.h>
#include <zephyr/win/associate_signal.h>
#include <zephyr/win/associate_signal_manager.h>
#include <zephyr/win/mutex_support.h>
#include <base/logging.h>
#include <chrono>
#include <memory>
#include <mutex>

#include <windows.h>

#if __has_include(<log/log.h>)
#include <log/log.h>
#define HAS_ANDROID_LIKE_LOG
#endif

std::recursive_mutex s_global_mutex;

class initializer
{

public:

    initializer();

};

initializer s_initializer;

#ifdef __cplusplus
extern "C" {
#endif

void require_global_lock()
{
    s_global_mutex.lock();
}

void release_global_lock()
{
#pragma warning(push)
#pragma warning(disable: 26110)
    s_global_mutex.unlock();
#pragma warning(pop)
}

int64_t get_system_up_time()
{
    std::chrono::steady_clock::time_point now_;
    now_ = std::chrono::steady_clock::now();
    auto dureation = now_.time_since_epoch();
    auto ms = std::chrono::duration_cast< std::chrono::milliseconds >( dureation );
    return ms.count();
}

uint64_t arch_k_cycle_get_64( void )
{
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( now.time_since_epoch() );
    return ( uint64_t )duration.count();
}

int set_thread_name( const char* a_name, uint64_t a_thread_id )
{
    HANDLE hdl = OpenThread( THREAD_ALL_ACCESS, FALSE, a_thread_id );
    if( NULL != hdl )
    {
        std::wstring thread_name = base::SysNativeMBToWide( a_name );
        SetThreadDescription( hdl, thread_name.c_str() );
        CloseHandle( hdl );
        return 0;
    }

    return -90;
}

void sleep_current_thread( uint64_t duration_in_us )
{
    std::this_thread::sleep_for(std::chrono::microseconds(duration_in_us));
}

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERR  1
#define LOG_LEVEL_WRN  2
#define LOG_LEVEL_INF  3
#define LOG_LEVEL_DBG  4
#define LOG_LEVEL_FATAL 10

void final_log_sinker( int level, const char* file, uint32_t line, const char* log )
{
#ifdef HAS_ANDROID_LIKE_LOG
    android_LogPriority pro = ANDROID_LOG_VERBOSE;
    switch( level )
    {
    case LOG_LEVEL_NONE:
        pro = ANDROID_LOG_VERBOSE;
        break;
    case LOG_LEVEL_ERR:
        pro = ANDROID_LOG_ERROR;
    break;
    case LOG_LEVEL_WRN:
        pro = ANDROID_LOG_WARN;
    break;
    case LOG_LEVEL_INF:
        pro = ANDROID_LOG_INFO;
    break;
    case LOG_LEVEL_DBG:
        pro = ANDROID_LOG_DEBUG;
    break;
    default:
    break;
    }
    __android_log_print_ext(pro, "", file, line, log);
    return;
#endif

    logging::LogSeverity log_severity = logging::LOG_VERBOSE;
    switch( level )
    {
    case LOG_LEVEL_NONE:
        log_severity = logging::LOG_VERBOSE;
    break;
    case LOG_LEVEL_ERR:
        log_severity = logging::LOG_ERROR;
    break;
    case LOG_LEVEL_WRN:
        log_severity = logging::LOG_WARNING;
    break;
    case LOG_LEVEL_INF:
        log_severity = logging::LOG_INFO;
    break;
    case LOG_LEVEL_DBG:
        log_severity = logging::LOG_INFO;
    break;
    case LOG_LEVEL_FATAL:
        log_severity = logging::LOG_FATAL;
    break;
    default:
    break;
    }
    logging::LogMessage log_msg(file, line, log_severity );
    log_msg.stream() << log;
}

void init_zephyr_runtime()
{
    mutex_manager::get_instance();
    condition_variable_manager::get_instance();
    semaphore_manager::get_instance();
}

int associate_signal_exist( uint64_t id )
{
    auto signal = associate_signal_manager::get_instance().get_associate_signal( id );
    return signal ? 0 : -1;
}

uint64_t allocate_underlying_associate_signal( const char* a_name )
{
    return associate_signal_manager::get_instance().allocate_new_signal();
}

void trigger_associate_signal( uint64_t id )
{
    associate_signal_manager::get_instance().trigger_signal( id );
}

void reset_associate_signal( uint64_t id )
{
    auto _signal = associate_signal_manager::get_instance().get_associate_signal( id );
    if( _signal )
    {
        _signal->reset();
    }
}

void poll_event_init
    (
    uint64_t a_chain_up_signal_id,
    uint64_t a_chain_down_signal_id,
    uint32_t type,
    int mode
    )
{
    auto down_signal = associate_signal_manager::get_instance().get_associate_signal( a_chain_down_signal_id );
    auto up_signal = associate_signal_manager::get_instance().get_associate_signal( a_chain_up_signal_id );

    if( !down_signal || !up_signal )
    {
        /*since we bind a null up signal, may not created?*/
        down_signal->reset();
        down_signal->trigger();
        return;
    }

    if( down_signal && up_signal )
    {
        down_signal->bind_upstream( up_signal );
    }

    auto user = down_signal->get_bond_user_data();
    if( !user )
    {
        user = std::make_shared<user_data_type>();
    }
    user->type = type;
    down_signal->bind_user_data( user );
}

int poll_event_
    (
    uint64_t a_signal_id,
    uint32_t* a_type
    )
{
    auto first_sig = associate_signal_manager::get_instance().get_associate_signal( a_signal_id );
    first_sig->wait();
    first_sig->get_status();
    first_sig->reset();
    auto user = first_sig->get_bond_user_data();
    if( user )
    {
        *a_type = user->type;
    }
    return 0;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

initializer::initializer()
{
#ifndef HAS_ANDROID_LIKE_LOG
    logging::LoggingSettings log_settings;
    log_settings.delete_old = logging::DELETE_OLD_LOG_FILE;
    log_settings.lock_log = logging::DONT_LOCK_LOG_FILE;
    log_settings.logging_dest = logging::LOG_TO_FILE;

    wchar_t module_name[MAX_PATH];
    GetModuleFileName( nullptr, module_name, MAX_PATH );
    std::wstring log_name = module_name;
    std::wstring::size_type last_backslash = log_name.rfind( '.', log_name.size() );
    if( last_backslash != std::wstring::npos )
        log_name.erase( last_backslash + 1 );
    log_name += L"debug.log";
    log_settings.log_file = log_name.c_str();

    logging::SetLogItems( false, true, true, false );
    logging::BaseInitLoggingImpl( log_settings );
#endif
    initialize_hci();
    initialize_net_pool();
}
