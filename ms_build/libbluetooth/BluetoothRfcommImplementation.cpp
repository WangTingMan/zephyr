#include "BluetoothRfcommImplementation.h"

BluetoothRfcommImplementation& BluetoothRfcommImplementation::GetInterface()
{
    static BluetoothRfcommImplementation instance;
    return instance;
}

BluetoothRfcommInterface& BluetoothRfcommInterface::GetInterface()
{
    return BluetoothRfcommImplementation::GetInterface();
}

void BluetoothRfcommImplementation::Init()
{

}

bool BluetoothRfcommImplementation::Connect
(
    BluetoothAddress a_address,
    std::string a_service_name,
    int a_channel
)
{
    return false;
}

bool BluetoothRfcommImplementation::disconnect
(
    BluetoothAddress a_address
)
{
    return false;
}

/**
 * Try to listen a rfcomm socket as a server.
 * a_service_name the rfcomm service's name
 * a_service_uuid the rfcomm service's UUID
 * a_channel the rfcomm service's port.
 * Warning: If the port specified by a_channel already in use,
 * then will return false.
 */
bool BluetoothRfcommImplementation::listen
(
    std::string const& a_service_name,
    std::vector<uint8_t> a_service_uuid,
    int a_id,
    int a_channel
)
{
    return false;
}

void BluetoothRfcommImplementation::custom_test()
{
}
