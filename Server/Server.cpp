#include "Server.hpp"

Server::Server (std::vector<ServerConfig> &servers): servers(servers), pfds(), maxfds(), ports(), sockets_created()
{
	int 				server_fd;
	struct sockaddr_in	address;
	socklen_t			addrlen = sizeof(address);

	for(int i = 0; i < servers.size(); i++)
	{
		if (checkTheport(servers[i].get_port()) == 1) // Check if the port is Already binded == 1, if not == 0
			continue;
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		{
			perror("In socket");
			exit(EXIT_FAILURE);
		}
		sockets_created.push_back(server_fd);

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons( servers[i].get_port() );

		memset(address.sin_zero, '\0', sizeof address.sin_zero);
		int yes = 1;

		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			perror("In bind");
			exit(EXIT_FAILURE);
		}
		if (listen(server_fd, 10) < 0)
		{
			perror("In listen");  
			exit(EXIT_FAILURE);
		}
	}
	/* END OF CREATON AND BINDING THE SOCKETS */
	pfds = (struct pollfd*)malloc(sizeof(*pfds) * sockets_created.size());
	maxfds = sockets_created.size();
	numfds = 0;
	for (int i = 0; i < sockets_created.size(); i++)
		add_pfd(sockets_created[i]);
}

int	Server::checkTheport(int port) {
	for(int i = 0; i < ports.size(); i++){
		if (ports[i] == port)
			return 1;
	}
	ports.push_back(port);
	return 0;
}

void	Server::run() {
	int					valread;
	struct sockaddr_in	address;
	socklen_t			addrlen = sizeof(address);
	std::string response;
	request		req;
	char buffer[4096] = {0};
	std::fstream ofs;
	Response	resp(servers);

	while (1)
	{
		std::cout << "Waiting for a connection " << std::endl;

		int poll_count = poll(pfds, numfds, -1);

		if (poll_count == -1)
		{
			perror("poll");  
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < numfds; i++)
		{
			// std::cout  << "i = " << i << std::endl;
			if (pfds[i].revents == POLLIN) // checking for reading
			{
				// poll_count--;
				int new_socket;
				bool new_cnx = 0;;
				for(int k = 0; k < sockets_created.size(); k++)
				{
					if (pfds[i].fd == sockets_created[k])
					{
						if ((new_socket = accept(sockets_created[k], (struct sockaddr *)&address, &addrlen)) < 0)
						{
							perror("accept");
							exit(EXIT_FAILURE);
						}
						add_pfd(new_socket);
						requests[pfds[i].fd] = request();
						// responses[pfds[i].fd] = Response();
						std::cout << "server: new connexion on socket (" << new_socket << ")" << std::endl;
						new_cnx = 1;
						break ;
					}
				}
				// if not a new connection, read the request
				if (!new_cnx)
				{
					valread = read(pfds[i].fd , buffer, 4095);
					// std::cout << "valread "  << " : " << valread << std::endl;
					if (valread <= 0)
					{
						if (valread == 0)
							std::cout << "server: socket " << pfds[i].fd << " hung up\n";
						else
							perror("read");
						close(pfds[i].fd);
						delete_pfd(i);
						continue;
					}
					buffer[valread] = 0;
					std::string part = std::string(buffer, valread);
					std::cout << "IN = " << pfds[i].fd << std::endl;
					if (requests[pfds[i].fd].assemble_request(part))
						pfds[i].events = POLLOUT;
					/* Added By Brahim */
					// try {
					// 	response = resp.get_response(req);

					// } catch (std::exception &e) {
					// 	std::cout << e.what() << std::endl;
					// }
					// ofs.open("outputo", std::ios_base::app | std::ios::binary);
					// ofs << "/////// chunk " << ++j << " /////////" << std::endl;
					// ofs << "**************** read *******************" << std::endl;
					// ofs.write(buffer, valread);
					// ofs.close();
				}
				// request.append(std::string(buffer));
				// request.push_back(std::string(buffer));
				// std::cout << *(--request.end()) << std::endl;
				// request req((std::string(buffer)));
				// path = std::string("/Users/abenouda/Desktop/WebServ") + req.getURI();
				// if (stat(path.c_str(), &st) == 0)
				// {
				// 	std::ifstream ifs(path);
				// 	// std::string	buf;
				// 	std::stringstream s;
				// 	if (ifs)
				// 	{
				// 		s << ifs.rdbuf();
				// 		response = header + s.str();
				// 		// std::cout << "*********** RESPONSE **********" << std::endl;
				// 		// std::cout << "********* END RESPONSE ********" << std::endl;
				// 		write(pfds[i].fd , response.c_str() , response.size());
				// 		std::cout << response << std::endl;
				// 	}
				// }
				// else
				// 	std::cout << "file not found !" << std::endl;
				// std::cout << "------------------Response sent-------------------" << std::endl;
				// close(pfds[i].fd);
				// delete_pfd(&pfds, i, &numfds);
			} //must check for writing here
			else if (pfds[i].revents == POLLOUT)
			{
				std::cout << "OUT = " << pfds[i].fd << std::endl;
				response = resp.get_response(requests[pfds[i].fd]);
				std::cout << "-----------Response-------------" << std::endl;
				std::cout << response << std::endl;
				write(pfds[i].fd, response.c_str(), response.length());
				// close(pfds[i].fd);
				// delete_pfd(i);
				if (requests[pfds[i].fd].getHeaders().find("Connection")->second == "keep-alive")
					pfds[i].events = POLLIN;
				else {
					close(pfds[i].fd);
					delete_pfd(i);
				}
				requests[(pfds[i].fd)].clear();
				requests.erase(pfds[i].fd);
				std::cout << "Socket == " << pfds[i].fd << std::endl;
			}
		}
	}
}

void	Server::add_pfd(int fd)
{
	if (numfds == maxfds)
	{
		maxfds *= 2;
		pfds = (struct pollfd *)realloc(pfds, sizeof(pfds) * (maxfds));
	}
	pfds[numfds].fd = fd;
	pfds[numfds].events = POLLIN;
	numfds++;
}

void	Server::delete_pfd(int i)
{
	pfds[i] = pfds[numfds - 1];
	numfds--;
}