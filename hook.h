#ifndef __HOOK_H__
#define __HOOK_H__

#include <windows.h>

extern BYTE code_4F58D9_old[10];
extern BYTE code_4F58D9_new[10];

BOOL hook_init(HINSTANCE hInst);
void hook();

#endif
