/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 15:27:31 by mbrija            #+#    #+#             */
/*   Updated: 2022/04/15 15:27:31 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile_src/ConfigfileClass.hpp"
#include "configfile_src/LocationConfig.hpp"
#include "configfile_src/ServerConfig.hpp"


int main(int ac, char **av)
{
    ConfigfileClass configFile;

    if (ac > 2)
    {
        std::cout << " bad parameters \n Usage: ./webserv configfile_path" << std::endl;
        return -1;
    }
    if (ac == 2)
        configFile = ConfigfileClass(std::string(av[1]));
    try
    {
        configFile.configfileparser();
        std::vector<ServerConfig> vc ;
        vc = configFile.getServerConfig();
        for (auto i: vc)
            std::cout << i << ' ';
        std::cout << "read successfully \n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Config File Invalid Syntax : " << e.what() << std::endl;
        return -1;
    }

}