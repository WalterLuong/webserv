/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 06:27:48 by wluong            #+#    #+#             */
/* ************************************************************************** */

#include "../includes/Service.hpp"

Service::Service() : _max_sd(0), _servers() {
	_timeout.tv_sec = 5;
	_timeout.tv_usec = 0;
	setup_cluster();
}


Service::~Service() {}


void	Service::setup_cluster() {
	for (int i(0); i < MAX_CLIENTS; i++)
		_clients_sd[i] = 0;
}

void	Service::check_opened_sd() {
	_max_sd = 0;
	FD_ZERO(&_fdset);
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (it->_serv_sock.getSocket() > _max_sd)
			_max_sd = it->_serv_sock.getSocket();
		FD_SET(it->_serv_sock.getSocket(), &_fdset);
		for (int i(0); i < MAX_CLIENTS; i++)
		{
			if (_clients_sd[i] > 0)
				FD_SET(_clients_sd[i], &_fdset);
			if (_clients_sd[i] > _max_sd)
				_max_sd = _clients_sd[i];
		}
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

	activity = select( _max_sd + 1, &_fdset, NULL, NULL, NULL);
	std::cout << _RED << activity << std::endl;
	if (activity < 0)
	{
	//	std::cout << _BL_RED << "ERROR : " << _NOR << "SELECT ERROR" << std::endl;
		return false;
	}
	return true;

}

bool	Service::accepting_connections() {

	int		new_connection = 0;

	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (FD_ISSET(it->_serv_sock.getSocket(), &_fdset))
		{
			new_connection = accept(it->_serv_sock.getSocket(), it->_serv_sock.castAddr(), it->_serv_sock.getAdLen());
			if (new_connection < 0)
			{
	//			std::cout << _BL_RED << "ERROR : " << _NOR << "ACCEPT ERROR" << std::endl;
				return false;
			}
			fcntl(new_connection, F_SETFL, O_NONBLOCK);
		//	std::cout << _GRE << "NEW CONNECTION" << _NOR << ", SOCKET FD IS " << new_connection << ", PORT IS: " << ntohs(it->_serv_sock.getAddr().sin_port) << std::endl;
		}
	}
	for (int i(0); i < MAX_CLIENTS; i++)
	{
		if (_clients_sd[i] == 0)
		{
			_clients_sd[i] = new_connection;
			break ;
		}
	}
	/*
	for (int j(0); j < 200; j++)
	{
		std::cout << _GRE << _clients_sd[j] << " " << _NOR;
	}
	std::cout << std::endl; */
	return true;
}

int	Service::check_methods(request req) {
	std::vector<std::string>::iterator ite = req.location_path.allow_methods.begin();
	int yes = 0;
	if (ite == req.location_path.allow_methods.end()) {
		if (req.methods != "GET") {
			yes = 0;
		}
		else 
			yes = 1;
	}
	else {
		while (ite != req.location_path.allow_methods.end()) {
			if (req.methods == *ite) {
				yes = 1;
					break;
			}
		ite++;
		}
	}
	return yes;
}

void	Service::receive() {

	int		len_recv;

	for (int i(0); i < MAX_CLIENTS; i++)
	{	
		if(_clients_sd[i] != 0 && FD_ISSET(_clients_sd[i], &_fdset))
		{
			len_recv = recv(_clients_sd[i], _buffer, 1024, 0);
			if (len_recv < 0)
			{
				std::cout << _BL_RED << "ERROR : " << _NOR << "RECV ERROR" << std::endl;
				break;
			}
			if (len_recv == 0)
			{
				FD_CLR(_clients_sd[i], &_fdset);
				if (_clients_sd[i] != 0)
					close(_clients_sd[i]);
				_clients_sd[i] = 0;
			}
			_buffer[len_recv] = 0;
			

			std::cout << "buffer before:" << _buffer << std::endl;
			request req(_buffer, _servers);
			std::cout << "body before:" << req.body << std::endl;

			int size(len_recv);
			if (req.methods == "POST") {

				while (len_recv > 0) {
					
					for (int i = 0; i < 10024; i++) {
						_buffer[i] = '\0';
					}
					if(_clients_sd[i] != 0 && FD_ISSET(_clients_sd[i], &_fdset))
					{
						len_recv = recv(_clients_sd[i], _buffer, 1, 0);
						if (len_recv < 0)
						{
							break;
						}
						if (len_recv == 0)
						{
							FD_CLR(_clients_sd[i], &_fdset);
							if (_clients_sd[i] != 0)
								close(_clients_sd[i]);
							_clients_sd[i] = 0;
						}

						_buffer[len_recv] = 0;
						req.body += _buffer;
						size += len_recv;
					}
					else {
						break;
					}
				}
			}

			
			if (req.chunked != -1) {
				while (req.chunked != 1) {
					len_recv = recv(_clients_sd[i], _buffer, 10024, 0);
					if (len_recv < 0)
					{
//						std::cout << _BL_RED << "ERROR : " << _NOR << "RECV ERROR" << std::endl;
						break;
					}
					if (len_recv == 0)
					{
						FD_CLR(_clients_sd[i], &_fdset);
						if (_clients_sd[i] != 0)
							close(_clients_sd[i]);
						_clients_sd[i] = 0;
					}
					_buffer[len_recv] = 0;
				}
				std::string res(_buffer);
				if (res == "\0\r\n" || res == "\0\r\n\r\n") {
					req.chunked = 1;
				}
				req.body +=  _buffer;

			}


			Response	resp(req);
				int is_valid_method = check_methods(req);
			 if (req.validity  >= 400) {
			 	resp.auto_response();
			 }
				else if (is_valid_method == 0) 
				{
					std::cout << "Mehtod not allowed" << std::endl;
					resp.set_validity(405);
					resp.responseGet(_servers);
				}
				else if (req.methods == "GET")
				{
					resp.responseGet(_servers);
				}
				else if (req.methods == "POST") {
					resp.responsePost(_servers);
				}
				else if (req.methods == "DELETE") {
					resp.responseDelete();
				}
				if (resp.getBody().length() > (unsigned long)resp._request.location_path.max_client) {
					resp._request.validity = 400;
					resp._resp = "";
					resp.auto_response();
				}
				std::cout << resp.getResponse() << std::endl;
				if (!(req.methods == "DELETE" && resp.is_request_valid() < 400)) {
				sending(i, resp);
				}
			if (req.validity != 0)
			{
				FD_CLR(_clients_sd[i], &_fdset);
				if (_clients_sd[i] != 0)
					close(_clients_sd[i]);
				_clients_sd[i] = 0;
			}
		}	
	}
}

void	Service::sending(int i, Response resp) {
	if (resp.getResponse().size() != 0) {
		send(_clients_sd[i], resp.getResponse().c_str(), resp.getResponse().length(), 0);
		return ;
	}
	std::string new_rep("HTTP/1.1 200 Ok\r\nContent-Length: 0\r\n\r\n");
	send(_clients_sd[i], new_rep.c_str(), new_rep.length(), 0);
}


/* RUSH FINAL POUR CE WEEKEND

- finir l'auto index pour /
- faire une bonne hierarchie du dossier www, et du fichier conf
- faire de belles pages html
- integrer les bons codes d'erreurs en fonctons des erreurs (404 pour 404 et pas 400 pour tout)
- clean le code
- ramener Rayan

*/