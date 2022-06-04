/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 00:29:56 by mbrija            #+#    #+#             */
/*   Updated: 2022/06/03 23:45:23 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"


ServerConfig::ServerConfig(/* args */) :
    port(-1), name(""), root(""), redirect(""),
    index(""), log(""), error_pages(), body_size(100000000000),
    location_count(0), location(0)
{
}

ServerConfig::ServerConfig(ServerConfig const &p)
{
    *this = p;
}

ServerConfig & ServerConfig::operator=(ServerConfig const &p)
{
    if(this != &p)
    {
        this->body_size = p.body_size;
        this->error_pages = p.error_pages;
        this->index = p.index;
        this->location = p.location;
        this->location_count = p.location_count;
        this->log = p.log;
        this->name = p.name;
        this->port = p.port;
        this->redirect = p.redirect;
        this->root = p.root;
        this->host = p.host;
    }
    return *this;
}

ServerConfig::~ServerConfig()
{
}

size_t ServerConfig::get_port()
{
    return this->port;
}
std::string ServerConfig::get_name()
{
    return this->name;
}
std::string ServerConfig::get_root()
{
    return this->root;
}
std::string ServerConfig::get_redirect()
{
    return this->redirect;   
}
std::string ServerConfig::get_index()
{
    return this->index;
}
std::string ServerConfig::get_log()
{
    return this->log;   
}
std::map<int, std::string> ServerConfig::get_error_pages()
{
    return this->error_pages;
}
size_t ServerConfig::get_body_size()
{
    return this->body_size;
}
size_t ServerConfig::get_location_count()
{
    return this->location_count;
}
std::vector<LocationConfig> ServerConfig::getLocation()
{
    return this->location;    
}
std::string ServerConfig::get_host()
{
    return this->host;
}
