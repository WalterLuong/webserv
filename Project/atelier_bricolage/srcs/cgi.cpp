#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

// clang-format off
typedef std::vector<std::pair<std::string, std::string> > env_t;
// clang-format on

std::string randomdigits(int nb)
{

	std::stringstream ss;
	for (int i = 0; i < nb; i++)
	{
		ss << rand() % 10;
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

int run_bin(std::string bin, std::string argfile, std::string infilename, std::string outfilename)
{
	std::string payload[] = {"cat ", infilename, " | ", bin, " ", argfile, " > ", outfilename};
	int size = *(&payload + 1) - payload;

	std::string cmd = joinstr(payload, size);

	std::cout << "go run cgi :" << cmd.c_str() << std::endl;
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

std::string cgi_execution(std::string bin, std::string arg, std::string body)
{

	std::string outfile = create_tmpfile();
	std::string infile = create_tmpfile();

	write_infile(infile, body);

	run_bin(bin, arg, infile, outfile);

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

std::vector<std::string> init_env(request &req, std::string &absolutepath)
{
	static std::string query;
	query = (req.methods == "GET") ? req.body : "";

	static std::string host =
		req.instruction["Host"].substr(0, req.instruction["Host"].find_first_of(":"));
	static std::string port =
		get_sub_str(req.instruction["Host"], req.instruction["Host"].find_first_of(":") + 1);

	char *realp = realpath(absolutepath.c_str(), NULL);
	std::string realpath;
	if (!realp)
	{
		std::cerr << "Realpath error" << std::endl;
		realpath = "";
	}
	else
	{
		std::cerr << "Realpath : " << realp << std::endl;
		realpath = std::string(realp);
		free(realp);
	}

	std::string envs[] = {"CONTENT_LENGTH=" + req.instruction["Content-Length"],
						  "CONTENT_TYPE=" + req.instruction["Content-Type"],
						  "GATEWAY_INTERFACE=CGI/1.1",
						  "PATH_INFO=" + req.path,
						  "PATH_TRANSLATED=" + req.path,
						  "QUERY_STRING=" + query,
						  "REDIRECT_STATUS=200",
						  "REQUEST_METHOD=" + req.methods,
						  "REQUEST_URI=" + req.path,
						  "SERVER_NAME=" + host,
						  "SERVER_PORT=" + port,
						  "SERVER_PROTOCOL=HTTP/1.1",
						  "SERVER_SOFTWARE=Weebserv/1.0",
						  "REMOTE_IDENT=" + req.instruction["Authorization"],
						  "REMOTE_USER=" + req.instruction["Authorization"],
						  "REMOTEaddr=" + port,
						  "SCRIPT_FILENAME=" + realpath,
						  "SCRIPT_NAME=" + realpath};

	std::vector<std::string> ret;
	int size = *(&envs + 1) - envs;
	for (int i = 0; i < size; i++)
	{
		ret.push_back(envs[i]);
	}

	if (req.instruction["Auth-Scheme"] != "")
		ret.push_back("AUTH_TYPE=" + req.instruction["Authorization"]);
	return ret;
};

void set_env(std::vector<std::string> &envs)
{
	for (size_t i = 0; i < envs.size(); i++)
	{
		putenv((char *)envs[i].c_str());
	}
}

std::string get_argfile(void)
{
	std::string ret;
	char *sfilename = getenv("SCRIPT_FILENAME");
	(sfilename != NULL) ? ret = std::string(sfilename) : ret = "";
	return ret;
}

std::string cgi_handler(request &req, std::string path_for_access, int extension_pos)
{
	std::cerr << __FUNCTION__ << std::endl;
	std::string bin = req.location_path.cgi_path[extension_pos].second;
	if (bin != "")
	{
		std::vector<std::string> envs = init_env(req, path_for_access);
		set_env(envs);
		return cgi_execution(bin, get_argfile(), req.body);
	}
	std::cerr << "CGI not found" << std::endl;
	return "";
}

int get_cgi_path_pos(std::string extension, env_t cgi_path)
{
	env_t::iterator res = cgi_path.begin();
	int i = 0;
	while (res != cgi_path.end())
	{
		if (res->first == extension)
		{
			return i;
		}
		res++;
		i++;
	}
	std::cerr << "CGI not found" << std::endl;
	return -1;
}