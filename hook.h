#ifndef __HOOK_H__
#define __HOOK_H__

#include <windows.h>

#define CODEADDR_TO_PATCH 0x4F58D9
#define CODESIZE_TO_PATCH 10

extern char *binary_data;
extern size_t binary_size;

extern BYTE CODE_OLD[10];
extern BYTE CODE_NEW[10];

void hook();

#endif
