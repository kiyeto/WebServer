#include <string>
#include <iostream>
#include <fstream>
#include "../V2/request.hpp"

// bool	upload_file(std::string filename)
// {
// 	std::ifstream body(filename, std::ifstream::binary);
// 	std::ofstream file;
// 	std::string boundry;
// 	std::string process;
// 	std::string line;
// 	std::string name;
// 	char buffer[4096];
// 	int start;
// 	bool f = 0;
// 	bool end = 0;
 
//  	while(!end)
// 	{
// 		body.read(buffer, 4096);
// 		if (body)
// 		{
// 			process = std::string(buffer, 4096);
// 			if (!boundry.length())
// 			{
// 				if (process.find("\r\n") != -1)
// 					boundry = process.substr(0, pos);
// 			}
// 			if (!f)
// 			{
// 				line = process.substr(0, process.find("\r\n"));
// 				if (line == boundry)
// 				process.erase(0, pos + 2);
			
// 				pos = process.find("\r\n");
// 				if (pos != -1)
// 				{
// 					int end = pos;
// 					line = process.substr(0, pos);
// 					pos = process.find("filename");
// 					if (pos != -1)
// 					{
// 						name = process.substr(pos + 10,  line.length() - (pos + 10) - 1);
// 						process.erase(0, pos + 2);
// 						pos = process.find("\r\n\r\n");0
// 						if (pos != -1)
// 						{
// 							process.erase(0, pos + 4);
// 							if (process.find(boundry) == -1 || process.find(boundry + "--") == -1)
// 							{
// 								file.open(name, std::ios_base::app | std::ios::binary);
// 								file.write(process.c_str(), process.length());				
// 								file.close();
// 							}
// 							else
// 							{
// 								pos = process.find(boundry);
// 								int diff = process.length() - pos;
// 								file.open(name, std::ios_base::app | std::ios::binary);
// 								file.write(process.c_str(), diff);
// 								file.close();
// 							}
// 						}
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if ()
// 			}
// 		}
// 	}
// 	return 0;
// }

bool	upload_file(std::string filename)
{
	std::ifstream body(filename, std::ifstream::binary);
	std::ofstream file;
	std::string boundry;
	std::string line;
	std::string name;
	int pos;
	bool notfile = 0;
	bool up = 0;
 
	std::getline(body, boundry);
	boundry.erase(boundry.length() - 1);
	// std::cout << boundry << "boundry len : " << boundry.length() << std::endl;
	while (std::getline(body, line))
	{
		if (line.find(boundry + "--") != -1)
		{
			std::cout << line << std::endl;
			return 1;
		}
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
				std::remove(name.c_str());
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
			file.open(name, std::ios_base::app | std::ios::binary);
			file.write(line.c_str(), line.length());
			file.write("\n", 1);
			file.close();
		}
	}
   
 
	return 0;
}

int main ()
{
	upload_file("example3");
}
