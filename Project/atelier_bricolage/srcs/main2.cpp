#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "../includes/ResponseHeader.hpp"

// int		main() {
// 	ResponseHeader	yo;
// 	yo.setContentLength("254");
// 	yo.setContentType("text/html");
// 	yo.setDate("Wed, 16 Jun 2022");
// 	yo.setServerName("test");
// 	yo.setStatus("200 OK");
// 	yo.setLastModified("Wed, 16 Jun 2022");
// 	yo.generateHeader();
// 	std::cout << yo.getHeader() << std::endl;
// 	yo.clearHeader();
// 	yo.generateHeader();
// 	std::cout << yo.getHeader() << std::endl;

// }

std::string		readFromFile(std::string path) {
	std::ifstream		ifs(path);
	std::string			ret_buffer;
	std::string			tmp;

	if (!ifs)
	{
		std::cout << "This file doesn't exist." << std::endl;
		return ("");
	}
	while (!ifs.eof())
	{
		std::getline(ifs, tmp);
		ret_buffer += tmp;
		if (!ifs.eof())
			ret_buffer += "\n";
	}
	ifs.close();
	return ret_buffer;
}

int		main(int ac, char **av)
{
	std::string		ret;
	if (ac != 2)
	{
		std::cout << "Wrong number of arguments" << std::endl;
		return 1;
	}
	ret = readFromFile(av[1]);
	std::cout << ret << std::endl;
}
