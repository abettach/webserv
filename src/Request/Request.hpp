#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "../utils/headers.hpp"
#include "../utils/StatusCode.hpp"
#define REQUEST_URI_MAX_LENGTH 2084
#define HEADER_MAX_LENGTH 1000
#define VALUE_MAX_LENGTH 4000

class Request
{
private:
	std::string request;
	std::string Method;
	std::string target;
	std::string protocol;
	std::string queryUrl;
	std::string body;
	std::string url;
	int		_status;
	std::string ContentDiposition;
	std::string name;
	std::string value;
	int			port;
	std::string fileName;
	std::string contentType;
	std::map<std::string, std::string> headers;

public:
	Request(/* args */);
	~Request();
	int		Request_start(std::string);
	int		request_line();
	int		request_headers();
	void	printRequestInformation();
	std::string &getMethod();
	std::string &getTarget();
	std::string &getProtocol();
	int			&getPort();
	std::string		getQueryString();
	int		request_body();
	std::map<std::string, std::string> &getHeaders();
	std::string	autoindex_run(std::string rooted_path);
	std::string		getReqValue(std::string _key);
	std::string		getUrl();
	void	setTarget(std::string _target);
	void	clear();

};

#endif