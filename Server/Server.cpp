#include "Server.hpp"

Server::Server (std::vector<ServerConfig> &servers): servers(servers), pfds(), maxfds(), ports(), sockets_created()
{
	int 				server_fd;
	struct sockaddr_in	address;
	socklen_t			addrlen = sizeof(address);

	for(int i = 0; i < servers.size(); i++)
	{
		if (checkTheport(servers[i]) == 1)
			continue;
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		{
			perror("In socket");
			exit(EXIT_FAILURE);
		}
		sockets_created.push_back(server_fd);

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(servers[i].get_host().c_str());
		address.sin_port = htons( servers[i].get_port() );

		memset(address.sin_zero, '\0', sizeof address.sin_zero);
		int yes = 1;

		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			perror("In bind");
			exit(EXIT_FAILURE);
		}
		if (listen(server_fd, 1000) < 0)
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

int	Server::checkTheport(ServerConfig &server) {
	for(int i = 0; i < ports.size(); i++){
		if (ports[i] == server.get_port())
		{
			return 1;
		}
	}
	ports.push_back(server.get_port());
	hosts.push_back(server.get_host());
	return 0;
}

void	Server::run()
{
	int					valread;
	struct sockaddr_in	address;
	socklen_t			addrlen = sizeof(address);
	std::string response;
	request		req;
	char buffer[4096] = {0};
	std::fstream ofs;

	signal(SIGPIPE, SIG_IGN);
	while (1)
	{
		// std::cout << "|||||||||||||||||| Waiting for a connection ||||||||||||||||||" << std::endl;
		int poll_count = poll(pfds, numfds, 10000);
		if (poll_count == -1)
		{
			perror("poll");  
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < numfds; i++)
		{
			bool new_cnx = 0;;
			if (pfds[i].revents == POLLIN) // checking for reading
			{
				int new_socket;
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
						new_cnx = 1;
						break ;
					}
				}
				// if not a new connection, read the request
				if (!new_cnx)
				{
					valread = read(pfds[i].fd , buffer, 4095);
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
					requests.insert(std::make_pair(pfds[i].fd, request()));
					responses.insert(std::make_pair(pfds[i].fd, Response(servers)));
					std::string part = std::string(buffer, valread);
					if (requests[pfds[i].fd].assemble_request(part))
						pfds[i].events = POLLOUT;
				} 
			}
			else if ((pfds[i].revents & POLLHUP) && (pfds[i].revents & POLLIN) && !new_cnx)
			{
				std::cout << "+++++++++++POLLHUP HANDELINGU " << pfds[i].fd << std::endl;
				pfds[i].events = POLLIN;
				pfds[i].revents = 0;
				close(pfds[i].fd);
				delete_pfd(i);
			}
			else if (pfds[i].revents == POLLOUT && !new_cnx)
			{
				std::cout << requests[pfds[i].fd].getQuery() << " " << requests[pfds[i].fd].getUri() << std::endl;
				if (!responses[pfds[i].fd].is_complete())
					response = responses[pfds[i].fd].build_response(requests[pfds[i].fd]);
				else
					response = responses[pfds[i].fd].get_response();
				ssize_t wrote;
				if ((wrote = send(pfds[i].fd, response.c_str() + responses[pfds[i].fd].get_total(), responses[pfds[i].fd].get_left(), 0))== -1)
				{
					close(pfds[i].fd);
					delete_pfd(i);
					responses.erase(pfds[i].fd);
					continue;
				}
				responses[pfds[i].fd].Check(wrote);
				if (responses[pfds[i].fd].is_sent())
				{
					if (requests[pfds[i].fd].getHeaders().find("Connection")->second == "keep-alive")
						pfds[i].events = POLLIN;
					else {
						close(pfds[i].fd);
						delete_pfd(i);
					}
					std::cout << "Message Sent!!!!" << std::endl;
					responses[pfds[i].fd] = Response(servers);
				}
				requests[(pfds[i].fd)].clear();
				requests.erase(pfds[i].fd);
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
	pfds[numfds].events = 1;
	pfds[numfds].revents = 0;
	numfds++;
}

void	Server::delete_pfd(int i)
{
	std::cout << "socket (" << pfds[i].fd << ") closed." << std::endl;
	pfds[i] = pfds[--numfds];
}