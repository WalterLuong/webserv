#include "webserve.hpp"
#include <fstream>
#include <string>



int	default_conf(server_conf *stc) {
	/*implement a default config file with open*/
	if (stc)
		return (1);
	return (1);
}


int	read_conffile_fill_stc(server_conf *stc, char *file_name){
	std::ifstream file(file_name);
	std::string line;

	size_t	index;

	if (!file) {
		return (default_conf(stc));
	}

	while (getline(file, line)) {
		/*first for empty line*/
		server_block	serv_to_fill;
		if ((line.size() != 0) && (skip_white_space(line) != line.size())) {
			index = line.find("server");
			if (index == std::string::npos || check_declaration_server_line(line)) {
				std::cout << "first you should have a server" << std::endl;
				return (1);
			}
			if (fill_serv(&serv_to_fill, &file, line) != 0) {
				std::cout << "bad config_file" << std::endl;
				return (1);
			}
			stc->server.push_back(serv_to_fill);
		}
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
