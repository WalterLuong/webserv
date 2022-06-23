/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 03:01:49 by wluong            #+#    #+#             */
/*   Updated: 2022/06/23 03:59:03 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "define.hpp"
# include "Socket.hpp"
# include "parsing_conffile.hpp"

class Socket;

class Server
{

	private:

		Server();


	public:
	
		Socket			_serv_sock;
		server_block	infos;

		Server( int port, std::string ip, server_block serv_info );
		// Server( Server const & src );
		~Server();

		// Server& operator=( Server const & other );
		
};

#endif