/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodsHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdidier <jdidier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 23:42:19 by jdidier           #+#    #+#             */
/*   Updated: 2022/06/14 02:41:10 by jdidier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODSHANDLER_HPP
# define METHODSHANDLER_HPP

# include <string>
# include <map>
# include <iostream>
#include "../includes/parsing_conffile.hpp"

class MethodsHandler {
	public:
		typedef	std::string			body;
		typedef	body (*exec_method)(void);
	private:
		server_conf						_sconf;
		std::map<std::string, exec_method>		_methods;
	public:
		static std::map<std::string, bool>	METHODS_LIST;
		MethodsHandler();
		MethodsHandler(MethodsHandler const& src);
		~MethodsHandler();
		MethodsHandler	&operator=(MethodsHandler const& rhs);

		body	execute(void);
		body	exec_get(void);
		body	exec_post(void);
		body	exec_delete(void);
};

#endif