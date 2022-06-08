/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Service.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 06:11:24 by wluong            #+#    #+#             */
/*   Updated: 2022/06/08 07:19:30 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICE_HPP
# define SERVICE_HPP

# include "define.hpp"

class Service
{

	private:

		std::vector<Socket>		_servers;
		char					_buffer[1025];
		int						_clients_sd[512];
		int						_max_sd;
		fd_set					_fdset;
		struct timeval 			_timeout;

	public:

		Service();
		Service( Service const & src );
		~Service();

		Service & operator=( Service const& other );

		void	setup_cluster();
		void	check_opened_sd();
		void	run_service();
		bool	selecting();
		bool	accepting_connections();
		void	receive();
		bool	sending(int i);

		std::vector<Socket>		getServers() const;

};

#endif