#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "webserv.hpp"
// length
#define REQUEST_URI_MAX_LENGTH 2084
#define HEADER_MAX_LENGTH 1000
#define VALUE_MAX_LENGTH 4000


class Request
{

public:
	Request();
	~Request();

	int parse(std::string &);
	int parse_request_line();
	int parse_headers();
	int prebody();
	int parse_body();

private:
	std::string _buffer;
	std::string _method;
	std::string _target;
	std::string _query_string;
	std::string _protocol;
	std::string _req_body;
	std::map<std::string, std::string> _headers;

	int _body_offset;
	size_t _length;
};

#endif