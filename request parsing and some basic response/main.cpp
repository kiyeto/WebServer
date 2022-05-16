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
#include <fstream>
#include <sstream>
#include <vector>
#include "request.hpp"
#define PORT 8080

int main(int argc, char const *argv[])
{
	int server_fd, new_socket;
	long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	// Only this line has been changed. Everything is same.
	std::string header = "HTTP/1.1 200 OK\r\n\r\n"; // = "HTTP/1.1 200 OK\n\n\nHello world!";    
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}
	

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}


	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");  
		exit(EXIT_FAILURE);
	}


	while (1)
	{
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n" << std::endl;
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}

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
		char buffer[1000] = {0};
		std::ofstream ofs("file", std::ios_base::app);

		valread = read(new_socket , buffer, 1000);
		buffer[valread] = '\0';
		request req(std::string(buffer, valread));

		std::cout << "valread " << " : " << valread << std::endl;
		ofs << buffer;

		// std::cout << "*********** REQUEST **********" << std::endl;
		// std::cout << buffer << std::endl;
		// std::cout << "********* END REQUEST ********" << std::endl;
	//	taking only the first line to parse it
		char*	token = strtok(buffer, " \n");
		std::string	path;
		int current = 0;

		while( token != NULL )
		{	
			token = strtok(NULL, " \n");
			if (current == 0)
			{
				path = std::string("/Users/abenouda/Desktop/WebServ") + std::string(token);
				// path = token;
				// std::cout << path << std::endl;
				break;
			}
		}

		struct stat st;
		if (stat(path.c_str(), &st) == 0)
		{
			std::ifstream ifs(path);
			// std::string	buf;
			std::stringstream s;
			if (ifs)
			{
				s << ifs.rdbuf();

			std::string response = header + s.str();
			// std::cout << "*********** RESPONSE **********" << std::endl;
			// std::cout << response << std::endl;
			// std::cout << "********* END RESPONSE ********" << std::endl;
			write(new_socket , response.c_str() , response.size());
			}
		}
		else
			std::cout << "file not found !" << std::endl;
		
		// std::cout << "------------------Response sent-------------------" << std::endl;
		close(new_socket);
	}
	return 0;
}
