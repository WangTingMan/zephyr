#include "zephyr_bluetooth_base.h"
#include "pairing_manager.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/classic/classic.h>
#include <zephyr/bluetooth/conn.h>

struct bt_br_discovery_cb discovery_cb;
struct bt_br_discovery_result scan_result[20];
struct bt_conn_cb connection_cb;
extern pairing_manager_t s_pairing_manager;

void _new_device_found
    (
    char const* a_name,
    char const* a_addr
    );

void _local_settings_changed( int disconverable, int connectable );

void _local_pairable_changed( int a_pairable );

void _inquiry_state_changed( int a_started );

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
    _inquiry_state_changed( false );
}

void bt_ready_cb_t_cb( int err )
{
    _adapter_state_ready_callback();

    const char* local_name = bt_get_name();

    _local_name_changed(local_name);
}

void _connected( struct bt_conn* conn, uint8_t err )
{
    handle_connected( &s_pairing_manager, conn );
}

void _disconnected( struct bt_conn* conn, uint8_t reason )
{

}

void _recycled( void )
{

}

bool _le_param_req( struct bt_conn* conn,
    struct bt_le_conn_param* param )
{
    return false;
}

void _le_param_updated( struct bt_conn* conn, uint16_t interval,
    uint16_t latency, uint16_t timeout )
{

}

void _le_param_update_rejected( struct bt_conn* conn, uint8_t hci_err )
{

}

void _identity_resolved( struct bt_conn* conn,
    const bt_addr_le_t* rpa,
    const bt_addr_le_t* identity )
{

}

void _security_changed( struct bt_conn* conn, bt_security_t level,
    enum bt_security_err err )
{

}

void _remote_info_available( struct bt_conn* conn,
    struct bt_conn_remote_info* remote_info )
{

}

void _tx_power_report( struct bt_conn* conn,
    const struct bt_conn_le_tx_power_report* report )
{

}

enum bt_br_conn_req_rsp bt_br_conn_req_func_impl( const bt_addr_t* addr, uint32_t cod )
{
    enum bt_br_conn_req_rsp rsp;
    rsp = BT_BR_CONN_REQ_ACCEPT_PERIPHERAL;

    return rsp;
}

struct bt_l2cap_br_fixed_chan {
    uint16_t		cid;
    int ( *accept )( struct bt_conn* conn, struct bt_l2cap_chan** chan );
};

void zephyr_init_()
{
    int err = 0;
    k_kernel_init();

    memset( &scan_result, 0x00, sizeof( scan_result ) );

    memset(&discovery_cb, 0x00, sizeof( struct bt_br_discovery_cb ));
    discovery_cb.recv = &recv_result;
    discovery_cb.timeout = &discovery_timeout;

    bt_br_discovery_cb_register( &discovery_cb );

    memset( &connection_cb, 0x00, sizeof( connection_cb ) );
    connection_cb.connected = &_connected;
    connection_cb.disconnected = &_disconnected;
    connection_cb.identity_resolved = &_identity_resolved;
    connection_cb.le_param_req = &_le_param_req;
    connection_cb.le_param_update_rejected = &_le_param_update_rejected;
    connection_cb.le_param_updated = &_le_param_updated;
    connection_cb.recycled = _recycled;
    connection_cb.remote_info_available = &_remote_info_available;
    connection_cb.security_changed = &_security_changed;
    connection_cb.tx_power_report = &_tx_power_report;
    bt_conn_cb_register( &connection_cb );

    pairing_manager_initialize(&s_pairing_manager);
    err = bt_enable( bt_ready_cb_t_cb );

}

void start_discovery()
{
    int status = 0;
    LOG_INF( "start inquiry neady classic devices..." );
    struct bt_br_discovery_param br_discover;
    br_discover.length = 10;
    br_discover.limited = false;

    status = bt_br_discovery_start(&br_discover, scan_result, 20);
    if( status == 0 )
    {
        _inquiry_state_changed( true );
    }
}

void zephyr_bt_manager_br_set_visual( int a_pairable, int discoverable, int connectable )
{
    int status = 0;
    bool current_connectable = false;
    bool current_discoverable = false;
    status = bt_br_set_connectable( connectable == 0 ? false : true , &bt_br_conn_req_func_impl );
    if( status == -EALREADY || status == 0 )
    {
        current_connectable = connectable;
    }
    status = bt_br_set_discoverable( discoverable == 0 ? false : true, false );
    if( status == -EALREADY || status == 0 )
    {
        current_discoverable = discoverable;
    }

    _local_settings_changed(current_discoverable, current_connectable);

    bt_set_bondable(a_pairable == 0 ? false : true);
    _local_pairable_changed(a_pairable);
}

void set_local_name( const char* a_name )
{
    int status = 0;
    status = bt_set_name(a_name);
    if( status == 0 )
    {
        _local_name_changed(a_name);
    }
}


