#ifndef SDATA_HPP
#define SDATA_HPP
#include "ConfigFilePars.hpp"

class sData
{
    private:
        std::vector<int> ports;
        std::string host;
        std::string server_name;
        int client_max_body_size;
        std::map<int, std::string> error_pages;
        std::string root_dir;
        // location var;
    public:
        sData(/* args */);
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


};

#endif