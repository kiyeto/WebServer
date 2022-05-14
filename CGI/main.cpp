#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sstream>
#include <sys/select.h>
#include <iostream>
#include <fstream>
#include "config.hpp"

#include "Cgi_request.hpp"

std::string	recv_all(int sock){
	char buffer[1024];
	std::string ret;
	int msg_len;
	
	// while (1)
	// {
		memset(buffer, 0, 1024);
		if ((msg_len = recv(sock, buffer, 1023, MSG_PEEK)) < 0)
			std::cout << "Hey" << std::endl;
		else{
			ret += buffer;
		}
	// }
	return ret;
}

std::string	read_file(char* filename){
	std::ifstream fd(filename);
	std::string ret( (std::istreambuf_iterator<char>(fd) ),
                       (std::istreambuf_iterator<char>()    ) );
	return ret;
}

int main()
{
	server def;
	struct sockaddr_in addr;
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	// fcntl(sock, F_SETFL, O_NONBLOCK);
	int opt = sizeof(addr);
	// if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	// 	std::cout << "setsockopt error" << std::endl;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(def.port);
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)))
		std::cout << "bind failed" << std::endl;
	if (listen(sock, 10) < 0)
	{
		std::cout << "Listen failed" << std::endl;
		return 0;
	}
	int new_sock, msg_len, kq;
	std::string msg;
	struct kevent changelist, eventlist;
	kq = kqueue();
	std::string respo = "HTTP/1.1 200 \"OK\"\nContent_Type: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n\n";
	while (1)
	{
		std::cout << ">>>>>> Waiting for connection <<<<<<<<" << std::endl;
		if (( new_sock = accept(sock, (struct sockaddr *)&addr, (socklen_t*)&opt)) < 0)
		{
			std::cout << "Accept failed " << new_sock << std::endl;
			return 1;
		}
		std::cout << ">>>>>> connection established <<<<<<<<" << std::endl;
		msg = recv_all(new_sock);
		// std::cout << msg << std::endl;
		std::cout << ">>>>>> The request is <<<<<<" << std::endl;
		std::cout << msg << std::endl;


		std::cout << ">>>>>> The response is <<<<<<" << std::endl;
		respo += read_file("projection_BY_TEMPLATED/index.html");
		send(new_sock, respo.c_str(), respo.length(), 0);
	}
	
	// Cgi_request cgi;

	// std::string path = "wordpress/index.php";
	// cgi.execute(path);
	
}