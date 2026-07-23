#include "BluetoothA2DPSinkImplementation.h"

BluetoothA2DPSinkInterface& BluetoothA2DPSinkInterface::GetInterface()
{
    return BluetoothA2DPSinkImplementation::GetInstance();
}

BluetoothA2DPSinkImplementation& BluetoothA2DPSinkImplementation::GetInstance()
{
    static BluetoothA2DPSinkImplementation instance;
    return instance;
}

void BluetoothA2DPSinkImplementation::Init()
{

}

bool BluetoothA2DPSinkImplementation::Connect
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothA2DPSinkImplementation::disconnect
    (
    BluetoothAddress a_address
    )
{
    return false;
}

void BluetoothA2DPSinkImplementation::SetAudioFocus
    (
    bool a_grant
    )
{

}
