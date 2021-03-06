#include "autoindex.hpp"

std::string AutoIndex::adapt_truncate(std::string str)
{
	if (str.length() > 42)
		return str.substr(0, 42) + "...";
	return str;
}

std::string AutoIndex::fixnl(char * time)
{
	std::string t = std::string(time);
	t.erase(t.length() - 1);
	return t;
}

AutoIndex::AutoIndex(std::string p, std::string uri)
{
	DIR *dir = opendir(p.c_str());
	struct stat result;
	std::string directories;
	std::string f;
	result_file = "<!DOCTYPE html><html><head><title>AutoIndex</title></head><body><style>a.file{background: url(/img/file.png) left top no-repeat;}a.dir{background: url(/img/directory.png) left top no-repeat;} a.icon{-webkit-padding-start: 1.5em;text-decoration: none;user-select: auto;}</style><h1>Index of " + uri + "</h1><hr><table style=\"width:100%\">";
	if (dir)
	{
		struct dirent* entity;
		entity = readdir(dir);
		while (entity)
		{
			std::string name = std::string(entity->d_name);
			std::string path = p + "/" + name;
			if (name[0] == '.' && name != "..")
			{
				entity = readdir(dir);
				continue;
			}
			if (name == "..")
			{
				// dirs << name + "/" << std::endl;
				directories += "<tr><td style=\"width:50%\"><a class=\"icon dir\" href=" + name + '/' + "> " + adapt_truncate(name) + '/' + " </a>"+ "</td><td style=\"width:20%\"></td><td style=\"width:20%\"></td   style=\"width:10%\"><td></td></tr>";
				entity = readdir(dir);
				continue;
			}
			if(stat(path.c_str(), &result) == 0)
			{
				if ((result.st_mode & S_IFMT) == S_IFDIR)
				{
					// dirs << std::left << std::setfill(' ') <<  std::setw(25) << adapt_truncate("[D] " + name) + '/' << "\t\t" << fixnl(std::ctime(&result.st_mtime)) << "\t\t-" << std::endl;
					directories += "<tr><td><a class=\"icon dir\" href=" + name + '/' + "> " + adapt_truncate(name) + '/' + " </a> </td>\n<td align=\"right\">" + fixnl(std::ctime(&result.st_mtime)) + "\n<td align=\"right\">" + "-" + "</td></tr>";
				}
				else if( result.st_mode & S_IFREG )
				{
					f += "<tr><td><a class=\"icon file\" href=" + name + "> " + adapt_truncate(name) + " </a></td><td align=\"right\">" + fixnl(std::ctime(&result.st_mtime)) + "</td><td align=\"right\">" + std::to_string(result.st_size) + "</td></tr>";
					// files << std::left << std::setfill(' ') <<  std::setw(25) << adapt_truncate("[F] " + name) << "\t\t" << fixnl(std::ctime(&result.st_mtime)) << "\t\t" << result.st_size << std::endl;
				}
			}
			entity = readdir(dir);
		}
		result_file += directories + f + "</table><hr></body></html>";
		file.open("html.html");
		file << result_file;
		file.close();
		closedir(dir);
		// std::cout << dirs.str() << files.str();
	}
}

std::string AutoIndex::getHtml() const
{
    return result_file;
}
