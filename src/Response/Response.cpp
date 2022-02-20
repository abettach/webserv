#include "Response.hpp"

Response::Response(/* args */)
{
    std::cout << "Default constractor called" << std::endl;
} 

Response::Response(Request request)
{
    std::cout << "Parametrsie constractor called" << std::endl;
}

Response::~Response()
{
    std::cout << "Default Destractor called " << std::endl;
}