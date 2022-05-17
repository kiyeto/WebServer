#include "Cgi_request.hpp"

std::string	Cgi_request::execute(std::string path){
	std::vector<std::string> args;
	std::string cmd("/goinfre/bamghoug/.brew/opt/php@7.4/bin/php-cgi");
	const char *ag[3];
	int fds[2];

	args.push_back(cmd); // Need To Be replaced With CMD
	args.push_back(path);
	const char *p[args.size() + 1];
	for(int i = 0; i < args.size(); i++)
		p[i] = args[i].c_str();
	p[args.size()] = NULL;
	ag[0] = cmd.c_str();
	ag[1] = path.c_str();
	ag[2] = NULL;
	meta("GET", 200);
	const char *env[meta_vars.size() + 1];
	for(int i = 0; i < meta_vars.size(); i++)
		p[i] = meta_vars[i].c_str();
	p[meta_vars.size()] = NULL;
	pipe(fds);
	int child = fork();
	if (child == 0) //child Process
	{
		dup2(fds[1], 1);
		dup2(fds[1], 2);
		close(fds[0]);
		execve(cmd.c_str(), (char *const *)p, (char *const *)envp);
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