#include <stdio.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/logging/log.h>

#if __has_include(<log/log.h>)
#include <log/log.h>
#define HAS_ANDROID_LIKE_LOG
#endif

#ifdef _MSC_VER
#pragma comment(lib, "bluetooth.lib")
#pragma comment(lib, "zephyr_runtime.lib")
#pragma comment(lib, "utils.lib")
#pragma comment(lib, "z_crc.lib")
#pragma comment(lib, "z_settings.lib")
#pragma comment(lib, "mbedtls.lib")
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "libChromeBase.lib")
#pragma comment(lib, "ws2_32.lib")

#ifdef HAS_ANDROID_LIKE_LOG
#pragma comment(lib, "liblog.lib")
#pragma comment(lib, "libcutils.lib")
#endif
#endif

void bt_ready_cb_t_cb( int err )
{
    printf("bluetooth enabled!\n");
}

void _work_handler_a( struct k_work* work )
{
    LOG_INF( "execution the background task!" );
}

void delay_work_handler_a( struct k_work* work )
{
    LOG_INF( "execution the delayed background task!" );
}

int main()
{
#ifdef _MSC_VER
    k_kernel_init();
#endif
    struct k_work work;
    work.handler = &_work_handler_a;
    int err;

    LOG_INF("submit a background task!");
    k_work_submit(&work);

    struct k_work_delayable work_delay;
    k_timeout_t timeout;
    timeout.ticks = 2000;
    k_work_init_delayable( &work_delay, &delay_work_handler_a );
    LOG_INF( "submit a delayed background task!" );
    k_work_schedule(&work_delay, timeout );

    err = bt_enable( bt_ready_cb_t_cb );

    k_msleep( 100000000 );
    return 0;
}

