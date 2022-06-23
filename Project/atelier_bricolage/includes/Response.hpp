/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 05:15:07 by wluong            #+#    #+#             */
/*   Updated: 2022/06/23 03:19:41 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "parsing_request.hpp"
# include "ResponseHeader.hpp"
# include "define.hpp"
# include <fstream>

class Response
{

	private:

		ResponseHeader	_header;
		std::string		_body;
		request			_request;
		std::string		_resp;

	public:
	
		Response();
		~Response();

		std::string		getHeader();
		std::string		getBody();
		std::string		getResponse();

		void			responseGet();
		void			responsePost();
		void			responseDelete();
		void			responseCGI();
		void			responseAutoIndex();
		std::string		setErrorPage();
		std::string		readFromFile(std::string path);

};

#endif