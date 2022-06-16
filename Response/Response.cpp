#include "Response.hpp"

bool	upload_file(std::string filename, std::string upload_dir);

Response::Response() : MIME_types(get_MIME_types()), status_defin(), servers(), headers(), Done(), sent(), response(), total(), left() {}

Response::Response(std::vector<ServerConfig> &servers) : MIME_types(get_MIME_types()), status_defin(), servers(servers), headers(), Done(), sent(), response(), total(), left() {
	status_defin[200] = "200 OK\r\n";
	status_defin[201] = "201 Created\r\n";
	status_defin[202] = "202 Accepted\r\n";
	status_defin[204] = "204 No Content\r\n";
	status_defin[400] = "400 Bad Request\r\n";
	status_defin[401] = "401 Unauthorized\r\n";
	status_defin[403] = "403 Forbidden\r\n";
	status_defin[404] = "404 Not Found\r\n";
	status_defin[405] = "405 Method Not Allowed\r\n";
	status_defin[413] = "413 Request Entity Too Large\r\n";
	status_defin[414] = "414 Request-URI Too Long\r\n";
	status_defin[300] = "300 Multiple Choices\r\n";
	status_defin[301] = "301 Moved Permanently\r\n";
	status_defin[302] = "302 Found\r\n";
	status_defin[303] = "303 See Other\r\n";
	status_defin[304] = "304 Not Modified\r\n";
	status_defin[305] = "305 Use Proxy\r\n";
	status_defin[307] = "307 Temporary Redirect\r\n";
	status_defin[501] = "501 Not Implemented\r\n";
}

Response::Response(const Response& response){
	*this = response;
}

Response	&Response::operator=(const Response& res){
	
	MIME_types = res.MIME_types;
	status_defin = res.status_defin;
	headers = res.headers;
	servers = res.servers;
	Done = res.Done;
	sent = res.sent;
	response = res.response;
	total = res.total;
	left = res.left;
	return *this;
}

std::string &Response::build_response(request	&req) {
	headers = req.getHeaders();
	std::map<std::string, std::string>::iterator it;
	int i, file_size;
	if ((it = headers.find("Host")) == headers.end())
	{
		response = make_error_response(400, servers[0]);
		headers.clear();
		left = response.length();
		Done = true;
		return response;
	}
	i = select_server(req);
	std::ifstream in_file(req.getFilename(), std::ios::binary);
	in_file.seekg(0, std::ios::end);
	file_size = in_file.tellg();
	if (req.getMethod() == "POST" && file_size != -1 && file_size > (int)servers[i].get_body_size())
		response = make_error_response(413, servers[i]);
	else if (req.getStatusCode())
		response = make_error_response(req.getStatusCode(), servers[i]);
	else if (!req.getExtension().empty()){ // request with file
		std::map<std::string, std::string>::iterator it = MIME_types.find(req.getExtension());
		if (it != MIME_types.end()) { // Regular file
			response = MIME_response(req, i, it);
		} else { // it's CGI
			response = CGI_response(req, i);
		}
	} else // request with Directory
		response = Dir_response(req, i);
	headers.clear();
	left = response.length();
	Done = true;
	return response;
}

int	Response::select_server(request &req){
	std::stringstream ss;
	std::string Host;
	std::map<std::string, std::string> head = req.getHeaders();
	std::map<std::string, std::string>::iterator it = head.find("Host");
	std::string server_name;
	size_t port;

	if (it != head.end())
		Host = it->second;
	else
		return 0;
	size_t i = Host.find(":");
	if (i == std::string::npos){
		port = 80;
		server_name.assign(Host.begin(), Host.end());
	}
	else {
		ss << Host.begin().base() + i + 1;
		ss >> port;
		server_name.assign(Host.begin(), Host.begin() + i);
	}
	
	std::vector<int> list_servers_p; // list of Servers with the same port by index
	for (size_t j = 0; j < servers.size(); j++)  // Looking in servers by Port if there is more than one
	{
		if (servers[j].get_port() == port)
			list_servers_p.push_back(j);
	}
	if (list_servers_p.size() > 1) // there is more than one server with the same port
	{
		for (size_t j = 0; j < list_servers_p.size(); j++) {
			if (servers[list_servers_p[j]].get_name() == server_name)
				return (list_servers_p[j]);
		}
		return list_servers_p[0];
	}
	return list_servers_p[0];
}

