#include "BluetoothPANImplementation.h"

BluetoothPANImplementation& BluetoothPANImplementation::GetInstance()
{
    static BluetoothPANImplementation instance;
    return instance;
}

BluetoothPANInterface& BluetoothPANInterface::GetInterface()
{
    return BluetoothPANImplementation::GetInstance();
}

void BluetoothPANImplementation::Init()
{

}

bool BluetoothPANImplementation::Connect( BluetoothAddress a_address )
{
    return false;
}

bool BluetoothPANImplementation::disconnect( BluetoothAddress a_address )
{
    return false;
}
