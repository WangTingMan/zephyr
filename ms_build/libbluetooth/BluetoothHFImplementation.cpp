#include "BluetoothHFImplementation.h"

BluetoothHFImplementation& BluetoothHFImplementation::GetInstance()
{
    static BluetoothHFImplementation instance;
    return instance;
}

BluetoothHFInterface& BluetoothHFInterface::GetInterface()
{
    return BluetoothHFImplementation::GetInstance();
}

BluetoothHFImplementation::BluetoothHFImplementation()
{

}

void BluetoothHFImplementation::Init()
{

}

bool BluetoothHFImplementation::Connect
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothHFImplementation::disconnect
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothHFImplementation::setActiveDevice
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothHFImplementation::setSilenceDevice
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothHFImplementation::dialNumber
    (
    BluetoothAddress a_address,
    std::string a_number
    )
{
    return false;
}

void BluetoothHFImplementation::answerIncomingCall
    (
    BluetoothAddress& a_address,
    bool a_answered
    )
{

}

// Send AT+CHLD=2 to phone.
void BluetoothHFImplementation::swapCalls
    (
    BluetoothAddress a_address
    )
{

}

void BluetoothHFImplementation::joinCalls
    (
    BluetoothAddress a_address
    )
{

}

void BluetoothHFImplementation::splitCall
    (
    BluetoothAddress a_address,
    int a_index
    )
{

}

bool BluetoothHFImplementation::QueryCallList
    (
    BluetoothAddress a_address
    )
{
    return false;
}

