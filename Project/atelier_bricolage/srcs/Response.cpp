/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 18:56:51 by wluong            #+#    #+#             */
/*   Updated: 2022/06/23 04:43:01 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

char *itoa(int num, char* buffer, int base) {

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

std::string		int_to_str(size_t size){
	char *intStr = NULL;
	itoa(size, intStr, 10);
	std::string	ret(intStr);
	return ret;
}


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

void			Response::responseGet() {
	/* NECESSITE LE PQRSING DE VICTOR
	du coup en attendant */
	// bool location = 1;
	// bool autoindex = 1;

	// this->_request.get_path();
	// if (!location)
	// {
	// 	if (autoindex)
	// 		responseAutoIndex();
	// 	this->_body = setErrorPage();
	// }
	this->_header.setStatus("200 ", "OK");
	//fill the header
	this->_body = readFromFile("../../www/index.html");
	this->_header.setContentLength(int_to_str(_body.size()));
	this->_header.setServerName("webserv");

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
