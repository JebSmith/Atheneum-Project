#include "date_parsing.h"



date_parsing::date_parsing(std::string raw)
{
	std::string sTmp = "";
	
	int c = 0;

	for (int i = 0; i < raw.length(); i++)
	{
		if (raw[i] != '/' && raw[i] != '-')
			sTmp += raw[i];
		else {
			if (c == 0)
				month = atoi(sTmp.c_str()),sTmp.clear(),c++;
			else
				day = atoi(sTmp.c_str()),  sTmp.clear();
		}
	}
	year = atoi(sTmp.c_str()), sTmp.clear();
}


date_parsing::~date_parsing()
{
}

struct Date
{
	int d, m, y;
};

const int monthDays[12] = { 31, 28, 31, 30, 31, 30,
31, 31, 30, 31, 30, 31 };

int countLeapYears(Date d)
{
	int years = d.y;

	if (d.m <= 2)
		years--;

	return years / 4 - years / 100 + years / 400;
}

int getDifference(Date dt1, Date dt2)
{
	long int n1 = dt1.y * 365 + dt1.d;

	for (int i = 0; i<dt1.m - 1; i++)
		n1 += monthDays[i];

	n1 += countLeapYears(dt1);

	long int n2 = dt2.y * 365 + dt2.d;
	for (int i = 0; i<dt2.m - 1; i++)
		n2 += monthDays[i];
	n2 += countLeapYears(dt2);

	return (n2 - n1);
}

int date_parsing::calc_difference(int month1, int day1, int year1, int month2, int day2, int year2)
{

	Date dt1 = { day1, month1, year1 };
	Date dt2 = { day2, month2, year2 };

	int days = getDifference(dt2, dt1);

	return days;
}

std::string date_parsing::now()
{
	time_t t;

	time(&t);

	tm*timenow;

	timenow = localtime(&t);

	return std::string(std::to_string(timenow->tm_mon+1)+"/"+std::to_string(timenow->tm_mday)+"/"+std::to_string(timenow->tm_year-100));
}

std::string date_parsing::then(int days)
{

	static int day_num[12] = { 31, 28, 31, 30, 31, 31,30, 31, 30, 31, 30, 31 };

	date_parsing dp(date_parsing::now());
	
	int forward_day = dp.day;
	int forward_mon = dp.month;
	int forward_year = dp.year;

	int mons = 0;
	int years = 0;

	if (forward_day + days > day_num[forward_mon - 1])
	{
		while (days > 0)
		{
			if (forward_day == day_num[forward_mon - 1])
			{

				forward_mon++;
				forward_day = 1;

				mons++;

				if (forward_mon == 13)
				{
					forward_year++;
					forward_mon = 1;

					years++;
				}
			}
			else
			{
				days--;
				forward_day++;
			}
			
		}
	}
	else {
		forward_day += days;
	}

	return std::string(std::to_string(forward_mon/*-years*/)+"/"+std::to_string(forward_day-mons)+"/"+std::to_string(forward_year));
}
