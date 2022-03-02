#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "serverINFO.hpp"
class serverINFO;
class location
{
    friend class serverINFO;
    private:
        std::string Locationtype;
        std::string type;
        std::string index;
        bool autoindex;
        bool UploadEnable;
        std::string UploadStore;
        size_t  ReturnCode;
        std::string fastcgi_pass;
        std::map<std::string, bool> allowed_methode;
    public:
        // ~location();
        void		setLocationExtention(std::string _type);
        std::string getLocationExtention();
        void		setLocationType(std::string _type);
        std::string	getLocationType();
        std::string getLocationIndex();
        void 		setLocationIndex(std::string _index);
        void 		setLocationAutoIndex(bool _AutoIndex);
        bool 		getLocationAutoIndex();
        void 		setLocationFastCgiPass(std::string _fastcgi_pass);
        std::string getLocationFastCgiPass();
        void        setLocationUploadEnable(bool _UploadEnable);
        bool        getLocationUploadEnable();
        void        setLocationUploadStore(std::string _UploadStore);
        std::string        getLocationUploadStore();
        void        setLocationReturnCode(size_t    &);
        size_t      &getLocationReturnCode();

        void    	clearAll();
        void setLocationAllowedMethods(std::map<std::string, bool> _allowed_methode);
        std::map<std::string, bool> getLocationAllowedMethods();
};


#endif