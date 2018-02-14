#include "mix_parsing.h"



mix_parsing::mix_parsing()
{
}


mix_parsing::~mix_parsing()
{
}


std::string mix_parsing::parse_in_array_items(std::vector<std::vector<std::string>> data)
{
	std::string sTmp = "{";
	for (std::vector<std::string> vec: data)
	{
		for (std::string str: vec)
		{
			sTmp += str + ',';
		}
		sTmp += ';';
	}
	sTmp += '}';
	return sTmp;
}

std::vector<std::vector<std::string>> mix_parsing::parse_out_array_items(std::string data)
{
	std::vector<std::string> vArray;

	std::vector<std::vector<std::string>> vReturn;

	std::string sTmp = "";

	for (int i = 0; i < data.length(); i++)
	{
		if (data[i] == '{')
		{
			
			i++;
			while (data[i] && data[i] != '}')
			{
				if (data[i] == ';')
				{
					vReturn.push_back(vArray);
					vArray.clear();
				}
				else if (data[i] != ',')
				{
					sTmp += data[i];
				}
				else 
				{
					vArray.push_back(sTmp);
					sTmp.clear();
				}
				i++;
			}
		}
	}
	return vReturn;
}


