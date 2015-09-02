#pragma once

extern int (*scePadInit)(void);
extern int (*scePadOpen)(int userID, int, int, void *);
extern int (*scePadClose)(int handle);
extern int (*scePadRead)(int handle, void *data, int count);
extern int (*scePadReadState)(int handle, void *data);

void initPad(void);
