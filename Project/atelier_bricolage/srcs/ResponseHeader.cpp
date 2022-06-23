/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeader.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 04:32:41 by wluong            #+#    #+#             */
/*   Updated: 2022/06/23 04:45:19 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ResponseHeader.hpp"

ResponseHeader::ResponseHeader() : _status(), _date(), _serverName(), _lastModified(), _contentLength(), _contentType(), _header() {
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

std::string		ResponseHeader::getLastModified() const {
	return this->_lastModified;
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

void			ResponseHeader::setStatus( std::string const & statuscode, std::string const & status ) {
	this->_status = "HTTP/1.1 " + statuscode + status;
}

void			ResponseHeader::setDate( std::string const & str ) {
	this->_date = "Date: " + str;
}

void			ResponseHeader::setServerName( std::string const & str ) {
	this->_serverName = "Server: " + str;
}

void			ResponseHeader::setLastModified( std::string const & str ) {
	this->_lastModified = "Last-Modified: " + str;
}

void			ResponseHeader::setContentLength( std::string const & str ) {
	this->_contentLength = "Content-Length: " + str;
}

void			ResponseHeader::setContentType( std::string const & str ) {
	this->_contentType = "Content-Type: " + str;
}

void			ResponseHeader::generateHeader() {
	this->_header = this->_status + N_LINE;
	this->_header += this->_date + N_LINE;
	this->_header += this->_serverName + N_LINE;
	this->_header += this->_lastModified + N_LINE;
	this->_header += this->_contentLength + N_LINE;
	this->_header += this->_contentType + N_LINE;
	this->_header += N_LINE;
}
