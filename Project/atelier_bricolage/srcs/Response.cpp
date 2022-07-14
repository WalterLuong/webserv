/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlormois <mlormois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 18:56:51 by wluong            #+#    #+#             */
/*   Updated: 2022/06/28 21:13:18by mlormois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

Response::Response(request req) : _header(), _body(), _resp() , _request(req){}

Response::~Response() {}


int		Response::is_request_valid() {

	return _request.validity;
}

void	Response::set_validity(int value) {
	_request.validity = value;
}


std::string		Response::getHeader() {
	return _header.getHeader();
}

std::string		Response::getBody() {
	return _body;
}

std::string		Response::getResponse() {
	_header.generateHeader();
	_resp = _header.getHeader();
	if (_request.methods == "POST" && _request.instruction["Content-Disposition"] != "") {
		_resp = _resp.substr(0, _resp.length() -2);
		_resp += "Content-Disposition: ";
		_resp += _request.instruction["Content-Disposition"];
		_resp += "\r\n\r\n";

	}
	_resp += _body;
	return _resp;
}

void		Response::auto_response() {
		char buff[250];

		itoa(_request.validity, buff, 10);
		std::string validity_c(buff);
		


		if (_request.cur_serv_index == -1) {
			_header.setStatusCode(_request.validity);	
			_body = "";
			_body += generator_error_file(validity_c);
			_header.setBodyLength(_body.length());
			_header.setContentType("text/html");
			this->_header.setStatus("HTTP/1.1", "OK");
		}
		
		else {
			std::vector<std::pair<std::string, std::string> >::iterator ite = _request.location_path.error_page.begin();
			while (ite != _request.location_path.error_page.end()) {
				if (validity_c == ite->first) {
					size_t pos;
					if ((pos = ite->second.find_last_of(".")) == std::string::npos) {
						_request.cur_serv_index = -1;
						return auto_response();
					}
					std::string extension(ite->second.substr(pos));

					_header.setStatusCode(_request.validity);	
					_body = "";
					_body += readFromFile(ite->second);
					_header.setBodyLength(_body.length());
					_header.setContentType(_request.map_file_type[extension]);
					this->_header.setStatus("HTTP/1.1", "OK");
	
					return ;
				}
				
				ite++;
			}

			ite = _request.location_path.redirection.begin();
			while (ite != _request.location_path.redirection.end()) {
				if (validity_c == ite->first) {
					size_t pos;
					if ((pos = ite->second.find_last_of(".")) == std::string::npos) {
						_request.cur_serv_index = -1;
						return auto_response();
					}
					std::string extension(ite->second.substr(pos));

					_header.setStatusCode(_request.validity);	
					_body += readFromFile(ite->second);
					_header.setBodyLength(_body.length());
					_header.setContentType(_request.map_file_type[extension]);
					this->_header.setStatus("HTTP/1.1", "OK");
	
					return ;

				}
				ite++;
			}
			_request.cur_serv_index = -1;
			return auto_response();

		}

}


void	Response::cgi_header(std::string body) {
	std::string str;
	size_t pos;

	while (body != "") {
		pos = body.find("\n");
		if (body.find("\r\n") == 0) {
			body = body.substr(2);
			_body = body;
			break ;
		}
		if (pos == std::string::npos || pos == 0) {
			break;
		}
		str = body.substr(0, pos-1);
		body = body.substr(pos + 1); 
		size_t res;
		if ((res = str.find("Content-type: ")) == 0) {
			res = str.find(":");
			std::string bite = str.substr(res + 2);
			_header.setContentType(bite);
		}

	}
	_header.setBodyLength(_body.length());
	_header.setContentLength();
	
}


void			Response::responsePost(std::vector<Server> lst_server) { // lol victor t'as inverser, ca va boucle inf
		_body = _request.body;
	
	std::string path_for_access;
	std::string extension;
	path_for_access = _request.location_path.root;
	if (_request.filename == "")
		path_for_access += _request.location_path.index;
	else
		path_for_access += _request.filename;

	size_t pos = path_for_access.find_last_of(".");
	if (pos == std::string::npos) {
		this->_header.setStatusCode(404);
		return ;
	}
	extension = path_for_access.substr(pos);
	if (_request.location_path.cgi_path.size() != 0) {
		int pos_cgi;
		if ((pos_cgi = get_cgi_path_pos(extension, _request.location_path.cgi_path)) != -1) {
			createHeader(extension, lst_server);
			_body = cgi_handler(_request, path_for_access, pos_cgi);
			cgi_header(_body);
		return ;
		}
	}

	
				this->_header.setStatusCode(_request.validity);
				this->_header.setStatus(this->_request.get_http_version(), "OK");
				this->_header.setDate();
				this->_header.setBodyLength(this->_body.length());
				this->_header.setContentLength();
				this->_header.setContentType(_request.instruction["Content-Type"]);
		if (lst_server.size() )
			return ;
		return ;
}

void			Response::responseGet(std::vector<Server> lst_server) {
	std::string path_for_access;
	std::string	extension = "html";
	
	path_for_access = _request.location_path.root;
	if (_request.path[_request.path.length() - 1] == '/'/* && _request.location_path.autoindex == "on"*/)
	{
		if (_request.path.length() >= _request.location_path.uri.length() + 2 && _request.location_path.uri != "")
		{
			std::string	directory;
			directory = path_for_access +_request.path.substr(this->_request.location_path.uri.length() + 2, this->_request.path.length());
			directory[directory.length() - 1] = 0;
			if (isDirectory(directory))
			{
				this->_body = AutoIndexGenerator(directory);
				this->_header.setContentType("text/html");
				createHeader(extension, lst_server);
				return ;
			}
		}
		else
		{
			std::string	directory;
			directory = path_for_access + _request.path;
			directory[directory.length() - 1] = 0;
			if ((lst_server[_request.cur_serv_index].infos.autoindex == "on"))
			{
				if (isDirectory(directory))
				{
					this->_body = AutoIndexGenerator(directory);
					this->_header.setContentType("text/html");
					createHeader(extension, lst_server);
					return ;
				}
			}
		}
		if (this->_request.autoindex_on || (lst_server[_request.cur_serv_index].infos.autoindex == "on" && _request.path == "/" && this->_request.filename == ""))
		{
			this->_body = AutoIndexGenerator(path_for_access);
			this->_header.setContentType("text/html");
			createHeader(extension, lst_server);
			return ;
		}
	}
	std::string		autoindexfile;
	autoindexfile = path_for_access;
	if (_request.path.length() >= _request.location_path.uri.length() + 2 && _request.location_path.uri != "")
	{
		autoindexfile += _request.path.substr(this->_request.location_path.uri.length() + 2, this->_request.path.length());
		if (access(autoindexfile.c_str(), F_OK) == 0)
		{
			size_t pos = path_for_access.find_last_of(".");
			if (pos == std::string::npos) {
				this->_header.setStatusCode(404);
				return ;
			}
			extension = path_for_access.substr(pos);
			this->_body += readFromFile(autoindexfile);
			createHeader(extension, lst_server);
			return ;
		}
	}
	if (_request.filename == "")
		path_for_access += _request.location_path.index;
	else
		path_for_access += _request.filename;
