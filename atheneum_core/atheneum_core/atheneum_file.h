#pragma once

#include <string>
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")


class atheneum_file
{
public:
	atheneum_file();
	~atheneum_file();

	static bool check_exists(const char*pPath);
	static int get_file_size(const char*pPath);
	static int write_all(const char*pPath,const char*pBuff);
	static char*read_all(const char*pPath);
	static void create_file(const char*pPath);
	static void clear_file(const char*pPath);
};

