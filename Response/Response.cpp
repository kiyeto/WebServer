#include "Response.hpp"


Response::Response(std::vector<ServerConfig> &servers) : MIME_types(get_MIME_types()), servers(servers) {
	std::map<std::string, std::string>::iterator it = MIME_types.begin();
	while (it != MIME_types.end())
	{
		std::cout << "[\"" << it->first << "\", \"" << it->second << "\"]" << std::endl;
		it++;
	}
	
}

std::string Response::get_response(request	&req) {
	return std::string();
}