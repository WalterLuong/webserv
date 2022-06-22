#include "parsing_request.hpp"

// check request a finir
// check host a fair

request::request() : methods(), path(), http_version(), body(), chunked(-1), validity(200), _end(0) {
	init_default_error();
	init_file_type();
	init_instruction();


	std::string test_request1 = "GET /hello.html HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\n\r\n";
	std::string test_request2 = "GET /hello.html HTTP/1.1\r\nContent-Length: 8\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\n\r\nbonjours";


	int res;
	if ((res = pars_request(test_request2)) != 0) {
		std::cout << "bad request" << std::endl;
		return;
	}
	else
		std::cout << "first line good" << std::endl;
	print_instruction();
	print_var();
	
	if (res == 0)	
		std::cout << "good request" << std::endl;
	else
		std::cout << "bad request" << std::endl;

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
			return 1;
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

int	request::check_request() {
	if (http_version == "HTTP/1.1") {

		if (instruction["Host"] == "") {
			return 400;
		}
	}
	if (check_connection() != 0) {
		return 1;
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
	fill_body(line);
	if (line.length() != static_cast<unsigned long>(atoi(instruction["Content-Length"].c_str()))) {
		std::cout << "ca depasse" << std::endl;
		return 1;
	}
	return 0;

}

/*-------ret for responce------*/
char* request::itoa(int num, char* buffer, int base) {
    int curr = 0;
 
    if (num == 0) {
        // Base case
        buffer[curr++] = '0';
        buffer[curr] = '\0';
        return buffer;
    }
 
    int num_digits = 0;
 
    if (num < 0) {
        if (base == 10) {
            num_digits ++;
            buffer[curr] = '-';
            curr ++;
            // Make it positive and finally add the minus sign
            num *= -1;
        }
        else
            // Unsupported base. Return NULL
            return NULL;
    }
 
    num_digits += (int)floor(log(num) / log(base)) + 1;
 
    // Go through the digits one by one
    // from left to right
    while (curr < num_digits) {
        // Get the base value. For example, 10^2 = 1000, for the third digit
        int base_val = (int) pow(base, num_digits-1-curr);
 
        // Get the numerical value
        int num_val = num / base_val;
 
        char value = num_val + '0';
        buffer[curr] = value;
 
        curr ++;
        num -= base_val * num_val;
    }
    buffer[curr] = '\0';
    return buffer;
}


std::string request::responce(){
	std::string ret;
	char validity_c[256];
	std::cout << "test" << std::endl;
	itoa(validity, validity_c, 10);
	std::cout << "test" << std::endl;
	std::string	validity_s = std::string(validity_c);
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
		std::cout << "request have not end" << std::endl;
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
		std::cout << "line a lire: " << line << "|" <<std::endl;
		if (start == 0){
			start++;
			if (get_first_line(&line) != 0)
				return 400;
		}
		else if (start == 1) {
			if (get_line(&line) != 0) 
				return 400;
		}
		str = str.substr(line_len + 2);
		if (line_len == end)
			break;
	}

	std::cout << "heh:" << str << "|" << std::endl;
	if (str != "\r\n") {
		std::cout << "il y a un body:" << str.substr(2) << "|"<<  std::endl;
		if (pars_body(str.substr(2)) != 0) {
			std::cout << "pars_body invalide" << std::endl;
			return 1;
		}
		
	}
	std::cout << "go to responce" << std::endl;
	std::cout << responce() << std::endl;
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
