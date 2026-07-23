#pragma once
#include <BT/StackLayer/BluetoothBaseInterface.h>

class BluetoothBaseImplementation : public BluetoothBaseInterface
{

public:

    static BluetoothBaseImplementation& GetInstance();

    BluetoothBaseImplementation();

    void InitPlatform() override;

    void Init() override;

    void Enable( bool a_enable = true ) override;

    void StartSearch( bool a_search = true ) override;

    void PairWithDevice( BluetoothAddress a_address ) override;

    void DeletePairedDevice( BluetoothAddress a_address ) override;

    void SetLocalName( std::string const& a_name ) override;

    bool SetLocalDeviceSettings
        (
        bool a_pairable,     //!< [in] whether BT module is pairable
        bool a_discoverable, //!< [in] whether BT module is discoverable
        bool a_connectable   //!< [in] whether BT module is connectable
        ) override;

    bool SspPairingReply
        (
        BluetoothAddress a_address,
        bool             a_accept,
        SppPairingMethod a_pairingMethod,
        uint32_t a_passkey
        ) override;

    bool PincodeReply
        (
        BluetoothAddress     a_address,
        bool                 a_accept,
        std::vector<uint8_t> a_pin
        )override;

    void* GetFakeAudioInterfaceFromModule();

    void test() override;

public:

    void OnPairedDeviceAddressReceived
    (
        std::vector<BluetoothAddress> a_addresses
    );

    void LoadAllPairedDevices();

private:

    bool LoadBluetoothLibrary();

    bool m_env_initialzied = false;
};

