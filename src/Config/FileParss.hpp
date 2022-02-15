#ifndef FileParssE_HPP
#define FileParssE_HPP
#include "../../headers.hpp"

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
#define OPEN_BRACKET "["
#define CLOSE_BRACKET "]"
#include "serverINFO.hpp"
#include "locationINFO.hpp"

class serverINFO;
class location;

class FileParss
{
    private:
        std::string file_name;
        std::vector<std::string> file_content;
        std::vector<int> servers_index;
        void	(FileParss::*pointer[SERVER_MAX_ELEMENT])(std::string &, serverINFO &);
        void	(FileParss::*location_pointer[LOCATION_MAX_ELEMENT])(std::string &, location &);
    public:
        std::vector<serverINFO> server;
        FileParss(int ac, char **av);
        ~FileParss();

        void    Arguments_checker(int ac, char **av);
        void    get_file_content();
        void    ft_strtrim(std::string &);
        void    remove_comments(std::string &, char );
        void    file_check();// check the inside of the server is valide ?

        std::vector<serverINFO>   getServer();
        void    get_elements();
        void    add_server(serverINFO &var);
        std::string    getFileName();
        std::vector<std::string> ft_split(std::string const &, char);
        void    get_servers_index();
        void    run_ports(std::string &, serverINFO &);
        void    run_error_pages(std::string &, serverINFO &);
        void    run_root_dir(std::string &, serverINFO &);
        void    run_server_name(std::string &, serverINFO &);
        void    run_host(std::string &, serverINFO &);
        void    run_body_size(std::string &, serverINFO &);
        void    run_location(int &, int , serverINFO &);
        void    add_location(serverINFO &, location &);

        void    locationAutoIndexRun(std::string &, location &);
        void    locationIndexRun(std::string &, location &);
        void    locationFastCgiPassRun(std::string &, location &);
        void    locationAllowMethodsRun(std::string &, location &);
        location    getlocationInfo(int &, int &);
        void    getTypeExtention(std::string &);
        std::vector<serverINFO>    SplitServers();
        void    init_pointer(bool);
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