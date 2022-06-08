/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 05:23:33 by wluong            #+#    #+#             */
/*   Updated: 2022/06/08 04:27:03 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "define.hpp"

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

	public:

		Socket();
		Socket(int port);
		Socket(Socket const & src);
		~Socket();

		Socket& operator=(Socket const & other);

		int					setup(int backlog);
		void				create_socket();
		void				fill_sockaddr(int port);
		void				binding();
		void				listening(int backlog);

		sockaddr_in			getAddr() const;
		socklen_t	*		getAdLen();
		int					getSocket() const;
		int					getError() const;
		struct sockaddr		* castAddr() const;

};

#endif