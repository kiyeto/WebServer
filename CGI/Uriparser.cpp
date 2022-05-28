#include "Uriparser.hpp"

Uriparser::Uriparser(std::string p) : path(), extens(), path_info(), query_string() {
	size_t i = p.find('?');
	if (i != std::string::npos)
	{
		query_string = p.begin().base() + i + 1;
		p.erase(p.begin() + i, p.end());
	}
	path = p;
	i = path.find(".");
	if (i != std::string::npos){
		extens = path.begin().base() + i;
	}
}