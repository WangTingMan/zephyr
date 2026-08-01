#include "BluetoothBaseImplementation.h"
#include "BluetoothAvrcpTargetInterfaceImpl.h"
#include "bluetooth_l2cap_coc_implementation.h"

#include "zephyr_bluetooth_base.h"
#include <base/logging.h>

#include <Zhen/ExecutbleEvent.h>
#include <Zhen/PageManager.h>
#include <Zhen/logging.h>

#include <BT/Adaptor.h>
#include <Utils.h>

extern "C"
{

int bt_br_discovery_stop( void );

void _adapter_state_ready_callback()
{
    std::shared_ptr<ExecutbleEvent> event = std::make_shared<ExecutbleEvent>();
    event->SetExecutableFunction( std::bind( &Adaptor::AdapterStateChanged, std::ref( Adaptor::GetInstance() ), true ) );
    PageManager::GetInstance().PostEvent( event );
}

void _new_device_found
    (
    char const* a_name,
    char const* a_addr
    )
{
    BluetoothAddress address;
    memcpy(address.address, a_addr, 6);
    RemoteDevice remote_dev;
    remote_dev.address = address;
    std::string name;
    name.assign( a_name );
    remote_dev.name = UTF8_To_string( name );

    auto fun = std::bind( &Adaptor::OnDeviceFound, std::ref( Adaptor::GetInstance() ), remote_dev );
    std::shared_ptr< ExecutbleEvent > event = std::make_shared<ExecutbleEvent>( fun );
    PageManager::GetInstance().PostEvent( event );
}

void _inquiry_work_finished()
{
    std::shared_ptr<ExecutbleEvent> event = std::make_shared<ExecutbleEvent>
        (
            std::bind( &Adaptor::OnDiscoveryStateChanged, std::ref( Adaptor::GetInstance() ), DiscoveryState::BT_DISCOVERY_STOPPED )
        );
    PageManager::GetInstance().PostEvent( event );
}

void _inquiry_work_started()
{
    std::shared_ptr<ExecutbleEvent> event = std::make_shared<ExecutbleEvent>
        (
            std::bind( &Adaptor::OnDiscoveryStateChanged, std::ref( Adaptor::GetInstance() ), DiscoveryState::BT_DISCOVERY_STARTED )
        );
    PageManager::GetInstance().PostEvent( event );
}

void _local_settings_changed( int disconverable, int connectable )
{
    std::shared_ptr<ExecutbleEvent> event = std::make_shared<ExecutbleEvent>
        (
            std::bind( &Adaptor::OnLocalSettingsChanged, std::ref( Adaptor::GetInstance() ), disconverable == 1, connectable == 1 )
        );
    PageManager::GetInstance().PostEvent( event );
}

void _local_pairable_changed( int a_pairable )
{
    std::shared_ptr<ExecutbleEvent> event = std::make_shared<ExecutbleEvent>
        (
            std::bind( &Adaptor::OnPairableChanged, std::ref( Adaptor::GetInstance() ), a_pairable == 1 )
        );
    PageManager::GetInstance().PostEvent( event );
}

void _local_name_changed( char const* a_name )
{
    std::string name(a_name);
    std::shared_ptr<ExecutbleEvent> event = std::make_shared<ExecutbleEvent>
        (
            std::bind( &Adaptor::OnLocalNameChanged, std::ref( Adaptor::GetInstance() ), name )
        );
    PageManager::GetInstance().PostEvent( event );
}

void _inquiry_state_changed( int a_started )
{
    DiscoveryState state = DiscoveryState::BT_DISCOVERY_STOPPED;
    if( a_started )
    {
        state = DiscoveryState::BT_DISCOVERY_STARTED;
    }
    std::shared_ptr<ExecutbleEvent> event = std::make_shared<ExecutbleEvent>
        (
            std::bind( &Adaptor::OnDiscoveryStateChanged, std::ref( Adaptor::GetInstance() ), state )
        );
    PageManager::GetInstance().PostEvent( event );
}

void _pairing_confirm_request
    (
    char const* a_name,
    char const* a_addr,
    uint32_t    a_psk,
    uint32_t    a_method
    )
{
    std::string name;
    if( a_name )
    {
        name.assign( a_name );
        name = UTF8_To_string( name );
    }

    BluetoothAddress address;
    memcpy( address.address, a_addr, 6 );

    SppPairingMethod pm;
    pm = SppPairingMethod::BT_SSP_VARIANT_PASSKEY_CONFIRMATION;
    auto fun = std::bind( &Adaptor::OnSppPairingConfRequest, std::ref( Adaptor::GetInstance() ),
        address, name, a_psk, pm );
    std::shared_ptr< ExecutbleEvent > event = std::make_shared<ExecutbleEvent>( fun );
    PageManager::GetInstance().PostEvent( event );
}

}

