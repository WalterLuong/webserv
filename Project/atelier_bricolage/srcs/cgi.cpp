#include "../includes/Response.hpp"
#include "../includes/utils.hpp"




std::string randomdigits(int nb){

	std::stringstream ss;
	for(int i = 0; i < nb; i++){
		ss << rand()%10;
	}
	return ss.str();
}

std::string create_tmpfile(void)
{
	std::string tmpfname;
	do
	{
		tmpfname = "/tmp/webservtmp" + randomdigits(20);
	} while (access(tmpfname.c_str(), F_OK) == 0);
	std::ofstream ftocreate(tmpfname.c_str());
	ftocreate.close();
	return std::string(tmpfname);
}



std::string joinstr(std::string *strs, int n)
{

	std::string str;
	for (int i = 0; i < n; i++)
	{
		str += strs[i];
	}
	return str;
}

int run_bin(std::string bin, std::string infilename, std::string outfilename)
{
	std::string payload[] = {"cat ", infilename, " | ", bin, " > ", outfilename};
	int size = *(&payload + 1) - payload;

	std::string cmd = joinstr(payload, size);


	std::cout << "go run cgiL :" << cmd.c_str() << std::endl;
	return system(cmd.c_str());
}


int write_infile(std::string infile, std::string body)
{
	return system(("echo " + body + " > " + infile).c_str());
}

std::string get_file_content(std::string filename)
{
	std::ifstream ifs;
	ifs.open(filename.c_str(), std::ifstream::in);

	std::stringstream strStream;
	strStream << ifs.rdbuf();

	return strStream.str();
};

std::string cgi_execution(std::string bin, std::string body)
{

	std::string outfile = create_tmpfile();
	std::string infile = create_tmpfile();

	write_infile(infile, body);

	run_bin(bin, infile, outfile);

	std::string out = get_file_content(outfile);

	remove(infile.c_str());
	remove(outfile.c_str());

	return out;
}

std::string tostr(int k)
{
	std::stringstream ss;
	ss << k;
	std::string s;
	ss >> s;

	return s;
}

std::string get_sub_str(std::string str, size_t pos)
{
	return str.substr((pos != std::string::npos) ? pos : 0);
}

void set_env(request &req, std::string &absolutepath)
{

	/* putenv prend des string static attention */ 
	putenv((char *)("CONTENT_LENGTH=" + req.instruction["Content-Length"]).c_str());
	putenv((char *)("CONTENT_TYPE=" + req.instruction["Content-Type"]).c_str());

	putenv((char *)("GATEWAY_INTERFACE=CGI/1.1"));

	// static std::string pathinf = get_sub_str(req.path, req.path.find_last_of("/"));
	putenv((char *)("PATH_INFO=" + req.path).c_str());
	putenv((char *)("PATH_TRANSLATED=" + req.path).c_str());

	static std::string query ;
	query = (req.methods == "GET") ? req.body : "";
	putenv((char *)("QUERY_STRING=" + query).c_str());

	putenv((char *)("REDIRECT_STATUS=200"));

	putenv((char *)("REQUEST_METHOD=" + req.methods).c_str());
	putenv((char *)("REQUEST_URI=" + req.path).c_str());


	static std::string host =
		req.instruction["Host"].substr(0, req.instruction["Host"].find_first_of(":"));
	static std::string port =
		get_sub_str(req.instruction["Host"], req.instruction["Host"].find_first_of(":") + 1);

	putenv((char *)("SERVER_NAME=" + host).c_str());
	putenv((char *)("SERVER_PORT=" + port).c_str());

	putenv((char *)("SERVER_PROTOCOL=HTTP/1.1"));
	putenv((char *)("SERVER_SOFTWARE=Weebserv/1.0"));

	putenv((char *)("REMOTE_IDENT=" + req.instruction["Authorization"]).c_str());
	putenv((char *)("REMOTE_USER=" + req.instruction["Authorization"]).c_str());
	putenv((char *)("REMOTEaddr=" + port).c_str());
	if (req.instruction["Auth-Scheme"] != "")
		putenv((char *)("AUTH_TYPE=" + req.instruction["Authorization"]).c_str());


	static std::string fname;
	fname = "SCRIPT_FILENAME="+std::string(realpath(absolutepath.c_str(), NULL));
	std::cout << "FRAME: " << fname << std::endl;
	static std::string sname;
	sname = "SCRIPT_NAME="+std::string(realpath(absolutepath.c_str(), NULL));
	// static std::string fname = "SCRIPT_FILENAME="+ std::string(getenv("PWD")) + "/"+ absolutepath;
	putenv((char *)fname.c_str());
	putenv((char *)sname.c_str());

};

std::string		cgi_handler(request &req, std::string path_for_access, int extension_pos) {

	std::string bin = req.location_path.cgi_path[extension_pos].second;
	if (bin != "") {
		std::cout << "_RED" << path_for_access << "_NOR" << std::endl;
		set_env(req, path_for_access);
	//	path_for_access += ""; 
		return cgi_execution(bin, req.body);
	}
	std::cerr << "CGI not found" << std::endl;
	return "";


}

int			get_cgi_path_pos(std::string extension, std::vector<std::pair<std::string, std::string> > cgi_path)  {

	std::vector<std::pair<std::string, std::string> >::iterator res = cgi_path.begin();
	int i = 0;
	while (res != cgi_path.end()) {
		if (res->first == extension) {
			return i;
		}
		res++;
		i++;
	}
	std::cerr << "CGI not found" << std::endl;
	return -1;
}