#ifndef CONFIGFILEPARSE_HPP
#define CONFIGFILEPARSE_HPP
#include "../headers.hpp"

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
#define ROOT_DIR "root"
#define OPEN_BRACE "{"
#define CLOSE_BRACE "}"
#include "sData.hpp"

class sData;

class ConfigFilePars
{
private:
        std::vector<sData> server;
        std::string file_name;
        std::vector<std::string> file_content;
        std::vector<int> servers_num;
        std::vector<int> servers_index;
public:
    ConfigFilePars(/* args */);
    ConfigFilePars(int ac, char **av);
    ~ConfigFilePars();

    void    Arguments_checker(int ac, char **av);
    void    get_file_content();
    void    ft_strtrim(std::string &str);
    void    remove_comments(std::string &str, char c);
    void    file_check();// check the inside of the server is valide ?

    void    get_elements();
    void    get_ports();
    void    get_server_name();
    void    get_error_pages();
    void    get_root_path();
    void    get_servers_start();
    void    add_server(sData &var);
    std::vector<std::string> ft_split(std::string const &str, char c);
    void    get_servers_index();
    void    clear_sv(sData &sv);


    class FILE_NAME_EXEPTION : public std::exception
    {
        const char *what() const throw();
    };
    class ERROR_EXEPTION : public std::exception
    {
        const char *what() const throw();
    };
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