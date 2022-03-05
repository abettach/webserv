#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "../../headers.hpp"
#include "../Request/Request.hpp"

class Request;

class Response
{
private:
    Request _request;
    std::string _statusLine;
    std::map<std::string, std::string> _headers;
    std::string _httpVersion;
    int         _statusCode;
    std::string _reasonPhrase;

public:
    Response(/* args */);
    std::string     &getStatusLine();
    ~Response();
};


#endif