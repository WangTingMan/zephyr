#pragma once

#include <BT/StackLayer/BluetoothHFInterface.h>
#include <BT/StackLayer/BluetoothHFAGInterface.h>

#include <memory>

class BluetoothHFAgImplementation : public BluetoothHFAGInterface
{

public:

    BluetoothHFAgImplementation();

    static BluetoothHFAgImplementation& GetInstance();

    void Init() override;

    bool Connect
    (
        BluetoothAddress a_address
    ) override;

    bool disconnect
    (
        BluetoothAddress a_address
    ) override;

    bool connectAudio( BluetoothAddress a_address ) override;

    bool DialNewNumber( std::string a_number ) override;

    void ResponseCind
    (
        BluetoothAddress a_address,
        bool a_cell_network_available,
        int a_active_call_count,
        int a_held_call_count,
        CallState a_call_state,
        int a_signal_level,
        bool a_is_roaming,
        int a_bettery_level
    ) override;

    void ResponseCops
    (
        BluetoothAddress a_address,
        std::string a_cops
    )override;

    void ResponseCnum
    (
        BluetoothAddress a_address,
        std::string a_cnum
    )override;

    void ResponseClcc
    (
        BluetoothAddress a_address,
        int a_index,
        CallDirection a_direction,
        CallState a_call_state,
        CallMode a_call_mode,
        CallType a_call_type,
        std::string a_number,
        CallAddrType a_call_addr_type
    )override;

    void AtResponse
    (
        bool a_result,
        int a_code,
        BluetoothAddress a_address
    )override;

    void PhoneStateChange
    (
        BluetoothAddress address,
        int num_active,
        int num_held,
        CallState call_setup_state,
        std::string number,
        CallAddrType type,
        std::string name
    );

private:

};

