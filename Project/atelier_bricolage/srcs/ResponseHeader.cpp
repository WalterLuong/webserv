/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeader.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 04:32:41 by wluong            #+#    #+#             */
/*   Updated: 2022/06/30 06:26:53 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ResponseHeader.hpp"
#include "../includes/utils.hpp"

ResponseHeader::ResponseHeader() : _status(), _date(), _serverName(), _contentLength(), _contentType(), _header(), _statuscode(200), _bodyLength(0) {
}

ResponseHeader::ResponseHeader( ResponseHeader const & src ) {
	*this = src;
}

ResponseHeader::~ResponseHeader() {}

// ResponseHeader & ResponseHeader::operator=( ResponseHeader const & other ) {
// 	*this = other;
// 	return *this;
// }

std::string		ResponseHeader::getStatus() const {
	return this->_status;
}

std::string		ResponseHeader::getDate() const {
	return this->_date;
}

std::string		ResponseHeader::getServerName() const {
	return this->_serverName;
}

std::string		ResponseHeader::getContentLength() const {
	return this->_contentLength;
}

std::string		ResponseHeader::getContentType() const {
	return this->_contentType;
}

std::string		ResponseHeader::getHeader() const {
	return this->_header;
}

void			ResponseHeader::setStatus( std::string const & protocol, std::string const & status ) {
	this->_status = protocol + " " + int_to_str(this->_statuscode) + " " + status;
}

void			ResponseHeader::setDate() {
	this->_date = "Date: " + get_date();
}

void			ResponseHeader::setServerName( std::string const & str ) {
	this->_serverName = "Server: " + str;
}

void			ResponseHeader::setContentLength() {
	this->_contentLength = "Content-Length: " + int_to_str(this->_bodyLength);
}

void			ResponseHeader::setContentType( std::string const & str ) {
	this->_contentType = "Content-Type: " + str;
}

void			ResponseHeader::generateHeader() {
	if (_status != "")
		this->_header = this->_status + N_LINE;
	if (_date != "")
	this->_header += this->_date + N_LINE;
	if (_serverName != "")
	this->_header += this->_serverName + N_LINE;
	if (_contentLength != "")
	this->_header += this->_contentLength + N_LINE;
	if (_contentType != "")
	this->_header += this->_contentType + N_LINE;
	if (_header != "")
	this->_header += N_LINE;
}

void			ResponseHeader::setStatusCode( int code ) {
	this->_statuscode = code;
}

void			ResponseHeader::setBodyLength( int length ) {
	this->_bodyLength = length;
}
