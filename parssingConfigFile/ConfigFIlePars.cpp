#include "ConfigFilePars.hpp"

ConfigFilePars::ConfigFilePars()
{
    std::cout << "Default constractor called !" << std::endl;   
}

void    ConfigFilePars::get_servers_index()
{
    for(size_t i = 0; i < file_content.size(); i++)
    {
        if (file_content[i].compare(SERVER) == 0 && file_content[i+1].compare("[") == 0)
            servers_index.push_back(i);
        else if (file_content[i].compare("]") == 0)
            servers_index.push_back(i);
    }
}

void    ConfigFilePars::clear_sv(sData &sv)
{
    sData new_sv;
    sv = new_sv;
}

void    ConfigFilePars::run_ports(std::string &tmp, sData &sv)
{
    tmp.erase(0,strlen(LISTEN));
    ft_strtrim(tmp);
    sv.setPort(std::stoi(tmp));
    tmp.erase(0,tmp.length());
}
void    ConfigFilePars::run_error_pages(std::string &tmp, sData &sv)
{
    tmp.erase(0,strlen(ERROR_PAGE));
    ft_strtrim(tmp);
    int error_num = std::stoi(tmp);
    std::string num = std::to_string(std::stoi(tmp));
    tmp.erase(0,strlen(num.c_str()));
    ft_strtrim(tmp);
    sv.setErrorPage(tmp, error_num);
}
void    ConfigFilePars::run_root_dir(std::string &tmp, sData &sv)
{
    tmp.erase(0,strlen(ROOT_DIR));
    ft_strtrim(tmp);
    sv.setRootDir(tmp);
}
void    ConfigFilePars::run_server_name(std::string &tmp, sData &sv)
{
    tmp.erase(0,strlen(SERVER_NAME));
    ft_strtrim(tmp);
    sv.setServerName(tmp);
}
void    ConfigFilePars::run_host(std::string &tmp, sData &sv)
{
    tmp.erase(0,strlen(HOST));
    ft_strtrim(tmp);
    sv.setHost(tmp);
}
void    ConfigFilePars::run_body_size(std::string &tmp, sData &sv)
{
    tmp.erase(0,strlen(CLIENT_BODY_SIZE));
    ft_strtrim(tmp);
    int num = std::stoi(tmp);
    sv.setClienBodySize(num);
}

int     isValide(std::string tmp)
{
    int i = -1;
    return  (i = tmp == LISTEN ? 0 : tmp == HOST ? 1 : tmp == SERVER_NAME ? 2 :
    tmp == CLIENT_BODY_SIZE ? 3 : tmp == ERROR_PAGE ? 4 : tmp == ROOT_DIR ? 5 : -1);
}
//get elements of the config file elemet by element
void    ConfigFilePars::get_elements()
{
    int start, end;
    std::string arr[6] = {LISTEN, HOST, SERVER_NAME, CLIENT_BODY_SIZE, ERROR_PAGE, ROOT_DIR};
    this->pointer[0] = &ConfigFilePars::run_ports;
    this->pointer[1] = &ConfigFilePars::run_host;
    this->pointer[2] = &ConfigFilePars::run_server_name;
    this->pointer[3] = &ConfigFilePars::run_body_size;
    this->pointer[4] = &ConfigFilePars::run_error_pages;
    this->pointer[5] = &ConfigFilePars::run_root_dir;

    sData sv = sData();
    for (size_t i = 0;i < servers_index.size();)
    {
        start = servers_index[i] + 1;
        end = servers_index[i + 1];
        while (++start < end)
            for (size_t count = 0; count < 6; count++)
                if (this->file_content[start].find(arr[count]) != std::string::npos)
                {
                    (this->*pointer[count])(this->file_content[start], sv);
                    break;
                }
        this->add_server(sv);
        i+=2;
    }
    int i = 0;
    while (i < this->server.size())
    {
        std::cout << "*************************************" << std::endl; std::cout << "************Server Number [" << i << "]*************" << std::endl;
        this->server[i].printServerData(); std::cout << std::endl << "*************************************" << std::endl;
        i++;
    }
}

ConfigFilePars::ConfigFilePars(int ac, char **av)
{
    this->Arguments_checker(ac, av);
    this->get_file_content();
    this->file_check();
    this->get_servers_index();
    // this->get_all_servers(); // get the line when we start a server and the line of the end of every server we created
    this->get_elements();
}

void    ConfigFilePars::add_server(sData &var)
{
    this->server.push_back(var);
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

std::vector<std::string> ConfigFilePars::ft_split(std::string const &str, char c)
{
	std::vector<std::string> new_str;
	std::stringstream ss(str);
	std::string buff;

	while (getline(ss, buff, c))
		new_str.push_back(buff);
	return new_str;
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
        ft_strtrim(tmp);
        if (tmp.empty() || tmp == "\n")
            continue;
        if(tmp[0] == COMMENTV1 || isspace(tmp[0]))
            continue;
        if(tmp.find(COMMENTV1) != std::string::npos)
            remove_comments(tmp, COMMENTV1);
        ft_strtrim(tmp);
        this->file_content.push_back(tmp);
    }
    // _print(this->file_content, "vector");
}

ConfigFilePars::~ConfigFilePars()
{
}
//