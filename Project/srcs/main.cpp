#include "../include/webserve.hpp"

void	print_location_block(std::vector<location_block>::iterator it, int tab) {
	if (it->uri.size() != 0) {
		for (int i = 0; i < tab; i++) {
			std::cout << "	";
		}
		std::cout << "uri: " << it->uri << "|" << std::endl;
	}
	if (it->root.size() != 0) {
		for (int i = 0; i < tab; i++) {
			std::cout << "	";
		}
		std::cout << "root: " << it->root << "|" << std::endl;
	}

	if (it->autoindex.size() != 0) {
		for (int i = 0; i < tab; i++) {
			std::cout << "	";
		}
		std::cout << "autoindex: " << it->autoindex << "|" << std::endl;
	}
	if (it->max_client != 0) {
		for (int i = 0; i < tab; i++) {
			std::cout << "	";
		}
		std::cout << "max_client: " << it->max_client << "|" << std::endl;
	}
	if (it->allow_methods.size() != 0) {
		std::vector<std::string>::iterator al_it;

		al_it = it->allow_methods.begin();
		for (int i = 0; i < tab; i++) {
			std::cout << "	";
		}
		std::cout<< "allow_methods list:" << std::endl;
		while (al_it != it->allow_methods.end()) {
			for (int i = 0; i < tab; i++) {
				std::cout << "	";
			}
			std::cout << *al_it << std::endl;
			al_it++;
		}
	}
	if (it->error_page.size() != 0) {
		std::vector<std::pair<std::string, std::string> >::iterator er_it;

		er_it = it->error_page.begin();
		for (int i = 0; i < tab; i++) {
			std::cout << "	";
		}
		std::cout<< "error_page list:" << std::endl;
		while (er_it != it->error_page.end()) {
			for (int i = 0; i < tab; i++) {
				std::cout << "	";
			}
			std::cout << er_it->first << " " << er_it->second << std::endl;
			er_it++;
		}
	}
	if (it->redirection.size() != 0) {
		std::vector<std::pair<std::string, std::string> >::iterator er_it;

		er_it = it->redirection.begin();
		for (int i = 0; i < tab; i++) {
			std::cout << "	";
		}
		std::cout<< "redirection list:" << std::endl;
		while (er_it != it->redirection.end()) {
			for (int i = 0; i < tab; i++) {
				std::cout << "	";
			}
			std::cout << er_it->first << " " << er_it->second << std::endl;
			er_it++;
		}
	}
	if (it->location.size() != 0) {
			for (int i = 0; i < tab; i++) {
				std::cout << "	";
			}
			std::cout << "listes des location:" << std::endl;
			std::vector<location_block>::iterator loc_it;

			loc_it = it->location.begin();
			while (loc_it != it->location.end()) {
				print_location_block(loc_it, ++tab);
				loc_it++;
			}

	}



}


void	print_stc(server_conf serv) {
	std::vector<server_block>::iterator it;
	int 								num_block = 1;

	it = serv.server.begin();
	while (it != serv.server.end()) {
		std::cout << std::endl;
		std::cout << "start with " << num_block << " server"  << std::endl;

		if (it->server_name.size() != 0)
			std::cout << "server_name " << it->server_name << "|" <<  std::endl;
		if (it->root.size() != 0)
			std::cout << "root " << it->root << "|" <<  std::endl;
		if (it->index.size() != 0)
			std::cout << "index " << it->index << "|" <<  std::endl;
		if (it->autoindex.size() != 0)
			std::cout << "autoindex " << it->autoindex<< "|" <<  std::endl;
		if (it->max_client != 0)
			std::cout << "max client " << it->max_client<< "|" <<  std::endl;
		if (it->port_ip.size()!= 0) {
			std::cout << "listes de port-ip :" << std::endl;
			std::vector<std::pair<int, std::string> >::iterator ip_it;
			ip_it = it->port_ip.begin();
			while (ip_it != it->port_ip.end()) {
				std::cout << "	" << ip_it->first << " " << ip_it->second << "|" << std::endl; 
				ip_it++;
			}
		}
		if (it->error_page.size() != 0) {
			std::cout << "listes des error pages:" << std::endl;
			std::vector<std::pair<std::string, std::string> >::iterator error_it;

			error_it = it->error_page.begin();
			while (error_it != it->error_page.end()) {
				std::cout << "	" << error_it->first << " " << error_it->second << "|" << std::endl;
				error_it++;
			}
		}
		if (it->redirection.size() != 0) {
			std::cout << "listes des redirections:" << std::endl;
			std::vector<std::pair<std::string, std::string> >::iterator redir_it;

			redir_it = it->redirection.begin();
			while (redir_it != it->redirection.end()) {
				std::cout << "	" << redir_it->first << " " << redir_it->second << "|" << std::endl;
				redir_it++;
			}
		}

		if (it->allow_methods.size() != 0) {
			std::cout << "listes des methods: " << std::endl;
			std::vector<std::string>::iterator methods_it;

			methods_it = it->allow_methods.begin();
			while (methods_it != it->allow_methods.end()) {
				std::cout << "	" << *methods_it << "|" << std::endl;
				methods_it++;
			}
		}

		if (it->location.size() != 0) {
			std::cout << "listes des location:" << std::endl;
			std::vector<location_block>::iterator loc_it;

			loc_it = it->location.begin();
			while (loc_it != it->location.end()) {
				print_location_block(loc_it, 2);
				loc_it++;
			}
		}

		num_block++;
		it++;
	}

}


int	main(int ac, char **av)
{
	server_conf	serv_conf;

	if (parsing(&serv_conf, ac, av) != 0) {
		std::cout << "error config file" << std::endl;
		return (1);
	}


	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	print_stc(serv_conf);


/*
	server_conf test(serv_conf);
	server_conf bite;
	bite = serv_conf;

*/
	

/*	location_block bite;
	bite = serv_conf.server[0].find_uri("catfkdlsjf");
	if (bite.uri.size() == 0) {
		std::cout << "pas d'uri trouve" << std::endl;
		return  (0);
	}
	std::cout << "res du find " << bite.root << std::endl;*/

//	std::cout << "pos uri to find " << serv_conf.server[0].pos_uri("cafhdst") << std::endl;
}
