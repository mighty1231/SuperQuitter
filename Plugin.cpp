#include "Plugin.h"
#include <stdio.h>

#define PLUGIN_NAME "SuperQuitter"
static_assert (sizeof(PLUGIN_NAME) <= MPQDRAFT_MAX_PLUGIN_NAME, "plugin name too long");

LPSTR Plugin::pluginName = PLUGIN_NAME;
const DWORD Plugin::pluginID = 0x22222222;
Plugin *Plugin::instance = NULL;

BYTE code_4F58D9[10] = {0x68, 0xD2, 0, 0, 0, 0x68, 0x68, 0x22, 0x50, 0x00};

Plugin *Plugin::getInstance() {
	if (instance == NULL)
		instance = new Plugin();
	return instance;
}
BOOL WINAPI Plugin::Identify(LPDWORD lpdwPluginID) {
	*lpdwPluginID = pluginID;
	return TRUE;
}
BOOL WINAPI Plugin::GetPluginName(LPSTR lpszPluginName, DWORD nNameBufferLength) {
	if (lpszPluginName != NULL){
		strncpy(lpszPluginName, this->pluginName, nNameBufferLength);
		return TRUE;	
	}
	return FALSE;
}
BOOL WINAPI Plugin::CanPatchExecutable(LPCSTR lpszEXEFileName) {
	return (lpszEXEFileName != NULL && checkStarCraftExeVersion(lpszEXEFileName));
}
BOOL WINAPI Plugin::Configure(HWND hParentWnd) {
	MessageBox(
		hParentWnd,
		"SuperQuitter\n"
		"Protect from aggressive map\n"
		"Visit http://klassical_31.blog.me for more information",
		PLUGIN_NAME,
		MB_TASKMODAL
	);
	return TRUE;
}
BOOL WINAPI Plugin::ReadyForPatch() {
	return TRUE;
}
BOOL WINAPI Plugin::GetModules(MPQDRAFTPLUGINMODULE *lpPluginModules, LPDWORD lpnNumModules) {
	*lpnNumModules = 0;
	return TRUE;
}
BOOL WINAPI Plugin::InitializePlugin(IMPQDraftServer *lpMPQDraftServer) {
	DWORD oldProtect;
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
	*(char *)0x4F58D9 = 0xBA;
	*(int *)(0x4F58D9+1) = (int)hook;  // mov edx, hook
	*(short *)(0x4F58D9+5) = 0xE2FF;            // jmp edx
	*(char *)(0x4F58D9+7) = 0x90;               // nop, covering previous code
	VirtualProtect((void *)0x4F58D9, 10, oldProtect, &oldProtect);
	return TRUE;
}
BOOL WINAPI Plugin::TerminatePlugin() {
	DWORD oldProtect;
	// repair
	VirtualProtect((void *)0x4F58D9, 10, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void *)0x4F58D9, code_4F58D9, sizeof(code_4F58D9));
	VirtualProtect((void *)0x4F58D9, 10, oldProtect, &oldProtect);
	return TRUE;

}

//Checks the file version of StarCraft.exe.
//If the version is correct, returns TRUE. Otherwise, this displays appropriate
//error messages and returns FALSE.
BOOL Plugin::checkStarCraftExeVersion(LPCSTR exePath) const {
  BOOL result = FALSE;
  DWORD dummy;
  char errorMessage[500];

  const DWORD fileVersionInfoBufferSize = GetFileVersionInfoSize(exePath, &dummy);

  if (fileVersionInfoBufferSize > 0) {
    VOID *fileVersionInfoBuffer = new BYTE[fileVersionInfoBufferSize];
    VS_FIXEDFILEINFO *fileInfo;
    UINT fileInfoSize;

    if (GetFileVersionInfo(exePath, NULL, fileVersionInfoBufferSize, fileVersionInfoBuffer)
      && VerQueryValue(fileVersionInfoBuffer, "\\", (LPVOID*)&fileInfo, &fileInfoSize))
    {
      //The FileVersion of StarCraft.exe must be 1.16.1.1
      const WORD requiredVersion[4] = {1, 16, 1, 1};
      const WORD currentVersion[4] = {
        HIWORD(fileInfo->dwFileVersionMS), LOWORD(fileInfo->dwFileVersionMS),
        HIWORD(fileInfo->dwFileVersionLS), LOWORD(fileInfo->dwFileVersionLS)
      };

      if (requiredVersion[0] == currentVersion[0]
          && requiredVersion[1] == currentVersion[1]
          && requiredVersion[2] == currentVersion[2]
          && requiredVersion[3] == currentVersion[3])
      {
        result = TRUE;
      }
      else {
        sprintf(errorMessage,
          "Error: Cannot patch %s"
          "\n"
          "\nThis plugin (" PLUGIN_NAME ") is incompatible with the current version of StarCraft."
          " The game will still be loaded, but no plugin features will be available."
          "\n"
          "\nCurrent StarCraft version: %hu.%hu.%hu.%hu"
          "\nRequired StarCraft version: %hu.%hu.%hu.%hu",
          exePath,
          currentVersion[0],  currentVersion[1],  currentVersion[2],  currentVersion[3],
          requiredVersion[0], requiredVersion[1], requiredVersion[2], requiredVersion[3]);
      }
    }
    else {
      sprintf(errorMessage, "Cannot retrieve version information from:\n  %s", exePath);
    }
    delete [] fileVersionInfoBuffer;
  }
  else {
    sprintf(errorMessage, "Cannot retrieve version information size from:\n  %s", exePath);
  }

  if (!result)
    MessageBox(NULL, errorMessage, NULL, MB_OK);
  return result;
}