/*
 * log.h
 *
 *  Created on: May 14, 2018
 *      Author: root
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void trace_printf(const char *file, int line, const char *fmt, ...);

//#define trace(fmt, ...)		trace_printf(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define trace(fmt,...)			printf("[%s]/[%d]:",__FILE__,__LINE__); printf(fmt, ##__VA_ARGS__)

#endif /* LOG_H_ */
