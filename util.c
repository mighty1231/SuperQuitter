#include "util.h"
#include "sqlog.h"
#include <windows.h>
#include <wchar.h>

static size_t length = 0;
static wchar_t dllPath[MAX_PATH] = {0,};

size_t GetDllPath(wchar_t *dest) {
	if (length != 0) {
		wcscpy(dest, dllPath);
		return length;
	} else {
		HMODULE hModule;
		if (!GetModuleHandleExW(
				GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
				GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				(LPWSTR) &GetDllPath, &hModule) ||
			GetModuleFileNameW(hModule, dllPath, _countof(dllPath)) == 0) {
			return 0;
		}
		length = wcslen(dllPath);
		wcscpy(dest, dllPath);
		sqlog(L"Path of dll is %s", dllPath);
		return length;
	}
}

