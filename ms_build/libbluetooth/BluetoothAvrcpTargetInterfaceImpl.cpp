#include "BluetoothAvrcpTargetInterfaceImpl.h"

BluetoothAvrcpTargetInterfaceImpl::BluetoothAvrcpTargetInterfaceImpl()
{

}

BluetoothAvrcpTargetInterfaceImpl& BluetoothAvrcpTargetInterfaceImpl::GetInterface()
{
    static BluetoothAvrcpTargetInterfaceImpl instance;
    return instance;
}

BluetoothAvrcpTargetInterface& BluetoothAvrcpTargetInterface::GetInterface()
{
    return BluetoothAvrcpTargetInterfaceImpl::GetInterface();
}

void BluetoothAvrcpTargetInterfaceImpl::Init()
{

}

bool BluetoothAvrcpTargetInterfaceImpl::Connect
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothAvrcpTargetInterfaceImpl::disconnect
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothAvrcpTargetInterfaceImpl::setActiveDevice
    (
    BluetoothAddress a_address
    )
{
    return false;
}

bool BluetoothAvrcpTargetInterfaceImpl::setSilenceDevice
    (
    BluetoothAddress a_address
    )
{
    return false;
}
