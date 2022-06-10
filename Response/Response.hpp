#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <dirent.h>

#include "MIME.hpp"
#include "../Parsing Conf/configfile_src/ServerConfig.hpp"
#include "../V2/request.hpp"
#include "../CGI/Uriparser.hpp"
#include "../CGI/Cgi_request.hpp"
#include "../AutoIndex/autoindex.hpp"

class	Response {
	std::map<std::string, std::string> MIME_types;
	std::map<int, std::string> status_defin;
	std::vector<ServerConfig>	servers;
	std::map<std::string, std::string> headers;

	int	select_server(request &req);

	std::string	MIME_response(request &req, int i, std::map<std::string, std::string>::iterator &it);
	std::string	CGI_response(request &req, int i);
	std::string	Dir_response(request &req, int i);

	std::string		make_response(int status, std::vector<std::string> &headers, std::string &body);
	std::string		make_error_response(int status, ServerConfig &server);
	std::string		status_code(int status);
	std::pair<int, std::string>			find_location(std::string name, int i);
	bool			is_allowed(std::string method, LocationConfig location);
	std::string		make_redirection(int status, std::string to);
	int				delete_dir(std::string dir, DIR *d, std::string star);

	public :
		Response();
		Response(std::vector<ServerConfig> &servers);
		Response(const Response& response);

		Response &operator=(const Response& response);
		std::string	get_response(request &req);
};