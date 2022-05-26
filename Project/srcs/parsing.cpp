#include "../include/webserve.hpp"
#include <fstream>
#include <string>


int	default_conf(server_conf *stc) {
	return (1);
}

size_t skip_white_space(std::string str) {
	size_t i = 0;
	while (i < str.size())
	{
		if (str[i] != '\f' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r' && str[i] != '\v' && str[i] != ' ')
			break;
		i++;
	}
	return i;
}

int	check_declaration_server_line(std::string line) {
	size_t start = 0;
	size_t braket = 0;

	start = skip_white_space(line);
	line.erase(0, start);
	if (line.find("server") == 0)
	{
	//	std::cout << "1 step good" <<std::endl;
		line.erase(0, 6);
		if (line.size() == 0) {
			std::cout << "miss bracket after server" << std::endl;
			return (1);
		}
		start = skip_white_space(line);
		line.erase(0, start);
	//	std::cout << "2 step good" <<std::endl;
		if (line.size() == 0) {
			std::cout << "miss bracket after server" << std::endl;
			return (1);
		}
	//	std::cout << "3 step good" <<std::endl;
		if (line[0] != '{')
		{
			std::cout << "miss bracket after server : line=" << line<< std::endl;
			return (1);
		}
	//	std::cout << "4 step good" <<std::endl;
		line.erase(0,1);
		start = skip_white_space(line);	
		if (start != line.size())
		{
			std::cout << "some after the bracket :" << line << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "line " << line << std::endl;
		return 1;
	}
	std::cout << "declaration server good" << std::endl;
	return (0);
}

int	fill_serv(server_conf *stc, int index, std::ifstream file, std::string line) {
	size_t	start = 0;
	while (getline(file, line))
	{
		if (line.size() != 0 && (start = skip_white_space(line)) != line.size())
		{
			line.erase(0, start);

		}
		else 
			std::cout << "line skip" << std::endl;
	}
	return (0);
}

int	read_conffile_fill_stc(server_conf *stc, char *file_name){
	std::ifstream file(file_name);
	std::string line;

	int open_serv = 0;
	int server_index = 0;
	size_t	index;

	while (getline(file, line)) {
		/*first for empty line*/
		if ((line.size() != 0) && (skip_white_space(line) != line.size())) {
			std::cout << line << std::endl;
			/*check for server {*/
			if (open_serv == 0) {
				index = line.find("server");
				if (index == std::string::npos || check_declaration_server_line(line)) {
					std::cout << "first you should have a server" << std::endl;
					return (1);
				}
				open_serv = 1;
				server_index++;
				fill_serv(stc, server_index - 1, file, line);
		}
		else
			std::cout << "skip line" << std::endl;
	}
	return (0);
}

int	parsing(server_conf *stc, int ac, char **av) {
	if (ac == 1)
		return (default_conf(stc));
	if (ac == 2)
		return (read_conffile_fill_stc(stc, av[1]));
	return (1);
}