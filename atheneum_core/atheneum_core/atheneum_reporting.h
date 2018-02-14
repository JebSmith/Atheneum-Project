#pragma once

#include <time.h>

#include <string>
#include <vector>
#include "atheneum_db.h"
#include "date_parsing.h"
#include "atheneum_file.h"

class atheneum_reporting
{
public:
	atheneum_reporting();
	~atheneum_reporting();
	static float calc_sum_due(bool admin, date_parsing now, date_parsing due);
	static int generate_report(atheneum_db db);

};

