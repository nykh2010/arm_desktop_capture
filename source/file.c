/*
 * file.c
 *
 *  Created on: May 14, 2018
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "file.h"
#include "source.h"
#include "log.h"

#define FILE_PATH	"out.ts"
FILE *outfile = NULL;
pthread_t write_file_pthread;

int open_file() {
	outfile = fopen(FILE_PATH, "wb");
	if (outfile == NULL) {
		trace("can not open file\n");
		return -1;
	}
	trace("open_file success\n");
	return 0;
}

int file_release() {
	trace("enter file_release\n");
	pthread_join(write_file_pthread, NULL);
	fclose(outfile);
	return 0;
}

static void *write_file_handle(void *arg) {
	uint8_t *data = NULL;
	int length;
	int status;
	trace("enter write_file_handle\n");
	while(1) {
		data = NULL;
		status = get_ts(&data, &length);
		if (status < 0) {
			continue;
		}
		fwrite(data, 1, length, outfile);
		free(data);
	}
	pthread_exit(0);
}

int write_file_start() {
	pthread_create(&write_file_pthread, NULL, write_file_handle, NULL);
	return 0;
}

