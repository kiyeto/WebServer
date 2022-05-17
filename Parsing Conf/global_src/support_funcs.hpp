/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_funcs.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrija <mbrija@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 10:49:22 by mbrija            #+#    #+#             */
/*   Updated: 2022/04/18 10:49:22 by mbrija           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include <string>
# include <cstring>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include <algorithm>
# include <unistd.h>

std::string ltrim(const std::string &str, const std::string set);
std::string rtrim(const std::string &str, const std::string set);
std::string trim(const std::string &str, const std::string set);
std::vector<std::string> split(std::string str, char sep);
