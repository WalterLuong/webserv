#include <iostream>
#include <ctime>
#include "../includes/webserve.hpp"
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

/* How use it */
/* Generator of body for responce */
/* Give your error status as string */
/* Get Content-Lenght by use string.size() */


std::string generator_error_file(std::string error_status) {
	request mes;
	std::string ret;

	ret += "<!DOCTYPE html>\n<html>\n<head>\n<title>";
	ret += error_status;
	ret += " ";
	ret += mes.map_error[error_status];
	ret += "</title>\n</head>\n<body>\n<h1>Error ";
	ret += error_status;
	ret += "</h1>\n<p>Noppppppppppppppppp</p>\n<p>Cherche encore et peut etre tu trouveras l'amour</p>\n<p><strong>coeur</strong></p>\n</body>\n</html>";

	return ret;
}