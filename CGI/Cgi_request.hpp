#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdlib.h>

class Cgi_request {
	
	public:
		void execute(std::string &Path);
};