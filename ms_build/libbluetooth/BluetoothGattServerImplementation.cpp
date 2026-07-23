#include "BluetoothGattServerImplementation.h"

BluetoothGattServerImplementation& BluetoothGattServerImplementation::GetInterface()
{
    static BluetoothGattServerImplementation instance;
    return instance;
}

BluetoothGattServerInterface& BluetoothGattServerInterface::GetInterface()
{
    return BluetoothGattServerImplementation::GetInterface();
}

void BluetoothGattServerImplementation::Init(){}

void BluetoothGattServerImplementation::RegisterGattServer
(
    std::vector<uint8_t> a_uuid,
    bool a_support_eatt
){}

void BluetoothGattServerImplementation::UnregisterGattServer
(
    int a_server_id
){}

void BluetoothGattServerImplementation::StopGattService
(
    int server_if,
    int service_handle
){}

void BluetoothGattServerImplementation::Connect
(
    int server_if,
    const BluetoothAddress& bd_addr,
    bool is_direct,
    int transport
){}

void BluetoothGattServerImplementation::Disconnect
(
    int server_if,
    const BluetoothAddress& bd_addr,
    int conn_id
){}

void BluetoothGattServerImplementation::AddServiceBody
(
    int server_if,
    std::vector<GATT_DB_ELEMENT> service_elements
){}

void BluetoothGattServerImplementation::SendResponse
(
    int conn_id,
    int trans_id,
    int status,
    GATTResponseContent response
){}

void BluetoothGattServerImplementation::SendIndicationOrNotification
(
    int server_if,
    int attribute_handle,
    int conn_id,
    int confirm,
    std::vector<uint8_t> value
){}

void BluetoothGattServerImplementation::TryRequestMaxDataLengthForLE( const BluetoothAddress& bd_addr ){}

boost_ns::signals2::connection BluetoothGattServerImplementation::ConnectToServiceRegistered
(
    std::function<void( int, int, std::vector<uint8_t> )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattServerImplementation::connect_to_service_body_added
(
    std::function<void( int status, int server_if, std::vector<GATT_DB_ELEMENT> )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattServerImplementation::connect_to_client_connection_status_changed
(
    std::function<void( int conn_id, int server_if, ConnectionStatus status, BluetoothAddress addr )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattServerImplementation::connect_to_client_read_desciptor_request
(
    std::function<void(
        int conn_id,
        int trans_id,
        const BluetoothAddress& bda,
        int attr_handle,
        int offset,
        bool is_long )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattServerImplementation::connect_to_client_read_characteristic_request
(
    std::function<void(
        int conn_id,
        int trans_id,
        const BluetoothAddress& bda,
        int attr_handle,
        int offset,
        bool is_long )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattServerImplementation::connect_to_client_write_characteristic_request
(
    std::function<void(
        int conn_id,
        int trans_id,
        const BluetoothAddress& bda,
        int attr_handle,
        int offset,
        bool need_rsp,
        bool is_prep,
        std::vector<uint8_t> value )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattServerImplementation::connect_to_client_request_write_descriptor_request
(
    std::function<void(
        int conn_id,
        int trans_id,
        const BluetoothAddress& bda,
        int attr_handle,
        int offset,
        bool need_rsp,
        bool is_prep,
        std::vector<uint8_t> value )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}

boost_ns::signals2::connection BluetoothGattServerImplementation::ConnectToIndicationSent
(
    std::function<void( int, int )> a_fun
)
{
    boost_ns::signals2::connection con;
    return con;
}