std::string	Response::MIME_response(request &req, int i, std::map<std::string, std::string>::iterator &it) {
	std::string respo;
	std::vector<std::string> headers;
	std::string filename, length;
	std::stringstream ss;

	std::pair<int, std::string> location = find_location(req.getUri(), i);
	filename = location.second;
	std::ifstream file(filename);
	if (location.first != -1 && !is_allowed(req.getMethod(), servers[i].getLocation()[location.first])) // Check if The Method Allowed
			return (make_error_response(405, servers[i]));
	if (req.getMethod() == "POST")
	{
		if (location.first == -1) // Default Upload directory
			return make_error_response(403, servers[i]);
		else{
			if (servers[i].getLocation()[location.first].get_upload().empty()) // Upload forbidden
				return make_error_response(403, servers[i]);
			else { // upload Allowed
				std::vector<std::string> headers;
				std::string body;
				location.second.erase(location.second.begin() + location.second.find_last_of("/"), location.second.end());
				upload_file(req.getFilename(), location.second + servers[i].getLocation()[location.first].get_upload());
				headers.push_back("Content-Type: " + MIME_types[".html"]);
				headers.push_back("Content-Length: 0");
				return make_response(201, headers, body);
			}
		}
	}
	else if (req.getMethod() == "DELETE") {
		if (access(location.second.c_str(), F_OK) == -1)
			return make_error_response(404, servers[i]);
		if (std::remove(location.second.c_str()) == 0)
		{
			std::vector<std::string> heads;
			std::string body;
			return make_response(204, heads, body);
		}
		return make_error_response(403, servers[i]);
	}
	if (file.is_open()) // Check the Path Received Directly
	{
		std::string content((std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ));
		ss << content.length();
		ss >> length;
		headers.push_back("Content-Type: " + it->second);
		headers.push_back("Content-Length: " + length);
		return (make_response(200, headers, content));
	}
	std::string server_root = servers[i].get_root();
	if (server_root[server_root.size() - 1] == '/')
		server_root.erase(server_root.size() - 1, 1);
	filename = server_root + req.getUri();
	file.open(filename.c_str());
	if (file.is_open()) // Check the root of the Server + the path received
	{
		std::string content((std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ));
		ss << content.length();
		ss >> length;
		headers.push_back("Content-Type: " + it->second);
		headers.push_back("Content-Length: " + length);
		return (make_response(200, headers, content));
	}
	return make_error_response(404, servers[i]);
}

std::string	Response::CGI_response(request &req, int i){
	std::string respo;
	std::pair<int, std::string> location = find_location(req.getUri(), i);
	std::string filename(servers[i].get_root() + req.getUri());

	if (location.first != -1)
	{
		Cgi_request cg(servers[i]);
		if (!is_allowed(req.getMethod(), servers[i].getLocation()[location.first]))
			return make_error_response(403, servers[i]);
		return cg.dir_execute(req, location.second, req.getExtension(), servers[i].getLocation()[location.first].get_name());
	}
	Cgi_request cgi(req, servers[i]);
	respo = cgi.execute();
	return respo;
}

