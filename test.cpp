#include <map>
#include <iostream>

int main()
{
	std::map<std::string, std::string> ret;
	ret[".hello"] = "wold";
	ret[".hes"] = "wold";
	ret["hellsasfo"] = "wold";
	ret["hesf"] = "wold";
	ret["helo"] = "wold";
	std::map<std::string, std::string>::iterator it = ret.begin();
	// while (it != ret.end())
	// {
	// 	std::cout << it->first << std::endl;
	// 	it++;
	// }
	return (0);
}