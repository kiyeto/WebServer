#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdlib.h>

class Cgi_request {
	std::vector<std::string> meta_vars;
	public:
		void execute(std::string &Path);
};