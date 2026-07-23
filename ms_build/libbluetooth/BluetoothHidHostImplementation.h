#pragma once
#include <BT/StackLayer/BluetoothHidHostInterface.h>

class BluetoothHidHostImplementation : public BluetoothHidHostInterface
{

public:

    static BluetoothHidHostImplementation& GetInstance();

    void Init() override;

    bool Connect
    (
        BluetoothAddress a_address
    ) override;

    bool disconnect
    (
        BluetoothAddress a_address
    ) override;

    void GetReport
    (
        BluetoothAddress a_address
    ) override;

private:

};
