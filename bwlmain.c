/* http://wiki.teamliquid.net/starcraft/BWL4_Plugin_Format_Specification */

#include "resources.h"
#include "sqlog.h"
#include "util.h"
#include "hook.h"
#include "main.h"
#include <windows.h>
#include <string.h>

// Sample BWL4 Plugin
// BOOL is a 32Bit Value
// All BWL4 functions are C-CallingConvention(cdecl) not stdcall

#define BWLAPI 4
#define STARCRAFTBUILD 13

/*  STARCRAFTBUILD
   -1   All
    0   1.04
    1   1.08b
    2   1.09b
    3   1.10
    4   1.11b
    5   1.12b
    6   1.13f
    7   1.14
    8   1.15
    9    1.15.1
    10    1.15.2
    11    1.15.3
    12    1.16.0
    13    1.16.1
*/

struct ExchangeData
{
	int iPluginAPI;
	int iStarCraftBuild;
	BOOL bNotSCBWmodule;                //Inform user that closing BWL will shut down your plugin
	BOOL bConfigDialog;                 //Is Configurable
};

//
// GetPluginAPI and GetData get called during the startup of the Launcher and gather information about the plugin
//
extern __declspec(dllexport) void GetPluginAPI(struct ExchangeData *Data)
{
	// BWL Gets version from Resource - VersionInfo
	Data->iPluginAPI = BWLAPI;
	Data->iStarCraftBuild = STARCRAFTBUILD;
	Data->bConfigDialog = FALSE;
	Data->bNotSCBWmodule = TRUE;
}

extern __declspec(dllexport) void GetData(char *name, char *description, char *updateurl)
{
	// if necessary you can add Initialize function here
	// possibly check CurrentCulture (CultureInfo) to localize your DLL due to system settings
	strcpy(name, "SuperQuitter");
	strcpy(description,
		"SuperQuitter for 1.16.1 Version "_VERSION"\r\n"
		"Enables quit game for any condition with just press ctrl+q+q\r\n"
		"https://github.com/mighty1231/SuperQuitter\r\n\r\n"
		"by mighty1231");
	strcpy(updateurl,   "https://github.com/mighty1231/SuperQuitter");
}

//
// Called when the user clicks on Config
//
extern __declspec(dllexport) BOOL OpenConfig(void)
{
	// If you set "Data.bConfigDialog = TRUE;" at function GetPluginAPI then
	// BWLauncher will call this function if user clicks Config button

	// You'll need to make your own Window here
	return TRUE; //everything OK
}

//
// ApplyPatchSuspended and ApplyPatch get
// called during the startup of Starcraft in the Launcher process
// the hProcess passed to them is shared between all plugins, so don't close it.
// Best practice is duplicating(DuplicateHandle from Win32 API) it if you want to use if after these function returns

extern __declspec(dllexport) BOOL ApplyPatchSuspended(HANDLE hProcess, DWORD dwProcessID)
{
	// This function is called in the Launcher process while Starcraft is still suspended
	// Durning the suspended process some modules of starcraft.exe may not yet exist.

	// This function is called in the Launcher process after the Starcraft window has been created
	return TRUE; //everything OK
}

// @TODO CloseHandle would be called more precisely.
#define FAIL_IF(stmt, msg) {if (stmt) {                               \
	sqlog(L"런처 패치중 에러: " msg L", 에러코드 %d", GetLastError());  \
	return FALSE;}}

#define FAIL_IF2(stmt, msg) {if (stmt) {                              \
	sqlog(L"런처 패치중 에러: " msg L", 에러코드 %d", GetLastError());  \
	CloseHandle(hNewProcess);                                         \
	CloseHandle(hThread);                                             \
	return FALSE;}}

#define FAIL_IF3(stmt, msg) {if (stmt) {                              \
	sqlog(L"런처 패치중 에러: " msg L", 에러코드 %d", GetLastError());  \
	VirtualFreeEx(hNewProcess, (LPVOID)pBuf, 0, MEM_RELEASE);         \
	CloseHandle(hNewProcess);                                         \
	CloseHandle(hThread);                                             \
	return FALSE;}}

extern __declspec(dllexport) BOOL ApplyPatch(HANDLE hProcess, DWORD dwProcessID)
{
	void *tokenHandle;
	TOKEN_PRIVILEGES privilegeToken;

	HMODULE hDllModule;
	wchar_t dllPath[MAX_PATH];

	FAIL_IF(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &tokenHandle) == 0, L"OpenProcessToken()");
	FAIL_IF(LookupPrivilegeValue(0, SE_DEBUG_NAME, &privilegeToken.Privileges[0].Luid) == 0,
		L"LookupPrivilegeValue()");
	privilegeToken.PrivilegeCount = 1;
	privilegeToken.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	FAIL_IF(AdjustTokenPrivileges(tokenHandle, 0, &privilegeToken, sizeof(TOKEN_PRIVILEGES), 0, 0) == 0,
		L"AdjustTokenPrivileges()");
	CloseHandle(tokenHandle);

	FAIL_IF(GetDllInfo(&hDllModule, dllPath) == FALSE, L"dll 탐색 실패");

	/* Injection */
	HANDLE hNewProcess = 0, hThread = 0, hExDllModule = 0;
	LPVOID pLoadLibrary, pBuf;
	FAIL_IF((hNewProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwProcessID)) == NULL, L"OpenProcess()");
	FAIL_IF2((pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryW")) == NULL,
		L"GetProcAddress()");
	FAIL_IF2((pBuf = (LPVOID)VirtualAllocEx(hNewProcess, NULL, sizeof(dllPath), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE)) == NULL,
		L"VirtualAllocEx()");
	FAIL_IF3(WriteProcessMemory(hNewProcess, (LPVOID)pBuf, dllPath, sizeof(dllPath), NULL) == 0,
		L"WriteProcessMemory()");
	FAIL_IF3((hThread = CreateRemoteThread(hNewProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)pLoadLibrary, (LPVOID)pBuf, 0, NULL)) == NULL,
		L"CreateRemoteThread(LoadLibraryW)");

	/* Waiting for injection */
	WaitForSingleObject(hThread, INFINITE);
	GetExitCodeThread(hThread, (DWORD *)&hExDllModule); // hExDllModule becomes returned value from LoadLibraryW
	VirtualFreeEx(hNewProcess, (LPVOID)pBuf, 0, MEM_RELEASE);

	/* Create thread for Starcraft */
	HANDLE hMainThread;
	FAIL_IF2(hExDllModule == 0, L"LoadLibraryW()");
	FAIL_IF2((hMainThread = CreateRemoteThread(hNewProcess, NULL, 0,
			(LPTHREAD_START_ROUTINE)((DWORD)hExDllModule + ((DWORD)&MainLoop - (DWORD)hDllModule)),
			(LPVOID)hExDllModule, 0, NULL)) == NULL,
		L"CreateRemoteThread(MainLoop)");

	CloseHandle(hThread);
	CloseHandle(hNewProcess);

	return TRUE; //everything OK
}
