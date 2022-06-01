#include "../include/webserve.hpp"

/* ne marche que pour les strings avec un seul mot */
std::string	erase_ws_end_string(std::string line) {
	size_t erase_from;	

	erase_from = line.find(' ');
	if (erase_from != std::string::npos)
		line.erase(erase_from);
	erase_from = line.find('\f');
	if (erase_from != std::string::npos)
		line.erase(erase_from);
	erase_from = line.find('\t');
	if (erase_from != std::string::npos)
		line.erase(erase_from);
	erase_from = line.find('\n');
	if (erase_from != std::string::npos)
		line.erase(erase_from);
	erase_from = line.find('\r');
	if (erase_from != std::string::npos)
		line.erase(erase_from);
	erase_from = line.find('\v');
	if (erase_from != std::string::npos)
		line.erase(erase_from);
	return (line);
}

int	count_word(std::string line) {
	int nbr_word = 0;
	int tf = 1;

	for (int i = 0; i < line.size(); i++) {
		if (line[i] == ' ' || line[i] == '\f' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r' || line[i] == '\v') {
			tf = 1;	
		}
		else {
			if (tf == 1) {
				tf = 0;
				nbr_word++;
			}
		}
	}
	return (nbr_word);
}

int check_max_body_client(std::string line, int *nbr) {
	if (count_word(line) != 1) {
		std::cout << "to much option for max_body_client" << std::endl;
		return (1);
	}
	line = erase_ws_end_string(line);	
	for (int i = 0; i < line.size(); i++) {
		if (line[i] > '9' || line[i] < '0')
			return (1);
	}
	*nbr = atoi(line.c_str());
	return 0;
}

int	check_error(std::string line, std::pair<std::string, std::string> *to_fill) {
	if (count_word(line) != 2) {
		std::cout << "error need 2 option" << std::endl;
		return 1;
	}
	int nbr = atoi(line.c_str());
	if (nbr < 400 || nbr > 420) {
		std::cout << "error port 400 --> 420" << std::endl;
		return 1;
	}
	std::string cpy(line, 0, 3);
	to_fill->first = cpy;
	line.erase(0,3);
	int skip_ws;
	if ((skip_ws = skip_white_space(line)) == 0) {
		std::cout << "error port containe only nbr" << std::endl;
		return (1);
	}
	line.erase(0, skip_ws);
	line = erase_ws_end_string(line);
	to_fill->second = line;
	return (0);
}



int	check_redirection(std::string line, std::pair<std::string, std::string> *to_fill) {
	if (count_word(line) != 2) {
		std::cout << "redirection need 2 option" << std::endl;
		return 1;
	}
	int nbr = atoi(line.c_str());
	if (nbr < 300 || nbr > 310) {
		std::cout << "redirection port 300 --> 310" << std::endl;
		return 1;
	}
	std::string cpy(line, 0, 3);
	to_fill->first = cpy;
	line.erase(0,3);
	int skip_ws;
	if ((skip_ws = skip_white_space(line)) == 0) {
		std::cout << "redirection port containe only nbr" << std::endl;
		return (1);
	}
	line.erase(0, skip_ws);
	line = erase_ws_end_string(line);
	to_fill->second = line;
	return (0);
}


int check_methods(std::string line, std::vector<std::string> *to_fill) {
	if (count_word(line) > 3) {
		std::cout << "too much arg for methods" << std::endl;
		return (0);
	}	
	std::cout << "line check methods after erase ws end strings:" << line << "|" << std::endl;
	int j = 0;
	int tf = 1;
	for (int i = 0; i < line.size(); i++) {
		if (line[i] == ' ' || line[i] == '\f' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r' || line[i] == '\v') {
			if (tf == 1) {
				std::string cpy(line, j, i-j);	
				if (cpy != "GET" && cpy != "POST" && cpy != "DELETE") {
					std::cout << "bad methods option" << std::endl;
					return 1;
				}
				else
					to_fill->push_back(cpy);
				std::cout << "print cpy line methods : " << cpy << "|" << std::endl;
				//to_fill->push_back(cpy);
				tf = 0;
			}
		}
		else {
			if (tf == 0) {
				j = i;
				tf = 1;
			}
		}
	}
	if (tf == 1) {
		std::string bytes(line, j, line.size()-j);
		if (bytes != "POST" && bytes != "GET" && bytes != "DELETE") {
			std::cout << "bad methods option" << std::endl;
			return 1;
		}
		else
			to_fill->push_back(bytes);
		std::cout << "print bytes line methods: " << bytes << "|" << std::endl;
	}
	return (0);
}


