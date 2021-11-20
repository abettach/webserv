#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "sData.hpp"

class location
{
    private:
        std::string Locationtype;
        std::string type;
        std::string index;
        bool autoindex;
        std::string fastcgi_pass;
        std::map<std::string, bool> allowed_methode;
    public:
        void setLocationExtention(std::string _type);
        std::string getLocationExtention();
        void setLocationType(std::string _type);
        std::string getLocationType();
        std::string getLocationIndex();
        void setLocationIndex(std::string _index);
        void setLocationAutoIndex(bool _AutoIndex);
        bool getLocationAutoIndex();
        void setLocationFastCgiPass(std::string _fastcgi_pass);
        std::string getLocationFastCgiPass();
        void setLocationAllowedMethods(std::map<std::string, bool> _allowed_methode);
        std::map<std::string, bool> getLocationAllowedMethods();
        void    clearAll();
};


#endif