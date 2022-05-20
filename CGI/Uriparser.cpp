#include "Uriparser.hpp"

Uriparser::Uriparser(std::string p) : path(p), script_name(), path_info(), query_string() {
	size_t i = path.find('?');
	if (i != std::string::npos)
		query_string = path.begin().base() + i + 1;
	
	std::cout << query_string << std::endl;	
}