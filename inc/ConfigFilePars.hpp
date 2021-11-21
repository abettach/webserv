#ifndef CONFIGFILEPARSE_HPP
#define CONFIGFILEPARSE_HPP
#include "../headers.hpp"

#define LOCATION_MAX_ELEMENT 4
#define SERVER_MAX_ELEMENT 6
#define FILE_EXTENTION "conf"
#define INVALIDE_FILE_NAME "Error: Invalide File Name"
#define COMMENTV1 '#'
#define COMMENTV2 ';'
#define CONFIG_FILE_PATH "config/"
#define SERVER "server"
#define LOCATION "location"
#define LISTEN "listen"
#define ERROR_PAGE "error_page"
#define SERVER_NAME "server_name"
#define CLIENT_BODY_SIZE "client_max_body_size"
#define HOST "host"
#define AUTOINDEX "autoindex"
#define INDEX "index"
#define ALLOW_METHODS "allow_methods"
#define FASTCGI_PASS "fastcgi_pass"
#define ROOT_DIR "root"
#define OPEN_BRACE "{"
#define CLOSE_BRACE "}"
#include "../inc/sData.hpp"
#include "../inc/location.hpp"
// #include "../inc/location.hpp"

class sData;
class location;

class ConfigFilePars
{
private:
        std::string file_name;
        std::vector<std::string> file_content;
        std::vector<int> servers_num;
        std::vector<int> servers_index;
        void	(ConfigFilePars::*pointer[SERVER_MAX_ELEMENT])(std::string &, sData &);
        void	(ConfigFilePars::*location_pointer[LOCATION_MAX_ELEMENT])(std::string &, location &);
public:
    std::vector<sData> server;
    ConfigFilePars(/* args */);
    ConfigFilePars(int ac, char **av);
    ~ConfigFilePars();

    void    Arguments_checker(int ac, char **av);
    void    get_file_content();
    void    ft_strtrim(std::string &str);
    void    remove_comments(std::string &str, char c);
    void    file_check();// check the inside of the server is valide ?

    std::vector<sData>   getServer();
    void    get_elements();
    void    add_server(sData &var);
    std::string    getFileName();
    std::vector<std::string> ft_split(std::string const &str, char c);
    void    get_servers_index();
    void    run_ports(std::string &tmp, sData &sv);
    void    run_error_pages(std::string &tmp, sData &sv);
    void    run_root_dir(std::string &tmp, sData &sv);
    void    run_server_name(std::string &tmp, sData &sv);
    void    run_host(std::string &tmp, sData &sv);
    void    run_body_size(std::string &tmp, sData &sv);
    void    run_location(int &start, int end, sData &sv);
    void    add_location(sData &var, location &sv_loc);

    void    locationAutoIndexRun(std::string &tmp, location &sv_loc);
    void    locationIndexRun(std::string &tmp, location &sv_loc);
    void    locationFastCgiPassRun(std::string &tmp, location &sv_loc);
    void    locationAllowMethodsRun(std::string &tmp, location &sv_loc);

    class FILE_NAME_EXEPTION : public std::exception
    {
        const char *what() const throw();
    };
    class ERROR_EXEPTION : public std::exception
    {
        const char *what() const throw();
    };

    //locait    this->location_pointer[0] = &ConfigFilePars::locationAutoIndexRun;

};

template <typename T>
void    _print(T &var, std::string type)
{
    if(type == "vector")
    {
        for (size_t i = 0; i < var.size(); i++)
            std::cout << var[i] << std::endl;
    }
    else if (type == "string")
    {
        for (size_t i = 0; i < var.size(); i++)
            std::cout << var[i];
        std::cout << std::endl;
    }
}


#endif//