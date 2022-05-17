#include "Cgi_request.hpp"

void	Cgi_request::execute(std::string &path){
	std::vector<std::string> args;
	std::string cmd("/goinfre/bamghoug/.brew/opt/php@7.4/bin/php-cgi");
	int fds[2];

	args.push_back(cmd); // Need To Be replaced With CMD
	args.push_back(path);
	pipe(fds);
	meta("GET", 200);
	int child = fork();
	if (child == 0) //child Process
	{
		std::cout << "CHild " << path << std::endl;
		dup2(fds[1], 1);
		close(fds[0]);
		if (execve(cmd.c_str(), (char *const *)args.begin().base(), NULL) < 0){
			std::cout << "Child Error" << std::endl;
			exit(1);
		}
	}
	wait(NULL);
	char buf[1025];
	memset(buf, 0, 1025);
	std::string content;
	while(read(fds[1], buf, 1024) > 0)
	{
		memset(buf, 0, 1025);
		content += buf;
	}
	std::cout << "Parent content = " << content << std::endl;
}