#include "BluetoothHidHostImplementation.h"

BluetoothHidHostImplementation& BluetoothHidHostImplementation::GetInstance()
{
    static BluetoothHidHostImplementation instance;
    return instance;
}

BluetoothHidHostInterface& BluetoothHidHostInterface::GetInterface()
{
    return BluetoothHidHostImplementation::GetInstance();
}

void BluetoothHidHostImplementation::Init()
{

}

bool BluetoothHidHostImplementation::Connect
(
    BluetoothAddress a_address
)
{
    return false;
}

bool BluetoothHidHostImplementation::disconnect
(
    BluetoothAddress a_address
)
{
    return false;
}

void BluetoothHidHostImplementation::GetReport
(
    BluetoothAddress a_address
)
{

}