#include "request.hpp"

request::request(std::string raw)
{

	std::cout << "*********** REQUEST **********" << std::endl;
	long header_end = raw.find("\r\n\r\n") + 4;

	this->body = raw.substr(header_end);

	header_size = header_end;
	body_size = body.length();
	total_size = header_size + body_size;

	std::cout << "header_size : " << header_size << std::endl << "body_size : "  << body_size << std::endl << "total : " << body_size +header_size << std::endl;

	std::string start_line = raw.substr(0, raw.find("\n"));

	this->method = start_line.substr(0, start_line.find(" "));
	start_line.erase(0, start_line.find(" ") + 1);
	this->URI = start_line.substr(0, start_line.find(" "));
	start_line.erase(0, start_line.find(" ") + 1);
	this->version = start_line.substr(0, start_line.find(" "));
	start_line.erase(0, start_line.find(" "));

	raw = raw.substr(raw.find("\r\n") + 2, header_size);

	while (raw.find("\r\n") != -1)
	{
		if (raw.find(": ") != -1)
		{
			std::string line = raw.substr(0, raw.find("\r\n"));
			headers.insert(std::pair<std::string, std::string>(line.substr(0, line.find(": ")), line.erase(0, line.find(": ") + 2)));
			raw.erase(0, raw.find("\r\n") + 2);
		}
		else
			break;
	}

	std::map<std::string, std::string>::iterator b = headers.begin();
	std::map<std::string, std::string>::iterator e = headers.end();

	while (b != e)
	{
		std::cout << b->first << "	|	" << b->second << std::endl;
		b++;
	}

	std::cout << "********* END REQUEST ********" << std::endl;
}
