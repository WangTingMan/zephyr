#ifndef PAIRING_MANAGER_H___
#define PAIRING_MANAGER_H___

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
    struct bt_conn* conn;
    pairing_state state;
} pairing_manager_t;

void handle_connected( pairing_manager_t* a_manager, struct bt_conn* );

void pagemanger_initialize( pairing_manager_t* a_manager );

#endif

