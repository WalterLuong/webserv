/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 03:14:46 by wluong            #+#    #+#             */
/*   Updated: 2022/05/25 05:14:35 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int port) {
	this->_error = 0;
	fill_sockaddr(port);
	this->_addr_len = sizeof(this->_addr);
	create_socket();

}

Socket::Socket(Socket const & src) {
	*this = src;
}

Socket::~Socket() {}

// Socket& Socket::operator=(Socket const & other) {
// 	*this = other;
// 	return *this;
// }

void	Socket::fill_sockaddr(int port) {
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	std::memset(this->_addr.sin_zero, '\0', sizeof(this->_addr.sin_zero));
}

void		Socket::create_socket() {
		this->_server_fd = socket(this->_addr.sin_family, SOCK_STREAM, 0);
		if (this->_server_fd <= 0) {
			std::cout << "SOCKET CREATION FAILED" << std::endl;
			this->_error = SOCKET_ERROR;
		}
}

void	Socket::binding() {
	int		ret = 0;
	ret = bind(this->_server_fd, this->castAddr(), this->_addr_len);
	if (ret < 0) {
		std::cout << "BIND ERROR" << std::endl;		
		this->_error = BIND_ERROR;
	}
}

void		Socket::listening(int backlog) {
	if (listen(this->_server_fd, backlog) < 0) {
		std::cout << "LISTEN FAILED" << std::endl;
		this->_error = LISTEN_ERROR;
	}
}

sockaddr_in		Socket::getAddr() const {
	return this->_addr;
}

socklen_t		*Socket::getAdLen() {
	return &this->_addr_len;
}

int				Socket::getSocket() const {
	return this->_server_fd;
}

int				Socket::getError() const {
	return this->_error;
}

sockaddr		*Socket::castAddr() const {
	return (struct sockaddr *)&this->_addr;
}