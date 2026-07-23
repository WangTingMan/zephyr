#pragma once

#include <BT/StackLayer/BluetoothHFInterface.h>

class BluetoothHFImplementation : public BluetoothHFInterface
{

public:

    static BluetoothHFImplementation& GetInstance();

    BluetoothHFImplementation();

    virtual void Init();

    virtual bool Connect
        (
        BluetoothAddress a_address
        );

    virtual bool disconnect
        (
        BluetoothAddress a_address
        );

    virtual bool setActiveDevice
        (
        BluetoothAddress a_address
        );

    virtual bool setSilenceDevice
        (
        BluetoothAddress a_address
        );

    virtual bool dialNumber
        (
        BluetoothAddress a_address,
        std::string a_number
        );

    virtual void answerIncomingCall
        (
        BluetoothAddress& a_address,
        bool a_answered = true
        );

    // Send AT+CHLD=2 to phone.
    virtual void swapCalls
        (
        BluetoothAddress a_address
        );

    virtual void joinCalls
        (
        BluetoothAddress a_address
        );

    virtual void splitCall
        (
        BluetoothAddress a_address,
        int a_index
        );

    bool QueryCallList
        (
        BluetoothAddress a_address
        );

private:

    bool m_initialized = false;
};

