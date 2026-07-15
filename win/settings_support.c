#include <zephyr/settings/settings.h>

struct settings_handler_static* _settings_handler_static_list_start = NULL;
struct settings_handler_static* _settings_handler_static_list_end = NULL;

void register_settings_handler( struct settings_handler_static* a_register_handler )
{
    //TODO...register these handler in other modules.
}

