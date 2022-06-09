/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 14:30:26 by jdidier           #+#    #+#             */
/*   Updated: 2022/06/09 05:05:03 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "includes/define.hpp"
# include "includes/parsing_conffile.hpp"
# include "includes/webserve.hpp"
# include "includes/Socket.hpp"
# include "includes/Server.hpp"
# include "includes/Service.hpp"

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
		webserv._servers.push_back(Server(serv_conf.server.at(i).port_ip.at(0).first, serv_conf.server.at(i)));
	}

	std::cout << webserv._servers.size() << std::endl;
	for (unsigned long i(0); i < webserv._servers.size(); i++)
	{
		std::cout << "PORT OF SOCKET " << i + 1 << " = " << ntohs(webserv._servers.at(i)._serv_sock.getAddr().sin_port) << std::endl;
	}
	std::cout << "WAITING FOR CONNECTION" << std::endl;
	webserv.run_service();
}

