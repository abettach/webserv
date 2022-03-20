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
        if (!this->file_content[i].compare("[") && this->file_content[i - 1].find(SERVER) == std::string::npos)
            throw std::runtime_error("Error: Check You Config File!");
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
    sv.addPorts(atoi(tmp.c_str()));
}

void    FileParss::run_error_pages(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(ERROR_PAGE));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your ERROR_PAGE!");
    for (size_t i = 0; tmp[i] != ' '; i++)
        if (tmp[i] < '0' || tmp[i] > '9')
            throw std::runtime_error("Error: Check Your ERROR_PAGE!");
    int error_num = atoi(tmp.c_str()); // stoi cpp11
    std::string num = std::to_string(atoi(tmp.c_str())); // t_string -42 cpp11
    tmp.erase(0,strlen(num.c_str()));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your ERROR_PAGE!");
    for (size_t i = 0; i < tmp.size(); i++)
        if (tmp[i] == ' ' || tmp[i] == '\t')
            throw std::runtime_error("Error: Check Your ERROR_PAGE!");
    std::ifstream file(tmp.c_str());
    if (!file.is_open())
        throw std::runtime_error("Error: Error Page [" + tmp +"] Dosnt Exist!");
    sv.setErrorPage(tmp, error_num);
}

void    FileParss::run_root_dir(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(ROOT_DIR));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your Root!");
    for (size_t i = 0; i < tmp.size(); i++)
        if (tmp[i] == ' ' || tmp[i] == '\t')
            throw std::runtime_error("Error: Check Your Root!");
    std::ifstream file(tmp.c_str());
    // if (!file.is_open())
    //     throw std::runtime_error("Error: Error Page [" + tmp +"] Dosnt Exist!");
    sv.setRootDir(tmp);
}
void    FileParss::run_server_name(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(SERVER_NAME));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your server_name!");
    for (size_t i = 0; i < tmp.size(); i++)
        if (tmp[i] == ' ' || tmp[i] == '\t')
            throw std::runtime_error("Error: Check Your server_name!");
    sv.setServerName(tmp);
}

void    FileParss::run_host(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(HOST));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your HOST!");
    for (size_t i = 0; i < tmp.size(); i++)
        if (tmp[i] == ' ' || tmp[i] == '\t')
            throw std::runtime_error("Error: Check Your HOST!");

    sv.setHost(tmp);
}

void    FileParss::run_body_size(std::string &tmp, serverINFO &sv)
{
    tmp.erase(0,strlen(CLIENT_BODY_SIZE));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your CLIENT_BODY_SIZE!");
    for (size_t i = 0; i < tmp.size(); i++)
        if (tmp[i] == ' ' || tmp[i] == '\t')
            throw std::runtime_error("Error: Check Your CLIENT_BODY_SIZE!");
    for (size_t i =0; i < tmp.size() - 1 ;i++)
        if (tmp[i] < '0' || tmp[i] > '9')
            throw std::runtime_error("Error: Check Your CLIENT_BODY_SIZE!");
    int num = atoi(tmp.c_str());
    sv.setClienBodySize(num);
}

void    FileParss::locationAutoIndexRun(std::string &tmp, location &sv_loc)
{
    tmp.erase(0,strlen(AUTOINDEX));
    ft_strtrim(tmp);
    if (!tmp.compare("on"))
        sv_loc.setLocationAutoIndex(true);
    else if (!tmp.compare("off"))
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
    for (size_t i = 0; i < tmp.size(); i++)
        if (tmp[i] == ' ' || tmp[i] == '\t')
            throw std::runtime_error("Error: Check Your Location Index!");
    sv_loc.setLocationIndex(tmp);
}

void    FileParss::locationUploadEnable(std::string &tmp, location &sv_loc)
{
    tmp.erase(0,strlen(UPLOAD_ENABLE));
    ft_strtrim(tmp);
    if (!tmp.compare("on"))
        sv_loc.setLocationUploadEnable(true);
    else if (!tmp.compare("off"))
        sv_loc.setLocationUploadEnable(false);
    else if (sv_loc.getLocationPath().find("upload") != std::string::npos)
        throw std::runtime_error("Error: Location autoindex must be 'on' or 'off'!");
    else
        sv_loc.setLocationUploadEnable(false);
}

void    FileParss::locationUploadStore(std::string &tmp, location &sv_loc)
{
    tmp.erase(0,strlen(UPLOAD_STORE));
    ft_strtrim(tmp);
    if (tmp.empty() && sv_loc.getLocationPath().find("upload") != std::string::npos)
        throw std::runtime_error("Error: Check You Location uploade_store!");
    else
    {
        for (size_t i = 0; i < tmp.size(); i++)
            if (tmp[i] == ' ' || tmp[i] == '\t')
                throw std::runtime_error("Error: Check You Location uploade_store!");
        sv_loc.setLocationUploadStore(tmp);
    }
}

void    FileParss::locationReturn(std::string &tmp, location &sv_loc)
{
    size_t _return;
    tmp.erase(0,strlen(RETURN));
    ft_strtrim(tmp);
    if (tmp.empty() && sv_loc.getLocationPath().find("return") != std::string::npos)
        throw std::runtime_error("Error: Check You Location return!");
    else
    {
        _return = atoi(tmp.c_str());
        sv_loc.setLocationReturnCode(_return);

        tmp.erase(0, std::to_string(_return).size());
        ft_strtrim(tmp);
        sv_loc.setLocationReturnPath(tmp);
    }
}

