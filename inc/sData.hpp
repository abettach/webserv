#ifndef SDATA_HPP
#define SDATA_HPP

#include "../inc/ConfigFilePars.hpp"
#include "../inc/location.hpp"

class loc;

class sData
{
    private:
        std::vector<int> ports;
        std::string host;
        std::string server_name;
        int client_max_body_size;
        std::map<int, std::string> error_pages;
        std::string root_dir;
    public:
        std::map<std::string, location> locat;
        // std::vector<location> locat;
        sData();
        ~sData();

        void    setPort(int port);
        std::vector<int>  getPort();
        void    printPortData();
        void    clear_all(sData &sv);
        void    setErrorPage(std::string &tmp, int &error_num);
        void    printErrorPagesData();
        void    setRootDir(std::string &tmp);
        void    printServerData();
        void    setServerName(std::string &tmp);
        void    setHost(std::string &tmp);
        void    setClienBodySize(int &tmp);
        std::map<int, std::string>   getErrorPage();
        std::string    getRootDir();
        std::string    getServerName();
        std::string    getHost();
        int            getClienBodySize();
        void    get_location_autoindex(std::string &, location &);
        void    get_location_index(std::string &, location &);
        void    get_allow_methods(std::string &, location &);
        void    get_fastcgi_pass(std::string &, location &);
        void    ft_strtrim(std::string &str);
        void    addLocation(std::map<std::string, location> &sv_loc);
        void    clearLocation(location &sv_loc);
        
};
#endif