#include "atheneum_reporting.h"



atheneum_reporting::atheneum_reporting()
{
}


atheneum_reporting::~atheneum_reporting()
{
}

float atheneum_reporting::calc_sum_due(bool admin, date_parsing now, date_parsing due)
{

	float sum = 0.0;
	
	int difference = date_parsing::calc_difference(now.month, now.day, now.year, due.month, due.day, due.year);

	if (admin)
		sum = (float)difference * 0.5;
	else
		sum = (float)difference * 2.5;
	return sum;
}

int atheneum_reporting::generate_report(atheneum_db db)
{	
	std::string sTmp = "";

	time_t now = time(0);

	tm *ltm = localtime(&now);

	srand(time(0));

	std::string name = "report-" + std::to_string(ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(rand());

	atheneum_file::create_file(name.c_str());

	for (int i = 0; i <= db.atheneum_get_row_count(); i++)
	{
		std::vector<std::string> rows = db.atheneum_get_row(i);

		sTmp = rows[0] + " " + rows[1] + " Status: " + rows[2] + "\n";

		printf("%s\n", sTmp.c_str());
		atheneum_file::write_all(name.c_str(), sTmp.c_str());
		sTmp.clear();
	}

	return 0;
}
