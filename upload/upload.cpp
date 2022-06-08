#include <string>
#include <iostream>
#include <fstream>
#include "../V2/request.hpp"

bool	upload_file(std::string filename, std::string upload_dir)
{
	std::ifstream body(filename, std::ifstream::binary);
	std::ofstream file;
	std::string boundry;
	std::string line;
	std::string name;
	int pos;
	bool notfile = 0;
	bool up = 0;
 
	std::cout << boundry << "boundry len : " << upload_dir + "/" + filename << std::endl;
	std::getline(body, boundry);
	boundry.erase(boundry.length() - 1);
	while (std::getline(body, line))
	{
		if (line.find(boundry + "--") != -1)
			return 1;
		// {
			// std::cout << line << std::endl;
		// }
		if (line.find(boundry) != -1)
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
			if (pos != -1)
			{
				name = line.substr(pos + 10, line.length() - (pos + 10) - 2);
				std::remove((upload_dir + name).c_str());
				std::cout << name << std::endl;
				up = 1;
				// std::getline(body, line);
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