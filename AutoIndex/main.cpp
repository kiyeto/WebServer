#include "autoindex.hpp"

int	main(int argc, char ** argv)
{
	if (argc == 2)
	{
		std::string path = std::string(argv[1]);
		AutoIndex ai(path);
		std::cout << ai.getHtml();
	}
}
