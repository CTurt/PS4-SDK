#pragma once

#include "types.h"
#include "file.h"

typedef struct DIR DIR;

extern void *(*malloc)(size_t size);
extern void (*free)(void *ptr);
extern void *(*calloc)(size_t num, size_t size);
extern void *(*realloc)(void* ptr, size_t size);
extern void *(*memset)(void *destination, int value, size_t num);
extern void *(*memcpy)(void *destination, const void *source, size_t num);
extern int (*memcmp)(const void *s1, const void *s2, size_t n);
extern char *(*strcpy)(char *destination, const char *source);
extern char *(*strncpy)(char *destination, const char *source, size_t num);
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

extern void (*srand)(unsigned int seed);
extern int (*rand)(void);

extern char *(*asctime)(const struct tm *tm);
extern char *(*asctime_r)(const struct tm *tm, char *buf);
extern char *(*ctime)(const time_t *timep);
extern char *(*ctime_r)(const time_t *timep, char *buf);
extern struct tm *(*gmtime)(const time_t *timep);
extern struct tm *(*gmtime_r)(const time_t *timep, struct tm *result);
extern struct tm *(*localtime)(const time_t *timep);
extern struct tm *(*localtime_r)(const time_t *timep, struct tm *result);
extern time_t (*mktime)(struct tm *tm);

extern DIR *(*opendir)(const char *filename);
extern struct dirent *(*readdir)(DIR *dirp);
extern int (*readdir_r)(DIR *dirp, struct dirent *entry, struct dirent **result);
extern long (*telldir)(const DIR *dirp);
extern void (*seekdir)(DIR *dirp, long loc);
extern void (*rewinddir)(DIR *dirp);
extern int (*closedir)(DIR *dirp);
extern int (*dirfd)(DIR *dirp);

void initLibc(void);