int check_root(std::string line, std::string *str) {
	if (count_word(line) != 1) {
		return (1);
	}
	line = erase_ws_end_string(line);
	*str = line;
	return (0);
}

int check_autoindex(std::string line, std::string *str) {
	if (count_word(line) != 1) {
		return (1);
	}
	line = erase_ws_end_string(line);
	if (line != "on" && line != "off") {
		std::cout << "autoindex completion by on, off" << std::endl;
		return 1;
	}
	std::cout << "good autoindex" << std::endl;
	*str = line;
	return (0);
}


int check_index(std::string line, std::string *str) {
	if (count_word(line) != 1) {
		return (1);
	}
	line =  erase_ws_end_string(line);
	*str = line;
	return 0;
}

int	check_server_name(std::string line, std::string *ret) {

	if (count_word(line) != 1) {
		return (1);
	}
	line = erase_ws_end_string(line);
	*ret = line;
	std::cout << "print line: " << line << "|" << std::endl;
	return (0);
}

int	check_listen(std::string line, std::pair<int, std::string> *to_fill) {
	char *str;
	int	nbr;	

	nbr = atoi(line.c_str());
	if (nbr < 1024 || nbr > 65535) {
		std::cout << "port non authorise a 42" << std::endl;
		return (1);
	}
	to_fill->first = nbr;	
	int len_nbr = 1;
	while (nbr >= 10) {
		len_nbr++;
		nbr = nbr / 10;
	}
	line.erase(0, len_nbr);	
	line.erase(0, skip_white_space(line));
	if (line.size() == 0) {
		to_fill->second = "127.0.0.1";
	}	
	else {
		int nbr_ip = 0;
		int point = 1;
		int nbr_point = 0;

		for (int i = 0; i < line.size(); i++) {
			if (line[i] == '.'){
				if (point == 1) {
					std::cout << "ip invalide, . first or . ." << std::endl;
					return (1);
				}
				else {
					point = 1;
					nbr_point++;
				}

			}
			else {
				if (line[i] > '9' || line[i] < '0') {
					std::cout << "ip invalide, not a ip_number : " << line << std::endl;	
					return (1);
				}
				else {
					if (point == 1) {
						point = 0;
						nbr_ip++;
					}
				}
			}
		}			
		if (nbr_ip != 4) {
			if (nbr_ip > 4)
				std::cout << "ip invalide, to much number for an ip, exemple valide ip : 127.0.0.1" << std::endl;
			if (nbr_ip < 4)
				std::cout << "ip invalide, to low number for an ip, exemple valide ip : 127.0.0.1" << std::endl;
			return (1);
		}
		if (nbr_point != 3) {
			std::cout << "ip invalide" << std::endl;
			return (1);
		}
		to_fill->second = line;
	}	

	return (0);
}

