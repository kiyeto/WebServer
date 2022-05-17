/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_funcs.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 10:49:47 by mbrija            #+#    #+#             */
/*   Updated: 2022/04/18 10:49:47 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "support_funcs.hpp"

std::string ltrim(const std::string &str, const std::string set)
{
    size_t start = str.find_first_not_of(set);
    if (start == std::string::npos)
        return ("");
    else
        return (str.substr(start));
}

std::string rtrim(const std::string &str, const std::string set)
{
    size_t end = str.find_last_not_of(set);
    if (end == std::string::npos)
        return ("");
    else
        return (str.substr(0, end + 1));
}

std::string trim(const std::string &str, const std::string set)
{
    return rtrim(ltrim(str, set), set);
}

std::vector<std::string> split(std::string str, char sep)
{
    std::vector<std::string> splitted;
    std::string subed  = "";
    size_t start = 0;
    size_t end = 0;
    size_t i = 0;

    while (i <= str.length())
    {
        if (str[i] == sep || i == str.length())
        {
            end = i;
            subed.append(str, start, end - start);
            splitted.push_back(subed);
            start = end + 1;
        }
        i++;
    }
    return splitted;
}