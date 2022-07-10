#include "../includes/parsing_request.hpp"
#include "../includes/utils.hpp"
#include "../includes/webserve.hpp"

// check request a finir
// check host a fair

request::request() : methods(), path(), http_version(), request_body(), body(), chunked(-1), validity(200), cur_serv_index(), in_location(), location_path(), autoindex_on(), filename(), _end(0) , dependance(){
	init_default_error();
	init_file_type();
	init_instruction();




}

request::request(std::string line, std::vector<Server> lst_inf) : methods(), path(), http_version(), request_body(),body(), chunked(-1), validity(200), cur_serv_index(), in_location(), location_path(), autoindex_on(), filename(),_end(0), dependance(){
	init_default_error();
	init_file_type();
	init_instruction();

	if (line.size() != 0) {
		std::cout << std::endl;
	}

//	std::string test_request1 = "GET /hello.html HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\n\r\n";
//	std::string test_request2 = "GET /hello.html HTTP/1.1\r\nContent-Length: 8\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\n\r\nbonjours";
//	std::string test_request2 = "POST /test.html HTTP/1.1\r\nHost: localhost:8001\r\nContent-Type: application/json\r\nContent-Length: 67\r\nContent-Disposition: filename=\"test.exemple.txt\"\r\n\r\n{\"Id\": 12345,\"Customer\": \"John Smith\",\"Quantity\": 1,\"Price\": 10.00}";


	int res;

	if ((res = pars_request(line)) != 0) {
		std::cout << "bad request" << std::endl;
		validity = 400;
		return;
	}
//	else
//		std::cout << "first line good" << std::endl;
//	print_instruction();
//	print_var();
	
//	if (res == 0)	
//		std::cout << "good request" << std::endl;
	if (lst_inf.size() == 0) {
		std::cout << "no server set" << std::endl;
		validity = 400;
		return ;
	}


	if (set_current_server(lst_inf) != 0) {
	//	std::cout << "bad path request" << std::endl;
		if (cur_serv_index != -1)
			complete_location_path(lst_inf);
			/*
		std::cout << "location path :" << std::endl;
		std::cout << "uri :"  << location_path.uri<< std::endl;
		std::cout << "root :"  << location_path.root<< std::endl;
		std::cout << "index:"  << location_path.index<< std::endl;
		std::cout << "max_client :"  << location_path.max_client<< std::endl;
		*/
		std::vector<std::string>::iterator ite = location_path.allow_methods.begin();
		while (ite != location_path.allow_methods.end()) {
		//	std::cout << "allow:"  << *ite<< std::endl;
			ite++;
		}

		std::vector<std::pair<std::string,std::string> >::iterator ite1 = location_path.error_page.begin();
		while (ite1 != location_path.error_page.end()) {
			//std::cout << "error :" << ite1->first << "|" << ite1->second << std::endl;
			ite1++;
		}
		std::vector<std::pair<std::string,std::string> >::iterator ite2 = location_path.redirection.begin();
		while (ite2 != location_path.redirection.end()) {
//			std::cout << "redirection :" << ite2->first << "|" << ite2->second << std::endl;
			ite2++;
		}

		validity = 400;
		return ;
	}
		complete_location_path(lst_inf);
	/*
		std::cout << "location path :" << std::endl;
		std::cout << "uri :"  << location_path.uri<< std::endl;
		std::cout << "root :"  << location_path.root<< std::endl;
		std::cout << "index:"  << location_path.index<< std::endl;
		std::cout << "max_client :"  << location_path.max_client<< std::endl;
	*/

		std::vector<std::string>::iterator ite = location_path.allow_methods.begin();
		while (ite != location_path.allow_methods.end()) {
	//		std::cout << "allow:"  << *ite<< std::endl;
			ite++;
		}

		std::vector<std::pair<std::string,std::string> >::iterator ite1 = location_path.error_page.begin();
		while (ite1 != location_path.error_page.end()) {
	//		std::cout << "error :" << ite1->first << "|" << ite1->second << std::endl;
			ite1++;
		}
		//boucle inf ????
	//	std::vector<std::pair<std::string,std::string> >::iterator ite2 = location_path.redirection.begin();
	/*	while (ite2 != location_path.redirection.end()) {
			std::cout << "redirection :" << ite2->first << "|" << ite2->second << std::endl;
			ite2++;
		}
		*/



}

