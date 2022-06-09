/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 14:30:26 by jdidier           #+#    #+#             */
/*   Updated: 2022/06/09 03:15:29 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "includes/define.hpp"

int		main() {
	
	// server_conf		serv_conf;
	Service			webserv;


	// if (parsing(&serv_conf, ac, av) != 0)
	// {
	// 	std::cout << "error config file" << std::endl;
	// 	return (1);
	// }


	for (int i(8000); i < 8050; i += 10)
	{
		std::cout << "i = " << i << std::endl;
		webserv._servers.push_back(Socket(i));
	}
	std::cout << webserv._servers.size() << std::endl;
	for (unsigned long i(0); i < webserv._servers.size(); i++)
	{
		std::cout << "PORT OF SOCKET " << i + 1 << " = " << ntohs(webserv._servers.at(i).getAddr().sin_port) << std::endl;
	}
	std::cout << "WAITING FOR CONNECTION" << std::endl;
	webserv.run_service();
}

