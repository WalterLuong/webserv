#ifndef PARSING_CONFFILE_HPP
#define PARSING_CONFFILE_HPP

#include <vector>
#include <string>


/* everything is string to helps concatenation for us */
/* if a variable can be variables, then us vector */

class location_block {
    std::string     uri; //(6.3)
    std::string     root; //(6.1)
    std::string     index; //(6.5)
    std::string     autoindex; //(6.4)
    int             max_client; //(5)
    std::vector<std::string>             allow_methods; //(6.1)
    std::vector<std::pair<std::string, std::string> >  error_page;
    std::vector<std::pair<std::string, std::string> >  redirection;
    // need getter to, same as server_block
};


class   server_block {
    public :
    std::string     server_name; //(2)
    std::string     root; // (6.1)
    std::string     index;
    std::string     autoindex; // (6.4)
    std::vector<std::pair<std::string, std::string> >     port_ip; //(1) un serveur peu listen sur plusieur port et ip
    int             max_client; //(5)
	/*pair link code error and root error_page, there can be multiple error_page define (vector)*/
    std::vector<std::pair<std::string, std::string> >  error_page; //(6.2) (4)  
    std::vector<std::pair<std::string, std::string> >  redirection; // (6.2)
    std::vector<std::string>    allow_methods; //(6.5)
    std::vector<location_block> location; //(6.3)

    // need getter
    // using string.size() to determine if string is empty can be usefull ex: error_page
    // 
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
