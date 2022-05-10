#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "config.hpp"

#include "Cgi_request.hpp"


int main()
{
	// server def;
	// struct sockaddr_in addr;
	// int sock;
	// if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	// 	return -1;
	// // fcntl(sock, F_SETFL, O_NONBLOCK);
	// int opt = sizeof(addr);
	// // if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	// // 	std::cout << "setsockopt error" << std::endl;
	// memset(&addr, 0, sizeof(addr));
	// addr.sin_family = AF_INET;
	// addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// addr.sin_port = htons(def.port);
	// if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)))
	// 	std::cout << "bind failed" << std::endl;
	// if (listen(sock, 10) < 0)
	// {
	// 	std::cout << "Listen failed" << std::endl;
	// 	return 0;
	// }
	// int new_sock, msg_len, kq;
	// std::string msg;
	// char buffer[1024] = {0};
	// struct kevent changelist, eventlist;
	// kq = kqueue();
	// while (1)
	// {
	// 	std::cout << ">>>>>> Waiting for connection <<<<<<<<" << std::endl;
	// 	if (( new_sock = accept(sock, (struct sockaddr *)&addr, (socklen_t*)&opt)) < 0)
	// 	{
	// 		std::cout << "Accept failed " << new_sock << std::endl;
	// 		return 1;
	// 	}
	// 	std::cout << ">>>>>> connection established <<<<<<<<" << std::endl;
	// 	msg_len = recv(new_sock, buffer, 1024, MSG_PEEK);
	// 	msg += buffer;
	// 	std::cout << "msg_len = " << msg_len << std::endl;
	// 	// std::cout << msg << std::endl;
	// 	std::cout << ">>>>>> The request is <<<<<<" << std::endl;
	// 	std::cout << msg << std::endl;
	// }
	
	Cgi_request cgi;

	std::string path = "wordpress/index.php";
	cgi.execute(path);
	
}