#include "ConfigFilePars.hpp"

ConfigFilePars::ConfigFilePars()
{
    std::cout << "Default constractor called !" << std::endl;   
}

void    ConfigFilePars::get_servers_start()
{
    int _start = 0;
    int _end = 0;
    bool BraceIsOpen = false;
    bool BraceIsclosed = false;
    // if(file_content[0].compare(SERVER) != 1)
    // {
    //     std::cout << "Error: Bad file" << std::endl;
    //     exit(0);
    // }
    // else
    // {
        sData var;
        for (size_t i = 0; i < file_content.size(); i++)
        {
            if (file_content[i].compare(SERVER) && file_content[i + 1].compare(OPEN_BRACE) == 1)
            {
                _start = i;
                BraceIsOpen = true;
            }
            else if (file_content[i].compare(SERVER) == 1 && file_content[i + 1].compare(OPEN_BRACE) != 1)
            {
                std::cout << "Error: bad file : line -> " << file_content[i] << std::endl;
                exit(0);
            }
            if (file_content[i].compare(LISTEN) == 1)
            {
                std::string tmp = file_content[i];

                size_t port = tmp.
                // var.setPort(std::stoi(&file_content[i][port]));
                std::cout << "port = " << port << ", " << &file_content[i][2] << std::endl;
            }
        }
    // }
    // std::vector<int> ports = var.getPort();
    // _print(ports, "vector");
}

ConfigFilePars::ConfigFilePars(int ac, char **av)
{
    this->Arguments_checker(ac, av);
    this->get_file_content();
    this->file_check();
    this->get_servers_start(); // get the line when we start a server and the line of the end of every server we created
    this->get_elements();
}

void    ConfigFilePars::add_server(sData &var)
{
    this->server.push_back(var);
}

//get elements of the config file elemet by element
void    ConfigFilePars::get_elements()
{
    sData var;

    this->add_server(var);
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
        if (tmp.empty())
            continue;
        if(tmp.find(COMMENTV1) != std::string::npos)
            remove_comments(tmp, COMMENTV1);
        ft_strtrim(tmp);
        if(tmp[0] == COMMENTV1 || tmp[0] == COMMENTV2 || isspace(tmp[0]))
            continue;
        this->file_content.push_back(tmp);
    }
    _print(this->file_content, "vector");
}

ConfigFilePars::~ConfigFilePars()
{
}
//