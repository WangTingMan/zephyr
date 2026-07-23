#pragma once

#include <BT/StackLayer/BluetoothA2DPInterface.h>

class BluetoothA2DPImplementation : public BluetoothA2DPInterface
{

public:

    static BluetoothA2DPImplementation& GetInterface();

    void Init() override;

    bool Connect
    (
        BluetoothAddress a_address
    )override;

    bool disconnect
    (
        BluetoothAddress a_address
    )override;

    bool setActiveDevice
    (
        BluetoothAddress a_address
    )override;

    bool setSilenceDevice
    (
        BluetoothAddress a_address
    )override;

private:

};

