/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 18:56:51 by wluong            #+#    #+#             */
/*   Updated: 2022/06/25 18:50:31 by wluong           ###   ########.fr       */
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

void			Response::responseGet() {
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
	if (access("../www/index.html", F_OK) == 0)
	{
		this->_header.setStatusCode(200);
		this->_header.setStatus(this->_request.get_http_version(), "OK");
		this->_body += readFromFile("../www/error_page/custom/error_page_404.html");
		this->_header.setDate();
		this->_header.setBodyLength(this->_body.length());
		this->_header.setContentLength();
		this->_header.setContentType("text/html");
		this->_header.setServerName("localhost");
	}

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
