#ifndef WEBSERVE_HPP
# define WEBSERVE_HPP 

# include "parsing_conffile.hpp"
# include "parsing_request.hpp"
# include <iostream>
# include <fstream>
# include <string>
# include <cstdlib>
# include <sstream>
# include <sys/types.h>
# include <unistd.h>
# include <algorithm>
# include <vector>

int parsing(server_conf *stc, int ac, char **av);


size_t skip_white_space(std::string str);
std::string skip_word_and_ws(std::string line, std::string word);

int	check_declaration_server_line(std::string line);
int	fill_serv(server_block *stc, std::ifstream *file, std::string line);


/* Fonction for Header */
std::string get_date();
std::string generator_error_file(std::string error_status);

#endif
