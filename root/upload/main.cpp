// #include "src/HTTPServer/HTTPserver.hpp"
#include "src/Server/Server.hpp"
#include "src/Config/FileParss.hpp"
#include "src/Request/Request.hpp"
#include "src/Response/Response.hpp"
#include "src/Cgi/cgi.hpp"
void    printServersINFO(std::vector<serverINFO> newServers)
{
    for (size_t i = 0; i < newServers.size(); i++)
    {
        std::cout<<"\e[4;36m  _____    _____   ______     __    __    _____   ______" << std::endl;
        std::cout<<" / ____\\  / ___/  (   __ \\    ) )  ( (   / ___/  (   __ \\" << std::endl;
        std::cout<<"( (___  
