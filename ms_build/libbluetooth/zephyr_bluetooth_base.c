#include "zephyr_bluetooth_base.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/classic/classic.h>

struct bt_br_discovery_cb discovery_cb;
struct bt_br_discovery_result scan_result[20];

void _new_device_found
    (
    char const* a_name,
    char const* a_addr
    );

void recv_result( const struct bt_br_discovery_result* result )
{
    uint16_t ltv_size = 0;
    struct bt_data out[20] = {0};
    char name[256] = {0};
    ltv_size = ltv_parser(result->eir, BT_BR_EIR_SIZE_MAX, out, 20);
    for( int i = 0; i < ltv_size; ++i )
    {
        switch( out[i].type )
        {
        case BT_DATA_NAME_COMPLETE:
            memcpy(name,out[i].data,out[i].data_len);
            break;
        case BT_DATA_NAME_SHORTENED:
            memcpy( name, out[i].data, out[i].data_len );
            break;
        default:
            break;
        }
    }

    _new_device_found(name, result->addr.val);
}

void discovery_timeout( const struct bt_br_discovery_result* results,
    size_t count )
{

}

void bt_ready_cb_t_cb( int err )
{
    _adapter_state_ready_callback();
}

void zephyr_init_()
{
    int err = 0;
    k_kernel_init();

    memset( &scan_result, 0x00, sizeof( scan_result ) );

    memset(&discovery_cb, 0x00, sizeof( struct bt_br_discovery_cb ));
    discovery_cb.recv = &recv_result;
    discovery_cb.timeout = &discovery_timeout;

    err = bt_enable( bt_ready_cb_t_cb );

    bt_br_discovery_cb_register(&discovery_cb);
}

void start_discovery()
{
    LOG_INF( "start inquiry neady classic devices..." );
    struct bt_br_discovery_param br_discover;
    br_discover.length = 10;
    br_discover.limited = false;

    bt_br_discovery_start(&br_discover, scan_result, 20);
}

void zephyr_bt_manager_br_set_visual( int enable, int discoverable, int connectable )
{

}