void    FileParss::locationFastCgiPassRun(std::string &tmp, location &sv_loc)
{
    tmp.erase(0, strlen(FASTCGI_PASS));
    ft_strtrim(tmp);
    if (tmp.empty())
        throw std::runtime_error("Error: Check Your Location FastCgiPass!");
    for (size_t i = 0; i < tmp.size(); i++)
        if (tmp[i] == ' ' || tmp[i] == '\t')
            throw std::runtime_error("Error: Check Your Location FastCgiPass!");
    sv_loc.setLocationFastCgiPass(tmp);
}

bool    FileParss::isValideMethod(std::string tmp)
{
    tmp.pop_back();
    std::vector<std::string> Methods = ft_split(tmp, ',');
    for (size_t i = 0; i < Methods.size(); i++)
        if (Methods[i].compare("GET") && Methods[i].compare("POST") && Methods[i].compare("DELETE"))
            throw std::runtime_error("Error: Method Not Allowed [" + Methods[i] + "]");
    return true;
}

void    FileParss::locationAllowMethodsRun(std::string &tmp, location &sv_loc)
{
    std::map<std::string , bool> Methods;
    tmp.erase(0, strlen(ALLOW_METHODS));
    ft_strtrim(tmp);
    if (tmp[0] != '[' || !isValideMethod(&tmp[1]))
        throw std::runtime_error("Error: Check You Location Allow_Method!");
    if (tmp.find("GET") != std::string::npos)
        Methods.insert(std::pair<std::string, bool>("GET", true));
    if (tmp.find("POST") != std::string::npos)
        Methods.insert(std::pair<std::string, bool>("POST", true));
    if (tmp.find("DELETE") != std::string::npos)
        Methods.insert(std::pair<std::string, bool>("DELETE", true));
    sv_loc.setLocationAllowedMethods(Methods);
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
        this->location_pointer[4] = &FileParss::locationUploadEnable;
        this->location_pointer[5] = &FileParss::locationUploadStore;
        this->location_pointer[6] = &FileParss::locationReturn;
    }
}

bool        isNotValidLocation(std::string _content)
{
    if (_content.find(AUTOINDEX) == std::string::npos &&
    _content.find(INDEX) == std::string::npos &&
    _content.find(FASTCGI_PASS)== std::string::npos &&
    _content.find(ALLOW_METHODS)== std::string::npos &&
    _content.find(UPLOAD_ENABLE)== std::string::npos &&
    _content.find(UPLOAD_STORE)== std::string::npos &&
    _content.find(RETURN) == std::string::npos)
        return true;
    return false;
}

location    FileParss::getlocationInfo(int &start, int &end)
{
    location sv_loc = location();
    std::string arr[7] = {AUTOINDEX, INDEX, FASTCGI_PASS, ALLOW_METHODS, UPLOAD_ENABLE, UPLOAD_STORE, RETURN};
    init_pointer(true);
    std::string tmp_type = this->file_content[start];
    tmp_type.erase(0, strlen(LOCATION));
    ft_strtrim(tmp_type);
    if (tmp_type.empty())
        throw std::runtime_error("Error: Check Your Location!");
    sv_loc.setLocationExtention(tmp_type);
    for (int i = start + 2; i < end; i++)
        for (size_t count = 0; count < LOCATION_MAX_ELEMENT ; count++)
        {
            if (isNotValidLocation(this->file_content[i]))
                throw std::runtime_error("Error: Check Your Location '" + tmp_type + "'");
            if(this->file_content[i].find(arr[count])!= std::string::npos)
            {
                (this->*location_pointer[count])(this->file_content[i],sv_loc);
                break;
            }
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

bool    isNotValid(std::string _content)
{
    if (_content.find(LISTEN) == std::string::npos && 
    _content.find(HOST) == std::string::npos &&
    _content.find(SERVER_NAME) == std::string::npos &&
    _content.find(CLIENT_BODY_SIZE) == std::string::npos &&
    _content.find(ERROR_PAGE) == std::string::npos &&
    _content.find(ROOT_DIR) == std::string::npos &&
    _content.find(LOCATION) == std::string::npos)
        return true;
    return false;
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
                if (isNotValid(this->file_content[start]))
                    throw std::runtime_error("Error: Check You Config File `[" + this->file_content[start] + "]` Is Not Valide!");
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
		new_str.push_back(buff);
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
        throw std::runtime_error("Error: Check You Config File!");
}

void   FileParss::ft_clean(std::vector<std::string> str)
{
    for(size_t i = 0; i < str.size(); i++)
    {
        if (str[i].empty() || str[i] == "\n"  || isspace(str[i][0]))
            continue;
        this->file_content.push_back(str[i]);
    }
}

void    FileParss::get_file_content()
{
    std::ifstream file(this->file_name.c_str());
    std::string tmp;
    std::string new_string;
    int i = 0;
    while(std::getline(file, tmp))
    {
        ft_strtrim(tmp); 
        if (tmp.empty() || tmp == "\n") continue;
        if(tmp[0] == COMMENTV1 || isspace(tmp[0])) continue;
        if(tmp.find(COMMENTV1) != std::string::npos)
            remove_comments(tmp, COMMENTV1);
        ft_strtrim(tmp);
        if (tmp[tmp.size() - 1] != ';')
            tmp += ';';
        new_string += tmp;
        i++;
    }
    this->ft_clean(ft_split(new_string, ';'));
    // _print(file_content, "vector");
}

FileParss::~FileParss() {}