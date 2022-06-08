#ifndef PARSING_CONFFILE_HPP
#define PARSING_CONFFILE_HPP

#include <vector>
#include <string>
#include "webserve.hpp"
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

	std::vector<location_block>	location;

	location_block() : uri(), root(), index(), autoindex(), max_client(0), allow_methods(), error_page(), redirection(), location() {};
	location_block(location_block &cpy) {
		uri = cpy.uri;
		root= cpy.root;
		autoindex= cpy.autoindex;
		index= cpy.index;
		max_client= cpy.max_client;
		allow_methods= cpy.allow_methods;
		error_page= cpy.error_page;
		redirection= cpy.redirection;
		location= cpy.location;
	}
	location_block &operator=(location_block &cpy) {
		location_block ret;	
		ret.uri = cpy.uri;
		ret.root= cpy.root;
		ret.autoindex= cpy.autoindex;
		ret.index= cpy.index;
		ret.max_client= cpy.max_client;
		ret.allow_methods= cpy.allow_methods;
		ret.error_page= cpy.error_page;
		ret.redirection= cpy.redirection;
		ret.location= cpy.location;
		return ret;
	}
    // need getter to, same as server_block
	location_block	find_uri(std::string uri_to_find) {
		std::vector<location_block>::iterator it;

		it = location.begin();
		while (it != location.end()) {
			if (it->uri == uri_to_find)
				return (*it);
			it++;
		}
		return (location_block());
	}

	int	pos_uri(std::string uri_to_find) {
		std::vector<location_block>::iterator it;
		int i = 0;

		it = location.begin();
		while (it != location.end()) {
			if (it->uri == uri_to_find)
				return (i);
			it++;
			i++;
		}
		return (-1);
	}
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

    // need getter
    // using string.size() to determine if string is empty can be usefull ex: error_page
    // 
	server_block() : server_name(), root(), index(), autoindex(), port_ip(), max_client(0), error_page(), redirection(), allow_methods(), location() {};
	server_block(server_block &cpy) {
		server_name = cpy.server_name;
		root = cpy.root;
		index = cpy.index;
		autoindex = cpy.autoindex;
		port_ip = cpy.port_ip;
		max_client = cpy.max_client;
		error_page = cpy.error_page;
		redirection = cpy.redirection;
		allow_methods = cpy.allow_methods;
		location = cpy.location;
	}
	server_block &operator=(server_block &cpy) {
		server_block ret;
		ret.server_name = cpy.server_name;
		ret.root = cpy.root;
		ret.index = cpy.index;
		ret.autoindex = cpy.autoindex;
		ret.port_ip = cpy.port_ip;
		ret.max_client = cpy.max_client;
		ret.error_page = cpy.error_page;
		ret.redirection = cpy.redirection;
		ret.allow_methods = cpy.allow_methods;
		ret.location = cpy.location;
		return ret;

	}

		int nbr_location() const { return location.size(); }

		location_block	find_uri(std::string uri_to_find) {
			std::vector<location_block>::iterator it;

			it = location.begin();
			while (it != location.end()) {
				if (it->uri == uri_to_find) {
					return (*it);
				}
				it++;
			}
			return (location_block());
		}

		int	pos_uri(std::string uri_to_find) {
			std::vector<location_block>::iterator it;
			int i = 0;

			it = location.begin();
			while (it != location.end()) {
				if (it->uri == uri_to_find)
					return (i);
				it++;
				i++;
			}
			return (-1);
		}

};

class   server_conf {

    public : 
    std::vector<server_block> server;

	server_conf() : server() {};
	server_conf(server_conf &cpy) {
		server = cpy.server;
	};

	server_conf &operator=(server_conf &cpy) {
		server_conf ret;
		ret.server = cpy.server;
		return ret;
	}

    // all getter is to use not for changing the variables (return const)
    // depending of needed,
    // server and location are vector, we can use index to reach directly to the target we need
    // need to find a solution for 0 of location, thinking about use abstract number like -int
    // need getter to determine index of target
    bool    is_autoindex_on(int server_index, int location_index) const;
    const std::vector<std::string>    get_allow_methods(int server_index, int location_index) const;
    const std::string   get_root(int server_index, int location_index) const;
    const std::string   get_index(int server_index, int location_index) const;
    const std::string   get_server_name(int server_index) const;
    int                 get_max_clien(int server_index, int location_index) const;
    const std::pair<std::string, std::string> get_error_page(int server_index, int location_index) const;
    const std::pair<std::string, std::string> get_redirection(int server_index, int location_index) const;



};

#endif
