#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "../../headers.hpp"
#include "StatusCode.hpp"
#define REQUEST_URI_MAX_LENGTH 2084
#define HEADER_MAX_LENGTH 1000
#define VALUE_MAX_LENGTH 4000

class Request
{
private:
	std::string request;
	std::string methode;
	std::string target;
	std::string protocol;
	int			port;
	std::map<std::string, std::string> headers;

public:
	Request(/* args */);
	~Request();
	int		Request_start(std::string);
	int		request_line();
	int		request_headers();
	void	printRequestInformation();
	std::string &getMethode();
	std::string &getTarget();
	std::string &getProtocol();
	int			&getPort();
	std::map<std::string, std::string> &getHeaders();
};

#endif