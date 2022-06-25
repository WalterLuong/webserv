/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeader.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 04:14:14 by wluong            #+#    #+#             */
/*   Updated: 2022/06/25 10:03:28 by wluong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHEADER_HPP
# define RESPONSEHEADER_HPP

# include "define.hpp"

class ResponseHeader
{

	private:

		std::string		_status;
		std::string		_date;
		std::string		_serverName;
		std::string		_contentLength;
		std::string		_contentType;
		std::string		_header;
		int				_statuscode;
		int				_bodyLength;

	public:

		ResponseHeader();
		ResponseHeader( ResponseHeader const & src );
		~ResponseHeader();

		// ResponseHeader & operator=( ResponseHeader const & other );

		std::string		getStatus() const;
		std::string		getDate() const;
		std::string		getServerName() const;
		std::string		getContentLength() const;
		std::string		getContentType() const;
		std::string		getHeader() const;

		void			setStatus( std::string const & protocol, std::string const & status );
		void			setDate();
		void			setServerName( std::string const & str );
		void			setContentLength();
		void			setContentType( std::string const & str );

		void			setStatusCode( int code );
		void			setBodyLength( int length );

		void			generateHeader();

};

#endif