/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 09:44:16 by wluong            #+#    #+#             */
/*   Updated: 2022/07/14 03:05:20 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <ctime>
# include "../includes/webserve.hpp"
# include <dirent.h>

std::string get_date();
std::string generator_error_file(std::string error_status);
char *itoa(int num, char* buffer, int base);
std::string		int_to_str(size_t size);
std::string		AutoIndexGenerator( std::string path );
bool	isDirectory( std::string path );
std::string		ignore_slashes(std::string s);

#endif