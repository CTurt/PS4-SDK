#pragma once

#include "types.h"

extern void *(*memset)(void *destination, int value, size_t num);
extern void *(*memcpy)(void *destination, const void *source, size_t num);
extern char *(*strcpy)(char *destination, const char *source);
extern int (*sprintf)(char *str, const char *format, ...);

void initLibc(void);