std::string	Response::Dir_response(request &req, int i){
	std::string respo, ser_root, loca_root;
	std::string filename(servers[i].get_root() + req.getUri());
	LocationConfig location;
	std::pair<int, std::string> loc_i;

	loc_i = find_location(req.getUri(), i);
	/* LOCATION FOUNDED */
	if (loc_i.first != -1) {
		location = servers[i].getLocation()[loc_i.first];
		if (!is_allowed(req.getMethod(), location)) // Check if The Method Allowed
			return (make_error_response(405, servers[i]));
		else if (location.get_redirect().size() == 2)
		{
			int redirect_code;
			std::string http_redi = location.get_redirect()[1];
			if (http_redi.find("http://") == std::string::npos)
				http_redi.insert(0, "http://");
			std::stringstream ss;
			ss << location.get_redirect()[0];
			ss >> redirect_code;
			return (make_redirection(redirect_code, http_redi));
		}
		else {
			if (req.getMethod() == "GET")
			{
				if (req.getUri()[req.getUri().size() - 1] != '/')
					return (make_redirection(301, req.getUri() + "/"));
				else if (location.get_location_index().empty()) // index File Not Specified
				{
					if (access(loc_i.second.c_str(), F_OK))
						make_error_response(404, servers[i]);
					if (location.get_autoindex()) // AutoIndex is on
					{
						AutoIndex autoindex(loc_i.second, req.getUri());
						std::vector<std::string> headers;
						std::string body(autoindex.getHtml()), length;
						std::stringstream ss;
						ss << body.length();
						ss >> length;
						headers.push_back(std::string("Content-Type: ") + MIME_types[".html"]);
						headers.push_back(std::string("Content-Length: ") + length);
						respo = make_response(200, headers, body);
					}
					else // AutoIndex Is Off
						return (make_error_response(403, servers[i]));
				}
				else if (!location.get_location_index().empty()) // Location index file specified
				{
					if (loc_i.second[loc_i.second.size() - 1] != '/')
						loc_i.second += "/";
					std::string filename = loc_i.second + location.get_location_index();
					std::ifstream file(filename);
					if (file.is_open())
					{
						std::string extension;
						size_t f = filename.find(".");
						if (f != std::string::npos)
							extension.assign(filename.begin() + f, filename.end());
						std::map<std::string, std::string>::iterator it = MIME_types.find(extension);
						if (it == MIME_types.end()){ // if it's CGI
							Cgi_request cgi(servers[i]);
 							return cgi.dir_execute(req, filename, extension, location.get_name());
						}
						std::vector<std::string> headers;
						std::stringstream ss;
						std::string body, length;
						body.assign((std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ));
						ss << body.length();
						ss >> length;
						headers.push_back(std::string("Content-Type: ") + MIME_types[".html"]);
						headers.push_back(std::string("Content-Length: ") + length);
						return make_response(200, headers, body);
					}
					else
					{
						if (access(loc_i.second.c_str(), F_OK))
							return make_error_response(404, servers[i]);
						if (location.get_autoindex())
						{
							AutoIndex autoindex(loc_i.second, req.getUri());
							std::vector<std::string> headers;
							std::string body(autoindex.getHtml()), length;
							std::stringstream ss;
							ss << body.length();
							ss >> length;
							headers.push_back(std::string("Content-Type: ") + MIME_types[".html"]);
							headers.push_back(std::string("Content-Length: ") + length);
							return (make_response(200, headers, body));
						}
						return (make_error_response(403, servers[i]));
					}
				}
			}
			else if (req.getMethod() == "POST")
			{
				std::vector<std::string> headers;
				std::string body;
				if (location.get_upload().empty())
					return make_error_response(403, servers[i]);
				if (location.get_root().empty()) // if Location doesn't have a root
					upload_file(req.getFilename(), servers[i].get_root() + location.get_name() + location.get_upload());
				else
				{
					if (location.get_upload()[location.get_upload().size() - 1] != '/')
						upload_file (req.getFilename(), location.get_root() + location.get_upload() + "/");
					else 
						upload_file (req.getFilename(), location.get_root() + location.get_upload());
				}
				headers.push_back("Content-Type: " + MIME_types[".html"]);
				headers.push_back("Content-Length: 0");
				return make_response(201, headers, body);
			}
			else { // Delete Request Method
				struct stat buf;
				if (stat(loc_i.second.c_str(), &buf) == 0)
				{
					if (buf.st_mode & S_IFDIR)
					{
						DIR *dirp;
						if ((dirp = opendir(loc_i.second.c_str())) != NULL)
						{
							if (delete_dir(loc_i.second, dirp, "*") != 0)
								return make_error_response(403, servers[i]);
							if (std::remove(loc_i.second.c_str()) != 0)
								return make_error_response(403, servers[i]);
							std::vector<std::string> heads;
							std::string body;
							return make_response(204, heads, body);
						}
						else
							return make_error_response(403, servers[i]);
					}
					std::vector<std::string> heads;
					std::string body;
					return make_response(204, heads, body);
				}
				return make_error_response(404, servers[i]);
			}
		}
	}
	else { // There is no Location with this Name
		std::string server_root = servers[i].get_root();
		std::string uri = req.getUri();
		std::string filename;

		if (uri[uri.size() - 1] != '/')
			return (make_redirection(301, uri + "/"));
		if (req.getMethod() == "POST" || req.getMethod() == "DELETE")
			return make_error_response(403, servers[i]);
		if (server_root[server_root.size() - 1] == '/')
			server_root.erase(server_root.size() - 1, 1);
		filename = server_root + uri + servers[i].get_index();
		std::ifstream file(filename);
		if (file.is_open())
		{
			std::string extension;
			size_t f = filename.find("."); // Looking for . to get Extension
			if (f != std::string::npos)
				extension.assign(filename.begin() + f, filename.end());
			std::map<std::string, std::string>::iterator it = MIME_types.find(extension);
			if (it == MIME_types.end()){ // if it's CGI
				Cgi_request cgi(servers[i]);
				std::string name = uri + servers[i].get_index();
 				return cgi.dir_execute(req, filename, extension, server_root + uri);
			}
			std::vector<std::string> headers;
			std::string body, length;
			std::stringstream ss;
			body.assign((std::istreambuf_iterator<char>(file) ), (std::istreambuf_iterator<char>() ));
			ss << body.length();
			ss >> length;
			headers.push_back(std::string("Content-Type: ") + MIME_types[".html"]);
			headers.push_back(std::string("Content-Length: ") + length);
			return (make_response(200, headers, body));
		}
		return (make_error_response(404, servers[i]));
	}
	return respo;
}

