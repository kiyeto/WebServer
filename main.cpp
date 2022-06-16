#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include "Request/request.hpp"

/* Includes Added by Brahim*/
#include "Parsing Conf/configfile_src/ServerConfig.hpp"
#include "Parsing Conf/configfile_src/LocationConfig.hpp"
#include "Parsing Conf/configfile_src/ConfigfileClass.hpp"
#include "Response/Response.hpp"
#include "Server/Server.hpp"


int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		std::cout << "Config file Needed!" << std::endl;
		return 1;
	}

	std::string arg(argv[1]);
	ConfigfileClass config(arg);
	try {
		config.configfileparser();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	std::vector<ServerConfig> servers = config.getServerConfig();
	Server server(servers);
	server.run();
	return 0;
}
