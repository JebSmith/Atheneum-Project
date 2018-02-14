#include "atheneum_db.h"


atheneum_db::atheneum_db(std::string sPath)
{
	if (!atheneum_file::check_exists(sPath.c_str()))
	{
		std::string back = sPath + ".bak";
		if (atheneum_file::check_exists(back.c_str()))
		{
			char*newdata = atheneum_file::read_all(back.c_str());
			atheneum_file::create_file(path.c_str());
			atheneum_file::write_all(sPath.c_str(), newdata);
			free(newdata);
		}
		else 
		{
			initialized = false;
		}
	}
	path = sPath;
}


atheneum_db::~atheneum_db()
{
}

bool atheneum_db::atheneum_initialize(std::vector<std::string> vHeaders)
{
	if (!vHeaders.size() > 0)
		return false;
	
	std::string sTmp = "[H:";
	for (std::string &header: vHeaders)
	{
		sTmp += header+',';
	}
	sTmp += "]\n";
		
	atheneum_file::create_file(path.c_str());
	
	atheneum_file::write_all(path.c_str(), sTmp.c_str());
	return true;
}

bool atheneum_db::atheneum_add_row(std::vector<std::string> vEntries)
{
	if (!vEntries.size() > 0)
		return false;
	


	std::string sTmp = "[E";
	sTmp += std::to_string(atheneum_get_current_row_id()) + ":";


	for (std::string &entry: vEntries)
	{
		//printf("==%s\n", entry.c_str());
		sTmp += entry + ",";
	}
	sTmp += "]\n";

	atheneum_file::write_all(path.c_str(), sTmp.c_str());

	return true;
}

int atheneum_db::atheneum_get_row_count()
{
	int i = 0;
	char*pTmp = atheneum_file::read_all(path.c_str());
	for (int c = 0; c < strlen(pTmp); c++)
	{
		if (pTmp[c] == '\n')
			i++;
	}
	return i-1;
}

int atheneum_db::atheneum_get_current_row_id()
{
	return atheneum_get_row_count()+1;
}

std::vector<std::string> atheneum_db::atheneum_get_row(int iId)
{
	
	std::vector<std::string> vRowdata;

	char*pBuff = atheneum_file::read_all(path.c_str());
	
	std::string sTmp = "";

	for (int s = 0; s < strlen(pBuff);s++)
	{

		if (pBuff[s] == '[')
		{
			if (pBuff[s+1] == 'E')
			{
				s+=2;
				while (s < strlen(pBuff) && pBuff[s] != ':')
				{
					sTmp += pBuff[s];
					s++;
				}
				//printf("%d == %d?\n", atoi(sTmp.c_str()), iId);
				if (atoi(sTmp.c_str()) == iId)
				{
					s++;
					sTmp.clear();
					while (pBuff[s] != ']')
					{
						if (pBuff[s] == '{')
						{
							while (pBuff[s] && pBuff[s] != '}')
							{
								sTmp += pBuff[s];
								s++;
							}
							sTmp += pBuff[s];
						}
						else if (pBuff[s] != ',')
							sTmp += pBuff[s];
						else 
						{
							vRowdata.push_back(sTmp);
							sTmp.clear();
						}
						s++;
					}
					break;
				}
				sTmp.clear();
				
			}
		}

	}

	free(pBuff);

	return vRowdata;
}

bool atheneum_db::atheneum_update_row(int iId, std::vector<std::string> vRowdata)
{
	char*pBuff = atheneum_file::read_all(path.c_str());

	std::string oBuff = "";
	std::string sTmp = "";

	for (int s = 0; s < strlen(pBuff); s++)
	{
		oBuff += pBuff[s];

		if (pBuff[s] == '[')
		{
			if (pBuff[s + 1] == 'E')
			{
				oBuff += pBuff[s+1];
				s += 2;

				while (s < strlen(pBuff) && pBuff[s] != ':')
				{
					oBuff += pBuff[s];
					sTmp += pBuff[s];
					s++;
				}
				oBuff += pBuff[s];				
				if (atoi(sTmp.c_str()) == iId)
				{
					//s++;
					sTmp.clear();
					for (std::string &nw: vRowdata)
					{
						oBuff += nw + ',';
						//s += nw.length();
					}
					//printf("%s\n", oBuff.c_str());
					
					
					while (pBuff[s] != ']')
					{
						s++;
					}

					oBuff += pBuff[s];
					s++;
					//oBuff += '\n';
					while (s < strlen(pBuff)) {
						//printf("--%c--\n", pBuff[s]);
						oBuff += pBuff[s];
						s++;
					}
				}
				sTmp.clear();
			}
		}

	}
	atheneum_file::clear_file(path.c_str());
	atheneum_file::write_all(path.c_str(), oBuff.c_str());

	//printf("%s\n\n\n", oBuff.c_str());

	free(pBuff);

	return true;
}

std::vector<std::string> atheneum_db::atheneum_get_header()
{
	char*pBuff = atheneum_file::read_all(path.c_str());

	std::vector<std::string> vHeaders;

	std::string sTmp = "";

	for (int i = 0; i < strlen(pBuff); i++)
	{
		if (pBuff[i] == '[')
		{
			if (pBuff[++i] == 'H')
			{
				if (pBuff[++i] == ':')
				{
					for (++i; i < strlen(pBuff) && pBuff[i] != '\n'; i++)
					{
						if (pBuff[i] != ',')
							sTmp += pBuff[i];
						else
						{
							vHeaders.push_back(sTmp);
							sTmp.clear();
						}
					}
					break;
				}
			}
		}
	}
	return vHeaders;
}

