#include "hook.h"
// #include <string.h>
#include "resources.h"
#include <windows.h>
#include <stdio.h>

char quitdlg[] = "rez\\quit2mnu.bin";
char *datasrc;
size_t datasize;
const int datasize_fixed = 705;
 
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
BYTE CODE_OLD[CODESIZE_TO_PATCH] = {0x68, 0xD2, 0, 0, 0, 0x68, 0x68, 0x22, 0x50, 0x00};
BYTE CODE_NEW[CODESIZE_TO_PATCH] = {
	0xBA, 0, 0, 0, 0,   // mov edx, hook
	0xFF, 0xE2,         // jmp edx
	0x90, 0x90, 0x90    // nop nop nop
};


/* load dialog binary from folder in dll */
BOOL hook_init(HINSTANCE hInst) {
	LPVOID lpMsgBuf;
	DWORD dw;
	HRSRC rc = FindResource(hInst, MAKEINTRESOURCE(IDC_QUIT2MNUBIN), RT_RCDATA);
	// if (rc == NULL) {
	// 	dw = GetLastError(); 

	// 	FormatMessage(
	// 		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
	// 		FORMAT_MESSAGE_FROM_SYSTEM |
	// 		FORMAT_MESSAGE_IGNORE_INSERTS,
	// 		NULL,
	// 		dw,
	// 		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	// 		(LPTSTR) &lpMsgBuf,
	// 		0, NULL );
	// 	MessageBox(0, (LPTSTR)lpMsgBuf,(LPTSTR) lpMsgBuf, MB_OK);
	// }
	HGLOBAL rcData = LoadResource(hInst, rc);
	datasrc = (char *) LockResource(rcData);
	datasize = SizeofResource(hInst, rc);
	if (datasize != datasize_fixed)
		return FALSE;

	*(int *)(CODE_NEW+1) = (int) hook;
	return TRUE;
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
		add esp, 8

		test eax, eax
		jnz __default

	__quit:
		; pop edi
		; pop ebx

		push ebx                  ; defaultValue
		push 0x000000D2           ; logline
		push 0x00502268           ; logfilename
		push 0x000002C1           ; 705
		mov edx, 0x0041006A
		call edx                  ; SMemAlloc

		mov ebx, eax
		push 0x000002C1           ; 705
		mov eax, offset datasrc
		mov eax, [eax]
		push eax                  ; src
		push ebx                  ; dest
		call memcpy

		; start load registers
		pop esi
		pop edi
		pop ecx
		pop ebx
		; end load registers

		mov edx, 0x004F58F9
		jmp edx

	__default:
		; start load registers
		pop esi
		pop edi
		pop ecx
		pop ebx
		; end load registers

		push 0x000000D2
		push 0x00502268
		mov edx, 0x004F58E3
		jmp edx
	}
}