#include "BluetoothHFAgImplementation.h"

BluetoothHFAgImplementation::BluetoothHFAgImplementation()
{

}

BluetoothHFAgImplementation& BluetoothHFAgImplementation::GetInstance()
{
    static BluetoothHFAgImplementation instance;
    return instance;
}

BluetoothHFAGInterface& BluetoothHFAGInterface::GetInterface()
{
    return BluetoothHFAgImplementation::GetInstance();
}

void BluetoothHFAgImplementation::Init()
{

}

bool BluetoothHFAgImplementation::Connect
(
    BluetoothAddress a_address
)
{
    return false;
}

bool BluetoothHFAgImplementation::disconnect
(
    BluetoothAddress a_address
)
{
    return false;
}

bool BluetoothHFAgImplementation::connectAudio( BluetoothAddress a_address )
{
    return false;
}

bool BluetoothHFAgImplementation::DialNewNumber( std::string a_number )
{
    return false;
}

void BluetoothHFAgImplementation::ResponseCind
(
    BluetoothAddress a_address,
    bool a_cell_network_available,
    int a_active_call_count,
    int a_held_call_count,
    CallState a_call_state,
    int a_signal_level,
    bool a_is_roaming,
    int a_bettery_level
)
{

}

void BluetoothHFAgImplementation::ResponseCops
(
    BluetoothAddress a_address,
    std::string a_cops
)
{

}

void BluetoothHFAgImplementation::ResponseCnum
(
    BluetoothAddress a_address,
    std::string a_cnum
)
{

}

void BluetoothHFAgImplementation::ResponseClcc
(
    BluetoothAddress a_address,
    int a_index,
    CallDirection a_direction,
    CallState a_call_state,
    CallMode a_call_mode,
    CallType a_call_type,
    std::string a_number,
    CallAddrType a_call_addr_type
)
{

}

void BluetoothHFAgImplementation::AtResponse
(
    bool a_result,
    int a_code,
    BluetoothAddress a_address
)
{

}

void BluetoothHFAgImplementation::PhoneStateChange
(
    BluetoothAddress address,
    int num_active,
    int num_held,
    CallState call_setup_state,
    std::string number,
    CallAddrType type,
    std::string name
)
{

}