request::request(request const & cpy) {
	methods = cpy.methods;
	path = cpy.path;
	http_version = cpy.http_version;
	body = cpy.body;
	chunked = cpy.chunked;
	validity = cpy.validity;
	map_error = cpy.map_error;
	map_file_type= cpy.map_file_type;
	instruction = cpy.instruction;
	cur_serv_index = cpy.cur_serv_index;
	in_location = cpy.in_location;
	location_path = cpy.location_path;
	dependance = cpy.dependance;
	filename = cpy.filename;
	autoindex_on = cpy.autoindex_on;
	_end = cpy._end;
}

request &request::operator=(request const & cpy) {
	methods = cpy.methods;
	path = cpy.path;
	http_version = cpy.http_version;
	body = cpy.body;
	chunked = cpy.chunked;
	validity = cpy.validity;
	map_error = cpy.map_error;
	map_file_type= cpy.map_file_type;
	instruction = cpy.instruction;
	cur_serv_index = cpy.cur_serv_index;
	in_location = cpy.in_location;
	location_path = cpy.location_path;
	dependance = cpy.dependance;
	filename = cpy.filename;
	autoindex_on = cpy.autoindex_on;
	
	_end = cpy._end;
	return (*this);
}

request::~request() {}

/*-------------Class Coplienne end------------*/

/*-------------printer-------------------*/
void	request::print_instruction() {
	for (std::map<std::string, std::string>::iterator ite(instruction.begin()) ; ite != instruction.end(); ite++) {
	
		std::cout << "instruction: " << ite->first << " , " ;
		if (ite->second  != "")
			std::cout << ite->second;
		std::cout << std::endl;
	}
}

void	request::print_var() {
	std::cout << "methods: " << get_methods() << std::endl;
	std::cout << "path: " << get_path() << std::endl;
	std::cout << "http_version: " << get_http_version() << std::endl;

	std::cout << "host: " << get_host() << std::endl;
	std::cout << "body_size: " << get_body_size() << std::endl;
	std::cout << "connection: " << get_connection_status() << std::endl;
	std::cout << "body:" << body << std::endl;

}

/*-------------printer end-------------------*/

/*------------------getter------------------*/

std::string	request::get_methods() {
	return methods;
}

std::string	request::get_path() {
	return path;
}

std::string request::get_http_version() {
	return http_version;
}

std::string request::get_host() {
	return instruction["Host"];
}

std::string request::get_body_size() {
	return instruction["Content-Length"];
}

std::string request::get_connection_status() {
	return instruction["Connection"];
}

/*------------------getter end------------------*/

/*-----------------parsing first line---------*/

int	request::get_method(std::string *line) {
	size_t start;

	if ((start = line->find("GET")) == 0) {
		methods = line->substr(0, 3);
		line->erase(0, 3);
		return 0;
	} 
	if ((start = line->find("POST")) == 0) {
		methods = line->substr(0, 4);
		line->erase(0, 4);
		return 0;
	}
	if ((start = line->find("DELETE")) == 0) {
		methods = line->substr(0, 6);
		line->erase(0, 6);
		return 0;
	}
	return (1);
}

int	request::get_path(std::string *line) {
	size_t start;
	if (line->find(" ") != 0) {
		return 1;
	}
	line->erase(0,1);
	if ((start = line->find(" ")) == std::string::npos)
		return 1;
	if (line->find("/") != 0)
		return 1;
	path = line->substr(0, start);
	line->erase(0, start);
	return 0;
}

int	request::get_http_version(std::string *line) {
	if (line->find(" ") != 0)
		return 1;
	line->erase(0, 1);
	if (*line != "HTTP/1.1" && *line != "HTTP/1.0") {
		return 1;
	}
	http_version = *line;
	return 0;
}

int	request::get_first_line(std::string *line) {
	if (get_method(line) != 0) {
		return 1;
	}
	if (get_path(line) != 0) {
		return 1;
	}	
	if (get_http_version(line) != 0) {
		return 1;
	}
	return 0;
}
/*-----------------parsing first line end---------*/

/*---------------------get_line-------------------*/

