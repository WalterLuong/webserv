#ifndef PARSING_REQUEST_HPP
# define PARSING_REQUEST_HPP

#include "webserve.hpp"
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Server.hpp"
#include "parsing_conffile.hpp"

class request {

	public :
		
		std::string	methods;
		std::string path;
		std::string walter_path;
		std::string	http_version;
		std::string request_body;
		std::string	body;

		int		chunked;
		int		validity; //code status pour la map error;

		int cur_serv_index;
		int in_location;
		location_block	location_path;
		int autoindex_on;

		std::string filename;
		/* need serve concerner par instruction"HOSt"*/
		/* body pour les chunked */

		request();
		request(std::string line);
		request(std::string line, std::vector<Server> lst_inf);
		request(request const & cpy); 
		request &operator=(request const & cpy);
		~request();


		std::map<std::string, std::string>	map_error;
		std::map<std::string, std::string>	map_file_type;
		std::map<std::string, std::string>	instruction;

		int	pars_request(std::string line);
		std::string	responce();
		std::string	get_methods();
		std::string	get_path();
		std::string	get_http_version();
		std::string	get_host();
		std::string	get_body_size();
		std::string	get_connection_status();

		int	set_current_server(std::vector<Server> sct);
		int check_host(std::string line, std::vector<std::pair<int, std::string> > lst_server);
		void	print_instruction();

	private :

		int	_end;
		std::vector<location_block> dependance;

		void	init_default_error();
		void	init_file_type();
		void	init_instruction();

		int fill_string(std::string str);
		int pars_body(std::string str);
		int	fill_body(std::string str);

		/* get_first_line */
		int get_first_line(std::string *line);
		int get_line(std::string *line);
		int get_method(std::string *line);
		int get_path(std::string *line);
		int get_http_version(std::string *line);


		int	check_request();
		int	check_connection();
		int	check_length();
		int	check_method_post();

		void	print_var();
		void	reverse(char str[], int length);

		int check_path_for_location(Server cur, std::string path);
		int deep_location(std::string path, location_block stc);
		void	complete_location_path(std::vector<Server> lst_inf);
};


#endif
