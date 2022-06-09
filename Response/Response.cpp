#include "Response.hpp"

bool	upload_file(std::string filename, std::string upload_dir);

Response::Response() : MIME_types(get_MIME_types()), status_defin(), servers(), headers() {}

Response::Response(std::vector<ServerConfig> &servers) : MIME_types(get_MIME_types()), status_defin(), servers(servers), headers() {
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

Response	&Response::operator=(const Response& response){
	
	MIME_types = response.MIME_types;
	status_defin = response.status_defin;
	headers = response.headers;
	servers = response.servers;
	return *this;
}

std::string Response::get_response(request	&req) {
	// std::cout << "-----------------Request--------------" << std::endl;
	// std::map<std::string, std::string> mp = req.getHeaders();
	// std::cout << req.getMethod() << std::endl;
	// std::map<std::string, std::string>::iterator it = mp.begin();
	// while (it != mp.end()) {
	// 	std::cout << "REquest headers = " << it->first << " " << it->second << std::endl;
	// 	it++;
	// }
	
	// std::cout << "Method = " << req.getMethod() << " file = " << req.getFilename() << std::endl;
	int i;
	i = select_server(req);
	// find_location(req.getUri(), i);
	std::string respo;
	if (!req.getExtension().empty()){ // request with file
		std::map<std::string, std::string>::iterator it = MIME_types.find(req.getExtension());
		if (it != MIME_types.end()) { // Regular file
			respo = MIME_response(req, i, it);
		} else { // it's CGI
			respo = CGI_response(req, i);
		}
	} else // request with Directory
		respo = Dir_response(req, i);
	headers.clear();
	// if (pr.path.find(".php") != std::string::npos) { 
	// 	Cgi_request cgi(req, servers[0]);
	// 	respo = cgi.execute();
	// }
	// else {
	// 	respo = "HTTP/1.1 200 OK\r\n\r\n";
	// 	std::string tmp (req.getHeaders().find("Accept")->second);
	// 	respo += read_file(getenv("PWD"), pr.path.c_str());
	// }
	return respo;
}

int	Response::select_server(request &req){
	std::stringstream ss;
	std::string Host;
	std::map<std::string, std::string> head = req.getHeaders();
	std::map<std::string, std::string>::iterator it = head.find("Host");
	if (it != head.end())
		Host = it->second;
	else
		return 0;
	size_t i = Host.find(":");
	if (i == std::string::npos)
		return 0;
	std::vector<int> list_servers_p; // list of Servers with the same port by index
	std::vector<int> list_servers_n; // list of Servers with the same name by index

	ss << Host.begin().base() + i + 1;
	int port;
	ss >> port;
	std::string server_name;
	// std::cout << "HOST = " << Host << " i = " << i << std::endl;
	try {
		server_name.assign(Host.begin(), Host.begin() + i);
	} catch (std::exception &e) {
		std::cout << "Exception Here = " << e.what() << std::endl;
	}
	for (size_t j = 0; j < servers.size(); j++)  // Looking in servers by Port if there is more than one
	{
		if (servers[j].get_port() == port)
			list_servers_p.push_back(j);
	}
	if (list_servers_p.size() > 1) // there is more than one server with the same port
	{
		for (size_t j = 0; j < list_servers_p.size(); j++) {
			if (servers[list_servers_p[j]].get_name() == server_name)
				list_servers_n.push_back(list_servers_p[j]);
		}
		return list_servers_n[0];
	}
	return list_servers_p[0];
}

std::string	Response::MIME_response(request &req, int i, std::map<std::string, std::string>::iterator &it) {
	std::string respo;
	std::vector<std::string> headers;
	std::string filename, length;
	std::stringstream ss;

	std::pair<int, std::string> location = find_location(req.getUri(), i);
	std::cout << "MIME + " << location.second << std::endl;
	filename = location.second;
	std::ifstream file(filename);
	if (location.first != -1 && !is_allowed(req.getMethod(), servers[i].getLocation()[location.first])) // Check if The Method Allowed
			return (make_error_response(405, servers[i]));
	// else if (servers[i].getLocation()[location.first].get_redirect().size() == 2)
	// {
	// 	int redirect_code;
	// 	std::string http_redi = servers[i].getLocation()[location.first].get_redirect()[1];
	// 	if (http_redi.find("http://") == std::string::npos)
	// 		http_redi.insert(0, "http://");
	// 	std::stringstream ss;
	// 	ss << servers[i].getLocation()[location.first].get_redirect()[0];
	// 	ss >> redirect_code;
	// 	return (make_redirection(redirect_code, http_redi));
	// }
	if (req.getMethod() == "POST")
	{
		if (location.first == -1) // Default Upload directory
		{}
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
		std::cout << "to be Deleted " << location.second << std::endl;
		std::ifstream file(location.second);
		if (!file.good())
			return make_error_response(404, servers[i]);
		if (std::remove(location.second.c_str()) == 0)
		{
			std::vector<std::string> heads;
			std::string body;
			return make_response(204, heads, body);
		}
		make_error_response(403, servers[i]);
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
	filename = servers[i].get_root() + req.getUri();
	file.open(filename.c_str());
	std::cout << "here = " << filename << std::endl;
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
	// std::pair<int, std::string> location = find_location(req.getExtension(), i);
	// std::cout << "ROOT = " << servers[i].get_root() << std::endl;
	std::string filename(servers[i].get_root() + req.getUri());

	std::cout << "CGI Filename = " << filename << std::endl;
	// std::cout << " BODY = " << req.getFilename() << std::endl;
	// std::ifstream file(req.getFilename());
	// if (file.is_open())
	// {
	// 	std::string line;
	// 	while(std::getline(file, line))
	// 		std::cout << line << std::endl;
	// }
	Cgi_request cgi(req, servers[i]);
	respo = cgi.execute();
	return respo;
}

std::string	Response::Dir_response(request &req, int i){
	std::string respo, ser_root, loca_root;
	std::string filename(servers[i].get_root() + req.getUri());
	LocationConfig location;
	std::pair<int, std::string> loc_i;

	// std::cout << "Dir Filename = " << req.getUri() << std::endl;
	loc_i = find_location(req.getUri(), i);
	// ser_root = servers[i].get_root();
	// if (!ser_root.empty() && ser_root[ser_root.size() - 1] == '/')
	// 	ser_root.erase(ser_root.end() - 1);

	/* LOCATION FOUNDED */
	if (loc_i.first != -1) {
		location = servers[i].getLocation()[loc_i.first];
		// loca_root = location.get_root();
		// if (!loca_root.empty() && loca_root[loca_root.size() - 1] != '/')
		// 	loca_root += "/";
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
					if (location.get_autoindex()) // AutoIndex is on
					{
						// std::cout << "loc " << loc_i.second << std::endl;
						AutoIndex autoindex(loc_i.second);
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
					std::cout << "file = " << filename << std::endl;
					if (file.is_open())
					{
						std::string extension;
						size_t f = filename.find(".");
						if (f != std::string::npos)
							extension.assign(filename.begin() + f, filename.end());
						std::map<std::string, std::string>::iterator it = MIME_types.find(extension);
						if (it == MIME_types.end()){ // if it's CGI
							Cgi_request cgi(servers[i]);
							return cgi.dir_execute(req.getHeaders(), filename, extension, location.get_name());
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
						if (location.get_autoindex())
						{
							AutoIndex autoindex(loc_i.second);
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
			else {} // Delete Request Method
			
			// else if (loca_root.empty()) // Location without root
			// 	respo = make_redirection(304, ser_root + req.getUri());
			// else { // Location with root
			// 	respo = make_redirection(301, loca_root);
		}
	}
	else { // There is no Location with this Name
		std::cout << "Location Not found" << std::endl;
		std::string server_root = servers[i].get_root();
		std::string uri = req.getUri();
		std::string filename;

		if (server_root[server_root.size() - 1] == '/')
			server_root.erase(servers.size() - 1, 1);
		filename = server_root + uri + "/" + servers[i].get_index();
		std::ifstream file(filename);
		if (file.is_open())
		{
			if (uri[uri.size() - 1] == '/')
				return (make_redirection(301, uri + servers[i].get_index()));
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
	for(int i = 0; i < headers.size(); i++) {
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
				// std::cout << "Location found = " << name << std::endl;
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
		filename = it->second;
	else {
		filename = "../Response/errors/";
		ss << status;
		ss >> code;
		filename += code + ".html";
	}
	file.open(filename);
	if (!file.is_open())
	{
		status = 404;
		file.open("../Response/errors/404.html");
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