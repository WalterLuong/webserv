#include "../include/webserve.hpp"


int	main(int ac, char **av)
{
	server_conf	serv_conf;

	if (parsing(&serv_conf, ac, av) != 0) {
		std::cout << "error config file" << std::endl;
		return (1);
	}



}