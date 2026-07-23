#pragma once

#include "BT/avrcp/BluetoothAvrcpInterface.h"
#include <cstdint>

class BluetoothAvrcpInterfaceImpl : public Bluetooth::BluetoothAvrcpInterface
{

public:

    BluetoothAvrcpInterfaceImpl();

    bool SendGetPlayBackStateCmd( BluetoothAddress const& ) override;

    virtual bool SendPassThroughCmd
        (
        BluetoothAddress const& a_address,
        AvrcpPassThroughKeyState a_state,
        AvrcpPassThroughKeyCode a_key
        )override;

    bool init() override;

private:

};


