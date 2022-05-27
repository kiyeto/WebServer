#pragma once
#include <cstring>
#include <iostream>

struct Uriparser {
	std::string path, extens, path_info, query_string;
	Uriparser(std::string path);
};