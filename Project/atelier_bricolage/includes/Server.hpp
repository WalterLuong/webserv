/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 03:01:49 by wluong            #+#    #+#             */
/*   Updated: 2022/06/09 02:42:49 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "define.hpp"

class Socket;

class Server
{

	private:

		Socket			_serv_sock;
		// server_block	infos;
		Server();


	public:

		Server( int port );
		// Server( Server const & src );
		~Server();

		// Server& operator=( Server const & other );
		
};

#endif