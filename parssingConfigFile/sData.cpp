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
sData::~sData()
{
}