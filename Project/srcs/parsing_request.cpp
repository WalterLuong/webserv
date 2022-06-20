#include "parsing_request.hpp"

request::request() : methods(), path(), http_version(),  chunked(-1), validity(200) {
	init_default_error();
	init_file_type();
	init_instruction();

	std::string first_line = "GET / HTTP/1.0";
	std::string test = "Accept-Charsets: bite";

	for (std::map<std::string, std::string>::iterator ite(instruction.begin()) ; ite != instruction.end(); ite++) {
		std::cout << "instruction: " << ite->first << " , " << ite->second << std::cout;
		std::cout << std::endl;
	}

	int res;
	if ((res = pars_request(&test)) != 0) {
		std::cout << "bad request" << std::endl;
	}
	else
		std::cout << "first line good" << std::endl;
	for (std::map<std::string, std::string>::iterator ite(instruction.begin()) ; ite != instruction.end(); ite++) {
		std::cout << "instruction: " << ite->first << " , " ;
		if (ite->second  != "")
			std::cout << ite->second;
		std::cout << std::endl;
	}
	if (res == 0)	
		std::cout << "good request" << std::endl;
	else
		std::cout << "bad request" << std::endl;

}

request::request(request const & cpy) {
	methods = cpy.methods;
	path = cpy.path;
	http_version = cpy.http_version;
	chunked = cpy.chunked;
	validity = cpy.validity;
	map_error = cpy.map_error;
	map_file_type= cpy.map_file_type;
	instruction = cpy.instruction;
}

request &request::operator=(request const & cpy) {
	methods = cpy.methods;
	path = cpy.path;
	http_version = cpy.http_version;
	chunked = cpy.chunked;
	validity = cpy.validity;
	map_error = cpy.map_error;
	map_file_type= cpy.map_file_type;
	instruction = cpy.instruction;
	return (*this);
}

request::~request() {}

/*-------------Class Coplienne------------*/

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
	if (*line != "HTTP/1.0" && *line != "HTTP/1.0") {
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
	if (instruction["Content-length"] != "") {
		if (instruction["Content-length"].find_first_not_of("0123456789") != std::string::npos) {
			return 1;
		}
	}
	return 0;
}

int	request::pars_request(std::string *line) {
/*	if (get_first_line(line) != 0)
		return 1; */
	if (get_line(line) != 0)
		return 1;
	return 0;

	if (http_version == "HTTP/1.1") {

		if (instruction["Host"] == "") {
			return 400;
		}
	}
	if (check_connection() != 0) {
		return 1;
	}
	if (check_length() != 0) {
		return 1;
	}
}

void	request::init_instruction() {
	instruction["Accept-Charsets"] = ""; // list des caracteres 
	instruction["Auth-Scheme"] = "";
	instruction["Authorization"] = "";
	instruction["Content-Language"] = "";
	instruction["Content-Length"] = ""; // max body size // only num
	instruction["Content-Location"] = "";
	instruction["Content-Type"] = "";
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