int	request::get_line(std::string *line) {
	size_t	pos;
	std::string		sub_line;
	std::map<std::string, std::string>::iterator last(instruction.end());
	std::map<std::string, std::string>::iterator	ite; 

	if ((pos = line->find(": ")) == std::string::npos) {
		return 1;
	}
	if (line->find(" ") != pos+1) {
		return 1;
	}
	if ((ite = instruction.find(line->substr(0, pos))) != last) {
		instruction[(line->substr(0, pos))] = line->substr(pos + 2);
	}
	return 0;


}

/*---------------------get_line end-------------------*/

/*--------------checker--------------------*/
int	request::check_connection() {
	if (instruction["Connection"] == "") {
		if (http_version == "HTTP/1.1") {
			instruction["Connection"] = "keep-alive";
		}
		else
			instruction["Connection"] = "close";

	}
	else {
		if (instruction["Connection"] != "keep-alive" && instruction["Connection"] != "close") {
			std::cout << "Header Connection doesn't support other string than keep-alive or close" << std::endl;
			return 400;
		}
	}
	return 0;
}


int	request::check_length() {
	if (instruction["Content-Length"] != "") {
		if (instruction["Content-Length"].find_first_not_of("0123456789") != std::string::npos) {
			return 1;
		}
	}
	return 0;
}

int	request::check_method_post() {
	if (instruction["Content-Length"] == "" && instruction["Transfer-Encoding"] == "chunked") {
		// prevoir pour le cas chunked
		_end = 1;
	}
	else if (check_length() != 0)
		return 411;
	return 0;
}

// case one path = /cat 
//		go found /cat location in lst location
//
// case two path = /cat/bite
//		go found /cat/bite location in lst location
//			else
//				go found /cat lcation
//					go found /bite location in /cat location



void	request::complete_location_path(std::vector<Server> lst_inf) {
	location_block ret;

	std::vector<location_block>::iterator ite = dependance.begin();

	if (lst_inf[cur_serv_index].infos.root.size() != 0)
		ret.root = lst_inf[cur_serv_index].infos.root;
	if (lst_inf[cur_serv_index].infos.index.size())
		ret.index= lst_inf[cur_serv_index].infos.index;
	if (lst_inf[cur_serv_index].infos.autoindex.size())
		ret.autoindex = lst_inf[cur_serv_index].infos.autoindex;
	if (lst_inf[cur_serv_index].infos.max_client != 0)
		ret.max_client= lst_inf[cur_serv_index].infos.max_client;
	if (lst_inf[cur_serv_index].infos.allow_methods.size()!= 0)
		ret.allow_methods = lst_inf[cur_serv_index].infos.allow_methods;
	if (lst_inf[cur_serv_index].infos.error_page.size()!= 0)
		ret.error_page= lst_inf[cur_serv_index].infos.error_page;
	if (lst_inf[cur_serv_index].infos.redirection.size()!= 0)
		ret.redirection = lst_inf[cur_serv_index].infos.redirection;
	ret.autoindex = lst_inf[cur_serv_index].infos.autoindex;
	ret.cgi_path = lst_inf[cur_serv_index].infos.cgi_path;
	
	while (ite != dependance.end()) {
		if (ite->uri.size() != 0)
			ret.uri = ite->uri;
		if (ite->root.size() != 0)
			ret.root= ite->root;
		if (ite->index.size() != 0)
			ret.index= ite->index;
		if (ite->autoindex.size() != 0)
			ret.autoindex= ite->autoindex;
		if (ite->max_client != 0)
			ret.max_client = ite->max_client;
		if (ite->allow_methods.size() != 0)
			ret.allow_methods = ite->allow_methods;
		if (ite->error_page.size() != 0)
			ret.error_page = ite->error_page;
		if (ite->redirection.size() != 0)
			ret.redirection= ite->redirection;
		ret.autoindex =  ite->autoindex;
		ret.cgi_path =  ite->cgi_path;
		ite++;
	}
	location_path = ret;

}

/*	How to use */
/*	if cur_serv_index == -1, pas de server trouver donc 404default */
/*	sinon cur_serv_index est la pos dans le vector de server */
/*	*/
/*	if in_location != 0, pas de location trouver pour le path donc404 du server trouve */
/*	in_location == 0, location_path == le location block en question	*/

