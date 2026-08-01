#ifndef PAIRING_MANAGER_H___
#define PAIRING_MANAGER_H___
#include <zephyr/kernel.h>

typedef enum pairing_state_
{
    pairing_idle,
    link_connecting,
    io_capability_checking,
    pairing_confirm,
    pairing_success,
    pairing_failed
} pairing_state;

typedef struct pairing_manager_
{
    struct k_mutex m_mutex;
    struct bt_conn* m_conn;
    unsigned int m_passkey;
    pairing_state state;
} pairing_manager_t;

void handle_connected( pairing_manager_t* a_manager, struct bt_conn* );

void pairing_manager_initialize( pairing_manager_t* a_manager );

void reply_pairing_passkey_confirm( pairing_manager_t* a_manager, const uint8_t* a_addr, int a_accept );

void handle_passkey_confirm_request( pairing_manager_t* a_manager, struct bt_conn*, unsigned int );

#endif

