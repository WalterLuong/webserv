/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdidier <jdidier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 14:30:26 by jdidier           #+#    #+#             */
/*   Updated: 2022/05/19 15:41:28 by jdidier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <string.h>
# include <stdio.h>

int		main(void) {
	int	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int	new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	const int PORT = 8080;
	char buff[30000 + 1];

	if (server_fd < 0) {
		std::cout << "SOCKET CREATION FAILED" << std::endl; // error management
		return 1;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY = 0.0.0.0
	address.sin_port = htons(PORT);
	std::memset(address.sin_zero, '\0', sizeof(address.sin_zero));

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		std::cout << "BIND FAILED" << std::endl; // error management
		return 1;
	}
	if (listen(server_fd, 10) < 0) {
		std::cout << "LISTEN FAILED" << std::endl;
		return 1;
	}
	while (1) {
		std::cout << "~~~~ Waiting for new connection ~~~~\n\n";
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (new_socket < 0) {
			std::cout << "ACCEPT FAILED" << std::endl;
			return 1;
		}
		int valread = read(new_socket, buff, 30000);
		buff[valread] = '\0';
		printf("%s\n", buff);
		write(new_socket, "Hello fron server!", 18);
		std::cout << "~~~~ Message sent ~~~~" << std::endl;
		close(new_socket);
	}

	return 0;
}