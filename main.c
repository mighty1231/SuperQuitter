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

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpReserved) {
	MEMORY_BASIC_INFORMATION mbi;
	wchar_t moduleName[MAX_PATH];
	DWORD oldProtect;
	int i;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;

		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			calledCount++;
			if (calledCount == 0) {
				/* Just check updatable, comparing expected memory */
				if (VirtualQuery((void *)CODEADDR_TO_PATCH, &mbi, sizeof(mbi)) != sizeof(mbi)) {
					sqlog(L"DllMain.Attach 에러: VirtualQuery 실패");
					return FALSE;
				}

				/* AllocationProtect
				 * my case : PAGE_EXECUTE_WRITECOPY 
				 * some(?) plugin may not recovered it from PAGE_EXECUTE_READWRITE */
				if (mbi.Type == MEM_IMAGE &&
					mbi.State == MEM_COMMIT &&
					(mbi.AllocationProtect & 
						(PAGE_EXECUTE_WRITECOPY | PAGE_EXECUTE_READWRITE |
							PAGE_EXECUTE | PAGE_EXECUTE_READ))) {

					/* Query done! just compare the memory to patch */
					if (memcmp((void *)CODEADDR_TO_PATCH, CODE_OLD, CODESIZE_TO_PATCH) == 0) {
						if (hook_init(hInst) == FALSE) {
							sqlog(L"DllMain.Attach 에러: hook_init 실패");
							return FALSE;
						}
						VirtualProtect((void *)CODEADDR_TO_PATCH, CODESIZE_TO_PATCH, PAGE_EXECUTE_READWRITE, &oldProtect);
						memcpy((void *)CODEADDR_TO_PATCH, CODE_NEW, CODESIZE_TO_PATCH);
						VirtualProtect((void *)CODEADDR_TO_PATCH, CODESIZE_TO_PATCH, oldProtect, &oldProtect);
						sqlog(L"DllMain.Attach 성공: 스타크래프트 프로세스 확인");
						isOnStarcraft = TRUE;
						return TRUE;
					} else {
						sqlog(L"DllMain.Attach 에러: 메모리가 스타크래프트와는 다름");
						sqlog(L"  - 스타크래프트가 맞다면 다른 플러그인을 의심");
						for (i = 0; i < CODESIZE_TO_PATCH; i++) {
							if (CODE_OLD[i] != ((BYTE *)CODEADDR_TO_PATCH)[i])
								sqlog(L"  - [0x%06X] 실제: %02X, 예상: %02X",
									CODEADDR_TO_PATCH+i,
									((BYTE *)CODEADDR_TO_PATCH)[i],
									CODE_OLD[i]);
						}
						sqlog(L" - 실행파일 경로");
						sqlog(L"   %s", moduleName);
						return FALSE;
					}
				} else {
					sqlog(L"DllMain.Attach 에러: 스타크래프트가 아닌 것으로 판단됨");
					sqlog(L" - 메모리 정보 %X %X %X", mbi.Type, mbi.State, mbi.AllocationProtect);
					GetModuleFileNameW(0, moduleName, MAX_PATH);
					sqlog(L" - 실행파일 경로");
					sqlog(L"   %s", moduleName);
					return FALSE;
				}
			}
			break;
		case DLL_THREAD_DETACH:
			if (--calledCount == 0 && isOnStarcraft) {
				VirtualProtect((void *)CODEADDR_TO_PATCH, CODESIZE_TO_PATCH, PAGE_EXECUTE_READWRITE, &oldProtect);
				memcpy((void *)CODEADDR_TO_PATCH, CODE_OLD, CODESIZE_TO_PATCH);
				VirtualProtect((void *)CODEADDR_TO_PATCH, CODESIZE_TO_PATCH, oldProtect, &oldProtect);
				sqlog(L"DllMain.Detach 성공");
			}
			break;
	}

	return TRUE;
}
