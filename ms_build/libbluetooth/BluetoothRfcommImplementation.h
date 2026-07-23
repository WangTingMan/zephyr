#pragma once
#include <BT/StackLayer/BluetoothRfcommInterface.h>

class BluetoothRfcommImplementation: public BluetoothRfcommInterface
{

public:

    static BluetoothRfcommImplementation& GetInterface();

    virtual ~BluetoothRfcommImplementation() {}

    virtual void Init();

    virtual bool Connect
    (
        BluetoothAddress a_address,
        std::string a_service_name,
        int a_channel
    );

    virtual bool disconnect
    (
        BluetoothAddress a_address
    );

    /**
     * Try to listen a rfcomm socket as a server.
     * a_service_name the rfcomm service's name
     * a_service_uuid the rfcomm service's UUID
     * a_channel the rfcomm service's port.
     * Warning: If the port specified by a_channel already in use,
     * then will return false.
     */
    virtual bool listen
    (
        std::string const& a_service_name,
        std::vector<uint8_t> a_service_uuid,
        int a_id,
        int a_channel
    );

    virtual void custom_test();
};