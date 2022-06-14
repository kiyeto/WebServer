#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include "../Parsing Conf/configfile_src/ServerConfig.hpp"


class request
{
private:
	std::string		method;
	std::string		URI;
	std::string		extension;
	std::string		query;
	std::string		version;
	std::string 	header_raw;
	bool			hdr_cmplt;
	bool			chunk_not_cmplt;
	size_t			chunk_len;
	int				sent;
	int				fd;
	int				status_code;
	bool			req_cmplt;
	std::string		filename;
	std::ofstream	file;


	std::map<std::string, std::string>	headers;

	long			body_size;
	long			header_size;
	long			total_size;

	int				check_URI();
	std::string		gen_random(const int len);
	bool			parse_unchunked(std::string & part);
	bool			parse_chunked(std::string& raw);
	bool			parse_body(std::string & part);
public:
	request();
	request(const request &req);

	request& operator=(const request &req);

	bool	assemble_request(std::string& part);
	int		parse_headers(std::string& raw);
	void	clear();

	request(std::string raw);
	std::string	getMethod() const;
	std::string	getUri() const;
	std::string	getExtension() const;
	std::string	getVersion() const;
	std::map<std::string, std::string> getHeaders() const;
	long		getBodySize() const;
	std::string&	getFilename();
	std::string	getQuery() const;
	int 		getStatusCode() const;

};