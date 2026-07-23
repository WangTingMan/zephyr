#include <stdlib.h>

#ifdef _MSC_VER
/*these not used in fact*/
struct bt_sco_hci_cb* _bt_sco_hci_cb_list_start = NULL;
struct bt_sco_hci_cb* _bt_sco_hci_cb_list_end = NULL;

struct bt_sco_conn_cb* _bt_sco_conn_cb_list_start = NULL;
struct bt_sco_conn_cb* _bt_sco_conn_cb_list_end = NULL;

struct bt_conn_cb* _bt_conn_cb_list_start = NULL;
struct bt_conn_cb* _bt_conn_cb_list_end = NULL;

struct bt_gatt_service_static* _bt_gatt_service_static_list_start = NULL;
struct bt_gatt_service_static* _bt_gatt_service_static_list_end = NULL;

struct bt_l2cap_br_fixed_chan* _bt_l2cap_br_fixed_chan_list_start = NULL;
struct bt_l2cap_br_fixed_chan* _bt_l2cap_br_fixed_chan_list_end = NULL;

struct bt_l2cap_fixed_chan* _bt_l2cap_fixed_chan_list_start = NULL;
struct bt_l2cap_fixed_chan* _bt_l2cap_fixed_chan_list_end = NULL;

#endif

void bt_testing_trace_ext_adv_reassembly_timeout( void )
{

}

void bt_testing_trace_ext_adv_reassembly_complete( void )
{

}

