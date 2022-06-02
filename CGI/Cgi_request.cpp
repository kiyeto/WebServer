#include "Cgi_request.hpp"

Cgi_request::Cgi_request(request &r, ServerConfig &server): req(r), server(server)
{
	// meta.insert(std::make_pair(std::string("GATEWAY_INTERFACE="), std::string("CGI/1.1")));
	meta.insert(std::make_pair(std::string("PATH_INFO="), std::string(server.get_root() + req.getUri())));
	meta.insert(std::make_pair(std::string("QUERY_STRING="), req.getQuery()));
	if (req.getMethod() != "GET")
		meta.insert(std::make_pair(std::string("REQUEST_METHOD="), std::string(req.getMethod())));
	if (req.getUri() == "/")
		meta.insert(std::make_pair(std::string("SCRIPT_NAME="), std::string(server.get_root() + "/index.php")));
	else
		meta.insert(std::make_pair(std::string("SCRIPT_NAME="), std::string(server.get_root() + req.getUri())));
	// meta.insert(std::make_pair(std::string("SERVER_NAME="), std::string("127.0.0.1")));
	meta.insert(std::make_pair(std::string("SERVER_PORT="), std::string("8080")));
	meta.insert(std::make_pair(std::string("SERVER_PROTOCOL="), std::string("HTTP/1.1")));
	// if (req.getBodySize())
	// {
	// 	meta.insert(std::make_pair(std::string("CONTENT_LENGTH="), std::string( req.getHeaders().find("Content-Length")->second )));
	// 	meta.insert(std::make_pair(std::string("CONTENT_TYPE="), std::string( req.getHeaders().find("Content-Type")->second )));
	// }
	// meta.insert(std::make_pair(std::string("REDIRECT_STATUS="), std::string( "302" )));
	// meta.insert(std::make_pair(std::string(""), std::string()));

}

std::string	Cgi_request::execute(){
	std::vector<std::string> args;

	std::string cmd(find_location(req.getExtension()));
	std::string response;
	std::stringstream ss;
	const char *meta_vars[meta.size() + 1];
	std::string start_line("HTTP/1.1");

	if (cmd.empty()) // there is no Location for this File Extension
	{
		std::ifstream file(meta.find("SCRIPT_NAME=")->second);
		if (file.is_open()) {
			std::string body( (std::istreambuf_iterator<char>(file) ),
                       (std::istreambuf_iterator<char>()    ) );
			ss << body.length();
			std::string size;
			ss >> size;
			start_line += " 200 OK\r\n";
			start_line += "Server: WebServ/bamghoug\r\n";
			start_line += "Content-Type: application/octet-stream\r\n";
			start_line += "Content-Length: ";
			start_line += size + "\r\n";
			start_line += "Connection: keep-alive\r\n\r\n";
			start_line += body;
			return start_line;
		}
	}
	args.push_back(cmd); // Need To Be replaced With CMD
	args.push_back(meta.find("SCRIPT_NAME=")->second);
	std::string query = meta.find("QUERY_STRING=")->second;
	int i = 0;
	while ((i = query.find("&")) != std::string::npos) {
		args.push_back(std::string(query.begin(), query.begin() + i));
		query.erase(query.begin(), query.begin() + i + 1);
	}
	if (!query.empty())
		args.push_back(std::string(query));
	int input = open (req.getFilename().c_str(), O_RDONLY);
	if (input != -1) {
		std::ifstream file(req.getFilename());

		std::string str((std::istreambuf_iterator<char>(file) ),
                   (std::istreambuf_iterator<char>()    ));
		std::cerr << str << std::endl;

		while ((i = str.find("&")) != std::string::npos) {
			args.push_back(std::string(str.begin(), str.begin() + i));
			str.erase(str.begin(), str.begin() + i + 1);
		}
			if (!str.empty())
				args.push_back(std::string(str));
		// dup2(input, 0);
	}
	// std::cout << "HERE" << std::endl;
	// if (req.getMethod() == "POST")
	// {
		// ss << meta.find("QUERY_STRING=")->second;
		// while (ss)
		// {
		// 	std::string tmp;
		// 	ss >> tmp;
		// 	args.push_back(tmp);
		// }
	// }
	ss.clear();
	const char *p[args.size() + 1];
	for(int i = 0; i < args.size(); i++)
		p[i] = args[i].c_str();
	p[args.size()] = NULL;

	std::string tmp[meta.size() + 1];
	std::map<std::string, std::string>::iterator it = meta.begin();
	for(int i = 0; i < meta.size(); i++)
	{
		tmp[i] = it->first + it->second;
		meta_vars[i] = tmp[i].c_str();
		it++;
	}
	meta_vars[meta.size()] = NULL;
	response = child_proce(p, (const char**)meta_vars);
	parse_cgiResponse(response);
	it = headers.find("Status");
	if (it != headers.end())
	{
		start_line += it->second;
		int len = response.length() - (response.find("\r\n\r\n") + 4);
		std::cout << "LENGTH = " << len << std::endl;
		std::string length;
		ss << len;
		ss >> length;
		ss.clear();
		length += "\r\n";
		start_line += "Content-Length: ";
		start_line += length;
		start_line += response;
		headers.clear();
		return start_line;
	}
	start_line += " 200 OK\r\n";
	int len = response.length() - (response.find("\r\n\r\n") + 4);
	std::cout << "LEN = " << len << std::endl;
	ss << len;
	std::string length;
	ss >> length;
	length += "\r\n";
	start_line += "Content-Length: ";
	start_line += length;
	start_line += response;
	headers.clear();
	return start_line;
}

