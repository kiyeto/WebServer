#include "request.hpp"



std::string request::gen_random(const int len)
{
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	
	return tmp_s;
}

void	request::check_URI()
{
	int	pos = URI.find("?");
	if (pos != -1)
	{
		query = URI.substr(pos + 1);
		URI.erase(pos);
	}
	pos = URI.find_last_of(".");
	if (pos != -1)
		extension = URI.substr(pos);
}

void	request::clear()
{
	method.clear();
	URI.clear();
	filename.clear();
	version.clear();
	header_raw.clear();
	hdr_cmplt = 0;
	chunk_not_cmplt = 0;
	chunk_len = 0;
	sent = 0;
	req_cmplt = 0;

	headers.clear();
	file.clear();

	body.clear();
	body_size = 0;
	header_size = 0;
	total_size = 0;
}

void	request::parse_unchunked(std::string & part)
{
	if (!chunk_not_cmplt)
	{
		std::map<std::string, std::string>::iterator it = headers.find("Content-Length");

		if (it != headers.end())
			chunk_len = stoi(it->second);
		chunk_not_cmplt = 1;
	}

	if (chunk_len == 0)
	{
		std::cout << "End of normal request !!! " << std::endl;
		req_cmplt = 1;
		this->clear();
		return ;
	}

	if (chunk_len > 0) 
	{
		file.open(filename, std::ios_base::app | std::ios::binary);
		file.write(part.c_str(), part.length());
		file.close();
		chunk_len -= part.length();
	}
	return ;
}

void	request::parse_chunked(std::string & part)
{
	while (part.length())
	{
		if (!chunk_not_cmplt)
		{
			int	pos = part.find("\r\n");
			sent = 0;

			std::stringstream	str(part.substr(0, pos));
			str >> std::hex >> chunk_len;
			std::cout << "hex to dec : " << chunk_len << std::endl;

			part = part.erase(0, pos + 2);
			if (chunk_len == 0)
			{
				std::cout << "End of chunked request !!! " << std::endl;
				req_cmplt = 1;
				this->clear();
				return ;
			}

			if (chunk_len >= part.length())
			{
				file.open(filename, std::ios_base::app | std::ios::binary);
				file.write(part.c_str(), part.length());
				file.close();
				chunk_len -= part.length();
				part.clear();
				chunk_not_cmplt = 1;
			}
			else
			{
				file.open(filename, std::ios_base::app | std::ios::binary);
				file.write(part.c_str(), chunk_len);
				file.close();
				part.erase(0, chunk_len + 2);
				chunk_not_cmplt = 0;
			}
		}
		else
		{
			if (chunk_len >= part.length())
			{
				file.open(filename, std::ios_base::app | std::ios::binary);
				file.write(part.c_str(), part.length());
				file.close();
				chunk_len -= part.length();
				part.clear();
				chunk_not_cmplt = 1;
			}
			else
			{
				file.open(filename, std::ios_base::app | std::ios::binary);
				file.write(part.c_str(), chunk_len);
				file.close();
				part.erase(0, chunk_len + 2);
				chunk_not_cmplt = 0;
			}
		}
	}
	return ;
}

void	request::parse_body(std::string & part)
{
	std::map<std::string, std::string>::iterator trnsfr_enc = headers.find("Transfer-Encoding");

	if (!filename.length())
		filename = gen_random(16);
	if (trnsfr_enc != headers.end() && (trnsfr_enc->second.find("chunked") != -1))
		parse_chunked(part);
	else
		parse_unchunked(part);
}


void	request::assemble_request(std::string & part)
{
	int header_end;

	if (!hdr_cmplt)
	{
		header_end = part.find("\r\n\r\n");
		if (header_end != -1)
		{
			this->hdr_cmplt = 1;
			header_raw += std::string(part.c_str(), header_end + 4);
			parse_headers(header_raw);
			if (method == "POST")
			{
				std::string sub = part.substr(header_end + 4);
				parse_body(sub);
			}
		}
		else if (header_end == -1)
			header_raw += std::string(part.c_str(), part.length());
	}
	else
		parse_body(part);
}

void	request::parse_headers(std::string& raw)
{
	// std::cout << "*********** REQUEST **********" << std::endl;
	long header_end = raw.find("\r\n\r\n") + 4;

	// if (raw.length() - header_end > 1)
	// 	this->body = raw.substr(header_end);
	// else
	// 	this->body.empty();

	header_size = header_end;
	// body_size = body.length();
	// total_size = header_size + body_size;

	// std::cout << "header_size : " << header_size << std::endl << "body_size : "  << body_size << std::endl << "total : " << body_size + header_size << std::endl;

	std::string start_line = raw.substr(0, raw.find("\n"));

	this->method = start_line.substr(0, start_line.find(" "));
	start_line.erase(0, start_line.find(" ") + 1);
	this->URI = start_line.substr(0, start_line.find(" "));
	check_URI();
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
		{
			raw.empty();
			break;
		}
	}

	// std::map<std::string, std::string>::iterator b = headers.begin();
	// std::map<std::string, std::string>::iterator e = headers.end();

	// while (b != e)
	// {
	// 	std::cout << b->first << "	|	" << b->second << std::endl;
	// 	b++;
	// }
	// std::cout << "--- START BODY ---" << std::endl;
	// std::cout << this->body;
	// std::cout << "--- END BODY ---" << std::endl;

	// std::cout << "********* END REQUEST ********" << std::endl;
	header_raw.clear();
}

std::string	request::getMethod() const {
	return method;
}

std::string	request::getUri() const {
	return URI;
}

std::string	request::getVersion() const {
	return version;
}

std::map<std::string, std::string> request::getHeaders() const{
	return headers;
}

long		request::getBodySize() const{
	return body_size;
}
