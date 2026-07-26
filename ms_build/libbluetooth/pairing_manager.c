#include <pairing_manager.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/classic/classic.h>
#include <zephyr/bluetooth/conn.h>

pairing_manager_t s_pairing_manager;
struct bt_conn_auth_cb auth_cb;

enum bt_security_err _pairing_accept( struct bt_conn* conn,
    const struct bt_conn_pairing_feat* const feat )
{
    enum bt_security_err err;
    err = BT_SECURITY_ERR_SUCCESS;
    return err;
}

void _passkey_display( struct bt_conn* conn, unsigned int passkey )
{

}

void _passkey_display_keypress( struct bt_conn* conn,
    enum bt_conn_auth_keypress type )
{

}

void _passkey_entry( struct bt_conn* conn )
{

}

void _passkey_confirm( struct bt_conn* conn, unsigned int passkey )
{

}

void _oob_data_request( struct bt_conn* conn,
    struct bt_conn_oob_info* info )
{

}

void _cancel( struct bt_conn* conn )
{

}

void _pairing_confirm( struct bt_conn* conn )
{

}

void _pincode_entry( struct bt_conn* conn, bool highsec )
{

}

uint32_t _app_passkey( struct bt_conn* conn )
{
    return 0;
}

int zephyr_pair_device( const uint8_t* a_addr )
{
    // 1. checking whether already paired?

    switch( s_pairing_manager.state )
    {
    case link_connecting:
    case io_capability_checking:
    case pairing_confirm:
        return -10;
        break;
    default:
    break;
    }

    // 2. try to connect to remote device
    s_pairing_manager.state = link_connecting;
    bt_addr_t addr;
    struct bt_br_conn_param param;
    param.allow_role_switch = true;
    memcpy( addr.val, a_addr, 6 );
    struct bt_conn* conn = NULL;
    conn = bt_conn_create_br( &addr, &param );
    if( conn )
    {
        bt_conn_unref( conn );
    }
}

void handle_connected( pairing_manager_t* a_manager, struct bt_conn* a_conn)
{
    bt_conn_set_security( a_conn, BT_SECURITY_L3 );
}

void pagemanger_initialize( pairing_manager_t* a_manager )
{
    int status = 0x00;
    memset(&auth_cb, 0x00, sizeof( auth_cb ));
    auth_cb.app_passkey = _app_passkey;
    auth_cb.cancel = _cancel;
    auth_cb.oob_data_request = _oob_data_request;
    auth_cb.pairing_accept = _pairing_accept;
    auth_cb.pairing_confirm = _pairing_confirm;
    auth_cb.passkey_confirm = _passkey_confirm;
    auth_cb.passkey_display = _passkey_display;
    auth_cb.passkey_display_keypress = _passkey_display_keypress;
    auth_cb.passkey_entry = _passkey_entry;
    auth_cb.pincode_entry = _pincode_entry;
    status = bt_conn_auth_cb_register(&auth_cb);

}
