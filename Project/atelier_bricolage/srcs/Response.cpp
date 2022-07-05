/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlormois <mlormois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 18:56:51 by wluong            #+#    #+#             */
/*   Updated: 2022/06/28 21:13:18by mlormois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"
#include "../includes/utils.hpp"


std::string randomdigits(int nb);


/**
 * It creates a temporary file, writes nothing to it, and returns the name of the file
 *
 * @return A string containing the name of a temporary file.
 */
std::string create_tmpfile(void);


/**
 * It reads the contents of a file into a string
 *
 * @param filename The name of the file to read.
 *
 * @return A string containing the contents of the file.
 */
std::string get_file_content(std::string filename);

/**
 * It takes an array of strings and the number of strings in the array, and returns a single string
 * that is the concatenation of all the strings in the array
 *
 * @param strs an array of strings
 * @param n the number of strings to join
 *
 * @return A string
 */
std::string joinstr(std::string *strs, int n);

int run_bin(std::string bin, std::string infilename, std::string outfilename);
/**
 * It writes a string to a file
 *
 * @param infile The file to write to.
 * @param body The body of the file to be written.
 *
 * @return The return value of the system call.
 */
int write_infile(std::string infile, std::string body);

/**
 * It takes a binary and a string, writes the string to a temporary file, runs the binary with the
 * temporary file as input, and returns the output of the binary as a string
 *
 * @param bin the path to the binary to run
 * @param body The body of the request.
 *
 * @return The output of the cgi script.
 */

std::string cgi_execution(std::string bin, std::string body);

/**
 * It converts an integer to a string
 *
 * @param k the number to convert to a string
 *
 * @return A string
 */
std::string tostr(int k);

/**
 * If the position is not equal to the npos constant, then return the substring starting at that
 * position, otherwise return the substring starting at the beginning of the string.
 *
 * @param str The string to search in.
 * @param pos The position of the first character of the substring.
 *
 * @return The substring of str starting at pos.
 */
std::string get_sub_str(std::string str, size_t pos);

/**
 * It sets the environment variables for the CGI script
 *
 * @param req The request object
 * @param absolutepath The absolute path of the cgi script.
 */

void set_environement(request &req, std::string &absolutepath);


std::string g_extension = "";

/**
 * It returns true if the first element of the pair is equal to the global variable g_extension.
 *
 * @param to_test This is the pair that we're testing.
 *
 * @return A boolean value.
 */
bool is_equal_to_extension(std::pair<std::string, std::string> to_test)
{
	return to_test.first == g_extension;
}

/**
 * It returns the extension of a file
 *
 * @param filename The name of the file to get the extension from.
 *
 * @return The extension of the file.
 */
std::string get_extension(std::string filename)
{
	std::string ext = "";
	std::string::size_type pos = filename.find_last_of(".");
	if (pos != std::string::npos)
	{
		ext = filename.substr(pos + 1);
	}
	return ext;
}

/**
 * It returns the path to the CGI executable for the file extension of the requested file
 *
 * @param cgi_path A vector of pairs of strings. The first string is the extension, the second is the
 * path to the CGI executable.
 *
 * @return The path to the cgi-bin directory.
 */
std::string get_cgipath(std::vector<std::pair<std::string, std::string> > cgi_path);


/**
 * It sets the environment variables, then executes the CGI script and returns the output
 *
 * @param req the request object
 * @param absolutepath the absolute path of the file to be executed
 *
 * @return The output of the cgi script
 */
std::string cgi_handler(request &req, std::string &absolutepath);






Response::Response(request req) : _header(), _body(), _request(req), _resp() {}

Response::~Response() {}


int		Response::is_request_valid() {

	return _request.validity;
}

void	Response::set_validity(int value) {
	_request.validity = value;
}


std::string		Response::getHeader() {
	return _header.getHeader();
}

std::string		Response::getBody() {
	return _body;
}

std::string		Response::getResponse() {
	_header.generateHeader();
	_resp = _header.getHeader();
	_resp += _body;
	return _resp;
}

// bool			Response::AllowedMethod() {<
// 	// if (this->_request.getMethod() == "GET")
// 	// 	return true;
//	chercher dans le path si la methode est allowed sinon
//	si oui, return true
//	si non error 405 et return false
// }

