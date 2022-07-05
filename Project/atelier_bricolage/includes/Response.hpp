/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlormois <mlormois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 05:15:07 by wluong            #+#    #+#             */
/*   Updated: 2022/07/05 08:33:16 by mlormois         ###   ########.fr       */
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

		void			createHeader(std::string & extension, std::vector<Server> & lst_server);
		void			setBody( std::string body );

};

std::string randomdigits(int nb);

std::string create_tmpfile(void);
std::string joinstr(std::string *strs, int n);
int run_bin(std::string bin, std::string infilename, std::string outfilename);
int write_infile(std::string infile, std::string body);
std::string get_file_content(std::string filename);
std::string cgi_execution(std::string bin, std::string body);
std::string tostr(int k);
std::string get_sub_str(std::string str, size_t pos);
void set_env(request &req, std::string &absolutepath);
std::string		cgi_handler(request &req, std::string path_for_access, int extension_pos);
int			get_cgi_path_pos(std::string extension, std::vector<std::pair<std::string, std::string> > cgi_path);

#endif