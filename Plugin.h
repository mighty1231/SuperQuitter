#include "MPQDraftPlugin.h"
#include "hook.h"

class Plugin: public IMPQDraftPlugin {
private:
	static LPSTR pluginName;
	static const DWORD pluginID;
	static Plugin *instance;

public:
	static Plugin *getInstance();
	BOOL WINAPI Identify(LPDWORD lpdwPluginID);
	BOOL WINAPI GetPluginName(LPSTR lpszPluginName, DWORD nNameBufferLength);
	BOOL WINAPI CanPatchExecutable(LPCSTR lpszEXEFileName);
	BOOL WINAPI Configure(HWND hParentWnd);
	BOOL WINAPI ReadyForPatch();
	BOOL WINAPI GetModules(MPQDRAFTPLUGINMODULE *lpPluginModules, LPDWORD lpnNumModules);
	BOOL WINAPI InitializePlugin(IMPQDraftServer *lpMPQDraftServer);
	BOOL WINAPI TerminatePlugin();

	BOOL checkStarCraftExeVersion(LPCSTR exePath) const;
};