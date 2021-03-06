/*
 * device.c
 *
 *  Created on: May 14, 2018
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "device.h"
#include "mrloopbf.h"
#include "log.h"

#define DEBUG
#undef DEBUG
FILE *file = NULL;

device_t *device;

device_t *device_init(uint8_t mode, uint8_t speed) {
	int status;

	device_t *device = NULL;
	device = (device_t *)calloc(1, sizeof(device_t));
	if (device == NULL) {
		return NULL;
	}
	device->mode = mode;
	device->speed = speed;
	memset(device->buff, 0x00, sizeof(device->buff));
	status = ML_Init();
#ifdef DEBUG
	status = 0;
	file = fopen("output.ts", "wb");
#endif
	if (status == 0) {
		trace("ML_Init failed\n");
		goto error;
	}
#ifndef DEBUG
	ML_HiddenDebugMsg();
#endif
	ML_GetDevGen();
	ML_SetMode(device->mode);
	ML_SetSpeed(device->speed);
	return device;
error:
	free(device);
	return NULL;
}

int device_release(device_t *device) {
	ML_Close();
	free(device);
	return 0;
}

int device_send(uint8_t *buff, int length) {
	if (device == NULL) {
		printf("device not attach\n");
		return -1;
	}
#ifdef DEBUG
	fwrite(buff,1,length,file);
#else
	return ML_Transfer(buff, length);
#endif
}

int device_receive(uint8_t *buff) {
	int recv_len = 4096;
	static bool ret = false;
	ret = ML_Receiver(buff, &recv_len);
	if (ret != true) {
		return -1;
	}
	else {
		return recv_len;
	}
}

