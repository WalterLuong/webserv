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

// bool			Response::AllowedMethod() {
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
//			std::cout << "tientientein body :" << body << std::endl;
			break ;
		}
		if (pos == std::string::npos || pos == 0) {
//			std::cout << "plus de newline found" << std::endl;
			break;
		}
		str = body.substr(0, pos-1);
		body = body.substr(pos + 1); 
		size_t res;
		if ((res = str.find("Content-type: ")) == 0) {
//			std::cout << "i got the content type" << std::endl;
			res = str.find(":");
		//	std::string bite = "text/html";
			std::string bite = str.substr(res + 2);
	//		std::cout << "ce que je met dans le header:" << str.substr(res+2) << std::endl;
			_header.setContentType(bite);
		}
//		std::cout << " str: " << str << "|" << std::endl;
	//	std::cout << " body: " << body[0]<< "|" << std::endl;

	}
	_header.setBodyLength(_body.length());
	_header.setContentLength();
	
//	_body = _body+"\r\n";
}


void			Response::responseGet(std::vector<Server> lst_server) {
	std::string path_for_access;
	std::string	extension = "html";

		std::cerr << _YEL <<this->_request.location_path.uri << _NOR << std::endl;
		std::cerr << _YEL <<this->_request.path << _NOR << std::endl;
		std::cout << this->_request.get_path() << std::endl;
		std::cout << this->_request.filename << std::endl;
	
	path_for_access = _request.location_path.root;
	if (_request.path[_request.path.length() - 1] == '/'/*_request.location_path.autoindex == "on"*/)
	{
		std::cerr << _YEL << "HERE" << _NOR << std::endl;

		if (_request.path.length() >= _request.location_path.uri.length() + 2)
		{
			std::string	directory;
			std::cout << _RED << "JE SUIS LA ABDOUL"<< _NOR << std::endl;
			directory = path_for_access +_request.path.substr(this->_request.location_path.uri.length() + 1, this->_request.path.length());
			directory[directory.length() - 1] = 0;
			std::cout <<_RED << directory << _NOR << std::endl;
			if (isDirectory(directory))
			{
				this->_body = AutoIndexGenerator(directory);
				this->_header.setContentType("text/html");
				createHeader(extension, lst_server);
				return ;
			}
		}
		else if (this->_request.autoindex_on || (lst_server[_request.cur_serv_index].infos.autoindex == "on" && _request.path == "/" && this->_request.filename == ""))
		{
			std::cout << _RED << "JE SUIS LA ABDOUL 2"<< _NOR << std::endl;
			this->_body = AutoIndexGenerator(path_for_access);
			this->_header.setContentType("text/html");
			createHeader(extension, lst_server);
			return ;
		}
	}
	std::string		autoindexfile;
	autoindexfile = path_for_access;
	if (_request.path.length() >= _request.location_path.uri.length() + 2)
	{
		autoindexfile += _request.path.substr(this->_request.location_path.uri.length() + 2, this->_request.path.length());
		if (access(autoindexfile.c_str(), F_OK) == 0)
		{
			size_t pos = path_for_access.find_last_of(".");
			if (pos == std::string::npos) {
				this->_header.setStatusCode(404);
				return ;
			}
			extension = path_for_access.substr(pos);
			this->_body += readFromFile(autoindexfile);
			createHeader(extension, lst_server);
			return ;
		}
	}
	std::cout << _RED << "JE SUIS LA WALTY"<< _NOR << std::endl;
	std::cout << _RED << _request.filename << _NOR << std::endl;
	if (_request.filename == "")
		path_for_access += _request.location_path.index;
	else
		path_for_access += _request.filename;
	std::cout << _RED << path_for_access << _NOR << std::endl;
	size_t pos = path_for_access.find_last_of(".");
	if (pos == std::string::npos) {
		this->_header.setStatusCode(404);
		return ;
	}
	extension = path_for_access.substr(pos);
	/*
		cgi 

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
	if (access(path_for_access.c_str(), F_OK) == 0 && _request.validity == 200)
	{
		this->_body += readFromFile(path_for_access);
		createHeader(extension, lst_server);
		return ;
	}
	return auto_response();
}
	
// 	std::string ath_for_access;
// 	std::string	extension = "html";

// 	static int in_autoindex;

// 	// if (_request.validity != 200)
// 	// {
// 	// 	std::cout << "request non valide" << std::endl;
// 	// 	return auto_response();
// 	// }

// //first line inutlie c'est la meme chose
// 	if (_request.path == "/")
// 		path_for_access = lst_server[_request.cur_serv_index].infos.root;
// 	else
// 		path_for_access = _request.location_path.root;

// 	std::cout << _YEL << "path:" << _request.path << _NOR << std::endl;
// 	std::cout << "path for access:" << path_for_access << "|" << std::endl;
// 	std::cout << "_loc.root:" << _request.location_path.root << "|" << std::endl;
// 	// std::cout << this->_request.location_path.uri << std::endl;
// 	// std::cout << this->_request.path << std::endl;
// 	// std::cout <<this->_request.path.substr(this->_request.location_path.uri.length() + 2, this->_request.path.length()) << _NOR << std::endl;

// 	if (_request.path == "/")
// 	{
// 		if (this->_request.location_path.autoindex == "on"  || in_autoindex) 
// 		{
// 			in_autoindex = 1;
// 			this->_body = AutoIndexGenerator(lst_server[_request.cur_serv_index].infos.root);
// 			this->_header.setContentType("text/html");
// 			createHeader(extension , lst_server);
// 			return ;
// 		}
// 		else
// 		{
// 			if (_request.filename == "")
// 				path_for_access += lst_server[_request.cur_serv_index].infos.index;
// 			else
// 				path_for_access += _request.filename;
// 			size_t pos = path_for_access.find_last_of(".");
// 			{
// 				std::cout << "Extension not found in path" << std::endl;
// 			extension = path_for_access.substr(pos);
// 			if (access(path_for_access.c_str(), FOK) == 0)
// 			{
// 				char buff[25];
// 				itoa(_request.validity, buff, 10);
// 				std::string validity_c(buff);
// 				this->_body += readFromFile(path_for_access);
// 				createHeader(extension, lst_server);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (_request.path[_request.path.length() - 1] == '/' && (in_autoindex == 1 || _request.location_path.autoindex == "on"))
// 		{
// 			std::cout << "step one" << std::endl;
// 			in_autoindex = 1;
// 			std::string	directory;
// 			if (_request.path.length() >= _request.location_path.uri.length() + 2)
// 					directory = path_for_access +_request.path.substr(this->_request.location_path.uri.length() + 1, this->_request.path.length());
// 			directory[directory.length() - 1] = 0;
// 			if (isDirectory(directory))
// 			{
// 				this->_body = AutoIndexGenerator(directory);
// 				this->_header.setContentType("text/html");
// 				createHeader(extension, lst_server);
// 				return ;
// 			}
// 			else //if (this->_request.autoindex_on)
// 			{
// 				this->_body = AutoIndexGenerator(path_for_access);
// 				this->_header.setContentType("text/html");
// 				createHeader(extension, lst_server);
// 				return ;
// 			}
// 		}
// 		else
// 		{
// 			std::cout << "ICI1???? " << std::endl;
// 			std::string		autoindexfile;
// 			autoindexfile = path_for_access;
// 			std::cout << "test sur le premier if" << std::endl;
// 			std::cout << "path: " << _request.path << "|" << std::endl;

// 			/* la le "/cat/bob_le_bricoleur" ne marchait plus
// 				if (access(autoindexfile.c_str(), F_OK == 0)
// 				{
// 					size_t pos = path_for_access.find_last_of(".");
// 					if (pos == std::string::npos) {
// 						this->_header.setStatusCode(404);
// 						return ;
// 					}
// 					extension = path_for_access.substr(pos);
// 					this->_body += readFromFile(autoindexfile);
// 					createHeader(extension, lst_server);
// 				}
// 			}
// 			*/
// 		//	else
// 		//	{
// 				in_autoindex = 0;
// 				std::cout << "ICI ????" << std::endl;
// 				if (_request.filename == "")
// 					path_for_access += _request.location_path.index;
// 				else
// 					path_for_access += _request.filename;
// 				size_t pos = path_for_access.find_last_of(".");
// 				if (pos == std::string::npos) {
// 					this->_header.setStatusCode(404);
// 					return ;
// 				}
// 				extension = path_for_access.substr(pos);
// 				if (access(path_for_access.c_str(), F_OK) == 0)
// 				{
// 					this->_body += readFromFile(path_for_access);
// 					createHeader(extension, lst_server);
// 				}
// 		//	}
// 		}
// 	}
// }

// void			Response::responseGet(std::vector<Server> lst_server) {
	
// 	std::string path_for_access;
// 	std::string	extension;

	
// 	if (_body.size() != 0) {
// //		std::cout << "WTF here are my pb let's look inside" << std::endl;
// //		std::cout << _body << std::endl;
// 	}

// 	if (_request.cur_serv_index == 1) {
// //		std::cout << "bite" << std::endl;
// 		// Host header no valide
// 		// make a error page
// 	//	return ;
// 	}
// 	if (_request.validity != 200) {
// 		std::cout << "request non valide" << std::endl;
// 		return auto_response();
// 	}
// 	if (_request.path == "/") {
// //			std::cout << "ROOT " << std::endl;
// 		//	std::cout << "bite" << std::endl;
// 			if (_request.filename == "")
// 				path_for_access = lst_server[_request.cur_serv_index].infos.root + lst_server[_request.cur_serv_index].infos.index;
// 			else
// 				path_for_access = lst_server[_request.cur_serv_index].infos.root + _request.filename;
// 			size_t pos = path_for_access.find_last_of(".");
// 			if (pos == std::string::npos) {
// 				std::cout << "Extension not found in path" << std::endl;
// 				this->_header.setStatusCode(404);
// 				return ;
// 			}
// 			extension = path_for_access.substr(pos);
// 			/*
// 				go pour les cgi 
// 			*/
// 			if (_request.location_path.cgi_path.size() != 0) {
// 				if (extension == _request.location_path.cgi_path[0].first) {
// 					std::cout <<  "RAYYYYYYAN" << std::endl;
// 				}
// 			}	
// //			std::cout << "extension" << extension << std::endl;
// //			std::cout << "acces::" <<  path_for_access << std::endl; 
// 			if (access(path_for_access.c_str(), F_OK) == 0) {
// 				char buff[25];
// 				itoa(_request.validity, buff, 10);
// 				std::string validity_c(buff);
// 				this->_body += readFromFile(path_for_access);
// 				createHeader(extension, lst_server);
// 			}
		
// 	}
// 	else {
// //			std::cout << "/location/" << std::endl;
// 			if (_request.filename == "") {
// //				std::cout << "no filename" << std::endl;
// 				path_for_access = _request.location_path.root + _request.location_path.index;
// 			}
// 			else {
// //				std::cout << "ya un filename" << std::endl;
// 				path_for_access = _request.location_path.root + _request.filename;
// 			}
// 			size_t pos = path_for_access.find_last_of(".");
// 			if (pos == std::string::npos) {
// //				std::cout << "Extension not found in path" << std::endl;
// 				this->_header.setStatusCode(404);
// 				return ;

// 			}
// 			extension = path_for_access.substr(pos);
// 			/*
// 				go pour les cgi 
// 			*/
// 			if (_request.location_path.cgi_path.size() != 0) {
// 				if (extension == _request.location_path.cgi_path[0].first) {
// 					std::cout <<  "RAYYYYYYAN" << std::endl;
// 				}
// 			}	

// //			std::cout << "extension" << extension << std::endl;
// //			std::cout << "reel 2  access:" << path_for_access.c_str() << std::endl;

// 			if (access(path_for_access.c_str(), F_OK) == 0) {
// 				std::cout << _YEL << path_for_access << _NOR << std::endl;
// 				this->_body += readFromFile(path_for_access);
// 				createHeader(extension, lst_server);
// 			}
// 	}
// }

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
