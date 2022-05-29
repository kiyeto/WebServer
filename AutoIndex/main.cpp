#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <sstream>
#include <fstream>

std::string fixnl(char * time)
{
	std::string t = std::string(time);
	t.erase(t.length() - 1);
	return t;
}

int	main(int argc, char ** argv)
{
	std::stringstream	dirs;
	std::stringstream	files;
	std::ofstream		ofs;

	if (argc != 2)
	{
		std::cout << "bad args !" << std::endl;
		return 0;
	}
	struct stat p;
	if (stat(argv[1], &p) == 0)
	{
		if ((p.st_mode & S_IFMT) != S_IFDIR)
		{
			
		}	
	}

	DIR *dir = opendir(argv[1]);
	struct stat result;
	if (dir)
	{
		struct dirent* entity;
		entity = readdir(dir);
		while (entity)
		{
			std::string name = std::string(entity->d_name);
			std::string path = std::string(argv[1]) + "/" + name;
			if (name[0] == '.' && name != "..")
			{
				entity = readdir(dir);
				continue;
			}
			if (name == "..")
			{
				dirs << name + "/" << std::endl;
				entity = readdir(dir);
				continue;
			}
			if(stat(path.c_str(), &result) == 0)
			{
				if ((result.st_mode & S_IFMT) == S_IFDIR)
					dirs << "[D] " << name << "/\t\t" << fixnl(std::ctime(&result.st_mtime)) << "\t\t-" << std::endl;
				else if( result.st_mode & S_IFREG )
					files << "[F] " << name << "\t\t" << fixnl(std::ctime(&result.st_mtime)) << "\t\t" << result.st_size << std::endl;
			}
			entity = readdir(dir);
		}
		closedir(dir);
		std::cout << dirs.str() << files.str();
	}

}
