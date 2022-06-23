#include <iostream>
#include <ctime>
#include "webserve.hpp"
using namespace std;
 
std::string get_date() {
 time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"%a, %d %b %G %T GMT",timeinfo);
  std::cout << (buffer) << std::endl;

	std::string ret(buffer);
  std::cout << ret << std::endl;
  return ret;
 }

 int main() {
	std::string test;

	test = get_date();
	std::cout << "confirmation:" << test<< std::endl;
 }