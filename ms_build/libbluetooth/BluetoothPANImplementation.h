#pragma once
#include "BT/StackLayer/BluetoothPANInterface.h"

class BluetoothPANImplementation : public BluetoothPANInterface
{

public:

    static BluetoothPANImplementation& GetInstance();

    void Init() override;

    virtual bool Connect( BluetoothAddress a_address ) override;

    virtual bool disconnect( BluetoothAddress a_address ) override;

private:

};

