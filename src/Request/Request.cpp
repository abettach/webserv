#include "Request.hpp"
#include "../Cgi/cgi.hpp"
/*********************************Request*******************************/
std::string MYREQUEST =
"GET /favicon.ico HTTP/1.1\r\n\
Host: localhost:8800\r\n\
Connection: keep-alive\r\n\
sec-ch-ua: 'Not A;Brand';v='99', 'Chromium';v='96', 'Google Chrome';v='96'\r\n\
sec-ch-ua-mobile: ?0\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.55 Safari/537.36\r\n\
sec-ch-ua-platform: 'macOS'\r\n\
Accept: image/avif,image/webp,image/apng,image/svg+xml,image/,/*;q=0.8\r\n\
Sec-Fetch-Site: same-origin\r\n\
Sec-Fetch-Mode: no-cors \r\n\
Sec-Fetch-Dest: image\r\n\
Referer: http://localhost:8800/\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Accept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n";
/************************************************************************/


void	Request::printRequestInformation()
{
	std::cout << "\e[1;32mMthode:\e[1;36m " << this->Method << std::endl;
	std::cout << "\e[1;32mTarget:\e[1;36m " << this->target<<"\e[1;37m" << std::endl;
	std::cout << "\e[1;32mProtocol: \e[1;36m" << this->protocol <<"\e[1;37m"<< std::endl; 
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
	{
		if (!it->second.empty())
			std::cout << "\e[1;32m" << it->first << ":\e[1;36m " << it->second <<"\e[1;37m" << std::endl;
	}
	if (!this->queryUrl.empty())
		std::cout << "\e[1;32mQuery: \e[1;36m" << this->queryUrl << "\e[1;37m" << std::endl;
	if (!this->url.empty())
		std::cout << "\e[1;32mUrl: \e[1;36m" << this->url << "\e[1;37m" << std::endl;
	std::cout << this->body << std::endl;
}
Request::Request() : _status(200), target(""), url(""), queryUrl("")
{
	this->headers.clear();
}

void	Request::clear()
{
	this->target.clear();
	this->url.clear();
	this->headers.clear();
	this->queryUrl.clear();
}

Request::~Request()
{
	this->target.clear();
	this->url.clear();
}
int		Request::Request_start(std::string _Request)
{
	int ret = 0;
	this->request = _Request;
	if ((ret = this->request_line()) || (ret = this->request_headers())
		|| (ret = this->request_body()))
		{
			this->_status = ret;
			return ret;
		}
	std::cout << BYEL <<"+++++++++++++++++++++++++++++ Request ++++++++++++++++++++++++++++++++" << BWHT <<std::endl;
	printRequestInformation();
	std::cout << BYEL << "+++++++++++++++++++++++++++++ Request ++++++++++++++++++++++++++++++++"<< BWHT << std::endl;

	return 0;
}

int		Request::request_body()
{
	// std::cout << this->request << std::endl;
	if (this->headers["Content-Type"].find("boundary") != std::string::npos && this->request.find("filename=") == std::string::npos)
	{
		if (this->request.find("Content-Disposition") != std::string::npos)
		{
			this->request.erase(0, request.find("Content-Disposition") + 21);
			ContentDiposition = this->request.substr(0,request.find(";"));
			this->headers["Content-Disposition"] = ContentDiposition;
			this->request.erase(0,request.find(";") + 1);
		}
		if (this->request.find("name") != std::string::npos)
		{
			this->name = this->request.erase(0, request.find("=")).substr(2, request.find("\r\n") - 3);
			this->headers["name"] = this->name;
			this->request.erase(0, request.find("\r\n") + 4);
		}
		if (this->request[0] != '-')
		{
			this->value = this->request.substr(0, request.find("-"));
			this->headers["value"] = this->value;
		}

	}
	else if (this->headers["Content-Type"].find("boundary") != std::string::npos && this->request.find("filename=") != std::string::npos)
	{
		if (this->request.find("Content-Disposition") != std::string::npos)
		{
			this->request.erase(0, request.find("Content-Disposition") + 21);
			ContentDiposition = this->request.substr(0,request.find(";"));
			this->headers["Content-Disposition"] = ContentDiposition;
			this->request.erase(0,request.find(";") + 1);
			this->request.erase(0,request.find(";") + 2);
		}
		if (this->request.find("filename") != std::string::npos)
		{
			this->request.erase(0, request.find("\"") + 1);
			this->name = this->request.substr(0, request.find("\""));
			this->headers["name"] = this->name;
			this->request.erase(0, request.find("\r\n") + 2);
		}
		if (this->request.find("Content-Type") != std::string::npos)
		{
			this->request.erase(0, request.find(":") + 2);
			this->contentType = this->request.substr(0, request.find("\r\n"));
			this->headers["ContentType"] = this->contentType;
			this->request.erase(0, request.find("\r\n\r\n") + 4);
			std::cout << this->request << std::endl;
		}
		this->value = this->request.substr(0, request.find("-") - 2);
		this->headers["value"] = this->value;
		// std::cout <<
	}
	else
	{
		if (this->request.find("\n") == std::string::npos)
			this->body = this->request;
		else
		{
			while (this->request.length())
			{	
				std::string tmp = this->request.substr(0, this->request.find("\n"));
				if (tmp.back() == '\r')
					tmp.pop_back();
				this->body.append(tmp);
				this->request.erase(0, this->request.find("\n") + 1);	
			}
		}
	}
	std::cout << body  << std::endl;
	return EXIT_SUCCESS;
}

