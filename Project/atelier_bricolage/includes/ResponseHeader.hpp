/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeader.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wluong <wluong@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 04:14:14 by wluong            #+#    #+#             */
/*   Updated: 2022/06/23 04:45:25 by wluong           ###   ########.fr       */
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
		std::string		_lastModified;
		std::string		_contentLength;
		std::string		_contentType;
		std::string		_header;

	public:

		ResponseHeader();
		ResponseHeader( ResponseHeader const & src );
		~ResponseHeader();

		// ResponseHeader & operator=( ResponseHeader const & other );

		std::string		getStatus() const;
		std::string		getDate() const;
		std::string		getServerName() const;
		std::string		getLastModified() const;
		std::string		getContentLength() const;
		std::string		getContentType() const;
		std::string		getHeader() const;

		void			setStatus( std::string const & statuscode, std::string const & status );
		void			setDate( std::string const & str );
		void			setServerName( std::string const & str );
		void			setLastModified( std::string const & str );
		void			setContentLength( std::string const & str );
		void			setContentType( std::string const & str );

		void			generateHeader();

};

#endif