int	request::deep_location(std::string path, location_block stc) {
	//check for filename
	//
//	std::cout << "deep location neew path :" << path << std::endl;
	if (path.find("/") == std::string::npos){
		size_t pos;
//		std::cout << "In location BITE" << std::endl;
		if (path == "") {
			std::cout << "Check pour le autoindex on " << std::endl;
			autoindex_on = 1;
			return 0;
		}
		// if path == "" && autoindex on alors go lst les dir 
		if ((pos = path.find(".")) != std::string::npos && map_file_type.find(path.substr(pos)) != map_file_type.end())
		{
//			std::cout << "BJR LES AMICHE in location" << std::endl;
			this->filename = path;
			return 0;
		}
	}
	if (stc.location.size() == 0)
		return 1;
	std::vector<location_block>::iterator ite = stc.location.begin();

//	std::cout << "new path uri:" << path << std::endl;
	while (ite != stc.location.end()) {
		if (ite->uri == path) {
			dependance.push_back(*ite);
			location_path = *ite;
			return 0;
		}
		ite++;
	}

	size_t end;
	if ((end = path.find("/")) != std::string::npos) {
		std::string new_str(path.substr(0, end));
		ite = stc.location.begin();
		while (ite != stc.location.end()) {
			if (ite->uri == new_str) {
				std::string second_str(path, end+ 1);
			dependance.push_back(*ite);
				return (deep_location(second_str, *ite));			
			}
			ite++;
		}
	}
	return 1;
}



int	request::check_path_for_location(Server cur, std::string path) {
	std::string sub(path, 1);
	size_t end;
	std::vector<location_block>::iterator ite = cur.infos.location.begin();

	// check if path == /filename
	// 
	if (path.find_last_of("/") == 0){
		size_t pos;
//		std::cout << "BITE" << std::endl;
		if ((pos = path.find(".")) != std::string::npos && map_file_type.find(path.substr(pos)) != map_file_type.end())
		{
//			std::cout << "BJR LES AMICHE" << std::endl;
			this->filename = path.substr(1);
			this->path = "/";
			return 0;
		}
		if (pos != std::string::npos && cur.infos.cgi_path.size() != 0) {
			if (cur.infos.cgi_path[0].first == path.substr(pos)) {
			this->filename = path.substr(1);
			this->path = "/";
			return 0;
			}
		}
		std::cout << "request check path for location :" << "path is not a file" << std::endl;
	//	return 1;
	}

	if (cur.infos.location.size() == 0)
		return 1;
//	std::cout << "new path uri: " << path << std::endl;
//	std::cout << "first step in check path" << std::endl;
	std::cout << _RED << "test request check path for location"  << _NOR<< std::endl;
	while (ite != cur.infos.location.end()) {
		if (ite->uri == path.substr(1)) {
			dependance.push_back(*ite);
			location_path = *ite;
			std::cout << _RED << "exit at bouble while" << _NOR << std::endl;
			return 0;
		}
		ite++;
	}	
	if (path.find_last_of("/") == 0) {
		std::cout << _RED << "path doesn't exist"  << _NOR<< std::endl;
	}
	std::cout << _RED << "test request check path for location" <<  _NOR<< std::endl;
	if ((end = sub.find("/")) != std::string::npos) {
		std::string first_loc(sub.substr(0, end));

//		std::cout << "check for a multi locatioh path: " << first_loc << std::endl;
		ite = cur.infos.location.begin();
		while (ite != cur.infos.location.end()) {
//			std::cout << "test:" << ite->uri << std::endl;
			if (ite->uri == first_loc) {
				std::string second_loc(path, end + 2);
			dependance.push_back(*ite);
//				std::cout << "test2 second loc:" << second_loc << std::endl;
				if (deep_location(second_loc, *ite) == 0)
					return 0;
				return 1;
			}
			ite++;
		}
		return 1;
	}
	return 1;


}

int	request::set_current_server(std::vector<Server> lst_server){

	int i;
	int j = 0;
	std::vector<Server>::iterator ite = lst_server.begin();
	while (ite != lst_server.end())
	{
		i = check_host(ite->infos.server_name, ite->infos.port_ip);
		if (i == 1)
			break; 
		j++;
		ite++;
	}
	if (ite == lst_server.end()) {
		cur_serv_index = -1;
//		std::cout << "Host header invalide" << std::endl;
		return 1;
	}

	cur_serv_index = j;
//	std::cout << "cur server index:" << j << std::endl;

		std::cout << "path in set_cureent_server:" << path << "|" << std::endl;
	if (path != "/") {
		std::cout << "go to check location " << std::endl;
	if ((in_location = check_path_for_location(lst_server[j], path)) != 0) {
//		std::cout << "no location found for this path" << std::endl;
		return 1;
	}
	}
	return 0;

}

