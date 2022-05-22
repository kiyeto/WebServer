#include "Cgi_request.hpp"

std::string	Cgi_request::execute(std::string path){
	std::vector<std::string> args;
	std::string cmd("/Volumes/WIFISLAX64-/.brew/opt/php@7.4/bin/php-cgi");
	std::string response;
	std::vector<std::string> meta_vars;
	std::string start_line("HTTP/1.1");

	args.push_back(cmd); // Need To Be replaced With CMD
	args.push_back(root + path);
	const char *p[args.size() + 1];
	for(int i = 0; i < args.size(); i++)
		p[i] = args[i].c_str();
	p[args.size()] = NULL;
	meta_vars = set_envp();
	const char **penv = (const char**)meta_vars.begin().base();

	response = child_proce(p, (const char**)meta_vars.begin().base());
	parse_cgiResponse(response);
	std::map<std::string, std::string>::iterator it = headers.find("Status");
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

std::vector<std::string> Cgi_request::set_envp(){
	std::vector<std::string> ret;

	ret.push_back("REQUEST_METHOD=GET");
	ret.push_back("SERVER_PORT=8080");
	ret.push_back("SCRIPT_NAME=" + path);
	ret.push_back("SERVER_NAME=127.0.0.1");
	ret.push_back("PATH_INFO=wordpress");
	ret.push_back(std::string());

	return ret;
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