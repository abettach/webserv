#include "FileParss.hpp"

FileParss::FileParss(int ac, char **av)
{
    this->Arguments_checker(ac, av);
    this->get_file_content();
    this->file_check();
    this->get_servers_index();
    this->get_elements();
}

std::string FileParss::getFileName()
{
    return this->file_name;
}
void    FileParss::add_server(serverINFO &var)
{
    this->server.push_back(var);
}

void    FileParss::get_servers_index()
{
    if (file_content[0] != SERVER)
        throw std::runtime_error("ERROR: Check Your Server Configuration File [EMPTY SEVER]");
    for(size_t i = 0; i < file_content.size(); i++)
    {
        if (file_content[i].compare(SERVER) == 0 && file_content[i+1].compare("[") == 0)
            servers_index.push_back(i);
        else if (file_content[i].compare("]") == 0)
            servers_index.push_back(i);
    }
}

void    FileParss::run_ports(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(LISTEN));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your Ports!");
    for (size_t i = 0; i < tmp.size(); i++)
        if (tmp[i] < '0' || tmp[i] > '9')
            throw std::runtime_error("Error: Check Your Ports!");
    sv.addPorts(std::stoi(tmp));
}

void    FileParss::run_error_pages(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(ERROR_PAGE));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your ERROR_PAGE!");
    int error_num = std::stoi(tmp); // stoi cpp11
    std::string num = std::to_string(std::stoi(tmp)); // t_string -42 cpp11
    tmp.erase(0,strlen(num.c_str()));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your ERROR_PAGE!");
    sv.setErrorPage(tmp, error_num);
}

void    FileParss::run_root_dir(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(ROOT_DIR));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your Root!");
    sv.setRootDir(tmp);
}
void    FileParss::run_server_name(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(SERVER_NAME));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your server_name!");
    sv.setServerName(tmp);
}

void    FileParss::run_host(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(HOST));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your HOST!");
    sv.setHost(tmp);
}

void    FileParss::run_body_size(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(CLIENT_BODY_SIZE));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your CLIENT_BODY_SIZE!");
    int num = std::stoi(tmp);
    sv.setClienBodySize(num);
}

void    FileParss::locationAutoIndexRun(std::string &tmp, location &sv_loc)
{
    tmp.erase(0,strlen(AUTOINDEX));
    ft_strtrim(tmp);
    if (tmp.compare("on") == 0)
        sv_loc.setLocationAutoIndex(true);
    else if (tmp.compare("off") == 0)
        sv_loc.setLocationAutoIndex(false);
    else
        throw std::runtime_error("Error: Location autoindex must be 'on' or 'off'!");
}
void    FileParss::locationIndexRun(std::string &tmp, location &sv_loc)
{
    tmp.erase(0, strlen(INDEX));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your Location Index!");
    sv_loc.setLocationIndex(tmp);
}

void    FileParss::locationFastCgiPassRun(std::string &tmp, location &sv_loc)
{
    tmp.erase(0, strlen(FASTCGI_PASS));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your Location FastCgiPass!");
    sv_loc.setLocationFastCgiPass(tmp);
}
void    FileParss::locationAllowMethodsRun(std::string &tmp, location &sv_loc)
{
    std::map<std::string , bool> methodes;
    tmp.erase(0, strlen(ALLOW_METHODS));
    ft_strtrim(tmp);
    if (tmp.find("GET") != std::string::npos)
        methodes.insert(std::pair<std::string, bool>("GET", true));
    if (tmp.find("POST") != std::string::npos)
        methodes.insert(std::pair<std::string, bool>("POST", true));
    if (tmp.find("DELETE") != std::string::npos)
        methodes.insert(std::pair<std::string, bool>("DELETE", true));
    sv_loc.setLocationAllowedMethods(methodes);
}

void    FileParss::init_pointer(bool location)
{
    if (!location)
    {
        this->pointer[0] = &FileParss::run_ports;
        this->pointer[1] = &FileParss::run_host;
        this->pointer[2] = &FileParss::run_server_name;
        this->pointer[3] = &FileParss::run_body_size;
        this->pointer[4] = &FileParss::run_error_pages;
        this->pointer[5] = &FileParss::run_root_dir;
    }
    else
    {
        this->location_pointer[0] = &FileParss::locationAutoIndexRun;
        this->location_pointer[1] = &FileParss::locationIndexRun;
        this->location_pointer[2] = &FileParss::locationFastCgiPassRun;
        this->location_pointer[3] = &FileParss::locationAllowMethodsRun;
    }
}

location    FileParss::getlocationInfo(int &start, int &end)
{
    location sv_loc = location();
    std::string arr[4] = {AUTOINDEX, INDEX, FASTCGI_PASS, ALLOW_METHODS};
    init_pointer(true);
    std::string tmp_type = this->file_content[start];
    tmp_type.erase(0, strlen(LOCATION));
    ft_strtrim(tmp_type);
    if (tmp_type.empty())
        throw std::runtime_error("Error: Check Your Location!");
    sv_loc.setLocationExtention(tmp_type);
    for (int i = start + 1; i < end; i++)
        for (size_t count = 0; count < LOCATION_MAX_ELEMENT ; count++)
            if(this->file_content[i].find(arr[count])!= std::string::npos)
            {
                (this->*location_pointer[count])(this->file_content[i],sv_loc);
                break;
            }
    return sv_loc;
}