int	request::check_request() {
	if (http_version == "HTTP/1.1") {

		if (instruction["Host"] == "") {
			std::cout << "HTTP/1.1 request need Host Header" << std::endl;
			return 400;
		}
	}
	if (check_connection() != 0) {
		return 400;
	}
	if (methods == "POST") {
		return check_method_post();
	}
	else if (methods == "GET" || methods == "DELETE") {
		return 0;
	}
	if (_end == 0 && methods == "POST") {
		std::string str(instruction["Content-Type"]);

	}

	return 0;


}

int	request::fill_body(std::string line) {
	body += line;
	return 1;
}

int	request::pars_body(std::string line) {
	std::cout << "line en question: " << line << std::endl;
	std::cout << "line lenght en question: " << line.length() << std::endl;
	fill_body(line);
/*	if (line.length() != static_cast<unsigned long>(atoi(instruction["Content-Length"].c_str()))) {
		std::cout << "Header Content-Length support only number:" << instruction["Content-Length"] << "|"<< std::endl;
		return 400;
	}*/
	return 0;

}


/* check Host */
/* Changer le localHost par server_name */

int		request::check_host(std::string server_name, std::vector<std::pair<int, std::string> > port) {

	
	std::string rd(int_to_str(port[0].first));
	std::string localhost("localhost");
	
	std::string line(instruction["Host"]);
	if (line == localhost + ":" + rd) {
//		std::cout << "Host fund in server config file" << std::endl;
		return 1;

	}
	else if (line == port[0].second + ":" + rd || line == server_name) {

//		std::cout << "Host fund in server config file" << std::endl;
		return 1;
	}
	return 0;
}

/*-------ret for responce------*/


std::string request::responce(){
	std::string ret;
	std::string validity_s;

	validity_s = int_to_str(validity);
	std::string validity_ret(map_error[validity_s]);


	ret = http_version + " " + validity_s + " " + validity_ret + "\r\n";
	for (std::map<std::string, std::string>::iterator ite = instruction.begin(); ite != instruction.end(); ite++) {
		if (ite->second != "") {
			ret += ite->first + ": ";
			ret += ite->second + "\r\n";
		}
	}
	ret += "\r\n";
	if (body != "" && chunked == -1) {
		ret += body;
	}
	if (chunked != -1) {
		ret += body;
	}
	return ret;
}

/*-------ret for responce end------*/
/*--------------start----------------*/

int	request::fill_string(std::string str) {
	std::string line;

	int			start = 0;
	size_t		end;
	size_t		line_len;
	if (str.find("\r\n\r\n") == std::string::npos) {
		std::cout << "request have not end, requuest:" << std::endl;
		std::cout << str << std::endl;
		std::cout << "at start fill_string" << std::endl;
		return 400;
	}
	while (str.find("\r\n\r\n") != 0) {
		line_len = str.find("\r\n");
		end	= str.find("\r\n\r\n");
		
		line = str.substr(0, line_len);
		if (line_len == 0) {
			if (_end == 0) {
				return 400;
			}
			else
				start++;
		}
//		std::cout << "line a lire: " << line << "|" <<std::endl;
		if (start == 0){
			start++;
			if (get_first_line(&line) != 0) {
				std::cout << "first line no valide"<< std::endl;
				return 400;
			}
		}
		else if (start == 1) {
			if (get_line(&line) != 0) 
				return 400;
		}
		str = str.substr(line_len + 2);
		if (line_len == end)
			break;
	}

	//std::cout << "heh:" << str << "|" << std::endl;
	if (str != "\r\n") {
		std::cout << "request pas fini, il y a un body:" << std::endl;
		std::cout << str<< std::endl;
	//	std::cout << "il y a un body:" << str.substr(2) << "|"<<  std::endl;
		if (pars_body(str.substr(2)) != 0) {
			std::cout << "pars_body invalide" << std::endl;
			return 400;
		}
		std::cout << std::endl;
		std::cout << "body request pars" << std::endl;
		
	}
	std::cout << "go to responce" << std::endl;
//	std::cout << responce() << std::endl;
	return 0;
}