//	std::cout << _RED << path_for_access << _NOR << std::endl;
	size_t pos = path_for_access.find_last_of(".");
	if (pos == std::string::npos) {
		this->_header.setStatusCode(404);
		return ;
	}
	extension = path_for_access.substr(pos);
		if (_request.location_path.cgi_path.size() != 0) {
				int pos_cgi;
				if ((pos_cgi = get_cgi_path_pos(extension, _request.location_path.cgi_path)) != -1) {
					createHeader(extension, lst_server);
					_body = cgi_handler(_request, path_for_access, pos_cgi);
					cgi_header(_body);
					return ;
				}
			}
	if (access(path_for_access.c_str(), F_OK) == 0 && _request.validity == 200)
	{
		this->_body += readFromFile(path_for_access);
		createHeader(extension, lst_server);
		return ;
	}
	// _request.validity = 404;
	return auto_response();
}

void			Response::createHeader( std::string & extension, std::vector<Server> & lst_server ) {
		if (extension.size() ) {
		}
				this->_header.setStatusCode(_request.validity);
				this->_header.setStatus(this->_request.get_http_version(), "OK");
				this->_header.setDate();
				this->_header.setBodyLength(this->_body.length());
				this->_header.setContentLength();
				this->_header.setContentType(_request.instruction["Content-Type"]);
				this->_header.setServerName(lst_server[_request.cur_serv_index].infos.server_name);
}


void			Response::responseDelete() {
	
	//is method allowed
	std::ifstream ifs;

	std::string path_for_access;
	path_for_access = _request.location_path.root;
	if (_request.filename == "")
		path_for_access += _request.location_path.index;
	else
		path_for_access += _request.filename;
	std::cout << "REQUEST DELETE" << std::endl;
	std::cout << "path we try to delelte: " << path_for_access<< std::endl;
	ifs.open(path_for_access.c_str());

    if (!ifs) {
		_request.validity = 404;
		return auto_response();
	}
	ifs.close();
	
	if (std::remove(path_for_access.c_str())) {
		_request.validity =403;
		this->_header.setStatus("403 ", "Forbidden");
		return auto_response();
	}
	
	//creer un body pour dire que c delete ?
}



std::string		Response::readFromFile(std::string path, int i) {
	std::ifstream		ifs(path.c_str(), std::ifstream::binary);
	std::string			ret_buffer;
	std::string			tmp;

	if (i != 1) {
		return tmp;
	}
	if (!ifs)
	{
		std::cout << "This file doesn't exist." << std::endl;
		return ("");
	}
	while (!ifs.eof())
	{
		std::getline(ifs, tmp);
		ret_buffer += tmp;
		ret_buffer += "\n";
	}
	ifs.close();
	return ret_buffer;
}

std::string		Response::readFromFile(std::string path) {
	std::ifstream		ifs(path.c_str());
	std::string			ret_buffer;
	std::string			tmp;

	if (!ifs)
	{
		std::cout << "This file doesn't exist." << std::endl;
		return ("");
	}
	while (!ifs.eof())
	{
		std::getline(ifs, tmp);
		ret_buffer += tmp;
		ret_buffer += "\n";
	}
	ifs.close();
	return ret_buffer;
}

void			Response::setBody( std::string body ) {
	this->_body = body;
}
