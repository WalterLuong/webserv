#ifndef PARSING_CONFFILE_HPP
#define PARSING_CONFFILE_HPP

#include <vector>
#include <string>


/* everything is string to helps concatenation for us */
/* if a variable can be variables, then us vector */

class location_block {
	public :
   /* done */ std::string     uri; //(6.3)
    std::string     root; //(6.1)
    std::string     index; //(6.5)
    std::string     autoindex; //(6.4)
    int             max_client; //(5)
    std::vector<std::string>             allow_methods; //(6.1)
    std::vector<std::pair<std::string, std::string> >  error_page;
    std::vector<std::pair<std::string, std::string> >  redirection;

	std::vector<location_block>	location;

	location_block() : uri(), root(), index(), autoindex(), max_client(0), allow_methods(), error_page(), redirection(), location() {};
    // need getter to, same as server_block
};


class   server_block {
    public :
/* done */    std::string     server_name; //(2)
  /* done */  std::string     root; // (6.1)
  /* done */  std::string     index;
  /* done */  std::string     autoindex; // (6.4)
   /* done */ std::vector<std::pair<int , std::string> >     port_ip; //(1) un serveur peu listen sur plusieur port et ip
	// pair pour port et ip, ex : 8000 127.0.0.1; par defaut, ip 127.0.0.1
 /* done */   int             max_client; //(5)
	/*pair link code error and root error_page, there can be multiple error_page define (vector)*/
  /* done */  std::vector<std::pair<std::string, std::string> >  error_page; //(6.2) (4)  
  /* done */  std::vector<std::pair<std::string, std::string> >  redirection; // (6.2)
   /* done */ std::vector<std::string>    allow_methods; //(6.5)
    std::vector<location_block> location; //(6.3)

    // need getter
    // using string.size() to determine if string is empty can be usefull ex: error_page
    // 
	server_block() : server_name(), root(), index(), autoindex(), port_ip(), max_client(0), error_page(), redirection(), allow_methods(), location() {};

    int nbr_location() const { return location.size(); }
};

class   server_conf {

    public : 
    std::vector<server_block> server;

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
