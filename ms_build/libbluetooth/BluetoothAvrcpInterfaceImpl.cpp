#include "BluetoothAvrcpInterfaceImpl.h"

std::shared_ptr<Bluetooth::BluetoothAvrcpInterface> Bluetooth::BluetoothAvrcpInterface::CreateAvrcpInterface()
{
    return std::make_shared<BluetoothAvrcpInterfaceImpl>();
}

BluetoothAvrcpInterfaceImpl::BluetoothAvrcpInterfaceImpl()
{

}

bool BluetoothAvrcpInterfaceImpl::SendGetPlayBackStateCmd( BluetoothAddress const& )
{
    return false;
}

bool BluetoothAvrcpInterfaceImpl::SendPassThroughCmd
    (
    BluetoothAddress const& a_address,
    AvrcpPassThroughKeyState a_state,
    AvrcpPassThroughKeyCode a_key
    )
{
    return false;
}

bool BluetoothAvrcpInterfaceImpl::init()
{
    return false;
}