BluetoothBaseImplementation& BluetoothBaseImplementation::GetInstance()
{
    static BluetoothBaseImplementation instance;
    return instance;
}

BluetoothBaseInterface& BluetoothBaseInterface::GetInterface()
{
    return BluetoothBaseImplementation::GetInstance();
}

BluetoothBaseImplementation::BluetoothBaseImplementation()
{
}

void BluetoothBaseImplementation::InitPlatform()
{
}

void BluetoothBaseImplementation::Init()
{
    if( !m_env_initialzied )
    {
        zephyr_init_();
    }
}

void BluetoothBaseImplementation::Enable( bool a_enable )
{
    Init();
}

void BluetoothBaseImplementation::StartSearch( bool a_search )
{
    if( a_search )
    {
        start_discovery();
    }
    else
    {
        int status = bt_br_discovery_stop();
        if( status == 0 || status == -EALREADY )
        {
            _inquiry_state_changed(false);
        }
    }
}

void BluetoothBaseImplementation::PairWithDevice( BluetoothAddress a_address )
{
    zephyr_pair_device(a_address.address);
}

void BluetoothBaseImplementation::DeletePairedDevice( BluetoothAddress a_address )
{
}

void BluetoothBaseImplementation::SetLocalName( std::string const& a_name )
{
    set_local_name(a_name.c_str());
}

bool BluetoothBaseImplementation::SetLocalDeviceSettings( bool a_pairable, bool a_discoverable, bool a_connectable )
{
    zephyr_bt_manager_br_set_visual( true, a_discoverable ? 1 : 0, a_connectable ? 1 : 0 );
    return true;
}

bool BluetoothBaseImplementation::SspPairingReply
    (
    BluetoothAddress a_address,
    bool a_accept,
    SppPairingMethod a_pairingMethod,
    uint32_t a_passkey
    )
{
    zephyr_pairing_passkey_reply( a_address.address, a_accept );
    return true;
}

bool BluetoothBaseImplementation::PincodeReply( BluetoothAddress a_address, bool a_accept, std::vector<uint8_t> a_pin )
{
    return false;
}

void* BluetoothBaseImplementation::GetFakeAudioInterfaceFromModule()
{
    return nullptr;
}

void BluetoothBaseImplementation::test()
{
}

void BluetoothBaseImplementation::OnPairedDeviceAddressReceived( std::vector<BluetoothAddress> a_addresses )
{
}

void BluetoothBaseImplementation::LoadAllPairedDevices()
{
}

bool BluetoothBaseImplementation::LoadBluetoothLibrary()
{
    return false;
}

std::vector<std::shared_ptr<bluetooth_interface>> BluetoothBaseInterface::LoadLowLevelInterfaces()
{
    std::vector<std::shared_ptr<bluetooth_interface>> interfaces;

    interfaces.push_back( std::make_shared<bluetooth_l2cap_coc_implementation>() );

    return interfaces;
}
