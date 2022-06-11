/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 06:27:48 by wluong            #+#    #+#             */
/*   Updated: 2022/06/10 04:38:08 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Service.hpp"

/*
*	TO DO LIST :
*		Remplacer 512 par un define (nombre de clients max, soit 512 soit 1024)
*		Corriger le parsing de Victor pour pouvoir l'implémenter dansmes classes Server
*		Essayer des send et receive avec des un parsing existant
*		Clarifier le code au maximum (normalement deja bien calir)
*
*/

Service::Service() : _max_sd(0), _servers() {
	_timeout.tv_sec = 5;
	_timeout.tv_usec = 0;
	setup_cluster();
}

/*=======================================
=				TO		DO				=
=======================================*/

// Service::Service( Service const & src ) {}

Service::~Service() {}

// Service & Service::operator=( Service const& other ) {}

/*=======================================
=					END					=
=======================================*/

void	Service::setup_cluster() {
	for (int i(0); i < 512; i++)
	{
		_clients_sd[i] = 0;
	}
}

void	Service::check_opened_sd() {

	int		socket_d;

	_max_sd = 0;
	FD_ZERO(&_fdset);
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (it->_serv_sock.getSocket() > _max_sd)
			_max_sd = it->_serv_sock.getSocket();
		FD_SET(it->_serv_sock.getSocket(), &_fdset);
	}
	for (int i(0); i < 512; i++)
	{
		socket_d = _clients_sd[i];
		if (socket_d > 0)
			FD_SET(socket_d, &_fdset);
		if (socket_d > _max_sd)
			_max_sd = socket_d;
	}
}

void	Service::run_service() {
	while (TRUE)
	{
		this->check_opened_sd();
		if (!this->selecting())
			exit(EXIT_FAILURE);
		if (!this->accepting_connections())
			exit(EXIT_FAILURE);
		this->receive();
	}
}

bool	Service::selecting() {
	
	int		activity = 0;

	activity = select( _max_sd + 1, &_fdset, NULL, NULL, &_timeout);
	if (activity < 0)
	{
		std::cout << _BL_RED << "ERROR : " << _NOR << "SELECT ERROR" << std::endl;
		return false;
	}
	return true;

}

bool	Service::accepting_connections() {

	int		new_connection;

	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (FD_ISSET(it->_serv_sock.getSocket(), &_fdset))
		{
			new_connection = accept(it->_serv_sock.getSocket(), it->_serv_sock.castAddr(), it->_serv_sock.getAdLen());
			if (new_connection < 0)
			{
				std::cout << _BL_RED << "ERROR : " << _NOR << "ACCEPT ERROR" << std::endl;
				return false;
			}
			std::cout << _GRE << "NEW CONNECTION" << _NOR << ", SOCKET FD IS " << new_connection << ", PORT IS: " << ntohs(it->_serv_sock.getAddr().sin_port) << std::endl;
		}
		for (int i(0); i < 512; i++)
		{
			if (_clients_sd[i]== 0)
			{
				_clients_sd[i] = new_connection;
				break ;
			}
		}
	}
	return true;
}

void	Service::receive() {

	int		len_recv;

	for (int i(0); i < 512; i++)
	{
		if(FD_ISSET(_clients_sd[i], &_fdset))
		{
			len_recv = recv(_clients_sd[i], _buffer, 10024, 0);
			if (len_recv < 0)
			{
				std::cout << _BL_RED << "ERROR : " << _NOR << "RECV ERROR" << std::endl;
				continue;
			}
			if (len_recv == 0 || (len_recv == 1 && _buffer[0] == 4))
			{
				close(_clients_sd[i]);
				_clients_sd[i] = 0;
			}
			_buffer[len_recv] = 0;
			std::cout << _buffer << std::endl;
		}
		// parsing request sur _buffer 
		// sending doit recevoir la stc du parsing request
		sending(i);
	}
}

void	Service::sending(int i) {
	std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\n\n";

	header += "<!DOCTYPE html>\n";
	header += "<html>\n";
	header += "<style>\n";
	header += "body {\n";
	header += "background-image: url('./wallpaper.jpg');\n";
	header += "background-size: cover; \n";
	header += "background-attachment: fixed;\n";
	header += "background-position: center;\n";
	header += "}\n";
	header += "div {\n";
	header += "margin-top: 60%;\n";
	header += "text-align: center;\n";
	header += "position: center;\n";
	header += "}\n";
	header += "img {\n";
	header += "width:  25%;\n";
	header += "height: 25%;\n";
	header += "}\n";
	header += "</style>\n";
	header += "<body>\n";
	header += "<div>\n";
	header += "<img ";
	header += "src=\"./ErrorPage404.png\" ";
	header += "alt=\"error message 404\">\n";
	header += "</div>\n";
	header += "</body>\n";
	header += "</html>\n";

	send(_clients_sd[i], header.c_str(), header.length(), 0);
	close(_clients_sd[i]);
	_clients_sd[i] = 0;
}

// std::vector<Socket>		&Service::getServers() const {
// 	return _servers;
// }
