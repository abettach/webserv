#include "../inc/sData.hpp"

sData::sData(/* args */)
{
    this->client_max_body_size = -1;
    this->server_name = "";
    this->host = "";
    this->root_dir = "";
}

void    sData::setPort(int port)
{
    this->ports.push_back(port);
}
std::vector<int>    sData::getPort()
{
    return this->ports;
}


void    sData::setErrorPage(std::string &tmp, int &error_num)
{
    this->error_pages.insert(std::pair<int, std::string>(error_num, tmp));
}
std::map<int, std::string>    sData::getErrorPage()
{
    return (this->error_pages);
}


void    sData::setRootDir(std::string &tmp)
{
    this->root_dir = tmp;
}
std::string    sData::getRootDir()
{
    return (this->root_dir);
}


void    sData::setServerName(std::string &tmp)
{
    this->server_name = tmp;
}
std::string    sData::getServerName()
{
    return (this->server_name);
}


void    sData::setHost(std::string &tmp)
{
    this->host = tmp;
}
std::string    sData::getHost()
{
    return (this->host);
}


void    sData::setClienBodySize(int &tmp)
{
    this->client_max_body_size = tmp;
}
int    sData::getClienBodySize()
{
    return (this->client_max_body_size);
}

//****************Location to the server*********************

void sData::ft_strtrim(std::string &str)
{
    while (isspace(str.front()))
        str.erase(0, 1);
    while (isspace(str.back()))
        str.erase(str.length() - 1, 1);
}

void    sData::clear_all(sData &sv)
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

sData::~sData()
{
    ports.clear();
    error_pages.clear();
}

void    sData::addLocation(location &sv_loc)
{
    this->locat.push_back(sv_loc);
}

void    sData::clearLocation(location &sv_loc)
{
    sv_loc.clearAll();
}

void    sData::printServerData()
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
    for (size_t i = 0; i < locat.size(); i++)
    {
        std::cout << "\e[1;31mlocation type = |";
        std::cout << locat[i].getLocationExtention() << "|" << std::endl;
        std::cout << "\e[1;34mAutoIndex     = \e[1;32m|";
        std::cout << locat[i].getLocationAutoIndex() << "|" << std::endl;
        std::cout << "\e[1;34mIndex         = \e[1;32m|";
        std::cout << locat[i].getLocationIndex() << "|" << std::endl;
        std::cout << "\e[1;34mfastCgiPass   = \e[1;32m|";
        std::cout << locat[i].getLocationFastCgiPass() << "|" << std::endl;
        std::map<std::string , bool> test;
        test = locat[i].getLocationAllowedMethods();
        std::cout << "\e[1;34mGET           = \e[1;32m|" << test["GET"] << "|\e[1;34m, POST = \e[1;32m|" << test["POST"] << "|\e[1;34m, DELETE = \e[1;32m|" << test["DELETE"] << "|" << std::endl;
    }
}