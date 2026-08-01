#pragma once
#include <BT/StackLayer/bluetooth_l2cap_coc_interface.h>

#include <vector>

class bluetooth_l2cap_coc_implementation : public bluetooth_l2cap_coc_interface
{

public:

    void Init() override;

    bool Connect
        (
        BluetoothAddress a_address,
        uint16_t a_psm,
        bool a_use_br_edr
        ) override;

    bool disconnect
        (
        BluetoothAddress a_address
        ) override;

    bool listen
        (
        uint16_t a_psm,
        bool a_use_br_edr
        ) override;

public:

    void handle_coc_data_received
        (
        std::vector<uint8_t> data,
        uint16_t a_psm,
        uint8_t a_address_type,
        BluetoothAddress a_address
        );

    void send_data_to_remote
        (
        std::vector<uint8_t> data,
        uint16_t a_psm,
        uint8_t a_address_type,
        BluetoothAddress a_address
        );

private:

};