#pragma once

#include <cstdint>
#include <memory>

#include "BT/StackLayer/BluetoothAvrcpTargetInterface.h"

class BluetoothAvrcpTargetInterfaceImpl : public BluetoothAvrcpTargetInterface
{

public:

    BluetoothAvrcpTargetInterfaceImpl();

    static BluetoothAvrcpTargetInterfaceImpl& GetInterface();

    void Init() override;

    bool Connect
        (
        BluetoothAddress a_address
        ) override;

    bool disconnect
        (
        BluetoothAddress a_address
        ) override;

    bool setActiveDevice
        (
        BluetoothAddress a_address
        ) override;

    bool setSilenceDevice
        (
        BluetoothAddress a_address
        ) override;

private:

};

