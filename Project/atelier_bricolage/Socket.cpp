/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 03:14:46 by wluong            #+#    #+#             */
/*   Updated: 2022/06/03 05:55:47 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~					CONSTRUCTORS					~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Socket::Socket() : _max_clients(30)
{
	this->_error = 0;
	fill_sockaddr(8080);
	this->_addr_len = sizeof(this->_addr);
	this->setup(1000);
}

Socket::Socket(int port) : _max_clients(30)
{
	this->_error = 0;
	fill_sockaddr(port);
	this->_addr_len = sizeof(this->_addr);
	this->setup(1000);

}

Socket::Socket(Socket const & src)
{
	this->_addr = src._addr;
	this->_addr_len = src._addr_len;
	this->_error = src._error;
	this->_server_fd = src._server_fd;
	this->_max_clients = src._max_clients;
}

Socket::~Socket() 
{
	for (int i(0); i < this->_max_clients; i++)
	{
		this->disconnect(&this->_clients_fd[i]);
	}
	close(this->_server_fd);
}

Socket& Socket::operator=(Socket const & other) 
{
	this->_addr = other._addr;
	this->_addr_len = other._addr_len;
	this->_error = other._error;
	this->_server_fd = other._server_fd;
	return *this;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~						SET UP						~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void	Socket::fill_sockaddr(int port) 
{
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port);
	std::memset(this->_addr.sin_zero, '\0', sizeof(this->_addr.sin_zero));
}

void		Socket::create_socket() 
{
		this->_server_fd = socket(this->_addr.sin_family, SOCK_STREAM, 0);
		if (this->_server_fd <= 0)
		{
			std::cout << _RED << "ERROR : " << _NOR << "SOCKET CREATION FAILED" << std::endl;
			this->_error = SOCKET_ERROR;
		}
}

void	Socket::binding() {
	int		ret = 0;
	ret = bind(this->_server_fd, this->castAddr(), this->_addr_len);
	if (ret < 0)
	{
		std::cout << _RED << "ERROR : " << _NOR << "BIND ERROR" << std::endl;
		this->_error = BIND_ERROR;
	}
}

void		Socket::listening(int backlog)
{
	if (listen(this->_server_fd, backlog) < 0)
	{
		std::cout << _RED << "ERROR : " << _NOR << "LISTEN FAILED" << std::endl;
		this->_error = LISTEN_ERROR;
	}
}

int			Socket::setup(int backlog)
{
	int		opt = 1;
	this->create_socket();
	if (this->getError() == SOCKET_ERROR)
		return -1;
	if ( setsockopt(this->getSocket(), SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cout << _RED << "ERROR : " << _NOR << "SETSOCKOPT ERROR" << std::endl;
		return -1;
	}
	this->binding();
	if (this->getError() == BIND_ERROR)
		return -1;
	this->listening(backlog);
	if (this->getError() == LISTEN_ERROR)
		return -1;
	if (fcntl(this->_server_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << _RED << "ERROR : " << _NOR << "FCNTL ERROR" << std::endl;
		return -1;
	}
	return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~					COMMUNICATION					~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool		accept_client(Client *current)
{
	current->client_addr_len = sizeof(sockaddr);
	current->client_fd = ::accept(this->_server_fd, (struct sockaddr *)&current->address, &current->client_addr_len);
	if (current->client_fd <= 0)
		return (false);
	else
	{
		current->connected = true;
		FD_ZERO( &current->client_data );
		FD_SET( current->client_fd, &current->client_data );
		return (true);
	}
	return (false);
}

int			accept_connections()
{
	if (this->_clients_fd[this->_max_clients] == 0) 
	{
		for ( int i = 0; i < this->_max_clients; i++)
		{
			if (!this->_clients_fd[i].connected)
			{
				if (accept_client( &client[i] ))
				{
					char *client_ip_address = inet_ntoa( this->_clients_fd[i].address.sin_addr );
					std::cout << "ACCEPTING CLIENT WITH ADRESS IP : " << client_ip_address << std::endl;
				}
			}
		}
	}
}

void		disconnect(Client *current)
{
	if (current->connected = true)
		close(current->client_fd);
	current->connected = false;
	std::cout << "DISCONNECT CLIENT" << std::endl;
}

bool		client_receive(Client *current)
{

}

void		receive_data()
{

}

int			send_data(Client *current, char *buff, in len)
{

}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~						GETTERS						~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

sockaddr_in		Socket::getAddr() const 
{
	return this->_addr;
}

socklen_t		*Socket::getAdLen() 
{
	return &this->_addr_len;
}

int				Socket::getSocket() const 
{
	return this->_server_fd;
}

int				Socket::getError() const 
{
	return this->_error;
}

sockaddr		*Socket::castAddr() const 
{
	return (struct sockaddr *)&this->_addr;
}