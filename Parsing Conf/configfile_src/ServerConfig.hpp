/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 00:29:22 by mbrija            #+#    #+#             */
/*   Updated: 2022/06/04 15:03:28 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../global_src/support_funcs.hpp"
# include "LocationConfig.hpp"

class ServerConfig
{
friend class ConfigfileClass;
private:
    size_t port;
    std::string host;
    std::string name;
    std::string root;
    // std::vector<std::string> redirect;
    std::string index;
    std::string log;
    std::map<int, std::string> error_pages;
    size_t body_size;
    size_t location_count;
    std::vector<LocationConfig> location; // location type edit later
    
    
public:
    ServerConfig(/* args */);
    ServerConfig(ServerConfig const &p);
    ServerConfig & operator=(ServerConfig const &p);
    ~ServerConfig();
    
friend std::ostream &operator<<(std::ostream &output, ServerConfig &D ) 
{ 
        output << "name : " << D.get_name() << std::endl;
        output << "host : " << D.get_host() << std::endl;
        output << "port : " << D.get_port() << std::endl;
        output << "root : " << D.get_root() << std::endl;
        output << "index : " << D.get_index() << std::endl;
        output << "log : " << D.get_log() << std::endl;
        output << "limit size : " << D.get_body_size() << std::endl;
        output << "location count : " << D.get_location_count() << std::endl;
        for (auto i:  D.get_error_pages())
            std::cout << "Error page : " << "key : " << i.first << " | Value :" << i.second << ' ' << std::endl;
        for (auto i: D.getLocation())
            std::cout << "locations \n " << i << ' ';
        return output;
}
    //getters 
    std::string get_name();
    std::string get_host();
    size_t get_port();
    std::string get_root();
    std::string get_index();
    std::string get_log();
    std::map<int, std::string> get_error_pages();
    size_t get_body_size();
    size_t get_location_count();
    std::vector<LocationConfig> getLocation();
};

