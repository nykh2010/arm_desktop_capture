/*
 * source.c
 *
 *  Created on: May 14, 2018
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "source.h"
#include "log.h"


source_t *source = NULL;
pthread_mutex_t ts_list_head_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t capture_source_pthread;

int ts_list_append(ts_list_t *ts) {
	pthread_mutex_lock(&ts_list_head_mutex);
	if (source->ts_list_tail->next == NULL) {
		source->ts_list_tail->next = ts;
		source->ts_list_tail = ts;
	}
	pthread_mutex_unlock(&ts_list_head_mutex);
	return 0;
}

int write_ts_frame(uint8_t *buff, int buff_size) {
	ts_list_t *ts = (ts_list_t *)calloc(1,sizeof(ts_list_t));
	if (ts == NULL) {
		trace("ts calloc failed\n");
		return -1;
	}
	ts->data = (uint8_t *)calloc(1,buff_size);
	if (ts->data == NULL) {
		trace("ts data calloc failed\n");
		return -1;
	}
	ts->length = buff_size;
	ts->next = NULL;
	memcpy(ts->data, buff, buff_size);
	ts_list_append(ts);
	return 0;
}

int get_ts(uint8_t **data, int *length) {
	ts_list_t *ts_list;
	if (source->ts_list_head->next == NULL) {
		return -1;
	}
	*data = (uint8_t *)calloc(1, source->ts_list_head->next->length);
	if (data == NULL) {
		return -1;
	}
	memcpy(*data, source->ts_list_head->next->data, source->ts_list_head->next->length);
	*length = source->ts_list_head->next->length;
	pthread_mutex_lock(&ts_list_head_mutex);
	ts_list = source->ts_list_head->next;
	source->ts_list_head->next = ts_list->next;
	if (source->ts_list_head->next == NULL) {
		source->ts_list_tail = source->ts_list_head;
	}
	free(ts_list->data);
	free(ts_list);
	pthread_mutex_unlock(&ts_list_head_mutex);
	return 0;
}

int open_source() {
	source = (source_t *)calloc(1,sizeof(source_t));
	if (source == NULL) {
		trace("calloc source failed\n");
		return -1;
	}
	source->device = device_init(SLAVE, 7);
	if (source->device == NULL) {
		free(source);
		trace("device init failed\n");
		return -1;
	}
	trace("device init success\n");
	source->ts_list_head = (ts_list_t *)calloc(1,sizeof(ts_list_t));
	if (source->ts_list_head == NULL) {
		device_release(source->device);
		free(source);
		trace("ts_list_head calloc failed\n");
		return -1;
	}
	source->ts_list_tail = source->ts_list_head;
	pthread_mutex_init(&ts_list_head_mutex, NULL);
	trace("open_source success\n");
	return 0;
}

int source_release() {
	pthread_join(capture_source_pthread, NULL);
	pthread_mutex_destroy(&ts_list_head_mutex);
	device_release(source->device);
	free(source->ts_list_head);
	free(source);
	return 0;
}

static void *capture_source_handle(void *arg) {
	int length;
	//volatile int status;
	trace("enter capture_source_handle\n");
	while(1) {
		//length = 4096;
		//memset(source->device->buff, 0x00, length);
		length = device_receive(source->device->buff);
		if (length >= 0) {
			//trace("receive one frame:%d\n",length);
			write_ts_frame(source->device->buff, length);
		}
	}
	pthread_exit(0);
}

int capture_source_start() {
	//int status;
	trace("capture_source_start\n");
	pthread_create(&capture_source_pthread, NULL, capture_source_handle, NULL);
	return 0;
}

