#ifndef __HOOK_H__
#define __HOOK_H__

#include <windows.h>

extern BYTE code_4F58D9[10];

BOOL hook_init(HINSTANCE hInst);
void hook(void);

#endif
