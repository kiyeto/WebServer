#pragma Uriparser
#include <cstring>
#include <iostream>

class Uriparser {
	std::string path, script_name, path_info, query_string;
	public:
		Uriparser(std::string path);
};