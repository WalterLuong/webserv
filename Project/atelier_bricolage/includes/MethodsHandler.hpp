/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodsHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdidier <jdidier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 23:42:19 by jdidier           #+#    #+#             */
/*   Updated: 2022/06/14 21:12:34 by jdidier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODSHANDLER_HPP
# define METHODSHANDLER_HPP

# include <string>
# include <map>
# include <iostream>
# include "../includes/parsing_conffile.hpp"
# include "../includes/parsing_request.hpp"

class MethodsHandler {
	public:
		typedef	std::string			body;
		typedef	body (*exec_method)(void);
	private:
		server_conf						_sconf;
		std::map<std::string, std::string (MethodsHandler::*)()>	_methods;
		

		std::string		make_header(int const& code);

	public:
		static std::map<std::string, bool>			METHODS_LIST;
		static std::map<std::string, std::string>	HTTP_CODE_LIST;
		static std::map<std::string, std::string>	FILE_TYPE_LIST;
		MethodsHandler();
		MethodsHandler(MethodsHandler const& src);
		~MethodsHandler();
		MethodsHandler	&operator=(MethodsHandler const& rhs);

		std::string	execute(void);
		std::string	exec_get(void);
		std::string	exec_post(void);
		std::string	exec_delete(void);
};

std::map<std::string, std::string> init_default_error();
std::map<std::string, std::string> init_file_type();

#endif