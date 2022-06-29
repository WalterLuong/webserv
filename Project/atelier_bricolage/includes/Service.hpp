/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlormois <mlormois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 06:11:24 by wluong            #+#    #+#             */
/*   Updated: 2022/06/29 15:38:18 by mlormois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICE_HPP
# define SERVICE_HPP

# include "Server.hpp"
# include "define.hpp"
# include "parsing_request.hpp"
# include "Response.hpp"
# include "utils.hpp"

class Server;

class Service
{

	private:

		char					_buffer[10025];
		int						_clients_sd[512];
		int						_max_sd;
		fd_set					_fdset;
		struct timeval 			_timeout;

	public:
		std::vector<Server>		_servers;

		Service();
		// Service( Service const & src );
		~Service();

		// Service & operator=( Service const& other );

		int		check_methods(request req);
		void	setup_cluster();
		void	check_opened_sd();
		void	run_service();
		bool	selecting();
		bool	accepting_connections();
		void	receive();
		void	sending(int i, Response resp);

		std::vector<Socket>		&getServers() const;

};

#endif