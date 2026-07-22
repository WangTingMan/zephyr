#include <zephyr/settings/settings.h>

#define SETTINGS_POOL_SIZE 100
struct settings_handler_static* _settings_handler_list[SETTINGS_POOL_SIZE] = { 0x00 };
uint32_t s_settings_next_id = 0;

void register_settings_handler( struct settings_handler_static* a_register_handler )
{
    uint32_t id = s_settings_next_id;
    s_settings_next_id++;
    _settings_handler_list[id] = a_register_handler;
}

/**
 * the macro named STRUCT_SECTION_FOREACH will call this.
 */
struct settings_handler_static** _settings_handler_static_list_start()
{
    return _settings_handler_list + 0;
}

struct settings_handler_static** _settings_handler_static_list_end()
{
    return _settings_handler_list + s_settings_next_id;
}
