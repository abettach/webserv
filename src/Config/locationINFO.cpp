#include "locationINFO.hpp"

void location::setLocationExtention(std::string _Locationtype)
{
    this->Locationtype = _Locationtype;
}
std::string location::getLocationExtention()
{
    return this->Locationtype;
}


void location::setLocationType(std::string _type)
{
    this->type = _type;
}
std::string location::getLocationType()
{
    return this->type;
}


void location::setLocationIndex(std::string _index)
{
    this->index = _index;

}
std::string location::getLocationIndex()
{
    return this->index;
}


void location::setLocationAutoIndex(bool _AutoIndex)
{
    this->autoindex = _AutoIndex;
}
bool location::getLocationAutoIndex()
{
    return this->autoindex;
}


void location::setLocationFastCgiPass(std::string _fastcgi_pass)
{
    this->fastcgi_pass = _fastcgi_pass;
}
std::string location::getLocationFastCgiPass()
{
    return this->fastcgi_pass;
}


void location::setLocationAllowedMethods(std::map<std::string, bool> _allowed_methode)
{
    this->allowed_methode = _allowed_methode;
}
std::map<std::string, bool> location::getLocationAllowedMethods()
{
    return this->allowed_methode;
}


void    location::clearAll()
{
    this->Locationtype.erase(0, strlen(this->Locationtype.c_str()));
    this->Locationtype.clear();
    this->autoindex = false;
    this->index.erase(0, strlen(this->index.c_str()));
    this->index.clear();
    this->fastcgi_pass.erase(0, strlen(this->fastcgi_pass.c_str()));
    this->fastcgi_pass.clear();
    this->allowed_methode.clear();
}