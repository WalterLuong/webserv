/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdidier <jdidier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:00:22 by jdidier           #+#    #+#             */
/*   Updated: 2022/05/31 14:32:20 by jdidier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cgi.hpp"

Cgi::Cgi(std::string const& script): _script(script) {
	this->_env["CONTENT_TYPE"] = "";
	this->_env["CONTENT_LENGTH"] = "";
	this->_env["HTTP_COOKIE"] = "";
	this->_env["HTTP_USER_AGENT"] = "";
	this->_env["PAth_INFO"] = "";
	this->_env["QUERY_STRING"] = "";
	this->_env["REMOTE_ADDR"] = "";
	this->_env["REMOTE_HOST"] = "";
	this->_env["REQUEST_METHOD"] = "";
	this->_env["SCRIPT_FILENAME"] = "";
	this->_env["SCRIPT_NAME"] = "";
	this->_env["SERVER_NAME"] = "";
	this->_env["SERVER_SOFTWARE"] = "";
}

Cgi::Cgi(Cgi const& src) {
	*this= src;
}

Cgi::~Cgi() {}

Cgi				&Cgi::operator=(Cgi const& rhs) {
	if (this != &rhs) {
		this->_env = rhs.getEnv();
	}
	return *this;
}

Cgi::Env				Cgi::getEnv(void) const {
	return this->_env;
}

std::string		Cgi::execute(void) {
	pid_t	pid;
	int		ret = 0;
	int		filedes[2];
	std::string body = "";

	if (pipe(filedes) < 0) {
		std::cout << "PIPE ERROR" << std::endl;
	}
	pid = fork();
	if (pid < 0) {
		std::cout << "ERROR" << std::endl;
	} else if (pid == 0) {
		while ((dup2(filedes[1], STDOUT_FILENO) == -1) /*&& (errno == EINTR)*/) {
			
		}
		close(filedes[1]);
		close(filedes[0]);
		char *prout[2];
		prout[0] = (char *)this->_script.c_str();
		prout[1] = NULL;
		ret = execve(this->_script.c_str(), prout, _getEnvAsCstrArray());
		if (ret < 0)
			std::cout << "ERROR" << std::endl;
	} else {
		close(filedes[1]);
		char buffer[4096];
		//7waitpid(pid, &status, 0);
		while (1) {
			int count = read(filedes[0], buffer, sizeof(buffer));
			if (count == -1) {
				std::cout << "ERROR" << std::endl;
			}
			body += buffer;
			if (count == 0)
				break;
		}
		close(filedes[0]);
		wait(0);
	}
	return body;
}

char					**Cgi::_getEnvAsCstrArray() const {
	char	**env = new char*[this->_env.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = this->_env.begin(); i != this->_env.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}