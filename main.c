#include "hook.h"
#include "sqlog.h"
#include <stdio.h>
#include <wchar.h>

/*
 * DllMain called multiple times,
 * since Starcraft is multi-threaded application,
 * and DllMain is called for every thread of Starcraft.
 * */
int calledCount = -1;
BOOL isOnStarcraft = FALSE;

struct windowInfo {
	HWND hwnd;
	wchar_t className[20];
};

BOOL CALLBACK EnumWindowsProc_getClassName(HWND hwnd, LPARAM lParam);

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpReserved) {
	DWORD oldProtect;
	wchar_t moduleName[MAX_PATH];
	HWND handle;
	wchar_t className[20];
	// DWORD startTick, curTick;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;

		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			calledCount++;
			if (calledCount == 0) {
				/* hwnd : the first enumerate window or SWarClass */
				struct windowInfo wi;
				wi.hwnd = 0;
				EnumWindows(&EnumWindowsProc_getClassName, (LPARAM) &wi);

				if (wi.hwnd != 0 &&
					wcscmp(wi.className, L"SWarClass") == 0){
					isOnStarcraft = TRUE;
				} else {
					sqlog(L"에러: 스타 이외의 프로세스가 dll를 로드하려함.");
					GetModuleFileNameW(0, moduleName, MAX_PATH);
					sqlog(L" - 클래스 이름: %s", wi.className);
					sqlog(L" - 실행파일 경로");
					sqlog(L"   %s", moduleName);
					// sqlog(L" - Currently, the module supports");
					// sqlog(L"   wLauncher.exe, Chaoslauncher.exe, Starcraft.exe");
					return FALSE;
				}
				// /* get module name */
				// GetModuleFileNameW(0, moduleName, MAX_PATH);
				// if (wcsstr(moduleName, L"Starcraft.exe") || wcsstr(moduleName, L"starcraft.exe")) {
				// 	isOnStarcraft = TRUE;
				// } else if (wcsstr(moduleName, L"wLauncher.exe") || 
				// 	wcsstr(moduleName, L"Chaoslauncher.exe")) {
				// 	sqlog(L"Launcher: load complete");
				// 	isOnStarcraft = FALSE;
				// } else {
				// 	sqlog(L"Unknown executable attempting to load the dll");
				// 	sqlog(L" - Path of the executable is %s", moduleName);
				// 	sqlog(L" - Currently, the module supports");
				// 	sqlog(L"   wLauncher.exe, Chaoslauncher.exe, Starcraft.exe");
				// 	return FALSE;
				// }
			}
			if (calledCount == 0 && isOnStarcraft) {
				if (hook_init(hInst) == FALSE) {
					sqlog(L"스타크래프트에 로드 실패: hook_init 함수 에러");
					return FALSE;
				}
				if (memcmp((void *)0x4F58D9, code_4F58D9_old, sizeof(code_4F58D9_new)) != 0) {
					sqlog(L"스타크래프트에 로드 실패: 메모리가 이상함을 발견");
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
				sqlog(L"스타크래프트에 로드 성공");
			}
			break;

		case DLL_THREAD_DETACH:
			if (--calledCount == 0 && isOnStarcraft) {
				VirtualProtect((void *)0x4F58D9, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
				memcpy((void *)0x4F58D9, code_4F58D9_old, 10);
				VirtualProtect((void *)0x4F58D9, 10, oldProtect, &oldProtect);
				sqlog(L"스타크래프트에 언로드 성공");
			}
			break;
	}

	return TRUE;
}

BOOL CALLBACK EnumWindowsProc_getClassName(HWND hwnd, LPARAM lParam){
	struct windowInfo *res = (struct windowInfo *)lParam;
	DWORD process;
	GetWindowThreadProcessId(hwnd, &process);
	if (GetCurrentProcessId() == process) {
		res->hwnd = hwnd;
		GetClassNameW(hwnd, res->className, 20);
		return FALSE;
	}
	return TRUE;
}
