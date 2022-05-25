/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 00:32:50 by mbrija            #+#    #+#             */
/*   Updated: 2022/05/25 13:07:39 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "../global_src/support_funcs.hpp"

class LocationConfig
{
private:
    std::string name;
    std::string path;
    std::string root;
    std::string redirect;
    std::string upload;
    std::vector<std::string> methods;
    std::vector<std::string> cgi;
    bool auto_index;
    
public:
    LocationConfig(/* args */);
    LocationConfig(LocationConfig const & p);
    LocationConfig & operator= (LocationConfig const &p);
    ~LocationConfig();

    //getters
    std::string get_name();
    std::string get_path();
    std::string get_root();
    std::string get_redirect();
    std::string get_upload();
    std::vector<std::string> getMethods();
    std::vector<std::string> getCgi();

    //parse here
    void locationParser(std::string buf);

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

