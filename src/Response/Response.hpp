#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "../../headers.hpp"
#include "../Request/Request.hpp"

class Request;

class Response
{
private:
    /* data */
public:
    Response(/* args */);
    Response(Request request);
    ~Response();
};


#endif