/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 05:00:45 by wluong            #+#    #+#             */
/*   Updated: 2022/06/08 05:03:08 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server( int port ) : _serv_sock(port)
{
	std::cout << _BL_GRE << "SERVER CREATED" << _NOR << std::endl;
}

// Server::Server( Server const & src ) 
// {
// }

Server::~Server()
{
	std::cout << _BL_RED << "SERVER SHUTTED DOWN" << _NOR << std::endl;
}

// Server& Server::operator=( Server const & other ) {
// }