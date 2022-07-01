/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 14:30:26 by jdidier           #+#    #+#             */
/*   Updated: 2022/07/01 02:20:08 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "includes/define.hpp"
# include "includes/parsing_conffile.hpp"
# include "includes/webserve.hpp"
# include "includes/Socket.hpp"
# include "includes/Server.hpp"
# include "includes/Service.hpp"
# include <signal.h>

void	sig_handler(int signum) {
	signum = 1;
	std::cout << _BL_GRE << "\b\b  \nWebserv is correctly closed bg !\n\n" << _NOR << std::endl;
	
	exit(EXIT_SUCCESS);
}

int		main(int ac, char **av) {
	
	server_conf		serv_conf;
	Service			webserv;

	if (parsing(&serv_conf, ac, av) != 0)
	{
		std::cout << "error config file" << std::endl;
		return (1);
	}
	for (unsigned long i(0); i < serv_conf.server.size(); i++)
	{
		std::cout << serv_conf.server.at(i).server_name << std::endl;
		std::cout << serv_conf.server.at(i).port_ip.at(0).first << std::endl;
		std::cout << serv_conf.server.at(i).port_ip.at(0).second << std::endl;
		webserv._servers.push_back(Server(serv_conf.server.at(i).port_ip.at(0).first, serv_conf.server.at(i).port_ip.at(0).second,serv_conf.server.at(i)));
	}

	std::cout << webserv._servers.size() << std::endl;
	for (unsigned long i(0); i < webserv._servers.size(); i++)
	{
		std::cout << "PORT OF SOCKET " << i + 1 << " = " << ntohs(webserv._servers.at(i)._serv_sock.getAddr().sin_port) << std::endl;
		std::cout << "IP OF SOCKET " << i + 1 << " = " << serv_conf.server.at(i).port_ip.at(0).second << std::endl;
	}
	std::cout << "WAITING FOR CONNECTION" << std::endl;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	webserv.run_service();
}

