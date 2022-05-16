#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdlib.h>

class Cgi_request {
	std::vector<std::string> meta_vars;

	void	meta_vars(std::sting method){
		meta_vars.push_back("REQUEST_METHOD=" + method);
	}
	public:
		void execute(std::string &Path);
};