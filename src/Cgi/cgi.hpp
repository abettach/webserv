#ifndef CGI_HPP
#define CGI_HPP
#include "../utils/headers.hpp"
#include "../Request/Request.hpp"

class Request;

class CGI
{
private:
public:
    CGI();
    std::string    runCGI(Request , std::string , std::string );
    ~CGI();
};



#endif