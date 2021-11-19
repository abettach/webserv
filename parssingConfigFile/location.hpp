#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "sData.hpp"

class location
{
    private:
        std::string type;
        std::string index;
        bool autoindex;
        std::string fastcgi_pass;
        std::map<std::string, int> allowed_methode;
    public:
};


#endif