std::string	Response::make_response(int status, std::vector<std::string> &headers, std::string &body) {
	std::string response("HTTP/1.1 ");

	response += status_code(status);
	for(size_t i = 0; i < headers.size(); i++) {
		response += headers[i] + "\r\n";
	}
	response += "\r\n" + body;
	return response;
}

std::string	Response::status_code(int status){
	std::stringstream ss;
	std::string tmp;
	std::map<int, std::string>::iterator it = status_defin.find(status);
	if (it != status_defin.end())
		return it->second;
	return std::string ("200 OK\r\n");
}

std::pair<int, std::string>		Response::find_location(std::string name, int i) {
	std::string copy (name);
	size_t	last_i = copy.find_last_of('/');
	std::vector<LocationConfig> locs = servers[i].getLocation();
	size_t j = 0;

	while (last_i != std::string::npos)
	{
		j = 0;
		for (; j < locs.size(); j++) {
			if (locs[j].get_name() == copy)
			{
				name.erase(0,copy.length());
				name.insert(0, locs[j].get_root());
				return std::make_pair(j, name);
			}
		}
		copy.erase(copy.begin() + last_i, copy.end());
		last_i = copy.find_last_of('/');
	}
	return std::make_pair(-1, std::string());
}

bool			Response::is_allowed(std::string method, LocationConfig location)
{
	std::vector<std::string> methods = location.getMethods();
	
	if (methods.empty())
		return true;
	for (size_t i = 0; i < methods.size(); i++){
		if (method == methods[i])
			return true;
	}
	return false;
}

std::string		Response::make_error_response(int status, ServerConfig &server) {
	std::string respo ("HTTP/1.1 ");
	std::stringstream ss;
	std::map<int, std::string> error_pages = server.get_error_pages();
	std::map<int, std::string>::iterator it = error_pages.find(status);
	std::string	code, length, filename, body;
	std::ifstream	file;

	if (it != error_pages.end())
	{
		// size_t i;
		filename = it->second;
		// if ((i = filename.find_last_of("/")) != std::string::npos)
		// {
		// 	if (i != 0)
		// 		return make_redirection(307, std::string(filename.begin(), filename.end()));
		// }
	}
	else {
		filename = "Response/errors/";
		ss << status;
		ss >> code;
		filename += code + ".html";
	}
	file.open(filename);
	if (!file.is_open())
	{
		status = 404;
		file.open("Response/errors/404.html");
	}
	body.append( (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()) );
	ss.clear();
	ss << body.length();
	ss >> length;
	respo += status_defin[status];
	respo += "Content-Type: " + (MIME_types[".html"] + "\r\n");
	respo += "Content-Length: " + (length + "\r\n\r\n");
	respo += body;
	return respo;
}

std::string		Response::make_redirection(int status, std::string to){
	std::string respo ("HTTP/1.1 ");

	respo += status_defin[status];
	respo += "Location: " + (to + "\r\n");
	respo += "Content-Length: 0\r\n\r\n";

	return respo;
}

int				Response::delete_dir(std::string dir, DIR *dirp, std::string star)
{
	struct dirent *d;
	struct stat buf;
	while ((d = readdir(dirp)))
	{
		if (stat((dir + "/" + d->d_name).c_str(), &buf) == 0)
		{
			if (buf.st_mode & S_IFDIR)
			{
				if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0){
					DIR *dirp1 = opendir((dir + "/" + d->d_name).c_str());
					if (dirp1 != NULL)
					{
						delete_dir((dir + "/") + d->d_name, dirp1, star + "*");
						if (std::remove(((dir + "/") + d->d_name).c_str()) != 0)
							return -1;
					}
				}
			}
			else
			{
				if (std::remove((dir + "/" + d->d_name).c_str()) != 0)
					return -1;
			}
		}
	}
	return 0;
}

bool Response::is_complete() const {
	return Done;
}

bool	Response::is_sent() const {
	return sent;
}

void	Response::Check(ssize_t s)
{
	total += s;
	left = response.length() - total;
	if (total == response.length())
		sent = true;
}

std::string& Response::get_response(){
	return response;
}

size_t	Response::get_total() const {
	return total;
}

size_t	Response::get_left() const {
	return left;
}