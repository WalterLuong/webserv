#ifndef PARSING_REQUEST_HPP
# define PARSING_REQUEST_HPP


class request {

	public :
		
		std::string	methods;
		std::string	http_version;
		std::string	request;
		int		chunked;
		int		validity; //code status pour la map error;

		std::map<std::string, std::string>	map_error;
		std::map<std::string, std::string>	map_file_type;
		std::map<std::string, std::string>	instruction;

	private :

		void	init_default_error();
		void	init_file_type();
		void	init_instruction();
};


#endif
