#pragma once

#include <string>
#include <time.h>
class date_parsing
{
public:
	date_parsing(std::string raw);
	~date_parsing();
	int month;
	int day;
	int year;

	static int calc_difference(int month1, int day1, int year1, int month2, int day2, int year2);

	static std::string now();
	static std::string then(int days);

};

