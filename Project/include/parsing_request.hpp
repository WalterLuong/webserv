#ifndef PARSING_REQUEST_HPP
# define PARSING_REQUEST_HPP

#include "webserve.hpp"
#include <vector>
#include <map>

class request {

	public :
		
		std::string	methods;
		std::string path;
		std::string	http_version;
		int		chunked;
		int		validity; //code status pour la map error;

		request();
		request(request const & cpy); 
		request &operator=(request const & cpy);
		~request();


		std::map<std::string, std::string>	map_error;
		std::map<std::string, std::string>	map_file_type;
		std::map<std::string, std::string>	instruction;

		int	pars_request(std::string *line);

	private :

		void	init_default_error();
		void	init_file_type();
		void	init_instruction();

		/* get_first_line */
		int get_first_line(std::string *line);
		int get_line(std::string *line);
		int get_method(std::string *line);
		int get_path(std::string *line);
		int get_http_version(std::string *line);

};


#endif
