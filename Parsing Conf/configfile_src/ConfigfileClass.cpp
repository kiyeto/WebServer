/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigfileClass.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 15:42:42 by mbrija            #+#    #+#             */
/*   Updated: 2022/04/15 15:42:42 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigfileClass.hpp"

ConfigfileClass::ConfigfileClass(/* args */) : configFile("../config/conf"), serverCount(0), serverConf(0)
{
}

ConfigfileClass::~ConfigfileClass()
{
}

ConfigfileClass::ConfigfileClass(std::string const & conf)
{
    this->configFile = conf;

}

ConfigfileClass::ConfigfileClass( ConfigfileClass const &p )
{
    *this = p;
}

ConfigfileClass &ConfigfileClass::operator=(const ConfigfileClass & p)
{
    if (this != &p)
    {
        this->configFile = p.configFile;
        this->serverConf = p.serverConf;
        this->serverCount = p.serverCount;
    }
    return *this;
}

std::string ConfigfileClass::getConfigfile()
{
    return this->configFile;
}

size_t ConfigfileClass::getServerCount()
{
    return this->serverCount;
}

std::vector<ServerConfig> ConfigfileClass::getServerConfig()
{
    return this->serverConf;
}

void ConfigfileClass::setServer()
{
    std::ifstream   file(this->configFile);
    std::string     buf;
    size_t          n = 0;
    
    /* get line from file and trim buff*/
    /*make the trim function, they don't have it ffs*/
    /* check for server count and throw exception */
    /*push to vector*/
    while (getline(file, buf))
    {
        buf = trim(buf, " ");
        if (buf == "server {")
            n++;
    }
    if (n > 0)
        this->serverCount = n;
    else
        throw Error_exc("Server not found");
    for (size_t i = 0; i < this->serverCount; i++)
    {
        this->serverConf.push_back(ServerConfig());
    }
    

}

void ConfigfileClass::setLocation()
{
    /*to do*/
    std::ifstream file(this->configFile);
    std::string buf;
    size_t n_servers = 0;
    size_t n_locations = 0;

    while (getline(file, buf))
    {
        buf = trim(buf, " ");
        if (buf == "server {")
        {
            while (getline(file, buf))
            {
                buf = trim(buf, " ");
                if (buf.find("}") != std::string::npos && buf != "}")
                    throw Error_exc("Synatax Err");
                if (buf == "}")
                {
                    if (n_locations > 0)
                       this->serverConf[n_servers].location_count = n_locations;
                    else
                        throw Error_exc("Invalid Location");
                    for (size_t i = 0; i < n_locations; i++)
                    {
                        this->serverConf[n_servers].location.push_back(LocationConfig());
                    }
                    n_servers++;
                    n_locations = 0;
                    break;
                }
                else if (buf == "location = [")
                    n_locations++;
            }
        }
    }
}

void ConfigfileClass::configfileparser()
{
    std::ifstream file(this->configFile);
    std::string   buf;
    size_t n_servers = 0;
    this->setServer();
    this->setLocation();

    while(getline(file, buf))
    {
        buf = trim(buf, " ");
        if (buf != "server {")
        {
            if ((buf[0] == '#' && buf.find_first_of("#") == 0) || buf.empty())
                continue;
            else
                throw Error_exc("Invalid Syntax");
        }
        else
        {
            size_t n_locations = 0;
            bool is_port = false, is_bodysize = false;

            while (getline(file, buf))
            {
                buf = trim(buf, " ");
                if (buf == "}")
                    break;
                /*now how to make this shit work*/
                switch (buf[0])
                {
                    case 'l' :
                    if (is_port == false && std::strncmp("listen = ", buf.c_str(), 9) == 0)
                    {
                        this->serverConf[n_servers].port = std::stoi(buf.substr(buf.find("listen = ") + strlen("listen = ")));
                        is_port = true;
                        break;
                    }
                    else if (std::strncmp("location = [", buf.c_str(), 12) == 0)
                    {
                        while (getline(file, buf))
                        {
                            buf = trim(buf, " ");
                            if (buf == "]")
                                break;
                            this->serverConf[n_servers].location[n_locations].locationParser(buf);
                        }
                        n_locations++;
                        break;
                    } 
                    throw Error_exc("Invalid Sytax : Location / port");

                case 'h':
                    if (this->serverConf[n_servers].host.empty() && std::strncmp("host = ", buf.c_str(), 7) == 0)
                    {
                        this->serverConf[n_servers].host = buf.substr(buf.find("host = ") + strlen("host = "));
                        break;
                    }
                    throw  Error_exc("Inavalid Syntax : host");

                case 'a':
                    if (this->serverConf[n_servers].log.empty() && std::strncmp("access_log = ", buf.c_str(), 13) == 0)
                    {
                        this->serverConf[n_servers].log = buf.substr(buf.find("access_log = ") + strlen("access)log = "));
                        break;
                    }
                    throw  Error_exc("Inavalid Syntax : log");

                case 'b' :
                    if (is_bodysize == false && std::strncmp("body_size_limit = ", buf.c_str(), 18) == 0)
                    {
                        this->serverConf[n_servers].body_size = std::stoul(buf.substr(buf.find("body_size_limit = ") + strlen("body_size_limit = "))) * 1048576;
                        if (this->serverConf[n_servers].body_size < 0)
                            throw Error_exc("body_size limit can't be negative");
                        else
                            break;
                    }
                        throw Error_exc("Invalid Syntax : BodySizeLimit");
                //to be fixed
                case 'e':
                    if (this->serverConf[n_servers].error_pages.empty() && std::strncmp("error_page = ", buf.c_str(), 13) == 0)
                    {
                        std::string error = buf.substr(buf.find("error_page = ") + strlen("error_page = "));
                        std::vector<std::string> tmp = split(error, ';');
                        for (size_t i = 0; i < tmp.size(); i++)
                        {
                            int key;
                            std::string value;
                            std::vector<std::string> tmp2 = split(tmp[i], ':');
                            if (tmp2.size() != 2)
                                throw Error_exc("Invalid Syntax : Error Pages must be Written in the format : error_page = 404: /404.html ; 500: /500.html ...");
                            key = std::stoi(tmp2[0]);
                            value = tmp2[1];
                            this->serverConf[n_servers].error_pages.insert(std::pair<int, std::string>(key, value));
                        }
                        break;
                    }
                    throw Error_exc("Invalid Syntax : Error page");        

                case 's' :
                    if (this->serverConf[n_servers].name.empty() && std::strncmp("server_name = ", buf.c_str(), 14) == 0)
                    {
                        this->serverConf[n_servers].name = buf.substr(buf.find("server_name = ") + strlen("server_name = "));
                        break;
                    }
                    throw Error_exc("Invalid Syntax : server name");

                case 'r':
                    if (this->serverConf[n_servers].root.empty() && std::strncmp("root = ", buf.c_str(), 7) == 0)
                    {
                        this->serverConf[n_servers].root = buf.substr(buf.find("root = ") + strlen("root = "));
                        break;
                    }
                    throw Error_exc("Invalid Syntax : Root");

                case 'i':
                    if (this->serverConf[n_servers].index.empty() && std::strncmp("index = ", buf.c_str(), 8) == 0)
                    {
                        this->serverConf[n_servers].index = buf.substr(buf.find("index = ") + strlen("index = "));
                        break;
                    }
                    throw Error_exc("Invalid Syntax : index");

                default:
                    if (buf.empty())
                        break;
                    throw Error_exc("Invalid Syntax : Config File");
                }
            }
        n_servers++;
        }
    }
}