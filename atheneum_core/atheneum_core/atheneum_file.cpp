#include "atheneum_file.h"



atheneum_file::atheneum_file()
{
}


atheneum_file::~atheneum_file()
{
}

bool atheneum_file::check_exists(const char * pPath)
{
	BOOL bPath = PathFileExistsA(pPath);
	if (bPath) {
		return true;
	}
	return false;
}

int atheneum_file::get_file_size(const char * pPath)
{
	if (!check_exists(pPath))
		return 0;
	HANDLE hFile = CreateFileA(pPath, FILE_ALL_ACCESS, 0, 0, OPEN_EXISTING, 0, 0);
	DWORD iSize = GetFileSize(hFile, 0);
	CloseHandle(hFile);
	return iSize;
}

int atheneum_file::write_all(const char*pPath,const char * pBuff)
{
	HANDLE hFile = CreateFileA(pPath,FILE_APPEND_DATA, 0, 0, OPEN_EXISTING,0, 0);
	DWORD dwWritten;
	if (!WriteFile(hFile, pBuff, strlen(pBuff), &dwWritten, 0)) {
		printf("\n\n****ERROR: %d\n\n", GetLastError());
		return 0;
	}
	CloseHandle(hFile);
	return dwWritten;
}

char * atheneum_file::read_all(const char * pPath)
{
	int iSize = get_file_size(pPath);
	char*pBuff = (char*)malloc(iSize+1);
	HANDLE hFile = CreateFileA(pPath, FILE_ALL_ACCESS, 0, 0, OPEN_EXISTING, 0, 0);
	DWORD read;
	ReadFile(hFile, pBuff, iSize, &read, 0);
	pBuff[read] = '\0';
	CloseHandle(hFile);
	return pBuff;
}

void atheneum_file::create_file(const char * pPath)
{
	HANDLE hFile = CreateFileA(pPath, FILE_ALL_ACCESS, 0, 0, CREATE_ALWAYS, 0, 0);
	CloseHandle(hFile);
}

void atheneum_file::clear_file(const char * pPath)
{
	DeleteFileA(pPath);
	create_file(pPath);
}


