#include "hook.h"
#include <windows.h>

/*
 * CODEADDR_TO_PATCH 0x4F58D9
 * CODESIZE_TO_PATCH 10
 *
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
char *binary_data = NULL;
size_t binary_size = 0;

__declspec(naked) void hook(void) {
	__asm {
		; start save registers
		push ebx
		push ecx
		push edi
		push esi
		; end save registers

		push edi                       ; filename
		push 0x0050295C                ; constant string of rez\\quit2mnu.bin
		mov edx, 0x00408E00
		call edx                       ; _strcmp
		add esp, 8

		test eax, eax
		jnz __default

	__quit:
		push ebx                       ; defaultValue
		push 0x000000D2                ; logline
		push 0x00502268                ; logfilename
		mov eax, offset binary_size
		mov eax, [eax]
		push eax                       ; size
		mov edx, 0x0041006A
		call edx                       ; SMemAlloc

		mov ebx, eax
		mov eax, offset binary_size
		mov eax, [eax]
		push eax                       ; size
		mov eax, offset binary_data
		mov eax, [eax]
		push eax                       ; src = original rez\\quit2mnu.bin
		push ebx                       ; dest = result of SMemAlloc
		mov edx, 0x00406BF0
		call edx                       ; _memcpy

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