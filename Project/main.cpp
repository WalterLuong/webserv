/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdidier <jdidier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 14:30:26 by jdidier           #+#    #+#             */
/*   Updated: 2022/05/19 23:33:53 by jdidier          ###   ########.fr       */
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

int		main(void) {
	int	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int	new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	const int PORT = 8080;
	char buff[30000 + 1];
	std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\n";

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
		/* Accept:  */
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (new_socket < 0) {
			std::cout << "ACCEPT FAILED" << std::endl;
			return 1;
		}

		/* Read data from client request */
		int valread = read(new_socket, buff, 30000);
		buff[valread] = '\0';
		printf("%s\n", buff);
		
		/* For this demo code we suppose the request is ok and correspond
		to a file index.html in www directory*/
		std::ifstream is("./www/index.html"); // open a file and get stream
		std::string line; //c++ buffer style
		if (is.is_open()) {
			while (std::getline(is, line)) { //get a line
				header += line + '\n';
			}
		}
		is.close();
		write(new_socket, header.c_str(), strlen(header.c_str()));
		std::cout << header << std::endl;
		std::cout << "~~~~ Message sent ~~~~" << std::endl;
		close(new_socket);
	}

	return 0;
}