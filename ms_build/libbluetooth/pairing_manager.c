#include <pairing_manager.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/classic/classic.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/hci_types.h>

#include <bluetooth/host/conn_internal.h>

pairing_manager_t s_pairing_manager;
struct bt_conn_auth_cb auth_cb;

void _pairing_confirm_request
    (
    char const* a_name,
    char const* a_addr,
    uint32_t    a_psk,
    uint32_t    a_method
    );

enum bt_security_err _pairing_accept( struct bt_conn* conn,
    const struct bt_conn_pairing_feat* const feat )
{
    enum bt_security_err err;
    err = BT_SECURITY_ERR_SUCCESS;
    return err;
}

void _passkey_display( struct bt_conn* conn, unsigned int passkey )
{
    int x = 10;
    x = 90;
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
    handle_passkey_confirm_request(&s_pairing_manager, conn, passkey);
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

void reply_pairing_passkey_confirm( pairing_manager_t* a_manager, const uint8_t* a_addr, int a_accept )
{
    int err = 0;
    struct bt_conn* conn = NULL;
    err = k_mutex_lock( &a_manager->m_mutex, K_FOREVER );
    conn = a_manager->m_conn;
    k_mutex_unlock( &a_manager->m_mutex );

    if( a_accept == 0 )
    {
        bt_conn_auth_cancel(conn);
    }
    else
    {
        bt_conn_auth_passkey_confirm(conn);
    }
}

void zephyr_pairing_passkey_reply( const uint8_t* a_addr, int a_accept )
{
    reply_pairing_passkey_confirm( &s_pairing_manager, a_addr, a_accept );
}

void handle_connected( pairing_manager_t* a_manager, struct bt_conn* a_conn)
{
    bt_conn_set_security( a_conn, BT_SECURITY_L3 );
}

void handle_passkey_confirm_request( pairing_manager_t* a_manager, struct bt_conn* a_conn, unsigned int a_psk )
{
    int err = 0;
    err = k_mutex_lock( &a_manager->m_mutex, K_FOREVER );
    a_manager->m_conn = a_conn;
    a_manager->m_passkey = a_psk;

    _pairing_confirm_request(NULL, a_conn->br.dst.val, a_psk, 0);
    k_mutex_unlock( &a_manager->m_mutex );
}

void pairing_manager_initialize( pairing_manager_t* a_manager )
{
    int status = 0x00;
    int capability = BT_IO_DISPLAY_YESNO;
    memset( &auth_cb, 0x00, sizeof( auth_cb ) );
    k_mutex_init(&a_manager->m_mutex);
    switch( capability )
    {
    case BT_IO_NO_INPUT_OUTPUT:
        return;
        break;
    case BT_IO_DISPLAY_YESNO:
        auth_cb.passkey_confirm = _passkey_confirm;
        auth_cb.passkey_display = _passkey_display;
        break;
    case BT_IO_KEYBOARD_ONLY:
        auth_cb.passkey_entry = _passkey_entry;
        break;
    case BT_IO_DISPLAY_ONLY:
        auth_cb.passkey_display = _passkey_display;
        break;
    default:
        return;
    }

    auth_cb.app_passkey = _app_passkey;
    auth_cb.cancel = _cancel;
    auth_cb.oob_data_request = _oob_data_request;
    auth_cb.pairing_accept = _pairing_accept;
    auth_cb.pairing_confirm = _pairing_confirm;
    auth_cb.passkey_display_keypress = _passkey_display_keypress;
    auth_cb.pincode_entry = _pincode_entry;
    status = bt_conn_auth_cb_register(&auth_cb);

}
