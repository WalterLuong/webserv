/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viporten <viporten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 09:45:51 by wluong            #+#    #+#             */
/*   Updated: 2022/07/10 01:28:32 by viporten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

using namespace std;
 
std::string get_date() {
 time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"%a, %d %b %G %T GMT",timeinfo);
 // std::cout << (buffer) << std::endl;

	std::string ret(buffer);
//  std::cout << ret << std::endl;
  return ret;
 }

/* How use it */
/* Generator of body for responce */
/* Give your error status as string */
/* Get Content-Lenght by use string.size() */


std::string generator_error_file(std::string error_status) {
	request mes;
	std::string ret;

	ret += "<!DOCTYPE html>\r\n<html>\n<head>\n<title>";
	ret += error_status;
	ret += " ";
	ret += mes.map_error[error_status];
	ret += "</title>\n</head>\n<body>\n<h1>Error ";
	ret += error_status;
	ret += "</h1>\n<p>Noppppppppppppppppp</p>\n<p>Cherche encore et peut etre tu trouveras l'amour</p>\n<p><strong>coeur</strong></p>\n</body>\n</html>";
	ret += "\r\n";

	return ret;
}

char *itoa(int num, char* buffer, int base) {

	int curr = 0;

	if (num == 0) {
		buffer[curr++] = '0';
		buffer[curr] = '\0';
		return buffer;
	}

	int num_digits = 0;

	if (num < 0) {
		if (base == 10) {
			num_digits ++;
			buffer[curr] = '-';
			curr ++;
			num *= -1;
		}
		else
			return NULL;
	}

	num_digits += (int)floor(log(num) / log(base)) + 1;
 
	while (curr < num_digits) {
		int base_val = (int) pow(base, num_digits-1-curr);

		int num_val = num / base_val;
 
		char value = num_val + '0';
		buffer[curr] = value;
 
		curr ++;
		num -= base_val * num_val;
	}
	buffer[curr] = '\0';
	return buffer;
}

std::string		int_to_str(size_t size) {
	char intStr[500];
	itoa(size, intStr, 10);
	std::string	ret(intStr);
	return ret;
}

std::string		AutoIndexGenerator( std::string path ) {
	
	DIR		*directory;
	dirent	*files;
	std::string		list;

	list = "<!DOCTYPE html>\n";
	list += "<html>\n<head><title>Index of ";
	list += path;
	list += "</title></head>\n";
	list += "<body bgcolor=\"white\"><hr>\n";
	list += "<h1>Index of ";
	list += path;
	list += " </h1>\n";
	directory = opendir(path.c_str());
	if (directory)
	{
		while ((files = readdir(directory)) != NULL)
		{
			list += "<a href=\"";
			list += files->d_name;
			if (files->d_type == DT_DIR)
				list += "/";
			list += "\">";
			list += files->d_name;
			if (files->d_type == DT_DIR)
				list += "/";
			list += "</a><hr>";
			list += "\n";
		}
	}
	else
	{
		std::cout << "Can't open directory" << std::endl;
		return "";
	}
	list += "</body>\n</html>";
	list += "\r\n";
	closedir(directory);
	return list;
}

bool	isDirectory( std::string path )
{
	DIR		*directory;
	directory = opendir(path.c_str());
	if (directory)
		return true;
	return false;
}