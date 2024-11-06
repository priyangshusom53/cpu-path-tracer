#include "FileImport.h"

FileImport::FileImport(LPCTSTR absPath)
	:buffer(nullptr)
{
	fileSize.QuadPart = 0;
	hIn = CreateFile(
		absPath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hIn == INVALID_HANDLE_VALUE) { ReportError(_T("can't open file "), 0, TRUE); }

}

CHAR* FileImport::readFile(BOOL isBinary)
{
	if (!GetFileSizeEx(hIn, &fileSize))
	{
		ReportError(_T("could not get file size"), 0, TRUE);
		CloseHandle(hIn);
		return nullptr;
	}
	DWORD bytesRead;
	if (!isBinary)
	{
		buffer = new CHAR[fileSize.QuadPart + 1];
		if (!(ReadFile(hIn, buffer, fileSize.QuadPart, &bytesRead, NULL)))
		{
			ReportError(_T("could not read file "), 0, TRUE);
			delete[] buffer;
			CloseHandle(hIn);
			return nullptr;
		}
		buffer[fileSize.QuadPart] = '\0';
		return buffer;
	}
	return nullptr;
}

VOID FileImport::ReportError(LPCTSTR userMessage, DWORD exitCode, BOOL printErrorMessage)
{
	DWORD eMsgLen, errNum = GetLastError();
	LPTSTR lpvSysMsg;

	_ftprintf(stderr, _T("%s \n"), userMessage);
	if (printErrorMessage)
	{
		eMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			errNum,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpvSysMsg,
			0,
			NULL
		);
		if (eMsgLen > 0)
		{
			_ftprintf(stderr, _T("%s"), lpvSysMsg);
		}
		else
		{
			_ftprintf(stderr, _T("Last Error Message; %d.\n"), errNum);
		}

		if (lpvSysMsg != NULL) LocalFree(lpvSysMsg);
	}
	if (exitCode > 0)
		ExitProcess(exitCode);
	return;
}

