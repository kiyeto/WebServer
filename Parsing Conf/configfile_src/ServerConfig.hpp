/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 00:29:22 by mbrija            #+#    #+#             */
/*   Updated: 2022/04/22 12:26:09 by mbrija           ###   ########.fr       */
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
    std::string name;
    std::string root;
    std::string redirect;
    std::string index;
    std::string log;
    std::string error;
    size_t body_size;
    size_t location_count;
    std::vector<LocationConfig> location; // location type edit later
    
    
public:
    ServerConfig(/* args */);
    ServerConfig(ServerConfig const &p);
    ServerConfig operator=(ServerConfig const &p);
    ~ServerConfig();

    //getters 
    size_t get_port();
    std::string get_name();
    std::string get_root();
    std::string get_redirect();
    std::string get_index();
    std::string get_log();
    std::string get_error();
    size_t get_body_size();
    size_t get_location_count();
    std::vector<LocationConfig> getLocation();
};

