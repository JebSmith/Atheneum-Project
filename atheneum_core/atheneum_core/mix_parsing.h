#pragma once

#include <string>
#include <vector>

class mix_parsing
{
public:
	mix_parsing();
	~mix_parsing();

	
	static std::string parse_in_array_items(std::vector<std::vector<std::string>> data);
	static std::vector<std::vector<std::string>> parse_out_array_items(std::string data);
};

