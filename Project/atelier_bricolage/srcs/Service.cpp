/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viporten <viporten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 06:27:48 by wluong            #+#    #+#             */
/*   Updated: 2022/06/26 03:12:16 by viporten         ###   ########.fr       */
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
	for (int i(0); i < MAX_CLIENTS; i++)
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
	for (int i(0); i < MAX_CLIENTS; i++)
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
			fcntl(new_connection, F_SETFL, O_NONBLOCK);
			std::cout << _GRE << "NEW CONNECTION" << _NOR << ", SOCKET FD IS " << new_connection << ", PORT IS: " << ntohs(it->_serv_sock.getAddr().sin_port) << std::endl;
		}
		for (int i(0); i < MAX_CLIENTS; i++)
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

	for (int i(0); i < MAX_CLIENTS; i++)
	{	
		if(FD_ISSET(_clients_sd[i], &_fdset))
		{
			len_recv = recv(_clients_sd[i], _buffer, 10024, 0);
			if (len_recv < 0)
			{
				std::cout << _BL_RED << "ERROR : " << _NOR << "RECV ERROR" << std::endl;
				break;
			}
			if (len_recv == 0 || (len_recv == 1 && _buffer[0] == 4))
			{
				close(_clients_sd[i]);
				_clients_sd[i] = 0;
			}
			_buffer[len_recv] = 0;
			std::cout << _buffer << std::endl;
			
			//do while

			request req(_buffer, _servers);
			std::cout << "bite" <<std::endl;
			// _buffer[0] = 0;
//			if (req)
			// {
			Response	resp(req);
			resp.responseGet();
			std::cout << resp.getResponse() << std::endl;
			sending(i, resp);
			std::string img;
			img = "Content-Type: image/jpeg\r\n";
			img += "Last-Modified: Wed, 22 Jun 2022 00:32:45 GMT \r\n";
			Response rsp2(req);
			rsp2.setBody(img);
		//	std::cout << rsp2.getResponse() << std::endl;
		//	sending(i, rsp2);
			// }

			/* IF BAD REQUEST
			create a bad request response .html
			sending this html
			close client_sd[i]
			client_sd[i] = 0 */
			close(_clients_sd[i]);
			_clients_sd[i] = 0;
		}
		// parsing request sur _buffer 
		// sending doit recevoir la stc du parsing request

		// METHOD TREATMENT: after parsing
		// GET : CGI ? -> Create CGI 
		// check ressource at / 
		// 
	}
}

void	Service::sending(int i, Response resp) {
	// std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\n\n";

	// header += "<!DOCTYPE html>\n";
	// header += "<html>\n";
	// header += "<style>\n";
	// header += "body {\n";
	// header += "background-image: url('https://4.bp.blogspot.com/-W0_HZx9NWfI/XD45qcBbaII/AAAAAAAAAn4/paWYCWR06l092c0vcBV2G9HAChXB3nhRwCKgBGAs/w919/astronaut-space-13-4k.jpg');\n";
	// header += "background-size: cover; \n";
	// header += "background-attachment: fixed;\n";
	// header += "background-position: center;\n";
	// header += "}\n";
	// header += "div {\n";
	// header += "margin-top: 60%;\n";
	// header += "text-align: center;\n";
	// header += "position: center;\n";
	// header += "}\n";
	// header += "img {\n";
	// header += "width:  25%;\n";
	// header += "height: 25%;\n";
	// header += "}\n";
	// header += "</style>\n";
	// header += "<body>\n";
	// header += "<div>\n";
	// header += "<img ";
	// header += "src=\"https://toppng.com/uploads/preview/erreur-404-11550708744oo95egrxlp.png\" ";
	// header += "alt=\"error message 404\">\n";
	// header += "</div>\n";
	// header += "</body>\n";
	// header += "</html>\n";

	send(_clients_sd[i], resp.getResponse().c_str(), resp.getResponse().length(), 0);
}

// std::vector<Socket>		&Service::getServers() const {
// 	return _servers;
// }
