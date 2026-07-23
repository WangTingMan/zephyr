#pragma once

#include "BT/StackLayer/BluetoothA2DPSinkInterface.h"

class BluetoothA2DPSinkImplementation : public BluetoothA2DPSinkInterface
{

public:

    static BluetoothA2DPSinkImplementation& GetInstance();

    void Init() override;

    bool Connect
        (
        BluetoothAddress a_address
        )override;

    bool disconnect
        (
        BluetoothAddress a_address
        )override;

    void SetAudioFocus
        (
        bool a_grant
        )override;

private:

};

