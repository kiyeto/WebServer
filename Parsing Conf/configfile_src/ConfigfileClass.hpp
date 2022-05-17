/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigfileClass.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 15:43:24 by mbrija            #+#    #+#             */
/*   Updated: 2022/04/15 15:43:24 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "../global_src/support_funcs.hpp"
# include "ServerConfig.hpp"

class ConfigfileClass
{
    
private:
    std::string		configFile;
	size_t			serverCount;
	std::vector<ServerConfig>	serverConf; //vector of servers

public:
    ConfigfileClass(/* args */);
    ~ConfigfileClass();
    ConfigfileClass( std::string const & conf );
	ConfigfileClass( ConfigfileClass const &p );
	ConfigfileClass & operator=( const ConfigfileClass & p);

    //getters & setters here
    std::string getConfigfile();
    size_t getServerCount();
    std::vector<ServerConfig> getServerConfig();
    //server_conf  
    
    void setServer();
    void setLocation();

    //parse here ...
    void configfileparser();

    class Error_exc : public std::exception {
        private:
            std::string error_msg;
        public:
            explicit Error_exc( const std::string &error_msg ) throw() : error_msg(error_msg) {}
            ~Error_exc() throw() {}

            virtual const char * what() const throw() {
                return (error_msg.c_str());
            }
        };
};

