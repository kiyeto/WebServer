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
	private:
		std::string path, query_str, path_info, root;
		std::string child_proce(const char **cmd);
	public:
		Cgi_request(): path(), query_str(), path_info(), root(getenv("PWD")){}
		std::string execute(std::string Path);
};