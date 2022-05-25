#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "../request parsing and some basic response/request.hpp"
#include "Uriparser.hpp"

#include "../Parsing Conf/configfile_src/ConfigfileClass.hpp"
#include "../Parsing Conf/configfile_src/LocationConfig.hpp"
#include "../Parsing Conf/configfile_src/ServerConfig.hpp"


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
		std::string path, query_str, path_info, body;
		std::map<std::string, std::string> headers, meta;
		Uriparser pr;
		ServerConfig &server;

		std::string child_proce(const char **cmd, const char **envp);
		void	parse_cgiResponse(std::string respo);
	public:
		Cgi_request(request &req, ServerConfig &server);
		std::string execute();
};