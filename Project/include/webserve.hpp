#ifndef WEBSERVE_HPP
# define WEBSERVE_HPP 

# include "parsing_conffile.hpp"
# include <iostream>
# include <fstream>
# include <string>
# include <cstdlib>

static std::vector<std::string> lst_server_option = {"listen", "server_name", "index", "max_body_client", "error_page", "location", "autoindex", "root", "cgi_path", "methods", "return", "}" };
static std::vector<std::string> lst_location_option = {"index", "autoindex", "cgi_path", "methods", "root", "error_page", "return", "}"};

int parsing(server_conf *stc, int ac, char **av);


size_t skip_white_space(std::string str);
std::string skip_word_and_ws(std::string line, std::string word);

int	check_declaration_server_line(std::string line);
int	fill_serv(server_block *stc, std::ifstream *file, std::string line);



#endif
