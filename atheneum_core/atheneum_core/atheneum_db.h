#pragma once

#include <string>  //string
#include <vector>  //vector

#include <Windows.h>
#include <stdio.h>

#include "atheneum_file.h"

class atheneum_db
{
public:
	atheneum_db(std::string sPath);
	~atheneum_db();
	
	bool atheneum_initialize(std::vector<std::string> vHeaders);

	bool atheneum_add_row(std::vector<std::string> vEntries);

	int atheneum_get_row_count();

	int atheneum_get_current_row_id();

	std::vector<std::string> atheneum_get_row(int iId);

	bool atheneum_update_row(int iId, std::vector<std::string> vHeaders);

	std::vector<std::string> atheneum_get_header();

	bool initialized;

	std::string path;

};

