#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <map>

class request
{
private:
    std::string method;
    std::string URI;
    std::string version;

    std::map<std::string, std::string> headers;

    std::string body;
    long        body_size;
    long        header_size;
    long        total_size;
public:
    request():method(), URI(), version(), body() {};

    request(std::string raw);
	std::string	getMethod() const;
	std::string	getUri() const;
	std::string	getVersion() const;
	std::map<std::string, std::string> getHeaders() const;
	long	getBodySize() const;
};