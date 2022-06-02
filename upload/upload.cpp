#include <string>
#include <iostream>
#include <fstream>
#include "request.hpp"

bool	upload_file(std::string filename)
{
	std::ifstream body(filename);
	std::string boundry;
	std::string process;
	char buffer[100];

   body.open(filename, ios::in | ios::binary);
    myFile.read(buffer, 4096);
	if (myFile)
	{
		process = std::string(buffer, 4096);
		int pos = process.find("\r\n");
		if (pos != -1)
		{
			boundry = treat.substr(0, pos);
			process.erase(0, po + 2);
			pos = find("filename");
			if (pos != -1)
			{
				
			}
		}
	}
}