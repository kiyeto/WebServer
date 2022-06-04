/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 00:33:32 by mbrija            #+#    #+#             */
/*   Updated: 2022/06/04 13:31:26 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "LocationConfig.hpp"

LocationConfig::LocationConfig(/* args */) : 
    name(""), location_index(""), root(""), redirect(),
    upload(""), methods(), cgi(), auto_index(0)
{
}

LocationConfig::LocationConfig(LocationConfig const & p)
{
    *this = p;
}

LocationConfig & LocationConfig::operator= (LocationConfig const &p)
{
    {
        this->name = p.name;
        this->cgi = p.cgi;
        this->methods = p.methods;
        this->location_index = p.location_index;
        this->redirect = p.redirect;
        this->root = p.root;
        this->upload = p.upload;
    }
    return *this;
}

LocationConfig::~LocationConfig()
{
}

std::string LocationConfig::get_name()
{
    return this->name;
}

std::string LocationConfig::get_location_index()
{
    return this->location_index;
}
std::string LocationConfig::get_root()
{
    return this->root;
}

std::vector<std::string> LocationConfig::get_redirect()
{
    return this->redirect;
}

std::string LocationConfig::get_upload()
{
    return this->upload;    
}

std::vector<std::string> LocationConfig::getMethods()
{
    return this->methods;
}
std::vector<std::string> LocationConfig::getCgi()
{
    return this->cgi;
}

void LocationConfig::locationParser(std::string buf)
{
    bool processed = false;
    if(!buf.empty())
    {
        switch(buf[0])
        {
            case 'a' :
            if (processed == false && std::strncmp("autoindex = ", buf.c_str(), 12) == 0)
            {
                if (std::strncmp("autoindex = on", buf.c_str(), 14) == 0)
                    this->auto_index = 1;
                else if (std::strncmp("autoindex = off", buf.c_str(), 15) == 0)
                    this->auto_index = 0;
                break;
            }
            throw Error_exc("syntax err : invalid autoindex");
            
            case 'l' :            
            if(this->location_index.empty() && std::strncmp("location_index = ", buf.c_str(), 17) == 0)
            {
                this->location_index = buf.substr(buf.find("location_index = ") + strlen("location_index = "));
                break;
            }
            throw Error_exc("syntax err : invalid location index");
            
        case 'r':
            if(this->root.empty() && std::strncmp("root = ", buf.c_str(), 7) == 0)
            {
                this->root = buf.substr(buf.find("root = ") + strlen("root = "));
                break;
            }
             else if (this->redirect.empty() && std::strncmp("redirect = ", buf.c_str(), 11) == 0)
            {
                std::string str = buf.substr(buf.find("redirect = ") + strlen("redirect = "));
                std::vector<std::string> tmp = split(str, ' ');
                if (tmp.size() != 2)
                    throw Error_exc("Invalid Syntax : redirect not valid");
                for (size_t i = 0; i < tmp.size(); i++)
                {
                    tmp[i] = trim(tmp[i], " ");
                    this->redirect.push_back(tmp[i]);
                }
                break;
            }
            throw Error_exc("syntax err : invalid root/redirect");
            
        case 'u':
            if (this->upload.empty() && std::strncmp("upload = ", buf.c_str(), 9) == 0)
            {
                this->upload = buf.substr(buf.find("upload = ") + strlen("upload = "));
                break;
            }
            throw Error_exc("syntax err : invalid upload");
            
        case 'n':
            if (this->name.empty() && std::strncmp("name = ", buf.c_str(), 7) == 0)
            {
                this->name = buf.substr(buf.find("name = ") + strlen("name = "));
                break;
            }
            throw Error_exc("syntax err : invalid name");
                
        case 'c':
            if (this->cgi.empty() && std::strncmp("cgi = ", buf.c_str(), 6) == 0)
            {   
                std::string str = buf.substr(buf.find("cgi = ") + strlen("cgi = "));
                std::vector<std::string> tmp = split(str, ' ');
                for (size_t i = 0; i < tmp.size(); i++)
                {
                    tmp[i] = trim(tmp[i], " ");
                    this->cgi.push_back(tmp[i]);
                }
                if (cgi.size() > 2)
                    throw Error_exc("invalid cgi virtual path");
                break;
            }
            throw Error_exc("syntax err : invalid cgi");

        case 'm':
            if (this->methods.empty() && std::strncmp("method = ", buf.c_str(), 9) == 0)
            {   
                std::string str = buf.substr(buf.find("method = ") + strlen("method = "));
                std::vector<std::string> tmp = split(str, ' ');
                for (size_t i = 0; i < tmp.size(); i++)
                {
                    tmp[i] = trim(tmp[i], " ");
                    this->methods.push_back(tmp[i]);
                }
                if (methods.size() > 3)
                    throw Error_exc("invalid method input");
                break;
            }
                throw Error_exc("syntax err : invalid method");
                
        default:
            if (buf.empty())
                break;
            throw Error_exc("syntax err : invalid location");
        }
    }
    return;
}