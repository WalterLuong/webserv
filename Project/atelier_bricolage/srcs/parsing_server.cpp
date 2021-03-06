#include "../includes/webserve.hpp"

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

	for (size_t i = 0; i < line.size(); i++) {
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
	for (size_t i = 0; i < line.size(); i++) {
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
		return (1);
	}	
	int j = 0;
	int tf = 1;
	for (size_t i = 0; i < line.size(); i++) {
		if (line[i] == ' ' || line[i] == '\f' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r' || line[i] == '\v') {
			if (tf == 1) {
				std::string cpy(line, j, i-j);	
				if (cpy != "GET" && cpy != "POST" && cpy != "DELETE") {
					std::cout << "bad methods option" << std::endl;
					return 1;
				}
				else
					to_fill->push_back(cpy);
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
	return (0);
}

int	check_cgi(std::string line, std::pair<std::string, std::string> *to_fill) {

	int nbr_w;
	if ((nbr_w = count_word(line)) != 3)
		return 1;

	int i = 0;
	while (line[i])	{
		if (line[i] == ' ' || line[i] == '\f'|| line[i] == '\t'|| line[i] == '\n'|| line[i] == '\r'|| line[i] == '\v')
			break ;
		i++;
	}
	line = line.substr(i);
	i = 0;
	while (line[i]){
		if (line[i] != ' ' && line[i] != '\f'&& line[i] != '\t'&& line[i] != '\n'&& line[i] != '\r'&& line[i] != '\v')
			break ;
		i++;
	}
	line = line.substr(i);
	i = 0;
	while (line[i]) {
		if (line[i] == ' ' || line[i] == '\f'|| line[i] == '\t'|| line[i] == '\n'|| line[i] == '\r'|| line[i] == '\v')
			break ;
		i++;
	}
	to_fill->first = line.substr(0, i);
	line = line.substr(i);
	i = 0;
	while (line[i]){
		if (line[i] != ' ' && line[i] != '\f'&& line[i] != '\t'&& line[i] != '\n'&& line[i] != '\r'&& line[i] != '\v')
			break ;
		i++;
	}
	line = line.substr(i); 
	size_t end;
	end = line.find_first_of(" \f\t\n\r\v");
	if (end != std::string::npos) {
		line = line.substr(0, end);
	}
	to_fill->second = line;
	if (to_fill->first.find(".") != 0) {
		std::cout << "first arg of cgi path is not an extension" << std::endl;
		return 1;
	}
	return 0;

}

int	check_listen(std::string line, std::pair<int, std::string> *to_fill) {
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

		for (size_t i = 0; i < line.size(); i++) {
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

int	go_to_indicated_cmd(std::string cmd, std::string line, location_block *stc) {
	std::pair<int, std::string> listen_port;
	std::string str;
	int			nbr;


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
		stc->redirection.push_back(redir);
	}

	if (cmd == "error_page") {
		std::pair<std::string, std::string> error;

		line = skip_word_and_ws(line, cmd);
		if (check_error(line, &error) != 0) {
				std::cout << "bad error in a server" << std::endl;
				return 1;
		}
		stc->error_page.push_back(error);
	}

	if (cmd == "cgi_path") {
		std::pair<std::string, std::string> cgi;
		if (check_cgi(line, &cgi) != 0) {
				std::cout << "bad cgi path declaration" << std::endl;
				return 1;
		}	
		stc->cgi_path.push_back(cgi);
	}
	return (0);

} 



int	go_to_indicated_cmd(std::string cmd, std::string line, server_block *stc) {
	std::pair<int, std::string> listen_port;
	std::string str;
	int			nbr;
	if (cmd == "listen") {
		if (stc->port_ip.size() > 0) {
			std::cout << "on port per server" << std::endl;
			return 1;
		}
		line = skip_word_and_ws(line, cmd);
		
		if (check_listen(line, &listen_port) != 0) {
			std::cout << "bad listen option" << std::endl;
			return (1);
		}	
//		std::cout << listen_port.first << " " << listen_port.second  <<  std::endl;
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
		stc->redirection.push_back(redir);
	}

	if (cmd == "error_page") {
		std::pair<std::string, std::string> error;

		line = skip_word_and_ws(line, cmd);
		if (check_error(line, &error) != 0) {
				std::cout << "bad error in a server" << std::endl;
				return 1;
		}
		stc->error_page.push_back(error);
	}
	if (cmd == "cgi_path") {
		std::pair<std::string, std::string> cgi;
		if (check_cgi(line, &cgi) != 0) {
				std::cout << "bad cgi path declaration" << std::endl;
				return 1;
		}	
		stc->cgi_path.push_back(cgi);
	}
	return (0);

} 

int	check_location_line(std::string line, std::string *to_fill) {
	if (count_word(line) != 2) {
		std::cout << "bad nbr arg for location" << std::endl;
		return (1);
	}

	if (line[0] != '/') {
		std::cout << "uri of location need to start with /" << std::endl;
		return 1;
	}
	for (size_t i = 0; i < line.size(); i++) {
		if (line[i] == ' ' || line[i] == '\f' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r' || line[i] == '\v') {
			if (i == 1) {
				std::cout << "need uri in location line" << std::endl;
				return (1);
			}
			std::string cpy(line, 1, i - 1);
			*to_fill =  cpy;
			line.erase(0, i);
			break ;
		}
	}
	int start = skip_white_space(line);
	line.erase(0, start);
	line = erase_ws_end_string(line);
	if (line != "{") {
		std::cout << "bad arg in location line:" << line << "|" << std::endl;
		return (1);
	}

	return (0);
}

int	check_location_block(std::ifstream *file, std::string line, location_block *to_fill) {
	size_t	start = 0;
	std::vector<std::string>::iterator first;

	std::string uri;
	line = skip_word_and_ws(line, "location");

	if (check_location_line(line, &uri) != 0) {
		std::cout  << "bad location line" << std::endl;
		return (1);
	}
	to_fill->uri = uri;

	while (getline(*file, line)) {
		if (line.size() != 0 && (start = skip_white_space(line)) != line.size()) {
			line.erase(0, start);
				
			for (first = to_fill->lst_location_option.begin(); first != to_fill->lst_location_option.end(); first++) {	
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
						return (0);
					}
					if (*first == "location"){
						location_block rec;

						if (check_location_block(file, line, &rec) != 0) {
							std::cout << "location bad" << std::endl;
							return (1);
						}
						to_fill->location.push_back(rec);
					}
					else {
						if (go_to_indicated_cmd(*first, line, to_fill) != 0) {
							std::cout << "bad option for commande line " << *first << std::endl;
							return (1);
						}
					}
					break;
				}
			}
			if (first == to_fill->lst_location_option.end()) {
				std::cout << "bad option: " << line << std::endl; 
				return (1);
			}
		}
	}
	return (0);
}


int	fill_serv(server_block *server_to_fill, std::ifstream *file, std::string line) {
	size_t	start = 0;
	std::vector<std::string>::iterator first;

	while (getline(*file, line)) {
		if (line.size() != 0 && (start = skip_white_space(line)) != line.size()) {
			line.erase(0, start);
			for (first = server_to_fill->lst_server_option.begin(); first != server_to_fill->lst_server_option.end(); first++) {	
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
						return (0);
					}
					if (*first == "location"){
						location_block to_fill;

						if (check_location_block(file, line, &to_fill) != 0) {
							std::cout << "location bad" << std::endl;
							return (1);
						}
						server_to_fill->location.push_back(to_fill);
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
			if (first == server_to_fill->lst_server_option.end()) {
				std::cout << "bad option: " << line << std::endl; 
				return (1);
			}
		}
	}
	return (0);
}



int	check_declaration_server_line(std::string line) {
	size_t start = 0;

	start = skip_white_space(line);
	line.erase(0, start);
	if (line.find("server") == 0) {
		line.erase(0, 6);
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
	return (0);
}

