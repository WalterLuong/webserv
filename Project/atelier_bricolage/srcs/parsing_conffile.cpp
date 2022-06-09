#include "../includes/parsing_conffile.hpp"


/*---location block class---*/

location_block::location_block() :  uri(), root(), index(), autoindex(), max_client(0), allow_methods(), error_page(), redirection(), location() {
	init_lst_location_option();
}

location_block::location_block(location_block const &cpy) {
		uri = cpy.uri;
		root= cpy.root;
		autoindex= cpy.autoindex;
		index= cpy.index;
		max_client= cpy.max_client;
		allow_methods= cpy.allow_methods;
		error_page= cpy.error_page;
		redirection= cpy.redirection;
		location= cpy.location;
		lst_location_option = cpy.lst_location_option;
}

location_block::~location_block() {};

location_block	&location_block::operator=(location_block const &cpy) {
		uri = cpy.uri;
		root= cpy.root;
		autoindex= cpy.autoindex;
		index= cpy.index;
		max_client= cpy.max_client;
		allow_methods= cpy.allow_methods;
		error_page= cpy.error_page;
		redirection= cpy.redirection;
		location= cpy.location;
		lst_location_option = cpy.lst_location_option;
		return *this;
}

/*---location block function---*/

void	location_block::init_lst_location_option() {
	lst_location_option.push_back("index");
	lst_location_option.push_back("error_page");
	lst_location_option.push_back("location");
	lst_location_option.push_back("autoindex");
	lst_location_option.push_back("root");
	lst_location_option.push_back("cgi_path");
	lst_location_option.push_back("methods");
	lst_location_option.push_back("return");
	lst_location_option.push_back("}");

}

location_block	location_block::find_uri(std::string uri_to_find) {
		std::vector<location_block>::iterator it;

		it = location.begin();
		while (it != location.end()) {
			if (it->uri == uri_to_find)
				return (*it);
			it++;
		}
		return (*it);
}

int location_block::pos_uri(std::string uri_to_find) {
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

/*---end location block class---*/


/*---server_block class---*/

server_block::server_block() : server_name(), root(), index(), autoindex(), port_ip(), max_client(0), error_page(), redirection(), allow_methods(), location() {
	init_lst_server_option();
	init_lst_location_option();
}



server_block::server_block(server_block const &cpy) {
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
		lst_server_option = cpy.lst_server_option;
		lst_location_option = cpy.lst_location_option;
}

server_block::~server_block() {}

server_block &server_block::operator=(server_block const &cpy) {
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
		lst_server_option = cpy.lst_server_option;
		lst_location_option = cpy.lst_location_option;
		return *this;
}

/*---location block function---*/

void	server_block::init_lst_server_option() {
	lst_server_option.push_back("listen");
	lst_server_option.push_back("server_name");
	lst_server_option.push_back("index");
	lst_server_option.push_back("max_body_client");
	lst_server_option.push_back("error_page");
	lst_server_option.push_back("location");
	lst_server_option.push_back("autoindex");
	lst_server_option.push_back("root");
	lst_server_option.push_back("cgi_path");
	lst_server_option.push_back("methods");
	lst_server_option.push_back("return");
	lst_server_option.push_back("}");
}

void	server_block::init_lst_location_option() {
	lst_location_option.push_back("index");
	lst_location_option.push_back("error_page");
	lst_location_option.push_back("location");
	lst_location_option.push_back("autoindex");
	lst_location_option.push_back("root");
	lst_location_option.push_back("cgi_path");
	lst_location_option.push_back("methods");
	lst_location_option.push_back("return");
	lst_location_option.push_back("}");

}

int	server_block::nbr_location() { return location.size(); }

location_block	server_block::find_uri(std::string uri_to_find) {
			std::vector<location_block>::iterator it;

			it = location.begin();
			while (it != location.end()) {
				if (it->uri == uri_to_find) {
					return (*it);
				}
				it++;
			}
			return (*it);
}

int	server_block::pos_uri(std::string uri_to_find) {
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

/*---end server block class---*/


/*---server_conf class---*/

server_conf::server_conf() : server() {}

server_conf::server_conf(server_conf const &cpy) {
		server = cpy.server;
}

server_conf	&server_conf::operator=(server_conf const &cpy) {
	server = cpy.server;
	return *this;
}

server_conf::~server_conf() {}

/*---end server_conf class---*/
