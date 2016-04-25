#include "sqlog.h"
#include <windows.h>
#include <locale.h>
#include <wchar.h>
#include <stdio.h>

wchar_t logfname[MAX_PATH];
wchar_t logbuf[1024];

int sqlog(wchar_t *format, ...){
	/* Set locale */
	setlocale(LC_ALL, "");

	/* Get time */
	SYSTEMTIME st;
	GetLocalTime(&st);

	/* Get log file */
	HMODULE hModule;
	wchar_t *pwc;
	pwc = wcsrchr(logfname, L'\\');
	if (pwc == NULL) {
		pwc = wcsrchr(logfname, L'/');
	}
	wcscpy(pwc+1, L"SuperQuitter_log.txt");

	FILE *pFile;
	size_t file_size; WORD first;
	{
		pFile = _wfopen(logfname, L"r+");
		if (pFile != 0) {
			fseek(pFile, 0, SEEK_SET);
			fread(&first, sizeof(WORD), 1, pFile);
			fseek(pFile, 0, SEEK_END);
			file_size = ftell(pFile);
			fclose(pFile);
		} else {
			file_size = 0;
		}
	}
	{
		if (first != 0xFEFF || file_size <= 2) {
			pFile = _wfopen(logfname, L"wb");
			WORD mark = 0xFEFF;
			fwrite(&mark, sizeof(WORD), 1, pFile);
		} else {
			pFile = _wfopen(logfname, L"ab");
		}

		/* Time */
		fwprintf(pFile, L"[%4d.%02d.%02d %02d:%02d:%02d] ",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		/* Log */
		va_list args;
		va_start(args, format);
		vfwprintf(pFile, format, args);
		va_end(args);

		/* End of line */
		fwprintf(pFile, L"\r\n");
		fclose(pFile);
	}
	return 0;
}
