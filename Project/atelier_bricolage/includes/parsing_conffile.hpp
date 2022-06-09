#ifndef PARSING_CONFFILE_HPP
#define PARSING_CONFFILE_HPP

#include <vector>
#include <string>
#include <iostream>


/* everything is string to helps concatenation for us */
/* if a variable can be variables, then us vector */

class location_block {

	public :
	std::string     uri; //(6.3)
    std::string     root; //(6.1)
    std::string     index; //(6.5)
    std::string     autoindex; //(6.4)
    int             max_client; //(5)
    std::vector<std::string>             allow_methods; //(6.1)
    std::vector<std::pair<std::string, std::string> >  error_page;
    std::vector<std::pair<std::string, std::string> >  redirection;
	std::vector<std::string> lst_location_option;

	std::vector<location_block>	location;

	location_block();	
	location_block(location_block  const &cpy);
	~location_block();

	location_block &operator=(location_block const &cpy);    // need getter to, same as server_block

	void init_lst_location_option();
	location_block	find_uri(std::string uri_to_find);

	int	pos_uri(std::string uri_to_find);
};


class   server_block {
    public :
		std::string     server_name; //(2)
		std::string     root; // (6.1)
		std::string     index;
		std::string     autoindex; // (6.4)
		std::vector<std::pair<int , std::string> >     port_ip; //(1) un serveur peu listen sur plusieur port et ip
	// pair pour port et ip, ex : 8000 127.0.0.1; par defaut, ip 127.0.0.1
		int             max_client; //(5)
	/*pair link code error and root error_page, there can be multiple error_page define (vector)*/
		std::vector<std::pair<std::string, std::string> >  error_page; //(6.2) (4)  
		std::vector<std::pair<std::string, std::string> >  redirection; // (6.2)
		std::vector<std::string>    allow_methods; //(6.5)
		std::vector<location_block> location; //(6.3)
		std::vector<std::string> lst_server_option;
		std::vector<std::string> lst_location_option;

    // need getter
    // using string.size() to determine if string is empty can be usefull ex: error_page
    // 
	server_block();
	server_block(server_block const &cpy);
	~server_block();

	server_block &operator=(server_block const &cpy);

	void init_lst_server_option();
	void init_lst_location_option();

	int nbr_location() ;

	location_block	find_uri(std::string uri_to_find);

	int	pos_uri(std::string uri_to_find);

};

class   server_conf {

    public : 
    std::vector<server_block> server;

	server_conf();
	server_conf(server_conf const &cpy);
	~server_conf();

	server_conf &operator=(server_conf const &cpy);

};

#endif
