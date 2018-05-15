/*
 * source.h
 *
 *  Created on: May 14, 2018
 *      Author: root
 */

#ifndef SOURCE_H_
#define SOURCE_H_

#include <stdint.h>
#include <device.h>

typedef struct TS {
	uint8_t *data;
	int length;
	struct TS *next;
} ts_list_t;

typedef struct {
	device_t *device;
	ts_list_t *ts_list_head;
	ts_list_t *ts_list_tail;
} source_t;

int open_source();
int source_release();
int capture_source_start();
int get_ts(uint8_t **data, int *length);

extern source_t *source;

#endif /* SOURCE_H_ */
