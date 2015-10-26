#pragma once

#include <stdint.h>

#define offsetof(a, b) ((size_t)(((void *)&((a *)NULL)->b) - NULL))

#ifndef NULL
#define NULL 0
#endif

#define BIT(n) (1 << (n))

typedef uint64_t size_t;
typedef int64_t ssize_t;
typedef uint16_t wchar_t;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float float32;
typedef double float64;

typedef volatile uint8_t vuint8;
typedef volatile uint16_t vuint16;
typedef volatile uint32_t vuint32;
typedef volatile uint64_t vuint64;

typedef volatile int8_t vint8;
typedef volatile int16_t vint16;
typedef volatile int32_t vint32;
typedef volatile int64_t vint64;

typedef volatile float32 vfloat32;
typedef volatile float64 vfloat64;

typedef uint8_t byte;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

//typedef int ptrdiff_t;
//typedef unsigned int *uintptr_t;
//typedef int *intptr_t;

/* POSIX types */

typedef uint32_t __dev_t;
typedef uint32_t blksize_t;
typedef int64_t  blkcnt_t;
typedef uint32_t dev_t;
typedef uint32_t fflags_t;
typedef uint32_t gid_t;
typedef uint32_t ino_t;
typedef uint16_t mode_t;
typedef uint16_t nlink_t;
typedef int64_t  off_t;
typedef uint32_t uid_t;
typedef int64_t time_t;
typedef long suseconds_t;

struct timespec {
	time_t tv_sec;
	long tv_nsec;
};

struct timeval {
	time_t tv_sec;
	suseconds_t tv_usec;
};

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

/* SCE types */

typedef unsigned int SceKernelUseconds;
