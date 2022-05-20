#include "Cgi_request.hpp"

std::string	Cgi_request::execute(std::string path){
	std::vector<std::string> args;
	std::string cmd("/goinfre/bamghoug/.brew/opt/php@7.4/bin/php-cgi");
	std::string content;

	args.push_back(cmd); // Need To Be replaced With CMD
	args.push_back(root + path);
	const char *p[args.size() + 1];
	for(int i = 0; i < args.size(); i++)
		p[i] = args[i].c_str();
	p[args.size()] = NULL;
	content = child_proce(p);
	return content;
}

std::string Cgi_request::child_proce(const char **cmd){
	int fds[2];
	pipe(fds);
	int child = fork();
	if (child == 0) //child Process
	{
		dup2(fds[1], 1);
		dup2(fds[1], 2);
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