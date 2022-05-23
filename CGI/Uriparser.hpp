#pragma once
#include <cstring>
#include <iostream>

class Uriparser {
	public:
		std::string path, script_name, path_info, query_string;
		Uriparser(std::string path);
};