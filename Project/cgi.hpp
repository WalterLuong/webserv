/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdidier <jdidier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 13:49:07 by jdidier           #+#    #+#             */
/*   Updated: 2022/05/30 19:57:59 by jdidier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <unistd.h>
# include <string>
# include <map>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include <iostream>
# include <cstring>

# define nll NULL


class Cgi {

	public:
		typedef std::map<std::string, std::string>		Env;
	private:
		Env				_env;
		std::string		_script;
	public:
		Cgi(std::string const& script);
		Cgi(Cgi const& src);
		~Cgi();
		Cgi				&operator=(Cgi const& src);
		Env				getEnv(void) const;
		char			**_getEnvAsCstrArray() const;

		std::string		execute(void);
};

#endif