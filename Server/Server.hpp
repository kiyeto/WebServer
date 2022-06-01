#pragma once

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

#include "../Parsing Conf/configfile_src/ServerConfig.hpp"
#include "../Parsing Conf/configfile_src/LocationConfig.hpp"
#include "../Parsing Conf/configfile_src/ConfigfileClass.hpp"
#include "../Response/Response.hpp"

class Server {
	std::vector<ServerConfig>	&servers;
	struct pollfd *pfds;
	int numfds, maxfds;
	std::vector<int> ports, sockets_created;
	std::map<int, request> requests;
	std::map<int, Response> respones;
	const char *envp[];

	int	checkTheport(int port);
	void	add_pfd(int fd);
	void	delete_pfd(int i);
	public:
		Server(std::vector<ServerConfig> &servers);
		void	run();
};