#include <string>
#include <iostream>
#include <fstream>
#include "../Request/request.hpp"

bool	upload_file(std::string filename, std::string upload_dir)
{
	std::ifstream body(filename, std::ifstream::binary);
	std::ofstream file;
	std::string boundry;
	std::string line;
	std::string name;
	size_t pos;
	bool notfile = 0;
	bool up = 0;
 
	std::getline(body, boundry);
	if(!boundry.empty())
		boundry.erase(boundry.length() - 1);
	while (std::getline(body, line))
	{
		if (line.find(boundry + "--") != std::string::npos)
			return 1;
		if (line.find(boundry) != std::string::npos)
		{
			up = 0;
			notfile = 0;
			continue;
		}
		if (notfile)
			continue;
		if (!up)
		{
			pos = line.find("filename");
			if (pos != std::string::npos)
			{
				name = line.substr(pos + 10, line.length() - (pos + 10) - 2);
				std::remove((upload_dir + name).c_str());
				std::cout << name << std::endl;
				up = 1;
				std::getline(body, line);
				std::getline(body, line);
			}
			else
				notfile = 1;
		}
		else
		{
			file.open(upload_dir + name, std::ios_base::app | std::ios::binary);
			file.write(line.c_str(), line.length());
			file.write("\n", 1);
			file.close();
		}
	}
	return 0;
}