int	request::pars_request(std::string str) {
	int ret;

	if ((ret = fill_string(str)) != 0)
		return ret;
	std::cout << "toute les lignes sont lues" << std::endl;

	int res;
	if ((res = check_request()) != 0)
		return res;

	return 0;
}
/*--------------start----------------*/
/*----------------------------------init instruction-----------------*/

void	request::init_instruction() {
	instruction["Accept-Charsets"] = ""; // list des caracteres 
	instruction["Auth-Scheme"] = "";
	instruction["Authorization"] = "";
	instruction["Content-Language"] = "";
	instruction["Content-Length"] = ""; // max body size // only num
	instruction["Content-Location"] = "";
	instruction["Content-Type"] = ""; // to pars aussi
	instruction["Date"] = "";
	instruction["Host"] = ""; // Host: <host>:<port> post optionnel
	instruction["Last-Modified"] = "";
	instruction["Location"] = "";
	instruction["Referer"] = "";
	instruction["Retry-After"] = "";
	instruction["Transfer-Encoding"] = ""; // if chunked follow by the lengt in hexa, 0 is the last chunked; Content-Lenght is ommited
	instruction["User-Agent"] = "";
	instruction["Connection"] = ""; // keep-Alive || close; par defaut close en http/1.0 et keep-Alive en http1.1
	instruction["Accept"] = ""; // map file type accepted
	instruction["Accept-Encoding"] = "";
	instruction["From"] = "";
	instruction["Content-Disposition"] = "";

}

void request::init_file_type() {

    	map_file_type[".aac"]      = "audio/aac";
	map_file_type[".abw"]      = "application/x-abiword";
	map_file_type[".arc"]      = "application/octet-stream";
	map_file_type[".avi"]      = "video/x-msvideo";
	map_file_type[".azw"]      = "application/vnd.amazon.ebook";
	map_file_type[".bin"]      = "application/octet-stream";
	map_file_type[".bz"]       = "application/x-bzip";
	map_file_type[".bz2"]      = "application/x-bzip2";
	map_file_type[".csh"]      = "application/x-csh";
	map_file_type[".css"]      = "text/css";
	map_file_type[".csv"]      = "text/csv";
	map_file_type[".doc"]      = "application/msword";
	map_file_type[".epub"]     = "application/epub+zip";
	map_file_type[".gif"]      = "image/gif";
	map_file_type[".htm"]      = "text/html";
	map_file_type[".html"]     = "text/html";
	map_file_type[".ico"]      = "image/x-icon";
	map_file_type[".ics"]      = "text/calendar";
	map_file_type[".jar"]      = "Temporary Redirect";
	map_file_type[".jpeg"]     = "image/jpeg";
	map_file_type[".jpg"]      = "image/jpeg";
	map_file_type[".js"]       = "application/js";
	map_file_type[".json"]     = "application/json";
	map_file_type[".mid"]      = "audio/midi";
	map_file_type[".midi"]     = "audio/midi";
	map_file_type[".mpeg"]     = "video/mpeg";
	map_file_type[".mp4"]      = "video/mp4";
	map_file_type[".mpkg"]     = "application/vnd.apple.installer+xml";
	map_file_type[".odp"]      = "application/vnd.oasis.opendocument.presentation";
	map_file_type[".ods"]      = "application/vnd.oasis.opendocument.spreadsheet";
	map_file_type[".odt"]      = "application/vnd.oasis.opendocument.text";
	map_file_type[".oga"]      = "audio/ogg";
	map_file_type[".ogv"]      = "video/ogg";
	map_file_type[".ogx"]      = "application/ogg";
	map_file_type[".png"]      = "image/png";
	map_file_type[".pdf"]      = "application/pdf";
	map_file_type[".ppt"]      = "application/vnd.ms-powerpoint";
	map_file_type[".rar"]      = "application/x-rar-compressed";
	map_file_type[".rtf"]      = "application/rtf";
	map_file_type[".sh"]       = "application/x-sh";
	map_file_type[".svg"]      = "image/svg+xml";
	map_file_type[".swf"]      = "application/x-shockwave-flash";
	map_file_type[".tar"]      = "application/x-tar";
	map_file_type[".tif"]      = "image/tiff";
	map_file_type[".tiff"]     = "image/tiff";
	map_file_type[".ttf"]      = "application/x-font-ttf";
	map_file_type[".txt"]      = "text/plain";
	map_file_type[".vsd"]      = "application/vnd.visio";
	map_file_type[".wav"]      = "audio/x-wav";
	map_file_type[".weba"]     = "audio/webm";
	map_file_type[".webm"]     = "video/webm";
	map_file_type[".webp"]     = "image/webp";
	map_file_type[".woff"]     = "application/x-font-woff";
	map_file_type[".xhtml"]    = "application/xhtml+xml";
	map_file_type[".xls"]      = "application/vnd.ms-excel";
	map_file_type[".xml"]      = "application/xml";
	map_file_type[".xul"]      = "application/vnd.mozilla.xul+xml";
	map_file_type[".zip"]      = "application/zip";
	map_file_type[".3gp"]      = "video/3gpp audio/3gpp";
	map_file_type[".3g2"]      = "video/3gpp2 audio/3gpp2";
	map_file_type[".7z"]       = "application/x-7z-compressed";
	map_file_type[".txt"]      = "text/plain";


}



