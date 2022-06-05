#include "request.hpp"

request::request() : method(""), URI(""), version(""), header_raw(), body(""), hdr_cmplt(0), chunk_not_cmplt(0), req_cmplt(0), chunk_len(0), sent(0), filename(), file(), status_code(0) {}

request::request(const request &req){
	(*this) = req;
}

request&		request::operator=(const request &req)
{
	method = 			req.method;
	URI = 				req.URI;
	extension = 		req.extension;
	query = 			req.query;
	version = 			req.version;
	header_raw = 		req.header_raw;
	hdr_cmplt = 		req.hdr_cmplt;
	chunk_not_cmplt = 	req.chunk_not_cmplt;
	chunk_len = 		req.chunk_len;
	sent = 				req.sent;
	fd = 				req.fd;
	req_cmplt = 		req.req_cmplt;
	filename = 			req.filename;
	headers =			req.headers;
	body = 				req.body;
	body_size = 		req.body_size;
	header_size = 		req.header_size;
	total_size = 		req.total_size;
	status_code	=		req.status_code;
	return (*this);
}

std::string request::gen_random(const int len)
{
	srand(0);
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

int	request::check_URI()
{
	std::string allowed_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	if (URI.length() > 2048)
		return (414);
	for (int i = 0; i < URI.length(); i++)
	{
		if (allowed_chars.find(URI[i]) == -1)
			return (400);
	}
	int	pos = URI.find("?");
	if (pos != -1)
	{
		query = URI.substr(pos + 1);
		URI.erase(pos);
	}
	pos = URI.find_last_of(".");
	if (pos != -1)
		extension = URI.substr(pos);
	return (0);
}

void	request::clear()
{
	method.clear();
	URI.clear();
	version.clear();
	extension.clear();
	query.clear();
	header_raw.clear();
	hdr_cmplt = 0;
	chunk_not_cmplt = 0;
	chunk_len = 0;
	sent = 0;
	req_cmplt = 0;

	headers.clear();
	std::remove(filename.c_str());
	filename.clear();

	body_size = 0;
	header_size = 0;
	total_size = 0;
	status_code = 0;
}

bool	request::parse_unchunked(std::string & part)
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
		chunk_not_cmplt = 0;
		return 1;
	}

	if (chunk_len > 0) 
	{
		file.open(filename, std::ios_base::app | std::ios::binary);
		file.write(part.c_str(), part.length());
		file.close();
		chunk_len -= part.length();
		if (chunk_len == 0)
		{
			std::cout << "End of normal request !!! " << std::endl;
			req_cmplt = 1;
			chunk_not_cmplt = 0;
			return 1;
		}
	}
	return 0;
}

bool	request::parse_chunked(std::string & part)
{
	while (part.length())
	{
		if (!chunk_not_cmplt)
		{
			int	pos = part.find("\r\n");
			sent = 0;

			std::stringstream	str(part.substr(0, pos));
			str >> std::hex >> chunk_len;
			// std::cout << "hex to dec : " << chunk_len << std::endl;

			part = part.erase(0, pos + 2);
			if (chunk_len == 0)
			{
				std::cout << "End of chunked request !!! " << std::endl;
				req_cmplt = 1;
				return 1;
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
	return 0;
}

bool	request::parse_body(std::string & part)
{
	std::map<std::string, std::string>::iterator trnsfr_enc = headers.find("Transfer-Encoding");

	if (!filename.length())
	{
		filename = gen_random(16);
		std::cout << filename << std::endl;
	}
	if (trnsfr_enc != headers.end() && (trnsfr_enc->second.find("chunked") != -1))
		return parse_chunked(part);
	else
		return parse_unchunked(part);
}


bool	request::assemble_request(std::string & part)
{
	int header_end;

	if (!hdr_cmplt)
	{
		header_end = part.find("\r\n\r\n");
		if (header_end != -1)
		{
			this->hdr_cmplt = 1;
			header_raw += std::string(part.c_str(), header_end + 4);
			if (int i = parse_headers(header_raw))
				return (1);

			std::string sub = part.substr(header_end + 4);
			return parse_body(sub);

		}
		else if (header_end == -1)
		{
			header_raw += std::string(part.c_str(), part.length());
			return 0;
		}
	}
	return parse_body(part);
}

int		request::parse_headers(std::string& raw)
{
	int	header_end = raw.find("\r\n\r\n") + 4;

	header_size = header_end;
	std::string start_line = raw.substr(0, raw.find("\n"));

	this->method = start_line.substr(0, start_line.find(" "));
	start_line.erase(0, start_line.find(" ") + 1);

	this->URI = start_line.substr(0, start_line.find(" "));
	status_code = check_URI();
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
	header_raw.clear();
	std::map<std::string, std::string>::iterator trnsfr_enc = headers.find("Transfer-Encoding");
	std::map<std::string, std::string>::iterator c_l = headers.find("Content-Length");
	if (status_code)
		return (status_code);
	if (method != "GET" && method != "POST" && method != "DELETE")
		return (status_code = 501);
	if (method == "POST" && trnsfr_enc == headers.end() && c_l == headers.end())
		return (status_code = 400);
	if (trnsfr_enc != headers.end() && trnsfr_enc->second != "chunked")
		return (status_code = 501);
	return (0);
}

std::string	request::getMethod() const {
	return method;
}

std::string	request::getUri() const {
	return URI;
}

std::string	request::getExtension() const{
	return extension;
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

std::string&	request::getFilename() {
	return	filename;
}

std::string	request::getQuery() const {
	return query;
}