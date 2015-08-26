#pragma once

extern int (*scePadInit)(void);
extern int (*scePadOpen)(int, int, int, void *);
extern int (*scePadRead)(int handle, void *data, int count);

void initPad(void);
