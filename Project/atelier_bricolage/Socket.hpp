/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 05:23:33 by wluong            #+#    #+#             */
/*   Updated: 2022/06/03 05:08:58 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

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
#include <fcntl.h>
# include <vector>

# define BIND_ERROR 1
# define LISTEN_ERROR 2
# define SOCKET_ERROR 3

# define _RED "\033[1;31m"
# define _GRE "\033[1;32m"
# define _BLU "\033[1;35m"
# define _YEL "\033[1;33m"
# define _NOR "\033[m"

struct Client
{
	bool			connected;
	sockaddr_in		address;
	int				client_fd;
	fd_set			client_data;
	socklen_t		client_addr_len;
};



class Socket
{
	private:

		int					_server_fd;
		int					_error;
		sockaddr_in			_addr;
		socklen_t			_addr_len;
		int					_max_clients;
		Client				_clients_fd[_max_clients];
		

	public:

		Socket();
		Socket(int port);
		Socket(Socket const & src);
		~Socket();

		Socket& operator=(Socket const & other);

		int			setup(int backlog);
		void		create_socket();
		void		fill_sockaddr(int port);
		void		binding();
		void		listening(int backlog);
		bool		accept_client(Client *current);
		int			accept_connections();
		void		disconnect(Client *current);
		bool		client_receive(Client *current);
		void		receive_data();
		int			send_data(Client *current, char *buff, in len);

		sockaddr_in			getAddr() const;
		socklen_t	*		getAdLen();
		int					getSocket() const;
		int					getError() const;
		struct sockaddr		* castAddr() const;

};

#endif