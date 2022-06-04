#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <sstream>
#include <fstream>

class AutoIndex
{
	private:
		std::string			result_file;
		std::ofstream		file;

		std::string adapt_truncate(std::string str);
		std::string fixnl(char * time);
		AutoIndex(){}
	public:
		AutoIndex(std::string path);
		std::string getHtml() const;
};