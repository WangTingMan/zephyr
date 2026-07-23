/*
 * Copyright (c) 2017-2021 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <zephyr/autoconf.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/logging/log.h>
#include <zephyr/net_buf.h>

#define LOG_LEVEL CONFIG_BT_HCI_CORE_LOG_LEVEL
LOG_MODULE_REGISTER(bt_data);

void bt_data_parse(struct net_buf_simple *ad,
		   bool (*func)(struct bt_data *data, void *user_data),
		   void *user_data)
{
	while (ad->len > 1) {
		struct bt_data data;
		uint8_t len;

		len = net_buf_simple_pull_u8(ad);
		if (len == 0U) {
			/* Early termination */
			return;
		}

		if (len > ad->len) {
			LOG_WRN("malformed advertising data %u / %u",
				len, ad->len);
			return;
		}

		data.type = net_buf_simple_pull_u8(ad);
		data.data_len = len - 1;
		data.data = ad->data;

		if (!func(&data, user_data)) {
			return;
		}

		net_buf_simple_pull(ad, len - 1);
	}
}

size_t bt_data_get_len(const struct bt_data data[], size_t data_count)
{
	size_t total_len = 0;

	for (size_t i = 0; i < data_count; i++) {
		total_len += sizeof(data[i].data_len) + sizeof(data[i].type) + data[i].data_len;
	}

	return total_len;
}

size_t bt_data_serialize(const struct bt_data *input, uint8_t *output)
{
	if (input == NULL) {
		LOG_DBG("input is NULL");
		return 0;
	}

	if (output == NULL) {
		LOG_DBG("output is NULL");
		return 0;
	}

	uint8_t ad_data_len = input->data_len;
	uint8_t data_len = ad_data_len + 1;

	output[0] = data_len;
	output[1] = input->type;

	memcpy(&output[2], input->data, ad_data_len);

	return data_len + 1;
}

uint16_t ltv_parser( uint8_t const* a_raw_data, uint16_t a_raw_data_size,
    struct bt_data* a_out, uint16_t a_max_ltv_to_use )
{
    if( ( a_raw_data == NULL ) || ( a_out == NULL ) || ( a_max_ltv_to_use == 0U ) ) {
        return 0U;
    }

    uint16_t parsed_cnt = 0U;
    uint16_t offset = 0U;

    while( ( offset < a_raw_data_size ) && ( parsed_cnt < a_max_ltv_to_use ) ) {
        if( ( offset + 1U ) > a_raw_data_size ) {
            break;
        }

        const uint8_t ltv_total_len = a_raw_data[offset];
        const uint8_t type_offset = offset + 1U;
        const uint8_t value_offset = type_offset + 1U;

        const uint16_t single_ltv_bytes = 1U + ltv_total_len;

        if( ( offset + single_ltv_bytes ) > a_raw_data_size ) {
            break;
        }

        uint8_t ad_type = a_raw_data[type_offset];
        if( ad_type == 0U ) {
            break;
        }

        uint8_t len = ( ltv_total_len >= 1U ) ? ( ltv_total_len - 1U ) : 0U;

        if( len == 0x00 )
        {
            break;
        }

        a_out[parsed_cnt].type = ad_type;
        a_out[parsed_cnt].data_len = len;
        a_out[parsed_cnt].data = &a_raw_data[value_offset];

        parsed_cnt++;
        offset += single_ltv_bytes;
    }

    return parsed_cnt;
}
