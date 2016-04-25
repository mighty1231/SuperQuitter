#include "hook.h"
#include <stdio.h>

/*
 * DllMain called multiple times,
 * since Starcraft is multi-threaded application,
 * and DllMain is called for every thread of Starcraft.
 * */
int calledCount = 0;
BOOL WINAPI DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved) {
	DWORD oldProtect;
	// DWORD startTick, curTick;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;

		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			if (calledCount++ == 0) {
				if (hook_init(hModule) == FALSE)
					return FALSE;
				if (memcmp((void *)0x4F58D9, code_4F58D9_old, sizeof(code_4F58D9_new)) != 0)
					return FALSE;

				/*
				 * 004F58D9 : push 0x000000DE
				 * 004F58DE : push 0x00502268 (== "Starcraft\SWAR\lang\gamedata.cpp")
				 * 004F58E3 : push ebx
				 * 004F58E4 : push ebx
				 * 004F58E5 : push edi (check for fileName == "rez\\quit2mnu.bin")
				 * 004F58E6 : xor ecx, ecx
				 * 004F58E8 : xor eax, eax
				 * 004F58EA : call 4D2D10 (file reading function)
				 */
				VirtualProtect((void *)0x4F58D9, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
				memcpy((void *)0x4F58D9, code_4F58D9_new, 10);
				VirtualProtect((void *)0x4F58D9, 10, oldProtect, &oldProtect);
			}
			break;

		case DLL_THREAD_DETACH:
			if (--calledCount == 0) {
				VirtualProtect((void *)0x4F58D9, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
				memcpy((void *)0x4F58D9, code_4F58D9_old, 10);
				VirtualProtect((void *)0x4F58D9, 10, oldProtect, &oldProtect);
			}
			break;
	}

	return TRUE;
}
