#include "../inc/ConfigFilePars.hpp"

void    ConfigFilePars::get_servers_index()
{
    for(size_t i = 0; i < file_content.size(); i++)
    {
        if (file_content[i].compare(SERVER) == 0 && file_content[i+1].compare("[") == 0)
            servers_index.push_back(i);
        else if (file_content[i].compare("]") == 0)
            servers_index.push_back(i);
    }
    // std::cout << servers_index[0] << ", " << servers_index[1] << std::endl;
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

void    ConfigFilePars::locationAutoIndexRun(std::string &tmp, location &sv_loc)
{
    tmp.erase(0,strlen(AUTOINDEX));
    ft_strtrim(tmp);
    if (tmp.compare("on") == 1)
        sv_loc.setLocationAutoIndex(true);
    else
        sv_loc.setLocationAutoIndex(false);
}
void    ConfigFilePars::locationIndexRun(std::string &tmp, location &sv_loc)
{
    tmp.erase(0, strlen(INDEX));
    ft_strtrim(tmp);
    sv_loc.setLocationIndex(tmp);
}
void    ConfigFilePars::locationFastCgiPassRun(std::string &tmp, location &sv_loc)
{
    tmp.erase(0, strlen(FASTCGI_PASS));
    ft_strtrim(tmp);
    sv_loc.setLocationFastCgiPass(tmp);
}
void    ConfigFilePars::locationAllowMethodsRun(std::string &tmp, location &sv_loc)
{
    std::map<std::string , bool> methodes;
    tmp.erase(0, strlen(ALLOW_METHODS));
    ft_strtrim(tmp);
    if (tmp.find("GET") != std::string::npos)
    {
        methodes.insert(std::pair<std::string, bool>("GET", true));
        methodes["GET"] = true;
    }
    if (tmp.find("POST") != std::string::npos)
        methodes.insert(std::pair<std::string, bool>("POST", true));
    if (tmp.find("DELETE") != std::string::npos)
        methodes.insert(std::pair<std::string, bool>("DELETE", true));
    sv_loc.setLocationAllowedMethods(methodes);
}

location    ConfigFilePars::do_location(int &start, int &end)
{
    location sv_loc = location();
    std::string arr[4] = {AUTOINDEX, INDEX, FASTCGI_PASS, ALLOW_METHODS};
    this->location_pointer[0] = &ConfigFilePars::locationAutoIndexRun;
    this->location_pointer[1] = &ConfigFilePars::locationIndexRun;
    this->location_pointer[2] = &ConfigFilePars::locationFastCgiPassRun;
    std::string tmp_type = this->file_content[start];
    tmp_type.erase(0, strlen(LOCATION));
    ft_strtrim(tmp_type);
    sv_loc.setLocationExtention(tmp_type);
    for (size_t i = start + 1; i < end; i++)
        for (size_t count = 0; count < LOCATION_MAX_ELEMENT ; count++)
            if(this->file_content[i].find(arr[count])!= std::string::npos)
            {
                (this->*location_pointer[count])(this->file_content[i],sv_loc);
                break;
            }
    return sv_loc;
}

void    ConfigFilePars::getTypeExtention(std::string &tmp)
{
    int size = 0;
    while (tmp[size] != '.' && tmp[size])
        size++;
    if (tmp[size] == '.')
        tmp.erase(0, ++size);
}

void    ConfigFilePars::run_location(int &start, int end, sData &sv)
{

    this->location_pointer[3] = &ConfigFilePars::locationAllowMethodsRun;
    std::string tmp_type = this->file_content[start];
    tmp_type.erase(0, strlen(LOCATION));
    ft_strtrim(tmp_type);
    getTypeExtention(tmp_type);
    std::cout << tmp_type << std::endl;
    std::map<std::string, location> sv_loc;
    std::map<std::string, location>::iterator it = sv_loc.begin();
    sv_loc.insert(std::pair<std::string, location>(tmp_type, do_location(start, end)));
    sv.addLocation(sv_loc);
}

std::vector<sData> ConfigFilePars::getServer()
{
    return this->server;
}

void    ConfigFilePars::get_elements()
{
    int start, end;
    bool isNotLocation = false;
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
        {
            for (size_t count = 0; count < 6; count++)
            {
                isNotLocation = false;
                if (this->file_content[start].find(arr[count]) != std::string::npos)
                {
                    isNotLocation = true;
                    (this->*pointer[count])(this->file_content[start], sv);
                    break;
                }
            }
            if (!isNotLocation)
            {
                isNotLocation = true;
                if (this->file_content[start + 1] != OPEN_BRACE)
                    std::cout << "ERROR: location probleme" << std::endl;
                else
                {
                    int locationEND = start;
                    while (this->file_content[locationEND].find(CLOSE_BRACE) == std::string::npos)
                        locationEND++;
                    this->run_location(start, locationEND, sv);
                    start = locationEND;
                    locationEND = 0;
                }
            }
        }
        this->add_server(sv);
        sv.clear_all(sv);
        i+=2;
    }
    int i = 0;
    while (i < this->server.size())
    {
        std::cout<<"\e[4;36m  _____    _____   ______     __    __    _____   ______" << std::endl;
        std::cout<<" / ____\\  / ___/  (   __ \\    ) )  ( (   / ___/  (   __ \\" << std::endl;
        std::cout<<"( (___   ( (__     ) (__) )  ( (    ) ) ( (__     ) (__) )" << std::endl;
        std::cout<<" \\___ \\   ) __)   (    __/    \\ \\  / /   ) __)   (    __/ " << std::endl;
        std::cout<<"     ) ) ( (       ) \\ \\  _    \\ \\/ /   ( (       ) \\ \\  _ " << std::endl;
        std::cout<<" ___/ /   \\ \\___  ( ( \\ \\_))    \\  /     \\ \\___  ( ( \\ \\_))" << std::endl;
        std::cout<<"/____/     \\____\\  )_) \\__/      \\/       \\____\\  )_) \\__/" << std::endl;
        std::cout<<"                            ["<<i<<"]                            " << std::endl;
        std::cout << std::endl;
        this->server[i].printServerData();
        i++;
    }
}

std::string ConfigFilePars::getFileName()
{
    return this->file_name;
}

ConfigFilePars::ConfigFilePars(int ac, char **av)
{
    this->Arguments_checker(ac, av);
    this->get_file_content();
    this->file_check();
    this->get_servers_index();
    this->get_elements();
}

// void    ConfigFilePars::add_location(sData &var, location &sv_loc)
// {
//     var.addLocation(sv_loc);
// }

void    ConfigFilePars::add_server(sData &var)
{
    this->server.push_back(var);
}


//check the validiti of argument normale usage [./webserv config.conf]
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
    std::string new_string;
    while(std::getline(file, tmp))
    {
        ft_strtrim(tmp);
        if (tmp.empty() || tmp == "\n")
            continue;
        if(tmp[0] == COMMENTV1 || isspace(tmp[0]))
            continue;
        if(tmp.find(COMMENTV1) != std::string::npos)
            remove_comments(tmp, COMMENTV1);
        if (tmp.find(COMMENTV2) != std::string::npos)
            remove_comments(tmp, COMMENTV2);
        ft_strtrim(tmp);
        this->file_content.push_back(tmp);
    }
    // _print(this->file_content, "vector");
}

ConfigFilePars::~ConfigFilePars(){}
//