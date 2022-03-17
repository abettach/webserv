#include "locationINFO.hpp"

void location::setLocationExtention(std::string _Locationtype)
{
    this->Locationtype = _Locationtype;
}
std::string location::getLocationPath()
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


void location::setLocationAllowedMethods(std::map<std::string, bool> _allowed_Method)
{
    this->allowed_Method = _allowed_Method;
}
std::map<std::string, bool> location::getLocationAllowedMethods()
{
    return this->allowed_Method;
}

void location::setLocationUploadEnable(bool _UploadEnable)
{
    this->UploadEnable = _UploadEnable;
}
bool location::getLocationUploadEnable()
{
    return this->UploadEnable;
}

void location::setLocationUploadStore(std::string _UploadStore)
{
    this->UploadStore = _UploadStore;
}
std::string location::getLocationUploadStore()
{
    return this->UploadStore;
}

void    location::setLocationReturnCode(size_t &_returnCode)
{
    this->ReturnCode = _returnCode;
}
size_t  &location::getLocationReturnCode()
{
    return this->ReturnCode;
}

void    location::setLocationReturnPath(std::string &path)
{
    this->returnPath = path;
}
std::string     location::getLocationReturnPath()
{
    return this->returnPath;
}

std::string     location::getLocationExtention()
{
    std::string path = this->getLocationPath();
    if (path.find(".") != std::string::npos)
        return (path.substr(path.find(".") + 1, path.length()));
    return path;
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
    this->allowed_Method.clear();
}