#pragma once

#include "types.h"

extern void *(*malloc)(size_t size);
extern void (*free)(void *ptr);
extern void *(*memset)(void *destination, int value, size_t num);
extern void *(*memcpy)(void *destination, const void *source, size_t num);
extern char *(*strcpy)(char *destination, const char *source);
extern char *(*strcat)(char *dest, const char *src);
extern char *(*strncat)(char *dest, const char *src, size_t n);
extern size_t (*strlen)(const char *s);
extern int (*strcmp)(const char *s1, const char *s2);
extern int (*strncmp)(const char *s1, const char *s2, size_t n);
extern int (*sprintf)(char *str, const char *format, ...);
extern int (*snprintf)(char *str, size_t size, const char *format, ...);
extern int (*sscanf)(const char *str, const char *format, ...);
extern char *(*strchr)(const char *s, int c);
extern char *(*strrchr)(const char *s, int c);

extern char *(*asctime)(const struct tm *tm);
extern char *(*ctime)(const time_t *timep);
extern struct tm *(*gmtime)(const time_t *timep);
extern struct tm *(*localtime)(const time_t *timep);
extern time_t (*mktime)(struct tm *tm);

void initLibc(void);
