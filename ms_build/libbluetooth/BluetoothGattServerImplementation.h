#pragma once
#include <BT/StackLayer/BluetoothGattServerInterface.h>

class BluetoothGattServerImplementation : public BluetoothGattServerInterface
{

public:

    static BluetoothGattServerImplementation& GetInterface();

    virtual void Init();

    virtual void RegisterGattServer
    (
        std::vector<uint8_t> a_uuid,
        bool a_support_eatt
    );

    virtual void UnregisterGattServer
    (
        int a_server_id
    );

    virtual void StopGattService
    (
        int server_if,
        int service_handle
    );

    virtual void Connect
    (
        int server_if,
        const BluetoothAddress& bd_addr,
        bool is_direct,
        int transport
    );

    virtual void Disconnect
    (
        int server_if,
        const BluetoothAddress& bd_addr,
        int conn_id
    );

    virtual void AddServiceBody
    (
        int server_if,
        std::vector<GATT_DB_ELEMENT> service_elements
    );

    virtual void SendResponse
    (
        int conn_id,
        int trans_id,
        int status,
        GATTResponseContent response
    );

    virtual void SendIndicationOrNotification
    (
        int server_if,
        int attribute_handle,
        int conn_id,
        int confirm,
        std::vector<uint8_t> value
    );

    virtual void TryRequestMaxDataLengthForLE( const BluetoothAddress& bd_addr );

public:

    virtual boost_ns::signals2::connection ConnectToServiceRegistered
    (
        std::function<void( int, int, std::vector<uint8_t> )> a_fun
    );

    virtual boost_ns::signals2::connection connect_to_service_body_added
    (
        std::function<void( int status, int server_if, std::vector<GATT_DB_ELEMENT> )> a_fun
    );

    virtual boost_ns::signals2::connection connect_to_client_connection_status_changed
    (
        std::function<void( int conn_id, int server_if, ConnectionStatus status, BluetoothAddress addr )> a_fun
    );

    virtual boost_ns::signals2::connection connect_to_client_read_desciptor_request
    (
        std::function<void(
            int conn_id,
            int trans_id,
            const BluetoothAddress& bda,
            int attr_handle,
            int offset,
            bool is_long )> a_fun
    );

    virtual boost_ns::signals2::connection connect_to_client_read_characteristic_request
    (
        std::function<void(
            int conn_id,
            int trans_id,
            const BluetoothAddress& bda,
            int attr_handle,
            int offset,
            bool is_long )> a_fun
    );

    virtual boost_ns::signals2::connection connect_to_client_write_characteristic_request
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
    );

    virtual boost_ns::signals2::connection connect_to_client_request_write_descriptor_request
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
    );

    virtual boost_ns::signals2::connection ConnectToIndicationSent
    (
        std::function<void( int, int )> a_fun
    );

};