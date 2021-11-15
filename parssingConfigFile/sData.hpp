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
};

#endif