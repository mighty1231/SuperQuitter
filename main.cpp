#include "plugin.h"
#include "hook.h"

BOOL WINAPI GetMPQDraftPlugin(IMPQDraftPlugin **lppMPQDraftPlugin) {
	*lppMPQDraftPlugin = Plugin::getInstance();
	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			break;

		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			if (hook_init(hModule) == FALSE)
				return FALSE;
			break;

		case DLL_THREAD_DETACH:
			break;
	}

	return TRUE;
}