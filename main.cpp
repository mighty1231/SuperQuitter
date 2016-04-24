// #include "plugin.h"
#include "hook.h"
#include <stdio.h>

// BOOL WINAPI GetMPQDraftPlugin(IMPQDraftPlugin **lppMPQDraftPlugin) {
// 	*lppMPQDraftPlugin = Plugin::getInstance();
// 	return TRUE;
// }

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved) {
	DWORD oldProtect;
	DWORD startTick, curTick;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;

		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			freopen("c:/workspace/dialogRestoration/t.txt", "w+", stdout);
			if (hook_init(hModule) == FALSE) {
				printf("hook failed fuck\n");
		fflush(stdout);
				return FALSE;
			}
			startTick = GetTickCount();
			while (memcmp((void *)0x4F58D9, code_4F58D9, sizeof(code_4F58D9)) != 0) {
				curTick = GetTickCount();
				if ((curTick - startTick) > 2000) {
					break;
				}
			}
			if (memcmp((void *)0x4F58D9, code_4F58D9, sizeof(code_4F58D9)) != 0) {
				printf("memcmp failed fuck %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
					((char *)0x4F58D9)[0], 
					((char *)0x4F58D9)[1], 
					((char *)0x4F58D9)[2], 
					((char *)0x4F58D9)[3], 
					((char *)0x4F58D9)[4], 
					((char *)0x4F58D9)[5], 
					((char *)0x4F58D9)[6], 
					((char *)0x4F58D9)[7], 
					((char *)0x4F58D9)[8], 
					((char *)0x4F58D9)[9]);
		fflush(stdout);
				return FALSE;
			}

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
			*(char *)0x4F58D9 = 0xBA;
			*(int *)(0x4F58D9+1) = (int)hook;           // mov edx, hook
			*(short *)(0x4F58D9+5) = 0xE2FF;            // jmp edx
			*(char *)(0x4F58D9+7) = 0x90;               // nop, covering previous code
			*(short *)(0x4F58D9+8) = 0x9090;            // nop, covering previous code
			VirtualProtect((void *)0x4F58D9, 10, oldProtect, &oldProtect);
			break;

		case DLL_THREAD_DETACH:
		printf("detached fuck\n");
		fflush(stdout);
			VirtualProtect((void *)0x4F58D9, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
			memcpy((void *)0x4F58D9, code_4F58D9, sizeof(code_4F58D9));
			VirtualProtect((void *)0x4F58D9, 10, oldProtect, &oldProtect);
			break;
	}

	return TRUE;
}