void request::init_default_error() {


    map_error[ "100" ] = "Continue";
    map_error[ "101" ] = "Switching Protocols";
    map_error[ "102" ] = "Processing";
    map_error[ "103" ] = "Checkpoint";

    map_error[ "200" ] = "OK";
    map_error[ "201" ] = "Created";
    map_error[ "202" ] = "Accepted";
    map_error[ "203" ] = "Non-Authoritative Information";
    map_error[ "204" ] = "No Content";
    map_error[ "205" ] = "Reset Content";
    map_error[ "206" ] = "Partial Content";
    map_error[ "207" ] = "Multi-Status";
    map_error[ "208" ] = "Already Reported";

    map_error[ "300" ] = "Multiple Choices";
    map_error[ "301" ] = "Moved Permanently";
    map_error[ "302" ] = "Found";
    map_error[ "303" ] = "See Other";
    map_error[ "304" ] = "Not Modified";
    map_error[ "305" ] = "Use Proxy";
    map_error[ "306" ] = "Switch Proxy";
    map_error[ "307" ] = "Temporary Redirect";
    map_error[ "308" ] = "Permanent Redirect";

    map_error[ "400" ] = "Bad Request";
    map_error[ "401" ] = "Unauthorized";
    map_error[ "402" ] = "Payment Required";
    map_error[ "403" ] = "Forbidden";
    map_error[ "404" ] = "Not Found";
    map_error[ "405" ] = "Method Not Allowed";
    map_error[ "406" ] = "Not Acceptable";
    map_error[ "407" ] = "Proxy Authentication Required";
    map_error[ "408" ] = "Request Timeout";
    map_error[ "409" ] = "Conflict";
    map_error[ "410" ] = "Gone";
    map_error[ "411" ] = "Length Required";
    map_error[ "412" ] = "Precondition Failed";
    map_error[ "413" ] = "Payload Too Large";
    map_error[ "414" ] = "URI Too Long";
    map_error[ "415" ] = "Unsupported Media Type";
    map_error[ "416" ] = "Requested Range Not Satisfiable";
    map_error[ "417" ] = "Expectation Failed";
    map_error[ "418" ] = "I'm a teapot";
    map_error[ "421" ] = "Misdirected Request";
    map_error[ "422" ] = "Unprocessable Entity";
    map_error[ "423" ] = "Locked";
    map_error[ "424" ] = "Failed Dependency";
    map_error[ "426" ] = "Upgrade Required";
    map_error[ "428" ] = "Precondition Required";
    map_error[ "429" ] = "Too Many Request";
    map_error[ "431" ] = "Request Header Fields Too Large";
    map_error[ "451" ] = "Unavailable For Legal Reasons";

    map_error[ "500" ] = "Internal Server Error";
    map_error[ "501" ] = "Not Implemented";
    map_error[ "502" ] = "Bad Gateway";
    map_error[ "503" ] = "Service Unavailable";
    map_error[ "504" ] = "Gateway Timeout";
    map_error[ "505" ] = "HTTP Version Not Supported";
    map_error[ "506" ] = "Variant Also Negotiates";
    map_error[ "507" ] = "Insufficient Storage";
    map_error[ "508" ] = "Loop Detected";
    map_error[ "510" ] = "Not Extended";
    map_error[ "511" ] = "Network Authentication Required";

}
