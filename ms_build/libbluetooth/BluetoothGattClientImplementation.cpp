#include "BluetoothGattClientImplementation.h"

BluetoothGattClientImplementation& BluetoothGattClientImplementation::GetInterface()
{
    static BluetoothGattClientImplementation instance;
    return instance;
}

BluetoothGattClientInterface& BluetoothGattClientInterface::GetInterface()
{
    return BluetoothGattClientImplementation::GetInterface();
}

void BluetoothGattClientImplementation::Init()
{

}

bool BluetoothGattClientImplementation::Connect
(
    BluetoothAddress a_address,
    int a_client_if
)
{
    return false;
}

bool BluetoothGattClientImplementation::disconnect
(
    BluetoothAddress a_address
)
{
    return false;
}

bool BluetoothGattClientImplementation::IsInitlialized() const
{
    return false;
}

void BluetoothGattClientImplementation::RegisterClientUuid
(
    std::vector<uint8_t> a_uuid,
    bool a_support_eatt
)
{

}

void BluetoothGattClientImplementation::SearchServiceByUuid
(
    std::vector<uint8_t> a_uuid,
    int a_connection_id
)
{

}

void BluetoothGattClientImplementation::RegisterNotification
(
    int client_if,
    BluetoothAddress a_address,
    int16_t a_handle
)
{

}

void BluetoothGattClientImplementation::WriteCharacteristic
(
    int conn_id, uint16_t handle,
    int write_type, int auth_req,
    const uint8_t* value, size_t length
)
{

}

void BluetoothGattClientImplementation::ReadDescriptor
(
    int conn_id, uint16_t handle, int auth_req
)
{

}

void BluetoothGattClientImplementation::GetDbByConnectId( int conn_id )
{

}

void BluetoothGattClientImplementation::UpdateConnectionParameters
(
    const BluetoothAddress& bd_addr,
    int min_interval,
    int max_interval,
    int latency,
    int timeout,
    uint16_t min_ce_len,
    uint16_t max_ce_len
)
{

}

void BluetoothGattClientImplementation::ConfigureMtuSize
(
    int connd_id,
    int mtu
)
{

}

boost_ns::signals2::connection BluetoothGattClientImplementation::ConnectToSignalGattClientInited
(
    std::function<void()> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattClientImplementation::ConnectToSignalGattClientMessageEmiited
(
    std::function<void( std::shared_ptr< GattClientBaseMessage> ) > a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattClientImplementation::ConnectToClientRegistered
(
    std::function<void( int, int, std::vector<uint8_t> )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattClientImplementation::ConnectToConnectionStatusChanged
(
    std::function<void( int, ConnectionStatus/*a_status*/, int/*client_if*/, BluetoothAddress )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattClientImplementation::ConnectToServiceSearchCompleted
(
    std::function<void( std::vector<GATT_DB_ELEMENT>, int )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattClientImplementation::ConnectToDescriptorReadCompleted
(
    std::function<void( int, int, btgatt_read_params_type )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattClientImplementation::ConnectToCharacteristicWriteCompleted
(
    std::function<void( int conn_id, int status, uint16_t handle, uint16_t len, std::vector<uint8_t> value )>
)
{
    boost_ns::signals2::connection con;
    return con;
}