void		Response::auto_response() {
		char buff[250];

		itoa(_request.validity, buff, 10);
		std::string validity_c(buff);

		/*		test cgi == json
			_header.setStatusCode(_request.validity);
			_body += "{\"response\":\"success\", \"body\":{ \"status\": \"right\"}}\n\r\n";
			_header.setBodyLength(_body.length());
			_header.setContentType("application/json");
			this->_header.setStatus("HTTP/1.1", "OK");
return ;
*/

		if (_request.cur_serv_index == -1) {
			_header.setStatusCode(_request.validity);
			_body += generator_error_file(validity_c);
			_header.setBodyLength(_body.length());
			_header.setContentType("text/html");
			this->_header.setStatus("HTTP/1.1", "OK");
		}

		else {
			std::vector<std::pair<std::string, std::string> >::iterator ite = _request.location_path.error_page.begin();
			while (ite != _request.location_path.error_page.end()) {
				if (validity_c == ite->first) {
					size_t pos;
					if ((pos = ite->second.find_last_of(".")) == std::string::npos) {
						_request.cur_serv_index = -1;
						return auto_response();
					}
					std::string extension(ite->second.substr(pos));

					_header.setStatusCode(_request.validity);
					_body += readFromFile(ite->second);
					_header.setBodyLength(_body.length());
					_header.setContentType(_request.map_file_type[extension]);
					this->_header.setStatus("HTTP/1.1", "OK");

					return ;
				}
				// je recup ite->first (string), je compare ca a _request.validity et si == _body =RFF(ite->second)

				ite++;
			}

			ite = _request.location_path.redirection.begin();
			while (ite != _request.location_path.redirection.end()) {
				if (validity_c == ite->first) {
					size_t pos;
					if ((pos = ite->second.find_last_of(".")) == std::string::npos) {
						_request.cur_serv_index = -1;
						return auto_response();
					}
					std::string extension(ite->second.substr(pos));

					_header.setStatusCode(_request.validity);
					_body += readFromFile(ite->second);
					_header.setBodyLength(_body.length());
					_header.setContentType(_request.map_file_type[extension]);
					this->_header.setStatus("HTTP/1.1", "OK");

					return ;

				}
				ite++;
			}
			_request.cur_serv_index = -1;
			return auto_response();

		}

}

void	Response::cgi_header(std::string body) {
	std::string str;
	size_t pos;

	while (body != "") {
		pos = body.find("\n");
		if (body.find("\r\n") == 0) {
			body = body.substr(2);
			_body = body;
			std::cout << "tientientein" << std::endl;
			break ;
		}
		if (pos == std::string::npos || pos == 0) {
			std::cout << "plus de newline found" << std::endl;
			break;
		}
		str = body.substr(0, pos-1);
		body = body.substr(pos + 1); 
		size_t res;
		if ((res = str.find("Content-type: ")) == 0) {
			std::cout << "i got the content type" << std::endl;
			res = str.find(":");
		//	std::string bite = "text/html";
			std::string bite = str.substr(res + 2);
			std::cout << "ce que je met dans le header:" << str.substr(res+2) << std::endl;
			_header.setContentType(bite);
		}
		std::cout << " str: " << str << "|" << std::endl;
		std::cout << " body: " << body[0]<< "|" << std::endl;

	}
	_header.setBodyLength(_body.length());
	_header.setContentLength();
	std::cout << "PUTE WALTER PUTE:" << _header.getContentLength() << std::endl;
	std::cout << "Str length :" << _body.length() << std::endl;
//	_body = _body+"\r\n";
}

