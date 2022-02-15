#include "webserv.hpp"

std::string ltrim(const std::string &s)
{
	size_t start = s.find_first_not_of(" ");
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
	size_t end = s.find_last_not_of(" ");
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s, std::string dil = " ")
{
	return rtrim(ltrim(s));
}

bool isValidMethod(std::string  &str)
{
	return (str == "GET" || str == "POST" || str == "DELETE");
}

Request::Request(){}

Request::~Request() {}

int Request::parse(std::string &buffer)
{
	size_t ret = 0;

	_buffer = buffer;

	if ((ret = parse_request_line()) || (ret = parse_headers()) || (ret = parse_body()))
		;
	return ret;
}

int Request::parse_request_line()
{
	// Request line must end with \r\n
	if (_buffer.find("\r\n") != std::string::npos)
	{

		// get the first word in the request line (METHOD)
		std::string tmp = _buffer.substr(0, _buffer.find(' '));

		if (isValidMethod(tmp))
		{
			_method = tmp;
			_buffer.erase(0, _method.length() + 1);
		}
		else
			return NOT_IMPLEMENTED;

		// it must be one space afet the (METHOD)
		if (_buffer.find(' ') == 0)
			return BAD_REQUEST;

		// get the seconde word in the request line (REQUEST_URI)
		tmp = _buffer.substr(0, _buffer.find(' '));

		if (tmp[0] != '/')
			return BAD_REQUEST;

		if (tmp.length() < REQUEST_URI_MAX_LENGTH)
		{
			_target = tmp;
			_buffer.erase(0, _target.length() + 1);
		}
		else
			return REQUEST_URI_TOO_LONG;

		// GET QUERIES
		if (_target.find('?') != std::string::npos)
		{
			_query_string = _target.substr(_target.find('?') + 1);
			_target.erase(_target.find('?'));
		}

		if (_buffer.find(' ') == 0)
			return BAD_REQUEST;

		// get the last word in the request line (PROTOCOL)
		size_t end = _buffer.find("\r\n");
		tmp = _buffer.substr(0, end);

		if (tmp == "HTTP/1.1")
		{
			_protocol = tmp;
			_buffer.erase(0, end + 2);
		}
		else
			return HTTP_VERSOIN_NOT_SUPPORTED;
	}
	return EXIT_SUCCESS;
}

int Request::parse_headers()
{
	size_t header_end, header_dil;
	std::string header;
	std::string value;

	// every header ends with "\r\n"
	while ((header_end = _buffer.find("\r\n")) != std::string::npos)
	{
		// the headers ends with an extra "\r\n"
		if (header_end == 0)
		{
			_buffer.erase(0, 2);
			break;
		}
		if ((header_dil = _buffer.find(':', 0)) != std::string::npos)
		{
			// if the header line starts or ends with ':' return BAD_REQUEST
			if (header_dil == 0 || _buffer[header_dil - 1] == ' ')
				return BAD_REQUEST;
			header = _buffer.substr(0, header_dil);
			value = _buffer.substr(header_dil + 1, header_end - header_dil - 1);

			// it must be only only one 'Host' header
			if (header == "Host" && _headers.count(header))
				return BAD_REQUEST;
			if (header.length() > HEADER_MAX_LENGTH || value.length() > VALUE_MAX_LENGTH)
				return BAD_REQUEST;
			_headers[header] = trim(value);
		}
		else
			return BAD_REQUEST;
		_buffer.erase(0, header_end + 2);
	}
	return EXIT_SUCCESS;
}

int Request::parse_body()
{
	if (_buffer.length() >= _length)
	{
		_req_body.insert(_body_offset, _buffer, 0, _length);
		_body_offset += _buffer.length();
		_buffer.clear();

		if (_req_body.length() == _length)
			return 1;
		else
			return BAD_REQUEST;
	}
	return EXIT_SUCCESS;
}

int Request::prebody()
{
	// only 'POST' and 'POST' request are allowed to hove body
	if (_method != "POST" && _method != "PUT")
		return EXIT_FAILURE;

	// one and only one 'Host' header is allowed
	if (_headers.find("Host") == _headers.end() || _headers["Host"].empty())
		return BAD_REQUEST;

	else if (_headers.find("Content-Length") != _headers.end())
	{
		// only digits is allowed in value of'Content-Length'
		if (_headers["Content-Length"].find_first_not_of("0123456789") != std::string::npos)
			return BAD_REQUEST;
	}
	else
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
