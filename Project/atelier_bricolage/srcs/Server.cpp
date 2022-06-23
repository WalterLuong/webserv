/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 05:00:45 by wluong            #+#    #+#             */
/*   Updated: 2022/06/23 03:52:56 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server( int port, std::string ip, server_block serv_info ) : _serv_sock(port, ip), infos(serv_info) {}

// Server::Server( Server const & src ) 
// {
// }

Server::~Server() {}

// Server& Server::operator=( Server const & other ) {
// }