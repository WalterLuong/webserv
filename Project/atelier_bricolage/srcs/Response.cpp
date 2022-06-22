/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 18:56:51 by wluong            #+#    #+#             */
/*   Updated: 2022/06/22 03:12:24 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

Response::Response() : _header(), _body(), _request(), _resp() {

}

Response::~Response() {}

std::string		Response::getHeader() {
	return _header.getHeader();
}

std::string		Response::getBody() {
	return _body;
}

std::string		Response::getResponse() {
	return _resp;
}

void			Response::responseGet() {
	/* NECESSITE LE PQRSING DE VICTOR
	du coup en attendant */
	bool location = 1;
	bool autoindex = 1;

	if (!location)
	{
		if (autoindex)
			responseAutoIndex();
		this->_body = setErrorPage();
	}
	std::string responsehead = "200 OK";
	this->_header.setStatus(responsehead);
	//fill the header
	this->_body = readFromFile("location");

}

void			Response::responsePost() {

}

void			Response::responseDelete() {

}

void			Response::responseCGI() {

}

void			Response::responseAutoIndex() {

}

std::string		Response::readFromFile(std::string path) {
	std::ifstream		ifs(path);
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