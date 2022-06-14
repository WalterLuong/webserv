/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodsHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdidier <jdidier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 23:45:35 by jdidier           #+#    #+#             */
/*   Updated: 2022/06/14 02:02:45 by jdidier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/MethodsHandler.hpp"

MethodsHandler::MethodsHandler() {}

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
	std::string		method;
	return(METHODS_LIST[method]());

}

MethodsHandler::body		MethodsHandler::exec_get(void) {
	
}

MethodsHandler::body		MethodsHandler::exec_post(void) {
	
}

MethodsHandler::body		MethodsHandler::exec_delete(void) {
	
}
std::map<std::string, MethodsHandler::body(*)()>		METHODS_LIST = {
	{"GET", &MethodsHandler::exec_get},
	{"POST", &MethodsHandler::exec_post},
	{"DELETE", &MethodsHandler::exec_delete},
};

/*
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
*/