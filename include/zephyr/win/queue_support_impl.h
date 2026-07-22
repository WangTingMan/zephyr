#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void queue_prepend(uint16_t id, void* data);

int queue_is_empty(uint16_t id);

void* queue_get(uint16_t id, uint32_t timeout);

uint16_t queue_init(uint16_t id);

void queue_set_element_size( uint16_t id, uint32_t size );

uint32_t queue_get_element_size( uint16_t id );

void queue_append(uint16_t id, void* data);

void* queue_peek_head(uint16_t id);

#ifdef __cplusplus
} /* extern "C" */
#endif

