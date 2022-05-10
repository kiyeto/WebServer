#include "Cgi_request.hpp"

void	Cgi_request::execute(std::string &path){
	int child = fork();

	char **args = (char**) malloc(sizeof(char*) * 2);
	char **envp = (char**) malloc(sizeof(char*) * 3);
	args[0] = "/goinfre/bamghoug/.brew/opt/php@7.4/bin/php-cgi";
	args[1] = "wordpress/index.php";
	if (child == 0) //child Process
	{
		// std::cout << "CHild " << path << std::endl;
		execve("/goinfre/bamghoug/.brew/opt/php@7.4/bin/php-cgi", args, NULL);
		exit(0);
	}
	wait(NULL);
}