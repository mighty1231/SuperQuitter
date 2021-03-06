#include "hook.h"
#include "sqlog.h"
#include "resources.h"
#include "main.h"
#include "hook.h"
#include <stdio.h>
#include <wchar.h>


BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}

	return TRUE;
}

// this part would work as main part of the thread on starcraft.
DWORD WINAPI MainLoop(LPVOID lpParam) {
	HMODULE hInst = (HMODULE)lpParam;

	// init resource for bin file
	HRSRC rc;
	HGLOBAL rcData;
	rc = FindResource(hInst, MAKEINTRESOURCE(IDC_QUIT2MNUBIN), RT_RCDATA);
	rcData = LoadResource(hInst, rc);
	binary_data = (char *) LockResource(rcData);
	binary_size = SizeofResource(hInst, rc);

	// Manage hook address on arg of jmp (assembly)
	*(DWORD *)(CODE_NEW+1) = (DWORD)&hook;

	#define IsMultiplayerMode (*(BYTE *) 0x57F0B4)
	#define CurrentNetworkMode (*(DWORD *) 0x59688C)

	DWORD oldProtect;
	DWORD isHookSet = FALSE;
	while (TRUE) {
		if (!isHookSet && CurrentNetworkMode != 0xFFFFFFFF) {
			// Set the hook
			VirtualProtect((LPVOID)CODEADDR_TO_PATCH, CODESIZE_TO_PATCH, PAGE_EXECUTE_READWRITE, &oldProtect);
			memcpy((LPVOID)CODEADDR_TO_PATCH, CODE_NEW, CODESIZE_TO_PATCH);
			VirtualProtect((LPVOID)CODEADDR_TO_PATCH, CODESIZE_TO_PATCH, oldProtect, &oldProtect);

			isHookSet = TRUE;
		}
		else if (isHookSet && CurrentNetworkMode == 0xFFFFFFFF) {
			// Before getting into Asia server
			// Unset the hook
			VirtualProtect((LPVOID)CODEADDR_TO_PATCH, CODESIZE_TO_PATCH, PAGE_EXECUTE_READWRITE, &oldProtect);
			memcpy((LPVOID)CODEADDR_TO_PATCH, CODE_OLD, CODESIZE_TO_PATCH);
			VirtualProtect((LPVOID)CODEADDR_TO_PATCH, CODESIZE_TO_PATCH, oldProtect, &oldProtect);

			isHookSet = FALSE;
		}

		Sleep(2000);
	}
}
