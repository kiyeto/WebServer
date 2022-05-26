#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "MIME.hpp"
#include "../Parsing Conf/configfile_src/ServerConfig.hpp"
#include "../request parsing and some basic response/request.hpp"

class	Response {
	std::map<std::string, std::string> MIME_types;
	std::vector<ServerConfig>	&servers;

	public :
		Response(std::vector<ServerConfig> &servers);
		std::string	get_response(request &req);
};