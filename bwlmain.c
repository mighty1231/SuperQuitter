/* http://wiki.teamliquid.net/starcraft/BWL4_Plugin_Format_Specification */

#include "sqlog.h"
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

// BOOL APIENTRY DllMain( HMODULE hModule,
//                        DWORD  ul_reason_for_call,
//                        LPVOID lpReserved
//                 )
// {
// 	//Is this DLL also StarCraft module?
	
// 	switch (ul_reason_for_call)
// 	{
// 		case DLL_PROCESS_ATTACH:
// 			MessageBox(NULL, "StarCraft messagebox", "Hue", MB_OK);
// 			return TRUE;
// 		case DLL_THREAD_ATTACH:
// 		case DLL_THREAD_DETACH:
// 		case DLL_PROCESS_DETACH:
// 			break;
// 	}
	
// 	return TRUE;
// }


HANDLE CreateRemoteThreadInject(DWORD ID, const wchar_t* dll);

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
	strcpy(name,      "SuperQuitter");
	strcpy(description,
		"SuperQuitter for 1.16.1 Version 1.0.0.1\r\n"
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

extern __declspec(dllexport) BOOL ApplyPatch(HANDLE hProcess, DWORD dwProcessID)
{
	void *tokenHandle;
	TOKEN_PRIVILEGES privilegeToken;
	wchar_t dll[MAX_PATH];
	HANDLE hThread;
	DWORD exitCode;

	OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &tokenHandle );
	LookupPrivilegeValue( 0, SE_DEBUG_NAME, &privilegeToken.Privileges[0].Luid );
	privilegeToken.PrivilegeCount = 1;
	privilegeToken.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges( tokenHandle, 0, &privilegeToken, sizeof( TOKEN_PRIVILEGES ), 0, 0 );
	if( !CloseHandle( tokenHandle ) ) {
		sqlog(L"Launcher: token failed");
		return FALSE;
	}

	GetFullPathNameW(L"SuperQuitter.bwl", MAX_PATH, dll, NULL);
	hThread = CreateRemoteThreadInject(dwProcessID, dll);
	if ( hThread != 0 ) {
		sqlog(L"Launcher: CreateRemoteThread success");
		WaitForSingleObject( hThread, INFINITE );
		GetExitCodeThread( hThread, &exitCode );
		CloseHandle( hThread );
	} else {
		sqlog(L"Launcher: CreateRemoteThread failed");
		return FALSE;
	}
	if (exitCode == NULL) {
		sqlog(L"Launcher: LoadLibrary Failed");
		return FALSE;
	}

	return TRUE; //everything OK
}

HANDLE CreateRemoteThreadInject(DWORD ID, const wchar_t* dll)
{
	HANDLE Process, Thread;
	LPVOID Memory;
	LPVOID LoadLibrary;
	wchar_t msgtemp[255];

	if (ID == 0)
	{
		return FALSE;
	}

	Process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, ID);
	if (Process == NULL)
	{
		swprintf_s(msgtemp, sizeof(msgtemp), L"OpenProcess returned %d", GetLastError());
		return FALSE;
	}

	LoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	if (LoadLibrary == NULL)
	{
		swprintf_s(msgtemp, sizeof(msgtemp), L"GetProcAddress returned %d", GetLastError());
		CloseHandle(Process);
		return FALSE;
	}

	char mbs[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, dll, -1, mbs, sizeof(mbs), NULL, NULL);

	Memory = (LPVOID)VirtualAllocEx(Process, NULL, strlen(mbs) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (Memory == NULL)
	{
		swprintf_s(msgtemp, sizeof(msgtemp), L"VirtualAllocEx returned %d", GetLastError());
		CloseHandle(Process);
		return FALSE;
	}

	WriteProcessMemory(Process, (LPVOID)Memory, mbs, strlen(mbs) + 1, NULL);

	Thread = CreateRemoteThread(Process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibrary, (LPVOID)Memory, NULL, NULL);

	CloseHandle(Process);

	VirtualFreeEx(Process, (LPVOID)Memory, 0, MEM_RELEASE);

	return Thread;
}