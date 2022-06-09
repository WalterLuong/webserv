#include "../includes/parsing_request.hpp"


std::map<std::string, std::string> request::init_file_type() {

    std::map<std::string, std::string> ret;

	ret[".aac"]      = "audio/aac";
	ret[".abw"]      = "application/x-abiword";
	ret[".arc"]      = "application/octet-stream";
	ret[".avi"]      = "video/x-msvideo";
	ret[".azw"]      = "application/vnd.amazon.ebook";
	ret[".bin"]      = "application/octet-stream";
	ret[".bz"]       = "application/x-bzip";
	ret[".bz2"]      = "application/x-bzip2";
	ret[".csh"]      = "application/x-csh";
	ret[".css"]      = "text/css";
	ret[".csv"]      = "text/csv";
	ret[".doc"]      = "application/msword";
	ret[".epub"]     = "application/epub+zip";
	ret[".gif"]      = "image/gif";
	ret[".htm"]      = "text/html";
	ret[".html"]     = "text/html";
	ret[".ico"]      = "image/x-icon";
	ret[".ics"]      = "text/calendar";
	ret[".jar"]      = "Temporary Redirect";
	ret[".jpeg"]     = "image/jpeg";
	ret[".jpg"]      = "image/jpeg";
	ret[".js"]       = "application/js";
	ret[".json"]     = "application/json";
	ret[".mid"]      = "audio/midi";
	ret[".midi"]     = "audio/midi";
	ret[".mpeg"]     = "video/mpeg";
	ret[".mp4"]      = "video/mp4";
	ret[".mpkg"]     = "application/vnd.apple.installer+xml";
	ret[".odp"]      = "application/vnd.oasis.opendocument.presentation";
	ret[".ods"]      = "application/vnd.oasis.opendocument.spreadsheet";
	ret[".odt"]      = "application/vnd.oasis.opendocument.text";
	ret[".oga"]      = "audio/ogg";
	ret[".ogv"]      = "video/ogg";
	ret[".ogx"]      = "application/ogg";
	ret[".png"]      = "image/png";
	ret[".pdf"]      = "application/pdf";
	ret[".ppt"]      = "application/vnd.ms-powerpoint";
	ret[".rar"]      = "application/x-rar-compressed";
	ret[".rtf"]      = "application/rtf";
	ret[".sh"]       = "application/x-sh";
	ret[".svg"]      = "image/svg+xml";
	ret[".swf"]      = "application/x-shockwave-flash";
	ret[".tar"]      = "application/x-tar";
	ret[".tif"]      = "image/tiff";
	ret[".tiff"]     = "image/tiff";
	ret[".ttf"]      = "application/x-font-ttf";
	ret[".txt"]      = "text/plain";
	ret[".vsd"]      = "application/vnd.visio";
	ret[".wav"]      = "audio/x-wav";
	ret[".weba"]     = "audio/webm";
	ret[".webm"]     = "video/webm";
	ret[".webp"]     = "image/webp";
	ret[".woff"]     = "application/x-font-woff";
	ret[".xhtml"]    = "application/xhtml+xml";
	ret[".xls"]      = "application/vnd.ms-excel";
	ret[".xml"]      = "application/xml";
	ret[".xul"]      = "application/vnd.mozilla.xul+xml";
	ret[".zip"]      = "application/zip";
	ret[".3gp"]      = "video/3gpp audio/3gpp";
	ret[".3g2"]      = "video/3gpp2 audio/3gpp2";
	ret[".7z"]       = "application/x-7z-compressed";
	ret[".txt"]      = "text/plain";

	return ret;

}



std::map<std::string, std::string> request::init_default_error() {

    std::map<std::string, std::string> ret;

    ret[ "100" ] = "Continue";
    ret[ "101" ] = "Switching Protocols";
    ret[ "102" ] = "Processing";
    ret[ "103" ] = "Checkpoint";

    ret[ "200" ] = "OK";
    ret[ "201" ] = "Created";
    ret[ "202" ] = "Accepted";
    ret[ "203" ] = "Non-Authoritative Information";
    ret[ "204" ] = "No Content";
    ret[ "205" ] = "Reset Content";
    ret[ "206" ] = "Partial Content";
    ret[ "207" ] = "Multi-Status";
    ret[ "208" ] = "Already Reported";

    ret[ "300" ] = "Multiple Choices";
    ret[ "301" ] = "Moved Permanently";
    ret[ "302" ] = "Found";
    ret[ "303" ] = "See Other";
    ret[ "304" ] = "Not Modified";
    ret[ "305" ] = "Use Proxy";
    ret[ "306" ] = "Switch Proxy";
    ret[ "307" ] = "Temporary Redirect";
    ret[ "308" ] = "Permanent Redirect";

    ret[ "400" ] = "Bad Request";
    ret[ "401" ] = "Unauthorized";
    ret[ "402" ] = "Payment Required";
    ret[ "403" ] = "Forbidden";
    ret[ "404" ] = "Not Found";
    ret[ "405" ] = "Method Not Allowed";
    ret[ "406" ] = "Not Acceptable";
    ret[ "407" ] = "Proxy Authentication Required";
    ret[ "408" ] = "Request Timeout";
    ret[ "409" ] = "Conflict";
    ret[ "410" ] = "Gone";
    ret[ "411" ] = "Length Required";
    ret[ "412" ] = "Precondition Failed";
    ret[ "413" ] = "Payload Too Large";
    ret[ "414" ] = "URI Too Long";
    ret[ "415" ] = "Unsupported Media Type";
    ret[ "416" ] = "Requested Range Not Satisfiable";
    ret[ "417" ] = "Expectation Failed";
    ret[ "418" ] = "I'm a teapot";
    ret[ "421" ] = "Misdirected Request";
    ret[ "422" ] = "Unprocessable Entity";
    ret[ "423" ] = "Locked";
    ret[ "424" ] = "Failed Dependency";
    ret[ "426" ] = "Upgrade Required";
    ret[ "428" ] = "Precondition Required";
    ret[ "429" ] = "Too Many Request";
    ret[ "431" ] = "Request Header Fields Too Large";
    ret[ "451" ] = "Unavailable For Legal Reasons";

    ret[ "500" ] = "Internal Server Error";
    ret[ "501" ] = "Not Implemented";
    ret[ "502" ] = "Bad Gateway";
    ret[ "503" ] = "Service Unavailable";
    ret[ "504" ] = "Gateway Timeout";
    ret[ "505" ] = "HTTP Version Not Supported";
    ret[ "506" ] = "Variant Also Negotiates";
    ret[ "507" ] = "Insufficient Storage";
    ret[ "508" ] = "Loop Detected";
    ret[ "510" ] = "Not Extended";
    ret[ "511" ] = "Network Authentication Required";

    return ret;
}
