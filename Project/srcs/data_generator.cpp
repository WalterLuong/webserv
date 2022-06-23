#include <iostream>
#include <ctime>
using namespace std;
 
int main( )
 {
 time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80,"Now it's %a, %d %b %G %T GMT",timeinfo);
  std::cout << (buffer) << std::endl;

  return 0;
 }