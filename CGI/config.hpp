#pragma once
#include <iostream>
class server
{
	public:
		int port;
		std::string root;

		server() : port(8080), root("wordpress/index.php") {}
};
