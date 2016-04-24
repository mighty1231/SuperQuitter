#include "hook.h"
// #include <string.h>
#include "resources.h"
#include <windows.h>
#include <stdio.h>

char quitdlg[] = "rez\\quit2mnu.bin";
char *data;
size_t size;

char buf[1000];

/* load binary from folder in dll
 */
BOOL hook_init(HINSTANCE hInst) {
	HRSRC rc = FindResource(0, MAKEINTRESOURCE(IDC_QUIT2MNUBIN), RT_RCDATA);
	HGLOBAL rcData = LoadResource(0, rc);
	data = (char *) LockResource(rcData);
	size = SizeofResource(0, rc);


	freopen("C:\\Workspace\\dialogRestoration\\t.txt", "w", stdout);
	printf("res rc %d rcData %d size %d", rc, rcData, size);
	fflush(stdout);

	return FALSE;
}

__declspec(naked) void hook(void) {
	__asm {
		; start save registers
		push ebx
		push ecx
		push edi
		push esi
		; end save registers

		push edi ; filename
		mov eax, offset quitdlg
		push eax
		call strcmp
		test eax, eax
		jnz __default

	__quit:
		; pop edi
		; pop ebx

		; start load registers
		pop esi
		pop edi
		pop ecx
		pop ebx
		; end load registers

		mov edx, 0x4F58F9
		jmp edx

	__default:
		; start load registers
		pop esi
		pop edi
		pop ecx
		pop ebx
		; end load registers

		push 0xD2
		push 0x502268
		mov edx, 0x4F58E3
		jmp edx
	}
}