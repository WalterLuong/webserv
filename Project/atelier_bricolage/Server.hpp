/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 03:01:49 by wluong            #+#    #+#             */
/*   Updated: 2022/05/26 05:01:12 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

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

class Server
{

	private:

		fd_set				_fdset;
		Socket				_ms;
		std::vector<int>	_clients;
		


	public:

		Server();
		~Server();

		RunServer();

};

#endif