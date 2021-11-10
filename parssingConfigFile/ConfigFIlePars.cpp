#include "ConfigFilePars.hpp"

ConfigFilePars::ConfigFilePars()
{
    std::cout << "Default constractor called !" << std::endl;   
}

void    ConfigFilePars::Arguments_checker(int ac, char **av)
{
    if (av[1])
        this->file_name = av[1];
    else
        this->file_name = "config.conf";
     std::string extention = &this->file_name[static_cast<int>(this->file_name.find(".") + 1)];
    
    if(extention != FILE_EXTENTION)
    {
        std::cout << INVALIDE_FILE_NAME << std::endl;
        exit(1);
    }
}

void ConfigFilePars::ft_strtrim(std::string &str)
{
    for (int i = 0; (str.front() == ' ' || str.front() == '\t') && str[i]; i++)
        str.erase(0, 1);
    for (int i = str.length() - 1 ; (str.back() == ' ' || str.front() == '\t') && str[i]; i--)
        str.erase(str.length() - 1, 1);
}

void    ConfigFilePars::remove_comments(std::string &str, char c)
{
    int i = str.find(c);
    str.erase(i, str.length() - 1);
}

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
        if(tmp[0] == COMMENTV1 || tmp[0] == COMMENTV2)
            continue;
        this->file_content.push_back(tmp);
    }
    // _print(this->file_content, "vector");
}
ConfigFilePars::ConfigFilePars(int ac, char **av)
{
    this->Arguments_checker(ac, av);
    this->get_file_content();
    this->file_check();
}


ConfigFilePars::~ConfigFilePars()
{
}