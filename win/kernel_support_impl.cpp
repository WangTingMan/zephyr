#include <zephyr/win/kernel_support_impl.h>
#include <base/strings/sys_string_conversions.h>

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
#endif
}

#ifdef __cplusplus
} /* extern "C" */
#endif

initializer::initializer()
{
    initialize_hci();
    initialize_net_pool();
}
