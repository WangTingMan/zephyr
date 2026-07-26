#include <stdlib.h>
#include <stdint.h>

#ifdef _MSC_VER
/*these not used in fact*/
struct bt_sco_hci_cb* _bt_sco_hci_cb_list_start = NULL;
struct bt_sco_hci_cb* _bt_sco_hci_cb_list_end = NULL;

struct bt_sco_conn_cb* _bt_sco_conn_cb_list_start = NULL;
struct bt_sco_conn_cb* _bt_sco_conn_cb_list_end = NULL;

struct bt_l2cap_fixed_chan* _bt_l2cap_fixed_chan_list_start = NULL;
struct bt_l2cap_fixed_chan* _bt_l2cap_fixed_chan_list_end = NULL;

#endif

#define REGISTERED_ARRAY_SIZE 10
struct bt_l2cap_br_fixed_chan* _bt_l2cap_br_fixed_chan[REGISTERED_ARRAY_SIZE] = { 0x00 };
uint32_t s_bt_l2cap_br_next_id = 0;

struct bt_conn_cb* _bt_conn_cb_chan[REGISTERED_ARRAY_SIZE] = { 0x00 };
uint32_t s_bt_conn_cb_next_id = 0;

struct bt_gatt_service_static* _bt_gatt_service_[REGISTERED_ARRAY_SIZE] = { 0x00 };
uint32_t s_bt_gatt_service__next_id = 0;

void bt_testing_trace_ext_adv_reassembly_timeout( void )
{

}

void bt_testing_trace_ext_adv_reassembly_complete( void )
{

}

void register_bt_l2cap_br_fix_chan_instance( struct bt_l2cap_br_fixed_chan* a_l2cap_br_chan )
{
    uint32_t id = s_bt_l2cap_br_next_id;
    s_bt_l2cap_br_next_id++;
    _bt_l2cap_br_fixed_chan[id] = a_l2cap_br_chan;
}

struct bt_l2cap_br_fixed_chan** _bt_l2cap_br_fixed_chan_list_start()
{
    return _bt_l2cap_br_fixed_chan + 0;
}

struct bt_l2cap_br_fixed_chan** _bt_l2cap_br_fixed_chan_list_end()
{
    return _bt_l2cap_br_fixed_chan + s_bt_l2cap_br_next_id;
}

void register_bt_conn_cb_instance( struct bt_conn_cb* a_con_cb )
{
    uint32_t id = s_bt_conn_cb_next_id;
    s_bt_conn_cb_next_id++;
    _bt_conn_cb_chan[id] = a_con_cb;
}

struct bt_conn_cb** _bt_conn_cb_list_start()
{
    return _bt_conn_cb_chan + 0;
}

struct bt_conn_cb** _bt_conn_cb_list_end()
{
    return _bt_conn_cb_chan + s_bt_conn_cb_next_id;
}

void register_bt_gatt_service_static_instance( struct bt_gatt_service_static* a_con_cb )
{
    uint32_t id = s_bt_gatt_service__next_id;
    s_bt_gatt_service__next_id++;
    _bt_gatt_service_[id] = a_con_cb;
}

struct bt_gatt_service_static** _bt_gatt_service_static_list_start()
{
    return _bt_gatt_service_ + 0;
}

struct bt_gatt_service_static** _bt_gatt_service_static_list_end()
{
    return _bt_gatt_service_ + s_bt_gatt_service__next_id;
}
