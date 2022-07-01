/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viporten <viporten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 05:15:07 by wluong            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/07/01 05:34:20 by wluong           ###   ########.fr       */
=======
/*   Updated: 2022/07/01 05:37:49 by viporten         ###   ########.fr       */
>>>>>>> 6bc47771ffc80a79616ea1c314c4f4a4a80cd2e6
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

		Response();

	public:
	
		Response(request req);
		~Response();

		std::string		getHeader();
		std::string		getBody();
		std::string		getResponse();

		void			responseGet(std::vector<Server> lst_server);
		void			responsePost();
		void			responseDelete();
		void			responseCGI();
		void			responseAutoIndex();

		std::string		setErrorPage();
		std::string		readFromFile(std::string path);
		std::string		readFromFile(std::string path, int i);
		void			auto_response();
		void			set_validity(int value);
		int				is_request_valid();

		void			createHeader();
		void			setBody( std::string body );

};

#endif