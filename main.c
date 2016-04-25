#include "hook.h"
#include "sqlog.h"
#include <stdio.h>
#include <wchar.h>

/*
 * DllMain called multiple times,
 * since Starcraft is multi-threaded application,
 * and DllMain is called for every thread of Starcraft.
 * */
int calledCount = 0;
BOOL isOnStarcraft = FALSE;
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpReserved) {
	DWORD oldProtect;
	wchar_t moduleName[MAX_PATH];
	// DWORD startTick, curTick;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;

		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			/* Techniques from 
			 * https://github.com/tec27/ObsGraph/blob/master/ObsGraph/DllMain.cpp
			 */
			if (calledCount == 0) {
				GetModuleFileNameW(0, moduleName, MAX_PATH);
				if (wcsstr(moduleName, L"Starcraft.exe")) {
					isOnStarcraft = TRUE;
				} else if (wcsstr(moduleName, L"wLauncher.exe") || 
					wcsstr(moduleName, L"Chaoslauncher.exe")) {
					sqlog(L"Launcher: load complete");
					isOnStarcraft = FALSE;
				} else {
					MessageBoxW(
						0,
						moduleName,
						L"The module could not be loaded",
						MB_ICONERROR);
					return FALSE;
				}
			}
			if (calledCount++ == 0 && isOnStarcraft) {
				if (hook_init(hInst) == FALSE) {
					sqlog(L"Starcraft: hook_init failed during attaching");
					return FALSE;
				}
				if (memcmp((void *)0x4F58D9, code_4F58D9_old, sizeof(code_4F58D9_new)) != 0) {
					sqlog(L"Starcraft: memcmp failed during attaching");
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
				memcpy((void *)0x4F58D9, code_4F58D9_new, 10);
				VirtualProtect((void *)0x4F58D9, 10, oldProtect, &oldProtect);
				sqlog(L"Starcraft: attaching success");
			}
			break;

		case DLL_THREAD_DETACH:
			if (--calledCount == 0 && isOnStarcraft) {
				VirtualProtect((void *)0x4F58D9, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
				memcpy((void *)0x4F58D9, code_4F58D9_old, 10);
				VirtualProtect((void *)0x4F58D9, 10, oldProtect, &oldProtect);
				sqlog(L"Starcraft: detaching success");
			}
			break;
	}

	return TRUE;
}
