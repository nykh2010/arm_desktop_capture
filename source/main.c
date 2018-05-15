/*
 * main.c
 *
 *  Created on: May 14, 2018
 *      Author: root
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "source.h"
#include "file.h"
#include "log.h"
//#include "device.h"

int main(int argc, char *argv[]) {
	int status;

	status = open_source();
	if (status < 0) {
		trace("open_source failed\n");
		exit(-1);
	}
	status = open_file();
	if (status < 0) {
		trace("open_file failed\n");
		exit(-1);
	}
	capture_source_start();
	write_file_start();

	source_release();
	file_release();
	exit(0);
}

