#pragma once

#include "types.h"

extern void *(*malloc)(size_t size);
extern void *(*memset)(void *destination, int value, size_t num);
extern void *(*memcpy)(void *destination, const void *source, size_t num);
extern char *(*strcpy)(char *destination, const char *source);
extern size_t (*strlen)(const char *s);
extern int (*sprintf)(char *str, const char *format, ...);
extern int (*sscanf)(const char *str, const char *format, ...);
extern char *(*strchr)(const char *s, int c);
extern char *(*strrchr)(const char *s, int c);


void initLibc(void);