int		Request::request_line()
{
	std::string Method;
 	bool _m = false;

	if (request.find("\r\n") != std::string::npos)
	{
		std::string tmp = request.substr(0, request.find(' '));

		if (tmp == "GET" || tmp == "POST" || tmp == "DELETE")
		{
			this->Method = tmp;
			request.erase(0, this->Method.length() + 1);
		}
		else
			return NOT_IMPLEMENTED;
		if (request.find(' ') == 0)
			return BAD_REQUEST;
		tmp = request.substr(0, request.find(' '));
		if (tmp[0] != '/')
			return BAD_REQUEST;

		if (tmp.length() < REQUEST_URI_MAX_LENGTH)
		{
			this->target = tmp;
			if (this->target.find("?") != std::string::npos)
			{
				this->url = this->target.substr(0, this->target.find("?"));
				this->queryUrl = this->target.substr(this->target.find('?') + 1);
			}
			request.erase(0, this->target.length() + 1);
			if (!this->url.empty())
				this->target = this->url;
		}
		else
			return REQUEST_URI_TOO_LONG;
		
		if (request.find(' ') == 0)
			return BAD_REQUEST;
		
		size_t end = request.find("\r\n");
		tmp = request.substr(0, end);
		
		if (tmp == "HTTP/1.1")
		{
			this->protocol = tmp;
			request.erase(0, end + 2);
		}
		else
			return HTTP_VERSOIN_NOT_SUPPORTED;
	}
	return EXIT_SUCCESS;
}

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

int		Request::request_headers()
{
	 size_t header_end, header_dil;
	 std::string header;
	 std::string value;

	 while ((header_end = this->request.find("\r\n")) != std::string::npos)
	 {
		 std::string tmp = this->request.substr(0, header_end);
		 if (header_end == 0)
		 {
			 this->request.erase(0, 2);
			 break;
		 }
		 if ((header_dil = tmp.find(':', 0)) != std::string::npos)
		 {
			if (header_dil == 0 || tmp[header_dil - 1] == ' ')
			 	return BAD_REQUEST;
			header = tmp.substr(0, header_dil);
			value = tmp.substr(header_dil + 1, header_end - header_dil - 1);
			if (tmp == "Host" && this->headers.count(header))
				return BAD_REQUEST;
			if (header.length() > HEADER_MAX_LENGTH || value.length() > VALUE_MAX_LENGTH)
				return BAD_REQUEST;
			this->headers[header] = trim(value);
		 }
		 else
		 	return BAD_REQUEST;
		request.erase(0, header_end + 2);
		tmp.clear();
	 }
	 return EXIT_SUCCESS;
}

std::string &Request::getMethod()
{
	return this->Method;
}

void	Request::setTarget(std::string _target)
{
	this->target = _target;
}
std::string &Request::getTarget()
{
	if (this->target[0] != '/')
		this->target = "/" + this->target;
	return this->target;
}

std::string &Request::getProtocol()
{
	return this->protocol;
}

std::map<std::string, std::string> &Request::getHeaders()
{
	return this->headers;
}

std::string		Request::getQueryString()
{
	return (this->queryUrl);
}

std::string		Request::getReqValue(std::string _key)
{
	return (this->headers[_key]);
}

std::string		Request::getUrl()
{
	return this->url;
}

int		&Request::getPort()
{
	std::string tmp;
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
		if (it->first == "Host")
			tmp = it->second;
	if (tmp.find(":") != std::string::npos)
	{
		tmp.erase(0, tmp.find(":") + 1);
		this->port = atoi(tmp.c_str());
	}
	else
		this->port = 80;
	
	return this->port;
}