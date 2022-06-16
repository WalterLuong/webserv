#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "../includes/ResponseHeader.hpp"

int		main() {
	ResponseHeader	yo;
	yo.setContentLength("254");
	yo.setContentType("text/html");
	yo.setDate("Wed, 16 Jun 2022");
	yo.setServerName("test");
	yo.setStatus("200 OK");
	yo.setLastModified("Wed, 16 Jun 2022");
	yo.generateHeader();
	std::cout << yo.getHeader() << std::endl;
	yo.clearHeader();
	yo.generateHeader();
	std::cout << yo.getHeader() << std::endl;

}