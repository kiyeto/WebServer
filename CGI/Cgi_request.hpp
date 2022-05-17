#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>


/*
	REDIRECT_STATUS=200
	REQUEST_METHOD=GET
	QUERY_STRING=
	SERVER_PORT=8080
	SCRIPT_NAME=/usr/bin/php-cgi
	SERVER_NAME=127.0.0.1
	CONTENT_TYPE=text/html
*/


class Cgi_request {
	std::vector<std::string> meta_vars;

	void	meta(std::string method, int redirect_status){
		std::stringstream conv(redirect_status);
		std::string redirect;
		conv >> redirect;
		meta_vars.push_back("REQUEST_METHOD=" + method);
		meta_vars.push_back("REDIRECT_STATUS=" + redirect);
	}
	public:
		void execute(std::string &Path);
};