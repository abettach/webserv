#ifndef CGI_HPP
#define CGI_HPP
#include "../../headers.hpp"
#include "../Request/Request.hpp"

class Request;

class CGI
{
private:
public:
    CGI(/* args */);
    std::string    runCGI(Request , std::string , std::string );
    ~CGI();
};



#endif