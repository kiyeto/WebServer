#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>

#include "MIME.hpp"
#include "../Parsing Conf/configfile_src/ServerConfig.hpp"
#include "../request parsing and some basic response/request.hpp"
#include "../CGI/Uriparser.hpp"
#include "../CGI/Cgi_request.hpp"

class	Response {
	std::map<std::string, std::string> MIME_types;
	std::vector<ServerConfig>	&servers;

	int	select_server(request &req);
	std::string	make_response(Uriparser &pr, ServerConfig &server, bool	regular_file);
	public :
		Response(std::vector<ServerConfig> &servers);
		std::string	get_response(request &req);
};