void			Response::responseGet(std::vector<Server> lst_server) {

	std::string path_for_access;
	std::string extension;

	if (_body.size() != 0) {
//		std::cout << "WTF here are my pb let's look inside" << std::endl;
//		std::cout << _body << std::endl;
	}

	if (_request.cur_serv_index == 1) {
//		std::cout << "bite" << std::endl;
		// Host header no valide
		// make a error page
	//	return ;
	}

	if (_request.validity != 200) {
		std::cout << "request non valide" << std::endl;
		return auto_response();
	}

	if (_request.path == "/") {
//			std::cout << "ROOT " << std::endl;
		//	std::cout << "bite" << std::endl;
			if (_request.filename == "")
				path_for_access = lst_server[_request.cur_serv_index].infos.root + lst_server[_request.cur_serv_index].infos.index;
			else
				path_for_access = lst_server[_request.cur_serv_index].infos.root + _request.filename;
			size_t pos = path_for_access.find_last_of(".");
			if (pos == std::string::npos) {
				std::cout << "g_Extension not found in path" << std::endl;
				this->_header.setStatusCode(404);
				return ;
			}
			extension = path_for_access.substr(pos);
			/*
				go pour les cgi
			*/
			if (_request.location_path.cgi_path.size() != 0) {
				int cgi_pos;
				if ((cgi_pos = get_cgi_path_pos(extension, _request.location_path.cgi_path)) != -1) {
					createHeader(extension, lst_server);
					_body = cgi_handler(_request, path_for_access, cgi_pos);
					std::cout << _body << std::endl;
					return;
				}
			}
//			std::cout << "g_extension" << g_extension << std::endl;
//			std::cout << "acces::" <<  path_for_access << std::endl;
			else if (access(path_for_access.c_str(), F_OK) == 0) {
				char buff[25];
				itoa(_request.validity, buff, 10);
				std::string validity_c(buff);
				this->_body += readFromFile(path_for_access);
				createHeader(extension, lst_server);
			}

	}
	else {
			std::cout << "/location/" << std::endl;
			if (_request.filename == "") {
//				std::cout << "no filename" << std::endl;
				path_for_access = _request.location_path.root + _request.location_path.index;
			}
			else {
//				std::cout << "ya un filename" << std::endl;
				path_for_access = _request.location_path.root + _request.filename;
			}
			size_t pos = path_for_access.find_last_of(".");
			if (pos == std::string::npos) {
//				std::cout << "g_Extension not found in path" << std::endl;
				this->_header.setStatusCode(404);
				return ;

			}
			extension = path_for_access.substr(pos);
			/*
				go pour les cgi
			*/
			if (_request.location_path.cgi_path.size() != 0) {
				int pos_cgi;
				if ((pos_cgi = get_cgi_path_pos(extension, _request.location_path.cgi_path)) != -1) {
					createHeader(extension, lst_server);
					std::cout << "go to exec cgi" << std::endl;
					_body = cgi_handler(_request, path_for_access, pos_cgi);
					std::cout << "ret of cgi hgandler:" << _body << "|" << std::endl;
					std::cout << "go treat the body" << std::endl;
					cgi_header(_body);
					return ;
				}
			}
			else if (access(path_for_access.c_str(), F_OK) == 0) {
				std::cout << _YEL << path_for_access << _NOR << std::endl;
				this->_body += readFromFile(path_for_access);
				createHeader(extension, lst_server);
			}
	}
}

void			Response::createHeader( std::string & extension, std::vector<Server> & lst_server ) {
				this->_header.setStatusCode(_request.validity);
				this->_header.setStatus(this->_request.get_http_version(), "OK");
				this->_header.setDate();
				this->_header.setBodyLength(this->_body.length());
				this->_header.setContentLength();
				this->_header.setContentType(_request.map_file_type[extension]);
				this->_header.setServerName(lst_server[_request.cur_serv_index].infos.server_name);
}

void			Response::responsePost() {

}

void			Response::responseDelete() {

	//is method allowed

	if (std::remove(this->_request.get_path().c_str()))
		this->_header.setStatus("403 ", "Forbidden");
	//creer un body pour dire que c delete ?
}

void			Response::responseCGI() {
	//jsp quoi faire ici
}

void			Response::responseAutoIndex() {
	// j'attends victor
	//
}

std::string		Response::readFromFile(std::string path, int i) {
	std::ifstream		ifs(path.c_str(), std::ifstream::binary);
	std::string			ret_buffer;
	std::string			tmp;

	if (i != 1) {
		return tmp;
	}
	if (!ifs)
	{
		std::cout << "This file doesn't exist." << std::endl;
		return ("");
	}
	while (!ifs.eof())
	{
		std::getline(ifs, tmp);
		ret_buffer += tmp;
		ret_buffer += "\n";
	}
	ifs.close();
	return ret_buffer;
}

std::string		Response::readFromFile(std::string path) {
	std::ifstream		ifs(path.c_str());
	std::string			ret_buffer;
	std::string			tmp;

	if (!ifs)
	{
		std::cout << "This file doesn't exist." << std::endl;
		return ("");
	}
	while (!ifs.eof())
	{
		std::getline(ifs, tmp);
		ret_buffer += tmp;
		ret_buffer += "\n";
	}
	ifs.close();
	return ret_buffer;
}

void			Response::setBody( std::string body ) {
	this->_body = body;
}