#include "Cgi_request.hpp"

Cgi_request::Cgi_request(request &req): path(), query_str(), path_info(), root(getenv("PWD")), body(), headers(), pr(req.getUri()) 
{
	meta.insert(std::make_pair(std::string("GATEWAY_INTERFACE="), std::string("CGI/1.1")));
	meta.insert(std::make_pair(std::string("PATH_INFO="), std::string(root + req.getUri())));
	meta.insert(std::make_pair(std::string("QUERY_STRING="), std::string(pr.query_string)));
	meta.insert(std::make_pair(std::string("REQUEST_METHOD="), std::string(req.getMethod())));
	if (pr.path == "/")
		meta.insert(std::make_pair(std::string("SCRIPT_NAME="), std::string(root + "/index.php")));
	else
		meta.insert(std::make_pair(std::string("SCRIPT_NAME="), std::string(root + pr.path)));
	meta.insert(std::make_pair(std::string("SERVER_NAME="), std::string("127.0.0.1")));
	meta.insert(std::make_pair(std::string("SERVER_PORT="), std::string("8080")));
	meta.insert(std::make_pair(std::string("SERVER_PROTOCOL="), std::string("HTTP/1.1")));
	if (req.getBodySize())
	{
		meta.insert(std::make_pair(std::string("CONTENT_LENGTH="), std::string( req.getHeaders().find("Content-Length")->second )));
		meta.insert(std::make_pair(std::string("CONTENT_TYPE="), std::string( req.getHeaders().find("Content-Type")->second )));
	}
	// meta.insert(std::make_pair(std::string(""), std::string()));

}

std::string	Cgi_request::execute(){
	std::vector<std::string> args;
	std::string cmd("/Volumes/WIFISLAX64-/.brew/opt/php@7.4/bin/php-cgi");
	std::string response;
	const char *meta_vars[meta.size() + 1];
	std::string start_line("HTTP/1.1");

	args.push_back(cmd); // Need To Be replaced With CMD
	args.push_back(meta.find("SCRIPT_NAME=")->second);

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
		start_line += it->second + "\n";
		start_line += response;
		headers.clear();
		return start_line;
	}
	start_line += " 200 OK\n";
	start_line += response;
	headers.clear();
	return start_line;
}

std::string Cgi_request::child_proce(const char **cmd, const char **envp){
	int i = -1;
	while(envp[++i])
		std::cout << envp[i] << std::endl;
	int fds[2];
	pipe(fds);
	int child = fork();
	if (child == 0) //child Process
	{
		dup2(fds[1], 1);
		close(fds[0]);
		execve(cmd[0], (char *const *)cmd, (char *const *)NULL);
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

	if ((i = respo.find("\r\n\r\n")) != std::string::npos)
	{
		body = respo.begin().base() + i + 4;
		respo.erase(respo.begin() + i, respo.end());
	}
	while ((i = respo.find("\n")) != std::string::npos)
	{
		std::string tmp(respo.begin(), respo.begin() + i);
		j = tmp.find(":");
		if (j != std::string::npos)
			headers.insert(std::make_pair(std::string(tmp.begin(), tmp.begin() + j), std::string(tmp.begin() + j + 1, tmp.end())));
		respo.erase(respo.begin(), respo.begin() + i + 1);
	}
}
