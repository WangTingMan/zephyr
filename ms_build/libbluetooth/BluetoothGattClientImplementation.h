#pragma once
#include <BT/StackLayer/BluetoothGattClientInterface.h>

class BluetoothGattClientImplementation : public BluetoothGattClientInterface
{

public:

    static BluetoothGattClientImplementation& GetInterface();

    virtual ~BluetoothGattClientImplementation() {}

    virtual void Init();

    virtual bool Connect
    (
        BluetoothAddress a_address,
        int a_client_if
    );

    virtual bool disconnect
    (
        BluetoothAddress a_address
    );

    virtual bool IsInitlialized() const;

    virtual void RegisterClientUuid
    (
        std::vector<uint8_t> a_uuid,
        bool a_support_eatt
    );

    virtual void SearchServiceByUuid
    (
        std::vector<uint8_t> a_uuid,
        int a_connection_id
    );

    virtual void RegisterNotification
    (
        int client_if,
        BluetoothAddress a_address,
        int16_t a_handle
    );

    virtual void WriteCharacteristic
    (
        int conn_id, uint16_t handle,
        int write_type, int auth_req,
        const uint8_t* value, size_t length
    );

    virtual void ReadDescriptor
    (
        int conn_id, uint16_t handle, int auth_req
    );

    virtual void GetDbByConnectId( int conn_id );

    virtual void UpdateConnectionParameters
    (
        const BluetoothAddress& bd_addr,
        int min_interval,
        int max_interval,
        int latency,
        int timeout,
        uint16_t min_ce_len,
        uint16_t max_ce_len
    );

    virtual void ConfigureMtuSize
    (
        int connd_id,
        int mtu
    );

public:

    virtual boost_ns::signals2::connection ConnectToSignalGattClientInited
    (
        std::function<void()> a_fun
    );

    virtual boost_ns::signals2::connection ConnectToSignalGattClientMessageEmiited
    (
        std::function<void( std::shared_ptr< GattClientBaseMessage> ) > a_fun
    );

    virtual boost_ns::signals2::connection ConnectToClientRegistered
    (
        std::function<void( int, int, std::vector<uint8_t> )> a_fun
    );

    virtual boost_ns::signals2::connection ConnectToConnectionStatusChanged
    (
        std::function<void( int, ConnectionStatus/*a_status*/, int/*client_if*/, BluetoothAddress )> a_fun
    );

    virtual boost_ns::signals2::connection ConnectToServiceSearchCompleted
    (
        std::function<void( std::vector<GATT_DB_ELEMENT>, int )> a_fun
    );

    virtual boost_ns::signals2::connection ConnectToDescriptorReadCompleted
    (
        std::function<void( int, int, btgatt_read_params_type )> a_fun
    );

    virtual boost_ns::signals2::connection ConnectToCharacteristicWriteCompleted
    (
        std::function<void( int conn_id, int status, uint16_t handle, uint16_t len, std::vector<uint8_t> value )>
    );
};
