#pragma once

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include "Engine/Base.h"

class FileImport
{
public:
	FileImport(LPCTSTR absolutePath);
	VOID ReportError(LPCTSTR userMessage, DWORD exitCode, BOOL printErrorMessage);
	CHAR* readFile(BOOL isBinary);
	inline LONGLONG getFileSize() { return fileSize.QuadPart; }
private:
	HANDLE hIn;
	LARGE_INTEGER fileSize;
	CHAR* buffer;
};

