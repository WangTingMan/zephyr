#include "bluetooth_l2cap_coc_implementation.h"

#include "zephyr_bluetooth_base.h"
#include <BT/Adaptor.h>

#include <Zhen/ExecutbleEvent.h>
#include <Zhen/PageManager.h>
#include <Zhen/logging.h>

extern "C"
{
    void _l2cap_coc_data_indication
        (
        uint8_t* p_data,
        uint16_t a_size,
        uint16_t a_psm,
        uint8_t a_device_type,
        uint8_t* a_device
        )
    {
        auto coc = Adaptor::GetInstance().GetLowLevelInterface(bluetooth_interface_type::l2cap_coc);
        auto detail_coc = std::dynamic_pointer_cast< bluetooth_l2cap_coc_implementation >( coc );

        std::vector<uint8_t> data;
        data.insert(data.end(), p_data, p_data + a_size);

        BluetoothAddress addr;
        memcpy(addr.address, a_device, 6);

        auto fun = std::bind( &bluetooth_l2cap_coc_implementation::handle_coc_data_received, detail_coc, std::move( data ),
            a_psm, a_device_type, addr );
        std::shared_ptr< ExecutbleEvent > event = std::make_shared<ExecutbleEvent>( fun );
        PageManager::GetInstance().PostEvent( event );
    }
}

void bluetooth_l2cap_coc_implementation::Init()
{

}

bool bluetooth_l2cap_coc_implementation::Connect
    (
    BluetoothAddress a_address,
    uint16_t a_psm,
    bool a_use_br_edr
    )
{
    return false;
}

bool bluetooth_l2cap_coc_implementation::disconnect
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool bluetooth_l2cap_coc_implementation::listen
    (
    uint16_t a_psm,
    bool a_use_br_edr
    )
{
    return false;
}

void bluetooth_l2cap_coc_implementation::handle_coc_data_received
    (
    std::vector<uint8_t> data,
    uint16_t a_psm,
    uint8_t a_address_type,
    BluetoothAddress a_address
    )
{
    std::string say_hello("hello");
    data.clear();
    data.insert(data.end(), say_hello.begin(), say_hello.end());
    auto fun = std::bind( &bluetooth_l2cap_coc_implementation::send_data_to_remote, this, std::move( data ),
        a_psm, a_address_type, a_address );
    PageManager::GetInstance().connectOneShotTimerTo(fun, 1000, true);
}

void bluetooth_l2cap_coc_implementation::send_data_to_remote
    (
    std::vector<uint8_t> data,
    uint16_t a_psm,
    uint8_t a_address_type,
    BluetoothAddress a_address
    )
{
}
