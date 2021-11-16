#include "sData.hpp"

sData::sData(/* args */)
{
}

void    sData::setPort(int port)
{
    this->ports.push_back(port);
}
std::vector<int>    sData::getPort()
{
    return this->ports;
}

void    sData::printServerData()
{
    std::cout << "ports = {";
    for (size_t i = 0; i < this->ports.size(); i++)
        std::cout << this->ports[i] << ", ";
    std::cout << "}" << std::endl;
    std::cout << "host = " << this->host << std::endl;
    std::cout << "server_name = " << this->server_name << std::endl;
    std::cout << "Client Max body size = " << this->client_max_body_size << std::endl;
    std::cout << "root = " << this->root_dir << std::endl;
    std::cout << "Error_pages :"<<std::endl;
    for (std::map<int,std::string>::iterator it = this->error_pages.begin(); it != this->error_pages.end(); it++)
        std::cout << "key = " << it->first << " value = "+ it->second << std::endl;
}

void    sData::setErrorPage(std::string &tmp, int &error_num)
{
    this->error_pages.insert({error_num, tmp});
}

void    sData::setRootDir(std::string &tmp)
{
    this->root_dir = tmp;
}

void    sData::setServerName(std::string &tmp)
{
    this->server_name = tmp;
}

void    sData::setHost(std::string &tmp)
{
    this->host = tmp;
}

void    sData::setClienBodySize(int &tmp)
{
    this->client_max_body_size = tmp;
}

void    sData::clear_all(sData &sv)
{
    std::cout << " im in clear " << std::endl;
    for(;ports.size();)
        this->ports.pop_back();
    this->ports.erase(ports.begin(),ports.end());
    this->ports.clear();
}
sData::~sData()
{
    std::cout << "Data Destractor called" << std::endl;
    ports.clear();
    error_pages.clear();
}