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
        std::cout<<"( (___   ( (__     ) (__) )  ( (    ) ) ( (__     ) (__) )" << std::endl;
        std::cout<<" \\___ \\   ) __)   (    __/    \\ \\  / /   ) __)   (    __/ " << std::endl;
        std::cout<<"     ) ) ( (       ) \\ \\  _    \\ \\/ /   ( (       ) \\ \\  _ " << std::endl;
        std::cout<<" ___/ /   \\ \\___  ( ( \\ \\_))    \\  /     \\ \\___  ( ( \\ \\_))" << std::endl;
        std::cout<<"/____/     \\____\\  )_) \\__/      \\/       \\____\\  )_) \\__/" << std::endl;
        std::cout<<"                            ["<<i<<"]                            " << std::endl;
        std::cout << std::endl;
        newServers[i].printServerALLData();
    }
}

int     main(int ac, char **av)
{
    if (!strcmp(av[1], "parssing"))
    {
        try
        {
            FileParss parss(ac, av);
            Response resp;
            //split all servers by port NB: use 'port' variable not 'ports' to get every server port
            std::vector<serverINFO> newServers = parss.SplitServers();
            // printServersINFO(newServers);
            Server server(newServers);
            // std::cout << _cgi.runCGI(request, root, cgi_path) << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else if (!strcmp(av[1], "request"))
    {
        std::string test = "NOne";
        Request request;
        request.Request_start(test);
        // request.printRequestInformation();
    }
    else if (!strcmp(av[1], "response"))
    {
        Response response;
    }
    else if (!strcmp(av[1], "CGI"))
    {
        Request request;
        CGI _cgi;
    }
    return 0;
}