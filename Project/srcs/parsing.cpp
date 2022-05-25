#include "../include/webserve.hpp"

int	default_conf(server_conf *stc) {

}

int	parsing(server_conf *stc, int ac, char **av) {
	if (ac == 1)
		return (default_conf(stc));
	if (ac == 2)
		return (read_conffile_fill_stc(stc, av[1]));
	return (1);
}