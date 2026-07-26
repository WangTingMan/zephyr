#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void zephyr_init_();

void start_discovery();

void zephyr_bt_manager_br_set_visual( int a_pairable, int discoverable, int connectable );

void set_local_name(const char* a_name);

int zephyr_pair_device(const uint8_t* a_addr);

#ifdef __cplusplus
}
#endif
