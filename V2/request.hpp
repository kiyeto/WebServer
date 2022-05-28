#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <fstream>
#include <ctime>
#include <unistd.h>

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
	int				chunk_len;
	int				sent;
	bool			req_cmplt;
	std::string		filename;
	std::ofstream	file;


	std::map<std::string, std::string>	headers;

	std::string		body;
	long			body_size;
	long			header_size;
	long			total_size;

	void			check_URI();
	std::string		gen_random(const int len);
	void			parse_unchunked(std::string & part);
	void			parse_chunked(std::string& raw);
	void			parse_body(std::string & part);
public:
	request():method(""), URI(""), version(""), header_raw(), body(""), hdr_cmplt(0), chunk_not_cmplt(0), req_cmplt(0), chunk_len(0), sent(0), filename() {};

	void	assemble_request(std::string& part);
	void	parse_headers(std::string& raw);
	void	clear();

	request(std::string raw);
	std::string	getMethod() const;
	std::string	getUri() const;
	std::string	getExtension() const;
	std::string	getVersion() const;
	std::map<std::string, std::string> getHeaders() const;
	long		getBodySize() const;

};