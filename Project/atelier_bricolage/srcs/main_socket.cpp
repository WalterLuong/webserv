/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_socket.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 14:30:26 by jdidier           #+#    #+#             */
/*   Updated: 2022/06/08 07:20:29 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/define.hpp"

// int		main() {

	// int		opt = 1;
	// int		client_socket[30];
	// int		max_clients = 30;
	// int		new_socket;
	// int		max_sockdescr;
	// int		activity;
	// int		socket_d;
	// int		valread = 0;


	// char buff[30000 + 1];

	// fd_set	read_fd;

	// Socket	master_socket(8080);
	// struct timeval tv;

	// for (int i(0); i < max_clients; i++) {
	// 	client_socket[i] = 0;
	// }

	// if (master_socket.getError() == SOCKET_ERROR)
	// 	exit(EXIT_FAILURE);
	
	// if ( setsockopt(master_socket.getSocket(), SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	// {
	// 	std::cout << "SETSOCKOPT ERROR" << std::endl;
	// 	exit(EXIT_FAILURE);
	// }
	// master_socket.binding();
	// if (master_socket.getError() == BIND_ERROR)
	// 	exit(EXIT_FAILURE);
	// std::cout << "Listening on port: " << ntohs(master_socket.getAddr().sin_port) << std::endl;
	// master_socket.listening(500);
	// if (master_socket.getError() == LISTEN_ERROR)
	// 	exit(EXIT_FAILURE);
	// std::cout << "~~~~ Waiting for new connection ~~~~" << std::endl << std::endl;



	// while (1)
	// {
	// 	// tv.tv_sec = 1;
	// 	// tv.tv_usec = 0;
	// 	FD_ZERO(&read_fd);
	// 	FD_SET(master_socket.getSocket(), &read_fd);
		// max_sockdescr = master_socket.getSocket();

		// for (int i(0); i < max_clients; i++) {
		// 	socket_d = client_socket[i];
		// 	if (socket_d > 0)
		// 		FD_SET(socket_d, &read_fd);
		// 	if (socket_d > max_sockdescr)
		// 		max_sockdescr = socket_d;
		// }
		// activity = select(max_sockdescr + 1, &read_fd, NULL, NULL, &tv);
		// if (activity < 0)
		// 	std::cout << "SELECT ERROR" << std::endl;
		// master_socket.accept_connections();
		// if (FD_ISSET(master_socket.getSocket(), &read_fd))
		// {
		// 	new_socket = accept(master_socket.getSocket(), master_socket.castAddr(), master_socket.getAdLen());
		// 	if (new_socket < 0) {
		// 		std::cout << "ACCEPT FAILED" << std::endl;
		// 		exit(EXIT_FAILURE);
		// 	}
		// 	else {
		// 		fcntl(new_socket, F_SETFL, O_NONBLOCK);
		// 	}
		// 	std::cout << "NEW CONNECTION, SOCKET FD IS " << new_socket << ", PORT IS: " << ntohs(master_socket.getAddr().sin_port) << std::endl;
		// 	if ( send(new_socket, "Hello World!\n", 8, 0) != 8 )
		// 		std::cout << "SEND ERROR" << std::endl;
		// 	std::cout << "WELCOME MESSAGE SENT SUCCESSFULLY" << std::endl;
		// 	for (int i(0); i < max_clients; i++) {
		// 		if (client_socket[i] == 0) {
		// 			client_socket[i] = new_socket;
		// 			std::cout << "Socket added at position " << i << std::endl;
		// 			break;
		// 		}
		// 	}
		// }
		// for (int i(0); i < max_clients; i++) {
		// 	socket_d = client_socket[i];
		// 	if (FD_ISSET(socket_d, &read_fd))
		// 	{
		// 		valread = read( socket_d, buff, 30000);
		// 		if (valread == 0)
		// 		{
		// 			getsockname(socket_d, master_socket.castAddr(), master_socket.getAdLen());
		// 			std::cout << "HOST DISCONNECTED" << std::endl;
		// 			close(socket_d);
		// 			client_socket[i] = 0;
		// 		}
		// 		else
		// 		{
		// 			buff[valread] = '\0';
		// 			write(socket_d, buff, strlen(buff));
		// 		}
		// 	}
		// }
	// }
// 	close (socket_d);
// 	return 0;
// }

int		main(int ac, char **av) {
	
	server_conf		serv_conf;
	Service			webserv;


	if (parsing(&serv_conf, ac, av) != 0)
	{
		std::cout << "error config file" << std::endl;
		return (1);
	}

	for (int i(8000); i < 8050; i+ 10)
	{
		Socket to_push(i);
		webserv.getServers().push_back(to_push);
	}
	webserv.run_service();
}