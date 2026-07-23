#include "BluetoothA2DPImplementation.h"

BluetoothA2DPInterface& BluetoothA2DPInterface::GetInterface()
{
    return BluetoothA2DPImplementation::GetInterface();
}

BluetoothA2DPImplementation& BluetoothA2DPImplementation::GetInterface()
{
    static BluetoothA2DPImplementation instance;
    return instance;
}

void BluetoothA2DPImplementation::Init()
{

}

bool BluetoothA2DPImplementation::Connect
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothA2DPImplementation::disconnect
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothA2DPImplementation::setActiveDevice
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothA2DPImplementation::setSilenceDevice
    (
    BluetoothAddress a_address
    )
{
    return false;
}