std::string Cgi_request::child_proce(const char **cmd, const char **envp){
	int fds[2];
	pipe(fds);
	int child = fork();

	// std::ifstream file(req.getFilename());
	// std::string tmp;
	// std::cout << "====== into file ==== " << std::endl;
	// while(std::getline(file, tmp))
	// {
	// 	write(fds[1], tmp.c_str(), tmp.length());
	// 	std::cout << tmp << std::endl;
	// }
	// std::cout << "====== out of file ==== " << std::endl;
	
	if (child == 0) //child Process
	{
		// int input = open (req.getFilename().c_str(), O_RDONLY);
		// if (input != -1) {
		// 	std::ifstream file(req.getFilename());
		// 	std::string str((std::istreambuf_iterator<char>(file) ),
        //                (std::istreambuf_iterator<char>()    ));
		// 	std::cerr << str << std::endl;
		// 	dup2(input, 0);
		// }
		// else
		int i = -1;
		while (cmd[++i])
			std::cout << "cmd = " << cmd[i] << std::endl;
		
		dup2(fds[1], 1);
		close(fds[0]);
		if (execve(cmd[0], (char *const *)cmd, (char *const *) NULL) == -1)
			exit(1);
	}
	wait(NULL);
	close(fds[1]);
	char buf[1025];
	memset(buf, 0, 1025);
	std::string content;
	while(read(fds[0], buf, 1024) > 0)
	{
		content += buf;
		memset(buf, 0, 1025);
	}
	return content;
}

void	 Cgi_request::parse_cgiResponse(std::string respo) {
	size_t i = 0, j = 0;

	// if ((i = respo.find("\r\n\r\n")) != std::string::npos)
	// {
	// 	body = respo.begin().base() + i + 4;
	// 	respo.erase(respo.begin() + i, respo.end());
	// }
	while ((i = respo.find("\r\n")) != std::string::npos)
	{
		std::string tmp(respo.begin(), respo.begin() + i + 2);
		j = tmp.find(":");
		if (j != std::string::npos)
			headers.insert(std::make_pair(std::string(tmp.begin(), tmp.begin() + j), std::string(tmp.begin() + j + 1, tmp.end())));
		respo.erase(respo.begin(), respo.begin() + i + 1);
	}
}

std::string	Cgi_request::find_location(std::string extension){
	std::vector<LocationConfig> locations = server.getLocation();
	std::vector<LocationConfig>::iterator it = locations.begin();
	for ( ; it < locations.end(); it++) {
		if (it->get_name() == extension)
			return it->get_path();
	}
	return std::string();
}
