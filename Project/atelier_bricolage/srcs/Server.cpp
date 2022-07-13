/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viporten <viporten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 05:00:45 by wluong            #+#    #+#             */
/*   Updated: 2022/07/14 01:44:10 by viporten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server( int port, std::string ip, server_block serv_info ) : _serv_sock(port, ip), infos(serv_info) {}



Server::~Server() {}

