#include <zephyr/devicetree.h>
#include <zephyr/device.h>

#include <zephyr/drivers/bluetooth.h>

#define BT_HCI_NODE_NAME   DT_CHOSEN(zephyr_bt_hci)
#define BT_HCI_DEV_NAME    DEVICE_DT_NAME_GET(BT_HCI_NODE_NAME)

struct device BT_HCI_DEV_NAME;
struct bt_hci_driver_data hci_driver_data;
struct bt_hci_driver_api hci_apis;

extern void hci_initialize();
extern int btdrv_send( uint8_t type, uint8_t* p_data, uint16_t len );
bt_hci_recv_t s_received_handler = NULL;

void hci_data_receive_zephyr_original( uint8_t a_type, uint8_t* a_data, uint16_t a_size );

int bt_hci_api_open_zephyr( const struct device* dev )
{
    dev->data;
    struct bt_hci_driver_data* detail_data = ( struct bt_hci_driver_data* )(dev->data);
    s_received_handler = detail_data->recv;
    hci_initialize();
    return 0;
}

int bt_hci_api_close_zephyr( const struct device* dev )
{
    return 0;
}

int bt_hci_api_send_zephyr( const struct device* dev, struct net_buf* buf )
{
    buf->data;
    buf->len;
    if( buf->len < 2 )
    {
        return -90;
    }

    uint8_t type = buf->data[0];
    uint8_t* buffer = buf->data + 1;
    uint16_t size = buf->len - 1;
    btdrv_send(type, buffer, size );

    net_buf_unref( buf );
    return 0;
}

void initialize_hci()
{
    hci_driver_data.recv = NULL;
    hci_apis.open = bt_hci_api_open_zephyr;
    hci_apis.close = bt_hci_api_close_zephyr;
    hci_apis.send = bt_hci_api_send_zephyr;
    struct device* hci = &BT_HCI_DEV_NAME;
    hci->data = &hci_driver_data;
    hci->api = &hci_apis;
}

struct net_buf* hci_get_buf_to_copy( uint8_t type, uint8_t evt, uint16_t exp_len )
{
    struct net_buf* buf = NULL;

    switch( type )
    {
    case 0x04:/*event*/
        buf = bt_buf_get_evt( evt, 0, K_FOREVER );
    break;
    case 0x02: /*ACL data*/
        buf = bt_buf_get_rx( BT_BUF_ACL_IN, K_FOREVER );
    break;
    case 0x03: /*SCO data*/
        /*zephyr do not support SCO via HCI?*/
        /*buf = bt_buf_get_rx( BT_BUF_SCO_IN, K_FOREVER );*/
    break;
    case 0x05: /*ISO data*/
        buf = bt_buf_get_rx( BT_BUF_ISO_IN, K_FOREVER );
    break;
    default:
        buf = NULL;
    break;
    }

    return buf;
}

void hci_data_receive_zephyr_original( uint8_t a_type, uint8_t* a_data, uint16_t a_size )
{
    struct net_buf* buf = NULL;
    uint8_t event_code = 0x00;
    uint8_t* buffer = NULL;
    if( a_type == 0x04 )
    {
        event_code = a_data[0];
    }
    if( s_received_handler )
    {
        buf = hci_get_buf_to_copy( a_type, event_code, a_size );
        buffer = buf->data + 1;
        if( buf )
        {
            buf->len = a_size + 1;
            memcpy( buffer, a_data, a_size );
            s_received_handler( NULL, buf );
        }
    }
}
