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
						auto_response();
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
		}

}

void			Response::responseGet(std::vector<Server> lst_server) {
	// _request.methods == "GET";
	std::string path_for_access;
	std::string	extension;

	
	if (_body.size() != 0) {
		std::cout << "WTF here are my pb let's look inside" << std::endl;
		std::cout << _body << std::endl;
	}

	if (_request.cur_serv_index == 1) {
		std::cout << "bite" << std::endl;
		// Host header no valide
		// make a error page
	//	return ;
	}
	if (_request.validity != 200) {
		std::cout << "request non valide" << std::endl;
		return auto_response();
	}
	if (_request.path == "/") {
		std::cout << "ROOT " << std::endl;
	//	std::cout << "bite" << std::endl;
		if (_request.filename == "")
			path_for_access = lst_server[_request.cur_serv_index].infos.root + lst_server[_request.cur_serv_index].infos.index;
		else
			path_for_access = lst_server[_request.cur_serv_index].infos.root + _request.filename;
		size_t pos = path_for_access.find_last_of(".");
		if (pos == std::string::npos) {
			std::cout << "Extension not found in path" << std::endl;
			this->_header.setStatusCode(404);
			return ;

		}

		extension = path_for_access.substr(pos);
	

		std::cout << "extension" << extension << std::endl;
		std::cout << "acces::" <<  path_for_access << std::endl; 
		
		if (access(path_for_access.c_str(), F_OK) == 0) {
			char buff[25];
			itoa(_request.validity, buff, 10);
			std::string validity_c(buff);
			this->_header.setStatusCode(_request.validity);
			this->_header.setStatus(this->_request.get_http_version(), "OK");
			this->_body = AutoIndexGenerator(_request.location_path.root);
			// this->_body += readFromFile(path_for_access);
			this->_header.setDate();
			this->_header.setBodyLength(this->_body.length());
			this->_header.setContentLength();
			this->_header.setContentType(_request.map_file_type[extension]);
			this->_header.setServerName(lst_server[_request.cur_serv_index].infos.server_name);
		}
	}
	else {
		std::cout << "/location/" << std::endl;
		if (_request.filename == "") {
			std::cout << "no filename" << std::endl;
			path_for_access = _request.location_path.root + _request.location_path.index;
		}
		else {
			std::cout << "ya un filename" << std::endl;
			path_for_access = _request.location_path.root + _request.filename;
		}
		size_t pos = path_for_access.find_last_of(".");
		if (pos == std::string::npos) {
			std::cout << "Extension not found in path" << std::endl;
			this->_header.setStatusCode(404);
			return ;

		}
		extension = path_for_access.substr(pos);
	

		std::cout << "extension" << extension << std::endl;
		std::cout << "reel 2  access:" << path_for_access.c_str() << std::endl;
		
		if (access(path_for_access.c_str(), F_OK) == 0) {
			std::cout << _YEL << path_for_access << _NOR << std::endl;
			this->_header.setStatusCode(_request.validity);
			this->_header.setStatus(this->_request.get_http_version(), "OK");
			this->_body += readFromFile(path_for_access);
		this->_header.setDate();
		this->_header.setBodyLength(this->_body.length());
		this->_header.setContentLength();
		this->_header.setContentType(_request.map_file_type[extension]);
		this->_header.setServerName(lst_server[_request.cur_serv_index].infos.server_name);



		}
		
		
	//	_header.setStatusCode(atoi(_request.validity));

	}
//getter a coder	if (this->_header.getStatusCode() != 200)
//		return ;
	/* NECESSITE LE PQRSING DE VICTOR
	du coup en attendant */
	// bool location = 1;
	// bool autoindex = 1;

	// if (this->_request.get_path() == "/")
	// {
		// if (autoindex)
			// responseAutoIndex();
		//check if error page exists
		// this->_body = setErrorPage();
		//else
		//generate error page
	// }
	//fill the header
	/*
	else if (access("../www/error_page/custom/error_page_404.html", F_OK) == 0)
	{
		std::cout << "HERE" << std::endl;
		this->_header.setStatusCode(404);
		this->_header.setStatus(this->_request.get_http_version(), "OK");
		this->_body += readFromFile("../www/error_page/custom/error_page_404.html");
	//	this->_body += readFromFile("../www/error_page/custom/todd1.jpeg", 1);
		this->_header.setDate();
		this->_header.setBodyLength(this->_body.length());
		this->_header.setContentLength();
		this->_header.setContentType("text/html");
		this->_header.setServerName("jean");
	}
	*/

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