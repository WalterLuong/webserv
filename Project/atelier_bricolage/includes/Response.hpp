/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 05:15:07 by wluong            #+#    #+#             */
/*   Updated: 2022/06/11 05:23:43 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "define.hpp"

class Response
{

	private:

		std::string		_header;
		std::string		_body;
		std::string		_request;
		std::string		_resp;

	public:
	
		Response(/* args */);
		~Response();

		std::string		getHeader()
		std::string		

};

#endif