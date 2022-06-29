#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "../includes/ResponseHeader.hpp"
#include <dirent.h>
	
std::string		AutoIndexGenerator( std::string path ) {
	
	DIR		*directory;
	dirent	*files;
	std::string		list;

	list = "<html>\n<head><title>Index of ";
	list += path;
	list += "</title></head>\n";
	list += "<body bgcolor=\"blue\"><hr>\n";
	list +=	"<a href=\"../\">../</a>";
	directory = opendir(path.c_str());
	if (directory)
	{
		while ((files = readdir(directory)) != NULL)
		{
			list += "<a href=\"";
			list += files->d_name;
			list += "\">";
			list += files->d_name;
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
	return list;
}

int		main(int ac, char **av)
{
	std::cout << AutoIndexGenerator("../../www/") << std::endl;
}
