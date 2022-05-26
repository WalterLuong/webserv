/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 14:30:26 by jdidier           #+#    #+#             */
/*   Updated: 2022/05/26 05:24:29 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <iostream>
# include <istream>
# include <fstream>
# include <cstring>
# include <string>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <vector>
# include "Socket.hpp"

// int		main() {

// 	int		opt = 1;
// 	int		client_socket[30];
// 	int		max_clients = 30;
// 	int		new_socket;
// 	int		max_sockdescr;
// 	int		activity;
// 	int		socket_d;
// 	int		valread;


// 	char buff[1025];

// 	fd_set	read_fd;

// 	Socket	master_socket(8080);

// 	for (int i(0); i < max_clients; i++) {
// 		client_socket[i] = 0;
// 	}

// 	if (master_socket.getError() == SOCKET_ERROR)
// 		exit(EXIT_FAILURE);
	
// 	if ( setsockopt(master_socket.getSocket(), SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
// 	{
// 		std::cout << "SETSOCKOPT ERROR" << std::endl;
// 		exit(EXIT_FAILURE);
// 	}
// 	master_socket.binding();
// 	if (master_socket.getError() == BIND_ERROR)
// 		exit(EXIT_FAILURE);
// 	std::cout << "Listening on port: " << ntohs(master_socket.getAddr().sin_port) << std::endl;
// 	master_socket.listening(10);
// 	if (master_socket.getError() == LISTEN_ERROR)
// 		exit(EXIT_FAILURE);
// 	std::cout << "~~~~ Waiting for new connection ~~~~" << std::endl << std::endl;

// 	while (1)
// 	{
// 		FD_ZERO(&read_fd);
		
// 		FD_SET(master_socket.getSocket(), &read_fd);
// 		max_sockdescr = master_socket.getSocket();

// 		for (int i(0); i < max_clients; i++) {
// 			socket_d = client_socket[i];
// 			if (socket_d > 0)
// 				FD_SET(socket_d, &read_fd);
// 			if (socket_d > max_sockdescr)
// 				max_sockdescr = socket_d;
// 		}
// 		activity = select(max_sockdescr + 1, &read_fd, NULL, NULL, NULL);
// 		if (activity < 0)
// 			std::cout << "SELECT ERROR" << std::endl;
// 		if (FD_ISSET(master_socket.getSocket(), &read_fd))
// 		{
// 			new_socket = accept(master_socket.getSocket(), master_socket.castAddr(), master_socket.getAdLen());
// 			if (new_socket < 0) {
// 				std::cout << "ACCEPT FAILED" << std::endl;
// 				exit(EXIT_FAILURE);
// 			}
// 			std::cout << "NEW CONNECTION, SOCKET FD IS " << new_socket << ", PORT IS: " << ntohs(master_socket.getAddr().sin_port) << std::endl;
// 			if ( send(new_socket, "Hello !\n", 8, 0) != 8 )
// 				std::cout << "SEND ERROR" << std::endl;
// 			std::cout << "WELCOME MESSAGE SENT SUCCESSFULLY" << std::endl;
// 			for (int i(0); i < max_clients; i++) {
// 				if (client_socket[i] == 0) {
// 					client_socket[i] = new_socket;
// 					std::cout << "Socket added at position " << i << std::endl;
// 					break;
// 				}
// 			}
// 		}
// 		for (int i(0); i < max_clients; i++) {
// 			socket_d = client_socket[i];
// 			if (FD_ISSET(socket_d, &read_fd))
// 			{
// 				if ((valread = read( socket_d, buff, 1024)) == 0)
// 				{
// 					getsockname(socket_d, master_socket.castAddr(), master_socket.getAdLen());
// 					std::cout << "HOST DISCONNECTED" << std::endl;
// 					close(socket_d);
// 					client_socket[i] = 0;
// 				}
// 				else
// 				{
// 					buff[valread] = '\0';
// 					send(socket_d, buff, strlen(buff), 0);
// 				}
// 			}
// 		}
// 	}
// 	return 0;
// }

int		main() {

	typedef std::vector<int>::iterator		iterator;

	int		opt = 1;
	
	std::vector<int>	clients;

	int		new_socket;
	int		max_sockdescr;
	int		activity;
	int		socket_d;

	std::string		buff;

	fd_set	read_fd;

	Socket	master_socket(8080);

	if (master_socket.getError() == SOCKET_ERROR)
		exit(EXIT_FAILURE);
	
	if ( setsockopt(master_socket.getSocket(), SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cout << "SETSOCKOPT ERROR" << std::endl;
		exit(EXIT_FAILURE);
	}

	master_socket.binding();
	if (master_socket.getError() == BIND_ERROR)
		exit(EXIT_FAILURE);

	std::cout << "Listening on port: " << ntohs(master_socket.getAddr().sin_port) << std::endl;
	master_socket.listening(10);
	if (master_socket.getError() == LISTEN_ERROR)
		exit(EXIT_FAILURE);

	std::cout << "~~~~ Waiting for new connection ~~~~" << std::endl << std::endl;

	while (1)
	{

		FD_ZERO(&read_fd);
		
		FD_SET(master_socket.getSocket(), &read_fd);
		max_sockdescr = master_socket.getSocket();
		for (iterator it = clients.begin(); it != clients.end(); it++) {
			socket_d = *it;
			if (socket_d > 0)
				FD_SET(socket_d, &read_fd);
			if (socket_d > max_sockdescr)
				max_sockdescr = socket_d;
		}

		activity = select(max_sockdescr + 1, &read_fd, NULL, NULL, NULL);
		if (activity < 0)
			std::cout << "SELECT ERROR" << std::endl;
		
		if (FD_ISSET(master_socket.getSocket(), &read_fd))
		{
			new_socket = accept(master_socket.getSocket(), master_socket.castAddr(), master_socket.getAdLen());
			if (new_socket < 0) {
				std::cout << "ACCEPT FAILED" << std::endl;
				exit(EXIT_FAILURE);
			}
			std::cout << "NEW CONNECTION, SOCKET FD IS " << new_socket << ", PORT IS: " << ntohs(master_socket.getAddr().sin_port) << std::endl;
			
			std::ifstream is("./www/index.html"); // open a file and get stream
			std::string line; //c++ buffer style
			if (is.is_open()) {
				while (std::getline(is, line)) { //get a line
					buff += line + '\n';
				}
			}
			is.close();
			write(new_socket, &buff, buff.length());

			if ( send(new_socket, &buff, buff.length(), 0) != 8 )
				std::cout << "SEND ERROR" << std::endl;
			std::cout << "WELCOME MESSAGE SENT SUCCESSFULLY" << std::endl;

			clients.push_back(new_socket);
		}
		for (iterator it = clients.begin(); it != clients.end(); it++) {
			socket_d = *it;
			if (FD_ISSET(socket_d, &read_fd))
			{
				if (!read( socket_d, &buff, buff.length()))
				{
					getsockname(socket_d, master_socket.castAddr(), master_socket.getAdLen());
					std::cout << "HOST DISCONNECTED" << std::endl;
					close(socket_d);
					clients.pop_back();
				}
				else
					send(socket_d, &buff, buff.length(), 0);
			}
		}
	}
	return 0;

}