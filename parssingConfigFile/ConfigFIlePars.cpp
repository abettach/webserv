#include "ConfigFilePars.hpp"

ConfigFilePars::ConfigFilePars()
{
    std::cout << "Default constractor called !" << std::endl;   
}

ConfigFilePars::ConfigFilePars(int ac, char **av)
{
    this->Arguments_checker(ac, av);
    this->get_file_content();
    this->file_check();
    this->get_elements();
}

void    ConfigFilePars::get_ports()
{
    for (size_t i = 0; i < this->file_content.size() ; i++)
    {
        std::string tmp = this->file_content[i];
        if (!tmp.find("listen"))
        {
            tmp.erase(0, 7);
            ft_strtrim(tmp);
            this->ports.push_back(std::stoi(tmp));
        }
    }
    _print(this->ports, "vector");
}

void    ConfigFilePars::get_root_path()
{
    for (size_t i = 0; i < this->file_content.size() ; i++)
    {
        std::string tmp = this->file_content[i];
        if (!tmp.find("root"))
        {
            tmp.erase(0, 4);
            ft_strtrim(tmp);
            this->root_path = tmp;
        }
    }
    _print(this->root_path, "string");
}

void    ConfigFilePars::get_error_pages()
{
    for (size_t i = 0; i < this->file_content.size() ; i++)
    {
        std::string tmp = this->file_content[i];
        if (!tmp.find("error_page"))
        {
            tmp.erase(0, 10);
            ft_strtrim(tmp);
            int error_num = std::stoi(tmp);
            tmp.erase(0, 4);
            ft_strtrim(tmp);
            std::string error_path = tmp;
            this->error_page.insert(std::pair<int , std::string>(error_num, tmp));
        }
    }
    for (std::map<int , std::string>::iterator it = this->error_page.begin(); it != this->error_page.end(); ++it)
        std::cout << "key = " << it->first << ", value = " << it->second << std::endl;
}


void    ConfigFilePars::get_server_name()
{
    for (size_t i = 0; i < this->file_content.size() ; i++)
    {
        std::string tmp = this->file_content[i];
        if (!tmp.find("server_name"))
        {
            tmp.erase(0, 11);
            ft_strtrim(tmp);
            this->server_name = tmp;
        }
    }
    std::cout << this->server_name << std::endl;
}

//get elements of the config file elemet by element
void    ConfigFilePars::get_elements()
{
    std::cout << "Ports :" << std::endl;
    this->get_ports();
    std::cout << std::endl;
    std::cout << "Server_name :" << std::endl;
    this->get_server_name();
    std::cout << std::endl;
    std::cout << "error_pages :" << std::endl;
    this->get_error_pages();
    std::cout << std::endl;
    std::cout << "root :" << std::endl;
    // this->get_host();
    this->get_root_path();

}

//check the validiti of argument normale usage [./webserv ./config.conf]
void    ConfigFilePars::Arguments_checker(int ac, char **av)
{
    if (ac == 2)
    {
        if (av[1])
            this->file_name = av[1];
        else
            this->file_name = "config/config.conf";
        std::string extention = &this->file_name[static_cast<int>(this->file_name.find(".") + 1)];
    
        if(extention != FILE_EXTENTION)
        {
            std::cout << INVALIDE_FILE_NAME << std::endl;
            exit(1);
        }
    }
    else
    {
        std::cout << "Error: bad arguments" << std::endl;
        std::cout << "Usage : ./$(PROGRAMME_NAME) $(CONFIG_FILE)" << std::endl;
        exit(0);
    }
}

int    isspace(char c)
{
    return ((c == ' ' || c == '\t' || c == '\r' || c == '\n') ? 1 : 0);
}

//remove spaces from the begining and the end of the file content
void ConfigFilePars::ft_strtrim(std::string &str)
{
    while (isspace(str.front()))
        str.erase(0, 1);
    while (isspace(str.back()))
        str.erase(str.length() - 1, 1);
}

//remove comments from the file
void    ConfigFilePars::remove_comments(std::string &str, char c)
{
    int i = str.find(c);
    str.erase(i, str.length() - 1);
}

//check the validiti of the file
void    ConfigFilePars::file_check()
{
    std::string tmp;
    int open_brace = 0;
    int close_brace = 0;

    for (std::vector<std::string>::iterator it = file_content.begin(); it != file_content.end(); it++)
    {
        tmp = *it;
        open_brace += std::count(tmp.begin(), tmp.end(), '{');
    }
    for (std::vector<std::string>::iterator it = file_content.begin(); it != file_content.end(); it++)
    {
        tmp = *it;
        close_brace += std::count(tmp.begin(), tmp.end(), '}');
    }
    if (open_brace != close_brace)
        std::cout<<"Error: Bad Config File content" << std::endl;
}

void    ConfigFilePars::get_file_content()
{
    std::ifstream file(this->file_name.c_str());
    std::string tmp;

    while(std::getline(file, tmp))
    {
        if(tmp.find(COMMENTV1) != std::string::npos)
            remove_comments(tmp, COMMENTV1);
        if(tmp.find(COMMENTV2) != std::string::npos)
            remove_comments(tmp, COMMENTV2);
        ft_strtrim(tmp);
        if(tmp[0] == COMMENTV1 || tmp[0] == COMMENTV2 || isspace(tmp[0]))
            continue;
        this->file_content.push_back(tmp);
    }
    // _print(this->file_content, "vector");
}

ConfigFilePars::~ConfigFilePars()
{
}