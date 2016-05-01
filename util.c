#include "util.h"
#include "sqlog.h"
#include <windows.h>
#include <wchar.h>

static HMODULE hModule = NULL;
static wchar_t dllPath[MAX_PATH] = {0,};

/* Get handle and path of loaded executable (handle for module itself)
 * Returns TRUE if it successes, FALSE if not. */
BOOL GetDllInfo(HMODULE *_out_module, wchar_t *_out_dllpath) {
	if (hModule == NULL) {
		if (!GetModuleHandleExW(
				GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
				GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				(LPWSTR) &GetDllInfo, &hModule) ||
			GetModuleFileNameW(hModule, dllPath, _countof(dllPath)) == 0) {
			return FALSE;
		}
	}
	if (_out_module != NULL)
		*_out_module = hModule;
	if (_out_dllpath != NULL)
		wcscpy(_out_dllpath, dllPath);
	return TRUE;
}

