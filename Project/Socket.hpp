/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 05:23:33 by wluong            #+#    #+#             */
/*   Updated: 2022/05/25 05:14:53 by wluong           ###   ########.fr       */
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

# define BIND_ERROR 1
# define LISTEN_ERROR 2
# define SOCKET_ERROR 3

class Socket
{
	private:

		int				_server_fd;
		int				_error;
		sockaddr_in		_addr;
		socklen_t		_addr_len;

	public:

		Socket(int port);
		Socket(Socket const & src);
		~Socket();

		// Socket& operator=(Socket const & other);

		void		create_socket();
		void	fill_sockaddr(int port);
		void		binding();
		void		listening(int backlog);

		sockaddr_in		getAddr() const;
		socklen_t	*	getAdLen();
		int				getSocket() const;
		int				getError() const;
		struct sockaddr		* castAddr() const;

};

#endif