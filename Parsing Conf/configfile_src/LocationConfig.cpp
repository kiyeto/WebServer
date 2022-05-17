/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 00:33:32 by mbrija            #+#    #+#             */
/*   Updated: 2022/05/03 18:01:51 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "LocationConfig.hpp"

LocationConfig::LocationConfig(/* args */) : 
    path(""), root(""), auto_index(0), cgi(),
    upload(""), redirect(""), methods()
{
}

LocationConfig::LocationConfig(LocationConfig const & p)
{
    *this = p;
}

LocationConfig LocationConfig::operator= (LocationConfig const &p)
{
    if (!this)
    {
        this->auto_index = p.auto_index;
        this->cgi = p.cgi;
        this->methods = p.methods;
        this->path = p.path;
        this->redirect = p.redirect;
        this->root = p.root;
        this->upload = p.upload;
    }
    return *this;
}

LocationConfig::~LocationConfig()
{
}

std::string LocationConfig::get_path()
{
    return this->path;
}
std::string LocationConfig::get_root()
{
    return this->root;
}

std::string LocationConfig::get_redirect()
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
            if(buf[0] == 'a')
            {
                if (processed == false && std::strncmp("autoindex = ", buf.c_str(), 12) == 0)
                {
                    if (std::strncmp("autoindex = on", buf.c_str(), 14 == 0))
                        this->auto_index = 1;
                    else if (std::strncmp("autoindex = off", buf.c_str(), 15 == 0))
                        this->auto_index = 0;
                }
                else
                    throw Error_exc("syntax err : invalid autoindex");
            }
            else if(buf[0] == 'p')
            {
                
                if(this->path.empty() && std::strncmp("path = ", buf.c_str(), 7) == 0)
                {
                    this->path = buf.substr(buf.find("path = ") + strlen("path = "));
                    if (this->path[this->path.size() - 1] != '/')
                        this->path += '/';
                }
                else
                    throw Error_exc("syntax err : invalid path");
            }
            else if(buf[0] == 'r')
            {
                if(this->root.empty() && std::strncmp("root = ", buf.c_str(), 7) == 0)
                {
                    this->root = buf.substr(buf.find("root = ") + strlen("root = "));
                    if (this->root[this->root.size() - 1] != '/')
                        this->root.resize(this->root.size() - 1);
                }
                else if (this->redirect.empty() && std::strncmp("redirect = ", buf.c_str(), 11) == 0)
                    this->redirect = buf.substr(buf.find("redirect = ") + strlen("redirect ="));
                else
                    throw Error_exc("syntax err : invalid root/redirect");
            }
           else if(buf[0] == 'u')
           {
               if (this->upload.empty() && std::strncmp("upload = ", buf.c_str(), 9) == 0)
               {
                    this->upload = buf.substr(buf.find("upload = " + strlen("upload = ")));
                    if (this->upload[this->upload.size() - 1] == '/')
                        this->upload.resize(this->upload.size() -1);
               }
               else
                 throw Error_exc("syntax err : invalid upload");
           }
           else if(buf[0] == 'c')
           {
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
               }
               else
                 throw Error_exc("syntax err : invalid cgi");
           }
            else if(buf[0] == 'm')
           {
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
               }
               else
                 throw Error_exc("syntax err : invalid method");
           }
            else
                throw Error_exc("syntax err : invalid location");
    }
}