void    FileParss::getTypeExtention(std::string &tmp)
{
    int size = 0;
    while (tmp[size] != '.' && tmp[size])
        size++;
    if (tmp[size] == '.')
        tmp.erase(0, ++size);
}

void    FileParss::run_location(int &start, int end, serverINFO &sv)
{
    std::string tmp_type = this->file_content[start];
    tmp_type.erase(0, strlen(LOCATION));
    ft_strtrim(tmp_type);
    getTypeExtention(tmp_type);
    std::map<std::string, location> sv_loc;
    sv_loc.insert(std::pair<std::string, location>(tmp_type, getlocationInfo(start, end)));
    sv.addLocation(sv_loc);
}

std::vector<serverINFO> FileParss::getServer()
{
    return this->server;
}

void    FileParss::get_elements()
{
    int start, end;
    bool isNotLocation = false;
    std::string arr[6] = {LISTEN, HOST, SERVER_NAME, CLIENT_BODY_SIZE, ERROR_PAGE, ROOT_DIR};
    init_pointer(false);
    serverINFO sv;
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
                    throw std::runtime_error("Error: Check your Location Braces ->line [" + this->file_content[start] + "]");
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
        sv.clear_all();
        i+=2;
    }
}

std::vector<serverINFO>    FileParss::SplitServers()
{
    std::vector<serverINFO> servers;

    for (size_t i = 0; i < this->server.size(); i++)
    {
        std::vector<int> _ports;
        _ports = server[i].getPorts();
        for (size_t j = 0; j < _ports.size(); j++)
        {
            serverINFO newServer;
            newServer = server[i];
            newServer.setPort(_ports[j]);
            servers.push_back(newServer);
        }
    }
    return servers;
}

//check the validiti of argument normale usage [./webserv config.conf]
void    FileParss::Arguments_checker(int ac, char **av)
{
    //for now i use only the config.conf because i need to use the first arg for other things ;)
    // if (ac > 2)
    // {
    //     throw std::runtime_error("Error: Check your Arguments!");
    // }
    // if (av[1])
    //     this->file_name = av[1];
    // else
        this->file_name = "conf/config.conf";
    std::string extention = &this->file_name[static_cast<int>(this->file_name.find(".") + 1)];
}

int    isspace(char c)
{
    return ((c == ' ' || c == '\t' || c == '\r' || c == '\n') ? 1 : 0);
}

std::vector<std::string> FileParss::ft_split(std::string const &str, char c)
{
	std::vector<std::string> new_str;
	std::stringstream ss(str);
	std::string buff;

    size_t i = 0;
	while (getline(ss, buff, c))
    {
        if (buff.find("location") != std::string::npos)
        {
            if ((i = buff.find("{")) != std::string::npos)
            {
                for (size_t j = 0; j < i; j++)
                {
                    std::string str;
                    str[j] = buff[j];
                }
                new_str.push_back(str);
                // str.erase(0, str.length());
                buff.erase(0, i);
            }
        }
		new_str.push_back(buff);
    }
	return new_str;
}

//remove spaces from the begining and the end of the file content
void FileParss::ft_strtrim(std::string &str)
{
    while (isspace(str.front()))
        str.erase(0, 1);
    while (isspace(str.back()))
        str.erase(str.length() - 1, 1);
}

//remove comments from the file
void    FileParss::remove_comments(std::string &str, char c)
{
    int i = str.find(c);
    str.erase(i, str.length() - 1);
}


//check the validiti of the file
void    FileParss::file_check()
{
    std::string tmp;
    int open_brace = 0;
    int open_bracket = 0;
    for (std::vector<std::string>::iterator it = file_content.begin(); it != file_content.end(); it++)
    {
        tmp = *it;
        open_bracket += std::count(tmp.begin(), tmp.end(), '[');
    }
    for (std::vector<std::string>::iterator it = file_content.begin(); it != file_content.end(); it++)
    {
        tmp = *it;
        open_bracket -= std::count(tmp.begin(), tmp.end(), ']');
    }
    for (std::vector<std::string>::iterator it = file_content.begin(); it != file_content.end(); it++)
    {
        tmp = *it;
        open_brace += std::count(tmp.begin(), tmp.end(), '{');
    }
    for (std::vector<std::string>::iterator it = file_content.begin(); it != file_content.end(); it++)
    {
        tmp = *it;
        open_brace -= std::count(tmp.begin(), tmp.end(), '}');
    }
    if (open_brace != 0 || open_bracket != 0)
        throw std::runtime_error("ERROR: Bad Config File content [Check your Brackets[] or Braces()]");
}

// void    FileParss::removeRline()
// {
// }
void    FileParss::get_file_content()
{
    std::ifstream file(this->file_name.c_str());
    std::string tmp;
    // std::string new_string = "";
    int i = 0;
    while(std::getline(file, tmp))
    {
        ft_strtrim(tmp); 
        if (tmp.empty() || tmp == "\n")
            continue;
        if(tmp[0] == COMMENTV1 || isspace(tmp[0]))
            continue;
        if(tmp.find(COMMENTV1) != std::string::npos)
            remove_comments(tmp, COMMENTV1);
        if(tmp.find(COMMENTV2) != std::string::npos)
            remove_comments(tmp, COMMENTV2);
        ft_strtrim(tmp);
        // new_string += tmp;
        file_content.push_back(tmp);
        i++;
    }
    // std::cout << new_string << std::endl;
    // this->file_content = this->ft_split(new_string, ';');
    // this->remove_Rline();
    // _print(file_content, "vector");
}

FileParss::~FileParss() {}