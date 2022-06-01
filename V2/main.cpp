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
#include "request.hpp"

/* Includes Added by Brahim*/
#include "../Parsing Conf/configfile_src/ServerConfig.hpp"
#include "../Parsing Conf/configfile_src/LocationConfig.hpp"
#include "../Parsing Conf/configfile_src/ConfigfileClass.hpp"
#include "../Response/Response.hpp"
#include "../Server/Server.hpp"


#define PORT 8080

// void	add_pfd(struct pollfd **pfds, int fd, int *numfds, int *maxfds)
// {
// 	if (*numfds == *maxfds)
// 	{
// 		*maxfds *= 2;
// 		*pfds = (struct pollfd *)realloc(*pfds, sizeof(pfds) * (*maxfds));
// 	}
// 	(*pfds)[*numfds].fd = fd;
// 	(*pfds)[*numfds].events = POLLIN;
// 	(*numfds)++;
// }

// void	delete_pfd(struct pollfd **pfds, int i, int *numfds)
// {
// 	(*pfds)[i] = (*pfds)[*numfds - 1];
// 	(*numfds)--;
// }

int main(int argc, char const *argv[], char const *envp[])
{
	if (argc != 2)
	{
		std::cout << "Config file Needed!" << std::endl;
		return 1;
	}
	// int					server_fd;
	// int					valread;
	// struct sockaddr_in	address;
	// socklen_t			addrlen = sizeof(address);

	std::string arg(argv[1]);
	ConfigfileClass config(arg);
	try {
		config.configfileparser();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::vector<ServerConfig> servers = config.getServerConfig();
	Server server(servers);

	// // Only this line has been changed. Everything is same.
	// std::string header = "HTTP/1.1 200 OK\r\n\r\n"; // = "HTTP/1.1 200 OK\n\n\nHello world!";    
	// // Creating socket file descriptor
	// if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	// {
	// 	perror("In socket");
	// 	exit(EXIT_FAILURE);
	// }
	

	// address.sin_family = AF_INET;
	// address.sin_addr.s_addr = INADDR_ANY;
	// address.sin_port = htons( servers[0].get_port() );
	
	// memset(address.sin_zero, '\0', sizeof address.sin_zero);
	// int yes = 1;
	
	// setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	// if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	// {
	// 	perror("In bind");
	// 	exit(EXIT_FAILURE);
	// }


	// if (listen(server_fd, 10) < 0)
	// {
	// 	perror("In listen");  
	// 	exit(EXIT_FAILURE);
	// }

	// int maxfds = 10, numfds = 0;
	// struct pollfd *pfds = (struct pollfd*)malloc(sizeof(*pfds) * maxfds);

	// pfds[0].fd = server_fd;
    // pfds[0].events = POLLIN;
    // numfds++;

	// char buffer[4096] = {0};
	// std::fstream ofs;

	// // std::vector<std::string> request;
	// std::string	path;
	// struct stat st;
	// // std::string request;
	// std::string response;
	// request		req;
	// Response	resp(servers);
	
	// int j = 0;
	server.run();
	// while (1)
	// {
	// 	std::cout << "Waiting for a connection " << std::endl;

	// 	int poll_count = poll(pfds, numfds, -1);

	// 	if (poll_count == -1)
	// 	{
	// 		perror("poll");  
	// 		exit(EXIT_FAILURE);
	// 	}

	// 	for (int i = 0; i < numfds; i++)
	// 	{
	// 		if ((pfds[i].revents & POLLIN)) // checking for reading
	// 		{
	// 			// poll_count--;
	// 			int new_socket;
	// 			if (pfds[i].fd == server_fd)
	// 			{
	// 				if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
	// 				{
	// 					perror("accept");
	// 					exit(EXIT_FAILURE);
	// 				}
	// 				j = 0;
	// 				add_pfd(&pfds, new_socket, &numfds, &maxfds);
	// 				std::cout << "server: new connexion on socket (" << new_socket << ")" << std::endl;
	// 			}
	// 			else	// if not a new connection, read the request
	// 			{
	// 				valread = read(pfds[i].fd , buffer, 4095);
	// 				std::cout << "valread "  << " : " << valread << std::endl;
	// 				if (valread <= 0)
	// 				{
	// 					if (valread == 0)
	// 						std::cout << "server: socket " << pfds[i].fd << " hung up\n";
	// 					else
	// 						perror("read");

	// 					close(pfds[i].fd);
	// 					delete_pfd(&pfds, i, &numfds);
	// 					continue;
	// 				}
	// 				buffer[valread] = 0;
	// 				ofs.open("outputo", std::ios_base::app | std::ios::binary);
	// 				std::string part = std::string(buffer, valread);
	// 				req.assemble_request(part);

	// 				/* Added By Brahim */
	// 				response = resp.get_response(req);

	// 				std::cout << "-----------Response--------" << std::endl;
	// 				std::cout << response << std::endl;
	// 				// ofs << "/////// chunk " << ++j << " /////////" << std::endl;
	// 				// ofs << "**************** read *******************" << std::endl;
	// 				ofs.write(buffer, valread);
	// 				ofs.close();
	// 				// request.append(std::string(buffer));
	// 				// request.push_back(std::string(buffer));
	// 				// std::cout << *(--request.end()) << std::endl;
	// 				// request req((std::string(buffer)));
					
	// 				// path = std::string("/Users/abenouda/Desktop/WebServ") + req.getURI();

	// 				// if (stat(path.c_str(), &st) == 0)
	// 				// {
	// 				// 	std::ifstream ifs(path);
	// 				// 	// std::string	buf;
	// 				// 	std::stringstream s;
	// 				// 	if (ifs)
	// 				// 	{
	// 				// 		s << ifs.rdbuf();

	// 				// 		response = header + s.str();
	// 				// 		// std::cout << "*********** RESPONSE **********" << std::endl;
	// 				// 		// std::cout << "********* END RESPONSE ********" << std::endl;
	// 				// 		write(pfds[i].fd , response.c_str() , response.size());
	// 				// 		std::cout << response << std::endl;
	// 				// 	}
	// 				// }
	// 				// else
	// 				// 	std::cout << "file not found !" << std::endl;

	// 				// std::cout << "------------------Response sent-------------------" << std::endl;
	// 				// close(pfds[i].fd);
	// 				// delete_pfd(&pfds, i, &numfds);
	// 			}
	// 		} //must check for writing here
	// 		else if ((pfds[i].revents & POLLOUT))
	// 		{

	// 		}
	// 	}




		// set up non blocking sockets here !!!!!!!!!!!!!!!
		// std::vector<std::string> request;
		// //	request info are in the buffer	
		// char buffer[30000] = {0};



		// std::cout << "*********** REQUEST **********" << std::endl;
		// while ((valread = read(new_socket , buffer, 30000)))
		// {
		// 	buffer[valread] = '\0';
		// 	std::cout << "valread = " << valread << std::endl;
		// 	request.push_back(std::string(buffer));
		// 	// valread = read(new_socket, buffer, 30000);
		// 	std::cout << *(--request.end()) << std::endl;
		// }	

		//	request info are in the buffer
		

		// std::cout << "*********** REQUEST **********" << std::endl;
		// std::cout << buffer << std::endl;
		// std::cout << "********* END REQUEST ********" << std::endl;
		//	taking only the first line to parse it
		// char*	token = strtok(buffer, " \n");
		// int current = 0;

		// while( token != NULL )
		// {	
		// 	token = strtok(NULL, " \n");
		// 	if (current == 0)
		// 	{
		

	// }
	return 0;
}
