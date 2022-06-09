/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 03:51:03 by wluong            #+#    #+#             */
/*   Updated: 2022/06/09 02:43:05 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
# define DEFINE_HPP

# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <iostream>
# include <istream>
# include <fstream>
# include <cstring>
# include <string>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <fcntl.h>
# include <vector>

// # include "parsing_conffile.hpp"
// # include "webserve.hpp"
# include "Socket.hpp"
// # include "Server.hpp"
# include "Service.hpp"


# define BIND_ERROR		1
# define LISTEN_ERROR	2
# define SOCKET_ERROR	3


# define TRUE			1
# define FALSE			0


# define _RED 			"\033[1;31m"
# define _BL_RED		"\033[1;5;31m"
# define _GRE 			"\033[1;32m"
# define _BL_GRE		"\033[1;5;32m"
# define _BLU 			"\033[1;35m"
# define _YEL 			"\033[1;33m"
# define _NOR 			"\033[m"

#endif