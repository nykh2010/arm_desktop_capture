/*
 * log.c
 *
 *  Created on: May 14, 2018
 *      Author: root
 */
#include "log.h"
#include <pthread.h>

pthread_mutex_t write_log_mutex = PTHREAD_MUTEX_INITIALIZER;

void trace_printf(const char *file, int line, const char *fmt, ...) {
	pthread_mutex_lock(&write_log_mutex);
	int len = strlen(fmt);
	char prompt[] = "[%s]/[%d]:";
	char *fmt_buf = (char *)calloc(1, len+strlen(prompt)+1);
	if (fmt_buf == NULL) {
		printf("write log failed\n");
		pthread_mutex_unlock(&write_log_mutex);
		return;
	}
	strcpy(fmt_buf, prompt);
	strcat(fmt_buf, fmt);
	va_list arg;
	va_start(arg,fmt);
	printf(fmt_buf, file, line, arg);
	va_end(arg);
	if (fmt_buf != NULL)
		free(fmt_buf);
	pthread_mutex_unlock(&write_log_mutex);
}
