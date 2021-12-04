#include "serverINFO.hpp"

serverINFO::serverINFO(/* args */)
{
    this->client_max_body_size = -1;
    this->server_name = "";
    this->host = "";
    this->root_dir = "";
}

void    serverINFO::addPorts(int port)
{
    this->ports.push_back(port);
}
std::vector<int>    serverINFO::getPorts()
{
    return this->ports;
}

void    serverINFO::setPort(int port)
{
    this->port = port;
}
int    serverINFO::getPort()
{
    return this->port;
}

void    serverINFO::setErrorPage(std::string &tmp, int &error_num)
{
    this->error_pages.insert(std::pair<int, std::string>(error_num, tmp));
}
std::map<int, std::string>    serverINFO::getErrorPage()
{
    return (this->error_pages);
}


void    serverINFO::setRootDir(std::string &tmp)
{
    this->root_dir = tmp;
}
std::string    serverINFO::getRootDir()
{
    return (this->root_dir);
}


void    serverINFO::setServerName(std::string &tmp)
{
    this->server_name = tmp;
}
std::string    serverINFO::getServerName()
{
    return (this->server_name);
}


void    serverINFO::setHost(std::string &tmp)
{
    this->host = tmp;
}
std::string    serverINFO::getHost()
{
    return (this->host);
}


void    serverINFO::setClienBodySize(int &tmp)
{
    this->client_max_body_size = tmp;
}
int    serverINFO::getClienBodySize()
{
    return (this->client_max_body_size);
}

//****************Location to the server*********************

void serverINFO::ft_strtrim(std::string &str)
{
    while (isspace(str.front()))
        str.erase(0, 1);
    while (isspace(str.back()))
        str.erase(str.length() - 1, 1);
}

void    serverINFO::clear_all()
{
    for(;ports.size();)
        this->ports.pop_back();
    this->ports.erase(ports.begin(),ports.end());
    this->ports.clear();
    this->host.erase(host.begin(), host.end());
    this->host.clear();
    this->server_name.erase(server_name.begin(), server_name.end());
    this->server_name.clear();
    this->client_max_body_size = -1;
    this->error_pages.erase(error_pages.begin(), error_pages.end());
    this->error_pages.clear();
    this->root_dir.erase(root_dir.begin(), root_dir.end());
    this->root_dir.clear();
    for (size_t i = 0; i < locat.size();i++)
        locat.clear();
}

serverINFO::~serverINFO()
{
    ports.clear();
    error_pages.clear();
}

void    serverINFO::addLocation(std::map<std::string, location> &sv_loc)
{
    std::map<std::string, location>::iterator it = sv_loc.begin();
    this->locat.insert(std::pair<std::string, location>(it->first, it->second));
}

void    serverINFO::clearLocation(location &sv_loc)
{
    sv_loc.clearAll();
}

void    serverINFO::printServerData()
{
    std::cout << "\e[1;31mports                = \e[1;32m";
    for (size_t i = 0; i < this->ports.size(); i++)
        std::cout << "|" << this->ports[i] << "|";
    std::cout << std::endl;
    std::cout << "\e[1;31mhost                 = \e[1;32m|" << this->host << "|" << std::endl;
    std::cout << "\e[1;31mserver_name          = \e[1;32m|" << this->server_name << "|" << std::endl;
    std::cout << "\e[1;31mClient Max body size = \e[1;32m|" << this->client_max_body_size << "|" << std::endl;
    std::cout << "\e[1;31mroot                 = \e[1;32m|" << this->root_dir << "|" << std::endl;
    std::cout << "\e[1;31mError_pages :"<<std::endl;
    for (std::map<int,std::string>::iterator it = this->error_pages.begin(); it != this->error_pages.end(); it++)
        std::cout << "\e[1;34mkey = \e[1;32m|" << it->first << "| \e[1;34mvalue = \e[1;32m|"+ it->second << "|" << std::endl;

    std::cout << "\e[1;33m///////////////////////LOACATION INFORMATION/////////////////\e[1;34m" << std::endl;
    std::string arr[5] = {"/", "php", "py", "upload"};
    size_t i = 0;
    for (; i < locat.size() ; i++)
    {
        std::cout << "\e[1;31mlocation type = |";
        std::cout << locat[arr[i]].getLocationExtention() << "|" << std::endl;
        std::cout << "\e[1;34mAutoIndex     = \e[1;32m|";
        std::cout << locat[arr[i]].getLocationAutoIndex() << "|" << std::endl;
        std::cout << "\e[1;34mIndex         = \e[1;32m|";
        std::cout << locat[arr[i]].getLocationIndex() << "|" << std::endl;
        std::cout << "\e[1;34mfastCgiPass   = \e[1;32m|";
        std::cout << locat[arr[i]].getLocationFastCgiPass() << "|" << std::endl;
        std::map<std::string , bool> test;
        test = locat[arr[i]].getLocationAllowedMethods();
        std::cout << "\e[1;34mGET           = \e[1;32m|" << test["GET"] << "|\e[1;34m, POST = \e[1;32m|" << test["POST"] << "|\e[1;34m, DELETE = \e[1;32m|" << test["DELETE"] << "|" << std::endl;
    }
}

void    serverINFO::printServerALLData()
{
    std::cout << "\e[1;31mports                = \e[1;32m";
    std::cout << "|" << this->port << "|";
    std::cout << std::endl;
    std::cout << "\e[1;31mhost                 = \e[1;32m|" << this->host << "|" << std::endl;
    std::cout << "\e[1;31mserver_name          = \e[1;32m|" << this->server_name << "|" << std::endl;
    std::cout << "\e[1;31mClient Max body size = \e[1;32m|" << this->client_max_body_size << "|" << std::endl;
    std::cout << "\e[1;31mroot                 = \e[1;32m|" << this->root_dir << "|" << std::endl;
    std::cout << "\e[1;31mError_pages :"<<std::endl;
    for (std::map<int,std::string>::iterator it = this->error_pages.begin(); it != this->error_pages.end(); it++)
        std::cout << "\e[1;34mkey = \e[1;32m|" << it->first << "| \e[1;34mvalue = \e[1;32m|"+ it->second << "|" << std::endl;

    std::cout << "\e[1;33m///////////////////////LOACATION INFORMATION/////////////////\e[1;34m" << std::endl;
    std::vector<std::string> types;
    for (std::map<std::string, location>::iterator it = locat.begin(); it != locat.end(); it++)
        types.push_back(it->first);
    for (size_t i = 0; i < types.size() ; i++)
    {
        std::cout << "\e[1;31mlocation type = |";
        std::cout << locat[types[i]].getLocationExtention() << "|" << std::endl;
        std::cout << "\e[1;34mAutoIndex     = \e[1;32m|";
        std::cout << locat[types[i]].getLocationAutoIndex() << "|" << std::endl;
        std::cout << "\e[1;34mIndex         = \e[1;32m|";
        std::cout << locat[types[i]].getLocationIndex() << "|" << std::endl;
        std::cout << "\e[1;34mfastCgiPass   = \e[1;32m|";
        std::cout << locat[types[i]].getLocationFastCgiPass() << "|" << std::endl;
        std::map<std::string , bool> test;
        test = locat[types[i]].getLocationAllowedMethods();
        std::cout << "\e[1;34mGET           = \e[1;32m|" << test["GET"] << "|\e[1;34m, POST = \e[1;32m|" << test["POST"] << "|\e[1;34m, DELETE = \e[1;32m|" << test["DELETE"] << "|" << std::endl;
    }
}