int	go_to_indicated_cmd(std::string cmd, std::string line, server_block *stc) {
	std::pair<int, std::string> listen_port;
	std::string str;
	int			nbr;
	if (cmd == "listen") {
		line = skip_word_and_ws(line, cmd);
		
		if (check_listen(line, &listen_port) != 0) {
			std::cout << "bad listen option" << std::endl;
			return (1);
		}	
		std::cout << listen_port.first << " " << listen_port.second  <<  std::endl;
		stc->port_ip.push_back(listen_port);	
	}

	if (cmd == "server_name") {
		line = skip_word_and_ws(line, cmd);
		if (stc->server_name.size() != 0) {
			std::cout << "multiple server_name in a server" << std::endl;
			return (1);
		}
		if (check_server_name(line, &str) != 0) {
			std::cout << "bad server name" << std::endl;	
			return (1);
		} 
		stc->server_name = str;
	}

	if (cmd == "root") {
		line = skip_word_and_ws(line, cmd);
		if (stc->root.size() != 0) {
			std::cout << "multiple root in a server" << std::endl;
			return (1);
		}
		if (check_root(line, &str) != 0) {
			std::cout << "bad root in a server" << std::endl;
			return 1;
		}
		stc->root = str;
	}

	if (cmd == "index")	{
		line = skip_word_and_ws(line, cmd);
		if (stc->index.size() != 0) {
			std::cout << "multiple index in a server" << std::endl;
			return (1);
		}
		if (check_index(line, &str) != 0) {
			std::cout << "bad index in a server" << std::endl;
			return 1;
		}
		stc->index = str;

	}

	if (cmd == "autoindex") {
		line = skip_word_and_ws(line, cmd);
		if (stc->autoindex.size() != 0) {
			std::cout << "multiple index in a server" << std::endl;
			return (1);
		}
		if (check_autoindex(line, &str) != 0) {
			std::cout << "bad autoindex in a server" << std::endl;
			return 1;
		}
		stc->autoindex = str;
	}

	if (cmd == "max_body_client") {
		line = skip_word_and_ws(line, cmd);
		if (stc->max_client != 0) {
			std::cout << "multiple max_body_client define in a server" << std::endl;
			return (1);
		}
		if (check_max_body_client(line, &nbr) != 0) {
			std::cout << "bad max_body_client in a server" << std::endl;
			return 1;
		}
		stc->max_client = nbr;
	}

	if (cmd == "methods") {
		std::vector<std::string> meths;

		line = skip_word_and_ws(line, cmd);
		if (stc->allow_methods.size() != 0) {
			std::cout << "multiple methods line declare in a server" << std::endl;
			return (1);
		}
		if (check_methods(line, &meths) != 0) {
			std::cout << "bad methods in a server" << std::endl;
			return 1;
		}
		stc->allow_methods = meths;
	}

	if (cmd == "return") {
		std::pair<std::string, std::string> redir;

		line = skip_word_and_ws(line, cmd);
		if (check_redirection(line, &redir) != 0) {
				std::cout << "bad redirection in a server" << std::endl;
				return 1;
		}
		std::cout << "print redirection pair->first:" << redir.first << "|pair->sec:" << redir.second << "|" << std::endl;
		stc->redirection.push_back(redir);
	}

	if (cmd == "error_page") {
		std::pair<std::string, std::string> error;

		line = skip_word_and_ws(line, cmd);
		if (check_error(line, &error) != 0) {
				std::cout << "bad error in a server" << std::endl;
				return 1;
		}
		std::cout << "print error pair->first:" << error.first << "|pair->sec:" << error.second << "|" << std::endl;
		stc->error_page.push_back(error);
	}

	return (0);

} 

int	fill_serv(server_block *server_to_fill, std::ifstream *file, std::string line) {
	size_t	start = 0;
	int	location_index = 0;
	std::vector<std::string>::iterator first;

	std::cout << std::endl;
	std::cout << "fill serv fonction" << std::endl;
	std::cout << std::endl;
	while (getline(*file, line)) {
		if (line.size() != 0 && (start = skip_white_space(line)) != line.size()) {
			line.erase(0, start);
				
			for (first = lst_server_option.begin(); first != lst_server_option.end(); first++) {	
				start = line.find(*first);
				if (start != std::string::npos && start == 0) {
					if (*first == "}") {
						line.erase(0,1);
						start = skip_white_space(line);	
						line.erase(0, start);
						if (line.size() != 0) {
							std::cout << "some after the bracket:" << line << std::endl;
							return (1);
						}
						if (location_index %2 == 1)
							location_index++;
						else
							return (0);
					}
					if (*first == "location"){
						location_index++;
					/*	if (fill_location(stc, file, line, index, location_index - 1) != 0) {
							std::cout << "location block bad" << std::endl;
							return (1);
						}
				*/		
					}
					else {
						if (go_to_indicated_cmd(*first, line, server_to_fill) != 0) {
							std::cout << "bad option for commande line " << *first << std::endl;
							return (1);
						}
					}
					break;
				}
			}
			if (first == lst_server_option.end()) {
				std::cout << "bad option: " << line << std::endl; 
				return (1);
			}
		}
		else 
			std::cout << "line skip" << std::endl;
	}
	return (0);
}



int	check_declaration_server_line(std::string line) {
	size_t start = 0;
	size_t braket = 0;

	start = skip_white_space(line);
	line.erase(0, start);
	if (line.find("server") == 0) {
		line.erase(0, 6);
/*		if (line.size() == 0) {
			std::cout << "miss bracket after server" << std::endl;
			return (1);
		} */
		start = skip_white_space(line);
		line.erase(0, start);
		if (line.size() == 0) {
			std::cout << "miss bracket after server" << std::endl;
			return (1);
		}
		if (line[0] != '{') {
			std::cout << "miss bracket after server : line=" << line<< std::endl;
			return (1);
		}
		line.erase(0,1);
		start = skip_white_space(line);	
		if (start != line.size()) {
			std::cout << "some after the bracket :" << line << std::endl;
			return 1;
		}
	}
	else {
		std::cout << "bad line: " << line << std::endl;
		return 1;
	}
	// std::cout << "declaration server good" << std::endl;
	return (0);
}

