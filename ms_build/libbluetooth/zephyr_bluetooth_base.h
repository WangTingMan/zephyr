#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void zephyr_init_();

void start_discovery();

void zephyr_bt_manager_br_set_visual( int enable, int discoverable, int connectable );

#ifdef __cplusplus
}
#endif
