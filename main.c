#include "hook.h"
#include "sqlog.h"
#include "resources.h"
#include "hook.h"
#include <stdio.h>
#include <wchar.h>


BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpReserved) {
	HRSRC rc;
	HGLOBAL rcData;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			rc = FindResource(hInst, MAKEINTRESOURCE(IDC_QUIT2MNUBIN), RT_RCDATA);
			rcData = LoadResource(hInst, rc);
			binary_data = (char *) LockResource(rcData);
			binary_size = SizeofResource(hInst, rc);
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
