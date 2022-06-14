/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodsHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdidier <jdidier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 23:45:35 by jdidier           #+#    #+#             */
/*   Updated: 2022/06/14 02:41:24 by jdidier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/MethodsHandler.hpp"

MethodsHandler::MethodsHandler() {
	this->_methods["GET"] = &exec_get;
	this->_methods["POST"] = &exec_post;
	this->_methods["DELETE"] = &exec_delete;
}

MethodsHandler::MethodsHandler(MethodsHandler const& src) {
	*this = src;
} 

MethodsHandler::~MethodsHandler() {}

MethodsHandler	&MethodsHandler::operator=(MethodsHandler const& rhs) {
	if (this != &rhs) {

	}
	return *this;
}

MethodsHandler::body		MethodsHandler::execute(void) {
	// CHECK METHOD :
	MethodsHandler::body	error; 
	std::string		m; //method value from parsing
	if (METHODS_LIST[m])
		return this->_methods[m]();
	else
		return error;
}

MethodsHandler::body		MethodsHandler::exec_get(void) {
	
}

MethodsHandler::body		MethodsHandler::exec_post(void) {
	
}

MethodsHandler::body		MethodsHandler::exec_delete(void) {
	
}

std::map<std::string, bool>		METHODS_LIST = {
	{"GET", true},
	{"HEAD", false},
	{"POST", true},
	{"PUT", false},
	{"DELETE", true},
	{"CONNECT", false},
	{"OPTIONS", false},
	{"TRACE", false},
	{"PATCH", false},
};
